//---------------------------------------------------------------------------
#ifndef USB_FT232H
#define USB_FT232H
//---------------------------------------------------------------------------

#include "FTD2XX.h"
#include "USB_FT_Devices.h"

class USB_FT232
{

   FT_HANDLE hFT;
   FT_STATUS Status;
   int nDevice;
   CRITICAL_SECTION CriticalSection;

public:

   USB_FT232(const int _nBaud, const char _ucParity='N', const int _nDataBits=8, const int _nStopBits=1, const int _nDevice=USB_AGGR_DEV, const int nSerialNum=-1);
   ~USB_FT232();

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

   void Setup(const int _nBaud, const char _ucParity='N', const int _nDataBits=8, const int _nStopBits=1);

   int nRXData(BYTE* pucData, const int nMaxSize);

   int nTXData(BYTE* pucData, const int nSize);

   String sGetDescr(const int nDev=-1);


};

#endif
