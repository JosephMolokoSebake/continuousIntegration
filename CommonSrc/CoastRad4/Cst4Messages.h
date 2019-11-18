//---------------------------------------------------------------------------
#ifndef Cst4MessagesH
#define Cst4MessagesH
//---------------------------------------------------------------------------

#include "JTransform.h"
#include "JTime.h"
#include "Cst4Defs.h"
#include "JList.h"
#include "AISDataDefs_210.h"
#include "Cst4SiteDefs.h"
#include "JUtils.h"
#include "JFile.h"

#pragma option push -a4    //Set compiler to DWORD alignment

//***************************************************************************

enum
{
	MAX_CST4_SOURCE_CNT  = 65536,
};

class Cst4_MessageUtils
{

public:

	void PackBYTE(const BYTE uc, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&uc,sizeof(uc));
		nOffs+=sizeof(uc);
	}

	void PackBool(const bool b, BYTE* pucBuf, int& nOffs)
	{
		PackBYTE((BYTE)b,pucBuf,nOffs);
	}

	void PackWORD(const WORD w, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&w,sizeof(w));
		nOffs+=sizeof(w);
	}

	void PackDWORD(const DWORD dw, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&dw,sizeof(dw));
		nOffs+=sizeof(dw);
	}

	void PackInt(const int n, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&n,sizeof(n));
		nOffs+=sizeof(n);
	}

//	void PackFloat(const float f, BYTE* pucBuf, int& nOffs)
//	{
//		memcpy(&pucBuf[nOffs],&f,sizeof(f));
//		nOffs+=sizeof(f);
//	}

	void PackString(const String s, BYTE* pucBuf, int& nOffs)
	{
		AnsiString s1=s;
		WORD w=s1.Length();
		PackWORD(w,pucBuf,nOffs);
		if (w>0)
			{
			for (WORD i=1; i<=w; i++)
				pucBuf[nOffs++]=s1[i];
			}
	}

	DWORD dwPackedStringSize(const String s) const
	{
		AnsiString s1=s;
		return s1.Length()+sizeof(WORD);
	}

	void PackLL(const LatLong& LL, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&LL,sizeof(LL));
		nOffs+=sizeof(LL);
	}

	void PackLL(const Cst4LatLong& LL, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&LL,sizeof(LL));
		nOffs+=sizeof(LL);
	}

	BYTE ucUnpack(const BYTE* pucBuf, int& nOffs) const
	{
		BYTE uc;
		memcpy(&uc,&pucBuf[nOffs],sizeof(uc));
		nOffs+=sizeof(uc);
		return uc;
	}

	bool bUnpackBool(const BYTE* pucBuf, int& nOffs) const
	{
		return (bool)ucUnpack(pucBuf,nOffs);
}


	WORD wUnpack(const BYTE* pucBuf, int& nOffs) const
	{
		WORD w;
		memcpy(&w,&pucBuf[nOffs],sizeof(w));
		nOffs+=sizeof(w);
		return w;
	}

	DWORD dwUnpack(const BYTE* pucBuf, int& nOffs) const
	{
		DWORD dw;
		memcpy(&dw,&pucBuf[nOffs],sizeof(dw));
		nOffs+=sizeof(dw);
		return dw;
	}

	int nUnpack(const BYTE* pucBuf, int& nOffs) const
	{
		int n;
		memcpy(&n,&pucBuf[nOffs],sizeof(n));
		nOffs+=sizeof(n);
		return n;
	}

//	float fUnpack(const BYTE* pucBuf, int& nOffs) const
//	{
//		float f;
//		memcpy(&f,&pucBuf[nOffs],sizeof(f));
//		nOffs+=sizeof(f);
//		return f;
//	}

	String sUnpack(const BYTE* pucBuf, int& nOffs) const
	{
		String s;
		WORD w=wUnpack(pucBuf,nOffs);
		if (w>0)
			{
			char* psz=new char[w+1];
			memcpy(psz,&pucBuf[nOffs],w);
			psz[w]=0;
			s=String(psz);
			delete[] psz;
			nOffs+=(int)w;
			}
		else
			s=L"";
		return s;
	}

	LatLong UnpackLL(const BYTE* pucBuf, int& nOffs)
	{
		LatLong LL;
		memcpy(&LL,&pucBuf[nOffs],sizeof(LL));
		nOffs+=sizeof(LL);
      return LL;
	}

	Cst4LatLong UnpackCst4LL(const BYTE* pucBuf, int& nOffs)
	{
		Cst4LatLong LL;
		memcpy(&LL,&pucBuf[nOffs],sizeof(LL));
		nOffs+=sizeof(LL);
      return LL;
	}


	void PackData(const BYTE* pucData, const WORD wSize, BYTE* pucBuf, int& nOffs)
	{
		PackWORD(wSize,pucBuf,nOffs);
		if (wSize>0)
			{
			memcpy(&pucBuf[nOffs],pucData,wSize);
			nOffs+=(int)wSize;
			}
	}

	WORD wUnpackData(BYTE* pucData, const BYTE* pucBuf, int& nOffs)
	{
		WORD wSize=wUnpack(pucBuf,nOffs);
		if (wSize>0)
			{
			memcpy(pucData,&pucBuf[nOffs],wSize);
			nOffs+=(int)wSize;
			}
		return wSize;
	}



};


class Cst4_MessageBase : public Cst4_MessageUtils
{

public:

	enum
	{
		CST4_TYPE_OFFS             = 0,
		CST4_TYPE_MASK             = 0x000000FF,
		CST4_STAT_SIZE_OFFS        = 8,
		CST4_STAT_SIZE_MASK        = 0x0FFFFF00,  //before shift
		CST4_AIS_TX_AVAIL      		= 0x10000000,
		CST4_STAT_MSG_CST3         = 0x20000000,
		CST4_AIS_TX_ON         		= 0x40000000,
//		CST4_STAT_MSG_COMPRESSED   = 0x40000000,
		CST4_STAT_MSG_VALID        = 0x80000000,

	};

private:

	DWORD dwStatus;
	DWORD dwDataSources;
	Cst4Time T;

	enum
	{
		CST4_SOURCE_MASK        = 0x0000FFFF,  //after shift
		CST4_SOURCE_SHIFT       = 0,
		CST4_SOURCE_CLEAR       = 0xFFFF0000,

		CST4_SERVERINDEX_MASK   = 0x0000FFFF,  //after shift !!!
		CST4_SERVERINDEX_SHIFT  = 16,
		CST4_SERVERINDEX_CLEAR  = 0x0000FFFF,  //after shift !!!
	};


protected:

	DWORD dwGetBaseSize(const bool bSkipStatus=false) const
	{
		if (bSkipStatus)
			return sizeof(dwDataSources)+T.dwGetSize();
		else
			return sizeof(dwStatus)+sizeof(dwDataSources)+T.dwGetSize();
	}



	void Init(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		dwDataSources=0;
		dwStatus=_MsgType;
		T.Invalidate();
	}



public:

	void Invalidate()
	{
		dwStatus&=(~CST4_STAT_MSG_VALID);
	}

	void SetValid()
	{
		dwStatus|=CST4_STAT_MSG_VALID;
	}

	bool bValid() const
	{
		return (dwStatus&CST4_STAT_MSG_VALID)!=0;
	}

	Cst4_MessageBase(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		Init(_MsgType);
	}

	void Assign(const Cst4_MessageBase& M)
	{
		dwStatus=M.dwStatus;
		dwDataSources=M.dwDataSources;
		T=M.T;
	}

	Cst4_MessageBase(const Cst4_MessageBase& M)
	{
		Assign(M);
	}

	~Cst4_MessageBase()
	{
	}

	Cst4_MessageBase& operator = (const Cst4_MessageBase& M)
	{
		Assign(M);
		return *this;
	}

	bool operator < (const Cst4_MessageBase& M) const
	{
		return (T.dSec_raw()<M.T.dSec_raw());
	}

	bool operator > (const Cst4_MessageBase& M) const
	{
		return (T.dSec_raw()>M.T.dSec_raw());
	}



	void SetSource(const WORD wSrc)
	{
		dwDataSources&=CST4_SOURCE_CLEAR;
		dwDataSources|=(wSrc<<CST4_SOURCE_SHIFT);
	}

	WORD wSource() const
	{
		return (WORD)((dwDataSources>>CST4_SOURCE_SHIFT)&CST4_SOURCE_MASK);
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


	void SetSize(const DWORD dwSize)   //Size <= 1048576 bytes
	{
		dwStatus&=(~CST4_STAT_SIZE_MASK);
		dwStatus|=(dwSize<<CST4_STAT_SIZE_OFFS);
	}

	DWORD dwSize() const
	{
		return (dwStatus&CST4_STAT_SIZE_MASK)>>CST4_STAT_SIZE_OFFS;
	}

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
		T.SetTime(JTime(dTime_sec_utc,true));
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

	double dTimeDiff_sec(const Cst4_MessageBase& M) const
	{
		return fabs(T.dSec_raw()-M.T.dSec_raw());
	}

	void PackBase(BYTE* pucBuf, int& nOffs)
	{
		PackDWORD(dwStatus,pucBuf,nOffs);
		PackDWORD(dwDataSources,pucBuf,nOffs);
		T.Pack(pucBuf,nOffs);
	}

	bool bUnpackBase(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
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

	DWORD dwPayloadSize(const bool bSkipStatus) const
	{
		return ((dwStatus&CST4_STAT_SIZE_MASK)>>CST4_STAT_SIZE_OFFS)-dwGetBaseSize(bSkipStatus);
	}


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

	JFile::FILE_ERROR Extract(JFile* pF);
String sExtract();


};

//***************************************************************************

class Cst4_AIS_Dynamic : public Cst4_MessageBase
{
   int nMMSI;
   Cst4LatLong LL;
	WORD wSpeed;
	WORD wCourse;

public:

	void InvalidateSpeedCourse()
	{
		wSpeed=0xFFFF;
		wCourse=0xFFFF;
	}

	bool bSpeedCourseValid() const
	{
		return !((wSpeed&wCourse)==0xFFFF);
	}

	void SetSpeed(const double dSpeed_kts)
	{
      wSpeed=dSpeed_kts*10.0+0.5;
   }

   double dSpeed_kts() const
	{
		return wSpeed/10.0;
	}

	void SetCourse(const double dCourse_deg)
	{
		wCourse=dCourse_deg*65536.0/360.0;
	}

	double dCourse_deg() const
	{
		return wCourse*360.0/65536.0;
	}

	void Set(const JTime& _T, const int _nMMSI, const LatLong& _LL, const double dSpeed_kts, const double dCourse_deg, const WORD _wSourceID)
	{
		SetTime(_T);
		nMMSI=_nMMSI;
		LL=Cst4LatLong(_LL);
		SetSpeed(dSpeed_kts);
		SetCourse(dCourse_deg);
      SetSource(_wSourceID);
      SetValid();
   }

   Cst4_AIS_Dynamic(const JTime& _T, const int _nMMSI, const LatLong& _LL, const double dSpeed_kts, const double dCourse_deg, const WORD _wSourceID) : Cst4_MessageBase(CST4_DYNAMIC_AIS_MSG)
	{
      Set(_T,_nMMSI,_LL,dSpeed_kts,dCourse_deg,_wSourceID);
   }

   Cst4_AIS_Dynamic() : Cst4_MessageBase(CST4_DYNAMIC_AIS_MSG)
	{
	}

   Cst4_AIS_Dynamic(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_DYNAMIC_AIS_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
   }

   ~Cst4_AIS_Dynamic()
   {

   }

   void Assign(const Cst4_AIS_Dynamic& M)
   {
      Cst4_MessageBase::Assign(M);
      nMMSI=M.nMMSI;
      LL=M.LL;
      wSpeed=M.wSpeed;
      wCourse=M.wCourse;
   }

   Cst4_AIS_Dynamic& operator = (const Cst4_AIS_Dynamic& M)
   {
      Assign(M);
      return *this;
   }

   Cst4_AIS_Dynamic(const Cst4_AIS_Dynamic& M)
   {
      Assign(M);
      nMMSI=M.nMMSI;
		LL=M.LL;
      wSpeed=M.wSpeed;
      wCourse=M.wCourse;
   }

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(nMMSI)+sizeof(LL)+sizeof(wSpeed)+sizeof(wCourse);
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackInt(nMMSI,pucBuf,nOffs);
		PackWORD(wSpeed,pucBuf,nOffs);
		PackWORD(wCourse,pucBuf,nOffs);
      LL.Pack(pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		nMMSI=nUnpack(pucBuf,nOffs);
		wSpeed=wUnpack(pucBuf,nOffs);
		wCourse=wUnpack(pucBuf,nOffs);
		LL.Unpack(pucBuf,nOffs);
		return true;
   };

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetMMSI(const int _nMMSI)
	{
		nMMSI=_nMMSI;
	}

   int nGetMMSI() const
   {
      return nMMSI;
   }

   LatLong GetLL() const
   {
      return LL.GetLL();
   }

   Cst4LatLong GetLL4() const
   {
      return LL;
	}

	void SetLL(const LatLong& _LL)
	{
		LL.SetLL(_LL);
	}

   bool bDuplicate(Cst4_AIS_Dynamic* pD, const double dMaxTimeDiff_sec)
   {
      if (pD->wSource()==wSource())   //Must be different sources
         return false;
      if (pD->nGetMMSI()!=nGetMMSI())
         return false;
      if (pD->GetLL4()!=LL)
         return false;
      if (fabs(pD->dSpeed_kts()-dSpeed_kts())>0.1)
         return false;
		if (fabs(pD->dCourse_deg()-dCourse_deg())>0.1)
			return false;
		if (dTimeDiff_sec(*pD)>dMaxTimeDiff_sec)
         return false;
      return true;
   }

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();


};

//***************************************************************************

class Cst4_AIS_Static : public Cst4_MessageBase
{

   int nMMSI;
   WORD wStaticSize;
   BYTE* pucStaticData;

public:

   void Assign(const Cst4_AIS_Static& M)
   {
      Cst4_MessageBase::Assign(M);
      nMMSI=M.nMMSI;
      wStaticSize=M.wStaticSize;
		delete[] pucStaticData;
		if (wStaticSize>0)
			{
			pucStaticData=new BYTE[wStaticSize];
			memcpy(pucStaticData,M.pucStaticData,wStaticSize);
			}
		else
			pucStaticData=NULL;

	}

	void Set(const int _nMMSI, const BYTE* _pucData, const WORD _wSize, const WORD _wSourceID)
	{
		nMMSI=_nMMSI;
		wStaticSize=_wSize;
      delete[] pucStaticData;
      if (wStaticSize>0)
         {
			pucStaticData=new BYTE[wStaticSize];
			memcpy(pucStaticData,_pucData,wStaticSize);
			}
		else
			pucStaticData=NULL;
		SetSource(_wSourceID);
		SetValid();
	}

   void SetCst3Info( const int _nMMSI, const int nIMO, const String sName, const String sCallsign, const int nType,
                     const WORD _wSourceID, const BYTE* pucExtra=NULL, const WORD wExtraSize=0)
   {
      SetCst3Msg();
      nMMSI=_nMMSI;
      wStaticSize=sizeof(nIMO)+sizeof(nType)+sizeof(WORD)*3+sName.Length()+sCallsign.Length()+wExtraSize;
		delete[] pucStaticData;
		pucStaticData=new BYTE[wStaticSize];
		int nOffs=0;
		PackInt(nIMO,pucStaticData,nOffs);
		PackInt(nType,pucStaticData,nOffs);
		PackString(sName,pucStaticData,nOffs);
		PackString(sCallsign,pucStaticData,nOffs);
		PackWORD(wExtraSize,pucStaticData,nOffs);
		if (wExtraSize>0)
			memcpy(&pucStaticData[nOffs],pucExtra,wExtraSize);
		SetSource(_wSourceID);
		SetValid();
	}

	Cst4_AIS_Static(const int _nMMSI, const BYTE* _pucData, const WORD _wSize, const WORD _wSourceID) : Cst4_MessageBase(CST4_STATIC_AIS_MSG)
	{
		pucStaticData=NULL;
		Set(_nMMSI,_pucData,_wSize,_wSourceID);
	}

	Cst4_AIS_Static(const int _nMMSI, const int nIMO, const String sName, const String sCallsign, const int nType,
						 const WORD _wSourceID, const BYTE* pucExtra=NULL, const WORD wExtraSize=0) : Cst4_MessageBase(CST4_STATIC_AIS_MSG)
	{
		pucStaticData=NULL;
		SetCst3Info(_nMMSI,nIMO,sName,sCallsign,nType,_wSourceID,pucExtra,wExtraSize);
	}

	Cst4_AIS_Static() : Cst4_MessageBase(CST4_STATIC_AIS_MSG)
	{
		pucStaticData=NULL;
		wStaticSize=0;
	}

	Cst4_AIS_Static(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_STATIC_AIS_MSG)
	{
		int nOffs=0;
		pucStaticData=NULL;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

	~Cst4_AIS_Static()
	{
		delete[] pucStaticData;
	}

	Cst4_AIS_Static& operator = (const Cst4_AIS_Static& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_AIS_Static(const Cst4_AIS_Static& M)
	{
		pucStaticData=NULL;
		Assign(M);
	}

	bool operator < (const Cst4_AIS_Static& M)
	{
		return nMMSI<M.nMMSI;
	}

	bool operator > (const Cst4_AIS_Static& M)
	{
		return nMMSI>M.nMMSI;
	}

	int nGetMMSI() const
	{
		return nMMSI;
	}

	WORD wGetDataSize() const
	{
		return wStaticSize;
	}

	BYTE* pucGetData() const
	{
		return pucStaticData;
	}

	void GetData(BYTE* _pucData, const WORD wMaxBytes)
	{
		if ((wStaticSize>0)&&(wStaticSize<=wMaxBytes))
			memcpy(_pucData,pucStaticData,wStaticSize);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(nMMSI)+sizeof(wStaticSize)+wStaticSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackInt(nMMSI,pucBuf,nOffs);
		PackData(pucStaticData,wStaticSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		delete[] pucStaticData;
		pucStaticData=NULL;
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		nMMSI=nUnpack(pucBuf,nOffs);
		wStaticSize=wUnpack(pucBuf,nOffs);
		if (wStaticSize>0)
			{
			if (nOffs+(int)wStaticSize<=nMaxSize)
				{
				pucStaticData=new BYTE[wStaticSize];
				memcpy(pucStaticData,&pucBuf[nOffs],wStaticSize);
				nOffs+=(int)wStaticSize;
				}
			else
				return false;
			}
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
      return nOffs;
   }

	void Invalidate()
	{
		nMMSI=-1;
	}

	bool bValid() const
	{
		return (nMMSI>0)&&(wStaticSize>0);
	}

   String sShipName()
	{
		AISMsgClass AISMsg;
		if (wStaticSize>0)
			{
			AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
			if (AISMsg.bShipNameValid())
				return AISMsg.sGetShipName();
			}
		return "";
	}

	int nShipType()
	{
		AISMsgClass AISMsg;
		AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
		if (AISMsg.bShipAndCargoTypeValid())
			return AISMsg.nGetShipAndCargoType();
		else
			return 0;
	}

	bool bSART()
	{
		AISMsgClass AISMsg;
		if (wStaticSize>0)
			{
			AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
			return (AISMsg.GetNavStatus()==AIS_NAVSTATUS_AIS_SART);
			}
		return false;
	}

	bool bOwnShip()
	{
		AISMsgClass AISMsg;
		if (wStaticSize>0)
			{
			AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
			return AISMsg.bIsOwnMsg();
			}
		return false;
	}

	void SetOwnShip()
	{
		AISMsgClass AISMsg;
		if (wStaticSize>0)
			{
			AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
         AISMsg.SetOwnMsg();
   		wStaticSize=AISMsg.dwGetExtraInfoBuf(pucStaticData,wStaticSize);
			}
	}

	String sDestination()
	{
		AISMsgClass AISMsg;
		AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
		if (AISMsg.bDestinationValid())
			return AISMsg.sGetDestination();
		else
			return "";
	}

   int nIMO()
   {
      AISMsgClass AISMsg;
      AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
      if (AISMsg.bIMOValid())
         return AISMsg.nGetIMO();
      else
         return -1;
   }


	int nGetCst3Info(int& nIMO, String& sName, String& sCallsign, int& nType, BYTE* pucExtra=NULL, const WORD wMaxExtraSize=0) const
   {
      int nOffs=0;
      nIMO=nUnpack(pucStaticData,nOffs);
      nType=nUnpack(pucStaticData,nOffs);
      sName=sUnpack(pucStaticData,nOffs);
      sCallsign=sUnpack(pucStaticData,nOffs);
      if (pucExtra)
         {
         WORD wExtraSize=wUnpack(pucStaticData,nOffs);
         if (wExtraSize>wMaxExtraSize)
            wExtraSize=0;
         if (wExtraSize>0)
            memcpy(pucExtra,&pucStaticData[nOffs],wExtraSize);
         return wExtraSize;
         }
      else
         return 0;
   }

	String sBasicInfo(int& nType, int& nIMO, String& sCallsign) const
	{
		AISMsgClass AISMsg;
		if (bCst3Msg())
			{
			BYTE aucExtra[1024];
			String sName;
			int nExtraSize=nGetCst3Info(nIMO,sName,sCallsign,nType,aucExtra,sizeof(aucExtra));
			if (nExtraSize>0)
				{
				AISMsg.SetExtraInfoBuf(aucExtra,nExtraSize);
				if (AISMsg.bShipAndCargoTypeValid())
					nType=AISMsg.nGetShipAndCargoType();
				else
					nType=0;
				}
			else if (nType==100)
				nType=0;
			return sName;
			}
		else
			{
			AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
			if (AISMsg.bShipAndCargoTypeValid())
				nType=AISMsg.nGetShipAndCargoType();
			else
				nType=0;
			if (AISMsg.bIMOValid())
				nIMO=AISMsg.nGetIMO();
			else
				nIMO=-1;
			if (AISMsg.bCallSignValid())
				sCallsign=AISMsg.sGetCallSign();
			else
				sCallsign=L"";
			if (AISMsg.bShipNameValid())
				return AISMsg.sGetShipName();
			else
				return "";
			}
	}

	String sBasicInfo(int& nType, int& nIMO) const
	{
		String sCallsign;
		return sBasicInfo(nType,nIMO,sCallsign);
}

	String sBasicInfo(int& nType) const
	{
		int nIMO;
		String sCallsign;
		return sBasicInfo(nType,nIMO,sCallsign);
	}

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};

//***************************************************************************

class Cst4_Radar_Scan : public Cst4_MessageBase
{
   Cst4LatLong RadarLL;
   JList<Cst4Plot> Plot;

public:

   void Init(const JTime& _T, const LatLong& _RadarLL, const WORD _wSourceID)
   {
      SetTime(_T);
      RadarLL=Cst4LatLong(_RadarLL);
      Plot.Clear();
      SetSource(_wSourceID);
      SetValid();
   }

   Cst4_Radar_Scan(const JTime& _T, const LatLong& _RadarLL, const WORD _wSourceID) : Cst4_MessageBase(CST4_RADAR_SCAN_MSG)
   {
      Init(_T,_RadarLL,_wSourceID);
   }

   Cst4_Radar_Scan() : Cst4_MessageBase(CST4_RADAR_SCAN_MSG)
	{
		RadarLL.Invalidate();
		Plot.Clear();
   }

	Cst4_Radar_Scan(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_RADAR_SCAN_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
   }


   ~Cst4_Radar_Scan()
   {
   }

   void Assign(const Cst4_Radar_Scan& M)
   {
      Cst4_MessageBase::Assign(M);
      Plot=M.Plot;
      RadarLL=M.RadarLL;
   }

	Cst4_Radar_Scan& operator = (const Cst4_Radar_Scan& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_Radar_Scan(const Cst4_Radar_Scan& M)
	{
      Assign(M);
   }

   LatLong GetRadarLL() const
   {
      return RadarLL.GetLL();
   }

   void AddPlot(const Cst4Plot& _p)
   {
      Plot.nAdd(new Cst4Plot(_p));
   }

	int nGetPlotCnt() const
   {
      return Plot.nGetCount();
   }

   Cst4Plot* pPlot(const int n)
   {
		if (!Plot.bPacked())
			Plot.Pack();
		return Plot[n];
	}

	void InvalidatePlot(const int n)
	{
		if (!Plot.bPacked())
			Plot.Pack();
		Plot[n]->Invalidate();
	}

	bool bPlotValid(const int n)
	{
		if (!Plot.bPacked())
			Plot.Pack();
      return Plot[n]->bValid();
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(int)+Plot.nGetCount()*sizeof(Cst4Plot)+sizeof(RadarLL);
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		RadarLL.Pack(pucBuf,nOffs);
		Plot.Pack();
		PackInt(Plot.nGetCount(),pucBuf,nOffs);
		for (int i=0; i<Plot.nGetCount(); i++)
			{
			memcpy(&pucBuf[nOffs],Plot[i],sizeof(Cst4Plot));
			nOffs+=sizeof(Cst4Plot);
			}
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		RadarLL.Unpack(pucBuf,nOffs);
		int nCnt=nUnpack(pucBuf,nOffs);
		Plot.Clear();
		for (int i=0; i<nCnt; i++)
			{
			Cst4Plot* p=new Cst4Plot;
			memcpy(p,&pucBuf[nOffs],sizeof(Cst4Plot));
			Plot.nAdd(p);
			nOffs+=sizeof(Cst4Plot);
			}
		Plot.Pack();
		return true;
   }

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
      return nOffs;
   }

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};

//***************************************************************************

class Cst4_Radar_Track : public Cst4_MessageBase
{

	enum
	{
		CST4_TRACK_ID_OFFS         = 0,
		CST4_TRACK_ID_MASK         = 0x003FFFFF,

      CST4_TRACK_SAR_MASK        = 0x00400000,
      CST4_TRACK_CYCLOPE_MASK    = 0x00800000,

		CST4_TRACK_QUALITY_OFFS    = 24,
		CST4_TRACK_QUALITY_MASK    = 0x7F000000,

		CST4_TRACK_ASSOCIATED		= 0x80000000,
	};

	DWORD dwID;
	Cst4LatLong LL;
	WORD wSpeed;
	WORD wCourse;

public:

	void InvalidateSpeedCourse()
	{
		wSpeed=0xFFFF;
		wCourse=0xFFFF;
	}

	bool bSpeedCourseValid() const
	{
		return !((wSpeed&wCourse)==0xFFFF);
	}

   int nMaskID(const int nID) const
   {
   	return nID&CST4_TRACK_ID_MASK;
   }

	void SetID(const int nID)
	{
		dwID&=(~CST4_TRACK_ID_MASK);
		dwID|=(nID&CST4_TRACK_ID_MASK);
	}


   DWORD dwRawID() const
   {
      return dwID;
   }

	int nID() const
	{
		return (int)(dwID&CST4_TRACK_ID_MASK);
	}

	bool bValidID() const
	{
		return (dwID&CST4_TRACK_ID_MASK)<0x00FFFFFF;
	}

	void SetQuality(const BYTE ucQuality)
	{
		dwID&=(~CST4_TRACK_QUALITY_MASK);
		DWORD dw=ucQuality;
      dw&=0x0000001F;
		dw<<=CST4_TRACK_QUALITY_OFFS;
		dw&=CST4_TRACK_QUALITY_MASK;
		dwID|=dw;
	}

	void SetCyclope(const bool bCycl=true)
	{
		if (bCycl)
			dwID|=CST4_TRACK_CYCLOPE_MASK;	//(1<<CST4_CYCLOPE);
		else
			{
//			DWORD dw=1<<CST4_CYCLOPE;
//			dwID&=(~dw);
			dwID&=~CST4_TRACK_CYCLOPE_MASK;
			}
	}

	bool bCyclope() const
	{
//		return (((dwID>>CST4_CYCLOPE)&0x00000001)!=0);
		return ((dwID&CST4_TRACK_CYCLOPE_MASK)!=0);
	}

	void SetSAR(const bool bSAR=true)
	{
		if (bSAR)
//			dwID|=(1<<CST4_SAR);
			dwID|=CST4_TRACK_SAR_MASK;
		else
			{
//			DWORD dw=1<<CST4_SAR;
//			dwID&=(~dw);
			dwID&=(~CST4_TRACK_SAR_MASK);
			}
	}

	bool bSAR() const
	{
//		return (((dwID>>CST4_SAR)&0x00000001)!=0);
		return ((dwID&CST4_TRACK_SAR_MASK)!=0);
	}

	BYTE ucQuality() const
	{
		return (BYTE)((dwID&CST4_TRACK_QUALITY_MASK)>>CST4_TRACK_QUALITY_OFFS);
	}

	void SetAssociated(const bool bAssoc=true)
	{
		if (bAssoc)
			dwID|=CST4_TRACK_ASSOCIATED;
		else
			dwID&=(~CST4_TRACK_ASSOCIATED);
	}

	bool bAssociated() const
	{
		return (dwID&CST4_TRACK_ASSOCIATED)!=0;
	}

	void SetSpeed(const double dSpeed_kts)
	{
		if (dSpeed_kts>=0.0)
			wSpeed=dSpeed_kts*10.0+0.5;
		else
			wSpeed=0xFFFF;
	}

	double dSpeed_kts() const
	{
		if (wSpeed==0xFFFF)
			return -1.0;
		else
			return wSpeed/10.0;
	}

	void SetCourse(const double dCourse_deg)
	{
		wCourse=dCourse_deg*65536.0/360.0;
	}

	double dCourse_deg() const
	{
		return wCourse*360.0/65536.0;
	}

	void Set(const JTime& _T, const int _nID, const LatLong& _LL, const double dSpeed_kts, const double dCourse_deg, const BYTE _ucQuality, const WORD _wSourceID)
	{
      dwID=0;
		SetTime(_T);
		SetID(_nID);
		SetQuality(_ucQuality);
		LL=Cst4LatLong(_LL);
		SetSpeed(dSpeed_kts);
		SetCourse(dCourse_deg);
		SetSource(_wSourceID);
		SetValid();
	}

	Cst4_Radar_Track(const JTime& _T, const int _nID, const LatLong& _LL, const double dSpeed_kts, const double dCourse_deg, const BYTE _ucQuality, const WORD _wSourceID) : Cst4_MessageBase(CST4_RADAR_TRACK_MSG)
	{
      dwID=0;
		Set(_T,_nID,_LL,dSpeed_kts,dCourse_deg,_ucQuality,_wSourceID);
	}

	Cst4_Radar_Track() : Cst4_MessageBase(CST4_RADAR_TRACK_MSG)
	{
      dwID=0;
		Invalidate();
	}

	Cst4_Radar_Track(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_RADAR_TRACK_MSG)
	{
      dwID=0;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

	~Cst4_Radar_Track()
	{

	}

	void Assign(const Cst4_Radar_Track& M)
	{
		Cst4_MessageBase::Assign(M);
		dwID=M.dwID;
		LL=M.LL;
		wSpeed=M.wSpeed;
		wCourse=M.wCourse;
	}

	Cst4_Radar_Track& operator = (const Cst4_Radar_Track& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_Radar_Track(const Cst4_Radar_Track& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(dwID)+sizeof(LL)+sizeof(wSpeed)+sizeof(wCourse);
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackDWORD(dwID,pucBuf,nOffs);
		PackWORD(wSpeed,pucBuf,nOffs);
		PackWORD(wCourse,pucBuf,nOffs);
		LL.Pack(pucBuf,nOffs);
	}

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		dwID=dwUnpack(pucBuf,nOffs);
		wSpeed=wUnpack(pucBuf,nOffs);
		wCourse=wUnpack(pucBuf,nOffs);
		LL.Unpack(pucBuf,nOffs);
		return true;
	}

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetLL(const LatLong& _LL)
	{
		LL.SetLL(_LL);
	}

	LatLong GetLL() const
	{
		return LL.GetLL();
	}

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};
//***************************************************************************

/*
class Cst4_Radar_TrackInfo : public Cst4_MessageBase
{
	int nTrackID;
	int nMMSI;
	int nIMO;
	String sName;
	String sCallsign;
	String sInfo;

public:

	Cst4_Radar_TrackInfo() : Cst4_MessageBase(CST4_RADAR_TRACK_INFO_MSG)
	{
		SetSource(0);
		nTrackID=0;
		nMMSI=-1;
		nIMO=-1;
		sName="";
		sCallsign="";
		sInfo="";
	}

	Cst4_Radar_TrackInfo(const WORD wSrc, const int _nID) : Cst4_MessageBase(CST4_RADAR_TRACK_INFO_MSG)
	{
		SetSource(wSrc);
		nTrackID=_nID;
		nMMSI=-1;
		nIMO=-1;
		sName="";
		sCallsign="";
		sInfo="";
	}

	void Assign(const Cst4_Radar_TrackInfo& I)
	{
		Cst4_MessageBase::Assign(I);
		nTrackID=I.nTrackID;
		nMMSI=I.nMMSI;
		nIMO=I.nIMO;
		sName=I.sName;
		sCallsign=I.sCallsign;
		sInfo=I.sInfo;
	}

	Cst4_Radar_TrackInfo(const Cst4_Radar_TrackInfo& I) : Cst4_MessageBase(CST4_RADAR_TRACK_INFO_MSG)
	{
		Assign(I);
	}

	Cst4_Radar_TrackInfo& operator = (const Cst4_Radar_TrackInfo& I)
	{
		Assign(I);
		return *this;
	}

	~Cst4_Radar_TrackInfo()
	{
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()
					+sizeof(nTrackID)
					+sizeof(nMMSI)
					+sizeof(nIMO)
					+sizeof(WORD)+AnsiString(sName).Length()
					+sizeof(WORD)+AnsiString(sCallsign).Length()
					+sizeof(WORD)+AnsiString(sInfo).Length();
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackInt(nTrackID,pucBuf,nOffs);
		PackInt(nMMSI,pucBuf,nOffs);
		PackInt(nIMO,pucBuf,nOffs);
		PackString(sName,pucBuf,nOffs);
		PackString(sCallsign,pucBuf,nOffs);
		PackString(sInfo,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,false))
			return false;
		nTrackID=nUnpack(pucBuf,nOffs);
		nMMSI=nUnpack(pucBuf,nOffs);
		nIMO=nUnpack(pucBuf,nOffs);
		sName=sUnpack(pucBuf,nOffs);
		sCallsign=sUnpack(pucBuf,nOffs);
		sInfo=sUnpack(pucBuf,nOffs);
		return true;
	};

	int nID() const						{return nTrackID;	}
	void SetID(const int nID)			{nTrackID=nID;		}

	int nGetMMSI() const 				{return nMMSI;		}
	void SetMMSI(const int n) 			{nMMSI=n;			}

	int nGetIMO() const					{return nIMO;		}
	void SetIMO(const int n)			{nIMO=n;				}

	String sGetName() const 			{return sName;		}
	void SetName(const String s)		{sName=s;			}

	String sGetCallsign() const		{return sCallsign;}
	void SetCallsign(const String s)	{sCallsign=s;		}

	String sGetInfo() const 			{return sInfo; 	}
	void SetInfo(const String s)		{sInfo=s; 			}

};

*/

//***************************************************************************

enum
{
	IFF_TRACK_SEX           = 0,
	IFF_TRACK_CANCEL_SEX    = 1,
	IFF_MRR_ASSOC_TRACK     = 2,
	IFF_ADSB                = 3,
};

class Cst4_IFF_Track : public Cst4_MessageBase
{

	BYTE ucType;
	Cst4LatLong LL;
	WORD wTgt_Track;
	WORD wTgt_ID;
	int nTgtHgt_ft;

public:

	void SetMsgType(const CST4_MSG_TYPE _MsgType)
	{
		Init(_MsgType);
	}

	void SetID(const WORD wID)
	{
		wTgt_ID=wID;
	}

	WORD wID() const
	{
		return wTgt_ID;
	}

	void SetTrack(const WORD wTrack)
	{
		wTgt_Track=wTrack;
	}

	WORD wTrack() const
	{
		return wTgt_Track;
	}

	void SetHeight(const int _nHeight_ft)
	{
		nTgtHgt_ft=_nHeight_ft;
	}

	int nHeight_ft() const
	{
		return nTgtHgt_ft;
	}

	double dHeight_m() const
	{
		return nTgtHgt_ft*0.0254*12.0;
	}

	BYTE ucGetType() const
	{
		return ucType;
	}

	bool bCancelled() const
	{
		return (ucType==IFF_TRACK_CANCEL_SEX);
	}

	void SetADSBType()
	{
		ucType=IFF_ADSB;
	}

	void SetIFFType()
	{
		ucType=IFF_TRACK_SEX;
	}

	bool bADSBType() const
	{
		return (ucType==IFF_ADSB);
	}

	void Set(const JTime& _T, const BYTE _ucType, const WORD _wID, const LatLong& _LL, const WORD _wTrack, const int _nHeight_ft, const WORD _wSourceID)
	{
		SetTime(_T);
		ucType=_ucType;
		SetID(_wID);
		SetTrack(_wTrack);
		LL=Cst4LatLong(_LL);
		SetHeight(_nHeight_ft);
		SetSource(_wSourceID);
		SetValid();
	}

	Cst4_IFF_Track(const JTime& _T, const BYTE _ucType, const WORD _wID, const LatLong& _LL, const WORD _wTrack, const int _nHeight_ft, const WORD _wSourceID) : Cst4_MessageBase(CST4_IFF_TRACK_MSG)
	{
		Set(_T,_ucType,_wID,_LL,_wTrack,_nHeight_ft,_wSourceID);
	}

	Cst4_IFF_Track() : Cst4_MessageBase(CST4_IFF_TRACK_MSG)
	{
	}

	Cst4_IFF_Track(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_IFF_TRACK_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

	~Cst4_IFF_Track()
	{

	}

	void Assign(const Cst4_IFF_Track& M)
	{
		Cst4_MessageBase::Assign(M);
		ucType=M.ucType;
		LL=M.LL;
		wTgt_Track=M.wTgt_Track;
		wTgt_ID=M.wTgt_ID;
		nTgtHgt_ft=M.nTgtHgt_ft;
	}

	Cst4_IFF_Track& operator = (const Cst4_IFF_Track& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_IFF_Track(const Cst4_IFF_Track& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(ucType)+sizeof(LL)+sizeof(wTgt_Track)+sizeof(wTgt_ID)+sizeof(nTgtHgt_ft);
	}

	void Pack(BYTE* pucBuf, int& nOffs, const bool bSetSize=true)
	{
		if (bSetSize)
			SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackBYTE(ucType,pucBuf,nOffs);
		PackWORD(wTgt_Track,pucBuf,nOffs);
		PackWORD(wTgt_ID,pucBuf,nOffs);
		PackInt(nTgtHgt_ft,pucBuf,nOffs);
		LL.Pack(pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		ucType=ucUnpack(pucBuf,nOffs);
		wTgt_Track=wUnpack(pucBuf,nOffs);
		wTgt_ID=wUnpack(pucBuf,nOffs);
		nTgtHgt_ft=nUnpack(pucBuf,nOffs);
		LL.Unpack(pucBuf,nOffs);
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetLL(const LatLong& _LL)
	{
		LL=Cst4LatLong(_LL);
	}

	LatLong GetLL() const
	{
		return LL.GetLL();
	}

	int nPack_DPSS(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='D';
		pucBuf[nOffs++]='P';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='S';
		CstTime().Pack(pucBuf,nOffs);
		PackBYTE(ucType,pucBuf,nOffs);
		PackWORD(wTgt_Track,pucBuf,nOffs);
		PackWORD(wTgt_ID,pucBuf,nOffs);
		PackInt(nTgtHgt_ft,pucBuf,nOffs);
		LL.Pack(pucBuf,nOffs);
		return nOffs;
	}

	void Unpack_DPSS(const BYTE* pucBuf, int& nOffs)
	{
		if (pucBuf[nOffs++]!='D')
			return;
		if (pucBuf[nOffs++]!='P')
			return;
		if (pucBuf[nOffs++]!='S')
			return;
		if (pucBuf[nOffs++]!='S')
			return;
		Cst4Time T1;
		T1.Unpack(pucBuf,nOffs);
		SetTime(T1);
		ucType=ucUnpack(pucBuf,nOffs);
		wTgt_Track=wUnpack(pucBuf,nOffs);
		wTgt_ID=wUnpack(pucBuf,nOffs);
		nTgtHgt_ft=nUnpack(pucBuf,nOffs);
		LL.Unpack(pucBuf,nOffs);
	};

	int nPack_RRS(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='R';
		pucBuf[nOffs++]='R';
		pucBuf[nOffs++]='S';
		CstTime().Pack(pucBuf,nOffs);
		PackBYTE(ucType,pucBuf,nOffs);
		PackWORD(wTgt_Track,pucBuf,nOffs);
		PackWORD(wTgt_ID,pucBuf,nOffs);
		PackInt(nTgtHgt_ft,pucBuf,nOffs);
		LL.Pack(pucBuf,nOffs);
		return nOffs;
	}

	void Unpack_RRS(const BYTE* pucBuf, int& nOffs)
	{
		if (pucBuf[nOffs++]!='R')
			return;
		if (pucBuf[nOffs++]!='R')
			return;
		if (pucBuf[nOffs++]!='S')
			return;
		Cst4Time T1;
		T1.Unpack(pucBuf,nOffs);
		SetTime(T1);
		ucType=ucUnpack(pucBuf,nOffs);
		wTgt_Track=wUnpack(pucBuf,nOffs);
		wTgt_ID=wUnpack(pucBuf,nOffs);
		nTgtHgt_ft=nUnpack(pucBuf,nOffs);
		LL.Unpack(pucBuf,nOffs);
	};

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};

//***************************************************************************

class Cst4_ADSB : public Cst4_IFF_Track
{

	int nFlightID;
	String sCallsign;
	WORD wGroundSpeed_kts;
	WORD wCourse;

public:

	void SetFlightID(const int _nID)
	{
		nFlightID=_nID;
	}

	int nGetFlightID() const
	{
		return nFlightID;
	}

	void SetCallsign(const String _sCallsign)
	{
		sCallsign=_sCallsign;
	}

	String sGetCallsign() const
	{
		return sCallsign;
	}

	void SetSpeed_kts(const int nSpeed)
	{
		wGroundSpeed_kts=(WORD)nSpeed;
	}

	int nGetSpeed_kts() const
	{
		return (int)wGroundSpeed_kts;
	}

	void SetCourse_deg(const double dC_deg)
	{
		wCourse=dC_deg*10;
	}

	double dGetCourse_deg() const
	{
		return wCourse*0.1;
	}

	void Set(const JTime& _T,
				const BYTE _ucType,
				const WORD _wID,
				const LatLong& _LL,
				const WORD _wTrack,
				const int _nHeight_ft,
				const int _nFightID,
				const String _sCallsign,
				const int  nSpeed_kts,
				const double dCourse_deg,
				const WORD _wSourceID)
	{
		Cst4_IFF_Track::Set(_T,_ucType,_wID,_LL,_wTrack,_nHeight_ft,_wSourceID);
		SetFlightID(_nFightID);
		SetCallsign(_sCallsign);
		SetSpeed_kts(nSpeed_kts);
		SetCourse_deg(dCourse_deg);
	}

	Cst4_ADSB(	const JTime& _T,
					const BYTE _ucType,
					const WORD _wID,
					const LatLong& _LL,
					const WORD _wTrack,
					const int _nHeight_ft,
					const int _nFightID,
					const String _sCallsign,
					const int  _nSpeed_kts,
					const double _dCourse_deg,
					const WORD _wSourceID)// : Cst4_MessageBase(CST4_ADSB_MSG)
	{
		Init(VISTA_ADSB_MSG);
		Set(_T,_ucType,_wID,_LL,_wTrack,_nHeight_ft,_nFightID,_sCallsign,_nSpeed_kts,_dCourse_deg,_wSourceID);
	}

	Cst4_ADSB()
	{
		Init(VISTA_ADSB_MSG);
	}

	Cst4_ADSB(const BYTE* pucBuf, const int nMaxSize)
	{
		Init(VISTA_ADSB_MSG);
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

	~Cst4_ADSB()
	{

	}

	void SetFromIFF(const Cst4_IFF_Track& T)
	{
		Cst4_IFF_Track::Assign(T);
		SetIFFType();
	}

	Cst4_IFF_Track GetIFF()
	{
		Cst4_IFF_Track IFF;
		IFF.Assign(*this);
		return IFF;
	}

	void Assign(const Cst4_ADSB& M)
	{
		Cst4_IFF_Track::Assign(M);
		nFlightID=M.nFlightID;
		sCallsign=M.sCallsign;
		wGroundSpeed_kts=M.wGroundSpeed_kts;
		wCourse=M.wCourse;
	}

	Cst4_ADSB& operator = (const Cst4_ADSB& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_ADSB(const Cst4_ADSB& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return Cst4_IFF_Track::dwCalcSize()
				+sizeof(nFlightID)
				+dwPackedStringSize(sCallsign)
				+sizeof(wGroundSpeed_kts)
				+sizeof(wCourse);
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		Cst4_IFF_Track::Pack(pucBuf,nOffs,false);   //Do not calculate size in IFF message
		PackInt(nFlightID,pucBuf,nOffs);
		PackString(sCallsign,pucBuf,nOffs);
		PackWORD(wGroundSpeed_kts,pucBuf,nOffs);
		PackWORD(wCourse,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		Cst4_IFF_Track::bUnpack(pucBuf,nOffs,nMaxSize,bSkipStatus);
		nFlightID=nUnpack(pucBuf,nOffs);
		sCallsign=sUnpack(pucBuf,nOffs);
		wGroundSpeed_kts=wUnpack(pucBuf,nOffs);
		wCourse=wUnpack(pucBuf,nOffs);
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};




//***************************************************************************

enum CST4_SHIP_CLASSIFICATION
{
  CST4_SHIP_UNKNOWN,// ( "Unknown" )
  CST4_SHIP_PENDING, //  "Pending")
  CST4_SHIP_FRIEND, //  "Friend")
  CST4_SHIP_NEUTRAL, //  "Neutral")
  CST4_SHIP_HOSTILE, //  "Hostile")
  CST4_SHIP_ASSUMED_FRIEND, //  "Assumed Friend")
  CST4_SHIP_SUSPECT, //  "Suspect")
  CST4_SHIP_EXERCISE_PENDING, //  "Exercise Pending")
  CST4_SHIP_EXERCISE_UNKNOWN, //  "Exercise Unknown")
  CST4_SHIP_EXERCISE_FRIEND, //  "Exercise Friend")
  CST4_SHIP_EXERCISE_NEUTRAL, //  "Exercise Neutral")
  CST4_SHIP_EXERCISE_ASSUMED_FRIEND, //  "Exercise Assumed Friend")
  CST4_SHIP_JOKER, //  "Joker")
  CST4_SHIP_FAKER, //  "Faker")
  CST4_SHIP_CLASS_COUNT,
};

class Cst4_SysTrack : public Cst4_MessageBase
{

public:

	enum
	{
		AIS_AVAIL			= 0x0001,
		RADAR_AVAIL			= 0x0002,
//		DESIGNATE_CAMERA	= 0x0004,
		UPLOAD_PICTURE		= 0x0008,
		PICTURE_AVAIL		= 0x0010,

		CLASS_MASK		= 0x000000FF,
		MSG_VERSION		= 1001,

      MAX_PICT_NUM	= 10,

	};

	enum SYSTRACK_ACTION
	{
		SYSTRACK_CREATE 	= 1,
		SYSTRACK_UPDATE 	= 2,
		SYSTRACK_DELETE 	= 3,
		SYSTRACK_DISTR		= 4,
	};

	struct OriginStruct
	{
		DWORD dwID;
		BYTE ucSeat;
		Cst4Time T_utc;

		void Init()
		{
			dwID=0;
			ucSeat=0;
			T_utc.Invalidate();
		}

		OriginStruct& operator = (const OriginStruct& D)
		{
			dwID=D.dwID;
			ucSeat=D.ucSeat;
			T_utc=D.T_utc;
			return *this;
		}

		bool bIdentical(const OriginStruct& D) const
		{
			if (dwID!=D.dwID)
				return false;
			if (ucSeat!=D.ucSeat)
				return false;
			if (!T_utc.bSame(D.T_utc))
				return false;
			return true;
		}


	};

	struct AISStruct : public Cst4_MessageUtils
	{
		int nMMSI;
		int nIMO;
		Cst4LatLong LL;
		WORD wSpeed;
		WORD wCourse;
		String sName;
		String sCallsign;
		int nType;
		BYTE ucNavStatus;
		WORD wLength;
		WORD wWidth;
		WORD wDraught;
		Cst4Time ETA_utc;
		String sDest;
		Cst4Time LastUpdate_utc;

		void Init()
		{
			nMMSI=-1;
			nIMO=-1;
			LL.Invalidate();
			wSpeed=0xFFFF;
			wCourse=0xFFFF;
			sName="";
			sCallsign="";
			nType=0;
			ucNavStatus=0;
			wLength=0;
			wWidth=0;
			wDraught=0;
			ETA_utc.Invalidate();
			sDest="";
			LastUpdate_utc.Invalidate();
		}

		AISStruct& operator = (const AISStruct& D)
		{
			nMMSI=D.nMMSI;
			nIMO=D.nIMO;
			LL=D.LL;
			wSpeed=D.wSpeed;
			wCourse=D.wCourse;
			sName=D.sName;
			sCallsign=D.sCallsign;
			nType=D.nType;
			ucNavStatus=D.ucNavStatus;
			wLength=D.wLength;
			wWidth=D.wWidth;
			wDraught=D.wDraught;
			ETA_utc=D.ETA_utc;
			sDest=D.sDest;
			LastUpdate_utc=D.LastUpdate_utc;
			return *this;
		}

		DWORD dwGetSize() const
		{

			return sizeof(nMMSI) 						//4
					+sizeof(nIMO)                    //4
					+sizeof(LL)                      //8
					+sizeof(wSpeed)                  //2
					+sizeof(wCourse)                 //2
					+dwPackedStringSize(sName)       //2+A1
					+dwPackedStringSize(sCallsign)   //2+A2
					+sizeof(nType)                   //4
					+sizeof(ucNavStatus)             //1
					+sizeof(wLength)                 //2
					+sizeof(wWidth)                  //2
					+sizeof(wDraught)                //2
					+ETA_utc.dwGetSize()             //6
					+dwPackedStringSize(sDest)      //2+A3
					+LastUpdate_utc.dwGetSize();     //2+A4
		}                                         //Total = 45+A1+A2+A3+A4

		void Pack(BYTE* pucBuf, int& nOffs)
		{
			int nOffs1=nOffs;
			PackInt(nMMSI,pucBuf,nOffs);
			PackInt(nIMO,pucBuf,nOffs);
			LL.Pack(pucBuf,nOffs);
			PackWORD(wSpeed,pucBuf,nOffs);
			PackWORD(wCourse,pucBuf,nOffs);
			PackString(sName,pucBuf,nOffs);
			PackString(sCallsign,pucBuf,nOffs);
			PackInt(nType,pucBuf,nOffs);
			PackBYTE(ucNavStatus,pucBuf,nOffs);
			PackWORD(wLength,pucBuf,nOffs);
			PackWORD(wWidth,pucBuf,nOffs);
			PackWORD(wDraught,pucBuf,nOffs);
			ETA_utc.Pack(pucBuf,nOffs);
			PackString(sDest,pucBuf,nOffs);
			LastUpdate_utc.Pack(pucBuf,nOffs);
			int nSize=nOffs-nOffs1;
			if (nSize!=(int)dwGetSize())
				ShowError("Calc Size = "+String(dwGetSize())+", True Size = "+String(nSize),"AIS");


		}

		void Unpack(const BYTE* pucBuf, int& nOffs)
		{
			nMMSI=nUnpack(pucBuf,nOffs);
			nIMO=nUnpack(pucBuf,nOffs);
			LL.Unpack(pucBuf,nOffs);
			wSpeed=wUnpack(pucBuf,nOffs);
			wCourse=wUnpack(pucBuf,nOffs);
			sName=sUnpack(pucBuf,nOffs);
			sCallsign=sUnpack(pucBuf,nOffs);
			nType=nUnpack(pucBuf,nOffs);
			ucNavStatus=ucUnpack(pucBuf,nOffs);
			wLength=wUnpack(pucBuf,nOffs);;
			wWidth=wUnpack(pucBuf,nOffs);;
			wDraught=wUnpack(pucBuf,nOffs);;
			ETA_utc.Unpack(pucBuf,nOffs);;
			sDest=sUnpack(pucBuf,nOffs);
			LastUpdate_utc.Unpack(pucBuf,nOffs);
		}

		void SetSpeed(const double dSpeed_kts)
		{
			wSpeed=dSpeed_kts*10.0+0.5;
		}

		double dSpeed_kts() const
		{
			return wSpeed/10.0;
		}

		void SetCourse(const double dCourse_deg)
		{
			wCourse=dCourse_deg*65536.0/360.0;
		}

		double dCourse_deg() const
		{
			return wCourse*360.0/65536.0;
		}

		void Set(const Cst4_AIS_Dynamic* pD)
		{
			nMMSI=pD->nGetMMSI();
			LL=pD->GetLL4();
			SetSpeed(pD->dSpeed_kts());
			SetCourse(pD->dCourse_deg());
			SetLastUpdate_utc(pD->Time_utc());
		}

		void Set(const Cst4_AIS_Static* pS)
		{
      	if (pS)
         	{
            if (pS->bValid())
               {
               AISMsgClass AISMsg;
               AISMsg.SetExtraInfoBuf(pS->pucGetData(),pS->wGetDataSize());
               if (AISMsg.bShipAndCargoTypeValid())
                  nType=AISMsg.nGetShipAndCargoType();
               else
                  nType=0;
               if (AISMsg.bIMOValid())
                  nIMO=AISMsg.nGetIMO();
               else
                  nIMO=-1;
               if (AISMsg.bCallSignValid())
                  sCallsign=AISMsg.sGetCallSign();
               else
                  sCallsign="";
               if (AISMsg.bShipNameValid())
                  sName=AISMsg.sGetShipName();
               else
                  sName="";
               if (AISMsg.bNavStatusValid())
                  ucNavStatus=(BYTE)AISMsg.GetNavStatus();
               else
                  ucNavStatus=AIS_NAVSTATUS_NOT_DEFINED;

               if (AISMsg.bDraughtValid())
                  {
                  double d=AISMsg.dGetDraught();
                  if (d>50.0)
                     d=0.0;
                  wDraught=(WORD)(d*10);
                  }
               else
                  wDraught=0;

               if (AISMsg.bShipDimsValid())
                  {
                  wLength=(WORD)AISMsg.nGetShipLength();
                  wWidth=(WORD)AISMsg.nGetShipWidth();
                  }
               else
                  {
                  wLength=0;
                  wWidth=0;
                  }
               if (AISMsg.bUTC_ETAValid())
                  ETA_utc=Cst4Time(AISMsg.GetUTC_ETA());
               else
                  ETA_utc.Invalidate();
               if (AISMsg.bDestinationValid())
                  sDest=AISMsg.sGetDestination();
               else
                  sDest="";
               }
            }
		}

		void SetLastUpdate_utc(const JTime& T_utc)
		{
			LastUpdate_utc=Cst4Time(T_utc);
		}

		JTime GetLastUpdate_utc()
		{
			return LastUpdate_utc.Get_utc();
		}

		JTime GetLastUpdate_loc()
		{
			return LastUpdate_utc.Get_loc();
		}

		DWORD dwGetLastUpdate_utc_sec() const
		{
			return LastUpdate_utc.dwSec_utc();
		}

	};


	struct RadarStruct : public Cst4_MessageUtils
	{
		DWORD dwID;
		int nTrack;
		Cst4LatLong LL;
		WORD wSpeed;
		WORD wCourse;
		BYTE ucQuality;
		Cst4Time LastUpdate_utc;

		void Init()
		{
			dwID=0;
			nTrack=-1;
			LL.Invalidate();
			wSpeed=0xFFFF;
			wCourse=0xFFFF;
			ucQuality=0;
			LastUpdate_utc.Invalidate();
		}

		RadarStruct& operator = (const RadarStruct& D)
		{
			dwID=D.dwID;
			nTrack=D.nTrack;
			LL=D.LL;
			wSpeed=D.wSpeed;
			wCourse=D.wCourse;
			ucQuality=D.ucQuality;
			LastUpdate_utc=D.LastUpdate_utc;
			return *this;
		}

		DWORD dwGetSize() const
		{
			return sizeof(dwID)   						//4
					+sizeof(nTrack)                  //4
					+sizeof(LL)                      //8
					+sizeof(wSpeed)                  //2
					+sizeof(wCourse)                 //2
					+sizeof(ucQuality)               //1
					+LastUpdate_utc.dwGetSize()      //6
					;                                //Total = 27
		}

		void Pack(BYTE* pucBuf, int& nOffs)
		{
			int nOffs1=nOffs;
			PackDWORD(dwID,pucBuf,nOffs);
			PackInt(nTrack,pucBuf,nOffs);
			LL.Pack(pucBuf,nOffs);
			PackWORD(wSpeed,pucBuf,nOffs);
			PackWORD(wCourse,pucBuf,nOffs);
			PackBYTE(ucQuality,pucBuf,nOffs);
			LastUpdate_utc.Pack(pucBuf,nOffs);
			int nSize=nOffs-nOffs1;
			if (nSize!=(int)dwGetSize())
				ShowError("Calc Size = "+String(dwGetSize())+", True Size = "+String(nSize),"AIS");
		}

		void Unpack(const BYTE* pucBuf, int& nOffs)
		{
			dwID=dwUnpack(pucBuf,nOffs);
			nTrack=nUnpack(pucBuf,nOffs);
			LL.Unpack(pucBuf,nOffs);
			wSpeed=wUnpack(pucBuf,nOffs);
			wCourse=wUnpack(pucBuf,nOffs);
			ucQuality=ucUnpack(pucBuf,nOffs);
			LastUpdate_utc.Unpack(pucBuf,nOffs);
		}

		void SetSpeed(const double dSpeed_kts)
		{
			wSpeed=dSpeed_kts*10.0+0.5;
		}

		double dSpeed_kts() const
		{
			return wSpeed/10.0;
		}

		void SetCourse(const double dCourse_deg)
		{
			wCourse=dCourse_deg*65536.0/360.0;
		}

		double dCourse_deg() const
		{
			return wCourse*360.0/65536.0;
		}

		void Set(const Cst4_Radar_Track* pT)
		{
			dwID=pT->wSource();
			nTrack=pT->nID();
			LL=Cst4LatLong(pT->GetLL());
			SetSpeed(pT->dSpeed_kts());
			SetCourse(pT->dCourse_deg());
			ucQuality=pT->ucQuality();
			SetLastUpdate_utc(pT->Time_utc());
		}

		void SetLastUpdate_utc(const JTime& T_utc)
		{
			LastUpdate_utc=Cst4Time(T_utc);
		}

		JTime GetLastUpdate_utc()
		{
			return LastUpdate_utc.Get_utc();
		}

		JTime GetLastUpdate_loc()
		{
			return LastUpdate_utc.Get_loc();
		}

		DWORD dwGetLastUpdate_utc_sec() const
		{
			return LastUpdate_utc.dwSec_utc();
		}

	};


private:


	WORD wVersion;
	int nTrack;
	DWORD dwSysStatus;
	BYTE ucAction;
	OriginStruct Origin;
	OriginStruct Update;
	DWORD dwExpire_utc_sec;
	int nMaxLife_h;

   DWORD dwLastSource;
	Cst4LatLong LastLL;
	WORD wLastSpeed;
	WORD wLastCourse;
	DWORD dwLastTime_utc_sec;

	AISStruct AIS;
	RadarStruct Radar;

	//Information
	String sDescr;
	DWORD dwClassification;
	String sComments;

   //Designation
   String sDesAddr;
   int nDesPort;

   struct
   {
   	String sFile;		//Not stored or transmitted
   	String sDescr;    //Not stored or transmitted
      JTime T;
   } aPict[MAX_PICT_NUM];

	void Init()
	{
		wVersion=MSG_VERSION;
		nTrack=-1;
		dwSysStatus=0;
		ucAction=0;
		Origin.Init();
		Update.Init();
		dwExpire_utc_sec=0;
		nMaxLife_h=0;
      dwLastSource=0;
		LastLL.Invalidate();
		wLastSpeed=0;
		wLastCourse=0;
		dwLastTime_utc_sec=0;
//		LastTime_utc.Invalidate();
//		dwLastSource=0;
		AIS.Init();
		Radar.Init();
		sDescr="";
		dwClassification=0;
		sComments="";
   	sDesAddr="";
   	nDesPort=-1;

	}


public:

	Cst4_SysTrack() : Cst4_MessageBase(CST4_SYSTRACK_MSG)
	{
		Init();
	}

	~Cst4_SysTrack()
	{
	}

	void Assign(const Cst4_SysTrack D)
	{
		Cst4_MessageBase::Assign(D);

		wVersion=D.wVersion;
		nTrack=D.nTrack;
		dwSysStatus=D.dwSysStatus;
		ucAction=D.ucAction;

		Origin.T_utc=D.Origin.T_utc;
		Origin.dwID=D.Origin.dwID;
		Origin.ucSeat=D.Origin.ucSeat;

		Update.T_utc=D.Update.T_utc;
		Update.dwID=D.Update.dwID;
		Update.ucSeat=D.Update.ucSeat;

		dwExpire_utc_sec=D.dwExpire_utc_sec;
		nMaxLife_h=D.nMaxLife_h;
      dwLastSource=D.dwLastSource;
		LastLL=D.LastLL;
		wLastSpeed=D.wLastSpeed;
		wLastCourse=D.wLastCourse;
		dwLastTime_utc_sec=D.dwLastTime_utc_sec;
//		LastTime_utc=D.LastTime_utc;
//		dwLastSource=D.dwLastSource;

		AIS=D.AIS;
		Radar=D.Radar;

		dwClassification=D.dwClassification;
		sDescr=D.sDescr;
		sComments=D.sComments;
   	sDesAddr=D.sDesAddr;
   	nDesPort=D.nDesPort;
	}

	Cst4_SysTrack& operator = (const Cst4_SysTrack& D)
	{
		Assign(D);
		return *this;
	}

	Cst4_SysTrack(const void* pPackedData, const int nMaxSize) : Cst4_MessageBase(CST4_SYSTRACK_MSG)
	{
		Init();
		int nOffs=0;
		bUnpack((BYTE*)pPackedData,nOffs,nMaxSize,false);
	}

	DWORD dwGetSize()
	{

	return sizeof(wVersion)           		//4
			+sizeof(nTrack)                  //4
			+sizeof(dwSysStatus)   				//4
			+sizeof(ucAction)             	//1

			+sizeof(Origin.dwID)          	//4
			+sizeof(Origin.ucSeat)        	//1
			+Origin.T_utc.dwGetSize()     	//6

			+sizeof(Update.dwID)     			//4
			+sizeof(Update.ucSeat)        	//1
			+Update.T_utc.dwGetSize()     	//6

			+sizeof(dwExpire_utc_sec)     	//4
			+sizeof(nMaxLife_h)           	//4
			+sizeof(dwLastSource)           	//8
			+sizeof(LastLL)               	//8
			+sizeof(wLastSpeed)              //2
			+sizeof(wLastCourse)             //2
			+sizeof(dwLastTime_utc_sec)      //6

			+sizeof(dwClassification)     	//4
			+dwPackedStringSize(sDescr)     	//2+N1
			+dwPackedStringSize(sComments) 	//2+N2

			+dwPackedStringSize(sDesAddr) 	//2+N2
			+sizeof(nDesPort)

			+AIS.dwGetSize()						//45+A1+A2+A3+A4
			+Radar.dwGetSize()               //27

			;                                //Total = 141+A1+A2+A3+A4+A1+A2
	}

	DWORD dwCalcSize()
	{
		return dwGetBaseSize()+dwGetSize();
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		int nOffs1=nOffs;

		DWORD dwSize=dwGetBaseSize();
		dwSize+=dwGetSize();
		SetSize(dwSize);

		PackBase(pucBuf,nOffs);                	//14

		PackWORD(wVersion,pucBuf,nOffs);       	// 1
		PackInt(nTrack,pucBuf,nOffs);       	// 1
		PackBYTE(ucAction,pucBuf,nOffs);       	// 1
		PackDWORD(dwSysStatus,pucBuf,nOffs);   	// 4

		Origin.T_utc.Pack(pucBuf,nOffs);       	// 6
		PackDWORD(Origin.dwID,pucBuf,nOffs);   	// 4
		PackBYTE(Origin.ucSeat,pucBuf,nOffs);  	// 1

		Update.T_utc.Pack(pucBuf,nOffs);          // 6
		PackDWORD(Update.dwID,pucBuf,nOffs);      // 4
		PackBYTE(Update.ucSeat,pucBuf,nOffs);     // 1

		PackDWORD(dwExpire_utc_sec,pucBuf,nOffs); // 4
		PackInt(nMaxLife_h,pucBuf,nOffs);         // 4

      PackDWORD(dwLastSource,pucBuf,nOffs);
		LastLL.Pack(pucBuf,nOffs);                // 8
		PackWORD(wLastSpeed,pucBuf,nOffs);
		PackWORD(wLastCourse,pucBuf,nOffs);
		PackDWORD(dwLastTime_utc_sec,pucBuf,nOffs);

		PackDWORD(dwClassification,pucBuf,nOffs); // 4	= 59
		PackString(sDescr,pucBuf,nOffs);          // 2+N
		PackString(sComments,pucBuf,nOffs);       // 2+N

		PackString(sDesAddr,pucBuf,nOffs);
      PackInt(nDesPort,pucBuf,nOffs);

		AIS.Pack(pucBuf,nOffs);                	//14
		Radar.Pack(pucBuf,nOffs);                	//14

		DWORD dwSize2=nOffs-nOffs1;

		if (dwSize!=dwSize2)
			{
			ShowError("Calc Size = "+String(dwSize)+", True Size = "+String(dwSize2));
			}

	}

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{

		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;

		wVersion=wUnpack(pucBuf,nOffs);

		if (wVersion!=MSG_VERSION)
			{
//			ShowError("Invalid Message Version "+String(wVersion)+" Detected","System Track Version ("+String(MSG_VERSION)+" Error");
			return false;
			}

		nTrack=nUnpack(pucBuf,nOffs);

		ucAction=ucUnpack(pucBuf,nOffs);
		dwSysStatus=dwUnpack(pucBuf,nOffs);

		Origin.T_utc.Unpack(pucBuf,nOffs);
		Origin.dwID=dwUnpack(pucBuf,nOffs);
		Origin.ucSeat=ucUnpack(pucBuf,nOffs);

		Update.T_utc.Unpack(pucBuf,nOffs);
		Update.dwID=dwUnpack(pucBuf,nOffs);
		Update.ucSeat=ucUnpack(pucBuf,nOffs);
		dwExpire_utc_sec=dwUnpack(pucBuf,nOffs);
		nMaxLife_h=nUnpack(pucBuf,nOffs);

      dwLastSource=dwUnpack(pucBuf,nOffs);
		LastLL.Unpack(pucBuf,nOffs);
		wLastSpeed=wUnpack(pucBuf,nOffs);
		wLastCourse=wUnpack(pucBuf,nOffs);
		dwLastTime_utc_sec=dwUnpack(pucBuf,nOffs);
//		LastTime_utc.Unpack(pucBuf,nOffs);
//		dwLastSource=dwUnpack(pucBuf,nOffs);;

		dwClassification=dwUnpack(pucBuf,nOffs);
		sDescr=sUnpack(pucBuf,nOffs);
		sComments=sUnpack(pucBuf,nOffs);

		sDesAddr=sUnpack(pucBuf,nOffs);
      nDesPort=nUnpack(pucBuf,nOffs);

		AIS.Unpack(pucBuf,nOffs);
		Radar.Unpack(pucBuf,nOffs);


		return true;
	}

	int nCreateTXBuf(BYTE* pucBuf)
	{
		wVersion=MSG_VERSION;
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetTrackNumber(const int n)
	{
		nTrack=n;
	}

	int nGetTrackNumber() const
	{
		return nTrack;
	}

	void SetAction(const SYSTRACK_ACTION uc){ucAction=uc;}
	SYSTRACK_ACTION GetAction() const{return (SYSTRACK_ACTION)ucAction;}

	void SetSysStatus(const DWORD dw){dwSysStatus=dw;}
	DWORD dwGetSysStatus() const{	return dwSysStatus;}

	void SetOrigin(const JTime& T_utc, const DWORD dwID, const BYTE ucSeat)
	{
		Origin.T_utc=T_utc;
		Origin.dwID=dwID;
		Origin.ucSeat=ucSeat;
	}

	void GetOrigin(JTime& T_utc, DWORD& dwID, BYTE& ucSeat)
	{
		if (Origin.T_utc.bValid())
			T_utc=Origin.T_utc.Get_utc();
		else
			T_utc.Invalidate();
		dwID=Origin.dwID;
		ucSeat=Origin.ucSeat;
	}

	void SetUpdate(const JTime& T_utc, const DWORD dwID, const BYTE ucSeat)
	{
		Update.T_utc=T_utc;
		Update.dwID=dwID;
		Update.ucSeat=ucSeat;
	}

	void GetUpdate(JTime& T_utc, DWORD& dwID, BYTE& ucSeat)
	{
		if (Update.T_utc.bValid())
			T_utc=Update.T_utc.Get_utc();
		else
			T_utc.Invalidate();
		dwID=Update.dwID;
		ucSeat=Update.ucSeat;
	}

	void SetExpire_utc_sec(const DWORD dwSec)
	{
		dwExpire_utc_sec=dwSec;
	}

	void CalcExpire()
	{
		dwExpire_utc_sec=Update.T_utc.dwSec_utc()+nMaxLife_h*3600;
	}

	DWORD dwGetExpire_utc_sec() const
	{
		return dwExpire_utc_sec;
	}

	int nGetTimeLeft_sec(const JTime& T_utc) const
	{
	 DWORD dwT_utc_sec=T_utc.dwTotalSec();
	 if (dwExpire_utc_sec>dwT_utc_sec)
		return dwExpire_utc_sec-dwT_utc_sec;
	 else
		return 0;
	}

	void SetMaxLife_h(const int nLife_h)
	{
		nMaxLife_h=nLife_h;
	}

	int nGetMaxLife_h() const
	{
		return nMaxLife_h;
	}

   DWORD dwGetLastSource() const
   {
   	return dwLastSource;
   }

	LatLong GetLastPos() const
	{
		return LastLL.GetLL();
	}

	void SetLastSpeed(const double dSpeed_kts)
	{
		wLastSpeed=dSpeed_kts*10.0+0.5;
	}

	double dLastSpeed_kts() const
	{
		return wLastSpeed/10.0;
	}

	void SetLastCourse(const double dCourse_deg)
	{
		wLastCourse=dCourse_deg*65536.0/360.0;
	}

	double dLastCourse_deg() const
	{
		return wLastCourse*360.0/65536.0;
	}

	void SetLastSourceTime_utc_sec(const DWORD dwT_utc_sec)
	{
		dwLastTime_utc_sec=dwT_utc_sec;
	}

	DWORD dwGetLastSourceTime_utc_sec() const
	{
		return dwLastTime_utc_sec;
	}

   String sGetDesigLine()
   {
   	String s,sV;
      s="$VISTADESIG";
      if (bAISAvail())
      	{
      	s=s+",A";	//AIS
         s=s+","+String(AIS.nMMSI);
         }
      else
      	{
      	s=s+",R";	//Radar
         s=s+","+String(Radar.nTrack);
         }
      sV.printf(L",%6.6f,%6.6f",LastLL.dLat(),LastLL.dLong());
		s=s+sV;
      sV.printf(L",%1.1f,%1.1f",dLastSpeed_kts(),dLastCourse_deg());
		s=s+sV;
      JTime T_utc;
      T_utc.SetFullSec(dwGetLastSourceTime_utc_sec());
      sV.printf(L",%d,%d,%d",T_utc.Get().wDay,T_utc.Get().wMonth,T_utc.Get().wYear);
		s=s+sV;
      sV.printf(L",%d,%d,%d",T_utc.Get().wHour,T_utc.Get().wMinute,T_utc.Get().wSecond);
		s=s+sV;
      s=s+","+sDescr;
      s=s+","+String(nTrack);
		if (bUploadPicture())
			s=s+",U";
		else
			s=s+",D";
		s=s+",*";
		s=s+"\r\n";
   	return s;
   }


	void SetAIS(const AISStruct& A)
	{
		AIS=A;
		LastLL.SetLL(AIS.LL.GetLL());
		SetLastSpeed(AIS.dSpeed_kts());
		SetLastCourse(AIS.dCourse_deg());
		dwSysStatus|=AIS_AVAIL;
	}

	void SetAIS(const Cst4_AIS_Dynamic* pD)
	{
		AIS.Set(pD);
		SetLastSourceTime_utc_sec(pD->dwTime_sec_utc());
    	dwLastSource=pD->wSource();
		LastLL.SetLL(AIS.LL.GetLL());
		SetLastSpeed(pD->dSpeed_kts());
		SetLastCourse(pD->dCourse_deg());
		dwSysStatus|=AIS_AVAIL;
	}

	void SetAIS(const Cst4_AIS_Static* pS)
	{
		AIS.Set(pS);
	}

	AISStruct GetAIS()
	{
		return AIS;
	}

	bool bAISAvail() const
	{
		return (dwSysStatus&AIS_AVAIL)!=0;
	}

	void ClearAIS()
	{
		dwSysStatus&=(~AIS_AVAIL);
	}

//	void SetDesig(const bool bDesig)
//	{
//		if (bDesig)
//			dwSysStatus|=DESIGNATE_CAMERA;
//		else
//			dwSysStatus&=(~DESIGNATE_CAMERA);
//	}
//
//	bool bDesignate() const
//	{
//		return (dwSysStatus&DESIGNATE_CAMERA)!=0;
//	}

   void SetDesIP(const String sAddr, const int nPort)
   {
      sDesAddr=sAddr;
      nDesPort=nPort;
   }

   String sGetDesAddr() const
   {
   	return sDesAddr;
   }

   int nGetDesPort() const
   {
   	return nDesPort;
   }

	void SetUploadPicture(const bool bUpload)
	{
		if (bUpload)
			dwSysStatus|=UPLOAD_PICTURE;
		else
			dwSysStatus&=(~UPLOAD_PICTURE);
	}

	bool bUploadPicture() const
	{
		return (dwSysStatus&UPLOAD_PICTURE)!=0;
	}

	void SetPicture(const bool bPict)
	{
		if (bPict)
			dwSysStatus|=PICTURE_AVAIL;
		else
			dwSysStatus&=(~PICTURE_AVAIL);
	}

	bool bPictAvail() const
	{
		return (dwSysStatus&PICTURE_AVAIL)!=0;
	}

   int nGetPictInFolder(const String sFolder, const DWORD dwUpdate_utc_sec);

   String sGetPictFile(const int n) const
   {
		return aPict[n].sFile;
   }

   String sGetPictDescr(const int n) const
   {
		return aPict[n].sDescr;
   }

   JTime GetPictTime(const int n) const
   {
		return aPict[n].T;
   }

	int nGetMMSI() const
	{
		return AIS.nMMSI;
	}

//	void SetRadar(const RadarStruct& R)
//	{
//		Radar=R;
//		dwSysStatus|=RADAR_AVAIL;
//	}

	void SetRadar(const Cst4_Radar_Track* pT, const bool bForceRadarUse=false)
	{
		Radar.Set(pT);
		bool bUseRadarPos=true;
		DWORD dwT_utc_sec=pT->dwTime_sec_utc();
		if ((AIS.dwGetLastUpdate_utc_sec()+30<dwT_utc_sec) || bForceRadarUse)
			bUseRadarPos=true;
		else
			bUseRadarPos=false;
		if (bUseRadarPos)
			{
			SetLastSourceTime_utc_sec(dwT_utc_sec);
			dwLastSource=Radar.dwID;
			LastLL.SetLL(Radar.LL.GetLL());
			SetLastSpeed(Radar.dSpeed_kts());
			SetLastCourse(Radar.dCourse_deg());

			}
		dwSysStatus|=RADAR_AVAIL;
	}

	RadarStruct GetRadar()
	{
		return Radar;
	}

	bool bRadarAvail() const
	{
		return (dwSysStatus&RADAR_AVAIL)!=0;
	}

	void ClearRadar()
	{
		dwSysStatus&=(~RADAR_AVAIL);
	}

	void GetRadar(DWORD& dwID, int& nTrack)
	{
		dwID=Radar.dwID;
		nTrack=Radar.nTrack;
	}

	bool bSameRadar(const DWORD dwID, const int nTrack) const
	{
		return (dwID==Radar.dwID)&&(nTrack==Radar.nTrack);
	}

/*
	void SetRawClassification(const DWORD dw)
	{
		dwClassification=dw;
	}

	DWORD dwGetRawClassification()const
	{
		return dwClassification;
	}
*/

	void SetDescr(const String s)
	{
		sDescr=s;
	}

	String sGetDescr() const
	{
		return sDescr;
	}

	void SetComments(const String s)
	{
		sComments=s;
	}

	String sGetComments() const
	{
		return sComments;
	}

	OriginStruct GetOrigin() const
	{
		return Origin;
	}

	bool bIdentical(const OriginStruct& Orig) const
	{
		return Origin.bIdentical(Orig);
	}

	bool bIdentical(const Cst4_SysTrack& D) const
	{
		return Origin.bIdentical(D.Origin);
	}

	bool bExpired(const DWORD dwT_utc_sec) const
	{
		return (dwT_utc_sec>dwExpire_utc_sec);
	}

	void SetClassification(const CST4_SHIP_CLASSIFICATION Class)
	{
		dwClassification&=(~CLASS_MASK);
		dwClassification|=Class;
	}

	CST4_SHIP_CLASSIFICATION GetClassification()
	{
		return (CST4_SHIP_CLASSIFICATION)(dwClassification&CLASS_MASK);
	}


	String sGetShipClassification(const CST4_SHIP_CLASSIFICATION Class)
	{
		switch(Class)
			{
			case CST4_SHIP_UNKNOWN: 			return "Unknown";
			case CST4_SHIP_PENDING: 			return "Pending";
			case CST4_SHIP_FRIEND: 				return "Friend";
			case CST4_SHIP_NEUTRAL: 			return "Neutral";
			case CST4_SHIP_HOSTILE: 			return "Hostile";
			case CST4_SHIP_ASSUMED_FRIEND: 	return "Assumed Friend";
			case CST4_SHIP_SUSPECT: 			return "Suspect";
			case CST4_SHIP_EXERCISE_PENDING: return "Exercise Pending";
			case CST4_SHIP_EXERCISE_UNKNOWN: return "Exercise Unknown";
			case CST4_SHIP_EXERCISE_FRIEND: 	return "Exercise Friend";
			case CST4_SHIP_EXERCISE_NEUTRAL: return "Exercise Neutral";
			case CST4_SHIP_EXERCISE_ASSUMED_FRIEND: return "Exercise Assumed Friend";
			case CST4_SHIP_JOKER: 				return "Joker";
			case CST4_SHIP_FAKER: 				return "Faker";
			default: 								return "Unknown";
			}
	}

	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};



//***************************************************************************

class Cst4_Request : public Cst4_MessageBase
{

	DWORD dwRequest;

public:

	void Set(const JTime& _T, const WORD _wSourceID, const DWORD _dwRequest)
	{
		SetTime(_T);
      SetSource(_wSourceID);
      SetValid();
      dwRequest=_dwRequest;
   }

   Cst4_Request(const JTime& _T, const WORD _wSourceID, const DWORD _dwRequest) : Cst4_MessageBase(CST4_REQUEST_MSG)
   {
      Set(_T,_wSourceID,_dwRequest);
   }

   Cst4_Request() : Cst4_MessageBase(CST4_REQUEST_MSG)
   {
   }

	Cst4_Request(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_REQUEST_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

   ~Cst4_Request()
   {

   }

   void Assign(const Cst4_Request& M)
   {
      Cst4_MessageBase::Assign(M);
      dwRequest=M.dwRequest;
   }

   Cst4_Request& operator = (const Cst4_Request& M)
   {
      Assign(M);
      return *this;
   }

	Cst4_Request(const Cst4_Request& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(dwRequest);
	}


	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackDWORD(dwRequest,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		dwRequest=dwUnpack(pucBuf,nOffs);
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetRequest(const DWORD _dwRequest)
	{
		dwRequest=_dwRequest;
	}

	DWORD dwGetRequest() const
	{
		return dwRequest;
	}
   

};

//***************************************************************************
class Cst4_Response : public Cst4_MessageBase
{

   DWORD dwRequest;
   BYTE* pucInfo;
   WORD wInfoSize;

public:

   void Set(const JTime& _T, const WORD _wSourceID, const DWORD _dwRequest, const void* _pucInfo=NULL, const WORD _wInfoSize=0)
	{
		SetTime(_T,true);
		SetSource(_wSourceID);
      SetValid();
      dwRequest=_dwRequest;
      delete[] pucInfo;
      wInfoSize=_wInfoSize;
      if ((wInfoSize>0)&&(_pucInfo!=NULL))
         {
         pucInfo=new BYTE[wInfoSize];
         memcpy(pucInfo,_pucInfo,wInfoSize);
         }
      else
         pucInfo=NULL;
   }

   Cst4_Response(const JTime& _T, const WORD _wSourceID, const DWORD _dwRequest, const void* _pucInfo=NULL, const WORD _wInfoSize=0) : Cst4_MessageBase(CST4_RESPONSE_MSG)
   {
      pucInfo=NULL;
      Set(_T,_wSourceID,_dwRequest,_pucInfo,_wInfoSize);
   }

   Cst4_Response() : Cst4_MessageBase(CST4_RESPONSE_MSG)
   {
		pucInfo=NULL;
		wInfoSize=0;
   }

   Cst4_Response(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_RESPONSE_MSG)
	{
		pucInfo=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
   }

   ~Cst4_Response()
   {
      delete[] pucInfo;
   }

   void Assign(const Cst4_Response& M)
   {
      Cst4_MessageBase::Assign(M);
      dwRequest=M.dwRequest;
      delete[] pucInfo;
      wInfoSize=M.wInfoSize;
      if ((wInfoSize>0)&&(M.pucInfo))
         {
         pucInfo=new BYTE[wInfoSize];
         memcpy(pucInfo,M.pucInfo,wInfoSize);
         }
      else
         pucInfo=NULL;
   }

   Cst4_Response& operator = (const Cst4_Response& M)
   {
      Assign(M);
      return *this;
   }

	Cst4_Response(const Cst4_Response& M)
	{
		pucInfo=NULL;
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(dwRequest)+sizeof(wInfoSize)+wInfoSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackDWORD(dwRequest,pucBuf,nOffs);
		PackData((BYTE*)pucInfo,wInfoSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		dwRequest=dwUnpack(pucBuf,nOffs);
		wInfoSize=wUnpack(pucBuf,nOffs);
		delete[] pucInfo;
		if (wInfoSize>0)
			{
			pucInfo=new BYTE[wInfoSize];
			memcpy(pucInfo,&pucBuf[nOffs],wInfoSize);
			nOffs+=(int)wInfoSize;
			}
		else
			pucInfo=NULL;
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetRequest(const DWORD _dwRequest)
	{
		dwRequest=_dwRequest;
	}

	DWORD dwGetRequest() const
	{
		return dwRequest;
	}

	BYTE* pucGetInfo()
	{
		return pucInfo;
	}

	int nGetInfoSize() const
	{
		return (int)wInfoSize;
	}

	int nGetInfo(BYTE* puc, const int nMaxSize)
	{
		int nSize=wInfoSize;
		if (nSize>nMaxSize)
			nSize=nMaxSize;
		if (nSize>0)
			memcpy(puc,pucInfo,nSize);
		return nSize;
	}

	void SetInfo(const BYTE* puc, const int nSize)
	{
		delete[] pucInfo;
		wInfoSize=(WORD)nSize;
		if (nSize>0)
			{
			pucInfo=new BYTE[wInfoSize];
			memcpy(pucInfo,puc,wInfoSize);
			}
		else
			pucInfo=NULL;
	}

};

//***************************************************************************

/*

class Cst4_KeepAlive : public Cst4_MessageBase
{

public:

	void Set(const JTime& _T, const WORD _wSourceID)
	{
		SetTime(_T);
		SetSource(_wSourceID);
		SetValid();
	}

/*
	Cst4_KeepAlive(const JTime& _T, const WORD _wSourceID) : Cst4_MessageBase(CST4_KEEPALIVE_MSG)
	{
		Set(_T,_wSourceID);
	}

	Cst4_KeepAlive() : Cst4_MessageBase(CST4_KEEPALIVE_MSG)
	{
	}

	Cst4_KeepAlive(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_KEEPALIVE_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}

	~Cst4_KeepAlive()
	{

	}

	void Assign(const Cst4_KeepAlive& M)
	{
		Cst4_MessageBase::Assign(M);
	}

	Cst4_KeepAlive& operator = (const Cst4_KeepAlive& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_KeepAlive(const Cst4_KeepAlive& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize();
	}


	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}


};

*/

//***************************************************************************

enum CST4_USER_MSG_TYPE
{
	CST4_USER_NONE					= 0,
	CST4_USER_INS					= 1,
	CST4_USER_TRACKER				= 2,  	// NA10/ORT/EOT
	CST4_USER_WEATHER				= 3,
	CST4_USER_AIS_STATUS			= 4,
	CST4_USER_SITE_DEFS			= 5,
	CST4_USER_SITE_UPDATE		= 6,
	CST4_USER_SOURCE_STATUS		= 7,
	CST4_USER_WIND					= 8,
	CST4_USER_AIS_ALT				= 9,
	CST4_USER_KEEP_ALIVE			= 10,
	CST4_USER_PICTUREBLOCK 		= 11,
	CST4_USER_CYCLOPE 			= 12,
	CST4_USER_PROCWATCH_ON 		= 13,
	CST4_USER_PROCWATCH_OFF 	= 14,
   CST4_USER_LOCALHUB_BITE    = 15,
};


class Cst4_User_Msg: public Cst4_MessageBase
{

	DWORD dwType;
	BYTE* pucPayload;
	WORD wPayloadSize;

public:

	void Set(const JTime& _T, const WORD _wSourceID, const DWORD _dwType, const void* _pucPayload=NULL, const WORD _wPayloadSize=0)
	{
		SetTime(_T,true);
		SetSource(_wSourceID);
		SetValid();
		dwType=_dwType;
		delete[] pucPayload;
		wPayloadSize=_wPayloadSize;
		if ((wPayloadSize>0)&&(_pucPayload!=NULL))
			{
         pucPayload=new BYTE[wPayloadSize];
         memcpy(pucPayload,_pucPayload,wPayloadSize);
         }
      else
         pucPayload=NULL;
   }

   Cst4_User_Msg(const JTime& _T, const WORD _wSourceID, const DWORD _dwType, const void* _pucPayload=NULL, const WORD _wPayloadSize=0) : Cst4_MessageBase(CST4_USER_MSG)
   {
      pucPayload=NULL;
      Set(_T,_wSourceID,_dwType,_pucPayload,_wPayloadSize);
   }

   Cst4_User_Msg() : Cst4_MessageBase(CST4_USER_MSG)
   {
      pucPayload=NULL;
   }

   Cst4_User_Msg(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_USER_MSG)
	{
		pucPayload=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
   }

   ~Cst4_User_Msg()
   {
      delete[] pucPayload;
   }

   void Assign(const Cst4_User_Msg& M)
   {
      Cst4_MessageBase::Assign(M);
      dwType=M.dwType;
		delete[] pucPayload;
      wPayloadSize=M.wPayloadSize;
      if ((wPayloadSize>0)&&(M.pucPayload))
         {
         pucPayload=new BYTE[wPayloadSize];
         memcpy(pucPayload,M.pucPayload,wPayloadSize);
         }
      else
         pucPayload=NULL;
   }

   Cst4_User_Msg& operator = (const Cst4_User_Msg& M)
   {
      Assign(M);
      return *this;
   }

	Cst4_User_Msg(const Cst4_User_Msg& M)
	{
		pucPayload=NULL;
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(dwType)+sizeof(wPayloadSize)+wPayloadSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackDWORD(dwType,pucBuf,nOffs);
		PackData((BYTE*)pucPayload,wPayloadSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		dwType=dwUnpack(pucBuf,nOffs);
		wPayloadSize=wUnpack(pucBuf,nOffs);
		delete[] pucPayload;
		if (wPayloadSize>0)
			{
			pucPayload=new BYTE[wPayloadSize];
			memcpy(pucPayload,&pucBuf[nOffs],wPayloadSize);
			nOffs+=(int)wPayloadSize;
			}
		else
			pucPayload=NULL;
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetType(const DWORD _dwType)
	{
		dwType=_dwType;
	}

	DWORD dwGetType() const
	{
		return dwType;
	}

	void* pGetPayload() const
	{
		return (void*)pucPayload;
	}

	int nGetPayloadSize() const
	{
		return (int)wPayloadSize;
	}

	int nGetPayload(void* p, const int nMaxSize)
	{
		int nSize=wPayloadSize;
		if (nSize>nMaxSize)
			nSize=nMaxSize;
		if (nSize>0)
			memcpy(p,pucPayload,nSize);
		return nSize;
	}

	void SetPayload(const void* p, const int nSize)
	{
		delete[] pucPayload;
		wPayloadSize=(WORD)nSize;
		if (nSize>0)
			{
			pucPayload=new BYTE[wPayloadSize];
			memcpy(pucPayload,p,wPayloadSize);
			}
		else
			pucPayload=NULL;
	}


	JFile::FILE_ERROR Extract(JFile* pF);
	String sExtract();

};

//***************************************************************************

class Cst4_BITE_Info : public Cst4_MessageBase
{

	BYTE* pucInfo;
	WORD wInfoSize;

public:

	void Set(const JTime& _T, const WORD _wSourceID, const void* _pucInfo, const WORD _wInfoSize)
	{
		SetTime(_T);
		SetSource(_wSourceID);
		SetValid();
		delete[] pucInfo;
		wInfoSize=_wInfoSize;
		if ((wInfoSize>0)&&(_pucInfo!=NULL))
			{
			pucInfo=new BYTE[wInfoSize];
			memcpy(pucInfo,_pucInfo,wInfoSize);
			}
		else
			pucInfo=NULL;
	}

	Cst4_BITE_Info(const JTime& _T, const WORD _wSourceID, const void* _pucInfo, const WORD _wInfoSize) : Cst4_MessageBase(CST4_BITE_MSG)
	{
		pucInfo=NULL;
		Set(_T,_wSourceID,_pucInfo,_wInfoSize);
	}

	Cst4_BITE_Info() : Cst4_MessageBase(CST4_BITE_MSG)
	{
      pucInfo=NULL;
   }

	Cst4_BITE_Info(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(CST4_BITE_MSG)
	{
		pucInfo=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
   }

   ~Cst4_BITE_Info()
   {
      delete[] pucInfo;
   }

   void Assign(const Cst4_BITE_Info& M)
   {
      Cst4_MessageBase::Assign(M);
      delete[] pucInfo;
      wInfoSize=M.wInfoSize;
      if ((wInfoSize>0)&&(M.pucInfo))
         {
         pucInfo=new BYTE[wInfoSize];
         memcpy(pucInfo,M.pucInfo,wInfoSize);
         }
      else
         pucInfo=NULL;
	}

   Cst4_BITE_Info& operator = (const Cst4_BITE_Info& M)
   {
      Assign(M);
      return *this;
   }

	Cst4_BITE_Info(const Cst4_BITE_Info& M)
	{
		pucInfo=NULL;
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+sizeof(wInfoSize)+wInfoSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackData((BYTE*)pucInfo,wInfoSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		wInfoSize=wUnpack(pucBuf,nOffs);
		delete[] pucInfo;
		if (wInfoSize>0)
			{
			pucInfo=new BYTE[wInfoSize];
			memcpy(pucInfo,&pucBuf[nOffs],wInfoSize);
			nOffs+=(int)wInfoSize;
			}
		else
			pucInfo=NULL;
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	BYTE* pucGetInfo()
	{
		return pucInfo;
	}

	int nGetInfoSize() const
	{
		return (int)wInfoSize;
	}

	int nGetInfo(BYTE* puc, const int nMaxSize)
	{
		int nSize=wInfoSize;
		if (nSize>nMaxSize)
			nSize=nMaxSize;
		if (nSize>0)
			memcpy(puc,pucInfo,nSize);
		return nSize;
	}

	void SetInfo(const BYTE* puc, const int nSize)
	{
		delete[] pucInfo;
		wInfoSize=(WORD)nSize;
		if (nSize>0)
			{
			pucInfo=new BYTE[wInfoSize];
			memcpy(pucInfo,puc,wInfoSize);
			}
		else
			pucInfo=NULL;
	}

};

//***************************************************************************

#pragma option pop    //Set compiler back to default alignment


#endif
