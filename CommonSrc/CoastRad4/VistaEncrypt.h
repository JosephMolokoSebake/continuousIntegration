//---------------------------------------------------------------------------

#ifndef VistaEncryptH
#define VistaEncryptH

#include "JAES.h"

//---------------------------------------------------------------------------

class VistaEncrypt
{

	enum
	{
		MAX_RX_BUF_SIZE 	= 1024*1024*10,
	};

	String sPW;
   JAES::KEY_SIZE KeySize;

	int nBlockCnt;
	int nDecErrCnt;

	int nIdx;
	int nRXSize;
	int nPlainSize;
	int nEncSize;

	BYTE* pucRXBuf;

	void Decrypt();


public:

	VistaEncrypt()
	{
      sPW="";
		pucRXBuf=new BYTE[MAX_RX_BUF_SIZE];
		Init();
	}

	~VistaEncrypt()
	{
		delete[] pucRXBuf;
	}

	void Init()
	{
		nBlockCnt=0;
		nDecErrCnt=0;
		nIdx=0;
		nRXSize=0;
	}

	void SetPW(const String s, const JAES::KEY_SIZE _KeySize)
	{
		sPW=s;
		KeySize=_KeySize;
   }

	int nEncrypt(BYTE* pucData, const int nPlainSize);

	int nAddDecryptByte(const BYTE uc);

	BYTE ucGetPlainByte(const int n) const
	{
		return pucRXBuf[n];
	}

	int nGetDecryptBlockCnt() const
	{
		return nBlockCnt;
	}

	int nGetDecryptErrCnt() const
	{
		return nDecErrCnt;
	}

};

#endif
