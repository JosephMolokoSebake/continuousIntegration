//---------------------------------------------------------------------------

#include "jpch.h"
#pragma hdrstop

#include "USBSerial31_4ch.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USBSerial31_4ch::USBSerial31_4ch(const int nSerialNum):USB_FT245(USB_SERIAL31_DEV,nSerialNum)
{
   aucSetup[0]=USB_SETUP0_WR;
   aucSetup[1]=USB_SETUP1_WR;
   wFWVer=0;
}

USBSerial31_4ch::~USBSerial31_4ch()
{
}

bool USBSerial31_4ch::bLoopbackOK()
{
   BYTE aucBuf[256],aucRXBuf[256];
   int n=0;

   DWORD dwWr=0x89ABCDEE;

   aucBuf[n++]=USB_SETUP0_WR|USB_SU0_CHAN_0;           //Setup chan 0
   aucBuf[n++]=USB_BAUD0_WR|((dwWr>>0)&0x0F);
   aucBuf[n++]=USB_BAUD1_WR|((dwWr>>4)&0x0F);
   aucBuf[n++]=USB_BAUD2_WR|((dwWr>>8)&0x0F);
   aucBuf[n++]=USB_BAUD3_WR|((dwWr>>12)&0x0F);

   aucBuf[n++]=USB_SETUP0_WR|USB_SU0_CHAN_1;           //Setup chan 1
   aucBuf[n++]=USB_BAUD0_WR|((dwWr>>16)&0x0F);
   aucBuf[n++]=USB_BAUD1_WR|((dwWr>>20)&0x0F);
   aucBuf[n++]=USB_BAUD2_WR|((dwWr>>24)&0x0F);
   aucBuf[n++]=USB_BAUD3_WR|((dwWr>>28)&0x0F);

   aucBuf[n++]=USB_LB_REQ;
   nWriteUSB(aucBuf,n);
   bool bFin=false;
   int nError=0;
   DWORD dwMs=::GetTickCount();
   while((!bFin)&&(::GetTickCount()-dwMs<2000))
      {
      int nUsed=nReadUSBRXSize();
      bFin=(nUsed==4);
      }
   if (bFin)
      {
      nReadUSB(aucRXBuf,4);
      DWORD dwRd=(aucRXBuf[3]<<24)|(aucRXBuf[2]<<16)|(aucRXBuf[1]<<8)|aucRXBuf[0];
      if ((dwRd&0x0000FFFF)!=(dwWr&0x0000FFFF))
         nError=1;   //Data Error
      if (!nError)
         {
         wFWVer=(dwRd>>16)&0x0000FFFF;
         n=0;
         dwWr=0xA5A5A5A5;
         aucBuf[n++]=USB_SETUP0_WR|USB_SU0_CHAN_0;           //Setup chan 0
         aucBuf[n++]=USB_BAUD0_WR|((dwWr>>0)&0x0F);
         aucBuf[n++]=USB_BAUD1_WR|((dwWr>>4)&0x0F);
         aucBuf[n++]=USB_BAUD2_WR|((dwWr>>8)&0x0F);
         aucBuf[n++]=USB_BAUD3_WR|((dwWr>>12)&0x0F);
         aucBuf[n++]=USB_SETUP0_WR|USB_SU0_CHAN_1;           //Setup chan 1
         aucBuf[n++]=USB_BAUD0_WR|((dwWr>>16)&0x0F);
         aucBuf[n++]=USB_BAUD1_WR|((dwWr>>20)&0x0F);
         aucBuf[n++]=USB_BAUD2_WR|((dwWr>>24)&0x0F);
         aucBuf[n++]=USB_BAUD3_WR|((dwWr>>28)&0x0F);
         aucBuf[n++]=USB_LB_REQ;
         nWriteUSB(aucBuf,n);
         bFin=false;
         dwMs=::GetTickCount();
         while((!bFin)&&(::GetTickCount()-dwMs<2000))
            {
            int nUsed=nReadUSBRXSize();
            bFin=(nUsed==4);
            }
         if (bFin)
            {
            nReadUSB(aucBuf,4);
            dwRd=(aucBuf[3]<<24)|(aucBuf[2]<<16)|(aucBuf[1]<<8)|aucBuf[0];
            if ((dwRd&0x0000FFFF)!=(dwWr&0x0000FFFF))
               nError=1;   //Data Error
            }
         else
            nError=2;   //Timeout Error
         }
      aucSetup[0]=USB_SETUP0_WR;
      aucSetup[1]=USB_SETUP1_WR;
      }
   else
      nError=2;   //Timeout Error
   if (nError)
      {
      if (nError==2)
         ShowError(L"Loopback Timeout Error",sGetDescr(USB_SERIAL3_DEV)+" Error");
      else
         ShowError(L"Loopback Data Error",sGetDescr(USB_SERIAL3_DEV)+" Error");
      return false;
      }
   else
      return true;
}

void USBSerial31_4ch::Reset()
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_SU0_RESET;
   aucBuf[n++]=aucSetup[1];
   aucBuf[n++]=aucSetup[0]; //do nothing
   nWriteUSB(aucBuf,n);
   Release();
}

void USBSerial31_4ch::SetupUART(const int nChan, const int nBaud, const char cParity, const int nDataBits, const int nStopBits)
{
   Acquire();

   nCh=-1;
   for (int i=0; i<4; i++)
      anRX[i]=0;


   BYTE aucBuf[256];
   int n=0;
   aucBuf[n++]=0x00;  //remove ENABLE

   aucBuf[n++]=USB_SETUP0_WR|(nChan<<1);           //Setup chan

   //Baud Rate
   DWORD dw=(CLOCK_FREQ*1e6)/(nBaud*2.0)+0.5;   //Half bit interval
   aucBuf[n++]=(BYTE)(USB_BAUD0_WR | (dw&0x0000000F));
   dw>>=4;
   aucBuf[n++]=(BYTE)(USB_BAUD1_WR | (dw&0x0000000F));
   dw>>=4;
   aucBuf[n++]=(BYTE)(USB_BAUD2_WR | (dw&0x0000000F));
   dw>>=4;
   aucBuf[n++]=(BYTE)(USB_BAUD3_WR | (dw&0x0000000F));
   //Data Bits
   int nDataB=nDataBits;
   if ((nDataB<7)||(nDataB>8))
      nDataB=8;
   aucBuf[n++]=(BYTE)(USB_DATABITS_WR | nDataB);
   //Parity
   switch(cParity)
      {
      case 'O': dw=1;   break;
      case 'E': dw=2;   break;
      default:  dw=0;
      }
   int nStop=nStopBits;
   if ((nStop<1)||(nStop>3))
      nStop=1;
   dw|=(nStop<<2);
   aucBuf[n++]=(BYTE)(USB_PARSTOP_WR | (dw&0x0000000F));
   PurgeTX();
   nWriteUSB(aucBuf,n);
   Release();
}

void USBSerial31_4ch::StartUART(const int nChan, const bool b422, const bool bAlwaysEn422TX)
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=USB_SETUP0_WR|USB_SU0_RESET;   //Reset All
   aucBuf[n++]=USB_SETUP0_WR|(nChan<<1);  //Setup channel
   aucSetup[0]=USB_SETUP0_WR;
   aucSetup[1]=USB_SETUP1_WR|USB_SU1_ALWAYS_EN_TX|USB_SU1_ENABLE;  //Always Enable TX (although not used by 232)
   if (b422)
      {
      aucSetup[1]|=USB_SU1_SEL422;
      if (!bAlwaysEn422TX)
         aucSetup[1]&=(~USB_SU1_ALWAYS_EN_TX);    //Auto Enable (with TX of byte)
      }
   else
      {
      aucSetup[1]&=(~USB_SU1_SEL422);
      }
   aucBuf[n++]=aucSetup[1];
   aucBuf[n++]=aucSetup[0];
   nWriteUSB(aucBuf,n);
   Release();
   ::SleepEx(1000,false);
}

void USBSerial31_4ch::StopUART(const int)
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucSetup[1]&=(~USB_SU1_ENABLE);
   aucBuf[n++]=aucSetup[1];
   nWriteUSB(aucBuf,n);
   Release();
}

int USBSerial31_4ch::nRXData(BYTE* pucData, const int)
{
   Acquire();
   BYTE aucData[512];
   int n=0;
   int nDataSize=0;
   int nRXed;
   aucData[n++]=USB_FIFO_REQ;
//   SleepEx(20,false);
   nWriteUSB(aucData,n);
   do
      {
      n=nReadUSBRXSize();
      Application->ProcessMessages();
      }
   while(n<3);
   if (n>=3)
      {
      nReadUSB(aucData,3);
      nDataSize=aucData[0]|(aucData[1]<<8)|(aucData[2]<<16);
      Application->ProcessMessages();
      }
   int nRead=0;
   while(nRead<nDataSize)
      {
      int n=nReadUSBRXSize();
      Application->ProcessMessages();
      if (n>0)
         {
         nRXed=nReadUSB(&pucData[nRead],n);
         nRead+=nRXed;
         }
      }
   Release();
   return nDataSize;
}

int USBSerial31_4ch::nTXData(const BYTE* pucData, const int nSize)
{
   BYTE* pucBuf=new BYTE[nSize*2];
   for (int i=0; i<nSize; i++)
      {
      pucBuf[i*2]=(pucData[i]&0x0F)|USB_TX0_WR;    //LS Nibble
      pucBuf[i*2+1]=(pucData[i]>>4)|USB_TX1_WR;    //MS Nibble
      }
   int nTotal=nSize*2;
   int n=0;
   while(n<nTotal)
      {
      int nLeft=384-nReadUSBTXSize();
      int nSend;
      if (nTotal-n>nLeft)
         nSend=nLeft;
      else
         nSend=nTotal-n;
      if (nSend>0)
         {
         nWriteUSB(&pucBuf[n],nSend);
         n+=nSend;
         }
      }
   delete[] pucBuf;
   return nSize;
}


void USBSerial31_4ch::ReadErrors(int& nFrameErrors, int& nParErrors)
{
   BYTE aucData[4];
   int n=0;
   aucData[n++]=USB_ERRORS_REQ;
   nWriteUSB(aucData,n);
   do
      n=nReadUSBRXSize();
   while(n<4);
   if (n>=4)
      {
      nReadUSB(aucData,4);
      nFrameErrors=aucData[0]|(aucData[1]<<8);
      nParErrors=aucData[2]|(aucData[3]<<8);
      }
}

void USBSerial31_4ch::StartMeasure()
{
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_SU0_RESET;
   aucBuf[n++]=aucSetup[0]; //do nothing
   aucSetup[1]|=USB_SU1_MEASURE;
   aucBuf[n++]=aucSetup[1];
   nWriteUSB(aucBuf,n);
}

void USBSerial31_4ch::MeasureBaud(int& nBaud, int& nCount)
{
   BYTE aucBuf[16];
   int n=0;
   aucSetup[1]&=(~USB_SU1_MEASURE);
   aucBuf[n++]=aucSetup[1];
   aucBuf[n++]=USB_MEASURE_REQ;
   nWriteUSB(aucBuf,n);
   do
      n=nReadUSBRXSize();
   while(n<4);
   if (n>=4)
      {
      nReadUSB(aucBuf,4);
      DWORD dw=aucBuf[0]|(aucBuf[1]<<8)|(aucBuf[2]<<16)|(aucBuf[3]<<24);
      nBaud=((dw>>UART_M_WIDTH_SHIFT)&UART_M_WIDTH_MASK);
      nCount=((dw>>UART_M_COUNT_SHIFT)&UART_M_COUNT_MASK);
      if (nCount>0)
         nBaud=(CLOCK_FREQ*1e6)/(nBaud+1);
      else
         nBaud=0;
      }
   else
      {
      nBaud=0;
      nCount=0;
      }
}

int USBSerial31_4ch::nRXMultiData(BYTE* pucData, const int)
{
   Acquire();
   BYTE aucData[512];
   int n=0;
   int nDataSize=0;
   int nRXed;
   aucData[n++]=USB_MULTI_REQ;
//   SleepEx(20,false);
   nWriteUSB(aucData,n);
   do
      {
      n=nReadUSBRXSize();
      Application->ProcessMessages();
      }
   while(n<3);
   if (n>=3)
      {
      nReadUSB(aucData,3);
      nDataSize=aucData[0]|(aucData[1]<<8)|(aucData[2]<<16);
      Application->ProcessMessages();
      }
   int nRead=0;
   while(nRead<nDataSize)
      {
      int n=nReadUSBRXSize();
      Application->ProcessMessages();
      if (n>0)
         {
         nRXed=nReadUSB(&pucData[nRead],n);
         nRead+=nRXed;
         }
      }
   Release();
   return nDataSize;
}


bool USBSerial31_4ch::bServiceMulti()
{
   bool bAvail=false;
   BYTE* pucRX=new BYTE[65536];
   int nRX=nRXMultiData(pucRX,65536);
   for (int i=0; i<nRX; i++)
      {
      if ((pucRX[i]&0xC0)==0xC0)
         {
         nCh=(pucRX[i]>>4)&0x03;
         ucData=pucRX[i]&0x0F;
         }
      else
         {
         if (nCh>=0)
            {
            ucData|=(pucRX[i]<<4);
            aucData[nCh][anRX[nCh]++]=ucData;
            bAvail=true;
            }
         nCh=-1;
         }
      }

   delete[] pucRX;
   return bAvail;
}

int USBSerial31_4ch::nGetMultiData(const int nChan, BYTE* pucData, const int)
{
   int nSize=anRX[nChan];
   if (nSize>0)
      {
      memcpy(pucData,aucData[nChan],nSize);
      anRX[nChan]=0;
      }
   return nSize;
}



