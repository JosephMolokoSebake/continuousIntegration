//---------------------------------------------------------------------------
#ifndef NMEA_RXH
#define NMEA_RXH
//---------------------------------------------------------------------------

#include "jpch.h"

#pragma hdrstop

#include "CommsPort.h"

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Communications Interface (using TAPI)
 *    MAIN PROGRAM   EOTDesig
 *    FILE NAME		NMEA_RX
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

class NMEA_RX : public PortThread
{

   enum
   {
      MAX_SENTENCES     = 1000,
		COMM_BUFFER_SIZE  = 4096,
		MAX_RAW_BUFFER		= 1024*1024,
	};

	BYTE aucRawBuffer[MAX_RAW_BUFFER];
	int nRawBufferSize;

	char szRXBuffer[COMM_BUFFER_SIZE];

   int nRXIndex;
   int nRXNum,nReadNum;
   int nCheckErrors;
   String asRX[MAX_SENTENCES];

   void __fastcall Execute();

   bool __fastcall bStartChar(const char c) const
   {
      return (c=='$')||(c=='!');
   }

   void __fastcall AddData(const BYTE ucData);
   char __fastcall H2C(const BYTE uc);

public:

   __fastcall NMEA_RX( const int _nPort,
                       const int _nBaud,
                       const char _cParity,
                       const int _nDataBits,
                       const int _nStopBits);

   void __fastcall Reset()
   {
      nRXIndex=-1;
      nRXNum=0;
      nReadNum=0;
      nCheckErrors=0;
   }


   String __fastcall sGetSentence(const bool bUseChk=true);

   bool bDataReady();

   int __fastcall nGetChecksumErrors() const
   {
      return nCheckErrors;
   }

   void __fastcall SendData(const BYTE* pucTXData, const int nSize, const bool bWait=true);

   void __fastcall TXSentence(const String sData, const String sPrefix=L"$");
   void __fastcall TXRawString(const String s);

	int nGetRawBuffer(BYTE* pucBuf)
	{
		SetCritical();
		int nSize=nRawBufferSize;
		if (nSize>0)
			{
			memcpy(pucBuf,aucRawBuffer,nSize);
			nRawBufferSize=0;
			}
		ReleaseCritical();
		return nSize;
	}


};


#endif
