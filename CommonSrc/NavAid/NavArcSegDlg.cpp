/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Surface Plot Analysis: Safety Arc Position Dialog
 *		PROJECT			Aegis III C1410
 *		ACTIVITY       5
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000 SP3
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      3.00: Initial Version JGT 26-10-2002
************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "NavArcSegDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavArcSegForm *NavArcSegForm;
//---------------------------------------------------------------------------
__fastcall TNavArcSegForm::TNavArcSegForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavArcSegForm::FormShow(TObject *)
{
	LatEd->SetFocus();

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);
	#endif

}
//---------------------------------------------------------------------------
