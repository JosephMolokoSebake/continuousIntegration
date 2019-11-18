//---------------------------------------------------------------------------
#ifndef Cst4_IFFMsgH
#define Cst4_IFFMshH
//---------------------------------------------------------------------------

#pragma option push -a4    //Set compiler to DWORD alignment

enum CST4_MSG_TYPE
{
	CST4_IFF_TRACK_MSG	 = 8,
};

//***************************************************************************

class Cst4LatLong
{
	int nLat;   //Store as 1/10000 of a minute
	int nLong;  //Store as 1/10000 of a minute

public:

	void Invalidate()
	{
		nLat=0x7FFFFFFF;
		nLong=0x7FFFFFFF;
	}

	Cst4LatLong()
	{
		Invalidate();
	}

	Cst4LatLong(const double dLat, const double dLong)
	{
		nLat=dLat*600000.0;
		nLong=dLong*600000.0;
	}

	Cst4LatLong(const LatLong& _LL)
	{
		nLat=_LL.dLat*600000.0;
		nLong=_LL.dLong*600000.0;
	}

	Cst4LatLong(const Cst4LatLong& C)
	{
		nLat=C.nLat;
		nLong=C.nLong;
	}

	~Cst4LatLong()
	{
	}

	Cst4LatLong& operator = (const Cst4LatLong& C)
	{
		nLat=C.nLat;
		nLong=C.nLong;
		return *this;
	}

	double dLat() const
	{
		return nLat/600000.0;
	}

	double dLong() const
	{
		return nLong/600000.0;
	}

	void SetLat(const double dLat)
	{
		nLat=dLat*600000.0;
	}

	void SetLong(const double dLong)
	{
		nLong=dLong*600000.0;
	}

	bool bValid() const
	{
		return (nLat!=0x7FFFFFFF);
	}

	void Pack(char* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&nLat,sizeof(nLat));
		nOffs+=sizeof(nLat);
		memcpy(&pucBuf[nOffs],&nLong,sizeof(nLong));
		nOffs+=sizeof(nLong);
	};

	void Unpack(const char* pucBuf, int& nOffs)
	{
		memcpy(&nLat,&pucBuf[nOffs],sizeof(nLat));
		nOffs+=sizeof(nLat);
		memcpy(&nLong,&pucBuf[nOffs],sizeof(nLong));
		nOffs+=sizeof(nLong);
	};

	bool operator == (const Cst4LatLong& LL) const
	{
		return (nLat==LL.nLat)&&(nLong==LL.nLong);
	}

	bool operator != (const Cst4LatLong& LL) const
	{
		return (nLat!=LL.nLat)||(nLong!=LL.nLong);
	}


};

//***************************************************************************

enum CST4_TIME_SOURCE
{
	CST4_LOC_PC_TIME,
	CST4_LOC_GPS_TIME,
	CST4_HUB_PC_TIME,
	CST4_HUB_GPS_TIME,
	CST4_HUB_INTERNET_TIME,
	CST4_HUB_NEMAS_TIME,

};

class Cst4Time
{
	enum
	{
		CST4_TIME_SOURCE_MASK   = 0xF0000000,
		CST4_TIME_SOURCE_OFFS   = 28,
		CST4_TIME_MASK          = 0x0FFFFFFF,
	};

	WORD wDate;
	DWORD dwT;     //bit 27..0 = time in 10ms steps
						//bit 31..28 = time source

	void SetDCSDate(const WORD wPacked, WORD& wDay, WORD& wMonth, WORD& wYear)
	{
		wDay=wPacked&0x001F;
		wMonth=(wPacked>>5)&0x000F;
		wYear=(wPacked>>9)&0x007F)+2000;
	}

	WORD wGetDCSDate(const WORD wDay, const WORD wMonth, const WORD wYear) const
	{
		WORD w=wDay;
		w|=(wMonth<<5);
		w|=(wYear-2000)<<9;
		return w;
	}

public:

	void SetSource(const CST4_TIME_SOURCE Src)
	{
		dwT&=CST4_TIME_MASK;
		DWORD dwSrc=(DWORD)Src;
		dwT|=(dwSrc<<CST4_TIME_SOURCE_OFFS);
	}

	void Set(const int nTime_ms, const WORD wDay, const WORD wMonth, const WORD wYear, const CST4_TIME_SOURCE Src=CST4_LOC_PC_TIME)
	{
		wDate=wGetDCSDate(wDay,dMonth,wYear);
		dwT=(T.nMilliSec()/10)&CST4_TIME_MASK;
		SetSource(Src);
	}

	Cst4Time()
	{
		wDate=0;   //Invalidate
	}

	Cst4Time(const Cst4Time& T)
	{
		wDate=T.wDate;
		dwT=T.dwT;
	}

	~Cst4Time()
	{
	}

	Cst4Time& operator = (const Cst4Time& T)
	{
		wDate=T.wDate;
		dwT=T.dwT;
		return *this;
	}

	void Invalidate()
	{
		wDate=0;
	}

	bool bValid() const
	{
		return (wDate!=0);
	}

	CST4_TIME_SOURCE GetSource() const
	{
		return (CST4_TIME_SOURCE)((dwT&CST4_TIME_SOURCE_MASK)>>CST4_TIME_SOURCE_OFFS);
	}

	DWORD dwGetSize() const
	{
		return sizeof(wDate)+sizeof(dwT);
	}

	void Pack(char* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&wDate,sizeof(wDate));
		nOffs+=sizeof(wDate);
		memcpy(&pucBuf[nOffs],&dwT,sizeof(dwT));
		nOffs+=sizeof(dwT);
	};

	void Unpack(const char* pucBuf, int& nOffs)
	{
		memcpy(&wDate,&pucBuf[nOffs],sizeof(wDate));
		nOffs+=sizeof(wDate);
		memcpy(&dwT,&pucBuf[nOffs],sizeof(dwT));
		nOffs+=sizeof(dwT);
	};

};

//***************************************************************************

class Cst4_MessageBase
{

public:

	enum
	{
		CST4_TYPE_OFFS             = 0,
		CST4_TYPE_MASK             = 0x000000FF,
		CST4_STAT_SIZE_OFFS        = 8,
		CST4_STAT_SIZE_MASK        = 0x0FFFFF00,  //before shift
		CST4_STAT_MSG_COMPRESSED   = 0x40000000,
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
	};


protected:

	DWORD dwGetBaseSize() const
	{
		return sizeof(dwStatus)+sizeof(dwDataSources)+T.dwGetSize();
	}

	void PackBYTE(const BYTE uc, BYTE* pucBuf, int& nOffs)
	{
		memcpy(&pucBuf[nOffs],&uc,sizeof(uc));
		nOffs+=sizeof(uc);
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

	void PackString(const String s, BYTE* pucBuf, int& nOffs)
	{
		AnsiString s1=s;	//Make AnsiString (String is UniCode!!)
		WORD w=s1.Length();
		PackWORD(w,pucBuf,nOffs);
		if (w>0)
			{
			memcpy(&pucBuf[nOffs],s1.c_str(),w);
			nOffs+=(int)w;
			}
	}

	BYTE ucUnpack(const BYTE* pucBuf, int& nOffs)
	{
		BYTE uc;
		memcpy(&uc,&pucBuf[nOffs],sizeof(uc));
		nOffs+=sizeof(uc);
		return uc;
	}

	WORD wUnpack(const BYTE* pucBuf, int& nOffs)
	{
		WORD w;
		memcpy(&w,&pucBuf[nOffs],sizeof(w));
		nOffs+=sizeof(w);
		return w;
	}

	DWORD dwUnpack(const BYTE* pucBuf, int& nOffs)
	{
		DWORD dw;
      memcpy(&dw,&pucBuf[nOffs],sizeof(dw));
      nOffs+=sizeof(dw);
      return dw;
   }

   int nUnpack(const BYTE* pucBuf, int& nOffs)
   {
      int n;
      memcpy(&n,&pucBuf[nOffs],sizeof(n));
      nOffs+=sizeof(n);
      return n;
   }

   String sUnpack(const BYTE* pucBuf, int& nOffs)
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

   void Init(const CST4_MSG_TYPE _MsgType=CST4_NO_MSG)
   {
      dwDataSources=0;
      dwStatus=_MsgType;
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

   bool bValid() const
   {
      return dwDataSources!=0;
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

   void SetSize(const DWORD dwSize)   //Size <= 1048576 bytes
   {
      dwStatus&=(~CST4_STAT_SIZE_MASK);
      dwStatus|=(dwSize<<CST4_STAT_SIZE_OFFS);
   }

   DWORD dwSize() const
   {
      return (dwStatus&CST4_STAT_SIZE_MASK)>>CST4_STAT_SIZE_OFFS;
	}

   void SetTime(const JTime& _T)
   {
      T=Cst4Time(_T);
   }

   JTime Time()
   {
      return T.Get();
   }

   double dTime_sec()
   {
      return T.dSec();
   }

   DWORD dwTime_sec()
   {
      return T.dwSec();
   }


   void PackBase(BYTE* pucBuf, int& nOffs)
   {
      PackDWORD(dwStatus,pucBuf,nOffs);
      PackDWORD(dwDataSources,pucBuf,nOffs);
      T.Pack(pucBuf,nOffs);
   }

   void UnpackBase(const BYTE* pucBuf, int& nOffs, const bool bSkipStatus=false)
   {
      if (!bSkipStatus)
         dwStatus=dwUnpack(pucBuf,nOffs);
      dwDataSources=dwUnpack(pucBuf,nOffs);
      T.Unpack(pucBuf,nOffs);
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

};

//***************************************************************************

enum
{
   IFF_TRACK_SEX           = 0,
   IFF_TRACK_CANCEL_SEX    = 1,
};

class Cst4_IFF_Track : public Cst4_MessageBase
{

   BYTE ucType;
   Cst4LatLong LL;
   WORD wTgt_Track;
   WORD wTgt_ID;
   int nTgtHgt_ft;

public:

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

   bool bCancelled() const
   {
      return (ucType==IFF_TRACK_CANCEL_SEX);
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

   Cst4_IFF_Track(const BYTE* pucBuf) : Cst4_MessageBase(CST4_IFF_TRACK_MSG)
   {
      int nOffs=0;
      Unpack(pucBuf,nOffs,false);
   }

   ~Cst4_IFF_Track()
   {

   }

   void Assign(Cst4_IFF_Track& M)
   {
      Cst4_MessageBase::Assign(M);
      ucType=M.ucType;
      LL=M.LL;
      wTgt_Track=M.wTgt_Track;
      wTgt_ID=M.wTgt_ID;
      nTgtHgt_ft=M.nTgtHgt_ft;
   }

   Cst4_IFF_Track& operator = (Cst4_IFF_Track& M)
   {
      Assign(M);
      return *this;
   }

   Cst4_IFF_Track(Cst4_IFF_Track& M)
   {
      Assign(M);
	}

   DWORD dwCalcSize() const
   {
      return dwGetBaseSize()+sizeof(ucType)+sizeof(LL)+sizeof(wTgt_Track)+sizeof(wTgt_ID)+sizeof(nTgtHgt_ft);
   }

   void Pack(BYTE* pucBuf, int& nOffs)
   {
      SetSize(dwCalcSize());
      PackBase(pucBuf,nOffs);
      PackBYTE(ucType,pucBuf,nOffs);
      PackWORD(wTgt_Track,pucBuf,nOffs);
      PackWORD(wTgt_ID,pucBuf,nOffs);
      PackInt(nTgtHgt_ft,pucBuf,nOffs);
      LL.Pack(pucBuf,nOffs);
   };

   void Unpack(const BYTE* pucBuf, int& nOffs, const bool bSkipStatus=false)
   {
      UnpackBase(pucBuf,nOffs,bSkipStatus);
      ucType=ucUnpack(pucBuf,nOffs);
      wTgt_Track=wUnpack(pucBuf,nOffs);
      wTgt_ID=wUnpack(pucBuf,nOffs);
      nTgtHgt_ft=nUnpack(pucBuf,nOffs);
      LL.Unpack(pucBuf,nOffs);
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

   LatLong GetLL() const
   {
      return LL.GetLL();
   }

};


//***************************************************************************




#pragma option pop    //Set compiler back to default alignment


#endif
