//---------------------------------------------------------------------------

#ifndef Cst4SiteInfoDlgH
#define Cst4SiteInfoDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TCst4SiteInfoForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo;
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TCst4SiteInfoForm(TComponent* Owner);

   int nID;

};
//---------------------------------------------------------------------------
extern PACKAGE TCst4SiteInfoForm *Cst4SiteInfoForm;
//---------------------------------------------------------------------------
#endif
