//---------------------------------------------------------------------------
#ifndef USB_NMEAH
#define USB_NMEAH
//---------------------------------------------------------------------------

#include "USBSerial.h"
#include "NMEADecode.h"

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Communications Interface 
 *    MAIN PROGRAM   EOTDesig
 *    FILE NAME		USB_NMEA
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

class USB_NMEA : public USBSerial
{

   NMEADecode NMEA;

   enum
   {
      RX_BUF_SIZE = 1024*1024,
   };

   BYTE aucRXBuf[RX_BUF_SIZE];

public:

   USB_NMEA(const bool b232,
            const int nBaud,
            const char cParity,
            const int nDataBits,
				const int nStopBits,
				const int nDevID=-1);

   int nUpdate(const bool bEcho=false);

   String sGetSentence(const bool bUseChk=true);

   int nGetChecksumErrors() const
   {
      return NMEA.nGetChecksumErrors();
   }

   void TXSentence(const String sData, const String sPrefix=L"$");
   void TXRawString(const String s);

/*
	BYTE* pucGetRXBuf()
	{
		return aucRXBuf;
	}
*/

	void GetRXBuf(BYTE* pucData, const int nSize)
	{
		if (nSize>0)
			memcpy(pucData,aucRXBuf,nSize);
	}

};


#endif
