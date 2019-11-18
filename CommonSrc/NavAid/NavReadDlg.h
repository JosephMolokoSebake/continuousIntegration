//---------------------------------------------------------------------------

#ifndef NavReadDlgH
#define NavReadDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TNavReadForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TEdit *CTextEd;
   TEdit *NTextEd;
   TLabel *Label2;
   TRadioButton *CurSymRad;
   TRadioButton *NewSymRad;
   TGroupBox *GroupBox2;
   TLabel *Label3;
   TLabel *Label4;
   TEdit *CLineEd;
   TEdit *NLineEd;
   TRadioButton *CurLineRad;
   TRadioButton *NewLineRad;
   TGroupBox *GroupBox3;
   TGroupBox *GroupBox4;
   TLabel *Label5;
   TLabel *Label6;
   TLabel *Label7;
   TEdit *CLatEd;
   TEdit *CLongEd;
   TEdit *CSizeEd;
   TGroupBox *GroupBox5;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TEdit *NLatEd;
   TEdit *NLongEd;
   TEdit *NSizeEd;
   TCheckBox *ViewChk;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations

   JEdit Ed;

   __fastcall TNavReadForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNavReadForm *NavReadForm;
//---------------------------------------------------------------------------
#endif
