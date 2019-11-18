/** CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Message Dialog with Scrollbar
 *		PROJECT			Aegis/C0953
 *		ACTIVITY       1-11
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium III
 *		OS					MS Windows NT4
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 12/09/2000
 ************************************************************************/
//---------------------------------------------------------------------------

#include "jpch.h"
#pragma hdrstop

#include "JMessageBar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)             
#pragma resource "*.dfm"
TMessageBarForm *MessageBarForm;
//---------------------------------------------------------------------------
__fastcall TMessageBarForm::TMessageBarForm(TComponent* Owner)
	: TForm(Owner)
{
	bProcessMessages=true;
}
//---------------------------------------------------------------------------

void __fastcall TMessageBarForm::SetCaption(const String sCapt, const bool bShowAbort)
{
	Caption=sCapt;
	ProgressBar->Position=0;
	AbortBut->Enabled=bShowAbort;
	if (bProcessMessages)
		Application->ProcessMessages();
	nPrev=-1;
}

void __fastcall TMessageBarForm::SetProgress(const int n, const int nMax)
{
	if (n!=nPrev)
		{
		nPrev=n;
		if (nMax>0)
			ProgressBar->Position=(n*100)/nMax;
		else
			ProgressBar->Position=n;
		if (bProcessMessages)
			Application->ProcessMessages();
		}
}

void __fastcall TMessageBarForm::FormShow(TObject *)
{
	ProgressBar->Position=0;
	bAbort=false;
	if (AbortBut->Enabled)
		AbortBut->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TMessageBarForm::AbortButClick(TObject *)
{
	bAbort=true;
}
//---------------------------------------------------------------------------

