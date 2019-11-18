//---------------------------------------------------------------------------

#ifndef JIniFileH
#define JIniFileH
//---------------------------------------------------------------------------

#include <System.IniFiles.hpp>

#include "JTransform.h"
#include "JTime.h"
#include "JAES.h"
#include "JEdit.h"

class JIniFile
{

	String sIniFileName;
	TMemIniFile* pIni;
	String sSection;
	String sDecName;
	String sEncName;
	bool bEx;
	bool bReadOnly;

   JAES* pAES;

   void Create(const String sFName, const bool bWipeFile, const bool bEncrypt, const bool _bReadOnly);


public:

	enum INIFILE_TYPE
	{
		INI_RW,
		INI_READ,
		INI_WRITE,
		INI_RW_ENCR,
		INI_READ_ENCR,
		INI_WRITE_ENCR,
	};


	JIniFile(const String sFName="", const bool bWipeFile=false, const bool bEncrypt=false, const bool _bReadOnly=false);

	JIniFile(const bool bStore, const String sFName="");

	JIniFile(const String sFName, const INIFILE_TYPE IniType=JIniFile::INI_RW);

   ~JIniFile();

   void CloseFile();

   bool bExist() const
   {
      return bEx;
   }

   void SetSection(const String sSect);

//int
   void Write(const String sSect, const String sID, const int nVal);
   void Write(const String sID, const int nVal);
   void Read(const String sSect, const String sID, int& nVal, const int nDefault=0);
   void Read(const String sID, int& nVal, const int nDefault=0);
   int nRead(const String sSect, const String sID, const int nDefault=0);
   int nRead(const String sID, const int nDefault=0);
//word
   void Write(const String sSect, const String sID, const WORD uVal);
   void Write(const String sID, const WORD uVal);
   WORD uRead(const String sSect, const String sID, const WORD uDefault=0);
	WORD uRead(const String sID, const WORD uDefault=0);
//DWORD (store as hex value)
	void WriteDW_H(const String sSect, const String sID, const DWORD dwVal);
	void WriteDW_H(const String sID, const DWORD dwVal);
	DWORD dwRead_H(const String sSect, const String sID, const DWORD dwDefault=0);
	DWORD dwRead_H(const String sID, const DWORD dwDefault=0);
//double
   void Write(const String sSect, const String sID, const double dVal);
   void Write(const String sID, const double dVal);
   void Read(const String sSect, const String sID, double& dVal, const double dDefault=0.0);
   void Read(const String sID, double& dVal, const double dDefault=0.0);
   double dRead(const String sSect, const String sID, const double dDefault=0.0);
   double dRead(const String sID, const double dDefault=0.0);
//float
   void Write(const String sSect, const String sID, const float fVal);
   void Write(const String sID, const float fVal);
   void Read(const String sSect, const String sID, float& fVal, const float fDefault=0.0);
   void Read(const String sID, float& fVal, const float fDefault=0.0);
   float fRead(const String sSect, const String sID, const float fDefault=0.0);
   float fRead(const String sID, const float fDefault=0.0);
//bool
   void Write(const String sSect, const String sID, const bool bVal);
   void Write(const String sID, const bool bVal);
   void Read(const String sSect, const String sID, bool& bVal, const bool bDefault);
   void Read(const String sID, bool& bVal, const bool bDefault);
//TColor
   void Write(const String sSect, const String sID, const TColor& Color);
   void Write(const String sID, const TColor& Color);
   void Read(const String sSect, const String sID, TColor& Color, const TColor& DefColor=clBlack);
   void Read(const String sID, TColor& Color, const TColor& DefColor=clBlack);
//String

	void SetEncrypt(const bool bEncrypt);

	void Write(const String sSect, const String sID, const String sVal);
	void Write(const String sID, const String sVal);
	void Write(const String sID, const char* pszVal);
	void Read(const String sSect, const String sID, String& sVal, const String sDefault="");
	void Read(const String sID, String& sVal, const String sDefault="");
	String sRead(const String sSect, const String sID, const String sDefault="");
	String sRead(const String sID, const String sDefault="");
//SYSTEMTIME
	void Write(const String sSect, const String sID, const SYSTEMTIME& T);
   void Write(const String sID, const SYSTEMTIME& T);
   void Read(const String sSect, const String sID, SYSTEMTIME& T);
   void Read(const String sID, SYSTEMTIME& T);
//JTime
   void Write(const String sSect, const String sID, const JTime& T);
   void Write(const String sID, const JTime& T);
   void Read(const String sSect, const String sID, JTime& T);
   void Read(const String sID, JTime& T);
//Font
   void Write(const String sSect, const String sID, TFont* pFont);
   void Write(const String sID, TFont* pFont);
   void Read(const String sSect, const String sID, TFont* pFont, const TColor DefaultCol=clLtGray);
   void Read(const String sID, TFont* pFont, const TColor DefaultCol=clLtGray);
   bool bRead(const String sSect, const String sID, TFont* pFont, const TColor DefaultCol=clLtGray);
   bool bRead(const String sID, TFont* pFont, const TColor DefaultCol=clLtGray);
//Lat Long
   void Write(const String sSect, const String sID, const LatLong& LL);
   void Write(const String sID, const LatLong& LL);
   void Read(const String sSect, const String sID, LatLong& LL, const LatLong& DefLL);
   void Read(const String sID, LatLong& LL, const LatLong& DefLL);
	void Read(const String sSect, const String sID, LatLong& LL);
	void Read(const String sID, LatLong& LL);

	double dAltRead(const String sSect, const String sID, const double dDefault);
	double dAltRead(const String sID, const double dDefault);

   String sAltConvert(const double d, const int nDeci);

   void CheckBlank(double& dV)
   {
      if (fabs(-1.0-dV)<1e-6)
         {
	      JEdit Ed;
         dV=Ed.dGetInvalid();
         }
   }

   void CheckBlank(int& nV)
   {
      if (nV==-1)
         {
	      JEdit Ed;
         nV=Ed.nGetInvalid();
         }
   }



};



#endif
