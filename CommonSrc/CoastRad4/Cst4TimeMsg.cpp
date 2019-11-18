//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4TimeMsg.h"
#include "Cst4Messages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Cst4TimeMsg::Cst4TimeMsg(const CST4_TIME_WIN_MSG MsgDescr)
{
	String sDescr;
	switch(MsgDescr)
		{
		case CST4_TIME_GPS_SOURCE:
			sDescr="Cst4_GPS_Time_Source_Update_Message";
			break;
		case CST4_TIME_LAST_GPS_UPDATE:
			sDescr="Cst4_Last_GPS_Time_Update_Message";
			break;
		case CST4_TIME_CURRENT:
			sDescr="Cst4_Current_Time_Message";
			break;
		case CST4_TIME_NOTIFY:
			sDescr="Cst4_Notify_Message";
			break;
		case CST4_TIME_PROCWATCH:
			sDescr="Cst4_ProcWatch_Message";
			break;
		default:
			sDescr="Cst4_Unknown_Message";
		}
	pLock=new TCriticalSection;
	wMsg=::RegisterWindowMessage(sDescr.c_str());
	T_utc.Invalidate();
	dwT_utc_sec=0;
	nRXCnt=0;
	wRXData=0xFFFF;
}

Cst4TimeMsg::Cst4TimeMsg(const String sMsgDescr)
{
	pLock=new TCriticalSection;
	wMsg=::RegisterWindowMessage(sMsgDescr.c_str());
	T_utc.Invalidate();
	dwT_utc_sec=0;
	nRXCnt=0;
	wRXData=0xFFFF;
}

Cst4TimeMsg::~Cst4TimeMsg()
{
	delete pLock;
}

Cst4TimeMsg::Cst4TimeMsg(const Cst4TimeMsg& T)
{
	pLock=new TCriticalSection;
	wMsg=T.wMsg;
	T_utc=T.T_utc;
	dwT_utc_sec=T.dwT_utc_sec;
	wRXData=T.wRXData;
	nRXCnt=T.nRXCnt;
}

Cst4TimeMsg& Cst4TimeMsg::operator = (const Cst4TimeMsg& T)
{
	pLock=new TCriticalSection;
	wMsg=T.wMsg;
	T_utc=T.T_utc;
	dwT_utc_sec=T.dwT_utc_sec;
	wRXData=T.wRXData;
	nRXCnt=T.nRXCnt;
	return *this;
}

void Cst4TimeMsg::TXTime(const JTime& GPSTime_utc, const WORD wData)
{
	Cst4Time T4(GPSTime_utc);
	WORD wDate;
	DWORD dwT;
	T4.GetNativePar(wDate,dwT);
	::PostMessage(HWND_BROADCAST,wMsg,(WPARAM)((wData<<16)|wDate),(LPARAM)dwT);
}

bool Cst4TimeMsg::bServiceMsg(TMsg& message, bool& handled, const bool bAutoUpdate)
{
	if ((int)message.message==(int)wMsg)
		{
		pLock->Acquire();
		Cst4Time T4;
		T4.SetNativePar(message.wParam&0x0000FFFF,message.lParam);
		wRXData=(WORD)(message.wParam>>16);
		T_utc=T4.Get_utc();
		dwT_utc_sec=T_utc.dwTotalSec();
		++nRXCnt;
		pLock->Release();
		handled=true;
		if (bAutoUpdate)
			return T_utc.bUpdatePCTime();
		return true;
		}
	return false;
}

int Cst4TimeMsg::nGetTimeAge_hours(const DWORD dwMsgTime_utc_sec)
{
	int nTimeAge_hours;
	DWORD dwLast_utc_sec=dwGetTime_utc_sec();
	if (dwLast_utc_sec>0)
		{
		nTimeAge_hours=(dwMsgTime_utc_sec-dwLast_utc_sec)/3600;
		if (nTimeAge_hours>7)
			nTimeAge_hours=7;
		}
	else
		nTimeAge_hours=7;
	return nTimeAge_hours;
}



