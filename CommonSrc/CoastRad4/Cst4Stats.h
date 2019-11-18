//---------------------------------------------------------------------------

#ifndef Cst4StatsH
#define Cst4StatsH
//---------------------------------------------------------------------------

#include "JFile.h"
#include "JTime.h"
#include "Cst4Defs.h"
#include "Cst4SiteDefs.h"


class Cst4SiteStats
{
	struct MinStruct
	{
		int nMin_utc;
		int nCnt;

	} aMin[24*60];

	WORD wSiteID;
	DWORD dwFirstMsgTime_utc_sec;
	DWORD dwLastMsgTime_utc_sec;
	DWORD dwTotalMsgCnt;
	String sDescr;

public:

	void Assign(const Cst4SiteStats& S)
	{
		wSiteID=S.wSiteID;
		dwFirstMsgTime_utc_sec=S.dwFirstMsgTime_utc_sec;
		dwLastMsgTime_utc_sec=S.dwLastMsgTime_utc_sec;
		dwTotalMsgCnt=S.dwTotalMsgCnt;
		memcpy(aMin,S.aMin,sizeof(aMin));
		sDescr=S.sDescr;
	}


	Cst4SiteStats()
	{
		wSiteID=0xFFFF;
		memset(aMin,0,sizeof(aMin));
		dwFirstMsgTime_utc_sec=0;
		dwLastMsgTime_utc_sec=0;
		dwTotalMsgCnt=0;
		sDescr="";
	}

	Cst4SiteStats(const WORD wID, const String _sDescr)
	{
		wSiteID=wID;
		memset(aMin,0,sizeof(aMin));
		dwFirstMsgTime_utc_sec=0;
		dwLastMsgTime_utc_sec=0;
		dwTotalMsgCnt=0;
		sDescr=_sDescr;
	}

   Cst4SiteStats(const Cst4SiteStats& S)
   {
      Assign(S);
	}

	~Cst4SiteStats()
	{
	}

	Cst4SiteStats& operator = (const Cst4SiteStats& S)
	{
		Assign(S);
		return *this;
	}


	bool operator < (const Cst4SiteStats& S) const
	{
		return (sDescr.CompareIC(S.sDescr)<0);
	}

	bool operator > (const Cst4SiteStats& S) const
	{
		return (sDescr.CompareIC(S.sDescr)>0);
	}

	JFile::FILE_ERROR Read(JFile* pFil);
	JFile::FILE_ERROR Store(JFile* pFil);
	JFile::FILE_ERROR ReadOldFormat(JFile* pFil);

	void SetDescr(const String s)
	{
		sDescr=s;
	}

	String sGetDescr() {return sDescr;}

	void AddMsg_utc(const DWORD dwSec_utc);

	void AddMsg_utc(const JTime& T_utc);

	DWORD dwGetMsgCnt_utc(const JTime& T_utc, const int nMinutes);
	DWORD dwGetMsgCnt_utc(const DWORD dwT_utc_sec, const int nMinutes);

	WORD wGetID() const 	{	return wSiteID;	}

	JTime GetFirstMsgTime_utc() 		 const{return JTime(dwFirstMsgTime_utc_sec,true);	}
	JTime GetFirstMsgTime_loc() 		 const{return JTime(dwFirstMsgTime_utc_sec-g_nCst4TimeBias_hours*3600,false);}
	DWORD dwGetFirstMsgTime_sec_utc() const{return dwFirstMsgTime_utc_sec;					}
	DWORD dwGetFirstMsgTime_sec_loc() const{return dwFirstMsgTime_utc_sec-g_nCst4TimeBias_hours*3600;	}
	double dGetFirstMsgTime_sec_utc() const{return dwFirstMsgTime_utc_sec;					}
	double dGetFirstMsgTime_sec_loc() const{return dwFirstMsgTime_utc_sec-g_nCst4TimeBias_hours*3600.0;}


	JTime GetLastMsgTime_utc() 		 const{return JTime(dwLastMsgTime_utc_sec,true);	}
	JTime GetLastMsgTime_loc() 		 const{return JTime(dwLastMsgTime_utc_sec-g_nCst4TimeBias_hours*3600,false);}
	DWORD dwGetLastMsgTime_sec_utc()  const{return dwLastMsgTime_utc_sec;					}
	DWORD dwGetLastMsgTime_sec_loc()  const{return dwLastMsgTime_utc_sec-g_nCst4TimeBias_hours*3600;	}
	double dGetLastMsgTime_sec_utc()  const{return dwLastMsgTime_utc_sec;					}
	double dGetLastMsgTime_sec_loc()  const{return dwLastMsgTime_utc_sec-g_nCst4TimeBias_hours*3600.0;	}

	DWORD dwGetTotalMsgCnt() 			 const{return dwTotalMsgCnt;		}

	int nGetMin_utc(const int n) 		 const{return aMin[n].nMin_utc;	}
	int nGetCnt(const int n) 			 const{return aMin[n].nCnt;		}

	void Pack(BYTE* pucPacked, int& n);
   int nGetPackedSize();
	void Unpack(const BYTE* pucPacked, int& n);


};


class Cst4Stats
{

	enum
	{
		SITESTATS_MIN_VERSION	= 0x80000000,
		SITESTATS_VERSION			= 0x80000001,
	};

	Cst4SiteStats* apSite[65536];

public:

	Cst4Stats();
	~Cst4Stats();

	void AddMsg_utc(const WORD wID, const DWORD dwSec_utc)
	{
		if (!apSite[wID])
			{
			String sDescr=g_pSiteDefs->sGetDescrFromID(wID);
			apSite[wID]=new Cst4SiteStats(wID,sDescr);
			}
		apSite[wID]->AddMsg_utc(dwSec_utc);
	}

	DWORD dwGetMsgCnt_utc(const WORD wID, const JTime& T_utc, const int nMinutes)
	{
		if (apSite[wID])
			return apSite[wID]->dwGetMsgCnt_utc(T_utc,nMinutes);
		return 0;
	}

	DWORD dwGetMsgCnt_utc(const WORD wID, const DWORD dwT_utc_sec, const int nMinutes)
	{
		if (apSite[wID])
			return apSite[wID]->dwGetMsgCnt_utc(dwT_utc_sec,nMinutes);
      return 0;
   }

   Cst4SiteStats* pStatFromID(const WORD wID)
   {
      return apSite[wID];
   }

   void Store(const String sFile);
   void Read(const String sFile);

	int nGetPackSize();
	int nPack(BYTE* pucPacked);
	void Unpack(const BYTE* pucPacked);

};



#endif
