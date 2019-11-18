//---------------------------------------------------------------------------
#ifndef USBSerial22H
#define USBSerial22H
//---------------------------------------------------------------------------

#include "FTD2XX.h"

class USBSerial2
{

   FT_HANDLE hFT;
   FT_STATUS Status;

   CRITICAL_SECTION CriticalSection;

public:

   USBSerial2(const int _nBaud, const char _cParity, const int _nDataBits=8, const int _nStopBits=1, const bool _b232=true, const int nDevNum=-1);
   USBSerial2(const int nDevNum=-1);
   ~USBSerial2();

   bool bOK() const
   {
      return (hFT!=NULL);
   }

   bool bPortValid() const //for compatibility with PortThread
   {
      return (hFT!=NULL);
   }

   void Purge()
   {
      Status=FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
   }

   void Acquire()
   {
      ::EnterCriticalSection(&CriticalSection);
   }

   void Release()
   {
      ::LeaveCriticalSection(&CriticalSection);
   }

   void Setup(const int _nBaud, const char _cParity='N', const int _nDataBits=8, const int _nStopBits=1, const bool _b232=true);

   int nRXData(BYTE* pucData, const int nMaxSize);

   int nTXData(BYTE* pucData, const int nSize);


};

#endif
