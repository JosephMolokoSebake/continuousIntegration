//---------------------------------------------------------------------------

#ifndef MidnightCloseDlgH
#define MidnightCloseDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "JTime.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class TMidnightCloseForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TProgressBar *ProgBar;
	TBitBtn *CancelBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CancelButClick(TObject *Sender);
private:	// User declarations

   DWORD dwNextClose_loc_sec;


public:		// User declarations
	__fastcall TMidnightCloseForm(TComponent* Owner);


};

extern DWORD dwStartClose_loc_sec;

//void InitCloseForm(JTime T_loc, const DWORD dwTime_sec);
//bool bUpdateCloseForm(TComponent* Owner, const JTime T_loc);
//bool bUpdateCloseForm(TComponent* Owner, const DWORD dwT_loc_sec);

//---------------------------------------------------------------------------
extern PACKAGE TMidnightCloseForm *MidnightCloseForm;
//---------------------------------------------------------------------------
#endif
