//---------------------------------------------------------------------------

#ifndef JTextFileH
#define JTextFileH
//---------------------------------------------------------------------------

#include "JFile.h"
#include "JList.h"

class JTextFile
{

   enum
   {
      MAX_BLOCK_SIZE       = 1024*1024,
      MAX_CD_STRING_LENGTH = 128,
      MAX_CD_COUNT         = 128,
   };

   char acCD[MAX_CD_COUNT][MAX_CD_STRING_LENGTH];

   int nFullCount;
   int nLastSize;
   int nBlock;
   int nBlockSize;
   int nBlockOffs;

   JFile* pFil;

   JFile::FILE_ERROR E;
   DWORD dwSize;
   int nLines;
   char* pcBuf;
   DWORD dwOffs;
   String sName;
   bool bLast;


   struct LineStruct
   {
      char* pcBuf;
      int nLen;

      LineStruct()
      {
         pcBuf=NULL;
      }

      LineStruct(const char* pc, const int n)
      {
         if (n>0)
            {
            pcBuf=new char[n];
            memcpy(pcBuf,pc,n);
            }
         else
            pcBuf=NULL;
         nLen=n;
      }

      ~LineStruct()
      {
         delete[] pcBuf;
      }

   };

   JList<LineStruct> BufList;


public:

   JTextFile(const String sFile, const bool bCreate);

   ~JTextFile();

   JFile::FILE_ERROR GetError()
   {
      return E;
   }

   int nGetNewLines() const
   {
      return nLines;
   }

   JFile::FILE_ERROR NextLine(String& s);
   void GetNextLine(char* psz, const int nMaxLen);

   int nNextCDLine();
   String sGetPar(const int n);

   int nAddLine(const String sLine);
   int nAddLine(const char* psz, const int nLen);

   JFile::FILE_ERROR Flush();

};

#endif
