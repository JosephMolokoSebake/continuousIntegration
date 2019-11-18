/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Communications Interface Thread (using TAPI)
 *    FILE NAME		CommsPort
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium III
 *		OS					MS Windows 2000 SP2
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 05/04/2002
*************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "CommsPort.h"
#include "JFile.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall PortThread::PortThread(const HANDLE hDevice) : TThread(true)
{
   hRXEvent=::CreateEvent(NULL,false,false,NULL);
   hTXEvent=::CreateEvent(NULL,false,false,NULL);
   memset(&RXOverLap,0,sizeof(RXOverLap));
   memset(&TXOverLap,0,sizeof(TXOverLap));
   RXOverLap.hEvent=hRXEvent;
   TXOverLap.hEvent=hTXEvent;
   ::InitializeCriticalSection(&CriticalSection);
   Priority=tpNormal;
   bDeviceCreated=false;
   bOK=false;
   hCommDevice=hDevice;
   if (hCommDevice)
      SetDevice(hCommDevice);
}

__fastcall PortThread::PortThread(const int _nPort,
                                  const int _nBaud,
                                  const char _cParity,
                                  const int _nDataBits,
                                  const int _nStopBits) : TThread(true)
{
   hRXEvent=::CreateEvent(NULL,false,false,NULL);
   hTXEvent=::CreateEvent(NULL,false,false,NULL);
   nPort=_nPort;
   nBaud=_nBaud;
   cParity=_cParity;
   nDataBits=_nDataBits;
   nStopBits=_nStopBits;
   CreatePort();
   memset(&RXOverLap,0,sizeof(RXOverLap));
   memset(&TXOverLap,0,sizeof(TXOverLap));
   RXOverLap.hEvent=hRXEvent;
   TXOverLap.hEvent=hTXEvent;
   ::InitializeCriticalSection(&CriticalSection);
   Priority=tpNormal;
   bDeviceCreated=true;
}

__fastcall PortThread::~PortThread()
{
   ::DeleteCriticalSection(&CriticalSection);
   ::PurgeComm(hCommDevice,PURGE_RXCLEAR);
   if (bDeviceCreated)
      ::CloseHandle(hCommDevice);
   ::CloseHandle(hRXEvent);
   ::CloseHandle(hTXEvent);
}

void __fastcall PortThread::SetDevice(const HANDLE hDevice)
{
   hCommDevice=hDevice;
   bOK=true;
   if (!::SetupComm(hCommDevice,1024,1024)) bOK=false;
   COMMPROP Prop;
   DCB State;
   COMMTIMEOUTS TimeOut;
   if (!::GetCommProperties(hCommDevice,&Prop)) bOK=false;
   if (!::GetCommTimeouts(hCommDevice,&TimeOut)) bOK=false;
   TimeOut.ReadIntervalTimeout=0;
   TimeOut.ReadTotalTimeoutMultiplier=2;
   TimeOut.ReadTotalTimeoutConstant=100;
   if (!::SetCommTimeouts(hCommDevice,&TimeOut)) bOK=false;
   if (!::PurgeComm(hCommDevice,PURGE_RXCLEAR)) bOK=false;
   if (!bOK)
      {
      DWORD dwErrors;
      ::ClearCommError(hCommDevice,&dwErrors,NULL);
      }
}

void __fastcall PortThread::CreatePort()
{
	TCHAR szPort[5]=L"COM?";
   szPort[3]=(char)(nPort+'0');
   hCommDevice=::CreateFile(  szPort,
                              GENERIC_READ|GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
										FILE_FLAG_OVERLAPPED,
                              NULL);
   if (hCommDevice!=INVALID_HANDLE_VALUE)
      {
      bOK=true;
      if (!::SetupComm(hCommDevice,1024,1024)) bOK=false;
      COMMPROP Prop;
      DCB State;
      COMMTIMEOUTS TimeOut;
      if (bOK)
         if (!::GetCommProperties(hCommDevice,&Prop)) bOK=false;
      if (bOK)
         if (!::GetCommState(hCommDevice,&State)) bOK=false;
      State.DCBlength=sizeof(DCB);
      State.fBinary=1;
      State.BaudRate=nBaud;
      State.fOutxDsrFlow=false;
      State.fOutxCtsFlow=false;
      State.fDtrControl=DTR_CONTROL_DISABLE;
      State.fOutX=false;
      State.fInX=false;
      State.fNull=false;
      State.fRtsControl=RTS_CONTROL_DISABLE; //RTS_CONTROL_HANDSHAKE;
      State.ByteSize=nDataBits;
      switch(cParity)
         {
         case 'E':
            State.fParity=1;
            State.Parity=EVENPARITY;
            break;
         case 'O':
            State.fParity=1;
            State.Parity=ODDPARITY;
            break;
         default:
            State.fParity=0;
            State.Parity=NOPARITY;
            break;
         }
      switch(nStopBits)
         {
         case 2:
            State.StopBits=TWOSTOPBITS;
            break;
         default:
            State.StopBits=ONESTOPBIT;
         }
      if (bOK)
         if (!::SetCommState(hCommDevice,&State)) bOK=false;
      if (bOK)
         if (!::GetCommTimeouts(hCommDevice,&TimeOut)) bOK=false;
      TimeOut.ReadIntervalTimeout=0;
      TimeOut.ReadTotalTimeoutMultiplier=2;
      TimeOut.ReadTotalTimeoutConstant=100;
      if (bOK)
         if (!::SetCommTimeouts(hCommDevice,&TimeOut)) bOK=false;
      if (bOK)
         if (!::PurgeComm(hCommDevice,PURGE_RXCLEAR)) bOK=false;
      if (!bOK)
         {
         DWORD dwErrors;
         if (bOK)
            ::ClearCommError(hCommDevice,&dwErrors,NULL);
         }
      }
   else
      bOK=false;
   if (!bOK)
      ShowLastError("COM Port Initialisation Failure");
}

void __fastcall PortThread::Terminate()
{
   SetEvent(hRXEvent);
   SetEvent(hTXEvent);
   TThread::Terminate();
}

void __fastcall PortThread::Close()
{
   if (hCommDevice)
      {
      Terminate();
      while(!Terminated){};  //Wait for thread to terminate
      }
}
