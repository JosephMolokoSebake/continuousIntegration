//---------------------------------------------------------------------------

#ifndef GPSTimeH
#define GPSTimeH
//---------------------------------------------------------------------------

#include "JTime.h"

class GPSTime
{

   DWORD dwWeek;

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

   int nMonthDays(const int nMonth, const int nYear) const;
   int nGetDays(const int nDay, const int nMonth, const int nYear) const;
   void GetDate(const int nDays, int& nDay, int &nMonth, int& nYear);


public:

   GPSTime();

   GPSTime(const JTime& _T);

   void Calc(const JTime& _T);

   JTime GetTime(const DWORD _dwOffs);  //dwOffs in ms

   JTime GetTime(const DWORD _dwWeek, const DWORD _dwOffs);  //dwOffs in ms

   JTime GetTime(const JTime& _T, const DWORD _dwOffs);  //dwOffs in ms

   int nGetGPSWeek() const
   {
      return dwWeek;
   }

};


#endif
