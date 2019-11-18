//---------------------------------------------------------------------------

#ifndef AISEncoder_210H
#define AISEncoder_210H

#include "JTransform.h"
#include "AISDataDefs_210.h"
#include "JList.h"

/*	CONFIGURATION HEADER ****************************************************************
 *	MODULE TITLE		AIS Encoder
 *	FILE NAME		AISEncoder.cpp
 *	COMPILER		Borland C++ Builder 6
 *	PROCESSOR		Pentium IV
 *	OS			MS Windows XP SP3
 *	PROGRAMMER		L.J. van Wyk
 *	CHANGE HISTORY
 *		V1.00		LJVW 19/05/2003
 *					Initial Version
 *		V2.00		LJVW 16/01/2009
 *					Module rewritten
********************************************************************************************/
//---------------------------------------------------------------------------


class AISEncoderClass
{
   TStringList* psBM;
   String sVDM;
   BYTE aucVDMData[256];
   int nBinaryVDMSize;
   int nMessageNr;
   int nMessageSequenceNr;
   String sSMS;
   int nCharsPerSentence;
   bool bEncodeToBinary();
   int nGetHeaderSize();
   int nGetFillBits(const int nVarSize);
   bool bEncode(const int nMessageNr);
   bool bEncodeMessage6or8();
   String sFormatBMString(const int nSentences,const int nSentence,const int nMMSI,const String sVDM);
   void GetStrings(TStringList* pslstBM,const String s,const int nToMMSI);

   String sEncodeBinaryToVDM();
   void EncodeBinaryToChar(BYTE& ucCode,const BYTE ucBin) const;

public:
   AISEncoderClass();
   ~AISEncoderClass();
   String sComposeAIR(const int nMMSI,const int nMessageID=5);
	TStringList* pslstComposeBM(const int nSourceMMSI,const int nDestMMSI,const String sText,const int nMessageSeqNr,const bool bBroadcast=false);
};



















#endif
