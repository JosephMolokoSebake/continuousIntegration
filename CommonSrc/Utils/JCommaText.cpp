/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General Comma Delimited Text handler
 *		PROJECT			Various
 *		ACTIVITY       
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000 SP3
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      3.00: Initial Version JGT 26-10-2002
************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JCommaText.h"
#include "JUtils.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CommaText::CommaText()
{
   pList=NULL;
}

CommaText::CommaText(const String sText, const bool bInsertCommas, const char cDelimiter)
{
   pList=NULL;
   SetText(sText,bInsertCommas,cDelimiter);
}

CommaText::~CommaText()
{
   delete pList;
}

void CommaText::SetText(const String sText, const bool bInsertCommas, const char cDelimiter)
{
   delete pList;
   pList=new TStringList;
   String s=sText.Trim();
   if (!bInsertCommas)
      {
      for (int i=1; i<=s.Length(); i++)
         if (s[i]==' ') s[i]='|';
      }
   if (!s.IsEmpty())
      {
      if (bInsertCommas)
         {
         bool bFirst=true;
         for (int i=1; i<=s.Length(); i++)
            {
            bool bDelimiter;
            if (cDelimiter==0)
               bDelimiter=(s[i]<33);
            else
               bDelimiter=(s[i]==cDelimiter);
            if (bDelimiter)
               {
               if (bFirst)
                  {
                  s[i]=',';
                  bFirst=false;
                  }
               else
                  s[i]=' ';   //Insert Space - this will be removed later
               }
            else
               bFirst=true;
            }
         s=s.Trim();
         for (int i=1; i<=s.Length(); i++)
            if (s[i]==' ') s[i]='_';
         int nLen=s.Length();
         if (nLen>0)
            {
            if (s[nLen]==',')
               s.SetLength(nLen-1);
            }
         }
      pList->CommaText=s;
      for (int n=0; n<pList->Count; n++)
         {
         pList->Strings[n]=pList->Strings[n].Trim();
         if (bInsertCommas)
            {
            s=pList->Strings[n];
            int nLen=s.Length();
            if (nLen>1)
               {
               for (int i=1; i<=nLen; i++)
                  if (s[i]=='_')
                     s[i]=' ';
               s=s.Trim();
               nLen=s.Length();
               for (int i=1; i<=nLen; i++)
                  {
                  if (s[i]==' ')
                     {
                     s.SetLength(i-1);
                     i=nLen+1;
                     }
                  }
               pList->Strings[n]=s;
               }
            }
         }
      }
}

int CommaText::nGetNum()
{
   return pList->Count;
}

String CommaText::sGetPar(const int n)
{
   String s=pList->Strings[n];
   for (int i=1; i<=s.Length(); i++)
      if (s[i]=='|') s[i]=' ';
   return s;
}

int CommaText::nGetVal(const int n)
{
   String s=pList->Strings[n];
   for (int i=1; i<=s.Length(); i++)
      if (s[i]=='|')
         s[i]=' ';
	s=s.Trim();
	return s.ToInt();
}

DWORD CommaText::dwGetVal(const int n)
{
   String s=pList->Strings[n];
   for (int i=1; i<=s.Length(); i++)
      if (s[i]=='|')
         s[i]=' ';
   s=s.Trim();
   DWORD dw;
   int nn=sscanf(AnsiString(s).c_str(),"%u",&dw);
   if (nn!=1)
      dw=0;
   return dw;
}

double CommaText::dGetVal(const int n)
{
   String s=pList->Strings[n];
   for (int i=1; i<=s.Length(); i++)
      if (s[i]=='|')
         s[i]=' ';
	s=s.Trim();
	return dAltToDouble(s);
}

double CommaText::dGetLat(const int nPar1, const int nPar2)
{
   double dV=-10000.0;
   if (nPar2<pList->Count)
      {
		AnsiString sa=pList->Strings[nPar1];
		int nL=sa.Length();
      int nPt=-1;
      for (int i=1; i<=nL; i++)
			{
			if (sa[i]=='.')
				{
            nPt=i;
            i=nL+1;
            }
         }
      if (nPt>1)
			{
			int nMinStart=nPt-3;
			double dDeg=dAltToDouble(sa.SubString(1,nMinStart));
			double dMin=dAltToDouble(sa.SubString(nPt-2,nL-nMinStart));
			dV=dDeg+dMin/60.0;
         if (pList->Strings[nPar2]==L"S") dV*=-1.0;
         }
      }
   return dV;
}

double CommaText::dGetLong(const int nPar1, const int nPar2)
{
   double dV=-10000.0;
   if (nPar2<pList->Count)
		{
		AnsiString sa=pList->Strings[nPar1];
		int nL=sa.Length();
      int nPt=-1;
      for (int i=1; i<=nL; i++)
         {
			if (sa[i]=='.')
            {
            nPt=i;
            i=nL+1;
            }
         }
      if (nPt>1)
         {
         int nMinStart=nPt-3;
			double dDeg=dAltToDouble(sa.SubString(1,nMinStart));
			double dMin=dAltToDouble(sa.SubString(nPt-2,nL-nMinStart));
         dV=dDeg+dMin/60.0;
         if (pList->Strings[nPar2]==L"W") dV*=-1.0;
         }
      }
   return dV;
}

int CommaText::nGetTime_ms(const int nPar, const bool bTranslate2SAST)
{
   int nT;
	if (nPar<pList->Count)
		{
		String s=pList->Strings[nPar];
		SYSTEMTIME T;
		T.wHour=s.SubString(1,2).ToInt();
		T.wMinute=s.SubString(3,2).ToInt();
		double dS=dAltToDouble(s.SubString(5,s.Length()-4));
		T.wSecond=floor(dS);
		T.wMilliseconds=(dS-floor(dS))*1000;
		if (bTranslate2SAST)
			{
			//Translate from UTC to SA time
			T.wHour+=2;
			T.wHour%=24;
			}
		nT=(T.wHour*3600+T.wMinute*60+T.wSecond)*1000+T.wMilliseconds;
		}
	else
		nT=-1;
	return nT;
}

int CommaText::nGetTime_sec(const int nPar)
{
	int nT_sec;
	if (nPar<pList->Count)
		{
		String s=pList->Strings[nPar];
		SYSTEMTIME T;
		T.wHour=s.SubString(1,2).ToInt();
		T.wMinute=s.SubString(3,2).ToInt();
		T.wSecond=s.SubString(5,s.Length()-4).ToInt();
		nT_sec=T.wHour*3600+T.wMinute*60+T.wSecond;
		}
	else
		nT_sec=-1;
	return nT_sec;
}

JTime CommaText::GetDateTime(const int nTimePar, const int nDatePar, const bool bTranslate2SAST)
{
	String s;
	SYSTEMTIME T;
	memset(&T,0,sizeof(T));
	if (nTimePar<pList->Count)
		{
		s=pList->Strings[nTimePar];
		T.wHour=s.SubString(1,2).ToInt();
		T.wMinute=s.SubString(3,2).ToInt();
		T.wSecond=s.SubString(5,2).ToInt();
		}
	if ((nDatePar<pList->Count)&&(nDatePar>=0))
		{
		s=pList->Strings[nDatePar];
		if (s.Length()>=6)
		T.wDay=s.SubString(1,2).ToInt();
		T.wMonth=s.SubString(3,2).ToInt();
		T.wYear=s.SubString(5,2).ToInt()+2000;
		}
	JTime T1(T);
	if (bTranslate2SAST)
		{
		int nLoc_hours=JTime(true,false).nTotalHours();
		int nUTC_hours=JTime(true,true).nTotalHours();
		int nTimeBias_hours=nUTC_hours-nLoc_hours;
		T1.ConvertToLocal(nTimeBias_hours);
		T1.ClearUTC();
		}
	else
		T1.SetUTC();
	return T1;
}

JTime CommaText::GetZDA_utc()
{
	String s;
	SYSTEMTIME T;
	memset(&T,0,sizeof(T));
	if (pList->Count>1)
		{
		s=pList->Strings[1];
		if (!s.IsEmpty())
			{
			T.wHour=s.SubString(1,2).ToInt();
			T.wMinute=s.SubString(3,2).ToInt();
			T.wSecond=s.SubString(5,2).ToInt();
			}
		}
	if (pList->Count>4)
		{
		s=pList->Strings[2];
		if (!s.IsEmpty())
			{
			T.wDay=s.ToInt();
			s=pList->Strings[3];
			T.wMonth=s.ToInt();
			s=pList->Strings[4];
			T.wYear=s.ToInt();
			}
		}
	JTime T1(T);
	T1.SetUTC();
	return T1;
}


