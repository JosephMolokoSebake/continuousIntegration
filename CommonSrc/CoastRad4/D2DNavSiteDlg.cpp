//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "D2DNavSiteDlg.h"
#include "Cst4SiteDefs.h"
#include "JColorDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavSiteForm *NavSiteForm;
//---------------------------------------------------------------------------
__fastcall TNavSiteForm::TNavSiteForm(TComponent* Owner)
	: TForm(Owner)
{
//	Personality=CST4_COASTRAD_PERS;
}
//---------------------------------------------------------------------------
void __fastcall TNavSiteForm::FormShow(TObject */*Sender*/)
{

   Ed.Init();

	NavSite.Pack();
	SiteLB->Clear();
	for (int i=0; i<NavSite.nGetCount(); i++)
		SiteLB->Items->Add(NavSite[i]->sGetText());
	for (int i=0; i<NavSite.nGetCount(); i++)
		SiteLB->Checked[i]=NavSite[i]->bDisplay;
	nSite=-1;
	pSite=NULL;
	if (NavSite.nGetCount()>0)
		SetSite(0);
	else
		SetSite(-1);
	DelSiteBut->Enabled=(nSite>=0);
	OKBut->SetFocus();

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(SiteGrp);
		g_Dict.Translate(Label1);
		g_Dict.Translate(SymbolBut);

		g_Dict.Translate(GroupBox2);
		g_Dict.Translate(Label2);
		g_Dict.Translate(Label3);
		g_Dict.Translate(Label4);
		g_Dict.Translate(Label7);

		g_Dict.Translate(GroupBox3);
		g_Dict.Translate(CoverChk);
		g_Dict.Translate(AutoChk);
		g_Dict.Translate(RaLab1);
		g_Dict.Translate(RaLab2);
		g_Dict.Translate(LineLab);
		g_Dict.Translate(FillChk);

		g_Dict.Translate(AddSiteBut);
		g_Dict.Translate(DelSiteBut);
		g_Dict.Translate(RefreshBut);


	#endif



}
//---------------------------------------------------------------------------
void __fastcall TNavSiteForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)
{
	if (nSite>=0)
		GetSite(nSite);
}

void TNavSiteForm::GetSite(const int n)
{
	if (n>=0)
		{
		pSite->SetText(DescrEd->Text);
		Ed.GetLL(LatEd,LongEd,pSite->Pos);
		Ed.GetVal(HeightEd,pSite->nHeight_m);
		pSite->bDrawCircle=CoverChk->Checked;
		pSite->bAutoRa=AutoChk->Checked;
		Ed.GetVal(MaxRaEd,pSite->dMaxRa_km);
		Ed.GetVal(WidthEd,pSite->nLineWidth);
		pSite->bFill=FillChk->Checked;
		pSite->LineCol=LinePan->Color;
		pSite->FillCol=FillPan->Color;
		pSite->bDisplay=SiteLB->Checked[n];
		}
}

void TNavSiteForm::SetSite(const int n)
{
	if (nSite>=0)
		GetSite(nSite);
	nSite=n;
	if (nSite>=0)
		{
		SiteGrp->Enabled=true;
		pSite=NavSite[nSite];
		DescrEd->Text=pSite->sGetText();
		Ed.nSetLL("Position",LatEd,LongEd,pSite->Pos);
		Ed.nSetVal(HeightEd,pSite->nHeight_m,"");
      Ed.SetLastLimits("Height",0,99999);
		CoverChk->Checked=pSite->bDrawCircle;
		AutoChk->Checked=pSite->bAutoRa;
		Ed.nSetVal(MaxRaEd,pSite->dMaxRa_km,1,"");
      Ed.SetLastLimits("Max Range",1.0,99999.0);
		Ed.nSetVal(WidthEd,pSite->nLineWidth,"");
      Ed.SetLastLimits("Line Width",1,9);
		FillChk->Checked=pSite->bFill;
		LinePan->Color=pSite->LineCol;
		FillPan->Color=pSite->FillCol;
		SymbolBut->Enabled=true;
		SiteLB->Checked[nSite]=pSite->bDisplay;
		CoverChkClick(0);
		}
	else
		{
		SiteGrp->Enabled=false;
		pSite=NULL;
		DescrEd->Text="";
		LatEd->Text="";
		LongEd->Text="";
		HeightEd->Text="";
		CoverChk->Checked=false;
		AutoChk->Checked=false;
		MaxRaEd->Text="";
		WidthEd->Text="";
		FillChk->Checked=false;
		LinePan->Color=clLtGray;
		FillPan->Color=clLtGray;
		SymbolBut->Enabled=false;
		}
	SiteLB->ItemIndex=nSite;
	DelSiteBut->Enabled=(nSite>=0);
	UpBut->Enabled=(SiteLB->ItemIndex>0);
	DownBut->Enabled=(SiteLB->ItemIndex>=0)&&(SiteLB->ItemIndex<SiteLB->Count-1);
}

//---------------------------------------------------------------------------
void __fastcall TNavSiteForm::AddSiteButClick(TObject */*Sender*/)
{
	if (nSite>=0)
		GetSite(nSite);
	NavSite.nAdd(new NavSiteStruct);
	SiteLB->Items->Add("");
	SiteLB->Checked[SiteLB->Count-1]=true;
	NavSite.Pack();
	pSite=NavSite[NavSite.nGetCount()-1];
	nSite=-1;
	SetSite(NavSite.nGetCount()-1);
	DescrEd->SetFocus();
	DelSiteBut->Enabled=(nSite>=0);
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::DelSiteButClick(TObject */*Sender*/)
{
	NavSite.Delete(nSite);
	NavSite.Pack();
	SiteLB->Items->Delete(nSite);
	nSite-=1;
	if (nSite<0)
		{
		if (NavSite.nGetCount()>0)
			nSite=0;
		}
	int n=nSite;
	nSite=-1;
	SetSite(n);
	DelSiteBut->Enabled=(nSite>=0);
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::SymbolButClick(TObject */*Sender*/)
{
	if (pSite)
		{
		pSite->SetText(DescrEd->Text);
		Ed.GetLL(LatEd,LongEd,pSite->Pos);
		pSite->bEditSiteSymbol();
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::LinePanClick(TObject */*Sender*/)
{
	#ifdef _CST4_DICT
		LinePan->Color=SelectColor(g_Dict.sTranslate("Circle Line Colour"),LinePan->Color,clBlack);
	#else
		LinePan->Color=SelectColor("Circle Line Colour",LinePan->Color,clBlack);
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::FillPanClick(TObject */*Sender*/)
{
	#ifdef _CST4_DICT
		FillPan->Color=SelectColor(g_Dict.sTranslate("Circle Fill Colour"),FillPan->Color,clBlack);
	#else
		FillPan->Color=SelectColor("Circle Fill Colour",FillPan->Color,clBlack);
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::CoverChkClick(TObject *Sender)
{
	AutoChk->Enabled=CoverChk->Checked;
	AutoChkClick(Sender);
	LineLab->Enabled=CoverChk->Checked;
	WidthEd->Enabled=CoverChk->Checked;
	LinePan->Enabled=CoverChk->Checked;
	FillChk->Enabled=CoverChk->Checked;
	FillPan->Enabled=CoverChk->Checked;
	FillChkClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::AutoChkClick(TObject */*Sender*/)
{
	RaLab1->Enabled=CoverChk->Checked && !AutoChk->Checked;
	RaLab2->Enabled=CoverChk->Checked && !AutoChk->Checked;
	MaxRaEd->Enabled=CoverChk->Checked && !AutoChk->Checked;
	CalcMaxRa();

}
//---------------------------------------------------------------------------

void TNavSiteForm::CalcMaxRa()
{
	if (AutoChk->Checked)
		{
		double dHgt_m=Ed.dGetVal(HeightEd);
		if (dHgt_m>=0)
			{
			double dRa_km=sqrt(16.79*(dHgt_m+25.0));
			Ed.nSetVal(MaxRaEd,dRa_km,"");
			}
		}
}

void __fastcall TNavSiteForm::HeightEdChange(TObject *Sender)
{
	AutoChkClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::FillChkClick(TObject */*Sender*/)
{
	FillPan->Enabled=CoverChk->Checked && FillChk->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::DescrEdChange(TObject */*Sender*/)
{
	if (nSite>=0)
		{
		SiteLB->Items->Strings[nSite]=DescrEd->Text;
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::SiteLBClick(TObject */*Sender*/)
{
	int n=SiteLB->ItemIndex;
	if (n>=0)
		{
		SetSite(n);
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::UpButClick(TObject */*Sender*/)
{
	int n=SiteLB->ItemIndex;
	if (n>0)
		{
		GetSite(n);
		SiteLB->Items->Exchange(n,n-1);
		NavSite.Exchange(n,n-1);
		nSite=-1;
		SetSite(n-1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::DownButClick(TObject */*Sender*/)
{
	int n=SiteLB->ItemIndex;
	if ((n>=0)&&(n<SiteLB->Count-1))
		{
		GetSite(n);
		SiteLB->Items->Exchange(n,n+1);
		NavSite.Exchange(n,n+1);
		nSite=-1;
		SetSite(n+1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavSiteForm::RefreshButClick(TObject */*Sender*/)
{
//	Cst4SiteDefs Sites;
//	if (g_pSiteDefs->bLoadSites())
//		{
		for (int nCountry=0; nCountry<g_pSiteDefs->nGetCountryCnt(); nCountry++)
			{
			for (int nLoc=0; nLoc<g_pSiteDefs->nGetLocCnt(nCountry); nLoc++)
				{
				String sLoc=g_pSiteDefs->sGetLocation(nCountry,nLoc);
				WORD wID=g_pSiteDefs->wGetIDFromRaw(nCountry,nLoc,0);
				LatLong LL;
				LL.Invalidate();
				int nHeight_m=0;
				for (int nType=0; (nType<g_pSiteDefs->nGetTypeCnt(nCountry,nLoc)) && !LL.bValid(); nType++)
					{
					LL=g_pSiteDefs->GetLL(nCountry,nLoc,nType);
					if (LL.bValid())
						nHeight_m=g_pSiteDefs->nGetHeight_m(nCountry,nLoc,nType);
					}
				if (!sLoc.IsEmpty()&&LL.bValid())
					{
               NavSite.GoFirst();
					NavSiteStruct* pS;
					NavSiteStruct* pFound=NULL;
               while(!NavSite.bLast() && !pFound)
               	{
                  pS=NavSite.pNext();
						if (pS->wID==wID)
							pFound=pS;
						}
					if (pFound)
						{
						pFound->SetText(sLoc);
						pFound->SetPos(LL);
						pFound->nHeight_m=nHeight_m;
						}
					else
						{
                  pS=new NavSiteStruct;
						pS->wID=wID;
						pS->nHeight_m=nHeight_m;
						pS->SetText(sLoc);
						pS->SetPos(LL);
						NavSite.nAdd(pS);
						NavSite.Pack();
						}
					}
				}
			}
      NavSite.Pack();
//		}
	SiteLB->Clear();
	for (int i=0; i<NavSite.nGetCount(); i++)
		SiteLB->Items->Add(NavSite[i]->sGetText());
	for (int i=0; i<NavSite.nGetCount(); i++)
		SiteLB->Checked[i]=NavSite[i]->bDisplay;
	nSite=-1;
	pSite=NULL;
	if (NavSite.nGetCount()>0)
		SetSite(0);
	else
		SetSite(-1);
	DelSiteBut->Enabled=(nSite>=0);
	OKBut->SetFocus();
}
//---------------------------------------------------------------------------


void __fastcall TNavSiteForm::DeleteAllButClick(TObject *Sender)
{
	if (bAskYes("This Will Delete All Site Infiormation. Are You Sure ?","Delete All Sites"))
   	{
		NavSite.Clear();
		SiteLB->Items->Clear();
      nSite=-1;
      }
}
//---------------------------------------------------------------------------

