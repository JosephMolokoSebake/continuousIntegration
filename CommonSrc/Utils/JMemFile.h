//---------------------------------------------------------------------------

#ifndef JMemFileH
#define JMemFileH
//---------------------------------------------------------------------------

#include "JFile.h"

class JMemFile
{

   BYTE* pucBuffer;
   DWORD dwPos;
   DWORD dwSize;

protected:

   JFile::FILE_ERROR Error;
   String sFileName;

public:

   JMemFile()
   {
      pucBuffer=NULL;
   }

   ~JMemFile()
   {
      delete[] pucBuffer;
   }

   JFile::FILE_ERROR ReadBuffer(const String sFile, const DWORD dwExtra=0);
	JFile::FILE_ERROR StoreBuffer(const String sFile);

	void XORBuffer(const BYTE ucXOR);

   void FillBuffer(const BYTE* pucData, const DWORD dwDataSize);

   bool bRead(void* pData, const DWORD dwBytes);
   bool bRead(const DWORD dwStart, void* pData, const DWORD dwBytes);
   bool bRead(String& s, const int nMaxLen=0);
	bool bRead(const DWORD dwStart, String& s, const int nMaxLen=0);
	bool bRead(TFont* pFont);

   bool bSeek(const DWORD dwNewPos);
   bool bAdvance(const DWORD dwBytes);

   bool bWrite(const DWORD dwStart, void* pData, const DWORD dwBytes);

   DWORD dwGetSize() const
   {
      return dwSize;
   }

   DWORD dwGetPos() const
   {
      return dwPos;
   }

   void SetPos(const DWORD _dwPos)
   {
      dwPos=_dwPos;
   }

   bool bValidSize(const DWORD dwBytes) const
   {
      return (dwPos+dwBytes<=dwSize);
   }

   BYTE* pucGetBuffer()
   {
      return pucBuffer;
   }

   bool bNextString(String& s);
	bool bNextString(char* pszSt, const int nMaxLen);

   int nNextVal(double& dV);
   int nNextVal(double& dV1, double& dV2);
   int nNextVal(double& dV1, double& dV2, double& dV3);
   int nNextVal(double& dV1, double& dV2, double& dV3, double& dV4);

};


#endif
