//---------------------------------------------------------------------------

#ifndef JAutoStartDlgH
#define JAutoStartDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAutoStartForm : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer;
	TPanel *Panel1;
   TProgressBar *Bar;
   TBitBtn *CancelBut;
   TButton *SkipBut;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall SkipButClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TAutoStartForm(TComponent* Owner);

   int nTotalTimeSec;
};

bool bAutoStart(const String sCaption="Automatic Start", const int nDelay_sec=5);

//---------------------------------------------------------------------------
extern PACKAGE TAutoStartForm *AutoStartForm;
//---------------------------------------------------------------------------
#endif
