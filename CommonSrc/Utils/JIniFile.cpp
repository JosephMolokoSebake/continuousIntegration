/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General Ini File Module
 *    FILE NAME		JIniFile.cpp
 * 	COMPILER       Borland C++ Builder 5 (SP1)
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000 Professional (SP2)
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *    	SANDF Availability Model - Main Form
 *		CHANGE HISTORY
 *			V3.00		Initial Version	JGT 31/10/2001
 ************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JIniFile.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JIniFile::JIniFile(const String sFName, const bool bWipeFile, const bool bEncrypt, const bool _bReadOnly)
{
	pAES=NULL;
	Create(sFName,bWipeFile,bEncrypt,_bReadOnly);
}

JIniFile::JIniFile(const bool bStore, const String sFName)
{
	pAES=NULL;
	if (bStore)
		Create(sFName,true,false,false);  //store
	else
		Create(sFName,false,false,true);  //read
}

JIniFile::JIniFile(const String sFName, const INIFILE_TYPE IniType)
{
	pAES=NULL;
	switch(IniType)
		{
		case INI_READ:
			Create(sFName,false,false,true);  //read
			break;
		case INI_WRITE:
			Create(sFName,true,false,false);  //store
			break;
		case INI_RW_ENCR:
			Create(sFName,false,true,false);  //read/store
			break;
		case INI_READ_ENCR:
			Create(sFName,false,true,true);  //read
			break;
		case INI_WRITE_ENCR:
			Create(sFName,true,true,false);  //store
			break;
		default:
			Create(sFName,false,false,false);  //read/store
		}
}

void JIniFile::Create(const String sFName, const bool bWipeFile, const bool bEncrypt, const bool _bReadOnly)
{
	bReadOnly=_bReadOnly;
   String sName;
   if (sFName.IsEmpty())
		sName=ChangeFileExt(Application->ExeName,".ini");
	else
		sName=sFName;
   bEx=bFileExist(sName);
   if (!bEx) bReadOnly=false;
   if ((bWipeFile)&&(bEx))
      DeleteFile(sName);
   sEncName=sName;
   if (bEncrypt)
      {
		sDecName=sFileNewExt(sName,".#dec");
      if (bEx)
         {
         JFile EncFil('I');
         JFile::FILE_ERROR E=EncFil.Open(sEncName);
         DWORD dwSize=EncFil.dwGetSize();
         BYTE* pucEnc=NULL;
         BYTE* pucDec=NULL;
         if ((!E)&&(dwSize>0))
            {
            pucEnc=new BYTE[dwSize];
				pucDec=new BYTE[dwSize];
				E=EncFil.Read(pucEnc,dwSize);
				}
			enum
			{
				NO_ENCR,
				OLD_ENCR,
				AES_ENCR,
			} EncryptMode;
			EncryptMode=OLD_ENCR;
			if (!E)
				{
				for (int i=0; (i<(int)dwSize-7)&&(EncryptMode!=NO_ENCR); i++)
					{
					if (toupper(pucEnc[i])=='V')
						{
						if (strncmpi((char*)&pucEnc[i],"VERSION",7)==0)
							EncryptMode=NO_ENCR;
						}
					}
				if (EncryptMode!=NO_ENCR)
					{
					if (dwSize>=8)
						{
						char szID[9];
						memcpy(szID,pucEnc,8);
						szID[8]=0;
						if (String(szID)=="AES_ENCR")
							EncryptMode=AES_ENCR;
						}
					}
				}
			if (!E)
				{
				if (EncryptMode==NO_ENCR)
					{
					//The ini file was not encrypted - use as is
					memcpy(pucDec,pucEnc,dwSize);
					}
				else if (EncryptMode==AES_ENCR)
					{
					JAES AES("hgr3802pdth7wPjDG3b,=@v&3$");
					int nDecSize=AES.nDecrypt(&pucEnc[8],pucDec,dwSize-8);
					if (nDecSize<=0)
						E=JFile::F_FORMAT_ERROR;
					}
				else
					{
               ShowInfo("File: "+sName,"Cannot Decrypt File (Use older version of software)");
/*					//Decode ini file
					Encryption Dec("hgr3802pdth7wPjDG3b,=@v&3$");
					Dec.Decrypt(pucEnc,pucDec,dwSize);
*/
					}
				JFile DecFil('O');
				if (!E)
					E=DecFil.Create(sDecName);
				if (!E)
					DecFil.Write(pucDec,dwSize);
				}
			delete[] pucDec;
			delete[] pucEnc;
			}
      sName=sDecName;
      }
   else
      sDecName=L"";
   String s;
   try
      {
		s=ExpandFileName(sName);
		pIni=new TMemIniFile(s);
		sIniFileName=s;
		}
	catch(...)
		{
      nShowError("Error Creating "+s,"INI File Creation Error");
      }
   sSection="";
}

JIniFile::~JIniFile()
{
	CloseFile();
	if (!sDecName.IsEmpty())
		{
		if (!bReadOnly)
			{
			JFile DecFil('I');
			JFile::FILE_ERROR E=DecFil.Open(sDecName);
			DWORD dwSize=DecFil.dwGetSize();
			BYTE* pucEnc=NULL;
			BYTE* pucDec=NULL;
			if ((!E)&&(dwSize>0))
				{
				pucEnc=new BYTE[dwSize+31];
				pucDec=new BYTE[dwSize];
				E=DecFil.Read(pucDec,dwSize);
				}
			if (!E)
				{
				JAES AES("hgr3802pdth7wPjDG3b,=@v&3$");
				int nEncSize=AES.nEncrypt(pucDec,pucEnc,dwSize);
				JFile EncFil('O');
				E=EncFil.Create(sEncName);
				char szID[9]="AES_ENCR";
				if (!E) EncFil.Write(szID,8);
				if (!E) EncFil.Write(pucEnc,nEncSize);
	/*
				Encryption Enc(L"hgr3802pdth7wPjDG3b,=@v&3$");
				Enc.Encrypt(pucDec,pucEnc,dwSize);
				JFile EncFil('O');
				E=EncFil.Create(sEncName);
				if (!E) EncFil.Write(pucEnc,dwSize);
	*/
				}
			delete[] pucDec;
			delete[] pucEnc;
			DecFil.Close();
			}
		if (bFileExist(sDecName))
			bWipeAndDeleteFile(sDecName);
		}
	delete pAES;
}

void JIniFile::CloseFile()
{
	if (pIni)
		{
		try
			{
         if (!bReadOnly)
            pIni->UpdateFile();
         delete pIni;
         pIni=NULL;
         }
      catch(...)
         {
         nShowError(L"Error Updating "+sEncName,L"INI File Update Error");
         }
      }
}

void JIniFile::SetSection(const String sSect)
{
   sSection=sSect;
}

//int
void JIniFile::Write(const String sSect, const String sID, const int nVal)
{
   sSection=sSect;
   Write(sID,nVal);
}

void JIniFile::Write(const String sID, const int nVal)
{
   pIni->WriteInteger(sSection,sID,nVal);
}

void JIniFile::Read(const String sSect, const String sID, int& nVal, const int nDefault)
{
   sSection=sSect;
   Read(sID,nVal,nDefault);
}

void JIniFile::Read(const String sID, int& nVal, const int nDefault)
{
   nVal=pIni->ReadInteger(sSection,sID,nDefault);
}

int JIniFile::nRead(const String sSect, const String sID, const int nDefault)
{
   sSection=sSect;
   return nRead(sID,nDefault);
}

int JIniFile::nRead(const String sID, const int nDefault)
{
   return pIni->ReadInteger(sSection,sID,nDefault);
}

void JIniFile::Write(const String sSect, const String sID, const WORD uVal)
{
   sSection=sSect;
   Write(sID,uVal);
}

void JIniFile::Write(const String sID, const WORD uVal)
{
   pIni->WriteInteger(sSection,sID,uVal);
}

WORD JIniFile::uRead(const String sSect, const String sID, const WORD uDefault)
{
	sSection=sSect;
	return uRead(sID,uDefault);
}

WORD JIniFile::uRead(const String sID, const WORD uDefault)
{
	return (WORD)pIni->ReadInteger(sSection,sID,uDefault);
}

//DWORD
void JIniFile::WriteDW_H(const String sSect, const String sID, const DWORD dwVal)
{
	String s;
	s.printf(L"0x%08X",dwVal);
	Write(sSect,sID,s);
}

void JIniFile::WriteDW_H(const String sID, const DWORD dwVal)
{
	WriteDW_H(sSection,sID,dwVal);
}

DWORD JIniFile::dwRead_H(const String sSect, const String sID, const DWORD dwDefault)
{
	String s;
	Read(sSect,sID,s,"");
	if (!s.IsEmpty())
		{
		bool bHex=false;
		DWORD dwVal;
		int n=s.Pos("0x");
		if (n==1)
			{
			s[1]=' ';
			s[2]=' ';
			s=s.Trim();
			bHex=true;
			}
		else if (s.Length()==8)
			bHex=true;	//legacy hex store (without 0x in front)
		if (bHex)
			n=swscanf(s.c_str(),L"%X",&dwVal);
		else
			n=swscanf(s.c_str(),L"%U",&dwVal);
		if (n==1)
			return dwVal;
		else
			return dwDefault;
		}
   return dwDefault;
}

DWORD JIniFile::dwRead_H(const String sID, const DWORD dwDefault)
{
   return dwRead_H(sSection,sID,dwDefault);
}

//double
void JIniFile::Write(const String sSect, const String sID, const double dVal)
{
   sSection=sSect;
   Write(sID,dVal);
}

void JIniFile::Write(const String sID, const double dVal)
{
   int nDeci;
   double dV=fabs(dVal);
   if (dV>1e9)
      nDeci=1;
   else if (dV>1e6)
      nDeci=2;
   else if (dV>1e3)
      nDeci=4;
   else if (dV>1.0)
      nDeci=8;
   else
      nDeci=12;
   String sV=sAltConvert(dVal,nDeci);
   Write(sID,sV);
//   pIni->WriteFloat(sSection,sID,dVal);
}

void JIniFile::Read(const String sSect, const String sID, double& dVal, const double dDefault)
{
   sSection=sSect;
   Read(sID,dVal,dDefault);
}

void JIniFile::Read(const String sID, double& dVal, const double dDefault)
{
	dVal=dAltRead(sSection,sID,dDefault);
//   dVal=pIni->ReadFloat(sSection,sID,dDefault);
}

double JIniFile::dRead(const String sSect, const String sID, const double dDefault)
{
   double dV;
	dV=dAltRead(sSect,sID,dDefault);
//	Read(sSect,sID,dV,dDefault);
	return dV;
}

double JIniFile::dRead(const String sID, const double dDefault)
{
   double dV;
	dV=dAltRead(sID,dDefault);
//	Read(sID,dV,dDefault);
   return dV;
}

//float
void JIniFile::Write(const String sSect, const String sID, const float fVal)
{
   int nDeci;
   double dV=fabs(fVal);
   if (dV>1e9)
      nDeci=1;
   else if (dV>1e6)
      nDeci=2;
   else if (dV>1e3)
      nDeci=4;
   else
      nDeci=8;
   String sV=sAltConvert(fVal,nDeci);
   sSection=sSect;
   Write(sID,sV);
}

void JIniFile::Write(const String sID, const float fVal)
{
   int nDeci;
   double dV=fabs(fVal);
   if (dV>1e9)
      nDeci=1;
   else if (dV>1e6)
      nDeci=2;
   else if (dV>1e3)
      nDeci=4;
   else
      nDeci=8;
   String sV=sAltConvert(fVal,nDeci);
   Write(sSection,sID,sV);
}

void JIniFile::Read(const String sSect, const String sID, float& fVal, const float fDefault)
{
   sSection=sSect;
   double dV;
   Read(sID,dV,fDefault);
   fVal=dV;
}

void JIniFile::Read(const String sID, float& fVal, const float fDefault)
{
   double dV;
   Read(sID,dV,fDefault);
   fVal=dV;
//   fVal=pIni->ReadFloat(sSection,sID,fDefault);
}

float JIniFile::fRead(const String sSect, const String sID, const float fDefault)
{
   float fV;
   Read(sSect,sID,fV,fDefault);
   return fV;
}

float JIniFile::fRead(const String sID, const float fDefault)
{
   float fV;
   Read(sID,fV,fDefault);
   return fV;
}


//bool
void JIniFile::Write(const String sSect, const String sID, const bool bVal)
{
   sSection=sSect;
   Write(sID,bVal);
}

void JIniFile::Write(const String sID, const bool bVal)
{
   pIni->WriteBool(sSection,sID,bVal);
}

void JIniFile::Read(const String sSect, const String sID, bool& bVal, const bool bDefault)
{
   sSection=sSect;
   Read(sID,bVal,bDefault);
}

void JIniFile::Read(const String sID, bool& bVal, const bool bDefault)
{
   bVal=pIni->ReadBool(sSection,sID,bDefault);
}

//TColor
void JIniFile::Write(const String sSect, const String sID, const TColor& Color)
{
   sSection=sSect;
   Write(sID,Color);
}

void JIniFile::Write(const String sID, const TColor& Color)
{
	pIni->WriteInteger(sSection,sID,Color);
}

void JIniFile::Read(const String sSect, const String sID, TColor& Color, const TColor& DefColor)
{
	sSection=sSect;
	Read(sID,Color,DefColor);
}

void JIniFile::Read(const String sID, TColor& Color, const TColor& DefColor)
{
	Color=(TColor)pIni->ReadInteger(sSection,sID,DefColor);
}

//String

void JIniFile::SetEncrypt(const bool bEncrypt)
{
	if (bEncrypt)
		{
		if (!pAES)
			pAES=new JAES("ConfigStrings");
		}
	else
		{
		delete pAES;
		pAES=NULL;
		}

}

void JIniFile::Write(const String sSect, const String sID, const String sVal)
{
	sSection=sSect;
	Write(sID,sVal);
}

void JIniFile::Write(const String sID, const String sVal)
{
	if (pAES)
		pIni->WriteString(sSection,sID,pAES->sEncrypt(sVal));
	else
		pIni->WriteString(sSection,sID,sVal);
}

void JIniFile::Write(const String sID, const char* pszVal)
{
	Write(sID,String(pszVal));
}

void JIniFile::Read(const String sSect, const String sID, String& sVal, const String sDefault)
{
	sSection=sSect;
	Read(sID,sVal,sDefault);
}

String JIniFile::sRead(const String sSect, const String sID, const String sDefault)
{
	String s;
	Read(sSect,sID,s,sDefault);
	return s;
}

String JIniFile::sRead(const String sID, const String sDefault)
{
	String s;
	Read(sID,s,sDefault);
	return s;
}

void JIniFile::Read(const String sID, String& sVal, const String sDefault)
{
	sVal=pIni->ReadString(sSection,sID,sDefault);
	if (pAES)
		sVal=pAES->sDecrypt(sVal);
}

void JIniFile::Write(const String sSect, const String sID, const SYSTEMTIME& T)
{
	sSection=sSect;
	Write(sID,T);
}

void JIniFile::Write(const String sID, const SYSTEMTIME& T)
{
	Write(sID+" Year",T.wYear);
	Write(sID+" Month",T.wMonth);
	Write(sID+" WeekDay",T.wDayOfWeek);
	Write(sID+" Day",T.wDay);
   Write(sID+" Hour",T.wHour);
   Write(sID+" Minute",T.wMinute);
   Write(sID+" Second",T.wSecond);
   Write(sID+" MilliSec",T.wMilliseconds);
}

void JIniFile::Read(const String sSect, const String sID, SYSTEMTIME& T)
{
   sSection=sSect;
   Read(sID,T);
}

void JIniFile::Read(const String sID, SYSTEMTIME& T)
{
   SYSTEMTIME Now;
   GetLocalTime(&Now);
   T.wYear=uRead(sID+" Year",Now.wYear);
   T.wMonth=uRead(sID+" Month",Now.wMonth);
   T.wDayOfWeek=uRead(sID+" WeekDay",Now.wDayOfWeek);
   T.wDay=uRead(sID+" Day",Now.wDay);
   T.wHour=uRead(sID+" Hour",Now.wHour);
   T.wMinute=uRead(sID+" Minute",Now.wMinute);
   T.wSecond=uRead(sID+" Second",Now.wSecond);
   T.wMilliseconds=uRead(sID+" MilliSec",Now.wMilliseconds);
}

void JIniFile::Write(const String sSect, const String sID, TFont* pFont)
{
   sSection=sSect;
   Write(sID,pFont);
}

void JIniFile::Write(const String sID, TFont* pFont)
{
   int nStyle=0;
   if (pFont->Style.Contains(fsBold)) nStyle|=0x01;
   if (pFont->Style.Contains(fsItalic)) nStyle|=0x02;
   if (pFont->Style.Contains(fsUnderline)) nStyle|=0x04;
   if (pFont->Style.Contains(fsStrikeOut)) nStyle|=0x08;
   Write(sID+" Name",pFont->Name);
   Write(sID+" Colour",pFont->Color);
   Write(sID+" Size",pFont->Size);
   Write(sID+" Style",nStyle);
}

bool JIniFile::bRead(const String sSect, const String sID, TFont* pFont, const TColor DefaultCol)
{
   sSection=sSect;
   return bRead(sID,pFont,DefaultCol);
}

bool JIniFile::bRead(const String sID, TFont* pFont, const TColor DefaultCol)
{
   int nStyle;
   String sName;
   Read(sID+" Name",sName,L"");
   bool bValid=!sName.IsEmpty();
   if (!bValid)
      sName=L"Arial";
   pFont->Name=sName;
   TColor Color;
   Read(sID+" Colour",Color,DefaultCol);
   pFont->Color=Color;
   pFont->Size=nRead(sID+" Size",10);
   Read(sID+" Style",nStyle,0);
   TFontStyles Style;
   if (nStyle&0x01)
      Style<<fsBold;
   if (nStyle&0x02)
      Style<<fsItalic;
   if (nStyle&0x04)
      Style<<fsUnderline;
   if (nStyle&0x08)
      Style<<fsStrikeOut;
   pFont->Style=Style;
   return bValid;
}

void JIniFile::Read(const String sSect, const String sID, TFont* pFont, const TColor DefaultCol)
{
   sSection=sSect;
   bRead(sID,pFont,DefaultCol);
}

void JIniFile::Read(const String sID, TFont* pFont, const TColor DefaultCol)
{
   bRead(sID,pFont,DefaultCol);
}

void JIniFile::Write(const String sSect, const String sID, const LatLong& LL)
{
   sSection=sSect;
   Write(sID,LL);
}

void JIniFile::Write(const String sID, const LatLong& LL)
{
   Write(String(sID+" Latitude").Trim(),LL.dLat);
   Write(String(sID+" Longitude").Trim(),LL.dLong);
}

void JIniFile::Read(const String sSect, const String sID, LatLong& LL, const LatLong& DefLL)
{
	sSection=sSect;
	LL.dLat=dAltRead(sID+" Latitude",DefLL.dLat);
	LL.dLong=dAltRead(sID+" Longitude",DefLL.dLong);
}

void JIniFile::Read(const String sID, LatLong& LL, const LatLong& DefLL)
{
   Read(sSection,sID,LL,DefLL);
}

void JIniFile::Read(const String sSect, const String sID, LatLong& LL)
{
   Read(sSect,sID,LL,LatLong(0.0,0.0));
}

void JIniFile::Read(const String sID, LatLong& LL)
{
   Read(sID,LL,LatLong(0.0,0.0));
}

//JTime
void JIniFile::Write(const String sSect, const String sID, const JTime& T)
{
   Write(sSect,sID,T.Get());
}

void JIniFile::Write(const String sID, const JTime& T)
{
	Write(sID,T.Get());
}

void JIniFile::Read(const String sSect, const String sID, JTime& T)
{
	SYSTEMTIME ST;
	Read(sSect,sID,ST);
	T=ST;
}

void JIniFile::Read(const String sID, JTime& T)
{
	SYSTEMTIME ST;
	Read(sID,ST);
	T=ST;
}

String JIniFile::sAltConvert(const double d, const int nDeci)
{
	String s,sV;
	sV.printf(L"%d.%df",nDeci,nDeci);
   	sV="%"+sV;
	s.printf(sV.c_str(),d);
	return s;

}

double JIniFile::dAltRead(const String sSect, const String sID, const double dDefault)
{
	String sVal;
	Read(sSect,sID,sVal,"");
	sVal=sVal.Trim();
return dAltToDouble(sVal,dDefault);
}

double JIniFile::dAltRead(const String sID, const double dDefault)
{
	return dAltRead(sSection,sID,dDefault);
}


