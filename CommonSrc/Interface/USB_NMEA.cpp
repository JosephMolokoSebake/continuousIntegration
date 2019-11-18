/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	VTSTrack TAPI NMEA Data Receiver
 *		PROJECT			CoastRad C1274
 *		ACTIVITY       2
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *
************************************************************************///---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "USB_NMEA.h"
#include "JCommaText.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

USB_NMEA::USB_NMEA(  const bool b232,
                     const int nBaud,
                     const char cParity,
							const int nDataBits,
							const int nStopBits,
							const int nDevID) : USBSerial(USB_DETECT_DEV,nDevID)
{
	if (bOK())
      {
      Reset();
      SetupUART(nBaud,cParity,nDataBits,nStopBits,!b232);
      NMEA.Reset();
      StartUART(!b232);
      }
}

int USB_NMEA::nUpdate(const bool bEcho)
{
   if (bOK())
      {
      int nSize=nRXUARTData(aucRXBuf,RX_BUF_SIZE);
      for (int i=0; i<nSize; i++)
         NMEA.AddByte(aucRXBuf[i]);
      if ((nSize>0)&&(bEcho))
         nTXUARTData(aucRXBuf,nSize);
      return nSize;
      }
   return 0;
}

String USB_NMEA::sGetSentence(const bool bUseChk)
{
	return NMEA.sGetSentence(bUseChk);
}

void USB_NMEA::TXSentence(const String sData, const String sPrefix)
{
   TXRawString(NMEA.sConstructSentence(sData,sPrefix));
}

void USB_NMEA::TXRawString(const String s)
{
   nTXUARTData(AnsiString(s).c_str(),AnsiString(s).Length());
}

