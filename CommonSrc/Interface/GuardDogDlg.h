//---------------------------------------------------------------------------

#ifndef GuardDogDlgH
#define GuardDogDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "NumEdit.h"

#include "GuardDog_Int.h"
#include "JFile.h"

//---------------------------------------------------------------------------
class TGuardDogForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *GPSTimer;
	JNumEdit *SerialEd;
	TLabel *Label1;
	TGroupBox *GroupBox1;
	TButton *GPSBut;
	TButton *StopGPSBut;
	TGroupBox *GroupBox2;
	TLabel *Label2;
	TLabel *Label3;
	TButton *EnWatch0But;
	TButton *DisWatch0But;
	JNumEdit *WatchTime0Ed;
	JNumEdit *WatchLeft0Ed;
	TLabel *Label4;
	TLabel *Label5;
	TButton *Trig0But;
	TTimer *WatchTimer;
	TEdit *Status0Ed;
	TEdit *GPSTimeEd;
	TEdit *GPSDateEd;
	TEdit *GPSLatEd;
	TEdit *GPSLongEd;
	TEdit *GPSHeightEd;
	TLabel *Label7;
	JNumEdit *GPSMsgCntEd;
	TPanel *TimePan;
	TPanel *DatePan;
	TPanel *LeapPan;
	TPanel *GSMPan;
	TGroupBox *GroupBox4;
	TLabel *Label6;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TButton *EnWatch1But;
	TButton *DisWatch1But;
	JNumEdit *WatchTime1Ed;
	JNumEdit *WatchLeft1Ed;
	TButton *Trig1But;
	TEdit *Status1Ed;
	TGroupBox *GroupBox5;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TButton *EnWatch2But;
	TButton *DisWatch2But;
	JNumEdit *WatchTime2Ed;
	JNumEdit *WatchLeft2Ed;
	TButton *Trig2But;
	TEdit *Status2Ed;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall GPSButClick(TObject *Sender);
	void __fastcall GPSTimerTimer(TObject *Sender);
	void __fastcall StopGPSButClick(TObject *Sender);
	void __fastcall EnWatch0ButClick(TObject *Sender);
	void __fastcall DisWatch0ButClick(TObject *Sender);
	void __fastcall Trig0ButClick(TObject *Sender);
	void __fastcall WatchTimerTimer(TObject *Sender);
	void __fastcall USBRadClick(TObject *Sender);
	void __fastcall RS232RadClick(TObject *Sender);
	void __fastcall RS422RadClick(TObject *Sender);
	void __fastcall EnWatch1ButClick(TObject *Sender);
	void __fastcall EnWatch2ButClick(TObject *Sender);
	void __fastcall DisWatch1ButClick(TObject *Sender);
	void __fastcall DisWatch2ButClick(TObject *Sender);
	void __fastcall Trig1ButClick(TObject *Sender);
	void __fastcall Trig2ButClick(TObject *Sender);
private:	// User declarations

	GuardDog_Int* pGuard;
	String sGPSLine;

	JFile* pGPSFil;

	bool abWatchEn[3];
	int anDelay_sec[3];

//	void EnableHardware();

	void CheckWatchTimer()
	{
		WatchTimer->Enabled=abWatchEn[0] || abWatchEn[1] || abWatchEn[2];
   }

public:		// User declarations
	__fastcall TGuardDogForm(TComponent* Owner);

	void SetInterface(GuardDog_Int* _pG)
	{
		pGuard=_pG;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TGuardDogForm *GuardDogForm;
//---------------------------------------------------------------------------
#endif
