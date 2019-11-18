//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "JTime.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JTime::JTime(const DWORD dwSec, const bool bUTC)
{
	SetTotalSec(dwSec,bUTC);
}

JTime& JTime::operator += (const int nSec)
{
   WORD dwMS=T.wMilliseconds;
	SetTotalSec(dwTotalSec()+nSec);
	T.wMilliseconds=dwMS;
	return *this;
}

JTime& JTime::operator -= (const int nSec)
{
   WORD dwMS=T.wMilliseconds;
	SetTotalSec(dwTotalSec()-nSec);
	T.wMilliseconds=dwMS;
   return *this;
}

void JTime::AddMilliSec(const int nMs)
{
   double dSec=dwTotalSec()+T.wMilliseconds*0.001;
   dSec+=(nMs*0.001);
   SetTotalSec(floor(dSec));
   T.wMilliseconds=(dSec-floor(dSec))*1000.0;
}

void JTime::AddSec(const double dSec)
{
   SetFullSec(dTotalSec()+dSec);
}


bool JTime::operator < (const JTime& _T)
{
   return (dTotalSec()<_T.dTotalSec());
}

bool JTime::operator <= (const JTime& _T)
{
   return (dTotalSec()<=_T.dTotalSec());
}

bool JTime::operator > (const JTime& _T)
{
   return (dTotalSec()>_T.dTotalSec());
}

bool JTime::operator >= (const JTime& _T)
{
   return (dTotalSec()>=_T.dTotalSec());
}

bool JTime::operator == (const JTime& _T)
{
   return (dwTotalSec()==_T.dwTotalSec());
}

void JTime::GoPrevDay()
{
	if (T.wDay>1)
		--T.wDay;
	else if (T.wMonth>1)
		{
		--T.wMonth;
		T.wDay=nMonthDays(T.wMonth,T.wYear);
		}
	else
		{
		--T.wYear;
		T.wMonth=12;
		T.wDay=31;
		}
}

void JTime::GoNextDay()
{
	if (T.wDay<nMonthDays(T.wMonth,T.wYear))
		++T.wDay;
	else
		{
		T.wDay=1;
		if (T.wMonth<12)
			++T.wMonth;
		else
			{
			T.wMonth=1;
			++T.wYear;
			}
		}
}

void JTime::AddDays(const int nDays)
{
	int nD=nGetDays()+nDays;
   SetDate(nD);
}

int JTime::nMonthDays(const int nMonth, const int nYear) const
{
	if ((nMonth==2)&&(bLeapYear(nYear)))
		return 29;
	else if ((nMonth>0)&&(nMonth<13))
		{
		const int anDays[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
		return anDays[nMonth];
		}
	return 1;
}

int JTime::nMonthDays() const
{
	return nMonthDays(T.wMonth,T.wYear);
}

int JTime::nGetDays() const
{
   int nD=0;
   if (T.wYear>=1990)
      {
      for (int nY=1990; nY<T.wYear; nY++)
         nD+=nYearDays(nY);
      for (int nM=1; nM<T.wMonth; nM++)
			nD+=nMonthDays(nM,T.wYear);
		nD+=T.wDay-1;
      }
   return nD;
}

int JTime::nGetDays_1970() const
{
	int nD=0;
	if (T.wYear>=1970)
		{
		for (int nY=1970; nY<T.wYear; nY++)
			nD+=nYearDays(nY);
		for (int nM=1; nM<T.wMonth; nM++)
			nD+=nMonthDays(nM,T.wYear);
		nD+=T.wDay-1;
		}
	return nD;
}

int JTime::nGetWeekDay() const
{
	return nGetDays()%7+1;
}

int JTime::nGetWeek() const   //Weeks from 1990
{
	return nGetDays()/7;
}

void JTime::SetDate(const int nDays)
{
	int nD=nDays;
	T.wYear=1989;
	do
		{
		++T.wYear;
		nD-=nYearDays(T.wYear);
		}
	while(nD>0);
	nD+=nYearDays(T.wYear);
	T.wMonth=0;
	do
		{
		++T.wMonth;
		nD-=nMonthDays(T.wMonth,T.wYear);
		}
	while(nD>0);
	nD+=nMonthDays(T.wMonth,T.wYear);
	T.wDay=nD;
}

void JTime::SetDate_1970(const int nDays)
{
	int nD=nDays;
	T.wYear=1969;
	do
		{
		++T.wYear;
		nD-=nYearDays(T.wYear);
		}
	while(nD>0);
	nD+=nYearDays(T.wYear);
	T.wMonth=0;
	do
		{
		++T.wMonth;
		nD-=nMonthDays(T.wMonth,T.wYear);
		}
	while(nD>0);
	nD+=nMonthDays(T.wMonth,T.wYear);
	T.wDay=nD;
}

DWORD JTime::dwTotalSec() const
{
	return (DWORD)(nGetDays()*86400u)+(DWORD)(nMilliSec()/1000);
}

double JTime::dTotalSec() const
{
	return nGetDays()*86400.0+dSec();
}

int JTime::nTotalHours() const
{
	return nGetDays()*24+T.wHour;
}


void JTime::SetTotalSec(const DWORD dwSec, const bool bUTC)
{
	memset(&T,0,sizeof(T));
	DWORD dwS=dwSec%86400;
	T.wHour=dwS/3600;
	T.wMinute=(dwS%3600)/60;
	T.wSecond=dwS%60;
	T.wMilliseconds=0;
	int nDays=dwSec/86400+1;
	SetDate(nDays);
	if (bUTC)
		SetUTC();
	else
		ClearUTC();
}

String JTime::sGetTimeZone() const
{
	String s="Z";
	if (!bUTC())
		{
		int n=nGetTimeZoneBias_hours();
		if (n!=0)
			{
			if (n>0)
				n-=24;
			n=abs(n);
			s[1]=(n-1)+'A';
			}
		}
	return s;
}

String JTime::sTime(const bool bShowMilliSec, const bool bAddRegion) const
{
	String s;
	if (bShowMilliSec)
		s.printf(L"%02u:%02u:%02u.%03u",T.wHour,T.wMinute,T.wSecond,T.wMilliseconds);
	else
		s.printf(L"%02u:%02u:%02u",T.wHour,T.wMinute,T.wSecond);
	if (bAddRegion)
		s=s+sGetTimeZone();
	return s;
}

String JTime::sGPSTime() const
{
	String s;
	s.printf(L"%02u%02u%02u.%03u",T.wHour,T.wMinute,T.wSecond,T.wMilliseconds);
	return s;
}

String JTime::sDate(const bool bSysRegion, const bool bAddRegion) const
{
	String s;
	if (bSysRegion)
		s=GetDateTime().DateString();
	else
		s.printf(L"%02u/%02u/%04u",T.wDay,T.wMonth,T.wYear);
	if (bAddRegion)
		s=s+sGetTimeZone();
	return s;
}

String JTime::sDateTime(const bool bSysRegion, const bool bShowMilliSec, const bool bAddRegion) const
{
	if (bSysRegion)
		return GetDateTime().DateTimeString();
	else
		{
		if (bAddRegion)
			return sDate(false,false)+" "+sTime(bShowMilliSec,false)+sGetTimeZone();
		else
			return sDate(false,false)+" "+sTime(bShowMilliSec,false);
		}
}

String JTime::sShortDateTime(const bool bSysRegion, const bool bAddRegion) const
{
	if (bSysRegion)
		return GetDateTime().DateTimeString();
   else
      {
      String s;
      s.printf(L" %02u:%02u",T.wHour,T.wMinute);
		if (bAddRegion)
			s=s+sGetTimeZone();
		return sDate()+s;
      }
}

JTime::JTime(const String sDateTime, const bool bUTC)
{
   memset(&T,0,sizeof(T));
   String s;
   bool bTime=false;
   bool bDate=false;
   int nP=1;
   int nT=0;
   int nD=0;
   try
      {
      for (int i=1; i<=sDateTime.Length(); i++)
         {
         switch(sDateTime[i])
            {
            case ':':
            case '.':
               bTime=true;
               bDate=false;
               s=sDateTime.SubString(nP,i-nP);
               nP=i+1;
               switch(nT)
                  {
                  case 0:  T.wHour=s.ToInt();         break;
                  case 1:  T.wMinute=s.ToInt();       break;
                  case 2:  T.wSecond=s.ToInt();       break;
                  case 3:  T.wMilliseconds=s.ToInt(); break;
                  default: ;
                  }
               ++nT;
               break;
            case '/':
            case '-':
               bDate=true;
               bTime=false;
               s=sDateTime.SubString(nP,i-nP);
               switch(nD)
                  {
                  case 0:  T.wDay=s.ToInt();    break;
                  case 1:  T.wMonth=s.ToInt();  break;
                  case 2:  T.wYear=s.ToInt();   break;
                  default: ;
                  }
               ++nD;
               nP=i+1;
               break;
            case ' ':
               s=sDateTime.SubString(nP,i-nP);
               if (bDate)
                  {
                  switch(nD)
                     {
                     case 0:  T.wDay=s.ToInt();    break;
                     case 1:  T.wMonth=s.ToInt();  break;
                     case 2:  T.wYear=s.ToInt();   break;
                     default: ;
                     }
                  ++nD;
                  nP=i+1;
                  }
               else if (bTime)
                  {
                  switch(nT)
                     {
                     case 0:  T.wHour=s.ToInt();         break;
                     case 1:  T.wMinute=s.ToInt();       break;
                     case 2:  T.wSecond=s.ToInt();       break;
                     case 3:  T.wMilliseconds=s.ToInt(); break;
                     default: ;
                     }
                  nP=i+1;
                  }
               break;
            default: ;
            }
         }
      if (nP<sDateTime.Length())
         {
         s=sDateTime.SubString(nP,sDateTime.Length()-nP+1);
         if (bDate)
            {
            switch(nD)
               {
               case 0:  T.wDay=s.ToInt();    break;
               case 1:  T.wMonth=s.ToInt();  break;
               case 2:  T.wYear=s.ToInt();   break;
               default: ;
               }
            }
         else if (bTime)
            {
            switch(nT)
               {
               case 0:  T.wHour=s.ToInt();         break;
               case 1:  T.wMinute=s.ToInt();       break;
               case 2:  T.wSecond=s.ToInt();       break;
               case 3:  T.wMilliseconds=s.ToInt(); break;
               default: ;
               }
            }
			}
		if (bUTC)
			SetUTC();
		else
			ClearUTC();
      }
   catch(...)
      {
		Invalidate();
      }
}

String JTime::sFileName(const String sExt, const bool bAddRegion) const
{
   String s;
	s.printf(L"%04u%02u%02u_%02u%02u%02u",T.wYear,T.wMonth,T.wDay,T.wHour,T.wMinute,T.wSecond);
	if (bAddRegion)
		s=s+sGetTimeZone();
	if (!sExt.IsEmpty())
      {
      if (sExt[1]!='.')
         s=s+".";
      s=s+sExt;
      }
   return s;
}

String JTime::sDateFileName(const String sExt, const bool bAddRegion) const
{
	String s;
	s.printf(L"%04u%02u%02u",T.wYear,T.wMonth,T.wDay);
	if (bAddRegion)
		s=s+sGetTimeZone();
	if (!sExt.IsEmpty())
		{
		if (sExt[1]!='.')
			s=s+".";
		s=s+sExt;
		}
	return s;
}

String JTime::sFileBody() const
{
	String s;
	s.printf(L"%04u%02u%02u%02u%02u%02u",T.wYear,T.wMonth,T.wDay,T.wHour,T.wMinute,T.wSecond);
	return s;
}

void JTime::GetCurrentTime()
{
	::GetLocalTime(&T);
}

bool JTime::bUpdatePCTime()
{
	OSVERSIONINFO VerInfo;
	VerInfo.dwOSVersionInfoSize=sizeof(VerInfo);
	::GetVersionEx(&VerInfo);
	bool bWinNT=(VerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT);
	bool bOK=true;
	if (bWinNT)
		{
		//Windows NT - Before setting time, create the privilege to do so
		HANDLE hToken;
		//Get a token to be able to adjust privileges
		bOK=::OpenProcessToken(::GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
		if (bOK)
			{
			LUID_AND_ATTRIBUTES Attr;
			//Get the LUID for time adjustment
			bOK=::LookupPrivilegeValue(NULL,SE_SYSTEMTIME_NAME,&Attr.Luid);
			if (bOK)
				{
				Attr.Attributes=SE_PRIVILEGE_ENABLED;
				TOKEN_PRIVILEGES TokenPriv;
				TokenPriv.PrivilegeCount=1;
				TokenPriv.Privileges[0]=Attr;
				//Set privilege to be able to adjust time
				bOK=::AdjustTokenPrivileges(hToken,false,&TokenPriv,NULL,NULL,NULL);
				if (bOK)
					{
					SYSTEMTIME TT;
					TT=T;
					TT.wDayOfWeek=0;
					if (bUTC())
						{
						if (!::SetSystemTime(&TT))
							bOK=false;
						}
					else
						{
						if (!::SetLocalTime(&TT))
							bOK=false;
						}
					}
				//Disable privilege again
				::AdjustTokenPrivileges(hToken,true,&TokenPriv,NULL,NULL,NULL);
				}
			}
		}
	else
		{
		//Windows 95 - no security
		SYSTEMTIME TT;
		TT=T;
		TT.wDayOfWeek=0;
		if (bUTC())
			{
			if (!::SetSystemTime(&TT))
				bOK=false;
			}
		else
			{
			if (!::SetLocalTime(&TT))
				bOK=false;
			}
		}
	return bOK;
}

void JTime::SetTimeFromFileName(const String sFile)
{
	memset(&T,0,sizeof(T));
	try
		{
		String s=sFileNameOnly(sFile);
		s=sFileNewExt(s,"");
		int nPos=s.Pos("2");
		if (nPos>0)
			{
			if (s.Length()>=nPos+8)
				{
				String sYear=s.SubString(nPos,4);
				String sMonth=s.SubString(nPos+4,2);
				String sDay=s.SubString(nPos+6,2);
				T.wYear=sYear.ToInt();
				T.wMonth=sMonth.ToInt();
				T.wDay=sDay.ToInt();
				}
			if (s.Length()>=nPos+14)
				{
				if (s[nPos+8]=='_')
					{
					String sHour=s.SubString(nPos+9,2);
					String sMin=s.SubString(nPos+11,2);
					String sSec=s.SubString(nPos+13,2);
					T.wHour=sHour.ToInt();
					T.wMinute=sMin.ToInt();
					T.wSecond=sSec.ToInt();
					}
				}
			if ((s.Pos("UTC")>0)||(s.Pos("Z")>0))
				SetUTC();
			}
		}
	catch(...)
		{
		Invalidate();
		}
}

int JTime::nGetPOSIX_sec()
{
	int nD=nGetDays_1970();
	int nMS=nMilliSec();
	int nT_sec=nD*86400+nMS/1000;
	if (!bUTC())
		nT_sec+=nGetTimeZoneBias_hours()*3600;
return nT_sec;
}

void JTime::SetPOSIX(const int nPOSIX_sec)
{
	memset(&T,0,sizeof(T));
	int nS=nPOSIX_sec%86400;
	T.wHour=nS/3600;
	T.wMinute=(nS%3600)/60;
	T.wSecond=nS%60;
	T.wMilliseconds=0;
	int nDays=nPOSIX_sec/86400+1;
	SetDate_1970(nDays);
	SetUTC();
}



int nGetTimeZoneBias_hours()
{
	int nLoc_hours=JTime(true,false).nTotalHours();
	int nUTC_hours=JTime(true,true).nTotalHours();
	return nUTC_hours-nLoc_hours;
}


