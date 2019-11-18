//---------------------------------------------------------------------------

#ifndef Cst4TimeMsgH
#define Cst4TimeMsgH
//---------------------------------------------------------------------------

#include "JTime.h"

class Cst4TimeMsg
{

	WORD wMsg;
	JTime T_utc;
	DWORD dwT_utc_sec;
	TCriticalSection* pLock;
	int nRXCnt;
	WORD wRXData;

	Cst4TimeMsg(const Cst4TimeMsg& T);
	Cst4TimeMsg& operator = (const Cst4TimeMsg& T);

public:

	//Predefined messages
	enum CST4_TIME_WIN_MSG
	{
		CST4_TIME_GPS_SOURCE			= 0,
		CST4_TIME_LAST_GPS_UPDATE	= 1,
		CST4_TIME_CURRENT				= 2,
		CST4_TIME_NOTIFY				= 3,
		CST4_TIME_PROCWATCH			= 4,
	};

	enum
	{
		CST4_RX_WATCHDOG	= 1,
		CST4_TX_WATCHDOG	= 2,
		CST4_KEEPALIVE		= 3,
		CST4_REBOOT			= 4,
	};

	//Use predefined description
	Cst4TimeMsg(const CST4_TIME_WIN_MSG MsgDescr); 	//New first in MainFormCreate

	//Use any description string
	Cst4TimeMsg(const String sMsgDescr); 			//New first in MainFormCreate

	~Cst4TimeMsg();								//Delete last in MainFormDestroy

	void TXTime(const JTime& GPSTime_utc, const WORD wData=0);

	//Use in ProcessMessages
	bool bServiceMsg(TMsg& message, bool& handled, const bool bAutoUpdate=true);

	JTime GetTime_utc() const
	{
		JTime T;
		pLock->Acquire();
		T=T_utc;
		pLock->Release();
		return T;
	}

	DWORD dwGetTime_utc_sec() const
	{
		DWORD dwT;
		pLock->Acquire();
		dwT=dwT_utc_sec;
		pLock->Release();
		return dwT;
	}

	WORD wGetRXData() const
	{
		WORD w;
		pLock->Acquire();
		w=wRXData;
		pLock->Release();
		return w;
	}

	int nGetRXCnt() const
	{
		int nCnt;
		pLock->Acquire();
		nCnt=nRXCnt;
		pLock->Release();
		return nCnt;
	}

	int nGetTimeAge_hours(const DWORD dwMsgTime_utc_sec);


};

#endif
