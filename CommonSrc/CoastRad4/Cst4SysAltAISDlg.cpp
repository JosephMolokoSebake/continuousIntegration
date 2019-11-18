//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4SysAltAISDlg.h"
#include "AISTargetInfoDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma //link "NumEdit"
#pragma resource "*.dfm"
TSysAltAISForm *SysAltAISForm;
//---------------------------------------------------------------------------
__fastcall TSysAltAISForm::TSysAltAISForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSysAltAISForm::FormShow(TObject */*Sender*/)
{
   Ed.Init();

	AltList.Pack();

	if (AIS.nMMSI>0)
		{
		CNameEd->Text=AIS.sName;
		int nMMSI=AIS.nMMSI;
		Ed.SetValRO(CMMSIEd,nMMSI);
		Ed.SetValRO(CSpeedEd,AIS.dSpeed_kts());
		Ed.SetValRO(CCourseEd,AIS.dCourse_deg());
		ReplaceBut->Caption="Replace Track";
		}
	else
		ReplaceBut->Caption="Add Track";

   TrackLB->Clear();
   String s,sV;
	for (int i=0; i<AltList.nGetCount(); i++)
		{
		s.printf(L"%5d ",AltList[i]->nSep_m);
		sV=AltList[i]->S.sShipName();
		while(sV.Length()<31)
			sV=sV+" ";
		s=s+sV;
		sV.printf(L"%9d ",AltList[i]->D.nGetMMSI());
		s=s+sV;
      sV.printf(L"%4.1f %3.0f",AltList[i]->D.dSpeed_kts(),AltList[i]->D.dCourse_deg());
		s=s+sV;
      TrackLB->Items->Add(s);
      }
   TrackLB->ItemIndex=0;
//12345 Ship Name                      12345678 13.3  254
}
//---------------------------------------------------------------------------
void __fastcall TSysAltAISForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TSysAltAISForm::TrackLBDblClick(TObject */*Sender*/)
{
	ReplaceButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TSysAltAISForm::ReplaceButClick(TObject */*Sender*/)
{
	int n=TrackLB->ItemIndex;
	if (n>=0)
   	{
		AIS.Set(&AltList[n]->D);
		AIS.Set(&AltList[n]->S);
		ModalResult=mrOk;
      }
}
//---------------------------------------------------------------------------
void __fastcall TSysAltAISForm::InfoButClick(TObject */*Sender*/)
{
	int n=TrackLB->ItemIndex;
   if (n>0)
   	{
      TAISTargetInfoForm* pT=new TAISTargetInfoForm(this);
		Cst4_AIS_Dynamic* pD;
		Cst4_AIS_Static* pS;
		if (AltList[n]->D.bValid())
			pD=&AltList[n]->D;
		else
			pD=NULL;
		if (AltList[n]->S.bValid())
			pS=&AltList[n]->S;
      else
         pS=NULL;
      pT->SetInfo(pD,pS);
      pT->SysTrackBut->Enabled=false;
      pT->ShowModal();
      delete pT;
      }
}
//---------------------------------------------------------------------------
