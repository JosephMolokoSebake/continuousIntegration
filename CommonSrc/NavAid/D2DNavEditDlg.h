//---------------------------------------------------------------------------

#ifndef D2DNavEditDlgH
#define D2DNavEditDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Syncobjs.hpp>

#include "D2DNavAid.h"

//---------------------------------------------------------------------------
class TNavEditForm : public TForm
{
__published:	// IDE-managed Components
   TListBox *SymList;
   TLabel *Label1;
   TListBox *LineList;
   TLabel *Label2;
   TListBox *ArcList;
   TLabel *Label3;
   TEdit *DescrEd;
   TBevel *Bevel1;
   TButton *EditBut;
   TBitBtn *CancelBut;
   TPanel *ColPan;
   TButton *DelBut;
   TButton *UpBut;
   TButton *DownBut;
   TBitBtn *OKBut;
   TButton *AddSymbolBut;
   TButton *AddLineBut;
   TButton *AddFootBut;
   TBevel *Bevel2;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall SymListClick(TObject *Sender);
   void __fastcall SymListDblClick(TObject *Sender);
   void __fastcall LineListClick(TObject *Sender);
   void __fastcall LineListDblClick(TObject *Sender);
   void __fastcall ArcListClick(TObject *Sender);
   void __fastcall ArcListDblClick(TObject *Sender);
   void __fastcall EditButClick(TObject *Sender);
   void __fastcall UpButClick(TObject *Sender);
   void __fastcall DownButClick(TObject *Sender);
   void __fastcall DelButClick(TObject *Sender);
   void __fastcall OKButClick(TObject *Sender);
   void __fastcall CancelButClick(TObject *Sender);
   void __fastcall AddSymbolButClick(TObject *Sender);
   void __fastcall AddLineButClick(TObject *Sender);
   void __fastcall AddFootButClick(TObject *Sender);
private:	// User declarations

   NavAidClass Nav;
   TCriticalSection* pLock;
   NM_DIALOG_STATE DlgState;

   void __fastcall SetButtons();
   void __fastcall DeleteSymList();
   void __fastcall DeleteLineList();
	void __fastcall DeleteArcList();

	String sGet(const String s);

public:		// User declarations
   __fastcall TNavEditForm(TComponent* Owner);

   void __fastcall SetNav(NavAidClass& _Nav)
   {
      Nav=_Nav;
   }

   void __fastcall GetNav(NavAidClass& _Nav)
   {
      _Nav=Nav;
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
extern PACKAGE TNavEditForm *NavEditForm;
//---------------------------------------------------------------------------
#endif
