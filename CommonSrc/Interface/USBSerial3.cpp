//---------------------------------------------------------------------------

#include "jpch.h"
#pragma hdrstop

#include "USBSerial3.h"
#include "Jcrc.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USBSerial3::USBSerial3(const int nSerialNum):USB_FT245(USB_SERIAL3_DEV,nSerialNum)
{
   aucSetup[0]=0;
   aucSetup[1]=0;
}

USBSerial3::~USBSerial3()
{
}

bool USBSerial3::bUSBLoopbackOK()
{
   BYTE aucBuf[256];
   int n=0;
   aucBuf[n++]=0x00;    //SETUP
   aucBuf[n++]=USB_SETUP1_WR|0x05;
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
      nReadUSB(aucBuf,4);
      if (aucBuf[0]!=0x50)
         nError=1;   //Data Error
      if (!nError)
         {
         n=0;
         aucBuf[n++]=0x00;    //SETUP
         aucBuf[n++]=USB_SETUP1_WR|0x0A;
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
            if (aucBuf[0]!=0xA0)
               nError=1;   //Data Error
            }
         else
            nError=2;   //Timeout Error
         }
      memset(aucSetup,0,sizeof(aucSetup));
      }
   else
      nError=2;   //Timeout Error
   if (nError)
      {
      if (nError==2)
         ShowError(L"Loopback Timeout Error",sGetDescr()+" Error");
      else
         ShowError(L"Loopback Data Error",sGetDescr()+" Error");
      return false;
      }
   else
      return true;

/*
   BYTE aucBuf[256];
   int n=0;
   aucBuf[n++]=0x00;    //SETUP0
   aucBuf[n++]=USB_SETUP1_WR|0x05;
   aucBuf[n++]=USB_LB_REQ;
   nWriteUSB(aucBuf,n);
   bool bFin=false;
   DWORD dwMs=::GetTickCount();
   while((!bFin)&&(::GetTickCount()-dwMs<2000))
      {
      int nUsed=nReadUSBRXSize();
      bFin=(nUsed==4);
      }
   if (bFin)
      {
      nReadUSB(aucBuf,4);
      bool bOK=(aucBuf[0]==0x50);
      if (bOK)
         {
         n=0;
         aucBuf[n++]=0x00;    //SETUP0
         aucBuf[n++]=USB_SETUP1_WR|0x0A;
         aucBuf[n++]=USB_LB_REQ;
         nWriteUSB(aucBuf,n);
         bFin=false;
         while(!bFin)
            {
            int nUsed=nReadUSBRXSize();
            bFin=(nUsed==4);
            }
         nReadUSB(aucBuf,4);
         bOK=(aucBuf[0]==0xA0);
         }
      aucSetup[0]=0;
      aucSetup[1]=0;
      return bOK;
      }
   else
      return false;
*/
}

void USBSerial3::Reset()
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_RESET;
   aucBuf[n++]=aucSetup[0]; //do nothing
   nWriteUSB(aucBuf,n);
   Release();
}

void USBSerial3::SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits)
{
   Acquire();
   BYTE aucBuf[256];
   int n=0;
   aucBuf[n++]=0x00;  //remove ENABLE

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

void USBSerial3::StartUART(const bool b422)
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_RESET;
   aucBuf[n++]=aucSetup[0]; //do nothing
   if (b422)
      aucSetup[0]|=(USB_ENABLE|USB_SEL422);
   else
      {
      aucSetup[0]&=(~USB_SEL422);
      aucSetup[0]|=USB_ENABLE;
      }
   aucBuf[n++]=aucSetup[0];
   nWriteUSB(aucBuf,n);
   Release();
   ::SleepEx(1000,false);
}

void USBSerial3::StopUART()
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucSetup[0]&=(~USB_ENABLE);
   aucBuf[n++]=aucSetup[0];
   nWriteUSB(aucBuf,n);
   Release();
}

int USBSerial3::nRXUARTData(BYTE* pucData, const int nMaxSize)
{
   Acquire();
   BYTE aucData[65536];
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
   if (nDataSize>nMaxSize)
      nDataSize=nMaxSize;
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

int USBSerial3::nTXUARTData(const BYTE* pucData, const int nSize)
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


void USBSerial3::ReadErrors(int& nFrameErrors, int& nParErrors)
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

void USBSerial3::StartMeasure()
{
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_RESET;
   aucBuf[n++]=aucSetup[0]; //do nothing
   aucSetup[0]|=USB_MEASURE;
   aucBuf[n++]=aucSetup[0];
   nWriteUSB(aucBuf,n);
}

void USBSerial3::MeasureBaud(int& nBaud, int& nCount)
{
   BYTE aucBuf[16];
   int n=0;
   aucSetup[0]&=(~USB_MEASURE);
   aucBuf[n++]=aucSetup[0];
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

/*
void USBSerial3::StartCapture(const int nMicroSec)
{
   DWORD dwInt=nMicroSec*10-1;
   BYTE aucData[9];
   for (int i=0; i<8; i++)
      {
      aucData[i]=(BYTE)(dwInt&0x0000000F);
      aucData[i]|=((i+1)<<4);
      dwInt>>=4;
      }
   aucData[8]=0x90;
   WriteUSB(aucData,9);
   ADList.Clear();
   nRXOffs=0;
}

void USBSerial3::StopCapture()
{
   BYTE aucData[1]={0xA0};
   WriteUSB(aucData,1);
}

void USBSerial3::ServiceFIFO()
{
   int nSize=nReadUSBSize();
   if (nSize>0)
      {
      BYTE* pucData=new BYTE[nSize];
      ReadUSB(pucData,nSize);
      for (int i=0; i<nSize; i++)
         {

         if (pTestFil)
            {
            String s;
            s.printf(L"%4d %02X",i,pucData[i]);
            pTestFil->TextLine(s);
            }

         if (pucData[i]&0x80)
            nRXOffs=0;
         aucRXData[nRXOffs++]=pucData[i];
         if (nRXOffs==15)
            {
            ADDataPacket* p=new ADDataPacket;
            for (int j=0; j<15; j++)
               p->aucData[j]=aucRXData[j];
            ADList.nAdd(p);

            if (pTestFil)
               {
               String s;
               if (p->bCRCOK())
                  {
                  s=L"CRC OK";
                  pTestFil->TextLine(s);
                  }


               }

            nRXOffs=0;
            }
         }
      delete[] pucData;
      }
}

ADDataPacket USBSerial3::GetDataPacket()
{
   ADDataPacket P;
   if (ADList.nGetCount()>0)
      ADList.PopFirst(P);
   return P;
}

bool ADDataPacket::bCRCOK() const
{
   RockCRC CRC(16,
               0x1021,
               0xffff,
               false,
               false,
               0x0000);
   for (int i=1; i<13; i++)
      CRC.Next(aucData[i]);
   WORD wCRC=(WORD)(CRC.ulGetCRC()&0x0000FFFF);
   BYTE ucLSB=wCRC&0x7F;
   BYTE ucMSB=((wCRC&0x7F80)>>7)&0x7F;
   bool bOK=(ucMSB==aucData[13])&&(ucLSB==aucData[14]);
//   return true;
   return bOK;
}
*/
