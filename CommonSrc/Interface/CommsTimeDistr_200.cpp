/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	NA-10 Interface
 *		PROJECT			Aegis III C1410
 *		ACTIVITY       6
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP SP1
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      1.00: Initial Version JGT 20-10-2003
************************************************************************/

//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "CommsTimeDistr_200.h"
#include "CommsIntDeviceInterface.h"
#include "JFile.h"
#include "JUtils.h"
#include "JMessageBar.h"
#include "GPSTime.h"
#include "JCommaText.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

GUID g_CommsInt200_GUID=CommsIntDevice_CLASS_GUID;

CommsTimeDistr_200::CommsTimeDistr_200(const String sRBF):PCI32_WDM_Int(PCI32_WDM_Int::PCI_9030,"Communications Interface Card",1000,g_CommsInt200_GUID)
{
   pLock=new TCriticalSection;
   if (bDeviceOK)
      {
      if (sRBF.IsEmpty())
         bDeviceOK=bConfigFlex("Comms_TimeDistr_200.rbf");
      else
         bDeviceOK=bConfigFlex(sRBF);
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
            ErrorMessage("Cyclone Configuration Loopback Failed");
            bDeviceOK=false;
            }
         }
      }
   pCRC=new RockCRC(16,0x1021,0xFFFF,true,false,0x0000);   //CRC-16
   TD2_PrevLL.Invalidate();
   sTD2_TempGGA="";
   sTD2_GGA="";
   nTD2_CS=0;
   nGGAMsgCnt=0;
	dwTD2_PrevLLTime=0;
	nTimeZoneBias_hours=nGetTimeZoneBias_hours();
}

CommsTimeDistr_200::~CommsTimeDistr_200()
{
   delete pCRC;
   delete pLock;
}

bool CommsTimeDistr_200::bLoopBackOK()
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

void CommsTimeDistr_200::StartTDCapt()
{
   pLock->Acquire();
   TD2_PrevLL.Invalidate();
   sTD2_TempGGA="";
   sTD2_GGA="";
   nTD2_CS=0;
   dwTD2_PrevLLTime=0;
   dwSetup|=RESET_TD;
   Write(SETUP_WR,dwSetup);
   dwSetup&=(~RESET_TD);
   dwSetup|=ENABLE_TD;
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

void CommsTimeDistr_200::StopTDCapt()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_TD);
   Write(SETUP_WR,dwSetup);
//   delete pFil;
   pLock->Release();
}

bool CommsTimeDistr_200::bCheckCRC(const BYTE* pucMsg, const int nSize) const
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

JTime CommsTimeDistr_200::ReadTime(const bool bUTCTime)
{
	pLock->Acquire();
	SelTD2_GGA(false);
	DWORD dwTime=dwRead(TD2_DATA_RD);
	DWORD dwDate=dwRead(TD2_STATUS_RD);
	pLock->Release();
	JTime JT;

	if (dwTime&TD2_TIME_VALID)
		{
		JT.SetDCSDate(dwDate);
		JT.SetDCSTime(dwTime);
      JT.SetUTC();	//Always gets utc from time distr
		if (!bUTCTime)
			JT.ConvertToLocal(nTimeZoneBias_hours);	//Will clear UTC bit
		}
/*
	if (dwTime&TD2_TIME_VALID)
		{
SYSTEMTIME T;
		memset(&T,0,sizeof(T));
		DWORD dwMs=dwTime&TD2_TIME_MASK;
		bool bNextDay;
		if (dwMs>864000000)  //was in last second of day
			{
			dwMs-=864000000;
			bNextDay=true;
			}
		else
			bNextDay=false;
		T.wMilliseconds=(dwMs%10000)/10;
		int nMicroSec=(dwMs%10)*100;
		dwMs/=10000;
		T.wHour=dwMs/3600;
		T.wMinute=(dwMs%3600)/60;
		T.wSecond=dwMs%60;
		T.wDay=(dwDate>>TD2_DAY_SHIFT)&TD2_DAY_MASK;
		T.wMonth=(dwDate>>TD2_MONTH_SHIFT)&TD2_MONTH_MASK;
		T.wYear=((dwDate>>TD2_YEAR_SHIFT)&TD2_YEAR_MASK)+2000;
		JT=JTime(T);
		if (bNextDay)
			JT.AddDays(1);
		if (!bUTCTime)
			{
			JT.ConvertToLocal(nTimeZoneBias_hours);
			JT.ClearUTC();
			}
		else
			JT.SetUTC();
		JT.SetMicroSec(nMicroSec);
		}
*/
	else
		JT.Invalidate();

	return JT;
}

bool CommsTimeDistr_200::bWaitForTime()
{
	bool bOK;
	DWORD dwT=::GetTickCount();
	do
		{
		bOK=bTimeValid();
		Application->ProcessMessages();
		}
	while((!bOK)&&(::GetTickCount()-dwT<3000));
	return bOK;
}

String CommsTimeDistr_200::sReadGGA()
{
   pLock->Acquire();
   String sMsg="";
   SelTD2_GGA(true);
   DWORD dw=dwRead(TD2_STATUS_RD);
   int nSize=(dw>>TD2_GGASIZE_SHIFT)&TD2_GGASIZE_MASK;
   if (nSize>0)
      {
      DWORD* pdwBuf=new DWORD[nSize];
      Read(TD2_DATA_RD,pdwBuf,nSize);
      for (int i=0; i<nSize; i++)
         {
         char c=(char)(pdwBuf[i]&TD2_GGA_MASK);
         switch(c)
            {
            case '$':
               sTD2_TempGGA="$";
               nTD2_CS=0;
               break;
            case '*':
               sTD2_TempGGA=sTD2_TempGGA+c;
               nTD2_CS=1;
               break;
            default: ;
               sTD2_TempGGA=sTD2_TempGGA+c;
               if (nTD2_CS>0)
                  ++nTD2_CS;
               if (nTD2_CS==3)
                  {
                  nTD2_CS=0;
                  sMsg=sTD2_TempGGA;
                  }
            }
         }
      delete[] pdwBuf;
      }
   pLock->Release();
   return sMsg;
}


LatLong CommsTimeDistr_200::ReadLL(const int nTimeoutMs)
{
   String s=sReadGGA();
   DWORD dwT=::GetTickCount();
   if (s.IsEmpty())
      {
      if ((int)(dwT-dwTD2_PrevLLTime)>nTimeoutMs)
         {
         TD2_PrevLL.Invalidate();
         sTD2_GGA="";
         }
      }
   else
      {
      sTD2_GGA=s;
      CommaText Txt(s);
      if (Txt.nGetNum()>=6)
         {
         try
            {
            TD2_PrevLL.dLat=Txt.dGetLat(2,3);
            TD2_PrevLL.dLong=Txt.dGetLong(4,5);
            dwTD2_PrevLLTime=dwT;
            ++nGGAMsgCnt;
            }
         catch(...)
            {
            TD2_PrevLL.Invalidate();
            }
         }
      }
   return TD2_PrevLL;
}

void CommsTimeDistr_200::ExtractGGAData(GGADataStruct& Data)
{
/*
   bool bDiff;
   int nSat;
   JTime T;          //Time Only
   LatLong LL;
   double dAlt;      // m
   double dHDOP;
*/
   Data.LL.Invalidate();
   String s=sGetGGA();
   if (!s.IsEmpty())
      {
      CommaText Txt(s);
      if (Txt.nGetNum()>=10)
         {
         try
            {
				int n=Txt.nGetTime_ms(1);
            Data.T.SetTime(n);
            Data.LL.dLat=Txt.dGetLat(2,3);
            Data.LL.dLong=Txt.dGetLong(4,5);
            n=Txt.nGetVal(6);
            switch(n)
               {
               case 1: Data.bDiff=false; break;
               case 2: Data.bDiff=true; break;
               default: Data.LL.Invalidate();
               }
            Data.nSat=Txt.nGetVal(7);
            Data.dHDOP=Txt.dGetVal(8);
            Data.dAlt=Txt.dGetVal(9);
            }
         catch(...)
            {
            Data.LL.Invalidate();
            }
         }
      }


}

void CommsTimeDistr_200::SetupUART(const int nBaud, const TIMEDISTR_UART_CHAN Chan)
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

void CommsTimeDistr_200::StartUART()
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

void CommsTimeDistr_200::StopUART()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_UART);
   dwSetup&=(~UART_CHAN_MASK);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

int CommsTimeDistr_200::nRXUARTData(BYTE* pucData, const int nMaxSize)
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

int CommsTimeDistr_200::nTXUARTData(const BYTE* pucData, const int nSize)
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

void CommsTimeDistr_200::SetupINS(const int nBaud, const int nFrameSize)
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

void CommsTimeDistr_200::StartINS(const bool bGPSTime)
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

void CommsTimeDistr_200::StopINS()
{
   pLock->Acquire();
   dwSetup&=(~ENABLE_INS);
   Write(SETUP_WR,dwSetup);
   pLock->Release();
}

int CommsTimeDistr_200::nRXINSData(BYTE* pucData, const int nMaxSize)
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

void CommsTimeDistr_200::StartORTCapt()
{
   nORTMsgOffs=-3;   //NB allow one extra for date
   nORTMsgSize=0;

   dwSetup|=RESET_ORT;
   dwSetup&=(~ENABLE_ORT);
   Write(SETUP_WR,dwSetup);
   dwSetup&=(~RESET_ORT);
   dwSetup|=ENABLE_ORT;
   Write(SETUP_WR,dwSetup);
}

void CommsTimeDistr_200::StopORTCapt()
{
   dwSetup&=(~ENABLE_ORT);
   Write(SETUP_WR,dwSetup);
}

bool CommsTimeDistr_200::bCheckORTCRC(const BYTE* pucMsg, const int nSize) const
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

void CommsTimeDistr_200::StartORTSim(const void* pData, const int nSize)
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

void CommsTimeDistr_200::StopORTSim()
{
   dwSetup&=~ENABLE_ORT_SIM;
   Write(SETUP_WR,dwSetup);
}

int CommsTimeDistr_200::nReadORTBuffer()
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

WORD CommsTimeDistr_200::uBitReverse(const WORD uData) const
{
   WORD u=0;
   for (int i=0; i<16; i++)
      {
      if (uData&(0x01<<i))
         u|=(0x8000>>i);
      }
   return u;
}

int CommsTimeDistr_200::nExtractORTMsg(void* pMsg, JTime& T, const int /*nMaxSize*/, bool& bCRCOK, WORD& uRXCRC)
{
   int nSize=0;
   while((nORTBufOffs<nORTBufSize)&&(nSize==0))
      {
      DWORD dwVal=adwORTBuf[nORTBufOffs++];
      switch(nORTMsgOffs)
         {
         case -3:
            nORTMsgSize=dwVal&FRAME_SIZE_MASK;
            bORTCRCOK=(dwVal&FRAME_CRC_OK)!=0;
            uORTCRC=(WORD)(dwVal>>FRAME_CRC_SHIFT);
            nORTMsgOffs=-2;
            break;
         case -2:
            dwORTTime=dwVal;
            nORTMsgOffs=-1;
            break;
         case -1:
            dwORTDate=dwVal;
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
               F.Create("M200_1.txt");
               String s;
               for (int i=0;i<nORTMsgSize; i++)
                  {
                  s.printf("%3d %02X",i,aucORTMsg[i]);
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
                  T=CalcORTTime(dwORTTime,dwORTDate);
               else
                  T.Invalidate();
               nORTMsgOffs=-3;
               }
         }
      }
   return nSize;
}


JTime CommsTimeDistr_200::CalcORTTime(const int nTime, const int nDate)
{
   JTime T;
   int nT=nTime;
   nT&=0x7FFFFFFF;
   nT+=72000000;  //Add 2 hours to make SAST from UTC (from ver 1.1.3.00)
   bool bNextDay;
   if (nT>=864000000)
      {
      bNextDay=true;    //After midnight SAST
      nT-=864000000;
      }
   else
      bNextDay=false;
   T.SetTime(nT/10); //Set time as ms
   T.SetMicroSec((nT%10)*100);

   WORD wDay=(nDate>>TD2_DAY_SHIFT)&TD2_DAY_MASK;
   WORD wMonth=(nDate>>TD2_MONTH_SHIFT)&TD2_MONTH_MASK;
   WORD wYear=((nDate>>TD2_YEAR_SHIFT)&TD2_YEAR_MASK)+2000;

   T.SetDate(wDay,wMonth,wYear);

   if (bNextDay)
      T.AddDays(1);

   return T;
}

/*
String CommsTimeDistr_200::sGetPOS()
{
   String s;
   pLock->Acquire();
   s=sPOS;
   pLock->Release();
   return s;
}

*/


void CommsTimeDistr_200::SetMRRIncrDec(const int nACPNum,
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

void CommsTimeDistr_200::EnableMRR(const bool bEn, const bool bSim)
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

double CommsTimeDistr_200::dReadMRRAngle(int& nMaxCount, double& dARP)
{
   DWORD dw=dwRead(MRR_ANGLE_RD);
   double dBe=((dw>>MRR_ANGLE_SHIFT)&MRR_ANGLE_MASK)*360.0/65536.0;
   nMaxCount=((dw>>MRR_MAX_CNT_SHIFT)&MRR_MAX_CNT_MASK);
   dw=dwRead(MRR_ARP_RD);
   dARP=dw/(CLOCK_FREQ*1e6);
   return dBe;
}

void CommsTimeDistr_200::ReadMRRWatchdogs(bool& bACPWatch, bool& bNRPWatch)
{
   DWORD dw=dwRead(MRR_STATUS_RD);
   bACPWatch=(dw&MRR_ACP_PRESENT)!=0;
   bNRPWatch=(dw&MRR_NRP_PRESENT)!=0;
}

int CommsTimeDistr_200::nReadMRRSectorTimes(DWORD* pdwData)   //Max = 256 DWORDS
{
   DWORD dw=dwRead(MRR_STATUS_RD);
   int nSize=(dw>>MRR_FIFO_SIZE_SHIFT)&MRR_FIFO_SIZE_MASK;
   Read(MRR_FIFO_RD,pdwData,nSize);
   return nSize;
}

