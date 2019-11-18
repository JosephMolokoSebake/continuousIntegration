//---------------------------------------------------------------------------

#ifndef JCommaTextH
#define JCommaTextH
//---------------------------------------------------------------------------

#include "JTime.h"

  class CommaText
{

   TStringList* pList;

public:

   CommaText(const String sText, const bool bInsertCommas=false, const char cDelimiter=0);
   CommaText();
   ~CommaText();

   void SetText(const String sText, const bool bInsertCommas=false, const char cDelimiter=0);

   int nGetNum();

   int nGetCount()
   {
		return nGetNum();
   }

	String sGetPar(const int n);
   int nGetVal(const int n);
   DWORD dwGetVal(const int n);
   double dGetVal(const int n);

//NMEA 0183 extraction
   double dGetLat(const int nPar1, const int nPar2);
   double dGetLong(const int nPar1, const int nPar2);
	int nGetTime_ms(const int nPar, const bool bTranslate2SAST=false);
	int nGetTime_sec(const int nPar);

	JTime GetDateTime(const int nTimePar, const int nDatePar=-1, const bool bTranslate2SAST=false);
	JTime GetZDA_utc();

};

#endif
