//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "LineDecode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


LineDecode::LineDecode()
{
	Reset();
}

bool LineDecode::bAddByte(const BYTE ucData)
{
	bool bFin=false;
	switch(ucData)
		{
		case CR_CHAR:
			break;
		case LF_CHAR:
			if (PrevChar==CR_CHAR)
				bFin=true;
			break;
		default:
			szRXBuffer[nRXIndex]=(char)ucData;
			if (nRXIndex<COMM_BUFFER_SIZE)
				++nRXIndex;
			else
				Reset(); 	//Buffer overflow Error - reset all
		}
	PrevChar=ucData;
	if (bFin)
		{
		szRXBuffer[nRXIndex]=0;
		nRXIndex=0;
		asLine=AnsiString(szRXBuffer).Trim();
		PrevChar=OTHER_CHAR;
		}
	return bFin;
}

String LineDecode::sGetLine(const bool bNMEA_Only)
{
	String s=asLine;
	if (bNMEA_Only)
		{
		int n=s.Pos("!AI");
		if (n>0)
			s=s.SubString(n,s.Length()-n+1);
		else
			s="";
		}
	return s;
}



