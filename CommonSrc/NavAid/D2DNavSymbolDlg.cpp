/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE   General Navigational Aid Symbol Dialog
 *    FILE NAME		NavSymbolDialog.cpp
 *		PROJECT
 *		ACTIVITY       
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 03/12/2001
 ************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "D2DNavSymbolDlg.h"
#include "D2DNavAid.h"
#include "JFile.h"
#include "JUtils.h"
#include "JColorDlg.h"
#include "JFontDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavSymbolForm *NavSymbolForm;
//---------------------------------------------------------------------------
__fastcall TNavSymbolForm::TNavSymbolForm(TComponent* Owner)
   : TForm(Owner)
{
	bEditSite=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::SymChkClick(TObject *)
{
   SymCombo->Enabled=SymChk->Checked || bDefault;
   SymSizeEd->Enabled=SymChk->Checked || bDefault;
   SymColPan->Enabled=SymChk->Checked || bDefault;
   SymSizeLab->Enabled=SymChk->Checked || bDefault;
   SolidChk->Enabled=SymChk->Checked || bDefault;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::FormShow(TObject *)
{
   SymCombo->Items->Clear();
   SymCombo->Items->Add("Circle");
   SymCombo->Items->Add("Square");
   SymCombo->Items->Add("Diamond");
   SymCombo->Items->Add("Cross");
   SymCombo->ItemIndex=nSymbol;

   bDelete=false;

   if (bDefault)
      {
      SymChk->Checked=false;
      SymChk->Enabled=false;
      TextChk->Checked=false;
      TextChk->Enabled=false;
      LockChk->Checked=false;
      LockChk->Enabled=false;
      RefChk->Checked=false;
      RefChk->Enabled=false;
      LatEd->Text=L"";
      LongEd->Text=L"";
      TextMemo->Clear();
      LatEd->Color=cl3DLight;
      LongEd->Color=cl3DLight;
      TextMemo->Color=cl3DLight;
      }
   else
      {
      LatEd->Color=clWindow;
      LongEd->Color=clWindow;
      TextMemo->Color=clWindow;
      }
   TextChkClick(0);
   SymChkClick(0);
   LatEd->Enabled=!bDefault;
   LatLab->Enabled=!bDefault;
   LongEd->Enabled=!bDefault;
   LongLab->Enabled=!bDefault;
	DelBut->Enabled=!bDefault && bShowDel;
	if (bEditSite)
		OKBut->SetFocus();
	else
		{
		if (TextChk->Checked)
			TextMemo->SetFocus();
		else if (SymChk->Checked)
			SymCombo->SetFocus();
		else
			OKBut->SetFocus();
		}
	SymColPan->Enabled=bSubEdit;
	FontBut->Enabled=bSubEdit;

	if (bEditSite)
		{
		LockChk->Checked=false;
		LockChk->Enabled=false;
		TextMemo->Enabled=false;
		RefChk->Enabled=false;
		DispChk->Enabled=false;
		LatEd->Enabled=false;
		LatLab->Enabled=false;
		LongEd->Enabled=false;
		LongLab->Enabled=false;
		}
	else
		{
		LockChkClick(0);
		RefChkClick(0);
		}

#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption
	g_Dict.Translate(DispChk);
	g_Dict.Translate(DelBut);

	g_Dict.Translate(GroupBox1);
	g_Dict.Translate(SymChk);
	g_Dict.Translate(SymCombo);
	g_Dict.Translate(SymSizeLab);
	g_Dict.Translate(SolidChk);

	g_Dict.Translate(GroupBox2);
	g_Dict.Translate(TextChk);
	g_Dict.Translate(FontBut);
	g_Dict.Translate(SolidBackChk);

	g_Dict.Translate(GroupBox3);

	g_Dict.Translate(GroupBox4);
	g_Dict.Translate(LatLab);
	g_Dict.Translate(LongLab);
	g_Dict.Translate(LockChk);
	g_Dict.Translate(RefChk);

	g_Dict.Translate(GroupBox5);
	g_Dict.Translate(ShowFrameChk);
	g_Dict.Translate(FrameLab1);
	g_Dict.Translate(FrameLab2);
	g_Dict.Translate(FrameLab3);
#endif

   pLock->Acquire();
   DlgState=NM_DIALOG_OPEN;
   pLock->Release();
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::TextChkClick(TObject *)
{
	TextMemo->Enabled=(TextChk->Checked && !bDefault) && !bEditSite;
	SolidBackChk->Enabled=(TextChk->Checked || bDefault);
	BackColPan->Enabled=(TextChk->Checked || bDefault);
	ShowFrameChk->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameColPan->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameWidthEd->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameWidthUD->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameLab1->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameLab2->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FrameLab3->Enabled=(TextChk->Checked || bDefault) && !bEditSite;
	FontBut->Enabled=(TextChk->Checked || bDefault);
	TLRad->Enabled=(TextChk->Checked || bDefault);
	TRad->Enabled=(TextChk->Checked || bDefault);
	TRRad->Enabled=(TextChk->Checked || bDefault);
	LRad->Enabled=(TextChk->Checked || bDefault);
	RRad->Enabled=(TextChk->Checked || bDefault);
	BLRad->Enabled=(TextChk->Checked || bDefault);
	BRad->Enabled=(TextChk->Checked || bDefault);
	BRRad->Enabled=(TextChk->Checked || bDefault);
	CRad->Enabled=(TextChk->Checked || bDefault);
	ShowFrameChkClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::SymColPanClick(TObject *)
{
   SymColPan->Color=SelectColor(L"Symbol Colour",SymColPan->Color,BackColor,pPrintColor);
}
//---------------------------------------------------------------------------


void __fastcall TNavSymbolForm::DelButClick(TObject *)
{
   if (bSubEdit)
      {
      if (bAskYes(L"Delete Symbol/Text ?",L"Symbol/Text"))
         {
         bDelete=true;
         ModalResult=mrOk;
         }
      }
   else
      {
      bDelete=true;
      pLock->Acquire();
      DlgState=NM_DIALOG_OK;
      pLock->Release();
      ModalResult=mrOk;
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::FormClose(TObject *,
      TCloseAction &Action)
{
   GetData();
   if (Action==Forms::caHide)
      {
      pLock->Acquire();
      DlgState=NM_DIALOG_CANCEL;
      pLock->Release();
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::GetData()
{
   nSymbol=SymCombo->ItemIndex;
}

void __fastcall TNavSymbolForm::FontButClick(TObject *)
{
   SelectFont(pFont,L"Select Text Font",BackColor,pPrintColor);
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::FormCreate(TObject *)
{
   pLock=new TCriticalSection();
   pFont=new TFont;
   bDefault=false;
   bDelete=true;
   bSubEdit=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::FormDestroy(TObject *)
{
   delete pFont;
   delete pLock;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::LockChkClick(TObject *)
{
   LatEd->Enabled=LockChk->Checked;
   LongEd->Enabled=LockChk->Checked;
   LatLab->Enabled=LockChk->Checked;
   LongLab->Enabled=LockChk->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::OKButClick(TObject *)
{
   GetData();
   pLock->Acquire();
   DlgState=NM_DIALOG_OK;
   pLock->Release();
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------


void __fastcall TNavSymbolForm::CancelButClick(TObject *)
{
   GetData();
   pLock->Acquire();
   DlgState=NM_DIALOG_CANCEL;
   pLock->Release();
   ModalResult=mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall TNavSymbolForm::BackColPanClick(TObject *)
{
	BackColPan->Color=SelectColor(sGet("Text Background Colour"),BackColPan->Color,BackColor,pPrintColor);
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::FrameColPanClick(TObject *)
{
   FrameColPan->Color=SelectColor(sGet("Text Frame Colour"),FrameColPan->Color,BackColor,pPrintColor);
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::ShowFrameChkClick(TObject *)
{
   FrameColPan->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   FrameWidthEd->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   FrameWidthUD->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   FrameLab1->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   FrameLab2->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   FrameLab3->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
   BackColPan->Enabled=ShowFrameChk->Checked && (TextChk->Checked || bDefault);
	SolidBackChkClick(0);
}
//---------------------------------------------------------------------------

String TNavSymbolForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}

void __fastcall TNavSymbolForm::CopyLatLongtoClipboard1Click(
      TObject *)
{
//   
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::PasteLatLongfromClipboard1Click(
      TObject *)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::RefChkClick(TObject *)
{
   if (RefChk->Checked)
      {
      LockChk->Checked=true;
      LockChk->Enabled=false;
      LockChkClick(0);
      }
   else
      LockChk->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavSymbolForm::SolidBackChkClick(TObject */*Sender*/)
{
	BackColPan->Enabled=ShowFrameChk->Checked && SolidBackChk->Checked;
}
//---------------------------------------------------------------------------

