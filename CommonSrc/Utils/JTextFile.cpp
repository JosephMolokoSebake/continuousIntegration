//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "JTextFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


JTextFile::JTextFile(const String sFile, const bool bCreate)
{
   pcBuf=NULL;
	sName=sFile;
	nLines=0;
	pFil=NULL;
   if (bCreate)
      {
      dwSize=0;
		}
   else
      {
      nFullCount=0;
		nLastSize=0;
      dwSize=0;
      nBlock=0;
      nBlockOffs=0;
      nBlockSize=0;
      pFil=new JFile('I');
      E=pFil->Open(sName);
      if (!E)
         E=pFil->GetSize(dwSize);
      if (!E)
         {
         nFullCount=dwSize/MAX_BLOCK_SIZE;
         nLastSize=dwSize%MAX_BLOCK_SIZE;
         }
		}
}

JTextFile::~JTextFile()
{
   if (BufList.nGetCount()>0)
      Flush();
	delete[] pcBuf;
	delete pFil;
}

JFile::FILE_ERROR JTextFile::NextLine(String& s)
{
   char szSt[4096];
   int i=0;
   bool bFin=false;
   do
      {
      if (nBlockOffs<nBlockSize)
         {
         if ((pcBuf[nBlockOffs]==13)||(pcBuf[nBlockOffs]==10))
            {
            bFin=true;
            ++nBlockOffs;
            }
         else
            {
            if (i<4095)
               {
               if (pcBuf[nBlockOffs]>=' ')
                  szSt[i++]=pcBuf[nBlockOffs++];
               else
                  ++nBlockOffs;
               }
            else
               bFin=true;
            }
         }
      else if (nBlock<nFullCount)
         {
         //read full block
         if (!pcBuf)
            pcBuf=new char[MAX_BLOCK_SIZE];
         E=pFil->Read(pcBuf,MAX_BLOCK_SIZE);
         if (E)
            bFin=true;
         else
            nBlockSize=MAX_BLOCK_SIZE;
         ++nBlock;
         nBlockOffs=0;
         }
      else if (nBlock==nFullCount)
         {
         //read last block
         if (!pcBuf)
            pcBuf=new char[nLastSize];
         E=pFil->Read(pcBuf,nLastSize);
         if (E)
            bFin=true;
         else
            nBlockSize=nLastSize;
         ++nBlock;
         nBlockOffs=0;
         }
      else
         {
         //finished
         bFin=true;
         s=L"";
         return JFile::F_EOF;
         }
      }
   while(!bFin);
   szSt[i]=0;
   s=String(szSt);
   return E;
}

int JTextFile::nNextCDLine()
{
   int i=0;
   int nCDCount=0;

   bool bFin=false;
   do
      {
      if (nBlockOffs<nBlockSize)
         {
         if ((pcBuf[nBlockOffs]==13)||(pcBuf[nBlockOffs]==10))
            {
            bFin=true;
            if (i>0)
               {
               acCD[nCDCount][i]=0;
               ++nCDCount;
               }
            ++nBlockOffs;
            }
         else
            {
            if (i<MAX_CD_STRING_LENGTH)
               {
               char c=pcBuf[nBlockOffs];
               if (c>=' ')
                  {
                  if (c==',')
                     {
                     acCD[nCDCount][i]=0;
                     i=0;
                     if (nCDCount<MAX_CD_COUNT)
                        ++nCDCount;
                     }
                  else
                     {
                     if (i<MAX_CD_STRING_LENGTH)
                        acCD[nCDCount][i++]=c;
                     }
                  }
               ++nBlockOffs;
               }
            else
               bFin=true;
            }
         }
      else if (nBlock<nFullCount)
         {
         //read full block
         if (!pcBuf)
            pcBuf=new char[MAX_BLOCK_SIZE];
         E=pFil->Read(pcBuf,MAX_BLOCK_SIZE);
         if (E)
            bFin=true;
         else
            nBlockSize=MAX_BLOCK_SIZE;
         ++nBlock;
         nBlockOffs=0;
         }
      else if (nBlock==nFullCount)
         {
         //read last block
         if (!pcBuf)
            pcBuf=new char[nLastSize];
         E=pFil->Read(pcBuf,nLastSize);
         if (E)
            bFin=true;
         else
            nBlockSize=nLastSize;
         ++nBlock;
         nBlockOffs=0;
         }
      else
         {
         //finished
         bFin=true;
         }
      }
   while(!bFin);
   return nCDCount;
}

String JTextFile::sGetPar(const int n)
{
   return String(acCD[n]);
}


void JTextFile::GetNextLine(char* psz, const int nMaxLen)
{
   int i=0;
   char c;
   do
      {
      c=pcBuf[dwOffs++];
      if ((c!=13)&&(i<nMaxLen)&&(c!=10))
         psz[i++]=c;
      }
   while((c!=13)&&(dwOffs<dwSize));
   psz[i]=0;
}

int JTextFile::nAddLine(const String sLine)
{
	AnsiString sL=sLine;
	return nAddLine(sL.c_str(),sL.Length());
}

int JTextFile::nAddLine(const char* pc, const int nLen)
{
   dwSize+=(DWORD)nLen;
   ++nLines;
   LineStruct* pL=new LineStruct(pc,nLen);
   return BufList.nAdd(pL);
}

JFile::FILE_ERROR JTextFile::Flush()
{
   JFile Fil('O');
   E=Fil.Create(sName);
   if ((!E)&&(dwSize+nLines*2>0))
      {
		pcBuf=new char[dwSize+nLines*2];
      BufList.GoFirst();
      LineStruct* pL;
      dwOffs=0;
      while(!BufList.bLast())
         {
         pL=BufList.pNext();
         if (pL)
            {
            if (pL->nLen>0)
               {
               memcpy(&pcBuf[dwOffs],pL->pcBuf,pL->nLen);
               dwOffs+=pL->nLen;
               }
            pcBuf[dwOffs++]=13;
            pcBuf[dwOffs++]=10;
            }
         }
      E=Fil.Write(pcBuf,dwOffs);
      delete[] pcBuf;
      pcBuf=NULL;
      }
   return E;
}


