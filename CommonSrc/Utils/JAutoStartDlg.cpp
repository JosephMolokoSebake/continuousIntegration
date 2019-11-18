//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JAutoStartDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAutoStartForm *AutoStartForm;
//---------------------------------------------------------------------------
__fastcall TAutoStartForm::TAutoStartForm(TComponent* Owner)
   : TForm(Owner)
{
   nTotalTimeSec=10;
}
//---------------------------------------------------------------------------
void __fastcall TAutoStartForm::FormShow(TObject *)
{
   Timer->Interval=(nTotalTimeSec*1000)/100;
   Timer->Enabled=true;
   CancelBut->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAutoStartForm::TimerTimer(TObject *)
{
	++Bar->Position;
//   Application->ProcessMessages();
   if (Bar->Position==100)
      ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TAutoStartForm::SkipButClick(TObject *)
{
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------

bool bAutoStart(const String sCaption, const int nDelay_sec)
{
	AutoStartForm=new TAutoStartForm(0);
	AutoStartForm->Caption=sCaption;
	AutoStartForm->nTotalTimeSec=nDelay_sec;
	int nRet=AutoStartForm->ShowModal();
	delete AutoStartForm;
	return (nRet==mrOk);
}

