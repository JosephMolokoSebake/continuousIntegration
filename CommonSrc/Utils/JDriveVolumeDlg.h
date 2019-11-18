//---------------------------------------------------------------------------

#ifndef JDriveVolumeDlgH
#define JDriveVolumeDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TDriveVolumeForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *VolLB;
	TBitBtn *OKBut;
	TBitBtn *CancelBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall VolLBDblClick(TObject *Sender);
	void __fastcall OKButClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDriveVolumeForm(TComponent* Owner);


   String sVolume;


};
//---------------------------------------------------------------------------
extern PACKAGE TDriveVolumeForm *DriveVolumeForm;
//---------------------------------------------------------------------------
#endif
