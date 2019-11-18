//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "D2DNavSite.h"
#include "D2DNavSiteDlg.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#ifdef __DDGRAPH
	void NavSiteStruct::CalcPix(DDGraph* pG)
	{
		PixPos=pG->LL2Pix(Pos);
		if (bDrawCircle)
			{
			XYPos XY1=pG->LL2XY(Pos);
			XYPos XY2=XY1;
			XY2.dx+=dMaxRa_km*1000.0;
			Pixel Pix2=pG->XY2Pix(XY2);
			int nRa=abs(Pix2.nx-PixPos.nx);
			if (nRa<1)
				nRa=1;
			R.Left=PixPos.nx-nRa;
			R.Top=PixPos.ny-nRa;
			R.Right=PixPos.nx+nRa;
			R.Bottom=PixPos.ny+nRa;
			}
	}
#else
	void NavSiteStruct::CalcPix(TransGraph* pG)
	{
		PixPos=pG->LL2Pix(Pos);
		if (bDrawCircle)
			{
			XYPos XY1=pG->LL2XY(Pos);
			XYPos XY2=XY1;
			XY2.dx+=dMaxRa_km*1000.0;
			Pixel Pix2=pG->XY2Pix(XY2);
			int nRa_pix=abs(Pix2.nx-PixPos.nx);
			if (nRa_pix<1)
				nRa_pix=1;
         Radius_pix=Pixel(nRa_pix,nRa_pix);
			}
	}
#endif


void NavSiteStruct::DrawSite(TransGraph* pG, const TColor BackColor)
{
	if (bDisplay)
		SymbolStruct::Draw(pG,DWG_LAYER_BACK_BMP,BackColor);
}

void NavSiteStruct::DrawArea(TransGraph* pG)
{
	if ((bDrawCircle)&&(bDisplay))
		{
      pG->SetCurrentLayer(DWG_LAYER_BACK_BMP);
      if (bFill)
	      pG->SetBrush(FillCol);
      pG->SetPen(LineCol,nLineWidth);
      pG->Ellipse(PixPos.nx,PixPos.ny,Radius_pix.nx,Radius_pix.ny,bFill);
		}
}

void NavSiteStruct::Store(JIniFile* pFil, const int n)
{
	Store(pFil,"NAVSITE "+String(n));
}

void NavSiteStruct::Store(JIniFile* pFil, const String sName)
{
	SymbolStruct::Store(pFil,sName);
	pFil->Write("ID",wID);
	pFil->Write("Height",nHeight_m);
	pFil->Write("Auto Range",bAutoRa);
	pFil->Write("Max Range",dMaxRa_km);
	pFil->Write("Draw Circle",bDrawCircle);
	pFil->Write("Line Colour",LineCol);
	pFil->Write("Line Width",nLineWidth);
	pFil->Write("Fill Area",bFill);
	pFil->Write("Fill Colour",FillCol);
}

void NavSiteStruct::Read(JIniFile* pFil, const int n)
{
	Read(pFil,"NAVSITE "+String(n));
}

void NavSiteStruct::Read(JIniFile* pFil, const String sName)
{
	SymbolStruct::Read(pFil,sName);
	wID=(WORD)pFil->nRead("ID",0);
	pFil->Read("Height",nHeight_m,0);
	pFil->Read("Auto Range",bAutoRa,true);
	pFil->Read("Max Range",dMaxRa_km,1.0);
	pFil->Read("Draw Circle",bDrawCircle,true);
	pFil->Read("Line Colour",LineCol,clLtGray);
	pFil->Read("Line Width",nLineWidth,1);
	pFil->Read("Fill Area",bFill,true);
	pFil->Read("Fill Colour",FillCol,clGray);
}

NavSites::NavSites()
{
}

NavSites::~NavSites()
{
}

void NavSites::Edit(TForm* pOwner)
{
	NavSiteForm=new TNavSiteForm(pOwner);
	NavSiteForm->NavSite=Site;
	if (NavSiteForm->ShowModal()==mrOk)
		Site=NavSiteForm->NavSite;
	delete NavSiteForm;
}

void NavSites::Store(const String sFile)
{
	JIniFile Ini(true,sFile);
	Store(&Ini);
}

void NavSites::Read(const String sFile)
{
	JIniFile Ini(false,sFile);
	Read(&Ini);
}

void NavSites::Store(JIniFile* pIni)
{
	Site.Pack();
	pIni->Write("NAV SITE DISPLAY","Site Count",Site.nGetCount());
	for (int i=0; i<Site.nGetCount(); i++)
		Site[i]->Store(pIni,i);
}

void NavSites::Read(JIniFile* pIni)
{
	int n;
	Site.Clear();
	pIni->Read("NAV SITE DISPLAY","Site Count",n,0);
	for (int i=0; i<n; i++)
		{
		NavSiteStruct* pS=new NavSiteStruct;
		pS->Read(pIni,i);
      Site.nAdd(pS);
		}
	Site.Pack();
}

#ifdef __DDGRAPH
	void NavSites::CalcPix(DDGraph* pG)
	{
		for (int i=0; i<Site.nGetCount(); i++)
			Site[i]->CalcPix(pG);
	}
#else
	void NavSites::CalcPix(TransGraph* pG)
	{
		for (int i=0; i<Site.nGetCount(); i++)
			Site[i]->CalcPix(pG);
	}
#endif


void NavSites::DrawSites(TransGraph* pG, const TColor BackColor)
{
	for (int i=0; i<Site.nGetCount(); i++)
		{
		Site[i]->CalcSize(pG,DWG_LAYER_BACK_BMP,false);
		Site[i]->DrawSite(pG,BackColor);
		}
}

void NavSites::DrawAreas(TransGraph* pG)
{
	for (int i=0; i<Site.nGetCount(); i++)
		Site[i]->DrawArea(pG);
}

void NavSites::Refresh(const WORD wID, const String sDescr, const LatLong&& LL, const int nHeight_m)
{
	Site.Pack();
	int nFound=-1;
	for (int i=0; i<Site.nGetCount(); i++)
		{
		if (Site[i]->wID==wID)
			{
			nFound=i;
			i=Site.nGetCount();
			}
		}
	if (nFound>=0)
		{
		Site[nFound]->SetText(sDescr);
		Site[nFound]->SetPos(LL);
		Site[nFound]->nHeight_m=nHeight_m;
		}
	else
		{
		NavSiteStruct* pS=new NavSiteStruct;
		pS->wID=wID;
		pS->nHeight_m=nHeight_m;
		pS->SetText(sDescr);
		pS->SetPos(LL);
		Site.nAdd(pS);
		Site.Pack();
		}
}

