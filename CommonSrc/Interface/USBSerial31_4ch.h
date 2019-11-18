//---------------------------------------------------------------------------

#ifndef USBSerial31_4chH
#define USBSerial31_4chH
//---------------------------------------------------------------------------

#include "USB_FT245.h"
#include "JList.h"
#include "JFile.h"

class USBSerial31_4ch : public USB_FT245
{

   enum
   {
      CLOCK_FREQ     = 20,    //PLL used to make 20MHz from 50 MHz on-board Clock
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

      USB_MULTI_REQ     = 0xB0,
      USB_ERRORS_REQ    = 0xC0,
      USB_MEASURE_REQ   = 0xD0,
      USB_FIFO_REQ      = 0xE0,
      USB_LB_REQ        = 0xF0,

   };

// USB_SETUP0_WR
   enum
   {
      USB_SU0_RESET         = 0x01,
      USB_SU0_CHAN_SHIFT    = 1,
      USB_SU0_CHAN_MASK     = 0x06,

      USB_SU0_CHAN_0        = 0x00,
      USB_SU0_CHAN_1        = 0x02,
      USB_SU0_CHAN_2        = 0x04,
      USB_SU0_CHAN_3        = 0x06,


   };

// USB_SETUP1_WR
   enum
   {
      USB_SU1_ENABLE        = 0x01,
      USB_SU1_SEL422        = 0x02,
      USB_SU1_MEASURE       = 0x04,
      USB_SU1_ALWAYS_EN_TX  = 0x08,
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


   BYTE aucSetup[2];

   int nCh;
   BYTE ucData;
   BYTE aucData[4][65536];
   int anRX[4];

   WORD wFWVer;

   int nRXMultiData(BYTE* pucData, const int nMaxSize);


public:

	USBSerial31_4ch(const int nSerialNum=-1);
   ~USBSerial31_4ch();

   void Reset();
   bool bLoopbackOK();

   double dGetFWVersion() const
   {
      return ((wFWVer>>8)&0x000F)+(wFWVer&0x00FF)*0.001;
   }

   int nGetFWChannels() const
   {
      return (wFWVer>>12)&0x000F;
   }

   void SetupUART(const int nChan, const int nBaud, const char cParity, const int nDataBits, const int nStopBits);
   void StartUART(const int nChan, const bool b422, const bool bAlwaysEn422TX=true);
   void StopUART(const int nChan);

   bool bServiceMulti();
   int nGetMultiData(const int nChan, BYTE* pucData, const int nMaxSize);  //Chan 0..2

   int nRXData(BYTE* pucData, const int nMaxSize);      //Always Chan 3
   int nTXData(const BYTE* pucData, const int nSize);   //Always Chan 3

   void ReadErrors(int& nFrameErrors, int& nParErrors);

   void StartMeasure();
   void MeasureBaud(int& nBaud, int& nCount);

};


#endif
