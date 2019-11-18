//---------------------------------------------------------------------------
#ifndef JHelpAboutDlgH
#define JHelpAboutDlgH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TJHelpAboutForm : public TForm
{
__published:	// IDE-managed Components
   TPanel *MainPan;
   TLabel *DescrLab;
   TBevel *MainBev;
   TLabel *Label1;
   TEdit *VerEd;
   TLabel *Label3;
   TEdit *BuildEd;
   TLabel *Label4;
   TEdit *CopyEd;
   TLabel *Label5;
   TEdit *RefEd;
   TLabel *DebugLab;
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations

	String sAltDescr;

public:		// User declarations
   __fastcall TJHelpAboutForm(TComponent* Owner);

	void SetAltDescr(const String sDescr)
	{
		sAltDescr=sDescr;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TJHelpAboutForm *JHelpAboutForm;
//---------------------------------------------------------------------------

void ShowHelpAbout(TComponent* pOwner);

#endif
