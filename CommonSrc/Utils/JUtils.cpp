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
 *			V1.00		Initial Version	JGT 25/06/1998
 *       V1.01    Add String Variable alternatives  JGT 20/12/1998
 *       V1.10    Use on C++ Builder 5
 ************************************************************************/

//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JUtils.h"
#include "JInfoDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

String g_sSelectFolder_Default;

String sFileNameOnly(const String sName, const bool bExt)
{
   ExtractFileName(sName);
   if (bExt)
      return ExtractFileName(sName);
   else
		return ExtractFileName(ChangeFileExt(sName,L""));
}

String sFileExtOnly(const String sName, const bool bIncludePeriod)
{
   String sExt=ExtractFileExt(sName);
   if (!bIncludePeriod)
      {
      if (sExt.Length()>0)
         {
         if (sExt[1]=='.')
            {
            sExt[1]=' ';
            sExt=sExt.Trim();
            }
         }
      }
   return sExt;
}

String sFileDriveOnly(const String sName)
{
	String sDrive=ExtractFileDrive(sName);
   return IncludeTrailingBackslash(sDrive);
}

String sStripDrive(const String sFolder)
{
	wchar_t drive[MAXDRIVE];
	wchar_t dir[MAXDIR];
	wchar_t file[MAXFILE];
	wchar_t ext[MAXEXT];
	String sF=sFolder+"\\Dummy.txt";
	::_wfnsplit(sF.c_str(),drive,dir,file,ext);
	sF=String(dir);
	return sF;
}



bool bFileExtMatch(const String sFile, const String sExt)
{
	String sFile1=sFileExtOnly(sFile,false);
	String sExt1=sExt;
	return sFile1.CompareIC(sExt1)==0;
}

String sFileDirOnly(const String sName)
{
	return IncludeTrailingBackslash(ExtractFilePath(sName));
}

String sFileNewExt(const String sName, const String sExt)
{
	String s=sExt;
	if (s.Length()>0)
		{
		if (s[1]!='.')
			s=L"."+s;
		}
	return ChangeFileExt(sName,s);
}

String sValidateFileName(const String sName, const char cReplacement)
{
	String s=sName;
	String sInvalid="//\\:""*?<>|";
	for (int i=1; i<sName.Length(); i++)
		{
		if (sName.IsDelimiter(sInvalid,i))
			s[i]=cReplacement;
		}
	return s;
}

JFile::FILE_ERROR EraseFile(const String sName)
{
	if (_wremove(sName.c_str()))
		return JFile::F_ERASE_ERROR;
	else
		return JFile::F_NO_ERROR;
}

DWORD dwGetFileSize(const String sName)
{
	DWORD dwSize=0;
	WIN32_FIND_DATA FileData;
   HANDLE FileHandle=::FindFirstFile(sName.c_str(),&FileData);
   if (FileHandle!=INVALID_HANDLE_VALUE)
      {
     	if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
         dwSize=FileData.nFileSizeLow;
      ::FindClose(FileHandle);
      }
   return dwSize;
}

bool bDirExist(const String sName)
{
	bool bExist=false;
	WIN32_FIND_DATA FileData;
	String s=ExcludeTrailingBackslash(sName);
	HANDLE FileHandle=::FindFirstFile(s.c_str(),&FileData);
	if (FileHandle!=INVALID_HANDLE_VALUE)
      {
     	bExist=(FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0;
      ::FindClose(FileHandle);
      }
   return bExist;
}

bool bFileExist(const String sName)
{
	bool bExist=false;
	WIN32_FIND_DATA FileData;
	HANDLE FileHandle=::FindFirstFile(sName.c_str(),&FileData);
	if (FileHandle!=INVALID_HANDLE_VALUE)
		{
		bExist=(FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0;
		::FindClose(FileHandle);
		}
	return bExist;
}

String sGetDefaultDir()
{
	return IncludeTrailingBackslash(GetCurrentDir());
}

String sGetVolumeDrive(const String sVolumeName)
{
	char cDrive='B';
	String sDrive="";
	bool bFound;
	String sVol=sVolumeName.UpperCase().Trim();
	do
		{
		bFound=false;
		sDrive=" :\\";
		cDrive++;
		sDrive[1]=cDrive;
		TCHAR volumeName[225];
		TCHAR fileSystemName[225];
		DWORD serialNumber=0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags=0;
		if(::GetVolumeInformation(sDrive.c_str(),volumeName,ARRAYSIZE(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			ARRAYSIZE(fileSystemName)))
			{
		  String sVolFound=String(volumeName).UpperCase().Trim();
		  bFound=(sVol==sVolFound);
/*
		  _tprintf(_T("Volume Name: %s\n"), volumeName);
		  _tprintf(_T("Serial Number: %lu\n"), serialNumber);
		  _tprintf(_T("File System Name: %s\n"), fileSystemName);
		  _tprintf(_T("Max Component Length: %lu\n"), maxComponentLen);
*/
		  }
		else
			{
			sDrive="";
			}
		}
	while((!bFound)&&(cDrive<'Z'));
	if (!bFound)
		sDrive="";
	return sDrive;
}

String sGetDriveVolume(const String sDrive)
{
		TCHAR volumeName[225];
		TCHAR fileSystemName[225];
		DWORD serialNumber=0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags=0;
		if(::GetVolumeInformation(sDrive.c_str(),volumeName,ARRAYSIZE(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			ARRAYSIZE(fileSystemName)))
		  	return String(volumeName).Trim();
		else
			return "";
}



String sGetRootDir(const String sDir)
{
	wchar_t drive[MAXDRIVE];
	wchar_t dir[MAXDIR];
	wchar_t file[MAXFILE];
	wchar_t ext[MAXEXT];
	::_wfnsplit(sDir.c_str(),drive,dir,file,ext);
	return String(drive);
}

DWORD dwGetDiskFree_MB(const String sDir)
{
   ULARGE_INTEGER qwFreeBytesAvailableToCaller;
	ULARGE_INTEGER qwTotalNumberOfBytes;
   int nRet;
   if (sDir.IsEmpty())
      nRet=::GetDiskFreeSpaceEx(NULL,&qwFreeBytesAvailableToCaller,&qwTotalNumberOfBytes,NULL);
   else
      nRet=::GetDiskFreeSpaceEx(sDir.c_str(),&qwFreeBytesAvailableToCaller,&qwTotalNumberOfBytes,NULL);
   if (nRet)
      {
      DWORD dwLow=qwFreeBytesAvailableToCaller.LowPart;
      DWORD dwHigh=qwFreeBytesAvailableToCaller.HighPart;
      dwLow/=(1024*1024);  //Get MegaBytes
      dwHigh*=4096;        //Get MegaBytes
      return (int)(dwLow+dwHigh);
      }
   else
      return 0xFFFFFFFF;
}

DWORD dwGetTotalDiskSpace_MB(const String sDir)
{
   ULARGE_INTEGER qwFreeBytesAvailableToCaller;
   ULARGE_INTEGER qwTotalNumberOfBytes;
   int nRet;
   if (sDir.IsEmpty())
      nRet=::GetDiskFreeSpaceEx(NULL,&qwFreeBytesAvailableToCaller,&qwTotalNumberOfBytes,NULL);
   else
      nRet=::GetDiskFreeSpaceEx(sGetRootDir(sDir).c_str(),&qwFreeBytesAvailableToCaller,&qwTotalNumberOfBytes,NULL);
   if (nRet)
      {
      DWORD dwLow=qwTotalNumberOfBytes.LowPart;
      DWORD dwHigh=qwTotalNumberOfBytes.HighPart;
      dwLow/=(1024*1024);  //Get MegaBytes
      dwHigh*=4096;        //Get MegaBytes
      return (dwLow+dwHigh);
      }
   else
      return 0xFFFFFFFF;
}


double dGetMemTotal_MB()
{
	MEMORYSTATUSEX mem;
	mem.dwLength=sizeof(mem);
	if (::GlobalMemoryStatusEx(&mem))
		{
		DWORD* pdw;
		pdw=(DWORD*)&mem.ullTotalPhys;
		DWORD dwLow=pdw[0];
		DWORD dwHigh=pdw[1];
		double dTotal_MB=dwLow/(1024.0*1024.0);
		dTotal_MB+=(dwHigh*4096.0);
		return dTotal_MB;
		}
	return 0.0;
}

double dGetMemAvail_MB()
{
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	if (::GlobalMemoryStatusEx(&mem))
		{
		DWORD* pdw;
		pdw=(DWORD*)&mem.ullAvailPhys;
		DWORD dwLow=pdw[0];
		DWORD dwHigh=pdw[1];
		double dAvail_MB=dwLow/(1024.0*1024.0);
		dAvail_MB+=(dwHigh*4096.0);
		return dAvail_MB;
		}
	return 0.0;
}

bool bMakeDir(const String sDir)
{
	String sNewDir=ExcludeTrailingBackslash(sDir);
	WIN32_FIND_DATA FileData;
	HANDLE FileHandle=::FindFirstFile(sNewDir.c_str(),&FileData);
	bool bExist=false;
	if (FileHandle!=INVALID_HANDLE_VALUE)
		{
		if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0)
         bExist=true;
      ::FindClose(FileHandle);
      }
   if (bExist)
      return true;
   else
      return ::CreateDirectory(sNewDir.c_str(),NULL);
}

DWORD dwGetExeVersion(const bool bNewStyle)
{
   DWORD dwVer=0;
	DWORD dwHandle;
	String s=ParamStr(0);
	DWORD dwSize=::GetFileVersionInfoSize(s.c_str(),&dwHandle);
	if (dwSize>0)
		{
		wchar_t* pcVer=new wchar_t[dwSize];
		bool bRet=::GetFileVersionInfo(s.c_str(),dwHandle,dwSize,pcVer);
		if (bRet)
			{
			void* pVer;
         VS_FIXEDFILEINFO VerInfo;
      	UINT uLen;
         bRet=::VerQueryValue(pcVer,L"\\",&pVer,&uLen);
         if (bRet)
            {
            VS_FIXEDFILEINFO VerInfo;
            memcpy(&VerInfo,pVer,sizeof(VS_FIXEDFILEINFO));
            if (bNewStyle)
               {
               dwVer=(VerInfo.dwFileVersionMS<<8)&0xFF000000;
               dwVer|=(VerInfo.dwFileVersionMS<<16)&0x00FF0000;
               dwVer|=(VerInfo.dwFileVersionLS>>8)&0x0000FF00;
               dwVer|=VerInfo.dwFileVersionLS&0x000000FF;
               }
            else
               {
               dwVer=(VerInfo.dwFileVersionMS>>16)*1000;
               dwVer+=(VerInfo.dwFileVersionMS&0x0000FFFF)*100;
               dwVer+=(VerInfo.dwFileVersionLS>>16)*10;
               dwVer+=(VerInfo.dwFileVersionLS&0x0000FFFF);
               }
            }
         }
      delete[] pcVer;
      }
   return dwVer;
}

DWORD dwGetExeVersion(const String sFileName,const bool bNewStyle)
{
	DWORD dwVer=0;
	DWORD dwHandle;
	DWORD dwSize=::GetFileVersionInfoSize(sFileName.c_str(),&dwHandle);
	if (dwSize>0)
		{
		wchar_t* pcVer=new wchar_t[dwSize];
		bool bRet=::GetFileVersionInfo(sFileName.c_str(),dwHandle,dwSize,pcVer);
		if (bRet)
			{
			void* pVer;
			VS_FIXEDFILEINFO VerInfo;
			UINT uLen;
			bRet=::VerQueryValue(pcVer,L"\\",&pVer,&uLen);
			if (bRet)
				{
				VS_FIXEDFILEINFO VerInfo;
				memcpy(&VerInfo,pVer,sizeof(VS_FIXEDFILEINFO));
				if (bNewStyle)
					{
					dwVer=(VerInfo.dwFileVersionMS<<8)&0xFF000000;
					dwVer|=(VerInfo.dwFileVersionMS<<16)&0x00FF0000;
					dwVer|=(VerInfo.dwFileVersionLS>>8)&0x0000FF00;
					dwVer|=VerInfo.dwFileVersionLS&0x000000FF;
					}
				else
					{
					dwVer=(VerInfo.dwFileVersionMS>>16)*1000;
					dwVer+=(VerInfo.dwFileVersionMS&0x0000FFFF)*100;
					dwVer+=(VerInfo.dwFileVersionLS>>16)*10;
					dwVer+=(VerInfo.dwFileVersionLS&0x0000FFFF);
					}
				}
			}
		delete[] pcVer;
		}
	return dwVer;
}


int nGetVersionPart(const int n, const DWORD dwVer)
{
   return (int)((dwVer>>(n*8))&0x000000FF);
}


String sGetExeVersion(const bool bNewStyle, const bool bHex)
{
   if (!bNewStyle)
      {
      DWORD dwVer=dwGetExeVersion();
      int nMajor=dwVer/1000;
		int nMinor=(dwVer%1000)/10;
		char c=(char)(dwVer%10+'a'-1);
		String s;
		if (c<'a')
			return s.sprintf(L"%d.%02d",nMajor,nMinor);
		else
			return s.sprintf(L"%d.%02d%c",nMajor,nMinor,c);
		}
	else
		{
		String s;
		if (bHex)
			s.printf(L"0x%08X",dwGetExeVersion(true));
		else
			{
			DWORD dwHandle;
			s=ParamStr(0);
			DWORD dwSize=::GetFileVersionInfoSize(s.c_str(),&dwHandle);
			if (dwSize>0)
				{
				wchar_t* pcVer=new wchar_t[dwSize];
				void* pVer;
				UINT uLen;
				bool bRet=::GetFileVersionInfo(s.c_str(),dwHandle,dwSize,pcVer);
				if (bRet)
					bRet=::VerQueryValue(pcVer,L"\\",&pVer,&uLen);
				if (bRet)
					{
					VS_FIXEDFILEINFO VerInfo;
					memcpy(&VerInfo,pVer,sizeof(VS_FIXEDFILEINFO));
					s.printf(L"version %u.%u.%u.%u",VerInfo.dwFileVersionMS>>16,VerInfo.dwFileVersionMS&0x0000FFFF,VerInfo.dwFileVersionLS>>16,VerInfo.dwFileVersionLS&0x0000FFFF);
					}
				if (!bRet)
					{
					ShowLastError(L"File Version");
					s=L"";
					}
				delete[] pcVer;
				}
			}
		return s;
		}
}

String sGetExeDescr()
{
	DWORD dwHandle;
	TCHAR szDescr[256];
	szDescr[0]=0;
	String s=ParamStr(0);
	DWORD dwSize=::GetFileVersionInfoSize(s.c_str(),&dwHandle);
	if (dwSize>0)
		{
		TCHAR* pcVer=new TCHAR[dwSize];
		void* pVer;
		UINT uLen;
		bool bRet=::GetFileVersionInfo(s.c_str(),dwHandle,dwSize,pcVer);
		if (bRet)
			bRet=::VerQueryValue(pcVer,L"\\StringFileInfo\\1C0904E4\\FileDescription",&pVer,&uLen);
		if (bRet)
			wcsncpy(szDescr,(TCHAR*)pVer,255);
		}
   return String(szDescr);
}

String sGetExeDir()
{
	String s=Application->ExeName;
	int n=s.Pos(L"\\.\\");
	if (n>0)
		s=s.Delete(n+1,2);
	s=sFileDirOnly(s);
	return s;
}

String sGetIniName()
{
   return ChangeFileExt(Application->ExeName,L".ini");
}

String sGetFullPath(const String sFile, const String sDefaultDir)
{
   if (ExtractFileDir(sFile).IsEmpty())
      {
      if (sDefaultDir.IsEmpty())
         return sGetDefaultDir()+sFile;
      else
         return IncludeTrailingBackslash(sDefaultDir)+sFile;
      }
   else
      return sFile;
}

void UpdateSystemTime(const WORD uDay, const WORD uMonth, const WORD uYear, const int nMilliSec)
{
   SYSTEMTIME Time;
   memset(&Time,0,sizeof(SYSTEMTIME));
   int nTime=nMilliSec;
   Time.wMilliseconds=(WORD)(nTime%1000);
   nTime/=1000;     //sec
   Time.wHour=(WORD)(nTime/3600);
   Time.wMinute=(WORD)((nTime%3600)/60);
   Time.wSecond=(WORD)(nTime%60);
   Time.wDay=uDay;
	Time.wMonth=uMonth;
   Time.wYear=uYear;
   bUpdateSystemTime(Time,true);
}

bool bUpdateSystemTime(const SYSTEMTIME& DT, const bool bVerbose)
{
   OSVERSIONINFO VerInfo;
   VerInfo.dwOSVersionInfoSize=sizeof(VerInfo);
   GetVersionEx(&VerInfo);
   bool bWinNT=(VerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT);
   bool bOK=true;
   if (bWinNT)
      {
      //Windows NT - Before setting time, create the privilege to do so
      HANDLE hToken;
      //Get a token to be able to adjust privileges
      bOK=OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
      if (bOK)
         {
         LUID_AND_ATTRIBUTES Attr;
         //Get the LUID for time adjustment
         bOK=LookupPrivilegeValue(NULL,SE_SYSTEMTIME_NAME,&Attr.Luid);
         if (bOK)
            {
            Attr.Attributes=SE_PRIVILEGE_ENABLED;
            TOKEN_PRIVILEGES TokenPriv;
            TokenPriv.PrivilegeCount=1;
            TokenPriv.Privileges[0]=Attr;
            //Set privilege to be able to adjust time
            bOK=AdjustTokenPrivileges(hToken,false,&TokenPriv,NULL,NULL,NULL);
            if (bOK)
               bOK=SetLocalTime(&DT);
            //Disable privilege again
            AdjustTokenPrivileges(hToken,true,&TokenPriv,NULL,NULL,NULL);
            }
         }
      if ((!bOK)&&(bVerbose))
			nShowError("System Time Set Failed","Access Denied");
		}
	else
		{
		//Windows 95 - no security
		if (!SetLocalTime(&DT))
			{
			bOK=false;
			if (bVerbose)
				nShowError("System Time Set Failed","Invalid Time");
         }
      }
   return bOK;
}

String sEncryptString(const String sDe)
{
   static const String sMod=L"BrakanJan";
   String sEn;
   sEn.Unique();
   sEn=sDe;
   for (int i=1; i<=sDe.Length(); i++)
      {
      int n=((i-1)%sMod.Length())+1;
      BYTE b=sDe[i]&0x7F;
      if (b<32) b=32;
      b-=32;
      b^=sMod[n];
      b&=0x7F;
      b+=33;
      sEn[i]=b;
      }
   return sEn;
}

String sDecryptString(const String sEn)
{
   static const String sMod=L"BrakanJan";
   String sDe;
   sDe.Unique();
   sDe=sEn;
   for (int i=1; i<=sEn.Length(); i++)
      {
      int n=((i-1)%sMod.Length())+1;
      BYTE b=sEn[i];
      b-=33;
      b^=sMod[n];
      b+=32;
      sDe[i]=b;
      }
   return sDe;
}

String sCreateDCSFile(const String sBase, const String sExt, int& nID)
{
   JFile::FILE_ERROR Error;
   String sFileExt;
   if (sExt[1]=='.')
      sFileExt=sExt.SubString(2,sExt.Length()-1);
   else
      sFileExt=sExt;
   String sFileBase=sFileNameOnly(sBase);
   if (sFileBase.IsEmpty()) sFileBase=L"NoName";
   sFileBase=sGetExeDir()+sFileNewExt(sFileBase,L"");
   String sIndexName=sFileBase+"_"+sFileExt;
   sIndexName=sFileNewExt(sIndexName,L".Index");
   DWORD dwIndex;
   if (bFileExist(sIndexName))
      {
      JFile Fil('I');
      Error=Fil.Open(sIndexName);
      if (!Error) Error=Fil.Read(&dwIndex,sizeof(dwIndex));
      if (Error) dwIndex=0;
      }
   else
      dwIndex=0;
   int n=0;
   String sFileName;
	bool bUnique;
	String sV;
	do
		{
		dwIndex++;
		sV.printf(L" #%05u",dwIndex);
		sFileName=sFileBase+sV;
		sFileName=sFileNewExt(sFileName,L"."+sFileExt);
		bUnique=!bFileExist(sFileName);
      n++;
      }
   while((!bUnique)&&(n<100));
   if (bUnique)
      {
      JFile Fil('O');
      Error=Fil.Create(sIndexName);
      if (!Error) Fil.Write(&dwIndex,sizeof(dwIndex));
      nID=dwIndex;
      }
   else
      {
      //Use date/time for file name
      SYSTEMTIME T;
		::GetLocalTime(&T);
		String sD;
		sD.printf(L"%02u%02u%04u_%02u%02u%02u.",T.wDay,T.wMonth,T.wYear,T.wHour,T.wMinute,T.wSecond);
		sFileName=sGetExeDir()+sD+sFileExt;
		nID=-1;
      }
   return sFileName;
}

String sCreateDCSFile(const String sBase, const String sExt)
{
   int nID;
   return sCreateDCSFile(sBase,sExt,nID);
}

int nShowError(const String sMessage, const String sHeading, const int nTimeout_sec)
{
   JInfoForm=new TJInfoForm(0);
   JInfoForm->SetInfo(sMessage,sHeading,nTimeout_sec);
   JInfoForm->InfoLab->Font->Color=clRed;
	int nRet=JInfoForm->ShowModal();
	delete JInfoForm;
	return nRet;
}

int nShowInfo(const String sMessage, const String sHeading, const int nTimeout_sec)
{
	JInfoForm=new TJInfoForm(0);
	JInfoForm->SetInfo(sMessage,sHeading,nTimeout_sec);
	int nRet=JInfoForm->ShowModal();
	delete JInfoForm;
   return nRet;
}

bool bAskYes(const String sMessage, const String sHeading, const int nTimeout_sec, const bool bOnTop, const bool bDefaultYes)
{
   JInfoForm=new TJInfoForm(0);
   JInfoForm->SetInfo(sMessage,sHeading,nTimeout_sec);
   if (bOnTop)
      {
		JInfoForm->FormStyle=fsStayOnTop;
		JInfoForm->BringToFront();
      }
   JInfoForm->SetAsk(true,bDefaultYes);
   int nRet=JInfoForm->ShowModal();
   delete JInfoForm;
   return (nRet==mrYes);
}


bool bGetFileTime(const String sName, int& nDate)
{
   nDate=-1;
   int nHandle=::FileOpen(sName,fmOpenRead);
   if (nHandle!=-1)
      {
      nDate=::FileGetDate(nHandle);
      ::FileClose(nHandle);
      }
   return (nDate!=-1);
}

bool bSetFileTime(const String sName, const int nDate)
{
	int nHandle=::FileOpen(sName,fmOpenWrite);
	if (nHandle!=-1)
		{
		int n=::FileSetDate(nHandle,nDate);
		::FileClose(nHandle);
		if (n==-1) nHandle=-1;
		}
	return (nHandle!=-1);
}

//0  = 95/98/Me
//40 = NT4
//50 = W2K
//51 = XP
//52 = Server 2003
//60 = Vista
//61 = Win7

int nGetWinVersion()
{
	OSVERSIONINFO VerInfo;
	VerInfo.dwOSVersionInfoSize=sizeof(VerInfo);
	::GetVersionEx(&VerInfo);
	if (VerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT)
		if (VerInfo.dwMajorVersion>=4)
			return (int)(VerInfo.dwMajorVersion*10+VerInfo.dwMinorVersion);
		else
			return 0;
	else
		return 0;
}

int nGetWinVersionBits()	//32 or 64
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	BOOL bIsWow64=FALSE; // assume 32 bit
	// can't call IsWow64Process on x32, so first look up the entry point in kernel32
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle (L"kernel32"),"IsWow64Process");
	// if we have an entry point for IsWow64Process, we can call it
	if (fnIsWow64Process)
		{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
			{
				// handle error
			}
		}
	if (bIsWow64)
		return 64;
	else
		return 32;
}


String sGetOSVersion()
{
	OSVERSIONINFO VerInfo;
	VerInfo.dwOSVersionInfoSize=sizeof(VerInfo);
	::GetVersionEx(&VerInfo);
	String s;
	switch(VerInfo.dwMajorVersion)
		{
		case 3:
			s="Windows NT 3."+String(VerInfo.dwMinorVersion);
			break;
		case 4:
			switch(VerInfo.dwMinorVersion)
				{
				case 0:
					if (VerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT)
						s="Windows NT 4";
					else
						s="Windows 95";
					break;
				case 10:
					s="Windows 98";
					break;
				case 90:
					s="Windows Me";
					break;
				default:
					s="Unknown Version";
				}
			if (VerInfo.szCSDVersion[0])
				s=s+" "+String(VerInfo.szCSDVersion);
			break;
		case 5:
			switch(VerInfo.dwMinorVersion)
				{
				case 0:
					s="Windows 2000";
					break;
				case 1:
					s="Windows XP";
					break;
				case 2:
					s="Windows Server 2003";
					break;
				default:
					s="Unknown Version";
				}
			if (VerInfo.szCSDVersion[0])
				s=s+" "+String(VerInfo.szCSDVersion);
			break;
		case 6:
			switch(VerInfo.dwMinorVersion)
				{
				case 0:
					s="Windows Vista";
					break;
				case 1:
					s="Windows 7";
					break;
				case 2:
					s="Windows 10";
					break;
				default:
					s="Version > Windows 10";
				}
			if (VerInfo.szCSDVersion[0])
				s=s+" "+String(VerInfo.szCSDVersion);
			break;
      case 10:
      	s="Windows 10";
         break;
		default:
			s="Unknown Version";
		}
	return s+" - "+String(nGetWinVersionBits())+" bit";
}


int __stdcall SelectFolder_BrowseProc(HWND hwnd,UINT uMsg,LPARAM,LPARAM)
{
	 wchar_t szDir[MAX_PATH];
	 switch(uMsg)
		{
		  case BFFM_INITIALIZED:
				SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)g_sSelectFolder_Default.c_str());
				break;
		  case BFFM_SELCHANGED:
				break;
		 default:;
		 }
	 return 0;
}

String sSelectFolder(TForm* pForm, const String sDefault, const String sHeading)
{
	LPITEMIDLIST pidlRoot=NULL;
	::BROWSEINFO bi={0};
	LPMALLOC pMalloc=NULL;
	wchar_t szName[1024];
	::SHGetMalloc(&pMalloc);
	if (pForm)
		bi.hwndOwner=pForm->Handle;
	else
		bi.hwndOwner=NULL;
	bi.pidlRoot=pidlRoot;
	bi.pszDisplayName=szName;
	if (sHeading.IsEmpty())
		bi.lpszTitle=L"Select a folder";
	else
		bi.lpszTitle=sHeading.c_str();
	bi.ulFlags=BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.lpfn=NULL;
	if (!sDefault.IsEmpty())
		{
		if (bDirExist(sDefault))
			{
			g_sSelectFolder_Default=sDefault;
			bi.lpfn=SelectFolder_BrowseProc; // callback function
			}
		}
	bi.lParam=0;
	LPITEMIDLIST pidlSelected=SHBrowseForFolder(&bi);
	if(pidlRoot)
		pMalloc->Free(pidlRoot);
	pMalloc->Release();
	if (pidlSelected)
		SHGetPathFromIDList(pidlSelected,szName);
	else
		szName[0]=0;
	return String(szName);
}


String sGetWinError(const DWORD dwError)
{
	String s;
	LPTSTR pMsg;
	if (::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								dwError,
								0,
								(LPTSTR)&pMsg,
								0,
								NULL))
		{
		s=String(pMsg);
      ::LocalFree(pMsg);
      }
   else
      s=L"Unknown Error";
   return s;
}

void ShowLastError(const String sCaption)
{
   LPTSTR pMsg;
   DWORD dwError=::GetLastError();
   if (::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL,
                        dwError,
                        0,
                        (LPTSTR)&pMsg,
                        0,
                        NULL))
      {
      if (sCaption.IsEmpty())
         ShowError(pMsg,L"Last Error");
      else
         ShowInfo(pMsg,sCaption.c_str());
      ::LocalFree(pMsg);
      }
   else
      ShowError(L"Unknown Error",sCaption.c_str());
}

bool bWipeAndDeleteFile(const String sFile, const bool bVerbose)
{
   JFile Fil('B');
   JFile::FILE_ERROR E=Fil.Open(sFile);
   if (!E)
      {
      DWORD dwSize=Fil.dwGetSize();
      if (dwSize>0)
			{
			BYTE* pucBuf=new BYTE[dwSize];
			memset(pucBuf,0xFF,dwSize);	//Write FFs to everything
         E=Fil.Write(0,pucBuf,dwSize);
         delete[] pucBuf;
         }
		}
	if (!E)
		E=Fil.Close();
	if (bVerbose)
		Fil.ShowError(E);
	if (!E)
		{
		if (::DeleteFile(sFile.c_str()))
			return true;
		else if (bVerbose)
			ShowLastError(L"Error Deleting File");
		}
	return false;
}

bool bCreateDir(const String sDir, const bool bVerbose)
{
   bool bOK=true;
   if (!bDirExist(sDir))
      {
      if (!::CreateDirectory(sDir.c_str(),NULL))
         {
			//Could not create directory
			bOK=false;
         int nSlash=0;
         int anSlash[100];
         bool bNetPath=false;
         if (sDir.Length()>2)
            {
            if ((sDir[1]=='\\')&&(sDir[2]=='\\'))
               bNetPath=true;
            }
         for (int i=1; i<=sDir.Length(); i++)
            {
            if (sDir.IsDelimiter(L"\\",i))
               {
               anSlash[nSlash]=i;
               nSlash++;
               }
            }
         String sRoot;
         int nStart;
         if (bNetPath)
            nStart=4;
         else
            nStart=1;
         for (int i=nStart; i<nSlash; i++)
            {
            sRoot=sDir.SubString(1,anSlash[i]);
            if (!bDirExist(sRoot))
               {
               if (!::CreateDirectory(sRoot.c_str(),NULL))
                  {
                  if (bVerbose)
                     ShowLastError(sRoot);
                  bOK=false;
						}
					else
						bOK=true;
					}
				}
			}
		}
	return bOK;
}

//*****************************************************************************************

double dAltToDouble(String sVal, const double dDefault)
{
	double dVal=dDefault;
//	bool bOK=false;
	if (!sVal.IsEmpty())
		{
		AnsiString as=sVal;
		int n=sscanf(as.c_str(),"%lf",&dVal);
		if (n!=1)
			dVal=dDefault;
		}
	return dVal;

}

//DECIMAL_SEPARATOR g_DECIMAL_SEPARATOR=DECIMAL_POINT;   //Always start with decimal point
//
//double dAltToDouble(String sVal, const double dDefault)
//{
//	double dVal;
//	bool bOK=false;
//	if (!sVal.IsEmpty())
//		{
//		bOK=true;
//		if (g_DECIMAL_SEPARATOR==DECIMAL_COMMA)
//			{
//			for (int i=1; i<=sVal.Length(); i++)
//				if (sVal[i]=='.')
//					sVal[i]=',';
//			}
//		try
//			{
//			dVal=sVal.ToDouble();
//			}
//		catch(...)
//			{
//			bOK=false;
//			if (g_DECIMAL_SEPARATOR==DECIMAL_COMMA)
//				g_DECIMAL_SEPARATOR=DECIMAL_POINT;
//			else
//				{
//				g_DECIMAL_SEPARATOR=DECIMAL_COMMA;
//				for (int i=1; i<=sVal.Length(); i++)
//				if (sVal[i]=='.')
//					sVal[i]=',';
//				bOK=true;
//				try
//					{
//					dVal=sVal.ToDouble();
//					}
//				catch(...)
//					{
//					bOK=false;
//					}
//				}
//			}
//		}
//	if (!bOK)
//		dVal=dDefault;
//return dVal;
//
//}

//*****************************************************************************************



