/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Data Encryption Module
 *    FILE NAME		Encrypt.cpp
 * 	COMPILER       Borland C++ Builder 5 (SP1)
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000 Professional (SP2)
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *       Implements a randomized Vigenere type encryption.
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 06/03/2001
 ************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JEncrypt.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


Encryption::Encryption(const String sCodeWord_u, const WORD _uSeed, const int _nSize)
{
	String sCW1=sCodeWord_u;
	nSize=_nSize;
	uSeed=_uSeed;
	if (nSize>256) nSize=256;
	for (int i=0; i<256; i++)
		{
		aRow[i].pucEncr=new BYTE[nSize];
		aRow[i].pucDecr=new BYTE[nSize];
		}

	int nLength=sCodeWord_u.Length();
	for (int i=1; i<=nLength; i++) uSeed^=sCW1[i];
	nNext=uSeed;
	int n=1;
	for (int i=0; i<256; i++)
		{
		aucCode[i]=(BYTE)(sCW1[n]^ucRandom());
		if (n<nLength) n++; else n=1;
		}

   for (int i=0; i<256; i++)
      {
      for (int j=0; j<nSize; j++)
         aRow[i].pucEncr[j]=(BYTE)j;
      for (int n=0; n<32; n++)
         Shuffle(aRow[i].pucEncr,ucRandom());
      }
   for (int i=0; i<256; i++)
      for (int j=0; j<nSize; j++)
         aRow[i].pucDecr[aRow[i].pucEncr[j]]=j;
}

Encryption::~Encryption()
{
   for (int i=0; i<256; i++)
      {
      delete[] aRow[i].pucDecr;
      delete[] aRow[i].pucEncr;
      }
}

BYTE Encryption::ucRandom()
{
   nNext=nNext*1103515245+12345;
   int nR=(nNext/65526)%0x32767;
   return (BYTE)(nR%256);
}

void Encryption::Shuffle(BYTE* pucArray, const int nCut)
{
   BYTE aucNew[256];
   //Cut
   if ((nCut>0)&&(nCut<nSize-1))
      {
      for (int i=0; i<nSize-nCut; i++)
         aucNew[i]=pucArray[nCut+i];
      for (int i=nSize-nCut; i<nSize; i++)
         aucNew[i]=pucArray[i-(nSize-nCut)];
      }
   else
      for (int i=0; i<nSize; i++)
         aucNew[i]=pucArray[i];
   //Shuffle
   int n=0;
   for (int i=0; i<nSize/2; i++)
      {
      pucArray[n]=aucNew[i];
      n++;
      pucArray[n]=aucNew[i+nSize/2];
      n++;
      }
}

void Encryption::Encrypt(const void* pData, void* pEncrypted, const int nSize)
{
   int nCode=0;
   nNext=uSeed;
   BYTE* pucData=(BYTE*)pData;
   BYTE* pucEncrypted=(BYTE*)pEncrypted;
   for (int i=0; i<nSize; i++)
      {
      pucEncrypted[i]=aRow[aucCode[nCode]].pucEncr[pucData[i]]^ucRandom();
      if (nCode<255) nCode++; else nCode=0;
      }
}

void Encryption::Decrypt(const void* pEncrypted, void* pDecrypted, const int nSize)
{
   int nCode=0;
   nNext=uSeed;
   BYTE* pucDecrypted=(BYTE*)pDecrypted;
   BYTE* pucEncrypted=(BYTE*)pEncrypted;
   for (int i=0; i<nSize; i++)
      {
      pucDecrypted[i]=aRow[aucCode[nCode]].pucDecr[pucEncrypted[i]^ucRandom()];
      if (nCode<255) nCode++; else nCode=0;
      }
}

String Encryption::sEncrypt(const String s_u)
{
	if (nSize>256-32)
		{
		ShowError(L"Encryption size too large for strings",L"Encryption Error");
		return "";
		}
	else
		{
		String s=s_u;
		char* pszSt=new char[s.Length()+1];
		char* pszEnc=new char[s.Length()+1];
		for (int i=0; i<s.Length(); i++) pszSt[i]=s[i+1]-32;
      Encrypt(pszSt,pszEnc,s.Length());
      for (int i=0; i<s.Length(); i++) pszEnc[i]+=32;
      pszEnc[s.Length()]=0;
      String sEn=pszEnc;
      delete[] pszEnc;
      delete[] pszSt;
      return sEn;
      }
}

String Encryption::sDecrypt(const String sEn_u)
{
	if (nSize>256-32)
		{
		ShowError(L"Encryption size too large for strings",L"Encryption Error");
		return "";
		}
	else
		{
		String sEn=sEn_u;
		char* pszEnc=new char[sEn.Length()+1];
		char* pszDec=new char[sEn.Length()+1];
		for (int i=0; i<sEn.Length(); i++) pszEnc[i]=sEn[i+1]-32;
		Decrypt(pszEnc,pszDec,sEn.Length());
		for (int i=0; i<sEn.Length(); i++) pszDec[i]+=32;
		pszDec[sEn.Length()]=0;
		String sDec=pszDec;
		delete[] pszDec;
		delete[] pszEnc;
		return sDec;
		}
}

