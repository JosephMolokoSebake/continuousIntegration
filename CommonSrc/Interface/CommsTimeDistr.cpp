//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "CommsTimeDistr.h"
#include "GPSTimeRXDlg_101.h"
#include "GPSTimeRXDlg_200.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)

void CommsTimeDistr::ShowGPSStatus(TComponent* Owner, const int nHeadBaud, const TIMEDISTR_UART_CHAN HeadChan)
{
   if (p101)
      {
      GPSTimeRX101Form=new TGPSTimeRX101Form(Owner);
      GPSTimeRX101Form->nHeadingBaud=nHeadBaud;
      GPSTimeRX101Form->HeadingChan=HeadChan;
      GPSTimeRX101Form->SetComms(p101);
      GPSTimeRX101Form->ShowModal();
      delete GPSTimeRX101Form;
      }
   else if (p200)
      {
      GPSTimeRX200Form=new TGPSTimeRX200Form(Owner);
      GPSTimeRX200Form->nHeadingBaud=nHeadBaud;
      GPSTimeRX200Form->HeadingChan=HeadChan;
      GPSTimeRX200Form->SetComms(p200);
      GPSTimeRX200Form->ShowModal();
      delete GPSTimeRX200Form;
      }
}

