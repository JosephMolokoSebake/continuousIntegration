//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "SiteStatusDlg.h"
#include "JFileDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TSiteStatusForm *SiteStatusForm;
//---------------------------------------------------------------------------
__fastcall TSiteStatusForm::TSiteStatusForm(TComponent* Owner)
	: TForm(Owner)
{
	pLock=NULL;
	pStat=NULL;
	nSite=-1;
}
//---------------------------------------------------------------------------
void __fastcall TSiteStatusForm::FormShow(TObject *)
{
   Ed.Init();
	PB->ControlStyle<<csOpaque;      //Prevent erasing of background with repaint
	CurrentTime_loc=JTime(true);
	CurrentTime_utc=CurrentTime_loc.ReturnUTC(g_nCst4TimeBias_hours);
	dCurrent_loc_sec=CurrentTime_loc.dTotalSec();
	dCurrent_utc_sec=CurrentTime_utc.dTotalSec();
	dwCurrent_loc_sec=CurrentTime_loc.dwTotalSec();
	dwCurrent_utc_sec=CurrentTime_utc.dwTotalSec();
	#ifdef _CST4_DICT
		Caption=g_Dict.sTranslate("Data Source Status")+" @ "+CurrentTime_loc.sDateTime();
	#else
		Caption="Data Source Status @ "+CurrentTime_loc.sDateTime();
	#endif
	if ((pLock)&&(pStat))
		{
		pLock->Acquire();
		Cst4SiteStats* pS;
		for (int nID=0; nID<65536; nID++)
			{
			pS=pStat->pStatFromID(nID);
			if (pS)
				SiteList.nAdd(new Cst4SiteStats(*pS));
			}
		SiteList.Sort();
		pLock->Release();
		SiteList.Pack();
		if (SiteList.nGetCount()<14)
			StatGrid->RowCount=15;
		else
			StatGrid->RowCount=SiteList.nGetCount()+1;
		for (int nR=1; nR<=SiteList.nGetCount(); nR++)
			ShowSite(nR,SiteList[nR-1]);
		}

	#ifdef _CST4_DICT

		StatGrid->ColWidths[0]=210;  StatGrid->Cells[0][0]=g_Dict.sTranslate("Site");
		StatGrid->ColWidths[1]=110;  StatGrid->Cells[1][0]=g_Dict.sTranslate("First Msg");
		StatGrid->ColWidths[2]=110;  StatGrid->Cells[2][0]=g_Dict.sTranslate("Last Msg");
		StatGrid->ColWidths[3]=110;  StatGrid->Cells[3][0]=g_Dict.sTranslate("Total Msg");
		StatGrid->ColWidths[4]=110;  StatGrid->Cells[4][0]=g_Dict.sTranslate("Msg Last 24h");
		StatGrid->ColWidths[5]=110;  StatGrid->Cells[5][0]=g_Dict.sTranslate("Msg Last Hour");

		g_Dict.Translate(Label1);
		g_Dict.Translate(GroupBox1);
		g_Dict.Translate(SaveBut);
		g_Dict.Translate(Panel1);
		g_Dict.Translate(Panel2);
		g_Dict.Translate(Panel3);
		g_Dict.Translate(Panel4);

	#else

		StatGrid->ColWidths[0]=210;  StatGrid->Cells[0][0]="Site";
		StatGrid->ColWidths[1]=110;  StatGrid->Cells[1][0]="First Msg";
		StatGrid->ColWidths[2]=110;  StatGrid->Cells[2][0]="Last Msg";
		StatGrid->ColWidths[3]=110;  StatGrid->Cells[3][0]="Total Msg";
		StatGrid->ColWidths[4]=110;  StatGrid->Cells[4][0]="Msg Last 24h";
		StatGrid->ColWidths[5]=110;  StatGrid->Cells[5][0]="Msg Last Hour";

	#endif

}
//---------------------------------------------------------------------------
void __fastcall TSiteStatusForm::FormClose(TObject *,
		TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------

void TSiteStatusForm::UpdateStats(const JTime& CurrentTime_loc)
{
	CurrentTime_utc=CurrentTime_loc.ReturnUTC(g_nCst4TimeBias_hours);
	dCurrent_loc_sec=CurrentTime_loc.dTotalSec();
	dCurrent_utc_sec=CurrentTime_utc.dTotalSec();
	dwCurrent_loc_sec=CurrentTime_loc.dwTotalSec();
	dwCurrent_utc_sec=CurrentTime_utc.dwTotalSec();
	Caption="Data Source Status @ "+CurrentTime_loc.sDateTime();
	if (pStat)
		{
		SiteList.Clear();
		Cst4SiteStats* pS;
		for (int nID=0; nID<65536; nID++)
			{
			pS=pStat->pStatFromID(nID);
			if (pS)
				SiteList.nAdd(new Cst4SiteStats(*pS));
			}
		SiteList.Sort();
		SiteList.Pack();
		if (SiteList.nGetCount()<14)
			StatGrid->RowCount=15;
		else
			StatGrid->RowCount=SiteList.nGetCount()+1;
		for (int nR=1; nR<=SiteList.nGetCount(); nR++)
			ShowSite(nR,SiteList[nR-1]);
		}
}

void TSiteStatusForm::ShowSite(const int nR, Cst4SiteStats* pS)
{
	String s;
	StatGrid->Cells[0][nR]=pS->sGetDescr();	//pSiteDefs->sGetDescrFromID(pS->wGetID(),s);
	StatGrid->Cells[1][nR]=pS->GetFirstMsgTime_loc().sDateTime();
	StatGrid->Cells[2][nR]=pS->GetLastMsgTime_loc().sDateTime();
   s.printf(L"%u",pS->dwGetTotalMsgCnt());
	StatGrid->Cells[3][nR]=s;
	s.printf(L"%u",pS->dwGetMsgCnt_utc(CurrentTime_utc,24*60));
	StatGrid->Cells[4][nR]=s;
   s.printf(L"%u",pS->dwGetMsgCnt_utc(CurrentTime_utc,60));
   StatGrid->Cells[5][nR]=s;
}

void __fastcall TSiteStatusForm::StatGridDrawCell(TObject *,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   if (State.Contains(gdFixed))
      {
      StatGrid->Canvas->Brush->Color=StatGrid->FixedColor;
      }
   else
      {
      TColor Col;
      if (ARow-1<SiteList.nGetCount())
         {
			Cst4SiteStats* pS=SiteList[ARow-1];
			double dT_utc_sec=pS->dGetLastMsgTime_sec_utc();
			if (dCurrent_utc_sec-dT_utc_sec<60.0)
				Col=clLime; //Data in last minute
			else if (pS->dwGetMsgCnt_utc(dwCurrent_utc_sec,60)>0)
				Col=clAqua;  //Data in last hour
			else if (pS->dwGetMsgCnt_utc(dwCurrent_utc_sec,24*60)>0)
				Col=clYellow;
			else
				Col=clRed;  //No data in last 24 hours
         }
      else
         Col=clWindow;
      StatGrid->Canvas->Brush->Color=Col;
      }
   StatGrid->Canvas->Font->Color=clBlack;
   StatGrid->Canvas->FillRect(Rect);
   int nH=(Rect.Bottom-Rect.Top-StatGrid->Canvas->TextHeight(L"H"))/2;
   StatGrid->Canvas->TextOut(Rect.Left+nH,Rect.Top+nH,StatGrid->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------

void __fastcall TSiteStatusForm::SaveButClick(TObject *)
{
   String sFile=L"c:\\CoastRad4 Shared\\Export\\CoastRad4 Site Status "+CurrentTime_loc.sFileName(L"csv");
   sFile=sSelectSaveFileDef(L"Site Status Files(*.csv)",L"*.csv",L"Specify Site Status File",sFile);
   if (!sFile.IsEmpty())
      {
      JFile Fil('O',JFile::ASCII_TYPE);
      Fil.Create(sFile);
      Fil.TextLine(L"Site,First Msg,Last Msg,Total Msg,Msg Last 24h,Msg Last Hour");
      for (int i=0; i<SiteList.nGetCount(); i++)
         {
         Cst4SiteStats* pS=SiteList[i];
			String s,sV;
//			s=pSiteDefs->sGetDescrFromID(pS->wGetID(),sV)+",L"+pS->GetFirstMsgTime_loc().sDateTime()+",L"+pS->GetLastMsgTime_loc().sDateTime();
			s=pS->sGetDescr()+",L"+pS->GetFirstMsgTime_loc().sDateTime()+",L"+pS->GetLastMsgTime_loc().sDateTime();
			sV.printf(L",%u,%u,%u",pS->dwGetTotalMsgCnt(),pS->dwGetMsgCnt_utc(CurrentTime_utc,24*60),pS->dwGetMsgCnt_utc(CurrentTime_utc,60));
			s=s+sV;
         Fil.TextLine(s);
         }

      }
}
//---------------------------------------------------------------------------

void __fastcall TSiteStatusForm::StatGridSelectCell(TObject *,
		int /*ACol*/, int ARow, bool &/*CanSelect*/)
{
   if (ARow-1<SiteList.nGetCount())
      {
      nSite=ARow-1;
      Cst4SiteStats* pS=SiteList[ARow-1];
      String s;
//      SiteGrp->Caption=pSiteDefs->sGetDescrFromID(pS->wGetID(),s);
		SiteGrp->Caption=pS->sGetDescr();
		}
   else
      {
      nSite=-1;
      SiteGrp->Caption=L"";
      }
   PB->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TSiteStatusForm::PBPaint(TObject *)
{
   TCanvas* pC=PB->Canvas;
   pC->Brush->Style=bsSolid;
   pC->Brush->Color=clBlack;
   int nW=PB->Width-1;
   TRect R(0,0,nW,PB->Height-1);
   pC->FillRect(R);
   if (nSite>=0)
      {
      Cst4SiteStats* pS=SiteList[nSite];
		int nMin2_loc=CurrentTime_loc.dwTotalSec()/60;
      int nMin1_loc=nMin2_loc-24*60;
      int anCnt[24*60];
      memset(anCnt,0,sizeof(anCnt));
      int nMaxCnt=0;
      for (int i=0; i<24*60; i++)
         {
			int nM_loc=pS->nGetMin_utc(i)-g_nCst4TimeBias_hours*60;
			if ((nM_loc>=nMin1_loc)&&(nM_loc<nMin2_loc))
				{
				if (pS->nGetCnt(i)>nMaxCnt)
               nMaxCnt=pS->nGetCnt(i);
            anCnt[nM_loc-nMin1_loc]=pS->nGetCnt(i);
            }
         }
      if (nMaxCnt<5)
         nMaxCnt=5;
      TPoint aPnt[24*60];
      int nH=PB->Height-10;
      for (int i=0; i<24*60; i++)
         {
         aPnt[i].x=(nW*i)/(24*60);
         aPnt[i].y=PB->Height-(anCnt[i]*nH)/nMaxCnt-5;
         }
      pC->Pen->Color=clLime;
      pC->Pen->Width=1;
      pC->Pen->Style=psSolid;
      pC->Polyline(aPnt,24*60-1);
      }


}
//---------------------------------------------------------------------------

void __fastcall TSiteStatusForm::PBMouseMove(TObject *,
      TShiftState /*Shift*/, int X, int /*Y*/)
{
   bool bValid=false;
   if (nSite>=0)
      {
      Cst4SiteStats* pS=SiteList[nSite];
      int n=(24*60*X)/PB->Width;
      if ((n>=0)&&(n<24*60))
         {
			int nMin2_loc=CurrentTime_loc.dwTotalSec()/60;
			int nMin_loc=nMin2_loc-24*60;
			nMin_loc+=n;
			for (int i=0; i<24*60; i++)
				{
				if (nMin_loc==pS->nGetMin_utc(i)-g_nCst4TimeBias_hours*60)
					{
					Ed.SetValRO(MsgEd,pS->nGetCnt(i));
					i=24*60;
					bValid=true;
					}
				}
			JTime T((DWORD)(nMin_loc*60));
         DateEd->Text=T.sDateTime();
         if (!bValid)
            Ed.SetValRO(MsgEd,0);
         }
      else
         {
         DateEd->Text=L"";
         MsgEd->Text=L"";
         }
      }
   else
      {
      DateEd->Text=L"";
      MsgEd->Text=L"";
      }
}
//---------------------------------------------------------------------------

