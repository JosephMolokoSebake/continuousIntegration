//---------------------------------------------------------------------------

#ifndef D2DNavArcSegDlgH
#define D2DNavArcSegDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.Buttons.hpp>
//#include <Psock.hpp>
//---------------------------------------------------------------------------
class TNavArcSegForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TEdit *LatEd;
   TEdit *LongEd;
   TLabel *Label1;
   TLabel *Label2;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TNavArcSegForm(TComponent* Owner);

JEdit Ed;

};
//---------------------------------------------------------------------------
extern PACKAGE TNavArcSegForm *NavArcSegForm;
//---------------------------------------------------------------------------
#endif
