//---------------------------------------------------------------------------

#ifndef JDCSFileH
#define JDCSFileH
//---------------------------------------------------------------------------

#include "JList.h"

class JDCSFile
{

   HANDLE hFile;
   String sName;
   DWORD dwError;

	void ShowError();

	bool bSeek(const DWORD dwOffset);
	bool bSeek(const LARGE_INTEGER lnOffset);

public:

	JDCSFile(const String _sName, const bool bCreate, const bool bWriteThrough=true);

	~JDCSFile();

	bool bWrite(const DWORD dwOffs, const void* pData, const DWORD dwSize);
	bool bWrite(const LARGE_INTEGER lnOffset, const void* pData, const DWORD dwSize);

   bool bWrite(const void* pData, const DWORD dwSize);

   bool bAppend(const void* pData, const DWORD dwSize);

   bool bClose();

   bool bOK() const
   {
      return (hFile!=INVALID_HANDLE_VALUE);
   }

   String sGetName();

};


class JDoubleFile
{

   JDCSFile* pFil1;
   JDCSFile* pFil2;

   struct BufferBlock
   {
      DWORD dwSize;
      BYTE* pucData;

      BufferBlock()
      {
         dwSize=0;
         pucData=NULL;
      }

      BufferBlock(const BYTE* _pucData, const DWORD _dwSize)
      {
         dwSize=_dwSize;
         pucData=new BYTE[dwSize];
         memcpy(pucData,_pucData,dwSize);
      }

      ~BufferBlock()
      {
         delete[] pucData;
      }

   };

   JList<BufferBlock> BufList1;
   JList<BufferBlock> BufList2;

   BYTE* pucHeader1;
   BYTE* pucHeader2;
   DWORD dwHeaderSize;

   DWORD dwStoreInterval;
   DWORD dwNextStore1;
   DWORD dwNextStore2;

   bool abFirst[2];

   bool bFlushFile(JDCSFile* pFil, const BYTE* pucHeader, JList<BufferBlock>* pBufList, bool& bFirst);


public:

   JDoubleFile(const String _sName, const int nStoreIntervalMs);

   ~JDoubleFile();

   void StoreHeader(const void* pData, const DWORD dwSize);

   bool bStoreData(const void* pData, const DWORD dwSize);

};


#endif
