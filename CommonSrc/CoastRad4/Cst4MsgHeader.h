//---------------------------------------------------------------------------

#ifndef Cst4MsgHeaderH
#define Cst4MsgHeaderH
//---------------------------------------------------------------------------

#include "Cst4Defs.h"

#pragma option push -a4    //Set compiler to DWORD alignment

enum
{
	MAX_CST4_SOURCE_CNT  = 65536,
};


class Cst4_MsgHeader : public Cst4_MsgUtils
{

	DWORD wMsgSource;
	DWORD wMsgType;
	DWORD dwInfo;
	DWORD dwPayloadSize;
	Cst4Time T;
	void* pPayload;

	enum
	{
		CST4_SERVERINDEX_MASK   = 0x0000FFFF,  //after shift !!!
		CST4_SERVERINDEX_SHIFT  = 16,
		CST4_SERVERINDEX_CLEAR  = 0x0000FFFF,  //after shift !!!
	};


protected:

	DWORD dwGetHeaderSize() const
	{
		return sizeof(wMsgSource)+sizeof(wMsgType)+sizeof(dwInfo)+sizeof(dwPayloadSize)+T.dwGetSize();
	}


	void Init(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		wMsgSource=0;
		wMsgType=_MsgType;
		dwInfo=NULL;
		T.Invalidate();
		delete pPayload;
		pPayload=NULL;
		dwPayloadSize=0;
	}



public:

	void Invalidate()
	{
		delete pPayload;
		pPayload=NULL;
		dwPayloadSize=0;
	}

	bool bValid() const
	{
		return dwPayloadSize!=0;
	}

	Cst4_MsgHeader(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		Init(_MsgType);
	}

	void Assign(const Cst4_MsgHeader& M)
	{
		wMsgSource=M.wMsgSource;
		wMsgType=M.wMsgType;
		dwInfo=M.dwInfo;
		if (dwPayloadSize==M.dwPayloadSize)
			memcpy(pPayload,M.pPayload,dwPayloadSize);
		else
			{
			dwPayloadSize=M.dwPayloadSize;
			delete pPayload;
			if (dwPayloadSize>0)
				{
				pPayload=new BYTE[dwPayloadSize];
				memcpy(pPayload,M.pPayload,dwPayloadSize);
				}
			else
				pPayload=NULL;
			}
		T=M.T;
	}

	Cst4_MsgHeader(const Cst4_MsgHeader& M)
	{
		Assign(M);
	}

	~Cst4_MsgHeader()
	{
	}

	Cst4_MsgHeader& operator = (const Cst4_MsgHeader& M)
	{
		Assign(M);
		return *this;
	}

	bool operator < (const Cst4_MsgHeader& M) const
	{
		return (T.dSec_raw()<M.T.dSec_raw());
	}

	bool operator > (const Cst4_MsgHeader& M) const
	{
		return (T.dSec_raw()>M.T.dSec_raw());
	}

	void SetSource(const WORD wSrc)
	{
		wMsgSource=wSrc;
	}

	WORD wSource() const
	{
		return wMsgSource;
	}

	WORD wSourceCountryID() const	//NB this is NOT the telephone code!!
	{
		return (wSource()>>12)&0x000F;    //1 = South Africa, 2 = Namibia, 3 = Mauritania, 4 = SAN Vessels
	}

	WORD wSourceLocation() const
	{
		return (wSource()&0xFFF0);
	}

	bool bSat_Source() const
	{
		return (wSource()==0x11C3);
	}

	bool bLRIT_Source() const
	{
		return (wSource()==0x11C4);
	}

	bool bSat_or_LRIT_Source() const
	{
		return ((wSource()&0xFFF0)==0x11C0);
	}

	void FixUTC()
	{
		WORD wCountryID=wSourceCountryID();
		if (wCountryID==3)  //Check if it is from Mauritania
			{
			if (T.nGetAge_hours()!=3)
				{
				T.SetLocal();  	//Mauritania data is actually SAST, although it is marked as UTC!!!
				T.SetAge(3);		//Set age to 3 to signify that the time has been corrected
				}
			}
		T.ConvertToUTC();
	}

	void ConvertTimeToLocal()
	{
		T.ConvertToLocal();
	}

/*
	void SetSize(const DWORD dwSize)   //Size <= 1048576 bytes
	{
		dwStatus&=(~CST4_STAT_SIZE_MASK);
		dwStatus|=(dwSize<<CST4_STAT_SIZE_OFFS);
	}

	DWORD dwSize() const
	{
		return (dwStatus&CST4_STAT_SIZE_MASK)>>CST4_STAT_SIZE_OFFS;
	}
*/

	void SetTime(const JTime& _T, const bool bUpdateAge)
	{
		int nAge_hours=7;
		if (bUpdateAge)
			{
			if (_T.bUTC())
				{
				if (g_nCst4LastUpdate_utc_hours!=0)
					nAge_hours=_T.nTotalHours()-g_nCst4LastUpdate_utc_hours;
				}
			else
				{
				if (g_nCst4LastUpdate_utc_hours!=0)
					{
					assert(g_nCst4TimeBias_hours!=100000);  		//must set Bias if UTC time
					nAge_hours=_T.nTotalHours()-(g_nCst4LastUpdate_utc_hours-g_nCst4TimeBias_hours);
					}
				}
			}
		T=Cst4Time(_T,nAge_hours);
	}

	void SetTimeAndAge(const JTime& _T, const int nAge_hours)
	{
		T=Cst4Time(_T,nAge_hours);
	}

	void SetTime_utc(const double dTime_sec_utc)
	{
		T.SetTime(JTime(dTime_sec_utc));
		T.SetUTC();
	}

	void SetTime(const Cst4Time& _T)		//Already contains age
	{
		T=_T;
	}

	JTime Time_loc() const
	{
		return T.Get_loc();
	}

	JTime Time_utc() const
	{
		return T.Get_utc();
	}

	int nTimeAge_hours() const
	{
		return T.nGetAge_hours();
	}

	Cst4Time CstTime() const
	{
		return T;
	}

	double dTime_sec_loc() const
	{
		return T.dSec_loc();
	}

	double dTime_sec_utc() const
	{
		return T.dSec_utc();
	}

	DWORD dwTime_sec_loc() const
	{
		return T.dwSec_loc();
	}

	DWORD dwTime_sec_utc() const
	{
		return T.dwSec_utc();
	}

	void CheckTime_utc(const double dT_utc_sec, const double dMaxTimeDiff_sec)
	{
		if (fabs(T.dSec_utc()-dT_utc_sec)>dMaxTimeDiff_sec)
			SetTime_utc(dT_utc_sec);
	}

	void FixTime_utc(const JTime& T_utc, const double dT_utc_sec)
	{
		if ((T.Get_utc().Get().wHour>23)||(T.Get_utc().Get().wMinute>59)||(T.Get_utc().Get().wSecond>59))
			SetTime_utc(dT_utc_sec);
	}

	double dTimeDiff_sec(const Cst4_MsgHeader& M) const
	{
		return fabs(T.dSec_raw()-M.T.dSec_raw());
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		PackWord(wMsgSource,pucBuf,nOffs);
		PackWord(wMsgType,pucBuf,nOffs);
		PackDWORD(dwInfo,pucBuf,nOffs);
		T.Pack(pucBuf,nOffs);
		PackData(pPayload,dwPayloadSize,pucBuf,nOffs);
	}

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (nOffs>nMaxSize-(int)dwGetBaseSize(bSkipStatus))
			return false;
		if (bSkipStatus)
			dwStatus=0;
		else
			{
			dwStatus=dwUnpack(pucBuf,nOffs);
			if (nOffs>nMaxSize-(int)dwPayloadSize(bSkipStatus))
				return false;
			}
		dwDataSources=dwUnpack(pucBuf,nOffs);
		T.Unpack(pucBuf,nOffs);
		FixUTC();	//All data will now be UTC
		return true;
	}

	void SetPayload(const void* p, const DWORD dwSize)
	{
		if (pPayload)
			{
			if (dwPayloadSize!=dwSize)
				{
				delete pPayload;
				dwPayloadSize=dwSize;
				pPayload=new BYTE[dwPayloadSize];
				}
			}
		else
			{
			dwPayloadSize=dwSize;
			pPayload=new BYTE[dwPayloadSize];
			}
		memcpy(pPayload,p,dwPayloadSize);
	}

	void* pGetPayload() const
	{
		return pPayload;
	}

	void GetPayload(void* p)
	{
		if (dwPayloadSize>0)
			memcpy(p,pPayload,dwPayloadSize);
	}

	DWORD dwGetPayloadSize() const
	{
		return dwPayloadSize;
	}

/*
	void SetServerIndex(const int nIndex)
	{
		dwDataSources&=CST4_SERVERINDEX_MASK;
		int n=nIndex+1;
		if (n<0)
			n=0;
		else if (n>65535)
			n=65535;
		dwDataSources|=(n<<CST4_SERVERINDEX_SHIFT);
	}

	int nGetServerIndex()
	{
		int n=(dwDataSources>>CST4_SERVERINDEX_SHIFT)&CST4_SERVERINDEX_MASK;
		return n-1;
	}

	void ClearServerIndex()
	{
		dwDataSources&=CST4_SERVERINDEX_MASK;
	}

	void SetCst3Msg()
	{
		dwStatus|=CST4_STAT_MSG_CST3;
	}

	void ClearCst3Msg()
	{
		dwStatus&=(~CST4_STAT_MSG_CST3);
	}

	bool bCst3Msg() const
	{
		return (dwStatus&CST4_STAT_MSG_CST3)!=0;
	}

	void SetAIS_TX(const bool bTX_On)
	{
		dwStatus|=CST4_AIS_TX_AVAIL;
		if (bTX_On)
			dwStatus|=CST4_AIS_TX_ON;
		else
			dwStatus&=(~CST4_AIS_TX_ON);
	}

	bool bAIS_TX_Avail() const
	{
		return (dwStatus&CST4_AIS_TX_AVAIL)!=0;
	}

	bool bAIS_TX_On() const
	{
		if (bAIS_TX_Avail())
			return (dwStatus&CST4_AIS_TX_ON)!=0;
		else
			return false;
	}
*/

};



#pragma option pop    		//Set compiler back to default alignment

#endif
