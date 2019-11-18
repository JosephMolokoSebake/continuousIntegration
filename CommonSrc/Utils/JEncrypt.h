//---------------------------------------------------------------------------

#ifndef JEncryptH
#define JEncryptH
//---------------------------------------------------------------------------

class Encryption
{

   BYTE* apucEncr[256];
   BYTE* apucDecr[256];
   BYTE aucCode[256];
   int nSize;
   int nNext;
   WORD uSeed;

   struct
   {
      BYTE* pucEncr;
      BYTE* pucDecr;
   } aRow[256];

   void Shuffle(BYTE* pucArray, const int nCut);

   BYTE ucRandom();

public:

   Encryption(const String sCodeWord, const WORD _uSeed=157, const int _nSize=256);

   ~Encryption();

   void Encrypt(const void* pData, void* pEncrypted, const int nSize);

   void Decrypt(const void* pEncrypted, void* pDecrypted, const int nSize);

   String sEncrypt(const String s);

	String sDecrypt(const String sEn);

};

#endif
