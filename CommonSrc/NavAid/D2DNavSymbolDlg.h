//---------------------------------------------------------------------------
#ifndef D2DNavSymbolDlgH
#define D2DNavSymbolDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Syncobjs.hpp>

#include "D2DNavAid.h"
#include <Vcl.Menus.hpp>

//---------------------------------------------------------------------------
class TNavSymbolForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TComboBox *SymCombo;
   TLabel *SymSizeLab;
   TPanel *SymColPan;
   TGroupBox *GroupBox2;
   TCheckBox *SymChk;
   TBevel *Bevel1;
   TCheckBox *TextChk;
   TBevel *Bevel2;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TButton *DelBut;
   TGroupBox *GroupBox3;
   TRadioButton *TLRad;
   TRadioButton *TRad;
   TRadioButton *LRad;
   TRadioButton *BLRad;
   TRadioButton *BRad;
   TRadioButton *TRRad;
   TRadioButton *BRRad;
   TRadioButton *RRad;
   TRadioButton *CRad;
   TBevel *Bevel3;
   TCheckBox *SolidChk;
   TGroupBox *GroupBox4;
   TLabel *LatLab;
   TLabel *LongLab;
   TUpDown *SymSizeUD;
   TButton *FontBut;
   TCheckBox *LockChk;
   TCheckBox *DispChk;
   TGroupBox *GroupBox5;
   TLabel *FrameLab1;
   TPanel *FrameColPan;
   TUpDown *FrameWidthUD;
   TMemo *TextMemo;
   TCheckBox *SolidBackChk;
   TCheckBox *ShowFrameChk;
   TPanel *BackColPan;
   TLabel *FrameLab2;
   TLabel *FrameLab3;
   TCheckBox *RefChk;
	TEdit *SymSizeEd;
	TEdit *LatEd;
	TEdit *LongEd;
	TEdit *FrameWidthEd;
   void __fastcall SymChkClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TextChkClick(TObject *Sender);
   void __fastcall SymColPanClick(TObject *Sender);
   void __fastcall DelButClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FontButClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall LockChkClick(TObject *Sender);
   void __fastcall OKButClick(TObject *Sender);
   void __fastcall CancelButClick(TObject *Sender);
   void __fastcall BackColPanClick(TObject *Sender);
   void __fastcall FrameColPanClick(TObject *Sender);
   void __fastcall ShowFrameChkClick(TObject *Sender);
   void __fastcall CopyLatLongtoClipboard1Click(TObject *Sender);
   void __fastcall PasteLatLongfromClipboard1Click(TObject *Sender);
   void __fastcall RefChkClick(TObject *Sender);
	void __fastcall SolidBackChkClick(TObject *Sender);

private:	// User declarations

   TColor BackColor;
   TColor* pPrintColor;
   TCriticalSection* pLock;
   NM_DIALOG_STATE DlgState;

	void __fastcall GetData();

	String sGet(const String s);

public:		// User declarations
   __fastcall TNavSymbolForm(TComponent* Owner);

   TFont* pFont;
   int nSymbol;
   bool bDefault;
   bool bDelete;
   bool bSubEdit;
   bool bShowDel;
   bool bEditSite;

   void SetColors(const TColor Back, TColor* pPrint)
   {
      BackColor=Back;
      pPrintColor=pPrint;
   }

   NM_DIALOG_STATE GetState()
   {
      NM_DIALOG_STATE State;
      pLock->Acquire();
      State=DlgState;
      pLock->Release();
      return State;
   }

   JEdit Ed;

};
//---------------------------------------------------------------------------
extern PACKAGE TNavSymbolForm *NavSymbolForm;
//---------------------------------------------------------------------------
#endif
