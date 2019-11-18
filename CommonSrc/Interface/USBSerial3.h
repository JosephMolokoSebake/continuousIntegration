//---------------------------------------------------------------------------

#ifndef USBSerial3H
#define USBSerial3H
//---------------------------------------------------------------------------

#include "USB_FT245.h"
#include "JList.h"
#include "JFile.h"

class USBSerial3 : public USB_FT245
{

   enum
   {
      CLOCK_FREQ     = 50,    //50 MHz on-board Clock
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

   enum
   {
      UART_M_WIDTH_SHIFT   = 0,
      UART_M_WIDTH_MASK    = 0x000FFFFF,
      UART_M_COUNT_SHIFT   = 20,
      UART_M_COUNT_MASK    = 0x00000FFF,
   };


   BYTE aucSetup[2];


public:

   USBSerial3(const int nSerialNum);
   ~USBSerial3();

   void Reset();
   bool bUSBLoopbackOK();

   void SetupUART(const int nBaud, const char cParity, const int nDataBits, const int nStopBits);
   void StartUART(const bool b422);
   void StopUART();
   int nRXUARTData(BYTE* pucData, const int nMaxSize);
   int nTXUARTData(const BYTE* pucData, const int nSize);
   void ReadErrors(int& nFrameErrors, int& nParErrors);

   void StartMeasure();
   void MeasureBaud(int& nBaud, int& nCount);

};


#endif
