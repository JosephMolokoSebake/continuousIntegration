//---------------------------------------------------------------------------

#ifndef NMEADlgH
#define NMEADlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include "NumEdit.h"
#include "NMEA_RX.h"
#include "USB_NMEA.h"
#include "CommsTimeDistr_101.h"
#include "JTime.h"
#include "NMEADecode.h"

//---------------------------------------------------------------------------
class TNMEAForm : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer;
   TMemo *Memo;
   TButton *StoreBut;
   TButton *ClearBut;
   TBitBtn *CloseBut;
   JNumEdit *SentenceEd;
   TLabel *Label1;
   TLabel *Label2;
   JNumEdit *CheckEd;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ClearButClick(TObject *Sender);
   void __fastcall StoreButClick(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
private:	// User declarations

   NMEA_RX* pCom;
   USB_NMEA* pUsb;
   CommsTimeDistr_101* pCommsTD;

   bool bStopTimer;

   NMEADecode Decode;


   JFile* pRawFil;

   String sRX;

public:		// User declarations
   __fastcall TNMEAForm(TComponent* Owner);

   void __fastcall SetInterface(NMEA_RX* _pCom)
   {
      pCom=_pCom;
   }

   void __fastcall SetInterface(USB_NMEA* _pUsb)
   {
      pUsb=_pUsb;
   }

   void __fastcall SetInterface(CommsTimeDistr_101* _pCommsTD)
   {
      pCommsTD=_pCommsTD;
   }

   bool bAllText;

};
//---------------------------------------------------------------------------
extern PACKAGE TNMEAForm *NMEAForm;
//---------------------------------------------------------------------------
#endif
