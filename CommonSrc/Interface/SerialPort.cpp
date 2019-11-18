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

#include "SerialPort.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall SerialPort::SerialPort(  const int _nPort,
                                    const int _nBaud,
                                    const char _ucParity,
                                    const int _nDataBits,
                                    const int _nStopBits)
                                 : PortThread(_nPort,_nBaud,_ucParity,_nDataBits,_nStopBits)
{
   nRXIndex=0;
   Resume();
}

void __fastcall SerialPort::Execute()
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

void __fastcall SerialPort::AddData(const BYTE ucData)
{
   SetCritical();
   if (nRXIndex<COMM_BUFFER_SIZE)
      aucRX[nRXIndex++]=ucData;
   ReleaseCritical();
}

int __fastcall SerialPort::nRXData(BYTE* pucData)
{
   int nSize=0;
   SetCritical();
   if (nRXIndex>0)
      {
      memcpy(pucData,aucRX,nRXIndex);
      nSize=nRXIndex;
      nRXIndex=0;
      }
   ReleaseCritical();
   return nSize;
}

void __fastcall SerialPort::SendData(const BYTE* pucTXData, const int nSize, const bool bWait)
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




