//---------------------------------------------------------------------------

#ifndef JTimeH
#define JTimeH
//---------------------------------------------------------------------------

#include <math.h>

class JTime
{
	SYSTEMTIME T;

	enum
	{
		MICROSEC_MASK	= 0x7FFF,
		MICROSEC_CLEAR	= 0x8000,
		UTC_SET			= 0x8000,
		UTC_CLEAR 		= 0x7FFF,
	};

public:

	JTime(const bool bGetCurrent=false, const bool bUTC=false)
	{
		if (bGetCurrent)
			{
			if (bUTC)
				{
				::GetSystemTime(&T);
				T.wDayOfWeek=UTC_SET;
				}
			else
				{
				::GetLocalTime(&T);
				T.wDayOfWeek=0;
				}
			}
		else
			memset(&T,0,sizeof(T));
	}

	JTime(const JTime& _T)
	{
		T=_T.T;
	}

	JTime(const int nDay, const int nMonth, const int nYear,const int nHour, const int nMinute, const int nSecond, const int nMilliseconds=0, const bool bUTC=false)
	{
		T.wDay=nDay;
		T.wMonth=nMonth;
		T.wYear=nYear;
		T.wHour=nHour;
		T.wMinute=nMinute;
		T.wSecond=nSecond;
		T.wMilliseconds=nMilliseconds;
		if (bUTC)
			T.wDayOfWeek=UTC_SET;
		else
			T.wDayOfWeek=0;
	}

   JTime(const SYSTEMTIME& _T)
   {
		T=_T;
   }

	JTime(const DWORD dwSec, const bool bUTC=false);

	JTime(const double dSec, const bool bUTC=false)
	{
		SetFullSec(dSec,bUTC);
	}

	JTime(const String sDateTime, const bool bUTC=false);

   JTime& operator = (const JTime& _T)
   {
      T=_T.T;
      return *this;
   }

   JTime& operator = (const SYSTEMTIME& _T)
   {
      T=_T;
      return *this;
   }

   JTime& operator += (const int nSec);
   JTime& operator -= (const int nSec);

   bool operator < (const JTime& _T);
   bool operator <= (const JTime& _T);
   bool operator > (const JTime& _T);
   bool operator >= (const JTime& _T);
	bool operator == (const JTime& _T);

	bool bUTC() const
	{
		return (T.wDayOfWeek&UTC_SET)!=0;
	}

	void SetUTC()
	{
		T.wDayOfWeek|=UTC_SET;
	}

	void ClearUTC()
	{
		T.wDayOfWeek&=UTC_CLEAR;
	}

	void GoPrevDay();
	void GoNextDay();

	void AddLocalBias(const int nLocalBias_hours)  //-23 to 23 hours only
	{
		assert((nLocalBias_hours>=-23)&&(nLocalBias_hours<=23));  	//must set Bias if UTC time
		int nNewHour=(int)T.wHour+nLocalBias_hours;
		if ((nNewHour>=0)&&(nNewHour<24))
			T.wHour=(WORD)nNewHour;	//conversion does not influence date
		else
			{
			if (nNewHour<0)
				{
				T.wHour=(WORD)(24+nNewHour);
				GoPrevDay();
				}
			else
				{
				T.wHour=(WORD)(nNewHour-24);
				GoNextDay();
				}
			}

	}

	void ForceConvertToUTC(const int nLocalBias_hours)	//NB for SAST, nLocalBias_hours = -2
	{
		AddLocalBias(nLocalBias_hours);
		SetUTC();
	}

	void ForceConvertToLocal(const int nLocalBias_hours)	//NB for SAST, nLocalBias_hours = -2
	{
		AddLocalBias(-nLocalBias_hours);
		ClearUTC();
	}

	void ConvertToUTC(const int nLocalBias_hours)	//NB for SAST, nLocalBias_hours = -2
	{
		if (!bUTC())
			ForceConvertToUTC(nLocalBias_hours);
	}

	JTime ReturnUTC(const int nLocalBias_hours) const
	{
		JTime TT=*this;
		TT.ConvertToUTC(nLocalBias_hours);
		return TT;
	}

	void ConvertToLocal(const int nLocalBias_hours)	//NB for SAST, nLocalBias_hours = -2
	{
		if (bUTC())
			ForceConvertToLocal(nLocalBias_hours);
	}

	JTime ReturnLocal(const int nLocalBias_hours) const
	{
		JTime TT=*this;
		TT.ConvertToLocal(nLocalBias_hours);
		return TT;
	}

   void AddMilliSec(const int nMs);

	void AddSec(const double dSec);

	void RoundToMinute()
	{
		if (T.wSecond>=30)
			{
			DWORD dwT=dwTotalSec();
			dwT/=60; 	//minutes
			dwT+=1;		//add one minute
			dwT*=60;		//back to seconds
			SetTotalSec(dwT,bUTC());
			}
		else
			{
			T.wSecond=0;
			T.wMilliseconds=0;
			}
	}

   int nSubtract(const JTime& T1) //difference in millisec
   {
      int nMilliSec=(this->dwTotalSec()-T1.dwTotalSec())*1000;
      nMilliSec+=((int)this->T.wMilliseconds-(int)T1.T.wMilliseconds);
      return nMilliSec;
   }


   void Invalidate()
   {
      T.wYear=0;
      T.wMonth=0;
      T.wDay=0;
   }

   bool bValid() const
   {
      return (T.wYear>0)&&(T.wMonth>0)&&(T.wDay>0);
   }

	bool bAllValid() const
   {
      if ((T.wYear<1980)||(T.wYear>2070)) return false;
      if ((T.wMonth<1)||(T.wMonth>12)) return false;
      if ((T.wDay<1)||(T.wDay>31)) return false;
      if (T.wHour>23) return false;
      if (T.wMinute>59) return false;
      if (T.wSecond>59) return false;
      return true;
   }

   int nGetDays() const;      //days from 1990

   int nGetDays_1970() const;      //days from 1970

   int nGetWeekDay() const;   //1=Sun,2=Mon,3=Tue ...  7=Sat

   int nGetWeek() const;      //Weeks from 1990

	void AddDays(const int nDays);

	int nMonthDays(const int nMonth, const int nYear) const;

	int nMonthDays() const;	//Current month/year

   bool bLeapYear(const int nYear) const
   {
      return ((nYear%4)==0)&&(!(((nYear%100)==0)&&((nYear%400)!=0)));
   }

   int nYearDays(const int nYear) const
   {
      if (bLeapYear(nYear))
         return 366;
      else
         return 365;
	}

	void SetDay(const WORD wDay)
	{
		T.wDay=wDay;
	}

	WORD wGetDay() const
	{
		return T.wDay;
	}

	int nTotalHours() const;

	DWORD dwTotalSec() const;
	void SetTotalSec(const DWORD dwSec, const bool bUTC=false);

	void SetHMS(const int nH, const int nM, const int nS, const int nMS=0)
	{
		T.wHour=nH;
		T.wMinute=nM;
		T.wSecond=nS;
		T.wMilliseconds=nMS;
	}


   int nMilliSec() const
   {
      return (T.wHour*3600+T.wMinute*60+T.wSecond)*1000+T.wMilliseconds;
   }

	void SetFullSec(const double dT, const bool bUTC=false)
	{
      DWORD dwSec=floor(dT);
      SetTotalSec(dwSec,bUTC);
      int nMilliSec=floor(fmod(dT*1000.0,1000.0));
		T.wMilliseconds=nMilliSec;
	}

	double dSec(const bool bAddMicroSec=false) const
	{
		if (bAddMicroSec)
			return T.wHour*3600.0+T.wMinute*60.0+T.wSecond+T.wMilliseconds*0.001+(T.wDayOfWeek&MICROSEC_MASK)*1e-6;
		else
			return T.wHour*3600.0+T.wMinute*60.0+T.wSecond+T.wMilliseconds*0.001;
	}

	int nSeconds() const
	{
		return T.wHour*3600+T.wMinute*60+T.wSecond;
	}

	int nMinutes() const
	{
		return T.wHour*60+T.wMinute;
	}

	double dTotalSec() const;

   void SetTime(const int nMs)
   {
      int nSec=nMs/1000;
      T.wHour=nSec/3600;
      T.wMinute=(nSec%3600)/60;
      T.wSecond=nSec%60;
      T.wMilliseconds=nMs%1000;
   }

	void SetTime_ms(const int nMs)
	{
		SetTime(nMs);
	}

	void SetDate(const int nDays);
	void SetDate_1970(const int nDays);

	void SetDate(const SYSTEMTIME& D, const bool bUTC=false)
	{
		T.wYear=D.wYear;
		T.wMonth=D.wMonth;
		T.wDay=D.wDay;
		if (bUTC)
			T.wDayOfWeek=UTC_SET;
		else
			T.wDayOfWeek=0;
	}

	void SetDate(const int nDay, const int nMonth, const int nYear, const bool bUTC=false)
	{
		T.wYear=nYear;
		T.wMonth=nMonth;
		T.wDay=nDay;
		if (bUTC)
			T.wDayOfWeek=UTC_SET;
		else
			T.wDayOfWeek=0;
		if (bUTC)
			SetUTC();
		else
			ClearUTC();
	}

	void SetDate(const JTime& D, const bool bUTC=false)
	{
		SetDate(D.Get().wDay,D.Get().wMonth,D.Get().wYear,bUTC);
	}

   void SetDCSTime(const DWORD dw100us)   //in 100us intervals
   {
      DWORD dw=dw100us&0x7FFFFFFF;     // Bit 31 shows time valid
      SetTime(dw/10);
      SetMicroSec((dw%10)*100);
   }

   DWORD dwGetDCSTime() const
   {
      return nMilliSec()*10+nGetMicroSec()/100;
   }

   void SetDCSDate(const WORD wPacked)
   {
      SetDate(wPacked&0x001F,(wPacked>>5)&0x000F,((wPacked>>9)&0x007F)+2000);
   }

   WORD wGetDCSDate() const
   {
      WORD w=T.wDay;
      w|=(T.wMonth<<5);
      w|=(T.wYear-2000)<<9;
      return w;
   }

	void SetDCSDateTime(const WORD wDate, const DWORD dwTime_100us)
	{
		SetDCSDate(wDate);
      SetDCSTime(dwTime_100us);
   }

   void SetTime(const int nHour, const int nMinute, const int nSecond, const int nMilliseconds=0)
   {
      T.wHour=nHour;
      T.wMinute=nMinute;
      T.wSecond=nSecond;
      T.wMilliseconds=nMilliseconds;
   }

   void ClearSeconds()
   {
      T.wSecond=0;
      T.wMilliseconds=0;
   }

   void SetDate(const TDateTime& DT)
   {
      DT.DecodeDate(&T.wYear,&T.wMonth,&T.wDay);
   }

   void SetTime(const TDateTime& DT)
   {
		DT.DecodeTime(&T.wHour,&T.wMinute,&T.wSecond,&T.wMilliseconds);
		T.wMilliseconds=0;
   }

   void SetDateTime(const TDateTime& DT)
   {
      DT.DecodeDate(&T.wYear,&T.wMonth,&T.wDay);
      DT.DecodeTime(&T.wHour,&T.wMinute,&T.wSecond,&T.wMilliseconds);
   }

   void SetMillisecOnly(const int nMilliSec)
   {
      T.wMilliseconds=nMilliSec;
   }

	void SetMicroSec(const int nMicroSec)
	{
		T.wDayOfWeek&=MICROSEC_CLEAR;
		T.wDayOfWeek|=((WORD)nMicroSec&MICROSEC_MASK);
	}

	int nGetMicroSec() const
	{
		return T.wDayOfWeek&MICROSEC_MASK;
   }

   TDateTime GetDateTime() const
   {
      TDateTime Date=TDateTime(T.wYear,T.wMonth,T.wDay);
      TDateTime Time=TDateTime(T.wHour,T.wMinute,T.wSecond,T.wMilliseconds);
      return Date+Time;
   }

   TDateTime GetDate()
   {
      return TDateTime(T.wYear,T.wMonth,T.wDay);
   }

   TDateTime GetTime()
   {
      return TDateTime(T.wHour,T.wMinute,T.wSecond,T.wMilliseconds);
   }


	void SetDateTime(const SYSTEMTIME& DT)
	{
		T=DT;
	}

	SYSTEMTIME Get() const
	{
		return T;
	}

	void Set(const SYSTEMTIME& _T)
	{
		T=_T;
	}

	int nPackDate()   //Pack date into 16 bits
	{
		return ((T.wYear-2000)<<9)|(T.wMonth<<5)|T.wDay;
	}

	void UnpackDate(const int nDate)
	{
		T.wYear=((nDate>>9)&0x0000007F)+2000;
		T.wMonth=(nDate>>5)&0x0000000F;
		T.wDay=nDate&0x0000001F;
   }

	String sGetTimeZone() const;
	String sTime(const bool bShowMilliSec=false, const bool bAddRegion=false) const;
	String sGPSTime() const;
	String sDate(const bool bSysRegion=false, const bool bAddRegion=false) const;
	String sDateTime(const bool bSysRegion=false, const bool bShowMilliSec=false, const bool bAddRegion=false) const;
	String sShortDateTime(const bool bSysRegion=false, const bool bAddRegion=false) const;
	String sFileName(const String sExt="",const bool bAddRegion=false) const;
	String sDateFileName(const String sExt="",const bool bAddRegion=false) const;
	String sFileBody() const;

	void SetTimeFromFileName(const String sFile);

	void GetCurrentTime();
	bool bUpdatePCTime();

	DWORD dwNextInterval_sec(const int nInterval_sec, const DWORD dwCurrentTime_sec=0) const
	{
		DWORD dwNext_sec;
		if (dwCurrentTime_sec==0)
			dwNext_sec=dwTotalSec()+nInterval_sec;
		else
			dwNext_sec=dwCurrentTime_sec+nInterval_sec;
		dwNext_sec/=nInterval_sec;
		dwNext_sec*=nInterval_sec;
		return dwNext_sec;
	}

	int nGetPOSIX_sec();
	void SetPOSIX(const int nPOSIX_sec);

};

int nGetTimeZoneBias_hours();


#endif
