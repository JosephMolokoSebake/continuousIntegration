//---------------------------------------------------------------------------
#ifndef D2DNavLineDlgH
#define D2DNavLineDlgH
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
#include "JList.h"

//---------------------------------------------------------------------------
class TNavLineForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TLabel *Lat1Lab;
   TLabel *Long1Lab;
   TBevel *Bevel1;
   TLabel *Head1Lab;
   TButton *DeleteBut;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TGroupBox *GroupBox2;
   TLabel *Label4;
   TEdit *WidthEd;
   TLabel *Label5;
   TComboBox *StyleCombo;
   TPanel *ColorPan;
   TGroupBox *GroupBox3;
   TLabel *Lat2Lab;
   TLabel *Long2Lab;
   TBevel *Bevel2;
   TCheckBox *Arrow2Chk;
   TLabel *Head2Lab;
   TCheckBox *Arrow1Chk;
   TUpDown *Head1UD;
   TUpDown *Head2UD;
   TUpDown *WidthUD;
   TCheckBox *LockChk;
   TCheckBox *DispChk;
   TGroupBox *GroupBox5;
   TListBox *SegList;
   TButton *AddSegBut;
   TButton *DelSegBut;
   TButton *UpBut;
   TButton *DownBut;
   TButton *EditSegBut;
   TButton *ImportSegBut;
   TGroupBox *GroupBox4;
   TRadioButton *SingleRad;
   TRadioButton *MultiRad;
	TEdit *Lat1Ed;
	TEdit *Long1Ed;
	TEdit *Head1Ed;
	TEdit *Head2Ed;
	TEdit *Long2Ed;
	TEdit *Lat2Ed;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall Arrow1ChkClick(TObject *Sender);
   void __fastcall Arrow2ChkClick(TObject *Sender);
   void __fastcall ColorPanClick(TObject *Sender);
   void __fastcall DeleteButClick(TObject *Sender);
   void __fastcall WidthEdChange(TObject *Sender);
   void __fastcall StyleComboChange(TObject *Sender);
   void __fastcall Lat1EdClick(TObject *Sender);
   void __fastcall Long1EdClick(TObject *Sender);
   void __fastcall Lat2EdChange(TObject *Sender);
   void __fastcall Long2EdChange(TObject *Sender);
   void __fastcall OKButClick(TObject *Sender);
   void __fastcall CancelButClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall SingleRadClick(TObject *Sender);
   void __fastcall MultiRadClick(TObject *Sender);
   void __fastcall AddSegButClick(TObject *Sender);
   void __fastcall EditSegButClick(TObject *Sender);
   void __fastcall DelSegButClick(TObject *Sender);
   void __fastcall UpButClick(TObject *Sender);
   void __fastcall DownButClick(TObject *Sender);
   void __fastcall ImportSegButClick(TObject *Sender);
   void __fastcall SegListClick(TObject *Sender);
   void __fastcall SegListDblClick(TObject *Sender);
private:	// User declarations

   bool abPosChanged[2];
   TColor BackColor;
   TColor* pPrintColor;
   TCriticalSection* pLock;
	NM_DIALOG_STATE DlgState;

	String sGet(const String s);

public:		// User declarations
   __fastcall TNavLineForm(TComponent* Owner);

   JEdit Ed;

   int nStyle;
   bool bDeleted;
   bool bDefault;
   int nSide;
   bool bSubEdit;
   bool bShowDel;

   JList<LatLong> Seg;

   bool bGetPosChanged(const int n) const{
      return abPosChanged[n];
   }

   void SetColors(const TColor Back, TColor* pPrint){
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

};
//---------------------------------------------------------------------------
extern PACKAGE TNavLineForm *NavLineForm;
//---------------------------------------------------------------------------
#endif
