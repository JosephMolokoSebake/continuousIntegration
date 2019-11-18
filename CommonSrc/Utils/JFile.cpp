/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	File IO Utilities
 *    FILE NAME		FileIO.cpp
 *		PROJECT			Virtue IV/C0564
 *		ACTIVITY       6
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium
 *		OS					MS Windows 95/NT4/Win2000
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *    	File IO utilities for use with C++ Builder 3.
 *		CHANGE HISTORY
 *			V1.00		JGT 25/06/1998
 * 					Initial Version

 *       V1.01    JGT 20/12/1998
 *						Add String Variable alternatives

 *       V1.10    Use on C++ Builder 5

 *       V2.00    JGT 23/06/2011
						Use on C++ Builder XE with full support for 64 bit file sizes
 ************************************************************************/

//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JUtils.h"
#include "JFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


JFile::JFile(const char _cFileDirection, const DATA_TYPE _DataType)
{
	cFileDirection=(char)toupper(_cFileDirection);
	DataType=_DataType;
	hFile=INVALID_HANDLE_VALUE;
	lnFileSize.QuadPart=0;
	lnFilePos.QuadPart=0;
	sFileName="";
	pcASCIIBuf=NULL;
	Error=F_NO_ERROR;
}

JFile::~JFile()
{
	Close();
}

String JFile::sGetName() const
{
	return sFileName;
}

JFile::FILE_ERROR JFile::Create(const String sName)
{
	Error=Close();
	if (!Error)
		{
		sFileName=sName;
		hFile=::CreateFile(  sName.c_str(),
									GENERIC_WRITE | GENERIC_READ,
									0,
									NULL,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
		if (hFile==INVALID_HANDLE_VALUE)
			Error=F_CREATE_ERROR;
		else
			{
			if (Seek(0))
				Error=F_CREATE_ERROR;
			else
				{
				lnFileSize.QuadPart=0;
				lnFilePos.QuadPart=0;
				}
			}
		}
	return Error;
}

void JFile::SetName(const String sName)
{
	sFileName=sName;
}

JFile::FILE_ERROR JFile::Open(const String sName)
{
	Error=Close();
	if (!Error)
		{
		sFileName=sName;
		DWORD dwAccess;
		switch(cFileDirection)
			{
			case 'I':
			case 'i': 	dwAccess=GENERIC_READ;						break;
			case 'O':
			case 'o': 	dwAccess=GENERIC_WRITE;						break;
			default:		dwAccess=GENERIC_WRITE | GENERIC_READ;	break;
			}
		hFile=::CreateFile(  sName.c_str(),
									dwAccess,
									0,
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
		if (hFile==INVALID_HANDLE_VALUE)
			Error=F_OPEN_ERROR;
		else
			{
			Error=GetSize(lnFileSize);
			if ((!Error)&&(DataType==ASCII_TYPE)&&(lnFileSize.QuadPart>0))
				{
				//Limit buffer to 10 MB
				if (lnFileSize.QuadPart>1024*1024*10)
					dwASCIIBufSize=1024*1024*10;
				else
					dwASCIIBufSize=lnFileSize.LowPart;
				pcASCIIBuf=new char[dwASCIIBufSize];
				DataType=BINARY_TYPE;
				Error=Read(pcASCIIBuf,dwASCIIBufSize);
				DataType=ASCII_TYPE;
				dwASCIIBufOffs=0;
				}
			lnFilePos.QuadPart=0;
			}
		}
	return Error;
}

JFile::FILE_ERROR JFile::Open()
{
	return Open(sFileName);
}

JFile::FILE_ERROR JFile::Close()
{
	Error=F_NO_ERROR;
	if (hFile!=INVALID_HANDLE_VALUE)
		{
		if (!::CloseHandle(hFile))
			Error=F_CLOSE_ERROR;
		hFile=INVALID_HANDLE_VALUE;
		}
	delete[] pcASCIIBuf;
	pcASCIIBuf=NULL;
	return Error;
}

JFile::FILE_ERROR JFile::Write(const DWORD dwOffset, const void *pData, const DWORD dwBytes)
{
	Error=Seek(dwOffset);
	if (!Error)
		Error=Write(pData,dwBytes);
	return Error;
}

JFile::FILE_ERROR JFile::Write(const LARGE_INTEGER lnOffset, const void *pData, const DWORD dwBytes)
{
	Error=Seek(lnOffset);
	if (!Error)
		Error=Write(pData,dwBytes);
	return Error;
}

JFile::FILE_ERROR JFile::Write(const void* pData, const DWORD dwBytes)
{
	Error=F_NO_ERROR;
	DWORD dwWritten;
	if (!::WriteFile(hFile,pData,dwBytes,&dwWritten,NULL))
		Error=F_WRITE_ERROR;
	else if (dwWritten!=dwBytes)
		Error=F_WRITE_ERROR;
	else
		lnFilePos.QuadPart+=dwBytes;
	return Error;
}

JFile::FILE_ERROR JFile::Append(const void* pData, const DWORD dwBytes)
{
	Error=F_NO_ERROR;
	LARGE_INTEGER lnDist;
	lnDist.QuadPart=0;
	if (!::SetFilePointerEx(hFile,lnDist,&lnFilePos,FILE_END))
		Error=F_SEEK_ERROR;
	else
		Error=Write(pData,dwBytes);
	return Error;
}

JFile::FILE_ERROR JFile::Read(const DWORD dwOffset, void* pData, const DWORD dwBytes)
{
	Error=Seek(dwOffset);
	if (!Error) Error=Read(pData,dwBytes);
	return Error;
}

JFile::FILE_ERROR JFile::Read(const LARGE_INTEGER lnOffset, void* pData, const DWORD dwBytes)
{
	Error=Seek(lnOffset);
	if (!Error) Error=Read(pData,dwBytes);
	return Error;
}

JFile::FILE_ERROR JFile::Read(void* pData, const DWORD dwBytes)
{
	Error=F_NO_ERROR;
	DWORD dwRead;
	if (!::ReadFile(hFile,pData,dwBytes,&dwRead,NULL))
		Error=F_READ_ERROR;
	else if (dwRead!=dwBytes)
		Error=F_READ_ERROR;
	else
		lnFilePos.QuadPart+=dwBytes;
	return Error;
}

JFile::FILE_ERROR JFile::Read(char* pszSt, const DWORD dwMaxLen)
{
	Error=F_NO_ERROR;
	if (DataType==ASCII_TYPE)
		{
		if (lnFilePos.QuadPart==lnFileSize.QuadPart)
			Error=F_EOF;
		else if (pcASCIIBuf)
			{
			sASCIIBuf="";
			bool bFin=false;
			while(!bFin)
				{
				while((dwASCIIBufOffs<dwASCIIBufSize)&&(!bFin))
					{
					bFin=bAddASCIIChar(pcASCIIBuf[dwASCIIBufOffs++]);
					++lnFilePos.QuadPart;
					}
				if (!bFin)
					{
					if (lnFilePos.QuadPart==lnFileSize.QuadPart)
						bFin=true;
					else
						{
						//Read next block
						__int64 ln=lnFileSize.QuadPart-lnFilePos.QuadPart;
						if (ln<dwASCIIBufSize)
							dwASCIIBufSize=ln;
						ln=lnFilePos.QuadPart;
						DataType=BINARY_TYPE;
						Error=Read(pcASCIIBuf,dwASCIIBufSize);
						DataType=ASCII_TYPE;
						lnFilePos.QuadPart=ln;
						dwASCIIBufOffs=0;
						if (Error)
							bFin=true;
						}
					}
				}
			if ((bFin)&&(!Error))
				{
				if (sASCIIBuf.Length()>0)
					strncpy(pszSt,sASCIIBuf.c_str(),dwMaxLen);
				else
					pszSt[0]=0;
				}
			}
		else
			Error=F_READ_ERROR;
		}
	else
		Error=Read((void*)pszSt,dwMaxLen);
	return Error;
}

JFile::FILE_ERROR JFile::Read(String& sText)
{
	Error=F_NO_ERROR;
	sText="";
	if (DataType==ASCII_TYPE)
		{
		if (lnFilePos.QuadPart==lnFileSize.QuadPart)
			Error=F_EOF;
		else if (pcASCIIBuf)
			{
			sASCIIBuf="";
			bool bFin=false;
			while((!bFin)&&(!Error))
				{
				while((dwASCIIBufOffs<dwASCIIBufSize)&&(!bFin))
					{
					bFin=bAddASCIIChar(pcASCIIBuf[dwASCIIBufOffs++]);
					++lnFilePos.QuadPart;
					}
				if (!bFin)
					{
					if (lnFilePos.QuadPart==lnFileSize.QuadPart)
						bFin=true;
					else
						{
						//Read next block
						__int64 ln=lnFileSize.QuadPart-lnFilePos.QuadPart;
						if (ln<dwASCIIBufSize)
							dwASCIIBufSize=ln;
						ln=lnFilePos.QuadPart;
						DataType=BINARY_TYPE;
						Error=Read(pcASCIIBuf,dwASCIIBufSize);
						DataType=ASCII_TYPE;
						lnFilePos.QuadPart=ln;
						dwASCIIBufOffs=0;
						if (Error)
							bFin=true;
						}
					}
				}
			if ((bFin)&&(!Error))
				sText=String(sASCIIBuf);
			}
		else
			Error=F_READ_ERROR;
		}
	else
		{
		int nLen;
		Error=Read(&nLen,sizeof(nLen));
		if (nLen>2048)
			nLen=2048;
		else if (nLen<0)
			nLen=0;
		char* pszSt=new char[nLen+1];
		if (!Error) Error=Read(pszSt,nLen);
		if (!Error)
			{
			pszSt[nLen]=0;
			sText=String(pszSt);
			}
		delete[] pszSt;
		}
	return Error;
}

JFile::FILE_ERROR JFile::Write(const String wsText)
{
	if (DataType==ASCII_TYPE)
		Error=Text(wsText);
	else
		{
		AnsiString sText=wsText;    	//NB Write AnsiString !!!
		int nLen=sText.Length();
		Error=Write(&nLen,sizeof(nLen));
		if (!Error)
			Error=Write(sText.c_str(),nLen);
		}
	return Error;
}

JFile::FILE_ERROR JFile::Write(TFont* pFont)
{
#pragma option push -a4    //Set compiler to DWORD alignment
	struct
	{
		TColor Color;
		SHORT nSize;
		WORD uStyle;
   } FontData;
#pragma option pop         //Set compiler to default alignment
   FontData.Color=pFont->Color;
   FontData.nSize=(SHORT)pFont->Size;
   FontData.uStyle=0;
   if (pFont->Style.Contains(fsBold)) FontData.uStyle|=0x01;
   if (pFont->Style.Contains(fsItalic)) FontData.uStyle|=0x02;
   if (pFont->Style.Contains(fsUnderline)) FontData.uStyle|=0x04;
   if (pFont->Style.Contains(fsStrikeOut)) FontData.uStyle|=0x08;
   Error=Write(&FontData,sizeof(FontData));
   if (!Error) Error=Write(pFont->Name);
   return Error;
}

JFile::FILE_ERROR JFile::Read(TFont* pFont)
{
#pragma option push -a4    //Set compiler to DWORD alignment
   struct
   {
      TColor Color;
      SHORT nSize;
      WORD uStyle;
   } FontData;
#pragma option pop         //Set compiler to default alignment
   Error=Read(&FontData,sizeof(FontData));
   String sSt;
   if (!Error) Error=Read(sSt);
   if (!Error)
      {
      pFont->Name=sSt;
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
   return Error;
}

JFile::FILE_ERROR JFile::Seek(const DWORD dwOffset)
{
	Error=F_SEEK_ERROR;
	LARGE_INTEGER lnOffset;
	lnOffset.LowPart=dwOffset;
	if (dwASCIIBufSize>0)
		dwASCIIBufOffs=dwOffset%dwASCIIBufSize;
	lnOffset.HighPart=0;
	if (!::SetFilePointerEx(hFile,lnOffset,&lnFilePos,FILE_BEGIN))
		Error=F_SEEK_ERROR;
	else
		Error=F_NO_ERROR;
	return Error;
}

JFile::FILE_ERROR JFile::Seek(const LARGE_INTEGER lnOffset)
{
	Error=F_SEEK_ERROR;
	if (!::SetFilePointerEx(hFile,lnOffset,&lnFilePos,FILE_BEGIN))
		Error=F_SEEK_ERROR;
	else
		Error=F_NO_ERROR;
	return Error;
}

JFile::FILE_ERROR JFile::GetSize(DWORD &dwSize)
{
	Error=F_SEEK_ERROR;
	LARGE_INTEGER lnSize;
	if (!::GetFileSizeEx(hFile,&lnSize))
		{
		Error=F_SEEK_ERROR;
		dwSize=0;
		}
	else
		{
		Error=F_NO_ERROR;
		if (lnSize.HighPart>0)
			dwSize=0xFFFFFFFF;
		else
			dwSize=lnSize.LowPart;
		}
	return Error;
}

JFile::FILE_ERROR JFile::GetSize(LARGE_INTEGER &lnSize)
{
	Error=F_SEEK_ERROR;
	if (!::GetFileSizeEx(hFile,&lnSize))
		Error=F_SEEK_ERROR;
	else
		Error=F_NO_ERROR;
	return Error;
}

DWORD JFile::dwGetSize()
{
	DWORD dwSize;
	if (GetSize(dwSize)==F_NO_ERROR)
		return dwSize;
	else
		return 0;
}

JFile::FILE_ERROR JFile::GetPos(DWORD& dwPos)
{
	dwPos=lnFilePos.LowPart;
	Error=F_NO_ERROR;
	return Error;
}

JFile::FILE_ERROR JFile::GetPos(LARGE_INTEGER& lnPos)
{
	lnPos=lnFilePos;
	Error=F_NO_ERROR;
	return Error;
}

JFile::FILE_ERROR JFile::Flush()
{
	if (::FlushFileBuffers(hFile))
		Error=F_NO_ERROR;
	else
		Error=F_FLUSH_ERROR;
	return Error;
}

JFile::FILE_ERROR JFile::Text(const String s)
{
	Error=F_WRITE_ERROR;
	if ((hFile)&&(DataType==ASCII_TYPE))
		{
		AnsiString s1=s;
		Error=Write(s1.c_str(),s1.Length());
		}
	return Error;
}

JFile::FILE_ERROR JFile::AppendText(const String s)
{
	Error=F_WRITE_ERROR;
	if ((hFile)&&(DataType==ASCII_TYPE))
		{
		LARGE_INTEGER lnDist,lnNewPnt;
		lnDist.QuadPart=0;
		if (!::SetFilePointerEx(hFile,lnDist,&lnNewPnt,FILE_END))
			Error=F_SEEK_ERROR;
		else
			Error=Text(s);
		}
	return Error;
}

JFile::FILE_ERROR JFile::AppendTextLine(const String s)
{
	return AppendText(s+"\r\n");
}


JFile::FILE_ERROR JFile::TextLine(const String s)
{
	return Text(s+"\r\n");
}

String JFile::sGetError(const JFile::FILE_ERROR Error)
{
	switch(Error)
		{
		case F_CREATE_ERROR:		return "File Creation Error";
		case F_OPEN_ERROR:		return "File Open Error";
		case F_CLOSE_ERROR:		return "File Close Error";
		case F_SEEK_ERROR:		return "File Seek Error";
		case F_READ_ERROR:		return "File Read Error";
		case F_WRITE_ERROR:		return "File Write Error";
		case F_MEM_ERROR:			return "File Memory Error";
		case F_DIR_ERROR:			return "Folder does not Exist";
		case F_DRIVE_ERROR:		return "Drive not Ready";
		case F_ABORT_ERROR:		return "Operation Aborted by User";
		case F_ERASE_ERROR:		return "File Erase Error";
		case F_FLUSH_ERROR:		return "Data Cache Flush Error";
		case F_FORMAT_ERROR:		return "Invalid Data Format";
		case F_VERSION_ERROR:	return "Incorrect File Version";
		case F_CRC_ERROR:			return "CRC Error";
		case F_COMMS_ERROR:		return "Communications Error";
		case F_CONNECT_ERROR:	return "Connect Error";
		case F_LOGIN_ERROR:		return "Login Error";
		case F_TRANSFER_ERROR:	return "Transfer Error";
		case F_CONFIG_ERROR: 	return "Configuration Error";
		case F_COPY_ERROR:		return "Copy Error";
		case F_SIZE_ERROR:		return "Size Error";
		case F_TEXTLINE_ERROR:	return "Text Line Too Long";
		case F_DECRYPT_ERROR:	return "Decryption Error";
		case F_NO_ERROR:    		return "No Error";
		default:						return "Unknown Error";
		}
}

//void JFile::ShowError(const String sMessage, const String sHeading, const int nTimeout_sec)
//{
//	ShowError(sMessage,sHeading,nTimeout_sec);
//}

void JFile::ShowError(const JFile::FILE_ERROR Error, const int nTimeout_sec)
{
	if ((Error)&&(Error!=F_ABORT_ERROR))
		{
		String s=L"File: "+sFileName+"\nType: "+sGetError(Error);
		nShowError(s,String("Disk I/O Error"),nTimeout_sec);
		}
}


JFile::FILE_ERROR JFile::WriteSciHeader(const int yn, const long dn)
{
	char st[20];
	strcpy(st," SCIPLOT");
	st[0]=7;
	st[8]=(BYTE)(yn>>8);
	st[9]=(BYTE)(yn&0xff);
	memcpy(&st[12],&dn,4);
	return Write(0,st,16);
}

JFile::FILE_ERROR JFile::ReadSciHeader(int& yn, DWORD& dwDataNum)
{
	char st[20];
	Error=Read(0,st,16);
	if (!Error)
		{
		char st1[10];
		strcpy(st1," SCIPLOT");
		st1[0]=7;
		if ((!strncmp(st,st1,8)))
         {
			yn=st[9];
			yn|=((int)(st[8]))<<8;
			DWORD dwSize;
			GetSize(dwSize);
			dwSize-=16;
			dwDataNum=dwSize/((yn+1)*sizeof(float));
			}
		else
			Error=F_FORMAT_ERROR;
		}
	return Error;
}

JFile::FILE_ERROR JFile::ReadASCIIFormat(int& yn)
{
	char szSt[1024];
	Error=Seek(0);
	if (!Error) Error=Read(szSt,1024);
	if (!Error)
      {
		float afDummy[30];
		yn=sscanf(szSt,"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",
			&afDummy[0],&afDummy[1],&afDummy[2],&afDummy[3],
			&afDummy[4],&afDummy[5],&afDummy[6],&afDummy[7],
			&afDummy[8],&afDummy[9],&afDummy[10],&afDummy[11],
			&afDummy[12],&afDummy[13],&afDummy[14],&afDummy[15],
			&afDummy[16],&afDummy[17],&afDummy[18],&afDummy[19])-1;
		if (yn<=0) Error=F_READ_ERROR; else Error=Seek(0);
		}
	return Error;
}





