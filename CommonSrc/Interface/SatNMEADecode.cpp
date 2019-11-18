//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "SatNMEADecode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


SatNMEADecode::SatNMEADecode()
{
//   pucTest=NULL;
	Reset(true);
}

void SatNMEADecode::AddByte(const BYTE ucData)
{

	if (bStringEnd(ucData))
		{
		if (nRXIndex>0)
			{
			szRXBuffer[--nRXIndex]=0;
			asRX[nRXNum++]=AnsiString(szRXBuffer).Trim();
			if (nRXNum>=MAX_SENTENCES)
				nRXNum=0;
			}
		nRXIndex=0;
		}
	else if (nRXIndex>=0)
		{
		szRXBuffer[nRXIndex]=ucData;
		if (nRXIndex<COMM_BUFFER_SIZE-3)
			++nRXIndex;
		}
/*
	if (bStartString(ucData))
		{
		nRXIndex=0;
		if (cStart=='!')
			{
			}
		else
			{
			szRXBuffer[nRXIndex++]='\\';
			szRXBuffer[nRXIndex++]=cStart;
			}
		}
	if (nRXIndex>=0)
		{
		szRXBuffer[nRXIndex]=(char)ucData;
		bool bFin;
		if (bCRLF)
			bFin=(nRXIndex>0)&&(ucData=='\n')&&(szRXBuffer[nRXIndex-1]=='\r');
		else
			bFin=(nRXIndex>0)&&(ucData=='\r');
		if (bFin)
			{
			//Sentence completed
			szRXBuffer[nRXIndex]=0;
			nRXIndex=0;
			asRX[nRXNum]=AnsiString(szRXBuffer).Trim();
			PrevChar=OTHER_CHAR;
			nRXNum++;
			if (nRXNum>=MAX_SENTENCES)
				nRXNum=0;
			}
		if (nRXIndex<COMM_BUFFER_SIZE-1)
			++nRXIndex;
		}
*/
}

String SatNMEADecode::sGetSentence()
{
	AnsiString s="";
	if (nRXNum!=nReadNum)
		{
		s=asRX[nReadNum];
		nReadNum++;
		if (nReadNum>=MAX_SENTENCES)
			nReadNum=0;
		sLastString=s;
		}
	return String(s);
}


