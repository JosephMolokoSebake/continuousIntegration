//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JInfoDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJInfoForm *JInfoForm;
//---------------------------------------------------------------------------
__fastcall TJInfoForm::TJInfoForm(TComponent* Owner)
   : TForm(Owner)
{
   bAsk=false;
}
//---------------------------------------------------------------------------
void __fastcall TJInfoForm::FormShow(TObject *)
{
   InfoLab->Caption=sI;
   if (InfoLab->Width>ClientWidth-32)
      {
      ClientWidth=InfoLab->Width+32;
      Bevel1->Width=ClientWidth-16;
      OKBut->Left=ClientWidth-(344-176);
      CancelBut->Left=ClientWidth-(344-260);
      PB->Width=ClientWidth-(344-157);
      }
	if (nSec>0)
		{
		PB->Max=nSec*10;
		PB->Position=0;
		if (!bAsk)
			{
			CancelBut->Hide();
			int nW=ClientWidth-(PB->Left+PB->Width);
			OKBut->Left=PB->Left+PB->Width+(nW-OKBut->Width)/2;
			}
		Timer->Enabled=true;
		}
	else
		{
		PB->Hide();
		if (!bAsk)
			{
			CancelBut->Hide();
			OKBut->Left=(ClientWidth-OKBut->Width)/2;
			}
		}
	if (bAsk)
		{
		OKBut->Kind=bkYes;
		CancelBut->Kind=bkNo;
		}
	else
	ModalResult=0;
   OKBut->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TJInfoForm::FormClose(TObject *,
      TCloseAction &)
{
//
}
//---------------------------------------------------------------------------

void TJInfoForm::SetInfo(const String sInfo, const String sHeader, const int nTimeOut_sec)
{
   Caption=sHeader;
   sI=sInfo;
   nSec=nTimeOut_sec;
}

void __fastcall TJInfoForm::TimerTimer(TObject *)
{
   PB->Position++;
   if (PB->Position==PB->Max)
      {
      if (bDefaultYes)
			ModalResult=mrYes;
      else
			ModalResult=mrNo;
      }
}
//---------------------------------------------------------------------------



