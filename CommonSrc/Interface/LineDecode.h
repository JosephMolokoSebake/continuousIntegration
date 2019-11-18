//---------------------------------------------------------------------------

#ifndef LineDecodeH
#define LineDecodeH

//---------------------------------------------------------------------------

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General decoder for NMEA format data
 *    FILE NAME		LineDecode
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP SP2
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 12/09/2004
*************************************************************************/

class LineDecode
{

	enum
	{
		COMM_BUFFER_SIZE  = 8192,
	};

	char szRXBuffer[COMM_BUFFER_SIZE+1];

	int nRXIndex;

	enum
	{
		OTHER_CHAR	= 0,
		CR_CHAR		= 0x0D,
		LF_CHAR		= 0x0A,
	} PrevChar;

	AnsiString asLine;

public:

	LineDecode();

	void Reset()
	{
		nRXIndex=0;
		PrevChar=OTHER_CHAR;
		asLine="";
	}

	bool bAddByte(const BYTE ucData);

	String sGetLine(const bool bNMEA_Only);


};






#endif
