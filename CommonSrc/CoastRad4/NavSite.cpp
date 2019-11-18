//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "NavSite.h"
#include "NavSiteDlg.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


NavSiteStruct::NavSiteStruct()
{
	sDescr="";
   bDispSite=true;
   nHeight_m=0;
   bDrawCircle=false;
   bAutoRa=false;
   dMaxRa_km=1.0;
   R=TRect(0,0,1,1);
   LineCol=clWhite;
   nLineWidth=1;
   bFill=true;
   FillCol=clGray;
   nID=0;
   bDispSymbol=true;
   bDispName=true;
   PixPos=Pixel(0,0);
}



#ifdef __DDGRAPH
	void NavSiteStruct::CalcPix(DDGraph* pG)
	{
		PixPos=pG->LL2Pix(LL);
		if (bDrawCircle)
			{
			XYPos XY1=pG->LL2XY(LL);
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
			nRa_pix=abs(Pix2.nx-PixPos.nx);
			if (nRa_pix<1)
				nRa_pix=1;
			R.Left=PixPos.nx-nRa;
			R.Top=PixPos.ny-nRa;
			R.Right=PixPos.nx+nRa;
			R.Bottom=PixPos.ny+nRa;
			}
	}
#endif


void NavSiteStruct::DrawSite(HDC DC,const int nSymSize, const TColor BackColor)
{
	if (bDispSite)
   	{
      int nx=PixPos.nx;
      int ny=PixPos.ny;
      if (bDispSymbol)
      	{
         ::Ellipse(DC,nx-nSymSize,ny-nSymSize,nx+nSymSize,ny+nSymSize);
			ny+=nSymSize;
         }
		if (bDispName)
         ::TextOut(DC,nx,ny,sDescr.c_str(),sDescr.Length());
      }
}

void NavSiteStruct::DrawArea(HDC DC)
{
	if ((bDrawCircle)&&(bDispSite))
		{
		HBRUSH hBrush;
		if (bFill)
			hBrush=::CreateSolidBrush(g_GetColor(FillCol));
		else
			hBrush=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HPEN hPen=::CreatePen(PS_SOLID,nLineWidth,g_GetColor(LineCol));
		HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
		HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
		::Ellipse(DC,R.Left,R.Top,R.right,R.Bottom);
		::SelectObject(DC,hOldBrush);
		::DeleteObject(hBrush);
		::SelectObject(DC,hOldPen);
		::DeleteObject(hPen);
		}
}

void NavSiteStruct::Store(JIniFile* pFil, const int n)
{
	Store(pFil,"NAVSITE "+String(n));
}

void NavSiteStruct::Store(JIniFile* pFil, const String sName)
{
	pFil->Write(sName,"Descr",sDescr);
	pFil->Write("Display Site",bDispSite);
	pFil->Write("Display Name",bDispName);
	pFil->Write("Display Symbol",bDispSymbol);
	pFil->Write("Position",LL);
	pFil->Write("ID",nID);
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
	pFil->Read(sName,"Descr",sDescr,"");
	pFil->Read("Display Site",bDispSite,true);
	pFil->Read("Display Name",bDispName,true);
	pFil->Read("Display Symbol",bDispSymbol,true);
	pFil->Read("Position",LL);
	pFil->Read("ID",nID);
	pFil->Read("Height",nHeight_m,0);
	pFil->Read("Auto Range",bAutoRa,true);
	pFil->Read("Max Range",dMaxRa_km,1.0);
	pFil->Read("Draw Circle",bDrawCircle,false);
	pFil->Read("Line Colour",LineCol,clWhite);
	pFil->Read("Line Width",nLineWidth,1);
	pFil->Read("Fill Area",bFill,false);
	pFil->Read("Fill Colour",FillCol,clGreen);
}

NavSites::NavSites()
{
	pFont=new TFont;
   pFont->Name="Arial";
   pFont->Size=10;
   pFont->Color=clLime;
  	nSymbolSize=3;
  	SymbolColor=clLime;
}

NavSites::~NavSites()
{
	delete pFont;
}

void NavSites::Edit(TForm* pOwner)
{
	NavSiteForm=new TNavSiteForm(pOwner);
	NavSiteForm->NavSite=Site;
	NavSiteForm->pFont->Assign(pFont);
   NavSiteForm->nSymbolSize=nSymbolSize;
   NavSiteForm->SymbolColor=SymbolColor;

	if (NavSiteForm->ShowModal()==mrOk)
   	{
		Site=NavSiteForm->NavSite;
		pFont->Assign(NavSiteForm->pFont);
   	nSymbolSize=NavSiteForm->nSymbolSize;
   	SymbolColor=NavSiteForm->SymbolColor;
      }
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


void NavSites::DrawSites(HDC DC, const TColor BackColor)
{

   int nWeight;
   if (pFont->Style.Contains(fsBold))
      nWeight=FW_BOLD;
   else
      nWeight=FW_NORMAL;
   HFONT hFont=::CreateFont(-pFont->Size,	// logical height of font
               0,	// logical average character width
               0,	// angle of escapement
               0,	// base-line orientation angle
               nWeight,	// font weight
               pFont->Style.Contains(fsItalic),	// italic attribute flag
               pFont->Style.Contains(fsUnderline),	// underline attribute flag
               pFont->Style.Contains(fsStrikeOut),	// strikeout attribute flag
               DEFAULT_CHARSET,	// character set identifier
               OUT_DEFAULT_PRECIS,	// output precision
               CLIP_DEFAULT_PRECIS,	// clipping precision
               PROOF_QUALITY,	// output quality
               DEFAULT_PITCH|FF_DONTCARE,	// pitch and family
               pFont->Name.c_str()); 	// pointer to typeface name string
   HFONT hOldFont=(HFONT)::SelectObject(DC,hFont);
   ::SetTextColor(DC,pFont->Color);

   HPEN hSymPen=::CreatePen(PS_SOLID,1,SymbolColor);
   HPEN hOldPen=(HPEN)::SelectObject(DC,hSymPen);
   int nOldMode=::SetBkMode(DC,TRANSPARENT);
   HBRUSH hBrush=(HBRUSH)::GetStockObject(NULL_BRUSH);
   HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);

	for (int i=0; i<Site.nGetCount(); i++)
		{
//		Site[i]->CalcSize(DC);
		Site[i]->DrawSite(DC,nSymbolSize,BackColor);



		}

   ::SelectObject(DC,hOldBrush);

   ::SetBkMode(DC,nOldMode);

	::SelectObject(DC,hOldPen);
   ::SelectObject(DC,hOldFont);

}

void NavSites::DrawAreas(HDC DC)
{
	for (int i=0; i<Site.nGetCount(); i++)
   	{
   	if (Site[i]->bDrawCircle)
			Site[i]->DrawArea(DC);
      }
}

void NavSites::Refresh(const int nID, const String sDescr, const LatLong&& LL, const int nHeight_m)
{

	NavSiteStruct* pS;
	NavSiteStruct* pFound=NULL;
	Site.GoFirst();
   while(!Site.bLast() && !pFound)
   	{
      pS=Site.pNext();
      if (pS->nID==nID)
      	pFound=pS;
      }
	if (pFound)
   	{
   	pFound->sDescr=sDescr;
      pFound->LL=LL;
      pFound->nHeight_m=nHeight_m;
      }
   else
   	{
		pS=new NavSiteStruct;
		pS->nID=nID;
		pS->nHeight_m=nHeight_m;
		pS->sDescr=sDescr;
		pS->LL=LL;
		Site.nAdd(pS);
      }
	Site.Pack();
}

