//---------------------------------------------------------------------------
#ifndef Cst4SiteDefsH
#define Cst4SiteDefsH
//---------------------------------------------------------------------------

#include "JList.h"
#include "Cst4Messages.h"
#include "JFile.h"
#include "JMemFile.h"

class Cst4SiteDefs
{

public:

	enum SYSTEM_TYPE
	{
		UNDEFINED_TYPE			= 0,
		AIS_TYPE					= 1,
		RADAR_TYPE				= 2,
		IFF_TYPE					= 3,
		WEATHER_TYPE			= 4,
		LRIT_TYPE				= 5,
		SAT_AIS_TYPE			= 6,
		SHIP_POS_TYPE			= 7,
		TRACKING_RADAR_TYPE	= 8,
		SITE_HUB_TYPE			= 9,
		VPN_HUB_TYPE			= 10,
		LOCAL_HUB_TYPE			= 11,
		DISPLAY_TYPE			= 12,
		REPLAY_TYPE				= 13,
		CAMERA_TYPE		  		= 14,
		ALARMPROC_TYPE	  		= 15,
	};


	enum
	{
		COUNTRY_MASK      = 0x000F,   //After shift
		COUNTRY_SHIFT     = 12,
		MAX_COUNTRY_CNT   = 16,

		LOCATION_MASK     = 0x00FF,   //After shift
		LOCATION_SHIFT    = 4,
		MAX_LOCATION_CNT  = 256,

      TYPE_MASK         = 0x000F,   //After shift
      TYPE_SHIFT        = 0,
		MAX_TYPE_CNT      = 16,

      SITEDEFS_VERSION	= 2000,

	};

   enum
   {
      TEMP_COUNTRY 	= 6,
      TEMP_LOC			= 63,
      TEMP_AIS_IDX	=0,
      TEMP_RADAR_IDX	=1,
   };

private:

	struct TypeStruct
	{
      String sName;
      WORD wID;
		int nSurfID;
		String sSurfExt;
		LatLong LL;
		int nHeight_m;

		int nType;				//New with version 2
		double dRadius_km;   //New with version 2
		String sDesigAddr;  //New with version 2
		int nDesigPort;				//New with version 2

		int anExtra[16];		//New with version 2

      TypeStruct()
		{
			sName="";
			wID=0;
			nSurfID=0;
			sSurfExt="";
			LL.Invalidate();
			nHeight_m=0;
			nType=0;
			dRadius_km=0.0;
			sDesigAddr="";
			nDesigPort=0;
			memset(anExtra,0,sizeof(anExtra));
		}


		TypeStruct& operator = (const TypeStruct& T)
		{
			sName=T.sName;
			wID=T.wID;
			nSurfID=T.nSurfID;
			sSurfExt=T.sSurfExt;
			LL=T.LL;
			nHeight_m=T.nHeight_m;

			nType=T.nType;
			dRadius_km=T.dRadius_km;
			sDesigAddr=T.sDesigAddr;
			nDesigPort=T.nDesigPort;
			memcpy(anExtra,T.anExtra,sizeof(anExtra));
			return *this;
		}

		JFile::FILE_ERROR Store(JFile* pFil);
		bool bRead(JMemFile* pFil, const DWORD dwVer);

		bool operator < (const TypeStruct& T) const
		{
			return (sName.CompareIC(T.sName)<0);
		}

		bool operator > (const TypeStruct& T) const
		{
			return (sName.CompareIC(T.sName)>0);
		}


	};

   struct LocStruct
   {
      String sName;
      WORD wID;
      JList<TypeStruct> TypeList;
		WORD awTypeIdx[MAX_TYPE_CNT];

		JFile::FILE_ERROR Store(JFile* pFil);
		bool bRead(JMemFile* pFil, const DWORD dwVer);

		bool operator < (const LocStruct& L) const
		{
			return (sName.CompareIC(L.sName)<0);
		}

		bool operator > (const LocStruct& L) const
		{
			return (sName.CompareIC(L.sName)>0);
		}


	};

	struct CountryStruct
	{
		String sName;
		WORD wID;
		JList<LocStruct> LocList;
		WORD awLocIdx[MAX_LOCATION_CNT];

		JFile::FILE_ERROR Store(JFile* pFil);
		bool bRead(JMemFile* pFil, const DWORD dwVer);

	};

	JList<CountryStruct> CountryList;
	WORD awCountryIdx[MAX_COUNTRY_CNT];

	LatLong GetLL(const String sLat, const String sLong);

   String sFolder;

   int nVersion;

public:

	Cst4SiteDefs();
	~Cst4SiteDefs();

	bool bLoadXLSList(const String sFile);

	bool bLoadSites5();
	bool bStore(const bool bEncrypt=true);

	int nGetCountryCnt()
	{
		return CountryList.nGetCount();
	}

	String sGetCountry(const int nCountryIdx)
	{
		return CountryList[nCountryIdx]->sName;
	}

	int nGetCountryID(const int nCountryIdx)
	{
		return CountryList[nCountryIdx]->wID;
	}

	int nGetLocCnt(const int nCountryIdx)
	{
		return CountryList[nCountryIdx]->LocList.nGetCount();
	}

	String sGetLocation(const int nCountryIdx, const int nLocIdx)
	{
		return CountryList[nCountryIdx]->LocList[nLocIdx]->sName;
	}

   int nGetTypeCnt(const int nCountryIdx, const int nLocIdx)
   {
      return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList.nGetCount();
   }

   String sGetType(const int nCountryIdx, const int nLocIdx, const int nTypeIdx)
   {
      return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sName;
   }

	LatLong GetLL(const int nCountryIdx, const int nLocIdx, const int nTypeIdx)
	{
      return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->LL;
   }

	int nGetHeight_m(const int nCountryIdx, const int nLocIdx, const int nTypeIdx)
	{
      return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->nHeight_m;
   }

	WORD wGetIDFromRaw(const WORD wCountry, const WORD wLoc, const WORD wType)
   {
      WORD wID=0;
		wID|=(wCountry<<COUNTRY_SHIFT);
      wID|=(wLoc<<LOCATION_SHIFT);
		wID|=(wType<<TYPE_SHIFT);
		return wID;
	}

	WORD wGetID(const int nCountryIdx, const int nLocIdx, const int nTypeIdx)
	{
		WORD wID=0;
		wID|=(CountryList[nCountryIdx]->wID<<COUNTRY_SHIFT);
		wID|=(CountryList[nCountryIdx]->LocList[nLocIdx]->wID<<LOCATION_SHIFT);
		wID|=(CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->wID<<TYPE_SHIFT);
		return wID;
	}

	void GetIndexFromID(const WORD wID, int& nCountryIdx, int& nLocIdx, int& nTypeIdx);

	String sGetDesigAddrFromID(const WORD wSiteID);
	int nGetDesigPortFromID(const WORD wSiteID);

	bool bDesigAvail(const WORD wSiteID)
	{
   	String sAddr=sGetDesigAddrFromID(wSiteID);
      int nPort=nGetDesigPortFromID(wSiteID);
		return (!sAddr.IsEmpty() && (nPort>0));

	}

	LatLong GetLLFromID(const WORD wSiteID);


	SYSTEM_TYPE GetSystemType(const int nCountryIdx, const int nLocIdx,const int nTypeIdx);

	SYSTEM_TYPE GetSystemTypeFromID(const WORD wSiteID);

   double dGetRadius_km(const int nCountryIdx, const int nLocIdx,const int nTypeIdx)
   {
   	double dRad_km=0.0;
		if (nCountryIdx>=0)
         dRad_km=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->dRadius_km;
		return dRad_km;
   }

	String sGetDescrFromID(const WORD wSiteID);
	String sGetNameOnlyFromID(const WORD wSiteID);

	String sGetAbbrFromID(const WORD wSiteID);
	String sGetDescrFileFromID(const WORD wSiteID);


	String sGetLocationFromID(const WORD wSiteID);
	String sGetTypeFromID(const WORD wSiteID);

	int nGetCountryFromID(const WORD wSiteID) const
	{
		return awCountryIdx[(wSiteID>>COUNTRY_SHIFT)&COUNTRY_MASK];
	}

	WORD wStripType(const WORD wSiteID) const
	{
		return (wSiteID & (~TYPE_MASK));
	}

	String sGetCountryFromID(const WORD wSiteID)
	{
		int n=nGetCountryFromID(wSiteID);
		return CountryList[n]->sName;
	}

	WORD wGetIDFromSurfID(const int nSurfID);
	WORD wGetIDFromSurfExt(const String sSurfExt);
	String sGetExtFromSurfID(const int nSurfID);

	Polar GetRaBe(const WORD wSiteID,const LatLong& TargetLL);

   bool bIsSensor(const WORD wSiteID);
	bool bIsSensor(const int nCountryIdx, const int nLocIdx,const int nTypeIdx);

	int nGetVersion() const
	{
      return nVersion;
	}

	double dGetVersion() const
	{
      return nVersion*0.001;
	}

   String sGetSiteDefFolder() const
   {
      return sFolder;
   }

   void GetTempIndexes(int& nCountryIdx, int& nLocIdx);
   String sGetTempLocation();
   void GetTempAISPar(LatLong& LL, int& nHeight_m);
   void GetTempRadarPar(LatLong& LL, int& nHeight_m);

   void SetTempLocation(const String sLoc);
   void SetTempAISPar(const LatLong LL, const int nHeight_m);
   void SetTempRadarPar(const LatLong LL, const int nHeight_m);

	void ReadTempFile(const String sFil,
   							String& sLoc,
                        LatLong& AISLL, int& AISHeight_m,
   	LatLong& RadarLL, int& nRadarHeight_m);


   void SetTmpFromFile();

};


String sGetCst4Site(const WORD wSiteID);
String sGetCst4SiteNameOnly(const WORD wSiteID);
String sGetCst4SiteName(const WORD wSiteID);
String sGet_SiteFile_Name();
String sGetCst4FileName(const WORD wSiteID);

extern Cst4SiteDefs* g_pSiteDefs;
void g_LoadSiteDefs5();
void g_CloseSiteDefs5();

#endif
