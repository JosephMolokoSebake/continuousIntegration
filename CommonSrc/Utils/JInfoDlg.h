//---------------------------------------------------------------------------

#ifndef JInfoDlgH
#define JInfoDlgH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TJInfoForm : public TForm
{
__published:	// IDE-managed Components
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TProgressBar *PB;
   TBevel *Bevel1;
   TLabel *InfoLab;
   TTimer *Timer;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TimerTimer(TObject *Sender);
private:	// User declarations

   String sI;
   int nSec;
   bool bAsk;
   bool bDefaultYes;

public:		// User declarations
   __fastcall TJInfoForm(TComponent* Owner);

	void SetInfo(const String sInfo, const String sHeader="Information", const int nTimeOut_sec=-1);

   void SetAsk(const bool _bAsk, const bool _bDefaultYes=false)
   {
      bAsk=_bAsk;
      bDefaultYes=_bDefaultYes;
   }

};
//---------------------------------------------------------------------------
extern PACKAGE TJInfoForm *JInfoForm;
//---------------------------------------------------------------------------
#endif
