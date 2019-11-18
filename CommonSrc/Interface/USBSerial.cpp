//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "USBSerial.h"
#include "jutils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USBSerial::USBSerial(const int _nHWType, const int nDevSerial)
{
   pUSB2=NULL;
   pUSB3=NULL;
   pUSB3100=NULL;
   pUSB4ch=NULL;
   nHWType=_nHWType;

   if (nHWType==USB_DETECT_DEV)
      nHWType=nAutoDetect(nDevSerial);

   switch(nHWType)
      {
      case USB_SERIAL2_DEV:
         pUSB2=new USBSerial2(nDevSerial);
         if (!pUSB2->bOK())
            {
            delete pUSB2;
            pUSB2=NULL;
            }
         break;
      case USB_SERIAL3_DEV:
         pUSB3=new USBSerial3(nDevSerial);
         if (!pUSB3->bOK())
            {
            delete pUSB3;
            pUSB3=NULL;
            }
         break;
      case USB_SERIAL31_DEV:
         pUSB3100=new USBSerial3100(nDevSerial);
         if (!pUSB3100->bOK())
            {
            delete pUSB3100;
            pUSB3100=NULL;
            }
         break;
      case USB_SERIAL3_4CH_DEV:
         pUSB4ch=new USBSerial31_4ch(nDevSerial);
         if (!pUSB4ch->bOK())
            {
            delete pUSB4ch;
            pUSB4ch=NULL;
            }
         break;
      default:    //Auto Detect failed
         ;
      }

}

USBSerial::~USBSerial()
{
   delete pUSB2;
   delete pUSB3;
   delete pUSB3100;
   delete pUSB4ch;
}

int USBSerial::nAutoDetect(const int nDevSerial)
{
   int nDev=-1,nSerial;
   FT245_Detect Detect;
   for (int i=0; i<Detect.nDevCount(); i++)
      {
      Detect.GetDev(i,nDev,nSerial);
      switch(nDev)
         {
         case USB_SERIAL2_DEV:
         case USB_SERIAL3_DEV:
         case USB_SERIAL31_DEV:
         case USB_SERIAL3_4CH_DEV:
            if (nDevSerial!=-1)
               {
               if (nDevSerial==nSerial)
                  i=Detect.nDevCount();   //Found valid serial number
					}
            else
               i=Detect.nDevCount();   //Found valid device
            break;
         default: nDev=-1;
         }
      }
   return nDev;
}

bool USBSerial::bOK()
{
   if (pUSB2)
      return pUSB2->bOK();
   if (pUSB3)
      return pUSB3->bOK();
   if (pUSB3100)
      return pUSB3100->bOK();
   if (pUSB4ch)
      return pUSB4ch->bOK();
   return false;
}

String USBSerial::sGetDescr()
{
   if (pUSB2)
      return "USB Serial Interface";
   if (pUSB3)
      return pUSB3->sGetDescr();
   if (pUSB3100)
      return pUSB3100->sGetDescr();
   if (pUSB4ch)
      return pUSB4ch->sGetDescr();
	return "";
}


void USBSerial::Purge()
{
   if (pUSB2)
      pUSB2->Purge();
   else if (pUSB3)
      pUSB3->Purge();
   else if (pUSB3100)
      pUSB3100->Purge();
   else if (pUSB4ch)
      pUSB4ch->Purge();
}


void USBSerial::Reset()
{
   if (pUSB3)
      pUSB3->Reset();
   else if (pUSB3100)
      pUSB3100->Reset();
   else if (pUSB4ch)
      pUSB4ch->Reset();
}

bool USBSerial::bUSBLoopbackOK()
{
   if (pUSB2)
      return true;
   if (pUSB3)
      return pUSB3->bUSBLoopbackOK();
	if (pUSB3100)
      return pUSB3100->bUSBLoopbackOK();
   if (pUSB4ch)
      return pUSB4ch->bLoopbackOK();
   return false;
}

double USBSerial::dGetFWVersion() const
{
   if (pUSB3100)
      return pUSB3100->dGetFWVersion();
   if (pUSB4ch)
      return pUSB4ch->dGetFWVersion();
   return 0.0;
}

int USBSerial::nGetFWVersion() const
{
   if (pUSB3100)
      return pUSB3100->nGetFWVersion();
   return 0;
}

bool USBSerial::bFWVersionOK(const int nVer) const
{
   if (pUSB3100)
      return pUSB3100->bFWVersionOK(nVer);
   return true;
}

int USBSerial::nGetFWChannels() const
{
   if (pUSB4ch)
		return pUSB3100->nGetFWChannels();
   return 1;
}

bool USBSerial::bVersionOK()
{
   if (pUSB3100)
      return pUSB3100->bVersionOK();
   return true;
}

void USBSerial::SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits, const bool _b422)
{
   b422=_b422;
   char cPar=toupper(cParity);
   switch(cParity)
      {
      case 'N':
      case 'E':
      case 'O':
         break;
      default:
         cPar='N';
      }
   int nDataB=nDataBits;
   if ((nDataB<7)||(nDataB>8))
      nDataB=8;
   int nStop=nStopBits;
   if ((nStop<1)||(nStop>3))
      nStop=1;
   if (pUSB2)
      pUSB2->Setup(nBaud,cPar,nDataB,nStop,!b422);
   else if (pUSB3)
		pUSB3->SetupUART(nBaud,cPar,nDataB,nStop);
   else if (pUSB3100)
      pUSB3100->SetupUART(nBaud,cPar,nDataB,nStop);
   else if (pUSB4ch)
      pUSB4ch->SetupUART(3,nBaud,cPar,nDataB,nStop); //Channel 3 only
}

void USBSerial::StartUART(const bool _b422, const bool bAlwaysEn422TX)
{
   if (pUSB2)
      {
      //Not needed
      }
   else if (pUSB3)
      pUSB3->StartUART(_b422);
   else if (pUSB3100)
      pUSB3100->StartUART(_b422,bAlwaysEn422TX);
   else if (pUSB4ch)
      pUSB4ch->StartUART(3,_b422,bAlwaysEn422TX);   //Channel 3 only
}

void USBSerial::StopUART()
{
   if (pUSB2)
      {
      //Not needed
      }
   else if (pUSB3)
      pUSB3->StopUART();
   else if (pUSB3100)
      pUSB3100->StopUART();
   else if (pUSB4ch)
      pUSB4ch->StopUART(3);   //Channel 3 only
}

//4 channel only
void USBSerial::SetupUART(const int nChan, const int nBaud, const char cParity, const int nDataBits, const int nStopBits)
{
   if (pUSB4ch)
      pUSB4ch->SetupUART(nChan,nBaud,cParity,nDataBits,nStopBits);
}

void USBSerial::StartUART(const int nChan, const bool _b422, const bool bAlwaysEn422TX)
{
   if (pUSB4ch)
      pUSB4ch->StartUART(nChan,_b422,bAlwaysEn422TX);
}

void USBSerial::StopUART(const int nChan)
{
   if (pUSB4ch)
      pUSB4ch->StopUART(nChan);  
}

bool USBSerial::bServiceMulti()
{
   if (pUSB4ch)
      return pUSB4ch->bServiceMulti();
   return false;
}

int USBSerial::nGetMultiData(const int nChan, BYTE* pucData, const int nMaxSize)  //Chan 0..2
{
   if (pUSB4ch)
     return pUSB4ch->nGetMultiData(nChan,pucData,nMaxSize);  //Chan 0..2
   return 0;
}

int USBSerial::nRXUARTData(BYTE* pucData, const int nMaxSize)
{
   if (pUSB2)
      return pUSB2->nRXData(pucData,nMaxSize);
   else if (pUSB3)
      return pUSB3->nRXUARTData(pucData,nMaxSize);
   else if (pUSB3100)
      return pUSB3100->nRXUARTData(pucData,nMaxSize);
   else if (pUSB4ch)
      return pUSB4ch->nRXData(pucData,nMaxSize);   //Chan 3 only
   return 0;
}

int USBSerial::nTXUARTData(const BYTE* pucData, const int nSize)
{
   if (pUSB2)
      {
      BYTE* puc=new BYTE[nSize];
      memcpy(puc,pucData,nSize);
      int n=pUSB2->nTXData(puc,nSize);
      delete[] puc;
      return n;
      }
   else if (pUSB3)
      return pUSB3->nTXUARTData(pucData,nSize);
   else if (pUSB3100)
      return pUSB3100->nTXUARTData(pucData,nSize);
   else if (pUSB4ch)
      return pUSB4ch->nTXData(pucData,nSize);   //Chan 3 only
   return 0;
}

void USBSerial::ReadErrors(int& nFrameErrors, int& nParErrors)
{
   nFrameErrors=0;
   nParErrors=0;
   if (pUSB2)
      {
      }
   else if (pUSB3)
      pUSB3->ReadErrors(nFrameErrors,nParErrors);
   else if (pUSB3100)
      pUSB3100->ReadErrors(nFrameErrors,nParErrors);
   else if (pUSB4ch)
      pUSB4ch->ReadErrors(nFrameErrors,nParErrors);
}

bool USBSerial::bMeasureAvail() const
{
   if (pUSB2)
      return false;
   else if (pUSB3)
      return true;
   else if (pUSB3100)
      return true;
   else if (pUSB4ch)
      return true;
   return false;
}


void USBSerial::StartMeasure(const bool b422)
{
   if (pUSB2)
		{
      }
   else if (pUSB3)
      pUSB3->StartMeasure();
   else if (pUSB3100)
      pUSB3100->StartMeasure(b422);
   else if (pUSB4ch)
		pUSB4ch->StartMeasure();
}



void USBSerial::MeasureBaud(int& nBaud, int& nCount)
{
	nBaud=0;
	nCount=0;
	if (pUSB2)
		{
		}
	else if (pUSB3)
		pUSB3->MeasureBaud(nBaud,nCount);
	else if (pUSB3100)
		pUSB3100->MeasureBaud(nBaud,nCount);
	else if (pUSB4ch)
		pUSB4ch->MeasureBaud(nBaud,nCount);
}

void USBSerial::StartTransCapt(const bool b422, const bool bInvert)
{
	if (pUSB3100)
		pUSB3100->StartTransCapt(b422,bInvert);
}

void USBSerial::StopTransCapt()
{
	if (pUSB3100)
		pUSB3100->StopTransCapt();
}




