//---------------------------------------------------------------------------
#ifndef Cst4DefsH
#define Cst4DefsH
//---------------------------------------------------------------------------

#include "JTransform.h"
#include "JTime.h"
#include "JFile.h"

#pragma option push -a4    //Set compiler to DWORD alignment

enum CST4_DATA_PORTS
{
	CST4_PORT_BASE                = 20177,
	CST4_PLOT_MAINTENANCE_PORT    = CST4_PORT_BASE+0,
	CST4_HUB_MAINTENANCE_PORT     = CST4_PORT_BASE+1,
	CST4_NOTIFY_PORT              = CST4_PORT_BASE+2,
	CST4_SOURCE_MAINTENANCE_PORT  = CST4_PORT_BASE+3,  //AISTrack4 or RadarScan
	CST4_SITEHUB_MAINTENANCE_PORT = CST4_PORT_BASE+4,
	CST4_PROCWATCH_PORT           = CST4_PORT_BASE+5,
	CST4_RADARSCAN_RX_PORT        = CST4_PORT_BASE+6,
	CST4_WINDSERVER_PORT        	= CST4_PORT_BASE+7,
	CST4_SYSMSG_PORT        		= CST4_PORT_BASE+8,

	CST4_UDP_PORT_BASE				= CST4_PORT_BASE+16,

	CST4_UDP_DATA_PORT		      = CST4_UDP_PORT_BASE+0,
	CST4_UDP_TIME_PORT		      = CST4_UDP_PORT_BASE+1,
	CST4_UDP_SURF_PORT		      = CST4_UDP_PORT_BASE+2,
	CST4_UDP_PROCWATCH_PORT		   = CST4_UDP_PORT_BASE+3,
	CST4_UDP_TIMEAGE_PORT	      = CST4_UDP_PORT_BASE+4,
	CST4_UDP_NOTIFY_PORT	      	= CST4_UDP_PORT_BASE+5,

	CST4_SITEHUB_UDP_PORT_BASE		= CST4_UDP_PORT_BASE+16,

	CST4_UDP_AISTRACK_PORT		   = CST4_SITEHUB_UDP_PORT_BASE+0,
	CST4_UDP_RADARSCAN_PORT		   = CST4_SITEHUB_UDP_PORT_BASE+1,
	CST4_UDP_IFFDESIG_PORT		 	= CST4_SITEHUB_UDP_PORT_BASE+2,
	CST4_UDP_ADD1_PORT		   	= CST4_SITEHUB_UDP_PORT_BASE+3,
	CST4_UDP_ADD2_PORT		   	= CST4_SITEHUB_UDP_PORT_BASE+4,
	CST4_UDP_ADD3_PORT		   	= CST4_SITEHUB_UDP_PORT_BASE+5,

	CST4_SITEHUB_UDP_PORT_CNT		= 6,

	VISTA_AIS_2_SITEHUB_PORT   		=  CST4_PORT_BASE+32,
	VISTA_RSCAN_2_SITEHUB_PORT   		=  CST4_PORT_BASE+33,
	VISTA_IFF_2_SITEHUB_PORT   		=  CST4_PORT_BASE+34,

	VISTA_SITERX_2_CONSOL_PORT     	=  CST4_PORT_BASE+35,

	VISTA_CONSOL_2_SYSTRACK_PORT  	=  CST4_PORT_BASE+36,

	VISTA_CONSOL_2_DISTR_PORT     	=  CST4_PORT_BASE+38,
	VISTA_SYSTRACK_2_DISTR_PORT    	=  CST4_PORT_BASE+39,

	VISTA_DISTR_2_ALARM_PORT   		=  CST4_PORT_BASE+40,

	VISTA_ALARM_2_LOCALHUB_PORT   	=  CST4_PORT_BASE+41,
	VISTA_LOCALHUB_2_LOCALHUB_PORT  	=  CST4_PORT_BASE+42,
	VISTA_LOCALHUB_2_VISTAPLOT_PORT	=  CST4_PORT_BASE+43,
	VISTA_SPIDER_2_SITEHUB_PORT     	=  CST4_PORT_BASE+44,

	VISTA_USBSCAN_2_SITEHUB_PORT   	=  CST4_PORT_BASE+45,
	VISTA_SITEHUB_2_SITERX_PORT   	=  CST4_PORT_BASE+46,

	VISTA_GPS_2_SITEHUB_PORT   		=  CST4_PORT_BASE+47,

	VISTA_DISTR_2_CHAKA_PORT   		=  CST4_PORT_BASE+48,
	VISTA_DISTR_2_CSIR_PORT   			=  CST4_PORT_BASE+49,
	VISTA_DISTR_2_EXTRA_PORT    		=  CST4_PORT_BASE+50,

	VISTA_CYCLOPE_2_SITEHUB_PORT     =  CST4_PORT_BASE+51,
	VISTA_ADSB_2_SITEHUB_PORT     	=  CST4_PORT_BASE+52,
	VISTA_SAR_2_SITEHUB_PORT     		=  CST4_PORT_BASE+53,

	VISTA_PROCWATCH_PORT     			=  CST4_PORT_BASE+54,

};


enum CST4_MSG_TYPE
{
	CST4_NO_MSG						= 0,
	CST4_DYNAMIC_AIS_MSG			= 1,
	CST4_STATIC_AIS_MSG			= 2,
   CST4_RADAR_SCAN_MSG			= 3,
	CST4_RADAR_TRACK_MSG			= 4,
   CST4_REQUEST_MSG				= 5,
   CST4_RESPONSE_MSG				= 6,
	CST4_BITE_MSG					= 7,
	CST4_IFF_TRACK_MSG			= 8,
	CST4_SAR_MSG_OBSOLETE	 	= 9,
	CST4_RADAR_TRACK_INFO_MSG	= 10,
	CST4_USER_MSG					= 11,
	CST4_SYSTRACK_MSG				= 12,
	VISTA_ALARM_CFG_MSG        = 13,
	VISTA_ALARM_TRIGGER_MSG    = 14,
	VISTA_TIME_MSG    			= 16,
   VISTA_ADSB_MSG         		= 17,

	CST4_MSG_CNT					= 18,
};

enum CST4_INFO_REQUEST_TYPE
{
	CST4_REQUEST_SERVER_PORT	= 0,
	CST4_REQUEST_TIME_SYNC		= 1,
	CST4_REQUEST_NOTIFY			= 2,
	CST4_REQUEST_RX_WATCHDOG	= 3,
	CST4_REQUEST_REBOOT			= 4,
	CST4_REQUEST_BITE_DATA		= 5,
	CST4_REQUEST_TX_WATCHDOG	= 6,
	CST4_REQUEST_SHUTDOWN		= 7,
	CST4_REQUEST_SOURCE_UPDATE	= 8,
	CST4_REQUEST_SITE_UPDATE	= 9,
	CST4_REQUEST_WIND_INFO		= 10,
	CST4_REQUEST_ODCS_START		= 16,	//Use offsets from 0 to 99
	CST4_REQUEST_ODCS_MAX		= CST4_REQUEST_ODCS_START+99,

};


//enum CST4_PERSONALITY
//{
//	CST4_COASTRAD_PERS,
//	CST4_VISTA_PERS,
//};

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

	int nGetLat() const
	{
		return nLat;
	}

	int nGetLong() const
	{
		return nLong;
	}

	void InitMax()
	{
		nLat=-360.0*60000.0;
		nLong=-360.0*60000.0;
	}

	void UpdateMax(const Cst4LatLong& LL)
	{
		if (LL.nGetLat()>nLat)
			nLat=LL.nGetLat();
		if (LL.nGetLong()>nLong)
			nLong=LL.nGetLong();
	}


	void InitMin()
	{
		nLat=360.0*60000.0;
		nLong=360.0*60000.0;
	}

	void UpdateMin(const Cst4LatLong& LL)
	{
		if (LL.nGetLat()<nLat)
			nLat=LL.nGetLat();
		if (LL.nGetLong()<nLong)
			nLong=LL.nGetLong();
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

	void SetLL(const LatLong& _LL)
	{
		SetLat(_LL.dLat);
		SetLong(_LL.dLong);
	}

   LatLong GetLL() const
   {
      return LatLong(dLat(),dLong());
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

extern int g_nCst4TimeBias_hours;
extern int g_nCst4LastUpdate_utc_hours;
extern DWORD g_dwCst4LastUpdate_utc_sec;

extern void UpdateCst4TimeLocalBias();
extern void SetLastCst4TimeUpdate(const JTime& T=JTime());

extern DWORD dwCst4ConvertToLocal(const DWORD dwT_utc_sec);
extern DWORD dwCst4ConvertToUTC(const DWORD dwT_loc_sec);
extern int nCst4ConvertHoursToUTC(const int nLoc_hours);
extern int nCst4ConvertHoursToLocal(const int nUTC_hours);

class Cst4Time
{
	enum
	{
		TIME_UTC_ZONE			= 0x80000000,
		TIME_AGE_MASK   		= 0x70000000,
		TIME_AGE_OFFS   		= 28,
		TIME_MASK         	= 0x03FFFFFF,
		TIME_AND_ZONE_MASK 	= 0x83FFFFFF,
	};

	WORD wDate;
	DWORD dwT;     //bit 25..0 	= time of day  in 10ms steps
						//bit 27..26	= spare
						//bit 30..28 	= time update age (hours)
						//bit 31 		= time zone (local=0/utc=1)

	JTime Get(const bool bGetUTC) const
	{
		JTime T;
		T.SetTime_ms((dwT&TIME_MASK)*10);
		T.SetDCSDate(wDate);
		if (bGetUTC)
			{
			if ((dwT&TIME_UTC_ZONE)==0)
				T.ForceConvertToUTC(g_nCst4TimeBias_hours);
			T.SetUTC();
			}
		else
			{
			if (dwT&TIME_UTC_ZONE)
				T.ForceConvertToLocal(g_nCst4TimeBias_hours);
			}
		return T;
	}



public:


	JTime Get_raw() const
	{
		JTime T;
		T.SetTime_ms((dwT&TIME_MASK)*10);
		T.SetDCSDate(wDate);
		if (bUTC())
			T.SetUTC();
		return T;
	}

	void SetUTC()
	{
		dwT|=TIME_UTC_ZONE;		//Set Zone bit
	}

	void SetLocal()
	{
		dwT&=(~TIME_UTC_ZONE);	//Clear Zone bit
	}

	bool bUTC() const
	{
		return (dwT&TIME_UTC_ZONE)!=0;
	}

	bool bLocal() const
	{
		return (dwT&TIME_UTC_ZONE)==0;
	}

	void SetAge(const int nUpdateAge_hours)
	{
		if (nUpdateAge_hours>=7)
			dwT|=TIME_AGE_MASK;  //set age to 7 hours
		else
			{
			dwT&=TIME_AND_ZONE_MASK;
			if (nUpdateAge_hours>0)
				dwT|=(nUpdateAge_hours<<TIME_AGE_OFFS);
			}
	}

	int nGetAge_hours() const
	{
		return (int)((dwT&TIME_AGE_MASK)>>TIME_AGE_OFFS);
	}

	void SetTime(const JTime& T, const int nUpdateAge_hours=0)
	{
		wDate=T.wGetDCSDate();
		dwT=(T.nMilliSec()/10)&TIME_MASK;
		if (T.bUTC())
			SetUTC();
		else
			SetLocal();
		SetAge(nUpdateAge_hours);
	}

	void SetCurrentTime(const bool bSetUTC=false)
	{
		JTime T;
		int nAge_hours=7;
		if (!bSetUTC)
			{
			T=JTime(true,false);
			if (g_nCst4LastUpdate_utc_hours!=0)
				nAge_hours=nCst4ConvertHoursToUTC(T.nTotalHours())-g_nCst4LastUpdate_utc_hours;
			}
		else
			{
			T=JTime(true,true);
			if (g_nCst4LastUpdate_utc_hours!=0)
				nAge_hours=T.nTotalHours()-g_nCst4LastUpdate_utc_hours;
			}
		SetTime(T,nAge_hours);
	}


	Cst4Time()
	{
		wDate=0;   //Invalidate
	}

	Cst4Time(const bGetCurrentTime, const bool bSetUTC=false)
	{
		if (bGetCurrentTime)
			SetCurrentTime(bSetUTC);
		else
			wDate=0;   //Invalidate
	}

	Cst4Time(const JTime& T, const int nUpdateAge_hours=0)
	{
		SetTime(T,nUpdateAge_hours);
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

	bool bSame(const Cst4Time& T) const
	{
      return (dwT==T.dwT)&&(wDate==T.wDate);
	}

   void ConvertToUTC()
	{
		if ((dwT&TIME_UTC_ZONE)==0)
			{
			JTime T;
			T.SetTime_ms((dwT&TIME_MASK)*10);
			T.SetDCSDate(wDate);
			T.ForceConvertToUTC(g_nCst4TimeBias_hours);
			wDate=T.wGetDCSDate();
			dwT&=TIME_AGE_MASK;			//Keep data age info
			dwT|=((T.nMilliSec()/10)&TIME_MASK);
			SetUTC();
			}
	}

	void ConvertToLocal()
	{
		if ((dwT&TIME_UTC_ZONE)!=0)
			{
			JTime T;
			T.SetTime_ms((dwT&TIME_MASK)*10);
			T.SetDCSDate(wDate);
			T.ForceConvertToLocal(g_nCst4TimeBias_hours);
			wDate=T.wGetDCSDate();
			dwT&=TIME_AGE_MASK;			//Keep data age info
			dwT|=((T.nMilliSec()/10)&TIME_MASK);
			SetLocal();
			}
	}


	JTime Get_loc() const
	{
		return Get(false);
	}

	JTime Get_utc() const
	{
		return Get(true);
	}

	double dSec_loc() const
	{
		return Get(false).dTotalSec();
	}

	double dSec_utc() const
	{
		return Get(true).dTotalSec();
	}

	DWORD dwSec_loc() const
	{
		return Get(false).dwTotalSec();
	}

	DWORD dwSec_utc() const
	{
		return Get(true).dwTotalSec();
	}

	double dSec_raw() const		//Ignore time zone -> used for comparisons only
	{
		JTime T;
		T.SetTime_ms((dwT&TIME_MASK)*10);
		T.SetDCSDate(wDate);
		return T.dTotalSec();
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

	void GetNativePar(WORD& _wDate, DWORD& _dwT)
	{
		_wDate=wDate;
		_dwT=dwT;
	}

	void SetNativePar(const WORD _wDate, const DWORD _dwT)
	{
		wDate=_wDate;
		dwT=_dwT;
	}


};

//***************************************************************************

enum CST4_PLOT_TYPE
{
	CST4_PLOT       		= 0,

//MRR Specific
	CST4_MRR_SEC        	= 20,
	CST4_MRR_ASSOC      	= 21,
	CST4_MRR_PSR_AIR    	= 22,
	CST4_MRR_PSR_3D     	= 23,
	CST4_MRR_PSR_CMS    	= 24,
	CST4_MRR_PSR_MEM    	= 25,
	CST4_MRR_PSR_SURF   	= 26,
	CST4_MRR_PSR_TWS    	= 27,
	CST4_MRR_SPLASH 		= 28,
};


class Cst4Plot
{

	enum
	{
		RA_MASK     = 0x007FFFFF,
		PLOT_USED   = 0x00800000,
		TYPE_MASK	= 0xFF000000,
		TYPE_SHIFT	= 24,
	};

	DWORD dwRa;		//Max Range = 8388608 m
	WORD wBe;

public:


	Cst4Plot()
	{
		dwRa=0;
	}

	Cst4Plot(const double dRa_m, const double dBe_deg)
	{
		SetRaBe(dRa_m,dBe_deg);
	}

	Cst4Plot(const Cst4Plot& P)
	{
		dwRa=P.dwRa;
		wBe=P.wBe;
	}

	~Cst4Plot()
	{
	}

	void Assign(const Cst4Plot& P)
	{
		*this=P;
	}

	Cst4Plot& operator = (const Cst4Plot& P)
	{
		dwRa=P.dwRa;
		wBe=P.wBe;
		return *this;
	}

	void Invalidate()		{dwRa=0;				}
	bool bValid() const  {return dwRa!=0;	}

	void SetRaBe(const double dRa_m, const double dBe_deg)
	{
		dwRa=dRa_m+0.5;
		wBe=dBe_deg*65536.0/360.0;
	}

	double dRa_m() const		{return (dwRa&RA_MASK);		}
	double dBe_deg() const	{return wBe*360.0/65536.0;	}

	void SetPlotType(const CST4_PLOT_TYPE Type)
	{
		dwRa&=(~TYPE_MASK);
		dwRa|=(Type<<TYPE_SHIFT);
	}

	CST4_PLOT_TYPE PlotType() const	{return (CST4_PLOT_TYPE)((dwRa&TYPE_MASK)>>TYPE_SHIFT);	}

	void SetUsed()			{dwRa|=PLOT_USED;					}
	void ClearUsed()		{dwRa&=RA_MASK;					}
	bool bUsed() const	{return ((dwRa&PLOT_USED)!=0);}

};

//***************************************************************************

class Cst4Grid
{

	enum
	{
		GRID_RES				= 10,  			//0.1 deg (~10km)
		MAX_GRID_X			= 360*GRID_RES-1,
		MAX_GRID_Y			= 180*GRID_RES-1,
		GRID_INVALID		= 0xFFFFFFFF,
		GRID_VISIBLE		= 0x80000000,
		GRID_VAL_MASK	 	= 0x00007FFF,	//after shift
		GRID_COORD_MASK	= 0x7FFFFFFF,
	};

	DWORD dwC;

	bool bInRange(const int nMin, const int nMax, const int nG) const
	{
		if (nMin>nMax)
			return (nG>=nMax)||(nG<=nMin);
		else
			return (nG>=nMin)&&(nG<=nMax);
	}


public:

	Cst4Grid()
	{
		dwC=GRID_INVALID;
	}

	Cst4Grid(const Cst4LatLong& LL)
	{
		SetGridLL(LL);
	}

	Cst4Grid(const LatLong& LL)
	{
		SetGridLL(LL);
	}

	Cst4Grid(const DWORD dwCoord)
	{
		dwC=dwCoord;
	}

	~Cst4Grid()
	{
	}

	void SetGridVisible()		{dwC|=GRID_VISIBLE;					}
	void ClearGridVisible()		{dwC&=(~GRID_VISIBLE);				}
	bool bGridVisible() const	{return (dwC&GRID_VISIBLE)!=0;	}

	Cst4Grid(const Cst4Grid& G)
	{
		dwC=G.dwC;
	}

	void Assign(const Cst4Grid& G)
	{
		*this=G;
	}

	Cst4Grid& operator = (const Cst4Grid& G)
	{
		dwC=G.dwC;
		return *this;
	}

	void InvalidateGrid()	{dwC=GRID_INVALID;						}
	bool bGridValid() const	{return (dwC!=(DWORD)GRID_INVALID);	}

	void SetGridLL(const Cst4LatLong& LL)	{SetGridLL(LL.GetLL());	}
	void SetGridLL(const LatLong& LL)
	{
		DWORD dwY=(LL.dLat+90.0)*GRID_RES;
		if (dwY>=MAX_GRID_Y)
			dwY=MAX_GRID_Y;
		DWORD dwX;
		if (LL.dLong<0.0)
			dwX=(LL.dLong+360.0)*GRID_RES;
		else
			dwX=LL.dLong*GRID_RES;
		if (dwX>=MAX_GRID_X)
			dwX=MAX_GRID_X;
		dwC=(dwY<<16)|dwX;
	}

	int nGridY() const			{return (int)((dwC>>16)&GRID_VAL_MASK);}
	int nGridX() const			{return (int)(dwC&GRID_VAL_MASK);		}
	DWORD dwGridCoord() const	{return dwC&GRID_COORD_MASK;				}

	bool bGridVisible(const Cst4Grid& TopLeft, const Cst4Grid& BotRight)
	{
		ClearGridVisible();
		if (TopLeft.dwGridCoord()==BotRight.dwGridCoord())
			{
			if (dwGridCoord()==TopLeft.dwGridCoord())
				{
				SetGridVisible();
				return true;
				}
			return false;
			}
		if (TopLeft.nGridX()==BotRight.nGridX())
			{
			if (bInRange(BotRight.nGridY(),TopLeft.nGridY(),nGridY()))
				{
				SetGridVisible();
				return true;
				}
			return false;
			}
		if (TopLeft.nGridY()==BotRight.nGridY())
			{
			if (bInRange(TopLeft.nGridX(),BotRight.nGridX(),nGridX()))
				{
				SetGridVisible();
				return true;
				}
			return false;
			}
		if (bInRange(BotRight.nGridY(),TopLeft.nGridY(),nGridY()) && bInRange(TopLeft.nGridX(),BotRight.nGridX(),nGridX()))
			{
			SetGridVisible();
			return true;
			}
		return false;
	}

	bool bGridVisible(const LatLong& TopLeftLL, const LatLong& BotRightLL)
	{
		Cst4Grid TopLeft(TopLeftLL);
		Cst4Grid BotRight(BotRightLL);
		return bGridVisible(TopLeft,BotRight);
	}

	bool bGridClose(const Cst4Grid& G) const
	{
		if (dwGridCoord()==G.dwGridCoord())
			return true;
		if (abs(nGridY()-G.nGridY())<2)
			{
			int nDiffX=abs(nGridX()-G.nGridX());
			if ((nDiffX<2)||(nDiffX==MAX_GRID_X))
				return true;
			}
		return false;
	}

};

#pragma option pop    //Set compiler back to default alignment

//extern JFile* g_pDebugTimeFil; 	//Not Used in 4U
extern String sGetMessageDescription(const CST4_MSG_TYPE Msg);


#endif
