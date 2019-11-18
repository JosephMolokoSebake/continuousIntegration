//---------------------------------------------------------------------------
#include <jpch.h>

#include "JAES.h"

#pragma hdrstop

#include "VistaEncrypt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


int VistaEncrypt::nEncrypt(BYTE* pucData, const int nPlainSize)
{
	BYTE* pucPlain;
	pucPlain=new BYTE[nPlainSize];
   String s;
	if (sPW.IsEmpty())
		s="JGTvIsTaNeT";
	else
		s=sPW;
	JAES AES(s,KeySize);
	memcpy(pucPlain,pucData,nPlainSize);
	int nTXSize=AES.nEncrypt(pucPlain,&pucData[8],nPlainSize);
	pucData[0]='V';
	pucData[1]='S';
	pucData[2]='T';
	pucData[3]='5';
	pucData[4]=(BYTE)(nTXSize&0x000000FF);
	pucData[5]=(BYTE)((nTXSize>>8)&0x000000FF);
	pucData[6]=(BYTE)((nTXSize>>16)&0x000000FF);
	pucData[7]=(BYTE)((nTXSize>>24)&0x000000FF);
	nTXSize+=8;
	delete[] pucPlain;
	return nTXSize;
}

int VistaEncrypt::nAddDecryptByte(const BYTE uc)
{
	switch(nIdx)
		{
		case 0:
			if (uc=='V')
				++nIdx;
			else
				nIdx=0;
			break;
		case 1:
			if (uc=='S')
				++nIdx;
			else
				nIdx=0;
			break;
		case 2:
			if (uc=='T')
				++nIdx;
			else
				nIdx=0;
			break;
		case 3:
			if (uc=='5')
				++nIdx;
			else
				nIdx=0;
			break;
		case 4:
			nRXSize=uc; //LSB of size
			++nIdx;
			break;
		case 5:
			nRXSize|=(uc<<8);
			++nIdx;
			break;
		case 6:
			nRXSize|=(uc<<16);
			++nIdx;
			break;
		case 7:
			nRXSize|=(uc<<24);
			++nIdx;
			break;
		default:
			if (nIdx<MAX_RX_BUF_SIZE+8)
				{
				pucRXBuf[nIdx-8]=uc;
				++nIdx;
				}
			if (nIdx-8==nRXSize)
				{
				Decrypt();
				nIdx=0;
            return nPlainSize;
				}
		}
	return 0;   //not finished
}

void VistaEncrypt::Decrypt()
{
	String s;
	if (sPW.IsEmpty())
		s="JGTvIsTaNeT";
	else
		s=sPW;
	JAES AES(s,KeySize);
	nPlainSize=AES.nDecrypt(pucRXBuf,nRXSize);
	if (nPlainSize<=0)
		++nDecErrCnt;
	else
		++nBlockCnt;
}

