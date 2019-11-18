//---------------------------------------------------------------------------

#ifndef NMEADecodeH
#define NMEADecodeH

#include "JCommaText.h"

//---------------------------------------------------------------------------

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General decoder for NMEA format data
 *    FILE NAME		NMEADecode
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP SP2
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 12/09/2004
*************************************************************************/

class NMEADecode : public CommaText
{

   enum
   {
      MAX_SENTENCES     = 1000,
      COMM_BUFFER_SIZE  = 8192,
   };

   char szRXBuffer[COMM_BUFFER_SIZE];

   int nRXIndex;
   int nRXNum,nReadNum;
	int nCheckErrors;
	AnsiString asRX[MAX_SENTENCES];

   bool bCRLF;
	bool bForceFin;

	String sLastString;

   enum
   {
      OTHER_CHAR,
		STAR_CHAR,
		CHECK1_CHAR,
	} PrevChar;

   bool bStartChar(const char c) const
   {
      return (c=='$')||(c=='!');
   }

   char H2C(const BYTE uc)
   {
      if (uc<10)
         return uc+'0';
      else
         return (uc-10)+'A';
   }

   bool bCheckSumChar(const char c) const
   {
      return ((c>='A')&&(c<='F'))||((c>='0')&&(c<='9'));
   }

public:

   NMEADecode();

   void Reset(const bool _bCRLF=true, const bool _bForceFin=false)
   {
      bCRLF=_bCRLF;
      bForceFin=_bForceFin;
      nRXIndex=-1;
      nRXNum=0;
      nReadNum=0;
      nCheckErrors=0;
      PrevChar=OTHER_CHAR;
   }

   void AddByte(const BYTE ucData);

   String sGetSentence(const bool bUseChk=true);

	String sGetRawSentence();

   int nGetChecksumErrors() const
   {
      return nCheckErrors;
   }

   String sGetHeader(const bool bUseChk=true);

   int nGetParCnt();

	String sConstructSentence(const String sData, const String sPrefix="$");

};






#endif
