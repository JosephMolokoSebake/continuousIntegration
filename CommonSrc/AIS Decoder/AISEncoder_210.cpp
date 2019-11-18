/* 	----------------- CONFIGURATION HEADER START --------------------------
 *
 *		MODULE TITLE		AIS Encoder
 *    -----------------------------------------------------------------------
 *		FILE NAME			AISEncoder_210.cpp
 *		COMPILER				Borland C++ Builder 15 (XE)
 *		PROCESSOR			Pentium IV
 *		OS						MS Windows 7
 *		PROGRAMMER			Jacques van Wyk
 *    -----------------------------------------------------------------------
 *		ABSTRACT:			AIS Encoder
 *
 *		   V2_10          LJVW 2011/12/07
 *                      - Updated to reflect changes in AISDataDefs_210
 *
 /* 	----------------- CONFIGURATION HEADER END -------------------------- */

#include "jpch.h"
#pragma hdrstop

#include "AISEncoder_210.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

/******************************** AIS Encoder Class ********************************/

AISEncoderClass::AISEncoderClass()
{
   sVDM="";
   memset(&aucVDMData[0],0,sizeof(aucVDMData));
   nBinaryVDMSize=0;
   nMessageNr=0;
   nMessageSequenceNr=1;
   sSMS="";
   psBM=new TStringList;
   psBM->Capacity=10;
   nCharsPerSentence=40;
}

AISEncoderClass::~AISEncoderClass()
{
   delete psBM;
   psBM=NULL;
}


String AISEncoderClass::sComposeAIR(const int nMMSI,const int nMessageID)
{
   String s;
	s.printf(L"AIAIR,%09d,%d,,,,,,",nMMSI,nMessageID);
   return s;
}

TStringList* AISEncoderClass::pslstComposeBM(const int nSourceMMSI,const int nDestMMSI,const String sText,const int nMessageSeqNr,const bool bBroadcast)
{
   psBM->Clear();
   sSMS=sText.UpperCase();
   if (sSMS.Length()>151)
      sSMS.SetLength(151);
   nMessageSequenceNr=nMessageSeqNr;
   if (!bBroadcast)
      nMessageNr=6;
   else
      nMessageNr=8;
   if (nSourceMMSI>0)
   {
      if (bEncodeToBinary());
      {
         String s=sEncodeBinaryToVDM();
         if (!s.IsEmpty())
            GetStrings(psBM,s,nDestMMSI);
      }
   }
   return psBM;
}

void AISEncoderClass::GetStrings(TStringList* pslstBM,const String s,const int nToMMSI)
{
   pslstBM->Clear();
   int nTotalSentences=s.Length()/nCharsPerSentence;
   if (s.Length()%nCharsPerSentence!=0)
      nTotalSentences++;
   for (int i=0;i<nTotalSentences;i++)
   {
      String sSentence = s.SubString((i*nCharsPerSentence)+1,nCharsPerSentence);
      psBM->Add(sFormatBMString(nTotalSentences,(i+1),nToMMSI,sSentence));
   }
}

// !AIABM,1,1,0,020002063,0,6,04004PDhhv1Lu8h@,2
String AISEncoderClass::sFormatBMString(const int nSentences,const int nSentence,const int nMMSI,const String sSentence)
{
   String s="";
	int n=0;
   if (nSentences==nSentence)
      n = nGetFillBits(sSMS.Length());
   if (nMessageNr==6)
		s.printf(L"AIABM,%d,%d,0,%09d,0,%d,%s,%d",nSentences,nSentence,nMMSI,nMessageNr,sSentence,n);
   else if (nMessageNr==8)
      s.printf(L"AIBBM,%d,%d,0,0,%d,%s,%d",nSentences,nSentence,nMessageNr,sSentence,n);
   return s;
}


String AISEncoderClass::sEncodeBinaryToVDM()
{
	String s="";
   int nStringLength=sSMS.Length()+nGetHeaderSize();
   s=s.StringOfChar(' ',nStringLength);
   for (int i=0;i<nStringLength;i++)
   {
      BYTE ucChar;
      DWORD dw=dwGetVDMDataBits(aucVDMData,i*6,6);
      EncodeBinaryToChar(ucChar,dw);
      s[i+1] = ucChar;
   }
   return s;
}

int AISEncoderClass::nGetFillBits(const int nVarSize)
{
   int nHeaderBits;
   switch (nMessageNr)
   {
      case 6:	nHeaderBits=28;		break;  // 1 + 12 + 16
      case 8:	nHeaderBits=28;		break;
      default: nHeaderBits=0;
   }
   int nTotalBits=nHeaderBits+(nVarSize*6);
   int nFillBits=6-(nTotalBits%6);
   if ((nFillBits<0)||(nFillBits>5))
      nFillBits=0;
   return (nFillBits);
}



bool AISEncoderClass::bEncodeToBinary()
{
   memset(&aucVDMData[0],0,sizeof(aucVDMData));
   int nSize=sSMS.Length()+nGetHeaderSize();
   nBinaryVDMSize=(nSize*6)/8;
   if (((nSize*6)%8)!=0)
      nBinaryVDMSize++;
   return (bEncode(nMessageNr));
}

bool AISEncoderClass::bEncode(const int nMessageNr)
{
   bool bOK;
   switch (nMessageNr)
   {
      case 6:	bOK=bEncodeMessage6or8();	break;
      case 8:	bOK=bEncodeMessage6or8();	break;
      default: bOK = false;
   }
   return bOK;
}

bool AISEncoderClass::bEncodeMessage6or8()
{
   bool bOK=true;
   int nBit=0;
   SetVDMDataBits(aucVDMData,nBit,16,0x0040);
   SetVDMDataBits(aucVDMData,nBit,1,0);             // AckReq = false
   SetVDMDataBits(aucVDMData,nBit,11,nMessageSequenceNr);
   for (int i=0;i<sSMS.Length()&&bOK;i++)          // Text Message
   {
      int n=ucGetByteFromASCII(sSMS[i+1]);
      bOK=((n>=0)&&(n<64));
      if (bOK)
         SetVDMDataBits(aucVDMData,nBit,6,n);
   }
   return bOK;
}


int AISEncoderClass::nGetHeaderSize()
{
   int nSize;
   switch (nMessageNr)
   {
      case 6:	nSize=5;	break;   // 16 bits for IAI + 12 bits for International Funtion Message 0 Header
      case 8:	nSize=5;	break;
      default: nSize=0;
   }
   return nSize;
}


void AISEncoderClass::EncodeBinaryToChar(BYTE& ucCode,const BYTE ucBin) const
{
   if (ucBin<0x28)  // 101000
      ucCode=ucBin+0x30;
   else
      ucCode=ucBin+0x38;
}
