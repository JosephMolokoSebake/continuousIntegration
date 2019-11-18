//---------------------------------------------------------------------------

#ifndef SatNMEADecodeH
#define SatNMEADecodeH

//---------------------------------------------------------------------------

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General decoder for NMEA format data
 *    FILE NAME		SatNMEADecode
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP SP2
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 12/09/2004
*************************************************************************/

class SatNMEADecode
{

   enum
   {
      MAX_SENTENCES     = 1000,
      COMM_BUFFER_SIZE  = 8192,
   };

   char szRXBuffer[COMM_BUFFER_SIZE];

   int nRXIndex;
   int nRXNum,nReadNum;
	AnsiString asRX[MAX_SENTENCES];

   bool bCRLF;

	int nStartIndex;
	char cStart;

	AnsiString sStartSeq;

   enum
   {
      OTHER_CHAR,
		STAR_CHAR,
		CHECK1_CHAR,
	} PrevChar;

   char cPrev;

	bool bStringEnd(const char c)
	{
		char cP=cPrev;
		cPrev=c;
		return (c==0x0A)&&(cP==0x0D);
	}


   bool bStartString(const char c)
	{
		switch(nStartIndex)
			{
			case 0:
				if (c=='\\')
					nStartIndex=1;
				else if (c=='!')
					{
					cStart='!';
					return true;
					}
				break;
			case 1:
				if ((c=='c')||(c=='g'))
					{
					cStart=c;
					nStartIndex=2;
					}
				else
					nStartIndex=0;
				break;
			case 2:
				nStartIndex=0;
				if (c==':')
					return true;
				else
					break;
			default:	;
			}
		return false;
	}

	String sLastString;


public:

	SatNMEADecode();

	void Reset(const bool _bCRLF=true)
	{
		bCRLF=_bCRLF;
		nRXIndex=-1;
		nRXNum=0;
		nReadNum=0;
		PrevChar=OTHER_CHAR;
		nStartIndex=0;
		cPrev=0;
	}

	void AddByte(const BYTE ucData);

	String sGetSentence();

	String sGetRawSentence()
	{
		return sLastString;
	}


};






#endif
