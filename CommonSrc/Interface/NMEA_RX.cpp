/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	VTSTrack TAPI NMEA Data Receiver 
 *		PROJECT			CoastRad C1274
 *		ACTIVITY       2
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *
************************************************************************///---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "NMEA_RX.h"
#include "JCommaText.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall NMEA_RX::NMEA_RX(  const int _nPort,
                              const int _nBaud,
                              const char _cParity,
                              const int _nDataBits,
                              const int _nStopBits)
                                 : PortThread(_nPort,_nBaud,_cParity,_nDataBits,_nStopBits)
{
   nRXIndex=-1;
   nRXNum=0;
	nReadNum=0;
	nRawBufferSize=0;
//   SetPriority(tpTimeCritical);
   Resume();
}

void __fastcall NMEA_RX::Execute()
{
   DWORD dwBytesRead;
   BYTE ucData;
   while(!Terminated)
      {
      if (!::ReadFile(hCommDevice,&ucData,1,&dwBytesRead,&RXOverLap))
         {
         if (::GetLastError()!=ERROR_IO_PENDING)
            {
            }
         else
            {
            ::WaitForSingleObjectEx(hRXEvent,INFINITE,true);
            ::GetOverlappedResult(hCommDevice,&RXOverLap,&dwBytesRead,false);
            if (dwBytesRead)
               AddData(ucData);
            }
         }
      else
         {
         if (dwBytesRead>0)
            AddData(ucData);
         }
      }
}

void __fastcall NMEA_RX::AddData(const BYTE ucData)
{
	if (nRawBufferSize<MAX_RAW_BUFFER)
		aucRawBuffer[nRawBufferSize++]=ucData;;
   if (bStartChar(ucData))
      nRXIndex=0;
   if (nRXIndex>=0)
      {
      szRXBuffer[nRXIndex]=ucData;
      if ((nRXIndex>0)&&(ucData=='\n')&&(szRXBuffer[nRXIndex-1]=='\r'))
         {
         //Sentence completed
         szRXBuffer[nRXIndex-1]=0;
         SetCritical();
         asRX[nRXNum]=szRXBuffer;
         nRXNum++;
         if (nRXNum>=MAX_SENTENCES)
            nRXNum=0;
         ReleaseCritical();
         }
      if (nRXIndex<COMM_BUFFER_SIZE-1) nRXIndex++;
      }
}

String __fastcall NMEA_RX::sGetSentence(const bool bUseChk)
{
   String s="";
   SetCritical();
   if (nRXNum!=nReadNum)
      {
      s=asRX[nReadNum];
      nReadNum++;
      if (nReadNum>=MAX_SENTENCES)
         nReadNum=0;
      }
   ReleaseCritical();
   int nLen=s.Length();   
   if (nLen>3)
      {
      if (bUseChk)
         {
         if (s.LastDelimiter("*")>0)
            {
            //Verify checksum
            int i=1;
            BYTE uc=0x00;
            while((s[i]!='*')&&(i<=nLen))
               {
               if (!bStartChar(s[i])) uc^=s[i];
               ++i;
               }
            bool bOK;
            if ((s[i]=='*')&&(i<nLen-1))
               bOK=((s[i+1]==H2C(uc>>4))&&(s[i+2]==H2C(uc&0x0F)));
            else
               bOK=false;
            if (bOK)
               s=s.SubString(2,i-2);   //Strip $,* and checksum off
            else
               {
               s="";
               nCheckErrors++;
               }
            }
         }
      else
         {
         s=s.SubString(2,s.Length()-1);   //Strip $ off
         }
      }
   else
      s="";
   return s;
}

char __fastcall NMEA_RX::H2C(const BYTE uc)
{
   if (uc<10)
      return uc+'0';
   else
      return (uc-10)+'A';
}

void __fastcall NMEA_RX::SendData(const BYTE* pucTXData, const int nSize, const bool bWait)
{
   DWORD dwWritten=0;
   if (!WriteFile(hCommDevice,pucTXData,nSize,&dwWritten,&TXOverLap))
      {
      if (GetLastError()!=ERROR_IO_PENDING)
         {
         }
      else
         {
         if (bWait)
            {
            ::WaitForSingleObjectEx(hTXEvent,INFINITE,true);
            DWORD dwBytes;
            ::GetOverlappedResult(hCommDevice,&TXOverLap,&dwBytes,false);
            if (dwBytes==(DWORD)nSize)
               {
               }
            }
         }
      }
}

void __fastcall NMEA_RX::TXSentence(const String sData, const String sPrefix)
{
   BYTE uc=0x00;
   for (int i=1; i<=sData.Length(); i++)
      uc^=sData[i];
   char cMSN=uc>>4;
   if (cMSN<10) cMSN+='0'; else cMSN+='A'-10;
   char cLSN=uc&0x0f;
   if (cLSN<10) cLSN+='0'; else cLSN+='A'-10;
   AnsiString sChk;
   sChk.printf("*%c%c\r\n",cMSN,cLSN);
   AnsiString s=sPrefix+sData+sChk;
   SendData(s.c_str(),s.Length(),false);
//   SendData(s.c_str(),s.Length(),true);
}

void __fastcall NMEA_RX::TXRawString(const String s)
{
	AnsiString s1=s;
	SendData(s1.c_str(),s1.Length(),false);
}

