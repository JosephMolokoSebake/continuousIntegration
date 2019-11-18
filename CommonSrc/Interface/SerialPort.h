//---------------------------------------------------------------------------
#ifndef SerialPortH
#define SerialPortH
//---------------------------------------------------------------------------

#include "jpch.h"

#pragma hdrstop

#include "CommsPort.h"

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Communications Interface (using TAPI)
 *    MAIN PROGRAM   EOTDesig
 *    FILE NAME		SerialPort
 *		PROJECT			C0953
 *		ACTIVITY       5
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium III
 *		OS					MS Windows 2000 SP1
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 22/01/2001
*************************************************************************/

class SerialPort : public PortThread
{

   enum
   {
      COMM_BUFFER_SIZE  = 30000,
   };

   BYTE aucRX[COMM_BUFFER_SIZE];

   int nRXIndex;

   void __fastcall Execute();

   void __fastcall AddData(const BYTE ucData);

public:

   __fastcall SerialPort(  const int _nPort,
                           const int _nBaud,
                           const char _ucParity,
                           const int _nDataBits,
                           const int _nStopBits);

   void __fastcall Reset()
   {
      nRXIndex=-1;
   }

   void __fastcall SendData(const BYTE* pucTXData, const int nSize, const bool bWait=false);

   int __fastcall nRXData(BYTE* pucData);

};


#endif
