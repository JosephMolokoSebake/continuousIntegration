//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Defs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int g_nCst4TimeBias_hours=100000; 	//Invalid -> must be set before use
DWORD g_dwCst4LastUpdate_utc_sec=0;
int g_nCst4LastUpdate_utc_hours=0;
JFile* g_pDebugTimeFil=NULL;	//Not Used in 4U

void UpdateCst4TimeLocalBias()
{
	int nLoc_hours=JTime(true,false).nTotalHours();
	int nUTC_hours=JTime(true,true).nTotalHours();
	g_nCst4TimeBias_hours=nUTC_hours-nLoc_hours;
}

void SetLastCst4TimeUpdate(const JTime& T)
{
	if (T.bValid())
		{
		g_nCst4LastUpdate_utc_hours=T.nTotalHours();
		g_dwCst4LastUpdate_utc_sec=T.dwTotalSec();
		if (!T.bUTC())
			{
			g_dwCst4LastUpdate_utc_sec=dwCst4ConvertToUTC(g_dwCst4LastUpdate_utc_sec);
			g_nCst4LastUpdate_utc_hours=nCst4ConvertHoursToUTC(g_nCst4LastUpdate_utc_hours);
			}
		}
	else
		{
		JTime TT(true,true);
		g_nCst4LastUpdate_utc_hours=TT.nTotalHours();
		g_dwCst4LastUpdate_utc_sec=TT.dwTotalSec();
		}
}

DWORD dwCst4ConvertToLocal(const DWORD dwT_utc_sec)
{
	assert(g_nCst4TimeBias_hours!=100000); 			//must set Bias if UTC time
	return dwT_utc_sec-g_nCst4TimeBias_hours*3600;
}

DWORD dwCst4ConvertToUTC(const DWORD dwT_loc_sec)
{
	assert(g_nCst4TimeBias_hours!=100000); 			//must set Bias if UTC time
	return dwT_loc_sec+g_nCst4TimeBias_hours*3600;
}

int nCst4ConvertHoursToUTC(const int nLoc_hours)
{
	assert(g_nCst4TimeBias_hours!=100000); 			//must set Bias if UTC time
	return nLoc_hours+g_nCst4TimeBias_hours;
}

int nCst4ConvertHoursToLocal(const int nUTC_hours)
{
	assert(g_nCst4TimeBias_hours!=100000); 			//must set Bias if UTC time
	return nUTC_hours-g_nCst4TimeBias_hours;
}

String sGetMessageDescription(const CST4_MSG_TYPE Msg)
{
	switch(Msg)
		{
		case CST4_DYNAMIC_AIS_MSG:			return "Dynamic AIS";
		case CST4_STATIC_AIS_MSG:			return "Static AIS";
		case CST4_RADAR_SCAN_MSG:			return "Radar Scan";
		case CST4_RADAR_TRACK_MSG:			return "Radar Track";
		case CST4_REQUEST_MSG:				return "Request";
		case CST4_RESPONSE_MSG:				return "Response";
		case CST4_BITE_MSG:					return "Site";
		case CST4_IFF_TRACK_MSG:			return "IFF";
		case VISTA_ADSB_MSG:					return "ADS-B";
//		case CST4_SAR_MSG_OBSOLETE: 		return "SAR";
		case CST4_RADAR_TRACK_INFO_MSG:	return "Radar Track Info";
		case CST4_USER_MSG:					return "User";
		case CST4_SYSTRACK_MSG:		 		return "System Track";
		default: 								return "Unknown Message";
		}
}


