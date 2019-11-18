#ifndef JUtilsH
#define JUtilsH

#include "JFile.h"

//Non-modal dialog responses
enum NM_DIALOG_STATE
{
   NM_DIALOG_OPEN,
   NM_DIALOG_OK,
	NM_DIALOG_CANCEL
};


//*************  FILE SYSTEM  *************

String sFileNameOnly(const String sName, const bool bExt=true);

String sFileExtOnly(const String sName, const bool bIncludePeriod=false);
bool bFileExtMatch(const String sFile, const String sExt);

String sFileDirOnly(const String sName);
String sFileDriveOnly(const String sName);
String sStripDrive(const String sFolder);


String sFileNewExt(const String sName, const String sExt);

String sValidateFileName(const String sName, const char cReplacement='_');

String sGetVolumeDrive(const String sVolumeName);
String sGetDriveVolume(const String sDrive);

bool bFileExist(const String sName);

DWORD dwGetFileSize(const String sName);

bool bDirExist(const String sName);

String sGetDefaultDir();
String sGetRootDir(const String sDir);

bool bMakeDir(const String sDir);

JFile::FILE_ERROR EraseFile(const String sName);

DWORD dwGetDiskFree_MB(const String sDir="");         //in megabytes
DWORD dwGetTotalDiskSpace_MB(const String sDir="");   //in megabytes

String sCreateDCSFile(const String sBase, const String sExt, int& nID);
String sCreateDCSFile(const String sBase, const String sExt);

String sGetIniName();

bool bGetFileTime(const String sName, int& nDate);
bool bSetFileTime(const String sName, const int nDate);

bool bWipeAndDeleteFile(const String sFile, const bool bVerbose=true);
bool bCreateDir(const String sDir, const bool bVerbose=true);    //any depth

String sSelectFolder(TForm* pForm=NULL, const String sDefault="", const String sHeading="");
extern String g_sSelectFolder_Default;


//*************  SYSTEM  *************

double dGetMemTotal_MB();
double dGetMemAvail_MB();

DWORD dwGetExeVersion(const bool bNewStyle=false);
DWORD dwGetExeVersion(const String sFileName,const bool bNewStyle);
String sGetExeVersion(const bool bNewStyle=false, const bool bHex=false);
int nGetVersionPart(const int n, const DWORD dwVer);
String sGetExeDescr();

String sGetExeDir();


String sEncryptString(const String sDe);
String sDecryptString(const String sEn);

String sGetFullPath(const String sFile, const String sDefaultDir=L"");


void UpdateSystemTime(const WORD uDay, const WORD uMonth, const WORD uYear, const int nMilliSec);

bool bUpdateSystemTime(const SYSTEMTIME& DT, const bool bVerbose);

int nGetWinVersion();   //0 = 95/98/Me, 40 = NT4, 50 = W2K, 51 = XP, 52 = .NET
String sGetOSVersion();
int nGetWinVersionBits();	//32 or 64

enum DECIMAL_SEPARATOR
{
	 DECIMAL_POINT,
    DECIMAL_COMMA,
};

extern DECIMAL_SEPARATOR g_DECIMAL_SEPARATOR;

double dAltToDouble(String sVal, const double dDefault=0.0);


//*************  MESSAGES  *************

int nShowError(const String sMessage, const String sHeading="Error", const int nTimeout_sec=-1);
int nShowInfo(const String sMessage, const String sHeading="Information", const int nTimeout_sec=-1);
bool bAskYes(const String sMessage, const String sHeading="Confirm", const int nTimeout_sec=-1, const bool bOnTop=false, const bool bDefaultYes=false);

void ShowError(const String sMessage, const String sHeading="Error")    //For Compatibility
{
   nShowError(sMessage,sHeading);
}

void ShowInfo(const String sMessage, const String sHeading="Information")     //For Compatibility
{
   nShowInfo(sMessage,sHeading);
}

void ShowLastError(const String sCaption="");

String sGetWinError(const DWORD dwError);




#endif
