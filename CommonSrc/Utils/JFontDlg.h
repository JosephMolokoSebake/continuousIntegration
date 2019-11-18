//---------------------------------------------------------------------------

#ifndef JFontDlgH
#define JFontDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "JEdit.h"
//---------------------------------------------------------------------------
class TFontForm : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TComboBox *FontCombo;
   TLabel *Label1;
   TGroupBox *GroupBox1;
   TCheckBox *BoldChk;
   TCheckBox *ItalicChk;
   TCheckBox *StrikeChk;
   TCheckBox *UnderChk;
   TLabel *Label2;
   TLabel *Label3;
   TEdit *SizeEd;
   TUpDown *SizeUD;
   TPanel *ColPan;
   TBevel *Bevel1;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TPaintBox *PaintBox;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FontComboChange(TObject *Sender);
   void __fastcall BoldChkClick(TObject *Sender);
   void __fastcall ItalicChkClick(TObject *Sender);
   void __fastcall StrikeChkClick(TObject *Sender);
   void __fastcall UnderChkClick(TObject *Sender);
   void __fastcall ColPanClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall SizeEdChange(TObject *Sender);
   void __fastcall PaintBoxPaint(TObject *Sender);
private:	// User declarations

   JEdit Ed;

   TFont* pFont;
	bool bSetup;

	String sGet(const String s);


public:		// User declarations
   __fastcall TFontForm(TComponent* Owner);

   void __fastcall SetFont(TFont* _pFont);
   void __fastcall GetFont(TFont* _pFont);

   TColor BackColor;
   TColor* pPrintColor;
   String sTitle;

};

void SelectFont(  TFont* pFont,
                  const String sTitle=L"",
                  const TColor Back=clBtnFace,
                  TColor* pPrintCol=NULL);

//---------------------------------------------------------------------------
extern PACKAGE TFontForm *FontForm;
//---------------------------------------------------------------------------
#endif
