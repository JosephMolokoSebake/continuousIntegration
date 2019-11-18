//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "CommsTimeDistr_101.h"
#include "CommsIntDeviceInterface.h"
#include "JFile.h"
#include "JUtils.h"
#include "JMessageBar.h"
#include "GPSTime.h"
#include "JCommaText.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

GUID g_CommsInt101_GUID=CommsIntDevice_CLASS_GUID;

CommsTimeDistr_101::CommsTimeDistr_101():PCI32_WDM_Int(PCI32_WDM_Int::PCI_9030,L"Communications Interface Card",1000,g_CommsInt101_GUID)
{
   pLock=new TCriticalSection;
   if (bDeviceOK)
      {
//      bDeviceOK=bConfigFlex(L"c:\\qdesigns\\CommsInt\\Comms_TimeDistr\\Comms_TimeDistr_101.rbf");
      bDeviceOK=bConfigFlex(L"Comms_TimeDistr_101.rbf");
      if (bDeviceOK)
         {
         Write(SETUP_WR,0);   //ensure that MRR is not selected
         if (bLoopBackOK())
            {
            dwSetup=0;
            dwBaud=0;
            Write(BAUD_WR,dwBaud);
            ResetAll();
            }
         else
            {
            ErrorMessage(L"Cyclone Configuration Loopback Failed");
            bDeviceOK=false;
            }
         }
      }
   pCRC=new RockCRC(16,0x1021,0xFFFF,true,false,0x0000);   //CRC-16
   PrevLL.Invalidate();
   ZDATime.Invalidate();
   dwLastZDATime=0;
   dwLastPOSTime=0;
   nPOSMsgCount=0;
   nZDAMsgCount=0;
   nLeap_ms=LEAP_MILLISEC_15;
}

CommsTimeDistr_101::~CommsTimeDistr_101()
{
   delete pCRC;
   delete pLock;
}

bool CommsTimeDistr_101::bLoopBackOK()
{
   Write(LOOPBACK_WR,0xaaaaaaaa);
   bool bOK=(dwRead(LOOPBACK_RD)==0xaaaaaaaa);
   if (bOK)
      {
      Write(LOOPBACK_WR,0x55555555);
      bOK=(dwRead(LOOPBACK_RD)==0x55555555);
      }
   return bOK;
}

void CommsTimeDistr_101::StartTDCapt()
{
//   pFil=new JFile('O');
//   pFil->Create(L"Data.bin");

   pLock->Acquire();
   nTDMsgSize=0;
   nTDMsgOffs=0;
   nTDCRCErrors=0;

   dwSetup|=RESET_TD;
   Write(SETUP_WR,dwSetup);
   dwSetup&=(~RESET_TD);
   dwSetup|=ENABLE_TD;
   Write(SETUP_WR,dwSetup);
//   sPOS=L"";
   pLock->Release();
}

void CommsTimeDistr_101::StopTDCapt()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_TD);
   Write(SETUP_WR,dwSetup);
//   delete pFil;
   pLock->Release();
}

int CommsTimeDistr_101::nReadTDBuffer()
{
   DWORD dwCount=dwRead(TD_STATUS_RD);
   dwCount&=TD_COUNT_MASK;
   if (dwCount>0)
      {
      DWORD adwBuf[MAX_TD_BUFFER_SIZE];
      Read(TD_DATA_RD,adwBuf,dwCount);
      for (DWORD i=0; i<dwCount; i++)
         aucTDBuf[i]=(BYTE)(adwBuf[i]&0x000000FF);
      nTDBufSize=(int)dwCount;
      nTDBufOffs=0;
      }
   return dwCount;
}

bool CommsTimeDistr_101::bCheckCRC(const BYTE* pucMsg, const int nSize) const
{
   pCRC->Reset();
   for (int i=0; i<nSize-2; i++)
      pCRC->Next(pucMsg[i]);
   WORD uRXCRC=(WORD)pCRC->ulGetCRC();
   BYTE ucMSB=pucMsg[nSize-2];
   BYTE ucLSB=pucMsg[nSize-1];
   BYTE ucMSB1=~ucMSB;
   BYTE ucLSB1=~ucLSB;
   ucMSB=0;
   ucLSB=0;
   BYTE uc1=0x01;
   BYTE uc2=0x80;
   for (int i=0; i<8; i++)
      {
      if (ucMSB1&uc1) ucMSB|=uc2;
      if (ucLSB1&uc1) ucLSB|=uc2;
      uc1<<=1;
      uc2>>=1;
      }
   WORD uTXCRC=(ucMSB<<8)|ucLSB;
   return (uRXCRC==uTXCRC);
}

int CommsTimeDistr_101::nExtractTDMsg(BYTE* pucMsg, const int, bool& bCRCOK)
{
   int nSize=0;
   while((nTDBufOffs<nTDBufSize)&&(nSize==0))
      {
      aucTDMsg[nTDMsgOffs]=aucTDBuf[nTDBufOffs++];
      switch(nTDMsgOffs)
         {
         case 0:
            TDMsgType=TD_NO_MSG;
            nTDMsgSize=0;
            if (aucTDMsg[nTDMsgOffs]==0x01)   //HDLC Address OK
               ++nTDMsgOffs;
            break;
         case 1:
            nTDMsgSize=aucTDMsg[nTDMsgOffs]&0xFE;  //Mask LSbit
            ++nTDMsgOffs;
            break;
         case 2:
            switch(aucTDMsg[nTDMsgOffs]>>4)
               {
               case 0x01:
                  TDMsgType=TD_TIME_MSG;
                  ++nTDMsgOffs;
                  break;
               case 0x02:
                  TDMsgType=TD_ZDA_MSG;
                  ++nTDMsgOffs;
                  break;
               case 0x03:
                  TDMsgType=TD_POS_MSG;
                  ++nTDMsgOffs;
                  break;
               default:
                  nTDMsgOffs=0; //invalid
               }
            break;
         default:
            if (++nTDMsgOffs==nTDMsgSize+5)
               {
               bCRCOK=bCheckCRC(aucTDMsg,nTDMsgSize+5);
               memcpy(pucMsg,&aucTDMsg[3],nTDMsgSize);
               nSize=nTDMsgSize;
               nTDMsgOffs=0;
               }
         }
      }
   return nSize;
}

int CommsTimeDistr_101::nReadTimeMS()
{
   pLock->Acquire();
   DWORD dwT=dwRead(TIME_RD);
   pLock->Release();
   if (dwT&0x80000000)
      {
      dwT=(dwT&0x7FFFFFFF)/10;
      dwT|=0x80000000;
      }
   else
      dwT=0;
   return (int)dwT;
}


JTime CommsTimeDistr_101::ReadTime()
{
   JTime T;
   int nT=nReadTimeMS();
   if (nT&0x80000000)
      {
      if (ZDATime.bValid())
         T=ZDATime;
      else
         T=JTime(true);
      if (T.Get().wYear>2008)
         nLeap_ms=LEAP_MILLISEC_15;
      else
         nLeap_ms=LEAP_MILLISEC_14;
      nT&=0x7FFFFFFF;
      nT-=nLeap_ms;
      T.SetTime(nT);
      T+=7200;    //add 2 hours
      }
   else
      T.Invalidate();
   return T;
}

bool CommsTimeDistr_101::bWaitForTime()
{
   bool bOK;
   DWORD dwT=::GetTickCount();
   do
      {
      bOK=ReadTime().bValid();
      Application->ProcessMessages();
      }
   while((!bOK)&&(::GetTickCount()-dwT<3000));
   return bOK;
}

LatLong CommsTimeDistr_101::ReadLL()
{
   nReadTDMessages();
   return POSData.LL;
}

LatLong CommsTimeDistr_101::ReadLL(double& dSpeed, double& dCourse)
{
   nReadTDMessages();
   dSpeed=POSData.dSpeed;
   dCourse=POSData.dCourse;
   return POSData.LL;
}

int CommsTimeDistr_101::nReadTDMessages()
{
   int nMsg=0;
   pLock->Acquire();
   int nBufSize=nReadTDBuffer();
   pLock->Release();
   if (nBufSize>0)
      {
      int nMsgSize;
      do
         {
         BYTE aucTDMsg[1024];
         bool bCRCOK;
         nMsgSize=nExtractTDMsg(aucTDMsg,1024,bCRCOK);
         if ((nMsgSize>0)&&(!bCRCOK))
            ++nTDCRCErrors;
         if ((nMsgSize>3)&&(bCRCOK))
            {
            switch(TDMsgType)
               {
               case TD_POS_MSG:
                  if (bExtractPOSData(aucTDMsg,nMsgSize))
                     ++nMsg;
                  break;
               case TD_ZDA_MSG:
                  if (bExtractZDAData(aucTDMsg,nMsgSize))
                     ++nMsg;
                  break;
               case TD_TIME_MSG:
                  ++nMsg;
                  break;
               default: ;
               }
            }
         }
      while(nMsgSize>0);
      }
   DWORD dwT=::GetTickCount();
   if (dwT-dwLastZDATime>1500)
      ZDATime.Invalidate();
   if (dwT-dwLastPOSTime>1500)
      POSData.Invalidate();
   return nMsg;
}

bool CommsTimeDistr_101::bExtractPOSData(const BYTE* pucMsg, const int nSize)
{
   bool bOK=false;
   char* pszSt=new char[nSize+1];
   memcpy(pszSt,pucMsg,nSize);
   pszSt[nSize]=0;
/*
   pLock->Acquire();
   sPOS=pszSt;
   pLock->Release();
*/
   String s=AnsiString(pszSt);
   delete[] pszSt;
   if (s.LastDelimiter(L"*")>0)
      {
      //Verify checksum
      int i=1;
      BYTE uc=0x00;
      while((s[i]!='*')&&(i<=nSize))
         {
         if (s[i]!='$') uc^=s[i];
         ++i;
         }
      if ((s[i]=='*')&&(i<nSize-1))
         bOK=((s[i+1]==H2C(uc>>4))&&(s[i+2]==H2C(uc&0x0F)));
      }
   if (bOK)
      {
      try
         {
         CommaText Txt(s);
         if (Txt.nGetNum()==19)
            {
            POSData.bDiff=(Txt.nGetVal(2)==1);
            POSData.nSat=Txt.nGetVal(3);
				POSData.T.SetTime(Txt.nGetTime_ms(4,false));
            POSData.LL.dLat=Txt.dGetLat(5,6);
            POSData.LL.dLong=Txt.dGetLat(7,8);
            POSData.dAlt=Txt.dGetVal(9);
            POSData.dCourse=Txt.dGetVal(11);
            POSData.dSpeed=Txt.dGetVal(12);
            POSData.dVertVel=Txt.dGetVal(13);
            POSData.dPDOP=Txt.dGetVal(14);
            POSData.dHDOP=Txt.dGetVal(15);
            POSData.dVDOP=Txt.dGetVal(16);
            POSData.dTDOP=Txt.dGetVal(17);
            POSData.bNew=true;
            }
         else
            bOK=false;
         }
      catch(...)
         {
         POSData.Invalidate();
         POSData.bNew=false;
         bOK=false;
         }
      }
   if (bOK)
      {
      dwLastPOSTime=::GetTickCount();
      ++nPOSMsgCount;
      }
   return bOK;
}

bool CommsTimeDistr_101::bExtractZDAData(const BYTE* pucMsg, const int nSize)
{
   bool bOK=false;
   char* pszSt=new char[nSize+1];
   memcpy(pszSt,pucMsg,nSize);
   pszSt[nSize]=0;
   String s=AnsiString(pszSt);
   delete[] pszSt;
   if (s.LastDelimiter(L"*")>0)
      {
      //Verify checksum
      int i=1;
      BYTE uc=0x00;
      while((s[i]!='*')&&(i<=nSize))
         {
         if (s[i]!='$') uc^=s[i];
         ++i;
         }
      if ((s[i]=='*')&&(i<nSize-1))
         bOK=((s[i+1]==H2C(uc>>4))&&(s[i+2]==H2C(uc&0x0F)));
      }
   if (bOK)
      {
      try
         {
         CommaText Txt(s);
         if (Txt.nGetNum()==7)
            {
				ZDATime.SetTime(Txt.nGetTime_ms(1,false));
            ZDATime.SetDate(Txt.nGetVal(2),Txt.nGetVal(3),Txt.nGetVal(4));
            }
         else
            bOK=false;
         }
      catch(...)
         {
         ZDATime.Invalidate();
         bOK=false;
         }
      }
   if (bOK)
      {
      dwLastZDATime=::GetTickCount();
      ++nZDAMsgCount;
      }
   return bOK;
}

int CommsTimeDistr_101::nReadTimeCRCErrors()
{
   DWORD dwStatus=dwRead(TD_STATUS_RD);
   dwStatus>>=TD_CRC_ERRORS_SHIFT;
   return (int)(dwStatus&TD_CRC_ERRORS_MASK);
}


void CommsTimeDistr_101::SetupUART(const int nBaud, const TIMEDISTR_UART_CHAN Chan)
{
   pLock->Acquire();
   dwSetup&=~ENABLE_UART;
   Write(SETUP_WR,dwSetup);
   DWORD dw=(CLOCK_FREQ*1e6)/(nBaud*2.0)+0.5;   //Half bit interval
   dwBaud&=(~UART_BAUD_MASK);
   dwBaud|=(dw<<UART_BAUD_SHIFT);
   Write(BAUD_WR,dwBaud);
   UARTChan=Chan;
   dwSetup&=(~UART_CHAN_MASK);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

void CommsTimeDistr_101::StartUART()
{
   pLock->Acquire();
   dwSetup|=RESET_UART;
   Write(SETUP_WR,dwSetup);
   dwSetup&=(~RESET_UART);
   dwSetup|=ENABLE_UART;
   dwSetup&=(~UART_CHAN_MASK);
   dwSetup|=(UARTChan<<UART_CHAN_SHIFT);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

void CommsTimeDistr_101::StopUART()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_UART);
   dwSetup&=(~UART_CHAN_MASK);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

int CommsTimeDistr_101::nRXUARTData(BYTE* pucData, const int nMaxSize)
{
   pLock->Acquire();
   DWORD dwSize=dwRead(UART_STATUS_RD);
   dwSize>>=UART_COUNT_SHIFT;
   dwSize&=UART_COUNT_MASK;
   if (dwSize>0)
      {
      if (dwSize>(DWORD)nMaxSize)
         dwSize=nMaxSize;
      DWORD* pdwRX=new DWORD[dwSize];
      Read(UART_DATA_RD,pdwRX,dwSize);
      for (DWORD dw=0; dw<dwSize; dw++)
         pucData[dw]=pdwRX[dw]&UART_DATA_MASK;
      delete[] pdwRX;
      }
   pLock->Release();
   return (int)dwSize;
}

int CommsTimeDistr_101::nTXUARTData(const BYTE* pucData, const int nSize)
{
   int nTXLeft=(dwRead(UART_STATUS_RD)>>UART_TX_LEFT_SHIFT)&UART_TX_LEFT_MASK;
   int nTXed;
   if (nTXLeft>=nSize)
      nTXed=nSize;
   else
      nTXed=nTXLeft;
   DWORD* pdwTX=new DWORD[nSize];
   for (int i=0; i<nSize; i++)
      pdwTX[i]=pucData[i];
   Write(UART_TX_WR,pdwTX,nTXed);
   delete[] pdwTX;
   return nTXed;
}

//*********************************************************************

void CommsTimeDistr_101::SetupINS(const int nBaud, const int nFrameSize)
{
   pLock->Acquire();
   dwSetup|=RESET_INS;
   dwSetup&=~ENABLE_INS;
   Write(SETUP_WR,dwSetup);
   DWORD dw=(CLOCK_FREQ*1e6)/(nBaud*2.0)+0.5;   //Half bit interval
   dwBaud&=(~INS_BAUD_MASK);
   dwBaud|=(dw<<INS_BAUD_SHIFT);
   Write(BAUD_WR,dwBaud);
   dwSetup&=(~INS_FRAME_MASK);
   dwSetup|=((nFrameSize-3)<<INS_FRAME_SHIFT);
   dwSetup&=(~RESET_INS);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

void CommsTimeDistr_101::StartINS(const bool bGPSTime)
{
   pLock->Acquire();
   dwSetup|=RESET_INS;
   dwSetup&=(~ENABLE_INS);
   Write(SETUP_WR,dwSetup);
   if (bGPSTime)
      dwSetup|=SEL_INS_TAMS;
   else
      dwSetup&=(~SEL_INS_TAMS);
   dwSetup&=(~RESET_INS);
   dwSetup|=ENABLE_INS;
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

void CommsTimeDistr_101::StopINS()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_INS);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

int CommsTimeDistr_101::nRXINSData(BYTE* pucData, const int nMaxSize)
{
   DWORD dwSize=dwRead(INS_ORT_STATUS_RD);
   dwSize>>=INS_COUNT_SHIFT;
   dwSize&=INS_COUNT_MASK;
   if (dwSize>0)
      {
      if (dwSize>(DWORD)nMaxSize)
         dwSize=nMaxSize;
      DWORD* pdwRX=new DWORD[dwSize];
      Read(INS_ORT_DATA_RD,pdwRX,dwSize);
      for (DWORD dw=0; dw<dwSize; dw++)
         pucData[dw]=pdwRX[dw]&INS_DATA_MASK;
      delete[] pdwRX;
      }
   return (int)dwSize;
}

//*********************************************************************

void CommsTimeDistr_101::StartORTCapt()
{
   nORTMsgOffs=-2;
   nORTMsgSize=0;

   dwSetup|=RESET_ORT;
   dwSetup&=(~ENABLE_ORT);
   Write(SETUP_WR,dwSetup);
   dwSetup&=(~RESET_ORT);
   dwSetup|=ENABLE_ORT;
   Write(SETUP_WR,dwSetup);
}

void CommsTimeDistr_101::StopORTCapt()
{
   dwSetup&=(~ENABLE_ORT);
   Write(SETUP_WR,dwSetup);
}

bool CommsTimeDistr_101::bCheckORTCRC(const BYTE* pucMsg, const int nSize) const
{
   pCRC->Reset();
   for (int i=0; i<nSize-2; i++)
      pCRC->Next(pucMsg[i]);
   WORD uRXCRC=(WORD)pCRC->ulGetCRC();
   BYTE ucMSB=pucMsg[nSize-2];
   BYTE ucLSB=pucMsg[nSize-1];
   BYTE ucMSB1=~ucMSB;
   BYTE ucLSB1=~ucLSB;
   ucMSB=0;
   ucLSB=0;
   BYTE uc1=0x01;
   BYTE uc2=0x80;
   for (int i=0; i<8; i++)
      {
      if (ucMSB1&uc1) ucMSB|=uc2;
      if (ucLSB1&uc1) ucLSB|=uc2;
      uc1<<=1;
      uc2>>=1;
      }
   WORD uTXCRC=(ucMSB<<8)|ucLSB;
   return (uRXCRC==uTXCRC);
}

void CommsTimeDistr_101::StartORTSim(const void* pData, const int nSize)
{
   dwSetup&=~RESET_ORT_SIM;
   Write(SETUP_WR,dwSetup);
   dwSetup|=RESET_ORT_SIM;
   Write(SETUP_WR,dwSetup);
   dwSetup&=~RESET_ORT_SIM;
   dwSetup|=SET_ORT_SIM;
   Write(SETUP_WR,dwSetup);
   BYTE* pucData=(BYTE*)pData;
   DWORD* pdwData=new DWORD[nSize];
   for (int i=0; i<nSize; i++)
      pdwData[i]=pucData[nSize-i-1];
   Write(ORT_SIM_DATA_WR,pdwData,nSize);
   delete[] pdwData;
   dwSetup&=~SET_ORT_SIM;
   dwSetup|=ENABLE_ORT_SIM;
   Write(SETUP_WR,dwSetup);

}

void CommsTimeDistr_101::StopORTSim()
{
   dwSetup&=~ENABLE_ORT_SIM;
   Write(SETUP_WR,dwSetup);
}

int CommsTimeDistr_101::nReadORTBuffer()
{
   DWORD dwCount=dwRead(INS_ORT_STATUS_RD);
   bORTClkPresent=(dwCount&ORT_CLOCK_PRESENT)!=0;
   dwCount>>=ORT_COUNT_SHIFT;
   dwCount&=ORT_COUNT_MASK;
   if (dwCount>0)
      Read(INS_ORT_DATA_RD,adwORTBuf,dwCount);
   nORTBufOffs=0;
   nORTBufSize=dwCount;
   return dwCount;
}

WORD CommsTimeDistr_101::uBitReverse(const WORD uData) const
{
   WORD u=0;
   for (int i=0; i<16; i++)
      {
      if (uData&(0x01<<i))
         u|=(0x8000>>i);
      }
   return u;
}

int CommsTimeDistr_101::nExtractORTMsg(void* pMsg, JTime& T, const int, bool& bCRCOK, WORD& uRXCRC)
{
   int nSize=0;
   while((nORTBufOffs<nORTBufSize)&&(nSize==0))
      {
      DWORD dwVal=adwORTBuf[nORTBufOffs++];
      switch(nORTMsgOffs)
         {
         case -2:
            nORTMsgSize=dwVal&FRAME_SIZE_MASK;
            bORTCRCOK=(dwVal&FRAME_CRC_OK)!=0;
            uORTCRC=(WORD)(dwVal>>FRAME_CRC_SHIFT);
            nORTMsgOffs=-1;
            break;
         case -1:
            dwORTTime=dwVal;
            nORTMsgOffs=0;
            break;
         default:
            aucORTMsg[nORTMsgOffs++]=(BYTE)(dwVal&0x000000FF);
            aucORTMsg[nORTMsgOffs++]=(BYTE)((dwVal>>8)&0x000000FF);
            aucORTMsg[nORTMsgOffs++]=(BYTE)((dwVal>>16)&0x000000FF);
            aucORTMsg[nORTMsgOffs++]=(BYTE)((dwVal>>24)&0x000000FF);
            if (nORTMsgOffs>=nORTMsgSize)
               {
               memcpy(pMsg,aucORTMsg,nORTMsgSize);
               nSize=nORTMsgSize;
/*
               JFile F('O',JFile::ASCII_TYPE);
               F.Create(L"M101.txt");
               String s;
               for (int i=0;i<nORTMsgSize; i++)
                  {
                  s.printf(L"%3d %02X",i,aucORTMsg[i]);
                  F.TextLine(s);
                  }
               F.Close();
*/
               RockCRC CRC(16,0x1021,0xFFFF,true,false,0x0000);   //CRC-16
               for (int i=0; i<nSize-2; i++)
                  CRC.Next(aucORTMsg[i]);
               uRXCRC=(WORD)CRC.ulGetCRC();
               uRXCRC=uBitReverse(~uRXCRC);

               bCRCOK=(uRXCRC==uORTCRC);

//               bCRCOK=bCheckCRC(pucMsg,nSize);
               if (dwORTTime&0x80000000)
                  T=CalcORTTime(dwORTTime);
               else
                  T.Invalidate();
               nORTMsgOffs=-2;
               }
         }
      }
   return nSize;
}


JTime CommsTimeDistr_101::CalcORTTime(const int nTime)
{
   JTime T=GetZDATime();
   if (!T.bValid())
      T=JTime(true);
   int nT=nTime;
   nT&=0x7FFFFFFF;
   nT-=nLeap_ms*10;     //Subtract leap sec 
   nT+=72000000;        //Add 2 hours
   if (nT<0)
      nT+=864000000;
   else if (nT>=864000000)
      nT-=864000000;
   T.SetTime(nT/10);    //Set time as ms
   T.SetMicroSec((nT%10)*100);
   return T;
}

/*
String CommsTimeDistr_101::sGetPOS()
{
   String s;
   pLock->Acquire();
   s=sPOS;
   pLock->Release();
   return s;
}

*/


void CommsTimeDistr_101::SetMRRIncrDec(const int nACPNum,
                                   const double dARP,
                                   const bool bInvertACP,
                                   const bool bInvertNRP,
                                   const double dNRPFilter,
                                   const double dACPFilter)
{
   dwSetup|=MRR_SELECT;
   dwSetup&=(~MRR_ENABLE);
   if (bInvertACP)
      dwSetup|=MRR_INV_ACP;
   else
      dwSetup&=(~MRR_INV_ACP);
   if (bInvertNRP)
      dwSetup|=MRR_INV_NRP;
   else
      dwSetup&=(~MRR_INV_NRP);
   Write(SETUP_WR,dwSetup);

   double dClockPeriod=1e-6/CLOCK_FREQ;
   int nDiv=(dARP/65536.0)/dClockPeriod-1.0;
   DWORD dw=nDiv;
   Write(MRR_ACP_DIV_WR,dw);

   int nACP=dACPFilter*CLOCK_FREQ-1;
   dw&=(~MRR_ACP_DEL_MASK);
   dw|=(nACP<<MRR_ACP_DEL_SHIFT);
   int nNRP=dNRPFilter*CLOCK_FREQ-1;
   dw&=(~MRR_NRP_DEL_MASK);
   dw|=(nNRP<<MRR_NRP_DEL_SHIFT);
   Write(MRR_DEL_WR,dw);

   dw&=(~MRR_MAX_ACP_MASK);
   dw|=(nACPNum<<MRR_MAX_ACP_SHIFT);
   DWORD dwACP=(dARP/nACPNum)*1e6-1;   //Gap between ACP in microsec (for sim only)
   dw&=(~MRR_ACP_INT_MASK);
   dw|=(dwACP<<MRR_ACP_INT_SHIFT);
   Write(MRR_ACP_MAX_INT_WR,dw);

}

void CommsTimeDistr_101::EnableMRR(const bool bEn, const bool bSim)
{
   if (bEn)
      {
      dwSetup|=MRR_SELECT;
      dwSetup|=MRR_RESET;
      dwSetup&=(~MRR_ENABLE);
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~MRR_RESET);
      Write(SETUP_WR,dwSetup);
      dwSetup|=MRR_ENABLE;
      if (bSim)
         dwSetup|=MRR_SEL_SIM;
      else
         dwSetup&=(~MRR_SEL_SIM);
      }
   else
      {
      dwSetup&=(~MRR_SELECT);
      dwSetup&=(~MRR_ENABLE);
      }
   Write(SETUP_WR,dwSetup);
}

double CommsTimeDistr_101::dReadMRRAngle(int& nMaxCount, double& dARP)
{
   DWORD dw=dwRead(MRR_ANGLE_RD);
   double dBe=((dw>>MRR_ANGLE_SHIFT)&MRR_ANGLE_MASK)*360.0/65536.0;
   nMaxCount=((dw>>MRR_MAX_CNT_SHIFT)&MRR_MAX_CNT_MASK);
   dw=dwRead(MRR_ARP_RD);
   dARP=dw/(CLOCK_FREQ*1e6);
   return dBe;
}

void CommsTimeDistr_101::ReadMRRWatchdogs(bool& bACPWatch, bool& bNRPWatch)
{
   DWORD dw=dwRead(MRR_STATUS_RD);
   bACPWatch=(dw&MRR_ACP_PRESENT)!=0;
   bNRPWatch=(dw&MRR_NRP_PRESENT)!=0;
}

int CommsTimeDistr_101::nReadMRRSectorTimes(DWORD* pdwData)   //Max = 256 DWORDS
{
   DWORD dw=dwRead(MRR_STATUS_RD);
   int nSize=(dw>>MRR_FIFO_SIZE_SHIFT)&MRR_FIFO_SIZE_MASK;
   Read(MRR_FIFO_RD,pdwData,nSize);
   return nSize;
}

