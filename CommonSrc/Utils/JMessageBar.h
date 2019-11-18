//---------------------------------------------------------------------------

#ifndef JMessageBarH
#define JMessageBarH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TMessageBarForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TProgressBar *ProgressBar;
   TBitBtn *AbortBut;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall AbortButClick(TObject *Sender);
private:	// User declarations
	int nPrev;
public:		// User declarations
	__fastcall TMessageBarForm(TComponent* Owner);

   void __fastcall SetCaption(const String sCapt, const bool bShowAbort=false);

   void __fastcall SetProgress(const int n, const int nMax=0);

   TProgressBar* pGetBar()
   {
      return ProgressBar;
   }

	bool bAbort;

	bool bProcessMessages;

};
//---------------------------------------------------------------------------
extern PACKAGE TMessageBarForm *MessageBarForm;
//---------------------------------------------------------------------------
#endif
