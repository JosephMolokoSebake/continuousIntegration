//---------------------------------------------------------------------------

#ifndef USBSerial310000H
#define USBSerial310000H
//---------------------------------------------------------------------------

#include "USB_FT245.h"
#include "JList.h"
#include "JFile.h"

class USBSerial3100 : public USB_FT245
{

   enum
   {
      CLOCK_FREQ     = 20,    //PLL used to make 20MHz from 50 MHz on-board Clock
   };

	enum
   {
		FIRMWARE_VERSION_3100  	= 3100,
		FIRMWARE_VERSION_4000  	= 4000,
	};

   enum
   {
      USB_SETUP0_WR     = 0x00,
      USB_SETUP1_WR     = 0x10,
      USB_BAUD0_WR      = 0x20,
      USB_BAUD1_WR      = 0x30,
      USB_BAUD2_WR      = 0x40,
      USB_BAUD3_WR      = 0x50,
      USB_DATABITS_WR   = 0x60,
      USB_PARSTOP_WR    = 0x70,
      USB_TX0_WR        = 0x80,
      USB_TX1_WR        = 0x90,

      USB_STATUS_REQ    = 0xB0,
      USB_ERRORS_REQ    = 0xC0,
      USB_MEASURE_REQ   = 0xD0,
      USB_FIFO_REQ      = 0xE0,
      USB_LB_REQ        = 0xF0,

   };

// USB_SETUP0_WR
   enum
   {
      USB_RESET      = 0x01,
      USB_ENABLE     = 0x02,
      USB_SEL422     = 0x04,
      USB_MEASURE    = 0x08,
   };

// USB_SETUP1_WR
   enum
   {
		USB_ALWAYS_EN_TX 		= 0x01,
		USB_INVERT_422_IN		= 0x02,
		USB_INVERT_422_OUT	= 0x04,
		USB_SEL_TRANS			= 0x08,
	};

   enum
   {
      USB_STAT_TXFIN = 0x01,
   };

   enum
   {
      UART_M_WIDTH_SHIFT   = 0,
      UART_M_WIDTH_MASK    = 0x000FFFFF,
      UART_M_COUNT_SHIFT   = 20,
      UART_M_COUNT_MASK    = 0x00000FFF,
   };


   WORD wFWVer;

   BYTE aucSetup[2];


public:

   USBSerial3100(const int nSerialNum);
   ~USBSerial3100();

   void Reset();

   bool bUSBLoopbackOK();

   double dGetFWVersion() const
   {
      return ((wFWVer>>8)&0x000F)+(wFWVer&0x00FF)*0.001;
   }

   int nGetFWVersion() const
   {
      return ((wFWVer>>8)&0x000F)*1000+(wFWVer&0x00FF);
   }

   bool bFWVersionOK(const int nVer) const
   {
      return (nVer==nGetFWVersion());
   }


   int nGetFWChannels() const
   {
      return (wFWVer>>12)&0x000F;
   }

   bool bVersionOK();

   void SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits);
   void StartUART(const bool b422, const bool bAlwaysEn422TX=true);
   void StopUART();
   int nRXUARTData(BYTE* pucData, const int nMaxSize);
   int nTXUARTData(const BYTE* pucData, const int nSize);
   void ReadErrors(int& nFrameErrors, int& nParErrors);

   void StartMeasure(const bool b422);
   void MeasureBaud(int& nBaud, int& nCount);

	void StartTransCapt(const bool b422, const bool bInvert=false);
	void StopTransCapt();


};


#endif
