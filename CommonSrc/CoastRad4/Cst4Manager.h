//---------------------------------------------------------------------------

#ifndef Cst4ManagerH
#define Cst4ManagerH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "VistaAlarmMessages.h"
#include "VistaTimeMessage.h"
#include "JFile.h"

class Cst4Manager
{

   enum
   {
		CST4_DATA_VERSION = 10000,
		MAX_RX_SIZE 		= 65536,
	};



#pragma option push -a4    //Set compiler to DWORD alignment

   struct FileHeaderStruct
   {
      DWORD dwDataVersion;
      DWORD dwSWVersion;
		JTime T1_utc;
		JTime T2_utc;

		DWORD dwAIS_D_Size;
		DWORD dwAIS_S_Size;
		DWORD dwRadar_S_Size;
		DWORD dwRadar_T_Size;


      int nAIS_D_cnt;
      int nAIS_S_cnt;
      int nRadar_S_cnt;
      int nRadar_T_cnt;

		DWORD dwIFF_T_Size1;
      int nIFF_T_cnt1;

		DWORD dwSysTrack_Size;
		int nSysTrack_cnt;

		DWORD dwUser_Size;
		int nUser_cnt;

		DWORD dwAlarmCfg_Size;
		int nAlarmCfg_cnt;

		DWORD dwAlarmTrig_Size;
		int nAlarmTrig_cnt;

		DWORD dwADSB_T_Size;
		int nADSB_T_cnt;

		DWORD adwDummy[118];

		void Init()
		{
			memset(this,0,sizeof(FileHeaderStruct));
			T1_utc.Invalidate();
			T2_utc.Invalidate();
		}

		FileHeaderStruct()
		{
			Init();
		}


	} Header;

#pragma option pop    //Set compiler back to default alignment


	JList<Cst4_AIS_Dynamic> AIS_D;
	JList<Cst4_AIS_Static> AIS_S;
	JList<Cst4_Radar_Scan> Radar_S;
	JList<Cst4_Radar_Track> Radar_T;
	JList<Cst4_IFF_Track> IFF_T;
	JList<Cst4_ADSB> ADSB_T;
	JList<Cst4_SysTrack> Sys_T;
	JList<Cst4_User_Msg> User;
	JList<VistaAlarmCfg> AlarmCfgList;
	JList<VistaAlarmTrigger> AlarmTrigList;

	Cst4_BITE_Info BITEInfo;
	Cst4_Request Request;
	Cst4_Response Response;
	Cst4_Radar_Scan Radar_Scan;
	Cst4_Radar_Track Radar_Track;
	Cst4_IFF_Track IFF_Track;
	Cst4_ADSB ADSB_Track;
	Cst4_SysTrack SysTrack;
	Cst4_AIS_Dynamic AIS_Dynamic;
	Cst4_AIS_Static AIS_Static;
	Cst4_User_Msg User_Msg;
	VistaAlarmCfg AlarmCfg;
	VistaAlarmTrigger AlarmTrig;
	VistaTimeMessage TimeMsg;

	BYTE aucRXStatus[4];
	DWORD dwRXStatus;

	BYTE aucTXNum[4];
	int nTXNum;

	BYTE aucRXBuf[MAX_RX_SIZE+4];
//	BYTE* pucRXBuf;
	int nRXOffs;
	int nRXSize;

bool bSaveMessages;

	CST4_MSG_TYPE AddPackedMessage(const BYTE* pucBuf, const int nMaxSize);

//   JFile* pTestFil;

//	int nKeepAliveCnt;

	int nErrorCnt;

//   JFile* pDebugFil;


public:


	Cst4Manager();
	~Cst4Manager();

	void ClearMsg();

   void Init(const bool _bSaveMessages);

	CST4_MSG_TYPE AddRXByte(const BYTE uc);

   int nGetTXNum() const
   {
   	return nTXNum;
   }

   Cst4_AIS_Dynamic* pGetAISDynamic(const int n=-1)
   {
		if (n>=0)
			{
			if (!AIS_D.bPacked())
				AIS_D.Pack();
			return AIS_D[n];
			}
      else
         {
         if (bSaveMessages)
            return AIS_D.pGetCurrent();
         else
            return &AIS_Dynamic;
         }
   }

   Cst4_AIS_Static* pGetAISStatic(const int n=-1)
   {
		if (n>=0)
			{
			if (!AIS_S.bPacked())
				AIS_S.Pack();
			return AIS_S[n];
			}
      else
			{
         if (bSaveMessages)
            return AIS_S.pGetCurrent();
         else
            return &AIS_Static;
         }
   }

   Cst4_Radar_Scan* pGetRadarScan(const int n=-1)
   {
		if (n>=0)
			{
			if (!Radar_S.bPacked())
				Radar_S.Pack();
			return Radar_S[n];
			}
      else
         {
         if (bSaveMessages)
            return Radar_S.pGetCurrent();
         else
            return &Radar_Scan;
         }
   }

   Cst4_Radar_Track* pGetRadarTrack(const int n=-1)
   {
		if (n>=0)
			{
			if (!Radar_T.bPacked())
				Radar_T.Pack();
			return Radar_T[n];
			}
		else
			{
			if (bSaveMessages)
				return Radar_T.pGetCurrent();
			else
				return &Radar_Track;
			}
	}

	Cst4_IFF_Track* pGetIFFTrack(const int n=-1)
	{
		if (n>=0)
			{
			if (!IFF_T.bPacked())
				IFF_T.Pack();
			return IFF_T[n];
			}
		else
			{
			if (bSaveMessages)
				return IFF_T.pGetCurrent();
			else
				return &IFF_Track;
			}
	}

	Cst4_ADSB* pGetADSBTrack(const int n=-1)
	{
		if (n>=0)
			{
			if (!ADSB_T.bPacked())
				ADSB_T.Pack();
			return ADSB_T[n];
			}
		else
			{
			if (bSaveMessages)
				return ADSB_T.pGetCurrent();
			else
				return &ADSB_Track;
			}
	}

	VistaAlarmTrigger* pGetAlarmTrig(const int n=-1)
	{
		if (n>=0)
			{
			if (!AlarmTrigList.bPacked())
				AlarmTrigList.Pack();
			return AlarmTrigList[n];
			}
		else
			{
			if (bSaveMessages)
				return AlarmTrigList.pGetCurrent();
			else
				return &AlarmTrig;
			}
	}

	Cst4_User_Msg* pGetUser(const int n=-1)
	{
		if (n>=0)
			{
			if (!User.bPacked())
				User.Pack();
			return User[n];
			}
      else
         {
         if (bSaveMessages)
            return User.pGetCurrent();
         else
            return &User_Msg;
         }
   }

	Cst4_SysTrack* pGetSysTrack(const int n=-1)
   {
		if (n>=0)
			{
			if (!Sys_T.bPacked())
				Sys_T.Pack();
			return Sys_T[n];
			}
		else
         {
         if (bSaveMessages)
				return Sys_T.pGetCurrent();
			else
            return &SysTrack;
         }
   }


	VistaAlarmCfg* pGetAlarmCfg(const int n=-1)
	{
		if (n>=0)
			{
			if (!AlarmCfgList.bPacked())
				AlarmCfgList.Pack();
			return AlarmCfgList[n];
			}
		else
			{
			if (bSaveMessages)
				return AlarmCfgList.pGetCurrent();
			else
				return &AlarmCfg;
			}
	}

	VistaAlarmTrigger* pGetAlarmTrigger(const int n=-1)
	{
		if (n>=0)
			{
			if (!AlarmTrigList.bPacked())
				AlarmTrigList.Pack();
			return AlarmTrigList[n];
			}
		else
			{
			if (bSaveMessages)
				return AlarmTrigList.pGetCurrent();
			else
				return &AlarmTrig;
			}
	}


	Cst4_BITE_Info GetBITEInfo() const
	{
		return BITEInfo;
	}

	Cst4_Request GetRequest() const
	{
		return Request;
	}

	Cst4_Response GetResponse() const
	{
		return Response;
	}

	VistaTimeMessage GetTimeMsg() const
	{
      return TimeMsg;
	}

	void InvalidateTime()
	{
		TimeMsg.Invalidate();
	}

	void AddMessage(const Cst4_AIS_Dynamic& Msg);
	void AddMessage(const Cst4_AIS_Static& Msg);
	void AddMessage(const Cst4_Radar_Scan& Msg);
	void AddMessage(const Cst4_Radar_Track& Msg);
	void AddMessage(const Cst4_IFF_Track& Msg);
	void AddMessage(const Cst4_ADSB& Msg);
	void AddMessage(const Cst4_SysTrack& Msg);
	void AddMessage(const Cst4_User_Msg& Msg);
	void AddMessage(const VistaAlarmCfg& Msg);
	void AddMessage(const VistaAlarmTrigger& Msg);
//	void AddMessage(const VistaAlarmAck& Msg);

	int nGetMsgCnt(const CST4_MSG_TYPE MsgType) const;

   int nGetTXSize();
   void PackTX(BYTE* pucTXBuf);

	bool bSaveToFile(const String sFile);
	bool bReadFromFile(const String sFile);

	JTime GetStartTime_utc() const
	{
		return Header.T1_utc;
	}

	JTime GetEndTime_utc() const
	{
		return Header.T2_utc;
	}

   int nGetErrors() const
   {
   	return nErrorCnt;
   }

//	int nGetKeepAliveCnt() const
//	{
//		return nKeepAliveCnt;
//	}

};

#endif
