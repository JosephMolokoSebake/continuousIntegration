//---------------------------------------------------------------------------

#include "jpch.h"
#pragma hdrstop

#include "USBSerial3100.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USBSerial3100::USBSerial3100(const int nSerialNum):USB_FT245(USB_SERIAL31_DEV,nSerialNum)
{
   aucSetup[0]=USB_SETUP0_WR;
   aucSetup[1]=USB_SETUP1_WR;
   wFWVer=0;
}

USBSerial3100::~USBSerial3100()
{
}

bool USBSerial3100::bUSBLoopbackOK()
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
            else
               {
               DWORD dwRd=(aucBuf[3]<<24)|(aucBuf[2]<<16)|(aucBuf[1]<<8)|aucBuf[0];
               wFWVer=(dwRd>>16)&0x0000FFFF;
               }
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

bool USBSerial3100::bVersionOK()
{
	if (bUSBLoopbackOK())
      {
		if ((!bFWVersionOK(FIRMWARE_VERSION_3100))||(!bFWVersionOK(FIRMWARE_VERSION_4000)))
         {
         ShowError(L"Incorrect Firmware Version",sGetDescr(USB_SERIAL3_DEV)+" Error");
         return false;
         }
      }
   return true;
}


void USBSerial3100::Reset()
{
   Acquire();
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_RESET;
   aucBuf[n++]=aucSetup[1];
   aucBuf[n++]=aucSetup[0]; //do nothing
   nWriteUSB(aucBuf,n);
   Release();
}

void USBSerial3100::SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits)
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

void USBSerial3100::StartUART(const bool b422, const bool bAlwaysEn422TX)
{
	Acquire();
	BYTE aucBuf[16];
	int n=0;
	aucBuf[n++]=USB_SETUP0_WR|USB_RESET;   //Reset All
	aucBuf[n++]=USB_SETUP0_WR;             //do nothing
	aucSetup[0]=USB_SETUP0_WR|USB_ENABLE;
	aucSetup[1]=USB_SETUP1_WR|USB_ALWAYS_EN_TX;  //Always Enable TX (although not used by 232)
	if (b422)
		{
		aucSetup[0]|=USB_SEL422;
		if (!bAlwaysEn422TX)
			aucSetup[1]=USB_SETUP1_WR&(~USB_ALWAYS_EN_TX);    //Auto Enable (with TX of byte)
		}
	else
		{
		aucSetup[0]&=(~USB_SEL422);
		aucSetup[1]=USB_SETUP1_WR|USB_ALWAYS_EN_TX;  //always enable (although not used by 232)
		}
	aucBuf[n++]=aucSetup[1];
	aucBuf[n++]=aucSetup[0];
	nWriteUSB(aucBuf,n);
	Release();
	::SleepEx(1000,false);
}

void USBSerial3100::StopUART()
{
	Acquire();
	BYTE aucBuf[16];
	int n=0;
	aucSetup[0]&=(~USB_ENABLE);
	aucBuf[n++]=aucSetup[0];
	nWriteUSB(aucBuf,n);
	Release();
}

int USBSerial3100::nRXUARTData(BYTE* pucData, const int nMaxSize)
{
	Acquire();
	BYTE aucData[65536];
	int n=0;
	int nDataSize=0;
	int nRXed;
	aucData[n++]=USB_FIFO_REQ;
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
   if (nDataSize>nMaxSize)
      nDataSize=nMaxSize;
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

int USBSerial3100::nTXUARTData(const BYTE* pucData, const int nSize)
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


void USBSerial3100::ReadErrors(int& nFrameErrors, int& nParErrors)
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

void USBSerial3100::StartMeasure(const bool b422)
{
   BYTE aucBuf[16];
   int n=0;
   aucBuf[n++]=aucSetup[0]|USB_RESET;
	aucBuf[n++]=aucSetup[0]; //do nothing
	aucSetup[0]|=USB_MEASURE;
	if (b422)
		aucSetup[0]|=USB_SEL422;
	else
		aucSetup[0]&=(~USB_SEL422);
	aucBuf[n++]=aucSetup[0];
	nWriteUSB(aucBuf,n);
}

void USBSerial3100::MeasureBaud(int& nBaud, int& nCount)
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


void USBSerial3100::StartTransCapt(const bool b422, const bool bInvert)
{
	if (nGetFWVersion()>=4000)
		{
		Acquire();
		BYTE aucBuf[16];
		int n=0;
		aucBuf[n++]=USB_SETUP0_WR|USB_RESET;   //Reset All
		aucBuf[n++]=USB_SETUP0_WR;             //do nothing
		aucSetup[0]=USB_SETUP0_WR|USB_ENABLE;
		aucSetup[1]=USB_SETUP1_WR|USB_ALWAYS_EN_TX;  //Always Enable TX (although not used by 232)
		if (b422)
			aucSetup[0]|=USB_SEL422;
		else
			aucSetup[0]&=(~USB_SEL422);
		if (bInvert)
			aucSetup[1]|=USB_INVERT_422_IN;
		else
			aucSetup[1]&=(~USB_INVERT_422_IN);
		aucSetup[1]|=USB_SEL_TRANS;
		aucBuf[n++]=aucSetup[1];
		aucBuf[n++]=aucSetup[0];
		nWriteUSB(aucBuf,n);
		Release();
		}
}


void USBSerial3100::StopTransCapt()
{
	if (nGetFWVersion()>=4000)
		{
		Acquire();
		BYTE aucBuf[16];
		int n=0;
		aucSetup[0]&=(~USB_ENABLE);
		aucBuf[n++]=aucSetup[0];
		aucSetup[1]&=(~USB_SEL_TRANS);
		aucBuf[n++]=aucSetup[1];
		nWriteUSB(aucBuf,n);
		Release();
		}
}

