//---------------------------------------------------------------------------
#ifndef Cst5MessagesH
#define Cst5MessagesH
//---------------------------------------------------------------------------

#include "JTransform.h"
#include "JTime.h"
#include "Cst4Defs.h"
#include "Cst4Messages.h"
#include "JList.h"
#include "AISDataDefs_210.h"
#include "Cst4SiteDefs.h"
#include "JUtils.h"
#include "JFile.h"

#pragma option push -a4    //Set compiler to DWORD alignment

//***************************************************************************


class Cst5_MessageBase : public Cst4_MessageUtils
{

public:

	enum
	{
		CST5_VERSION   = 1000,
	};

	enum
	{
		CST5_TYPE_OFFS             = 0,
		CST5_TYPE_MASK             = 0x0000001F,
		CST5_STAT_SIZE_OFFS        = 5,
		CST5_STAT_SIZE_MASK        = 0x7FFFFFE0,  //before shift (max size = 67 MB
		CST5_STAT_MSG_VALID        = 0x80000000,

	};

private:

	DWORD dwStatus;
	WORD wVersion;
	DWORD dwDataSource;
	Cst4Time T;

	enum
	{
		CST5_SOURCE_MASK        = 0x0000FFFF,  //after shift
		CST5_SOURCE_SHIFT       = 0,
		CST5_SOURCE_CLEAR       = 0xFFFF0000,

//		CST5_SERVERINDEX_MASK   = 0x0000FFFF,  //after shift !!!
//		CST5_SERVERINDEX_SHIFT  = 16,
//		CST5_SERVERINDEX_CLEAR  = 0x0000FFFF,  //after shift !!!
	};


protected:

	DWORD dwGetBaseSize() const
	{
		return sizeof(dwStatus)+sizeof(wVersion)+sizeof(dwDataSource)+T.dwGetSize();
	}



	void Init(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		dwDataSource=0;
		wVersion=CST5_VERSION;
		dwStatus=_MsgType;
		T.Invalidate();
	}



public:

	void Invalidate()
	{
		dwStatus&=(~CST5_STAT_MSG_VALID);
	}

	void SetValid()
	{
		dwStatus|=CST5_STAT_MSG_VALID;
	}

	bool bValid() const
	{
		return (dwStatus&CST5_STAT_MSG_VALID)!=0;
	}

	Cst5_MessageBase(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
	{
		Init(_MsgType);
	}

	void Assign(const Cst5_MessageBase& M)
	{
		dwStatus=M.dwStatus;
		wVersion=M.wVersion;
		dwDataSource=M.dwDataSource;
		T=M.T;
	}

	Cst5_MessageBase(const Cst5_MessageBase& M)
	{
		Assign(M);
	}

	~Cst5_MessageBase()
	{
	}

	Cst5_MessageBase& operator = (const Cst5_MessageBase& M)
	{
		Assign(M);
		return *this;
	}

	bool operator < (const Cst5_MessageBase& M) const
	{
		return (T.dSec_raw()<M.T.dSec_raw());
	}

	bool operator > (const Cst5_MessageBase& M) const
	{
		return (T.dSec_raw()>M.T.dSec_raw());
	}



	void SetSource(const WORD wSrc)
	{
		dwDataSource&=CST5_SOURCE_CLEAR;
		dwDataSource|=(wSrc<<CST5_SOURCE_SHIFT);
	}

	WORD wSource() const
	{
		return (WORD)((dwDataSource>>CST5_SOURCE_SHIFT)&CST5_SOURCE_MASK);
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
		dwStatus&=(~CST5_STAT_SIZE_MASK);
		dwStatus|=(dwSize<<CST5_STAT_SIZE_OFFS);
	}

	DWORD dwSize() const
	{
		return (dwStatus&CST5_STAT_SIZE_MASK)>>CST5_STAT_SIZE_OFFS;
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

//	double dTimeDiff_sec(const Cst4_MessageBase& M) const
//	{
//		return fabs(T.dSec_raw()-M.T.dSec_raw());
//	}

	void PackBase(BYTE* pucBuf, int& nOffs)
	{
		PackDWORD(dwStatus,pucBuf,nOffs);
		PackWORD(wVersion,pucBuf,nOffs);
		PackDWORD(dwDataSource,pucBuf,nOffs);
		T.Pack(pucBuf,nOffs);
	}

	bool bUnpackBase(const BYTE* pucBuf, int& nOffs, const DWORD dwMaxSize)
	{
		if (nOffs>(int)dwMaxSize-(int)dwGetBaseSize())
			return false;
		dwStatus=dwUnpack(pucBuf,nOffs);
		if (nOffs>(int)dwMaxSize-(int)dwPayloadSize())
				return false;
		wVersion=wUnpack(pucBuf,nOffs);
		dwDataSource=dwUnpack(pucBuf,nOffs);
		T.Unpack(pucBuf,nOffs);
		FixUTC();	//All data will now be UTC
		return true;
	}

	DWORD dwPayloadSize() const
	{
		return ((dwStatus&CST5_STAT_SIZE_MASK)>>CST5_STAT_SIZE_OFFS)-dwGetBaseSize();
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
*/

	JFile::FILE_ERROR Extract(JFile* pF);



};

//***************************************************************************

class Cst5_Image : public Cst5_MessageBase
{
	int nSysTrackNum;
	String sDescr;
	String sFileName;
	DWORD dwImageSize;
	BYTE* pucImage;

public:

	void Set(const JTime& _T,
				const DWORD _dwSourceID,
				const int _nSysTrackNum,
				const String _sFileName,
				const String _sDescr,
				const BYTE* _pucImage,
				const DWORD _dwImageSize)
	{
		SetTime(_T);
		nSysTrackNum=_nSysTrackNum;
		sDescr=_sDescr;
		sFileName=_sFileName;
		SetImage(_pucImage,_dwImageSize);
		SetValid();
	}

	Cst5_Image(	const JTime& _T,
					const DWORD _dwSourceID,
					const int _nSysTrackNum,
					const String _sFileName,
					const String _sDescr,
					const BYTE* _pucImage,
					const DWORD _dwImageSize)
	{
		dwImageSize=0;
		pucImage=NULL;
		Set(_T,_dwSourceID,_nSysTrackNum,_sFileName,_sDescr,_pucImage,_dwImageSize);
	}

	Cst5_Image() : Cst5_MessageBase(CST5_IMAGE_MSG)
	{
		dwImageSize=0;
		pucImage=NULL;
	}

	Cst5_Image(const BYTE* pucBuf, const DWORD dwMaxSize) : Cst5_MessageBase(CST5_IMAGE_MSG)
	{
		dwImageSize=0;
		pucImage=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,dwMaxSize);
	}

	~Cst5_Image()
	{
		delete[] pucImage;
	}

	void Set(const BYTE* pucBuf, const DWORD dwMaxSize)
	{
		dwImageSize=0;
		pucImage=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,dwMaxSize);
	}


	void Assign(const Cst5_Image& M)
	{
		Cst5_MessageBase::Assign(M);
      nSysTrackNum=M.nSysTrackNum;
		sFileName=M.sFileName;
		sDescr=M.sDescr;
		SetImage(M.pucImage,M.dwImageSize);
	}

	Cst5_Image& operator = (const Cst5_Image& M)
	{
		Assign(M);
		return *this;
	}

	Cst5_Image(const Cst5_Image& M)
	{
		dwImageSize=0;
		pucImage=NULL;
		Assign(M);
	}

	void SetImage(const BYTE* _pucImage, const DWORD dwSize)
	{
		dwImageSize=dwSize;
		delete[] pucImage;
		if (dwImageSize>0)
			{
			pucImage=new BYTE[dwImageSize];
			memcpy(pucImage,_pucImage,dwImageSize);
			}
		else
			pucImage=NULL;

	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()+dwPackedStringSize(sDescr)+dwPackedStringSize(sFileName)+sizeof(dwImageSize)+dwImageSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackInt(nSysTrackNum,pucBuf,nOffs);
		PackString(sFileName,pucBuf,nOffs);
		PackString(sDescr,pucBuf,nOffs);
		PackBigData(pucImage,dwImageSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize))
			return false;
		nSysTrackNum=nUnpack(pucBuf,nOffs);
		sFileName=sUnpack(pucBuf,nOffs);
		sDescr=sUnpack(pucBuf,nOffs);
//		dwImageSize=dwUnpackBigData(pucImage,pucBuf,nOffs);

		dwImageSize=dwUnpack(pucBuf,nOffs);
		if (dwImageSize>0)
			{
			pucImage=new BYTE[dwImageSize];
			memcpy(pucImage,&pucBuf[nOffs],dwImageSize);
			nOffs+=(int)dwImageSize;
			}
		return true;
	};

	void SetSysTrackNum(const int nNum)
	{
		nSysTrackNum=nNum;
	}

	int nGetSysTrackNum() const
	{
		return nSysTrackNum;
	}

	bool bLoadJPEG(const String sFile)
	{
		sFileName=sFile;
		delete[] pucImage;
		pucImage=NULL;
		dwImageSize=0;
		JMemFile F;
		JFile::FILE_ERROR E=F.ReadBuffer(sFileName);
		if (!E)
			SetImage(F.pucGetBuffer(),F.dwGetSize());
		return (E==JFile::F_NO_ERROR);
	}

		bool bStoreJPEG(const String sFile="")
	{
		String sF;
		if (sFile.IsEmpty())
			sF=sFileName;
		else
			sF=sFile;
		if (dwImageSize>0)
			{
			JFile F('O');
			JFile::FILE_ERROR E=F.Create(sF);
			if (!E)
				E=F.Write(pucImage,dwImageSize);
			return (E==JFile::F_NO_ERROR);
			}
		return false;
	}

	void SetFileName(const String sFile)
	{
		sFileName=sFile;
	}

	String sGetFileName() const
	{
		return sFileName;
	}

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='5';
		Pack(pucBuf,nOffs);
		return nOffs;
	}


};

//***************************************************************************

#pragma option pop    //Set compiler back to default alignment


#endif
