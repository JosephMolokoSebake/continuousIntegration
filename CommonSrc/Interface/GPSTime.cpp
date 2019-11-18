//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "GPSTime.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

GPSTime::GPSTime()
{
   dwWeek=0;
}

GPSTime::GPSTime(const JTime& T)
{
   Calc(T);
}

void GPSTime::Calc(const JTime& _T)
{
   SYSTEMTIME T=_T.Get();
   int nDays=nGetDays(T.wDay,T.wMonth,T.wYear);
   dwWeek=nDays/7;
}

JTime GPSTime::GetTime(const DWORD _dwWeek, const DWORD _dwOffs)  //dwOffs in ms
{
   DWORD dwDays=_dwWeek*7+_dwOffs/86400000;
   DWORD dwMs=_dwOffs%86400000;
   int nDay,nMonth,nYear;
   GetDate(dwDays,nDay,nMonth,nYear);
   JTime T;
   T.SetDate(nDay,nMonth,nYear);
   T.SetTime(dwMs);
   return T;
}

JTime GPSTime::GetTime(const DWORD _dwOffs)  //dwOffs in ms
{
   return GetTime(dwWeek,_dwOffs);
}

JTime GPSTime::GetTime(const JTime& _T, const DWORD _dwOffs)  //dwOffs in ms
{
   SYSTEMTIME T=_T.Get();
   int nDays=nGetDays(T.wDay,T.wMonth,T.wYear);
   DWORD dwW=nDays/7;
   return GetTime(dwW,_dwOffs);
}


int GPSTime::nMonthDays(const int nMonth, const int nYear) const
{
   if (nMonth==2)
      {
      if (bLeapYear(nYear))
         return 29;
      else
         return 28;
      }
   else
      {
      const int anDays[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
      return anDays[nMonth];
      }
}

int GPSTime::nGetDays(const int nDay, const int nMonth, const int nYear) const
{
   //Started on Sat/Sun midnight Jan 06 1980   (6th was a Sunday)
   int nD=0;
   for (int nY=1980; nY<nYear; nY++)
      nD+=nYearDays(nY);
   for (int nM=1; nM<nMonth; nM++)
      nD+=nMonthDays(nM,nYear);
   nD+=nDay;
   nD-=6;   
   return nD;
}

void GPSTime::GetDate(const int nDays, int& nDay, int &nMonth, int& nYear)
{
   int nD=nDays+6;
   nYear=1979;
   do
      {
      ++nYear;
      nD-=nYearDays(nYear);
      }
   while(nD>0);
   nD+=nYearDays(nYear);
   nMonth=0;
   do
      {
      ++nMonth;
      nD-=nMonthDays(nMonth,nYear);
      }
   while(nD>0);
   nD+=nMonthDays(nMonth,nYear);
   nDay=nD;
}

