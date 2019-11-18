/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General DCS File Functions
 *		PROJECT        Various
 *		ACTIVITY
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000 SP3
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      1.00: Initial Version JGT 26-10-2002
************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JDCSFile.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JDCSFile::JDCSFile(const String _sName, const bool bCreate, const bool bWriteThrough)
{
   sName=_sName;
   DWORD dwDistr;
   if (bCreate)
      dwDistr=CREATE_ALWAYS;
   else
      dwDistr=OPEN_EXISTING;
   DWORD dwAttr=FILE_ATTRIBUTE_ARCHIVE;
   if (bWriteThrough)
      dwAttr|=FILE_FLAG_WRITE_THROUGH;
   hFile=::CreateFile(  sName.c_str(),
                        GENERIC_WRITE,
                        0,
                        NULL,
                        dwDistr,
                        dwAttr,
                        NULL);
   if (hFile==INVALID_HANDLE_VALUE)
      {
      dwError=::GetLastError();
      ShowError();
      }
}

JDCSFile::~JDCSFile()
{
   if (hFile!=INVALID_HANDLE_VALUE)
      bClose();
}

bool JDCSFile::bClose()
{
   if (!::CloseHandle(hFile))
      {
      dwError=::GetLastError();
      ShowError();
      hFile=INVALID_HANDLE_VALUE;
      return false;
      }
   hFile=INVALID_HANDLE_VALUE;
   return true;
}

bool JDCSFile::bSeek(const DWORD dwOffset)
{
	LARGE_INTEGER lnOffset,lnPos;
	lnOffset.LowPart=dwOffset;
	lnOffset.HighPart=0;
	if (!::SetFilePointerEx(hFile,lnOffset,&lnPos,FILE_BEGIN))
		{
		dwError=::GetLastError();
		ShowError();
		return false;
		}
	return true;
}

bool JDCSFile::bSeek(const LARGE_INTEGER lnOffset)
{
	LARGE_INTEGER lnPos;
	if (!::SetFilePointerEx(hFile,lnOffset,&lnPos,FILE_BEGIN))
		{
		dwError=::GetLastError();
		ShowError();
		return false;
		}
	return true;
}

bool JDCSFile::bWrite(const DWORD dwOffs, const void* pData, const DWORD dwSize)
{
	if (bSeek(dwOffs))
		return bWrite(pData,dwSize);
	return false;
}

bool JDCSFile::bWrite(const LARGE_INTEGER lnOffs, const void* pData, const DWORD dwSize)
{
	if (bSeek(lnOffs))
		return bWrite(pData,dwSize);
	return false;
}

bool JDCSFile::bAppend(const void* pData, const DWORD dwSize)
{
	LARGE_INTEGER lnDist,lnNewPnt;
	lnDist.QuadPart=0;
	if (!::SetFilePointerEx(hFile,lnDist,&lnNewPnt,FILE_END))
		{
		dwError=::GetLastError();
		ShowError();
		return false;
		}
	return bWrite(pData,dwSize);
}

bool JDCSFile::bWrite(const void* pData, const DWORD dwSize)
{
	DWORD dwWritten;
	if (!::WriteFile(hFile,pData,dwSize,&dwWritten,NULL))
		{
		dwError=::GetLastError();
		ShowError();
		return false;
		}
	if (dwWritten!=dwSize)
		{
      dwError=::GetLastError();
      ShowError();
      return false;
      }
   if (!::FlushFileBuffers(hFile))
      {
      dwError=::GetLastError();
      ShowError();
      return false;
      }
   return true;
}

void JDCSFile::ShowError()
   {
   LPVOID pMsgBuf;
   ::FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      dwError,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR)&pMsgBuf,
      0,
      NULL
      );
	String s=L"File Error in "+sName;
	::ShowError((char*)pMsgBuf,s);
   LocalFree(pMsgBuf );
}

String JDCSFile::sGetName()
{
   return sName;
}


JDoubleFile::JDoubleFile(const String _sName, const int _nStoreIntervalMs)
{
   pFil1=new JDCSFile(_sName,true);
   String sExt=sFileExtOnly(_sName,false);
	String sFile2=sFileNewExt(_sName,L".~"+sExt);
   pFil2=new JDCSFile(sFile2,true);
   pucHeader1=NULL;
   pucHeader2=NULL;
   dwHeaderSize=0;
   dwStoreInterval=_nStoreIntervalMs;
   dwNextStore1=::GetTickCount()+dwStoreInterval/2;
   dwNextStore2=dwNextStore1+dwStoreInterval/2;
   abFirst[0]=true;
   abFirst[1]=true;
}

JDoubleFile::~JDoubleFile()
{
   bFlushFile(pFil1,pucHeader1,&BufList1,abFirst[0]);
   delete pFil1;
   bFlushFile(pFil2,pucHeader2,&BufList2,abFirst[1]);
   delete pFil2;
   delete[] pucHeader1;
   delete[] pucHeader2;
}

void JDoubleFile::StoreHeader(const void* pData, const DWORD dwSize)
{
   if (dwHeaderSize==0)
      {
      dwHeaderSize=dwSize;
      pucHeader1=new BYTE[dwHeaderSize];
      pucHeader2=new BYTE[dwHeaderSize];
      }
   memcpy(pucHeader1,pData,dwHeaderSize);
   memcpy(pucHeader2,pData,dwHeaderSize);
}

bool JDoubleFile::bFlushFile(JDCSFile* pFil, const BYTE* pucHeader, JList<BufferBlock>* pBufList, bool& bFirst)
{
   bool bOK=true;
   if (pBufList->nGetCount()>0)
      {
      if (dwHeaderSize>0)
         bOK=pFil->bWrite(0,pucHeader,dwHeaderSize);
      if (bOK)
         {
         DWORD dwBufSize=0;
         pBufList->GoFirst();
         while(!pBufList->bLast())
            {
            BufferBlock* pBuf=pBufList->pNext();
            dwBufSize+=pBuf->dwSize;
            }
         if (dwBufSize>0)
            {
            BYTE* pucBuf=new BYTE[dwBufSize];
            DWORD dwOffs=0;
            pBufList->GoFirst();
            while(!pBufList->bLast())
               {
               BufferBlock* pBuf=pBufList->pNext();
               memcpy(&pucBuf[dwOffs],pBuf->pucData,pBuf->dwSize);
               dwOffs+=pBuf->dwSize;
               }
            bOK=pFil->bAppend(pucBuf,dwOffs);
            delete[] pucBuf;
            }
         }
      pBufList->Clear();
		}
   else if (bFirst)
      {
      if (dwHeaderSize>0)
         bOK=pFil->bWrite(0,pucHeader,dwHeaderSize);
      }
   bFirst=false;
   return bOK;
}


bool JDoubleFile::bStoreData(const void* pData, const DWORD dwSize)
{
   BufferBlock* pBuf1=new BufferBlock((BYTE*)pData,dwSize);
   BufList1.nAdd(pBuf1);
   BufferBlock* pBuf2=new BufferBlock((BYTE*)pData,dwSize);
   BufList2.nAdd(pBuf2);
   DWORD dwMS=::GetTickCount();
   bool bOK=true;
   if (dwMS>dwNextStore1)
      {
      bOK=bFlushFile(pFil1,pucHeader1,&BufList1,abFirst[0]);
      dwNextStore1+=dwStoreInterval;
      }
   if (dwMS>dwNextStore2)
      {
      bOK=bFlushFile(pFil2,pucHeader2,&BufList2,abFirst[1]);
      dwNextStore2+=dwStoreInterval;
      }
   return bOK;
}

