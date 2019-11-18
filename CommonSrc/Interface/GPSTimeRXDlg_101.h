//---------------------------------------------------------------------------

#ifndef GPSTimeRXDlg_101H
#define GPSTimeRXDlg_101H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "JEdit.h"

#include "CommsTimeDistr_101.h"
#include "NMEADecode.h"

//---------------------------------------------------------------------------
class TGPSTimeRX101Form : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer;
   TEdit *TimeEd;
   TLabel *Label3;
   TEdit *LatEd;
   TLabel *Label4;
   TEdit *LongEd;
   TPanel *ClkPan;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *ZDAMsgEd;
   TEdit *CRCEd;
   TBevel *Bevel1;
   TLabel *Label5;
   TEdit *HeadEd;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *Label11;
   TBevel *Bevel2;
   TEdit *SatEd;
   TEdit *AltEd;
   TEdit *UTCTimeEd;
   TEdit *HDOPEd;
   TLabel *Label12;
   TEdit *DiffEd;
   TBevel *Bevel3;
   TButton *UpdateTimeBut;
   TLabel *Label13;
   TEdit *POSMsgEd;
   TLabel *Label14;
   TEdit *UTCDateEd;
   TLabel *Label15;
   TLabel *Label16;
   TLabel *Label17;
   TEdit *CourseEd;
   TEdit *SpeedEd;
   TEdit *VertVelEd;
   TLabel *Label18;
   TLabel *Label19;
   TLabel *Label20;
   TLabel *Label21;
   TEdit *PDOPEd;
   TLabel *Label22;
   TEdit *VDOPEd;
   TLabel *Label23;
   TEdit *TDOPEd;
   TLabel *Label24;
   TEdit *TimeCRCEd;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall UpdateTimeButClick(TObject *Sender);
private:	// User declarations

	JEdit Ed;

   CommsTimeDistr_101* pComms;

   int nGPSCount;

   int nMsgCount;
   int nCRCErrors;

   bool bExtComms;

   NMEADecode HeadingDecode;

   int nHeadMissed;

   double __fastcall dReadHeading();


public:		// User declarations
   __fastcall TGPSTimeRX101Form(TComponent* Owner);

   void SetComms(CommsTimeDistr_101* _pComms)
   {
      pComms=_pComms;
      bExtComms=true;
   }

   int nHeadingBaud;
   TIMEDISTR_UART_CHAN HeadingChan;

};
//---------------------------------------------------------------------------
extern PACKAGE TGPSTimeRX101Form *GPSTimeRX101Form;
//---------------------------------------------------------------------------
#endif
