//---------------------------------------------------------------------------

#ifndef GPSTimeRXDlgH
#define GPSTimeRXDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "JEdit.h"

#include "CommsTimeDistr_200.h"
#include "NMEADecode.h"

//---------------------------------------------------------------------------
class TGPSTimeRX200Form : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer;
   TEdit *TimeEd;
   TLabel *Label3;
   TEdit *LatEd;
   TLabel *Label4;
   TEdit *LongEd;
   TPanel *ClkPan;
   TLabel *Label2;
   TEdit *CRCEd;
   TBevel *Bevel1;
   TLabel *Label5;
   TEdit *HeadEd;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *Label11;
   TBevel *Bevel2;
   TEdit *SatEd;
   TEdit *AltEd;
   TEdit *HDOPEd;
   TLabel *Label12;
   TEdit *DiffEd;
   TButton *UpdateTimeBut;
   TLabel *Label24;
   TEdit *TimeCRCEd;
   TEdit *DateEd;
   TLabel *Label1;
   TEdit *GGAMsgEd;
	TLabel *Label8;
	TLabel *Label13;
	TEdit *TimeBiasEd;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall UpdateTimeButClick(TObject *Sender);
private:	// User declarations

	JEdit Ed;

   CommsTimeDistr_200* pComms;

   int nGPSCount;

   int nMsgCount;
   int nCRCErrors;

   bool bExtComms;

   NMEADecode HeadingDecode;

   int nHeadMissed;

   double __fastcall dReadHeading();


public:		// User declarations
   __fastcall TGPSTimeRX200Form(TComponent* Owner);

   void SetComms(CommsTimeDistr_200* _pComms)
   {
      pComms=_pComms;
      bExtComms=true;
   }

   int nHeadingBaud;
   TIMEDISTR_UART_CHAN HeadingChan;

};
//---------------------------------------------------------------------------
extern PACKAGE TGPSTimeRX200Form *GPSTimeRX200Form;
//---------------------------------------------------------------------------
#endif
