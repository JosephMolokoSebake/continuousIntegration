//---------------------------------------------------------------------------

#ifndef JMapDisplayDlgH
#define JMapDisplayDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMapDisplayForm : public TForm
{
__published:	// IDE-managed Components
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TPanel *Panel;
   TButton *ResetBut;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ResetButClick(TObject *Sender);
private:	// User declarations
   TPanel* apDisp[48];
   TPanel* apPrint[48];
   TLabel* apLab[48];

   TBevel* apBev[5];

   TColor aDispCol[48];
   TColor aPrintCol[48];

   void __fastcall OnPrintClick(TObject* Sender);

public:		// User declarations
   __fastcall TMapDisplayForm(TComponent* Owner);

   void __fastcall MapColor(const int n, const TColor DispCol, const TColor PrintCol);
   TColor __fastcall GetMapColor(const int n);

};
//---------------------------------------------------------------------------
extern PACKAGE TMapDisplayForm *MapDisplayForm;
//---------------------------------------------------------------------------
#endif
