//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JFontDlg.h"
#include "JColorDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TFontForm *FontForm;

int CALLBACK g_EnumFontFamProc(

	 ENUMLOGFONT *lpelf,			// pointer to logical-font data
	 NEWTEXTMETRIC */*lpntm*/,		// pointer to physical-font data
	 int FontType,					// type of font
	 LPARAM lParam 				// address of application-defined data
   ){
   if ((FontType&TRUETYPE_FONTTYPE)&&(lpelf->elfLogFont.lfCharSet==ANSI_CHARSET)){
      TComboBox* pCombo=(TComboBox*)lParam;
      pCombo->Items->Add(String(lpelf->elfLogFont.lfFaceName));
      }
   return 1;
}

//---------------------------------------------------------------------------
__fastcall TFontForm::TFontForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFontForm::FormShow(TObject *)
{
	::EnumFontFamilies(Canvas->Handle,NULL,(FONTENUMPROC)g_EnumFontFamProc,(LPARAM)FontCombo);
   FontCombo->ItemIndex=FontCombo->Items->IndexOf(pFont->Name);
   if (FontCombo->ItemIndex<0) FontCombo->ItemIndex=0;
   BoldChk->Checked=pFont->Style.Contains(fsBold);
   ItalicChk->Checked=pFont->Style.Contains(fsItalic);
   StrikeChk->Checked=pFont->Style.Contains(fsStrikeOut);
   UnderChk->Checked=pFont->Style.Contains(fsUnderline);
   Ed.nSetVal(SizeEd,pFont->Size,"Font Size",1,99);
   SizeUD->Position=pFont->Size;
   ColPan->Color=pFont->Color;
   Caption=sTitle;
   OKBut->SetFocus();
   PaintBox->Color=BackColor;
	bSetup=false;

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);
		g_Dict.Translate(Label3);
		g_Dict.Translate(BoldChk);
		g_Dict.Translate(ItalicChk);
		g_Dict.Translate(StrikeChk);
		g_Dict.Translate(UnderChk);
	#endif


}
//---------------------------------------------------------------------------
void __fastcall TFontForm::FormClose(TObject *, TCloseAction &)
{
//
}
//---------------------------------------------------------------------------

String TFontForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}

void __fastcall TFontForm::FontComboChange(TObject *)
{
   pFont->Name=FontCombo->Items->Strings[FontCombo->ItemIndex];
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------


void __fastcall TFontForm::BoldChkClick(TObject *)
{
   TFontStyles Style=pFont->Style;
   if (BoldChk->Checked)
      Style=Style<<fsBold;
   else
      Style=Style>>fsBold;
   pFont->Style=Style;
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::ItalicChkClick(TObject *)
{
   TFontStyles Style=pFont->Style;
   if (ItalicChk->Checked)
      Style=Style<<fsItalic;
   else
      Style=Style>>fsItalic;
   pFont->Style=Style;
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::StrikeChkClick(TObject *)
{
   TFontStyles Style=pFont->Style;
   if (StrikeChk->Checked)
      Style=Style<<fsStrikeOut;
   else
      Style=Style>>fsStrikeOut;
   pFont->Style=Style;
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::UnderChkClick(TObject *)
{
   TFontStyles Style=pFont->Style;
   if (UnderChk->Checked)
      Style=Style<<fsUnderline;
   else
      Style=Style>>fsUnderline;
   pFont->Style=Style;
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::ColPanClick(TObject *)
{
	TColorForm* pForm=new TColorForm(0);
   pForm->Caption=sTitle;
   pForm->Color=pFont->Color;
   pForm->BackColor=BackColor;
   if (pPrintColor)
      for (int i=0; i<48; i++)
         pForm->MapColor(i,pPrintColor[i]);
   if (pForm->ShowModal()==mrOk){
      pFont->Color=pForm->Color;
      ColPan->Color=pForm->Color;
      if (pPrintColor){
         for (int i=0; i<48; i++)
            pPrintColor[i]=pForm->GetMapColor(i);
         }
      }
   delete pForm;
   PaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::SetFont(TFont* _pFont){
   pFont->Assign(_pFont);
}

void __fastcall TFontForm::GetFont(TFont* _pFont){
   _pFont->Assign(pFont);
}

void __fastcall TFontForm::FormCreate(TObject *)
{
   bSetup=true;
   pFont=new TFont;
   BackColor=clBtnFace;
   pPrintColor=NULL;
   sTitle=sGet("Select Font Colour");
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::FormDestroy(TObject *)
{
   delete pFont;
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::SizeEdChange(TObject *)
{
   if (!bSetup){
      pFont->Size=Ed.nGetVal(SizeEd);
      PaintBox->Repaint();
      }
}
//---------------------------------------------------------------------------

void __fastcall TFontForm::PaintBoxPaint(TObject *)
{
   PaintBox->Canvas->Brush->Color=BackColor;
   PaintBox->Canvas->FillRect(TRect(0,0,PaintBox->Width,PaintBox->Height));
   PaintBox->Canvas->Font->Assign(pFont);
   PaintBox->Canvas->TextOut(0,0,pFont->Name);
}
//---------------------------------------------------------------------------

void SelectFont(  TFont* pFont,
                  const String sTitle,
                  const TColor Back,
                  TColor* pPrintCol){
   TFontForm* pF=new TFontForm(0);
   pF->sTitle=sTitle;
   pF->BackColor=Back;
   pF->pPrintColor=pPrintCol;
   pF->SetFont(pFont);
   if (pF->ShowModal()==mrOk)
      pF->GetFont(pFont);
   delete pF;
}

