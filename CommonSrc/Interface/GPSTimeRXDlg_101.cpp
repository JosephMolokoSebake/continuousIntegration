//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "GPSTimeRXDlg_101.h"
#include "GPSTime.h"
#include "JCommaText.h"
#include "JTransform.h"
#include "Config.h"
#include "JUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TGPSTimeRX101Form *GPSTimeRX101Form;
//---------------------------------------------------------------------------
__fastcall TGPSTimeRX101Form::TGPSTimeRX101Form(TComponent* Owner)
   : TForm(Owner)
{
   bExtComms=false;
   pComms=NULL;
   nHeadingBaud=0;
}
//---------------------------------------------------------------------------
void __fastcall TGPSTimeRX101Form::FormShow(TObject * /*Sender*/)
{
   ClkPan->SetFocus();
   if (!bExtComms)
      {
      pComms=new CommsTimeDistr_101();
      if (!pComms->bOK())
         {
         delete pComms;
         pComms=NULL;
         }
      }
   if (pComms)
      {
      pComms->StartTDCapt();
      if (nHeadingBaud>0)
         {
         pComms->SetupUART(nHeadingBaud,HeadingChan);
         pComms->StartUART();
         }
      }
   HeadEd->Text=L"N/A";
   nMsgCount=0;
   nCRCErrors=0;
   nHeadMissed=0;
   UpdateTimeBut->Enabled=false;
   Timer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TGPSTimeRX101Form::FormClose(TObject * /*Sender*/,
      TCloseAction & /*Action*/)
{
   if (!bExtComms)
      {
      if (pComms)
         {
         pComms->StopTDCapt();
         if (nHeadingBaud>0)
            pComms->StopUART();
         }
      delete pComms;
      pComms=NULL;
      }
   Timer->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TGPSTimeRX101Form::TimerTimer(TObject * /*Sender*/)
{
   if (pComms)
      {

      if (pComms->bTDClockPresent())
         {
         ClkPan->Color=clLime;
         ClkPan->Caption=L"HDLC Clock Present";
         }
      else
         {
         ClkPan->Color=clRed;
         ClkPan->Caption=L"HDLC Clock Missing";
         }
      JTime T=pComms->ReadTime();
      if (T.bValid())
         {
         TimeEd->Text=T.sTime(true);
         UpdateTimeBut->Enabled=true;
         }
      else
         {
         TimeEd->Text=L"";
         UpdateTimeBut->Enabled=false;
         }

      Application->ProcessMessages();
      Ed.SetValRO(TimeCRCEd,pComms->nReadTimeCRCErrors());
      pComms->nReadTDMessages();
      POSDataStruct POSData=pComms->GetPOSData();
      if (POSData.bValid())
         {
         Ed.SetValRO(POSMsgEd,pComms->nGetPOSMessages());
         LatEd->Text=POSData.LL.sLat();
         LongEd->Text=POSData.LL.sLong();
         Ed.SetValRO(SatEd,POSData.nSat);
         Ed.SetTimeRO(UTCTimeEd,POSData.T);
         if (POSData.bDiff)
            DiffEd->Text=L"YES";
         else
            DiffEd->Text=L"NO";

         Ed.SetValRO(CourseEd,POSData.dCourse,1);
         Ed.SetValRO(SpeedEd,POSData.dSpeed,1);
         Ed.SetValRO(VertVelEd,POSData.dVertVel,1);

         Ed.SetValRO(AltEd,POSData.dAlt,1);
         Ed.SetValRO(PDOPEd,POSData.dPDOP,2);
         Ed.SetValRO(HDOPEd,POSData.dHDOP,2);
         Ed.SetValRO(VDOPEd,POSData.dVDOP,2);
         Ed.SetValRO(TDOPEd,POSData.dTDOP,2);

         }
      else
         {
         LatEd->Text=L"";
         LongEd->Text=L"";
         SatEd->Text=L"";
         UTCTimeEd->Text=L"";
         DiffEd->Text=L"";

         AltEd->Text=L"";
         CourseEd->Text=L"";
         SpeedEd->Text=L"";
         VertVelEd->Text=L"";

         PDOPEd->Text=L"";
         HDOPEd->Text=L"";
         VDOPEd->Text=L"";
         TDOPEd->Text=L"";
         }

      JTime ZDATime=pComms->GetZDATime();
      if (ZDATime.bValid())
         {
         Ed.SetValRO(ZDAMsgEd,pComms->nGetZDAMessages());
         Ed.SetDateRO(UTCDateEd,ZDATime);
         }
      else
         {
         ZDAMsgEd->Text=L"";
         UTCDateEd->Text=L"";
         }
      static int nPrevErr=-1;
      int nE=pComms->nGetTDCRCErrors();
      if (nE!=nPrevErr)
         {
         Ed.SetValRO(CRCEd,nE);
         nPrevErr=nE;
         }
//      Edit1->Text=pComms->sGetPOS();
      if (nHeadingBaud>0)
         {
         double dHeading=dReadHeading();
         if (dHeading>=0.0)
            {
            Ed.SetValRO(HeadEd,dHeading,1);
            nHeadMissed=0;
            }
         else
            {
            ++nHeadMissed;
            if (nHeadMissed>=20)
               {
               nHeadMissed=0;
               HeadEd->Text=L"N/A";
               }
            }
         }
      }
}
//---------------------------------------------------------------------------

double __fastcall TGPSTimeRX101Form::dReadHeading()
{
   double dHeading=-1.0;
   if (pComms)
      {
      BYTE aucData[8192];
      int nSize=pComms->nRXUARTData(aucData,8192);
      if (nSize>0)
         {
         for (int i=0; i<nSize; i++)
            HeadingDecode.AddByte(aucData[i]);
         String s;
         CommaText Txt;
         double dV;
         do
            {
            s=HeadingDecode.sGetSentence();
            if (!s.IsEmpty())
               {
               Txt.SetText(s);
               if (Txt.nGetNum()==3)
                  {
                  if (Txt.sGetPar(0)==L"HEHDT")
                     {
                     try
                        {
                        dV=Txt.dGetVal(1);
                        }
                     catch(...)
                        {
                        dV=-1.0;
                        }
                     if (dV>=0.0)
                        dHeading=dV;
                     }
                  }

               }
            }
         while(!s.IsEmpty());
         }
      }
   return dHeading;
}


void __fastcall TGPSTimeRX101Form::UpdateTimeButClick(TObject * /*Sender*/)
{
   JTime T;
   if (pComms)
      T=pComms->ReadTime();
   if (T.bValid())
      {
      if (T.bUpdatePCTime())
         ShowInfo(L"System Time Updated",L"GPS Time");
      else
         ShowError(L"System Time Update FAILED",L"GPS Time");
      }
   else
      ShowError(L"GPS Time Invalid: System Time Not Updated",L"GPS Time");
}
//---------------------------------------------------------------------------





