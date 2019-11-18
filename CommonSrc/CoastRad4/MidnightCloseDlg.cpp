//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "MidnightCloseDlg.h"
#include "JUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

DWORD dwStartClose_loc_sec;

TMidnightCloseForm *MidnightCloseForm;
//---------------------------------------------------------------------------
__fastcall TMidnightCloseForm::TMidnightCloseForm(TComponent* Owner)
	: TForm(Owner)
{
	dwNextClose_loc_sec=0;
}
//---------------------------------------------------------------------------
void __fastcall TMidnightCloseForm::FormShow(TObject */*Sender*/)
{
   Caption=sGetExeDescr();
	CancelBut->SetFocus();
	ProgBar->Position=0;
}
//---------------------------------------------------------------------------
void __fastcall TMidnightCloseForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)

{
//
}
//---------------------------------------------------------------------------

void __fastcall TMidnightCloseForm::CancelButClick(TObject */*Sender*/)
{
	Close();
	delete MidnightCloseForm;
	MidnightCloseForm=NULL;
	dwStartClose_loc_sec+=24*60*60;
}
//---------------------------------------------------------------------------


void InitCloseForm(JTime T_loc, const DWORD dwTime_sec)
{
	MidnightCloseForm=NULL;
	T_loc+=24*60*60;    					//Increment date by one day
	T_loc.SetTime(dwTime_sec*1000);  //Set time only
	DWORD dwT_loc_sec=T_loc.dwTotalSec();
	dwStartClose_loc_sec=dwT_loc_sec-60;  //one minute earlier

 //	dwStartClose_loc_sec=JTime(true).dwTotalSec()+90;


}

bool bUpdateCloseForm(TComponent* Owner, const JTime T_loc)
{
	return bUpdateCloseForm(Owner,T_loc.dwTotalSec());
}

bool bUpdateCloseForm(TComponent* Owner, const DWORD dwT_loc_sec)
{
	if ((dwT_loc_sec>=dwStartClose_loc_sec)&&(dwStartClose_loc_sec>0))
		{
		if (!MidnightCloseForm)
			{
			MidnightCloseForm=new TMidnightCloseForm(Owner);
			MidnightCloseForm->Show();
			}
		MidnightCloseForm->ProgBar->Position=dwT_loc_sec-dwStartClose_loc_sec;
		if (dwT_loc_sec>=dwStartClose_loc_sec+60)
			{
			MidnightCloseForm->Close();
			delete MidnightCloseForm;
			MidnightCloseForm=NULL;
			return true;
			}
		}
	return false;
}

