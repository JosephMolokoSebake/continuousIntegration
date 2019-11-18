//---------------------------------------------------------------------------

#ifndef JDESEncryptH
#define JDESEncryptH
//---------------------------------------------------------------------------

#include "JDES.h"

class DESEncrypt
{

   triple_des* p3Des;
   des* pDes;

   void Encrypt(BYTE* pucData); //8 bytes at a time
   void Decrypt(BYTE* pucData); //8 bytes at a time

   String sEncode6Bit(const BYTE* pucData, const int nLen);
   int nDecode6Bit(const String sData, BYTE* pucDec);

   char cLookupByte(const BYTE uc) const
   {
      if (uc<=0x1F)
         return uc+0x40;
      else
         return uc;
   }

   BYTE ucLookupChar(const char c) const
   {
      if (c<=0x3F)
         return c;
      else
         return c-0x40;
   }

public:

   DESEncrypt(const String sPW, const bool bTripleDes=false);

   ~DESEncrypt()
   {
      delete p3Des;
      delete pDes;
   }

   int nEncryptRaw(BYTE* pucData, const WORD uSize);
   //Encrypt without adding signature
   //NB. pucData must have space reserved for 7 bytes more than dwSize !!!

   int nDecryptRaw(BYTE* pucData, const WORD uSize);
   //Decrypt without signature
   //returns decrypted size

   int nEncrypt(BYTE* pucData, const DWORD dwSize, TProgressBar* pProg=NULL);   //returns encrypted size
   //NB. pucData must have space reserved for 27 bytes more than dwSize !!!

   int nDecrypt(BYTE* pucData, const DWORD dwSize, TProgressBar* pProg=NULL);   //returns decrypted size
   //returns Decrypted size if OK
   //returns 0 if unknown error
   //returns -1 if decryption error (probably incorrect password)

	String sEncryptString(const String sDec);
	//sDec can have any characters
	//Returns encrypted string (valid chars only)

	String sDecryptString(const String sEnc);
	//Returns Decrypted string

	bool bEncryptFile(const String sDecFile, const String sEncFile);

	bool bFileEncrypted(const String sFile);

   bool bDecryptFile(const String sEncFile, const String sDecFile);

};


#endif
