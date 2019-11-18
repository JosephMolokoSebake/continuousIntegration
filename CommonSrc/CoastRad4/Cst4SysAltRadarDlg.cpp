//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4SysAltRadarDlg.h"
#include "AISTargetInfoDlg.h"
#include "RadarTrackInfoDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TSysAltRadarForm *SysAltRadarForm;
//---------------------------------------------------------------------------
__fastcall TSysAltRadarForm::TSysAltRadarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSysAltRadarForm::FormShow(TObject */*Sender*/)
{
	AltList.Pack();
	TrackLB->Clear();
	String s,sV;
	for (int i=0; i<AltList.nGetCount(); i++)
		{
		s.printf(L"%5d ",AltList[i]->nSep_m);
		sV=sGetCst4Site(AltList[i]->T.wSource());
		while(sV.Length()<31)
			sV=sV+" ";
		s=s+sV;
		sV.printf(L"%5d ",AltList[i]->T.nID());
		s=s+sV;
		sV.printf(L"%4.1f %3.0f",AltList[i]->T.dSpeed_kts(),AltList[i]->T.dCourse_deg());
		s=s+sV;
      TrackLB->Items->Add(s);
      }
   TrackLB->ItemIndex=0;
//12345 Ship Name                      12345678 13.3  254
}
//---------------------------------------------------------------------------
void __fastcall TSysAltRadarForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TSysAltRadarForm::TrackLBDblClick(TObject */*Sender*/)
{
	ReplaceButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TSysAltRadarForm::ReplaceButClick(TObject */*Sender*/)
{
	int n=TrackLB->ItemIndex;
   if (n>=0)
   	{
		Radar=AltList[n]->T;
		ModalResult=mrOk;
      }
}
//---------------------------------------------------------------------------
