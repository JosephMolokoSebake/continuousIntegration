//---------------------------------------------------------------------------

#ifndef USBSerialH
#define USBSerialH


#include "USBSerial2.h"
#include "USBSerial3.h"
#include "USBSerial3100.h"
#include "USBSerial31_4ch.h"

//---------------------------------------------------------------------------

class USBSerial
{

   USBSerial2* pUSB2;
   USBSerial3* pUSB3;
   USBSerial3100* pUSB3100;
   USBSerial31_4ch* pUSB4ch;

   int nHWType;

   bool b422;

   int nAutoDetect(const int nDevSerial=-1);


public:

   USBSerial(const int nHWType=USB_DETECT_DEV, const int nDevSerial=-1);
   ~USBSerial();

   bool bOK();

   void Purge();

   void Reset();
   bool bUSBLoopbackOK();

	String sGetDescr();

   double dGetFWVersion() const;
   int nGetFWVersion() const;
   bool bFWVersionOK(const int nVer) const;
   int nGetFWChannels() const;
   bool bVersionOK();

   void SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits, const bool _b422=true);
	void StartUART(const bool b422, const bool bAlwaysEn422TX=true);
   void StopUART();

//4 channel only
   void SetupUART(const int nChan, const int nBaud, const char cParity, const int nDataBits, const int nStopBits);
   void StartUART(const int nChan, const bool b422, const bool bAlwaysEn422TX=true);
   void StopUART(const int nChan);
   bool bServiceMulti();
   int nGetMultiData(const int nChan, BYTE* pucData, const int nMaxSize);  //Chan 0..2

   int nRXUARTData(BYTE* pucData, const int nMaxSize);
   int nTXUARTData(const BYTE* pucData, const int nSize);
   void ReadErrors(int& nFrameErrors, int& nParErrors);

   bool bMeasureAvail() const;
   void StartMeasure(const bool b422);
   void MeasureBaud(int& nBaud, int& nCount);

	void StartTransCapt(const bool b422, const bool bInvert=false);
	void StopTransCapt();


};




#endif
