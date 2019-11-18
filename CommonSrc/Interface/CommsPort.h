//---------------------------------------------------------------------------

#ifndef CommsPortH
#define CommsPortH
//---------------------------------------------------------------------------

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

class PortThread : public TThread
{

   int nPort;
   int nBaud;
   char cParity;
   int nDataBits;
   int nStopBits;
   bool bDeviceCreated;

   void __fastcall CreatePort();

protected:

   OVERLAPPED RXOverLap,TXOverLap;
   HANDLE hRXEvent,hTXEvent,hCommDevice;
   CRITICAL_SECTION CriticalSection;
   bool bOK;

   void __fastcall SetDevice(const HANDLE hDevice);


public:

   virtual __fastcall PortThread(const HANDLE hDevice=0);

   virtual __fastcall PortThread(const int _nPort=1,
                                 const int _nBaud=9600,
                                 const char _cParity='N',
                                 const int _nDataBits=8,
                                 const int _nStopBits=1);

   virtual __fastcall ~PortThread();

   void __fastcall Terminate();

   void SetPriority(const TThreadPriority _Priority)
   {
      Priority=_Priority;
   }

   bool bPortValid() const
   {
      return bOK;
   }

   void __fastcall SetCritical()
   {
      ::EnterCriticalSection(&CriticalSection);
   }

   void __fastcall ReleaseCritical()
   {
      ::LeaveCriticalSection(&CriticalSection);
   }

   void __fastcall Close();


};

#endif
