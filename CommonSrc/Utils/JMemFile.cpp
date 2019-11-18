//---------------------------------------------------------------------------

#include "jpch.h"
#pragma hdrstop

#include "JMemFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JFile::FILE_ERROR JMemFile::ReadBuffer(const String sFile, const DWORD dwExtra)
{
   sFileName=sFile;
   delete[] pucBuffer;
   pucBuffer=NULL;
   JFile Fil('I');
   JFile::FILE_ERROR Error=Fil.Open(sFileName);
   if (!Error) Error=Fil.GetSize(dwSize);
   if ((!Error)&&(dwSize>0))
      {
      pucBuffer=new BYTE[dwSize+dwExtra];
      Error=Fil.Read(pucBuffer,dwSize);
      }
   if (Error)
      {
      delete[] pucBuffer;
      pucBuffer=NULL;
      dwSize=0;
      }
   else if (dwExtra>0)
      {
      memset(&pucBuffer[dwSize],0,dwExtra);
      dwSize+=dwExtra;
      }
   dwPos=0;
   return Error;
}

JFile::FILE_ERROR JMemFile::StoreBuffer(const String sFile)
{
   JFile Fil('O');
   JFile::FILE_ERROR Error=Fil.Create(sFile);
   if (!Error) Error=Fil.Write(pucBuffer,dwSize);
   return Error;
}

void JMemFile::XORBuffer(const BYTE ucXOR)
{
	for (DWORD dw=0; dw<dwSize; dw++)
		pucBuffer[dw]^=ucXOR;
}

void JMemFile::FillBuffer(const BYTE* pucData, const DWORD dwDataSize)
{
   dwSize=dwDataSize;
   dwPos=0;
   delete[] pucBuffer;
   pucBuffer=new BYTE[dwSize];
   memcpy(pucBuffer,pucData,dwSize);
}

bool JMemFile::bRead(void* pData, const DWORD dwBytes)
{
   return bRead(dwPos,pData,dwBytes);
}

bool JMemFile::bRead(const DWORD dwStart, void* pData, const DWORD dwBytes)
{
   if (dwStart+dwBytes<=dwSize)
      {
      memcpy(pData,&pucBuffer[dwStart],dwBytes);
      dwPos=dwStart+dwBytes;
      return true;
      }
   else
      return false;
}

bool JMemFile::bWrite(const DWORD dwStart, void* pData, const DWORD dwBytes)
{
   if (dwStart+dwBytes<=dwSize)
      {
      memcpy(&pucBuffer[dwStart],pData,dwBytes);
      dwPos=dwStart+dwBytes;
      return true;
      }
   else
      return false;

}

bool JMemFile::bSeek(const DWORD dwNewPos)
{
   if (dwNewPos<=dwSize)
      {
      dwPos=dwNewPos;
      return true;
      }
   else
      return false;
}

bool JMemFile::bAdvance(const DWORD dwBytes)
{
   return bSeek(dwPos+dwBytes);
}

bool JMemFile::bRead(String& s, const int nMaxLen)
{
   return bRead(dwPos,s,nMaxLen);
}

bool JMemFile::bRead(const DWORD dwStart, String& s, const int nMaxLen)
{
   s=L"";
   int nLen;
   bool bOK=bRead(dwStart,&nLen,sizeof(nLen));
   if (bOK)
      {
      if (nLen>65536) nLen=65536; else if (nLen<0) nLen=0;
      if ((nMaxLen>0)&&(nLen>nMaxLen)) nLen=0;
      if (nLen>0)
         {
         char* pszSt=new char[nLen+1];
         bOK=bRead(pszSt,nLen);
         if (bOK)
            {
            pszSt[nLen]=0;
            s=String(pszSt);
            }
         delete[] pszSt;
         }
      }
   return bOK;
}

bool JMemFile::bRead(TFont* pFont)
{
#pragma option push -a4    //Set compiler to DWORD alignment
   struct
   {
      TColor Color;
      SHORT nSize;
      WORD uStyle;
   } FontData;
#pragma option pop         //Set compiler to default alignment
   bool bOK=bRead(&FontData,sizeof(FontData));
	String wsSt;
	if (bOK) bOK=bRead(wsSt);
	if (bOK)
      {
		pFont->Name=wsSt;
		pFont->Color=FontData.Color;
      pFont->Size=FontData.nSize;
      TFontStyles Style;
      if (FontData.uStyle&0x01)
         Style<<fsBold;
      if (FontData.uStyle&0x02)
         Style<<fsItalic;
      if (FontData.uStyle&0x04)
         Style<<fsUnderline;
      if (FontData.uStyle&0x08)
         Style<<fsStrikeOut;
      pFont->Style=Style;
      }
   return bOK;
}

bool JMemFile::bNextString(String& s)
{
	s=L"";
   if (dwPos>=dwSize)
      return false;
   else
      {
      while(dwPos<dwSize)
         {
         if ((pucBuffer[dwPos]==13)||(pucBuffer[dwPos]==10))
            {
            ++dwPos;
            if (dwPos<dwSize)
               {
               if (pucBuffer[dwPos]==10)
                  ++dwPos;
               }
            return true;
            }
         else
            {
            s=s+(char)pucBuffer[dwPos++];
            }
         }
		}
   return false;
}


bool JMemFile::bNextString(char* pszSt, const int nMaxLen)
{
	String s;
	if (bNextString(s))
		{
		AnsiString s1=s;
		::strncpy(pszSt,s1.c_str(),nMaxLen-1);
		pszSt[nMaxLen-1]=0;
		return true;
		}
	return false;
}

int JMemFile::nNextVal(double& dV)
{
	char szSt[1024];
	if (bNextString(szSt,1024))
		{
      if (szSt[0]==';')
         return 0;
      else
			return sscanf(szSt,"%lf",&dV);
      }
   return -1;
}

int JMemFile::nNextVal(double& dV1, double& dV2)
{
   char szSt[1024];
   if (bNextString(szSt,1024))
      {
      if (szSt[0]==';')
         return 0;
		else
		  return sscanf(szSt,"%lf%lf",&dV1,&dV2);
      }
   return -1;
}

int JMemFile::nNextVal(double& dV1, double& dV2, double& dV3)
{
   char szSt[1024];
   if (bNextString(szSt,1024))
      {
      if (szSt[0]==';')
         return 0;
      else
         return sscanf(szSt,"%lf%lf%lf",&dV1,&dV2,&dV3);
      }
   return -1;
}

int JMemFile::nNextVal(double& dV1, double& dV2, double& dV3, double& dV4)
{
   char szSt[1024];
   if (bNextString(szSt,1024))
      {
      if (szSt[0]==';')
         return 0;
      else
         return sscanf(szSt,"%lf%lf%lf%lf",&dV1,&dV2,&dV3,&dV4);
      }
   return -1;
}


