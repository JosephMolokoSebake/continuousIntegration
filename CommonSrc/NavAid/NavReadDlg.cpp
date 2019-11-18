//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "NavReadDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavReadForm *NavReadForm;
//---------------------------------------------------------------------------
__fastcall TNavReadForm::TNavReadForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavReadForm::FormShow(TObject *)
{
#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption

	g_Dict.Translate(GroupBox1);
	g_Dict.Translate(Label1);
	g_Dict.Translate(Label2);
	g_Dict.Translate(CurSymRad);
	g_Dict.Translate(NewSymRad);

	g_Dict.Translate(GroupBox2);
	g_Dict.Translate(Label3);
	g_Dict.Translate(Label4);
	g_Dict.Translate(CurLineRad);
	g_Dict.Translate(NewLineRad);

	g_Dict.Translate(GroupBox3);
	g_Dict.Translate(ViewChk);

	g_Dict.Translate(GroupBox4);
	g_Dict.Translate(Label5);
	g_Dict.Translate(Label6);
	g_Dict.Translate(Label7);

	g_Dict.Translate(GroupBox5);
	g_Dict.Translate(Label8);
	g_Dict.Translate(Label9);
	g_Dict.Translate(Label10);
#endif

	OKBut->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TNavReadForm::FormClose(TObject *,
      TCloseAction &/*Action*/)
{
//   
}
//---------------------------------------------------------------------------
