//---------------------------------------------------------------------------

#ifndef D2DNavArcDlgH
#define D2DNavArcDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Syncobjs.hpp>

#include "D2DNavAid.h"
#include "JList.h"
#include "JEdit.h"

//---------------------------------------------------------------------------
class TNavArcForm : public TForm
{
__published:	// IDE-managed Components
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TGroupBox *FirePosGrp;
   TGroupBox *GroupBox2;
   TLabel *ArcLab3;
   TLabel *ArcLab5;
   TLabel *ArcLab1;
   TLabel *ArcLab2;
   TLabel *ArcLab7;
   TLabel *ArcLab9;
   TEdit *LatEd;
   TEdit *LongEd;
   TEdit *FRaEd;
   TEdit *ARaEd;
   TEdit *BeEd;
   TEdit *ArcEd;
   TLabel *ArcLab4;
   TLabel *ArcLab6;
   TLabel *ArcLab8;
   TLabel *ArcLab10;
   TGroupBox *GroupBox3;
   TLabel *Label11;
   TLabel *Label12;
   TLabel *FillLab;
   TCheckBox *FillChk;
   TPanel *FramePan;
   TEdit *WidthEd;
   TUpDown *WidthUD;
   TPanel *FillPan;
   TBevel *Bevel1;
   TButton *DelBut;
   TEdit *DescrEd;
   TLabel *Label13;
   TCheckBox *DispChk;
   TGroupBox *GroupBox4;
   TRadioButton *ArcRad;
   TRadioButton *SegRad;
   TGroupBox *GroupBox5;
   TListBox *SegList;
   TButton *AddSegBut;
   TButton *DelSegBut;
   TButton *UpBut;
   TButton *DownBut;
   TButton *EditSegBut;
   TRadioButton *CircleRad;
   TGroupBox *CircleGrp;
   TLabel *CircleLab1;
   TEdit *CircleEd;
   TLabel *CircleLab2;
   TCheckBox *CircleDotChk;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FramePanClick(TObject *Sender);
   void __fastcall FillChkClick(TObject *Sender);
   void __fastcall FillPanClick(TObject *Sender);
   void __fastcall DelButClick(TObject *Sender);
   void __fastcall OKButClick(TObject *Sender);
   void __fastcall CancelButClick(TObject *Sender);
   void __fastcall ArcRadClick(TObject *Sender);
   void __fastcall SegRadClick(TObject *Sender);
   void __fastcall SegListClick(TObject *Sender);
   void __fastcall AddSegButClick(TObject *Sender);
   void __fastcall DelSegButClick(TObject *Sender);
   void __fastcall UpButClick(TObject *Sender);
   void __fastcall DownButClick(TObject *Sender);
   void __fastcall SegListDblClick(TObject *Sender);
   void __fastcall EditSegButClick(TObject *Sender);
   void __fastcall CircleRadClick(TObject *Sender);
   void __fastcall RoseRadClick(TObject *Sender);
private:	// User declarations


   TColor BackColor;
   TColor* pPrintColor;
   TCriticalSection* pLock;
   NM_DIALOG_STATE DlgState;

public:		// User declarations
   __fastcall TNavArcForm(TComponent* Owner);

   JEdit Ed;

   bool bDeleted;
   bool bSubEdit;
   JList<LatLong> Seg;
   bool bShowDel;

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

};
//---------------------------------------------------------------------------
extern PACKAGE TNavArcForm *NavArcForm;
//---------------------------------------------------------------------------
#endif
