//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JDESEncrypt.h"
#include "JFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


const BYTE DES_ENCRYPTION_SIGNATURE[16] =
{
  85, 66, 67, 127, 128, 248, 92, 152, 15, 252, 175, 38, 158, 218, 22, 141
};

DESEncrypt::DESEncrypt(const String sPW, const bool bTripleDes)
{
   randomize();
   if (bTripleDes)
      {
      p3Des=new triple_des;
      pDes=NULL;
		p3Des->password(AnsiString(sPW).c_str());
      }
   else
      {
      p3Des=NULL;
      pDes=new des;
      pDes->password(AnsiString(sPW).c_str());
      }
}

void DESEncrypt::Encrypt(BYTE* pucData) //8 bytes at a time
{
   BYTE aucBuf[DES_DATA_SIZE];
   int n=0;
   for (int i=0; i<8; i++)
      {
      BYTE uc=pucData[i];
      aucBuf[n++]=(uc>>7)&0x01;
      aucBuf[n++]=(uc>>6)&0x01;
      aucBuf[n++]=(uc>>5)&0x01;
      aucBuf[n++]=(uc>>4)&0x01;
      aucBuf[n++]=(uc>>3)&0x01;
      aucBuf[n++]=(uc>>2)&0x01;
      aucBuf[n++]=(uc>>1)&0x01;
      aucBuf[n++]=uc&0x01;
      }
   if (pDes)
      pDes->encrypt(aucBuf);
   else if (p3Des)
      p3Des->encrypt(aucBuf);
   for (int i=0; i<8; i++)
      {
      n=i*8;
      pucData[i]= aucBuf[n]   << 7 |
                  aucBuf[n+1] << 6 |
                  aucBuf[n+2] << 5 |
                  aucBuf[n+3] << 4 |
                  aucBuf[n+4] << 3 |
                  aucBuf[n+5] << 2 |
                  aucBuf[n+6] << 1 |
                  aucBuf[n+7];
      }
}

void DESEncrypt::Decrypt(BYTE* pucData) //8 bytes at a time
{
   BYTE aucBuf[DES_DATA_SIZE];
   int n=0;
   for (int i=0; i<8; i++)
      {
      BYTE uc=pucData[i];
      aucBuf[n++]=(uc>>7)&0x01;
      aucBuf[n++]=(uc>>6)&0x01;
      aucBuf[n++]=(uc>>5)&0x01;
      aucBuf[n++]=(uc>>4)&0x01;
      aucBuf[n++]=(uc>>3)&0x01;
      aucBuf[n++]=(uc>>2)&0x01;
      aucBuf[n++]=(uc>>1)&0x01;
      aucBuf[n++]=uc&0x01;
      }
   if (pDes)
      pDes->decrypt(aucBuf);
   else if (p3Des)
      p3Des->decrypt(aucBuf);
   for (int i=0; i<8; i++)
      {
      n=i*8;
      pucData[i]= aucBuf[n]   << 7 |
                  aucBuf[n+1] << 6 |
                  aucBuf[n+2] << 5 |
                  aucBuf[n+3] << 4 |
                  aucBuf[n+4] << 3 |
                  aucBuf[n+5] << 2 |
                  aucBuf[n+6] << 1 |
                  aucBuf[n+7];
      }
}

String DESEncrypt::sEncode6Bit(const BYTE* pucData, const int nLen)
{
   char* pszSt=new char[nLen*2];
   int n6Byte=0,n6Bit=0;
   char c=0;
   for (int nByte=0; nByte<nLen; nByte++)
      {
      BYTE uc=pucData[nByte];
      for (int nBit=0; nBit<8; nBit++)
         {
         if (uc&(1<<nBit))
            c|=(1<<n6Bit);
         ++n6Bit;
         if (n6Bit==6)
            {
            pszSt[n6Byte]=cLookupByte(c);
            n6Bit=0;
            n6Byte++;
            c=0;
            }
         }
      }
   if (n6Bit!=0)
      {
      pszSt[n6Byte]=cLookupByte(c);
      ++n6Byte;
      }
   pszSt[n6Byte]=0;
   String s=pszSt;
   delete[] pszSt;
   return s;
}

int DESEncrypt::nDecode6Bit(const String sData, BYTE* pucDec)
{
   int n8Bit=0,n8Byte=0;
   int nLen=sData.Length();
   pucDec[n8Byte]=0;
   for (int n=1; n<=nLen; n++)   //NB include trailing zero
      {
      BYTE uc=ucLookupChar(sData[n]);
      for (int i=0; i<6; i++)
         {
         if (uc&(1<<i))
            pucDec[n8Byte]|=(1<<n8Bit);
         ++n8Bit;
         if (n8Bit==8)
            {
            ++n8Byte;
            n8Bit=0;
            pucDec[n8Byte]=0;
            }
         }
      }
   return n8Byte;
}

int DESEncrypt::nEncryptRaw(BYTE* pucData, const WORD uSize)
{
   DWORD dwEncSize=uSize+sizeof(WORD);  //include size
   DWORD dwTotal=dwEncSize%8;
   DWORD dwExtra;
   if (dwTotal>0)
      {
      dwExtra=8-dwTotal;
      dwTotal=dwEncSize+dwExtra;
      }
   else
      {
      dwExtra=0;
      dwTotal=dwEncSize;
      }
   BYTE* pucTemp=new BYTE[dwTotal];
   if (dwExtra>0)
      {
      DWORD dwCnt=0;
      DWORD dw=dwTotal;
      while((dw>0)&&(dwCnt<dwExtra))
         {
         pucTemp[--dw]=random(256);
         ++dwCnt;
         }
      }
   memcpy(pucTemp,&uSize,sizeof(uSize));
   memcpy(&pucTemp[sizeof(uSize)],pucData,uSize);
   DWORD dwBlocks=dwTotal/8;
   for (DWORD dw=0; dw<dwBlocks; dw++)
      Encrypt(&pucTemp[dw*8]);
   memcpy(pucData,pucTemp,dwTotal);
   delete[] pucTemp;
   return (int)dwTotal;
}

int DESEncrypt::nDecryptRaw(BYTE* pucData, const WORD uSize)
{
   BYTE* pucTemp=new BYTE[uSize];
   memcpy(pucTemp,pucData,uSize);
   WORD uBlocks=uSize/8;
   for (WORD u=0; u<uBlocks; u++)
      Decrypt(&pucTemp[u*8]);
   WORD uDecSize;
   memcpy(&uDecSize,pucTemp,sizeof(uDecSize));
   if (uDecSize<=(WORD)(uSize-sizeof(WORD)))
      memcpy(pucData,&pucTemp[sizeof(uDecSize)],uDecSize);
   else
      uDecSize=0;
   delete[] pucTemp;
   return (int)uDecSize;
}

int DESEncrypt::nEncrypt(BYTE* pucData, const DWORD dwSize, TProgressBar* pProg)   //returns encrypted size
{
   if (pProg)
      pProg->Position=0;
   DWORD dwEncSize=dwSize+sizeof(DWORD)+sizeof(DES_ENCRYPTION_SIGNATURE);  //include size & signature
   DWORD dwTotal=dwEncSize%8;
   if (dwTotal>0)
      dwTotal=dwEncSize+(8-dwTotal);
   else
      dwTotal=dwEncSize;
   BYTE* pucTemp=new BYTE[dwTotal];
   memcpy(pucTemp,DES_ENCRYPTION_SIGNATURE,sizeof(DES_ENCRYPTION_SIGNATURE));
   memcpy(&pucTemp[sizeof(DES_ENCRYPTION_SIGNATURE)],&dwSize,sizeof(dwSize));
   memcpy(&pucTemp[sizeof(DWORD)+sizeof(DES_ENCRYPTION_SIGNATURE)],pucData,dwSize);
   DWORD dwBlocks=dwTotal/8;
   int nPrev=0;
   for (DWORD dw=0; dw<dwBlocks; dw++)
      {
      Encrypt(&pucTemp[dw*8]);
      if (pProg)
         {
         int nPerc=(100*dw)/dwBlocks;
         if (nPerc!=nPrev)
            {
            nPrev=nPerc;
            pProg->Position=nPerc;
            }
         }
      }
   memcpy(pucData,pucTemp,dwTotal);
   delete[] pucTemp;
   if (pProg)
      pProg->Position=100;
   return (int)dwTotal;
}

int DESEncrypt::nDecrypt(BYTE* pucData, const DWORD dwSize, TProgressBar* pProg)   //returns decrypted size
{
   if (pProg)
      pProg->Position=0;
   BYTE* pucTemp=new BYTE[dwSize];
   memcpy(pucTemp,pucData,dwSize);
   DWORD dwBlocks=dwSize/8;
   int nPrev=0;
   for (DWORD dw=0; dw<dwBlocks; dw++)
      {
      Decrypt(&pucTemp[dw*8]);
      if (pProg)
         {
         int nPerc=(100*dw)/dwBlocks;
         if (nPerc!=nPrev)
            {
            nPrev=nPerc;
            pProg->Position=nPerc;
            }
         }
      }
   if (memcmp(pucTemp,DES_ENCRYPTION_SIGNATURE,sizeof(DES_ENCRYPTION_SIGNATURE))==0)
      {
      DWORD dwDecSize;
      memcpy(&dwDecSize,&pucTemp[sizeof(DES_ENCRYPTION_SIGNATURE)],sizeof(DWORD));
      if (dwDecSize<=dwSize-sizeof(DWORD))
         memcpy(pucData,&pucTemp[sizeof(DWORD)+sizeof(DES_ENCRYPTION_SIGNATURE)],dwDecSize);
      else
         dwDecSize=0;
      delete[] pucTemp;
      if (pProg)
         pProg->Position=100;
      return (int)dwDecSize;
      }
   else
      {
      delete[] pucTemp;
      return -1;
      }
}

String DESEncrypt::sEncryptString(const String sDec)
{
	int nLen=sDec.Length()+1;  //space for zero added
	nLen=((nLen+7)/8)*8;
	char* pcEnc=new char[nLen];
	memset(pcEnc,0,nLen);
	strncpy(pcEnc,AnsiString(sDec).c_str(),AnsiString(sDec).Length());
	pcEnc[sDec.Length()]=0;
	for (int i=0; i<nLen/8; i++)
		Encrypt(&pcEnc[i*8]);
	String s=sEncode6Bit(pcEnc,nLen);
	delete[] pcEnc;
	return s;
}

String DESEncrypt::sDecryptString(const String sEnc)
{
   int nLen=sEnc.Length()*2;
   char* pcData=new char[nLen];
   nLen=nDecode6Bit(sEnc,pcData);
   for (int i=0; i<nLen/8; i++)
      Decrypt(&pcData[i*8]);
   pcData[nLen-1]=0; //ensure valid AnsiString0
   String s=pcData;
   delete[] pcData;
   return s;
}

bool DESEncrypt::bEncryptFile(const String sDecFile, const String sEncFile)
{
   bool bOK=false;
   JFile DecFil('I');
   JFile::FILE_ERROR Error=DecFil.Open(sDecFile);
   DWORD dwSize=0;
   BYTE* pucBuf=NULL;
   if (!Error)
      {
      dwSize=DecFil.dwGetSize();
      if (dwSize==0)
         Error=JFile::F_SIZE_ERROR;
      }
   if (!Error)
      {
      pucBuf=new BYTE[dwSize+27];
      Error=DecFil.Read(pucBuf,dwSize);
      }
   if (!Error)
      Error=DecFil.Close();
   DecFil.ShowError(Error);
   if (!Error)
      {
      dwSize=nEncrypt(pucBuf,dwSize);
      JFile EncFil('O');
      Error=EncFil.Create(sEncFile);
      if (!Error)
         Error=EncFil.Write(pucBuf,dwSize);
      if (!Error)
         Error=EncFil.Close();
      EncFil.ShowError(Error);
      if (!Error)
         bOK=true;
      }
   delete[] pucBuf;
   return bOK;
}

bool DESEncrypt::bFileEncrypted(const String sFile)
{
   bool bOK=false;
   JFile EncFil('I');
   JFile::FILE_ERROR Error=EncFil.Open(sFile);
   if (!Error)
      {
      DWORD dwSize=EncFil.dwGetSize();
      if (dwSize<sizeof(DES_ENCRYPTION_SIGNATURE))
         Error=JFile::F_SIZE_ERROR;
      }
   BYTE aucBuf[sizeof(DES_ENCRYPTION_SIGNATURE)];
   if (!Error)
      Error=EncFil.Read(aucBuf,sizeof(DES_ENCRYPTION_SIGNATURE));
   if (!Error)
      {
      Decrypt(&aucBuf[0]);
      Decrypt(&aucBuf[8]);
      if (memcmp(aucBuf,DES_ENCRYPTION_SIGNATURE,sizeof(DES_ENCRYPTION_SIGNATURE))==0)
         bOK=true;
      }
   return bOK;
}

bool DESEncrypt::bDecryptFile(const String sEncFile, const String sDecFile)
{
   bool bOK=false;
   JFile EncFil('I');
   JFile::FILE_ERROR Error=EncFil.Open(sEncFile);
   DWORD dwSize=0;
   BYTE* pucBuf=NULL;
   if (!Error)
      {
      dwSize=EncFil.dwGetSize();
      if (dwSize==0)
         Error=JFile::F_SIZE_ERROR;
      }
   if (!Error)
      {
      pucBuf=new BYTE[dwSize];
      Error=EncFil.Read(pucBuf,dwSize);
      }
   if (!Error)
      Error=EncFil.Close();
   EncFil.ShowError(Error);
   if (!Error)
      {
      dwSize=nDecrypt(pucBuf,dwSize);
      JFile DecFil('O');
      Error=DecFil.Create(sDecFile);
      if (!Error)
         Error=DecFil.Write(pucBuf,dwSize);
      if (!Error)
         Error=DecFil.Close();
      DecFil.ShowError(Error);
      if (!Error)
         bOK=true;
      }
   delete[] pucBuf;
   return bOK;
}



