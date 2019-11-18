//---------------------------------------------------------------------------

#ifndef JColorDlgH
#define JColorDlgH
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
class TColorForm : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TPaintBox *PaintBox;
   TLabel *PrintLab;
   TPanel *PrintPan;
	TGroupBox *GroupBox1;
	TTrackBar *RedTB;
	TCheckBox *UserChk;
	TTrackBar *GreenTB;
	TTrackBar *BlueTB;
	TPanel *UserPan;
	TLabel *RedLab;
	TLabel *GreenLab;
	TLabel *BlueLab;
	TEdit *RedEd;
	TEdit *GreenEd;
	TEdit *BlueEd;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall PaintBoxPaint(TObject *Sender);
   void __fastcall PrintPanClick(TObject *Sender);
	void __fastcall UserChkClick(TObject *Sender);
	void __fastcall RedTBChange(TObject *Sender);
	void __fastcall GreenTBChange(TObject *Sender);
	void __fastcall BlueTBChange(TObject *Sender);
private:	// User declarations

   JEdit Ed;

   TPaintBox* apPan[48];
   int nSelected;
   int nPrevSel;
   TColor aPrintColor[48];
	bool bShowPrint;
	bool bSettingTB;

   void __fastcall OnPanClick(TObject* Sender);
   
   void __fastcall OnPanDblClick(TObject* Sender);
   
   void __fastcall ShowSel(const int nSel);

public:		// User declarations
   __fastcall TColorForm(TComponent* Owner);

   TColor Color;
   TColor BackColor;

   void __fastcall MapColor(const int n, const TColor PrintCol)
   {
		aPrintColor[n]=PrintCol;
      bShowPrint=true;
	}

   TColor __fastcall GetMapColor(const int n) const
   {
      return aPrintColor[n];
   }

};

TColor SelectColor(  const String sTitle="",
                     const TColor Default=clWhite,
                     const TColor Back=clBtnFace,
                     TColor* pPrintCol=NULL);

//---------------------------------------------------------------------------
extern PACKAGE TColorForm *ColorForm;
//---------------------------------------------------------------------------
#endif
