//---------------------------------------------------------------------------

#ifndef Cst4DataClientH
#define Cst4DataClientH
//---------------------------------------------------------------------------

#include "LJTcpClientSocket.h"
#include "Cst4Manager.h"
#include "JAES.h"

class Cst4DataClient : public LJTcpClientSockets
{

	struct DestInstance
	{
		int nDestClientPort;
		int nDestClientIndex;
		int nManageClientIndex;
		String sIPAddr;
		WORD wOwnSite;
		Cst4Manager ResponseRX;
		int nRequestsPending;

		int nTXAIS_D_Cnt;
		int nTXAIS_S_Cnt;
		int nTXRadar_S_Cnt;
		int nTXRadar_T_Cnt;
		int nTXIFF_T_Cnt;
		int nTXSysTrack_Cnt;
		int nTXUser_Cnt;
		int nTXAlarm_Cnt;
		int nTXErrorCnt;
		bool bDestClientConnected;
		int nTotalTXCnt;
		String sClientStatus;
		TColor ClientStatusCol;
		String sManageStatus;
		TColor ManageStatusCol;
		DWORD dwNextRequest_loc_sec;

		bool bManageConnected;

		int nConnectedTXErrorCnt;

		JList<Cst4_AIS_Dynamic> AIS_D;
		JList<Cst4_AIS_Static> AIS_S;
		JList<Cst4_Radar_Scan> Radar_S;
		JList<Cst4_Radar_Track> Radar_T;
		JList<Cst4_IFF_Track> IFF_T;
		JList<Cst4_ADSB> ADSB_T;
		JList<Cst4_SysTrack> Sys_T;
		JList<Cst4_User_Msg> User_M;
		JList<VistaAlarmTrigger> Alarm_T;

		Cst4Manager TimeRX;

		void Assign(DestInstance& D)
		{

			nDestClientPort=D.nDestClientPort;
			nDestClientIndex=D.nDestClientIndex;
			nManageClientIndex=D.nManageClientIndex;
			sIPAddr=D.sIPAddr;
			wOwnSite=D.wOwnSite;
			ResponseRX=D.ResponseRX;
			nRequestsPending=D.nRequestsPending;

			nTXAIS_D_Cnt=D.nTXAIS_D_Cnt;
			nTXAIS_S_Cnt=D.nTXAIS_S_Cnt;
			nTXRadar_S_Cnt=D.nTXRadar_S_Cnt;
			nTXRadar_T_Cnt=D.nTXRadar_T_Cnt;
			nTXIFF_T_Cnt=D.nTXIFF_T_Cnt;
			nTXSysTrack_Cnt=D.nTXSysTrack_Cnt;
			nTXUser_Cnt=D.nTXUser_Cnt;
			nTXAlarm_Cnt=D.nTXAlarm_Cnt;
			nTXErrorCnt=D.nTXErrorCnt;
			bDestClientConnected=D.bDestClientConnected;
			nTotalTXCnt=D.nTotalTXCnt;
			sClientStatus=D.sClientStatus;
			ClientStatusCol=D.ClientStatusCol;
			sManageStatus=D.sManageStatus;
			ManageStatusCol=D.ManageStatusCol;
			dwNextRequest_loc_sec=D.dwNextRequest_loc_sec;

			bManageConnected=D.bManageConnected;

			nConnectedTXErrorCnt=D.nConnectedTXErrorCnt;


			AIS_D=D.AIS_D;
			AIS_S=D.AIS_S;
			Radar_S=D.Radar_S;
			Radar_T=D.Radar_T;
			IFF_T=D.IFF_T;
			Sys_T=D.Sys_T;
			User_M=D.User_M;
			Alarm_T=D.Alarm_T;

		}

		DestInstance& operator = (DestInstance& D)
		{
			Assign(D);
			return *this;
		}


		DestInstance()
		{
			nDestClientPort=-1;
			nDestClientIndex=-1;
			nManageClientIndex=-1;
			wOwnSite=0;
			sIPAddr="";
			ResponseRX.Init(false);
			nRequestsPending=0;
			nTXAIS_D_Cnt=0;
			nTXAIS_S_Cnt=0;
			nTXRadar_S_Cnt=0;
			nTXRadar_T_Cnt=0;
			nTXIFF_T_Cnt=0;
			nTXSysTrack_Cnt=0;
			nTXUser_Cnt=0;
			nTXAlarm_Cnt=0;
			nTXErrorCnt=0;
			nConnectedTXErrorCnt=-1;
			nTotalTXCnt=0;
			bDestClientConnected=false;
			sClientStatus="";
			ClientStatusCol=clWindow;
			sManageStatus="";
			ManageStatusCol=clWindow;
			dwNextRequest_loc_sec=0;
			bManageConnected=false;
         TimeRX.Init(false);
		}

		DestInstance(DestInstance& D)
		{
			Assign(D);
		}

		void ClearMsg()
		{
			AIS_D.Clear();
			AIS_S.Clear();
			Radar_S.Clear();
			Radar_T.Clear();
			IFF_T.Clear();
			Sys_T.Clear();
			User_M.Clear();
			Alarm_T.Clear();
		}

		void UpdateTXCount()
		{
			nTXAIS_D_Cnt+=AIS_D.nGetCount();
			nTXAIS_S_Cnt+=AIS_S.nGetCount();
			nTXRadar_S_Cnt+=Radar_S.nGetCount();
			nTXRadar_T_Cnt+=Radar_T.nGetCount();
			nTXIFF_T_Cnt+=IFF_T.nGetCount();
			nTXSysTrack_Cnt+=Sys_T.nGetCount();
			nTXUser_Cnt+=User_M.nGetCount();
			nTXAlarm_Cnt+=Alarm_T.nGetCount();
			nTotalTXCnt+=AIS_D.nGetCount();
         nTotalTXCnt+=AIS_S.nGetCount();
         nTotalTXCnt+=Radar_S.nGetCount();
         nTotalTXCnt+=Radar_T.nGetCount();
         nTotalTXCnt+=IFF_T.nGetCount();
         nTotalTXCnt+=Sys_T.nGetCount();
         nTotalTXCnt+=User_M.nGetCount();
         nTotalTXCnt+=Alarm_T.nGetCount();
		}

   };

   JList<DestInstance> DestList;
//	int nTimeOffs_sec;

	TComponent* pOwner;

   enum
   {
		MAX_TX_BUF_SIZE   = 1024*1024*10,
   };

   BYTE aucTXBuf[MAX_TX_BUF_SIZE];
	int nTXBufErrCnt;
	int nMaxTXBuf;

	bool bEncrypt;
	String sPW;
   JAES::KEY_SIZE KeySize;

	int nMsgAvail;
	JTime LastTimeSync_utc;
	JTime LastTimeUpdate_utc;
	int nTimeRXCnt;
	int nPortRXCnt;

	bool bAbortAll;

	VistaTimeMessage LastTimeMsg;

	struct RequestListStruct
	{
		int nDest;
		DWORD dwRequest;
		WORD wSourceSite;
		int nPort;

		RequestListStruct()
		{
		}

		RequestListStruct(const int _nDest, const DWORD _dwRequest, const WORD _wSourceSite, const int _nPort=-1)
		{
			nDest=_nDest;
			dwRequest=_dwRequest;
			wSourceSite=_wSourceSite;
			nPort=_nPort;
		}

	};

	JList<RequestListStruct> RequestList;

	bool bTXSingleDest(const int nDest=-1);

//	Cst4Manager TXDebug;

 //  int nTXCnt;

public:

	Cst4DataClient(TComponent* _pOwner, const bool bDebugTCP, const int nWinsockBufferSize_bytes) : LJTcpClientSockets(5,bDebugTCP,nWinsockBufferSize_bytes)
	{
		pOwner=_pOwner;
		DestList.Clear();
		nMsgAvail=0;
		nTimeRXCnt=0;
		nPortRXCnt=0;
		nTXBufErrCnt=0;
		nMaxTXBuf=0;
//      nTXCnt=0;
		bAbortAll=false;
		bEncrypt=false;
      sPW="";
		LastTimeSync_utc.Invalidate();
		LastTimeUpdate_utc.Invalidate();
//		nTimeOffs_sec=0;
		LastTimeMsg.Invalidate();
	}

   ~Cst4DataClient()
	{
		bAbortAll=true;
	}

	void AbortAll()
	{
		bAbortAll=true;
	}

   void AddMessage(Cst4_AIS_Dynamic& Msg, const int nDest=-1);
   void AddMessage(Cst4_AIS_Static& Msg, const int nDest=-1);
   void AddMessage(Cst4_Radar_Scan& Msg, const int nDest=-1);
   void AddMessage(Cst4_Radar_Track& Msg, const int nDest=-1);
   void AddMessage(Cst4_IFF_Track& Msg, const int nDest=-1);
	void AddMessage(Cst4_ADSB& Msg, const int nDest=-1);
	void AddMessage(Cst4_SysTrack& Msg, const int nDest=-1);
	void AddMessage(Cst4_User_Msg& Msg, const int nDest=-1);
	void AddMessage(VistaAlarmTrigger& Msg, const int nDest=-1);
	bool bTXMessages(const int nDest=-1);
	int nMessagesAvail() const {return nMsgAvail; }

   int nCreateDestination(const String sIPAddr, const int nManagePort, const WORD wOwnSite);
	bool bRequestDestPort_utc(const int nDest, DWORD dwT_utc_sec=0);
	bool bRequestDestTime_utc(const int nDest, DWORD dwT_utc_sec=0);
	bool bRequestSources(const int nDest, DWORD dwT_utc_sec=0);
	bool bSendRXWatchdogMsg(const int nDest);
	bool bSendTXWatchdogMsg(const int nDest);

   int nServiceResponse();

   int nGetRequestDest(const int n)
   {
      return RequestList[n]->nDest;
   }

   WORD wGetRequestSource(const int n)
   {
      return RequestList[n]->wSourceSite;
   }

   CST4_INFO_REQUEST_TYPE GetRequestType(const int n)
   {
      return (CST4_INFO_REQUEST_TYPE)RequestList[n]->dwRequest;
   }

	int nGetRequestPort(const int n)
	{
		return RequestList[n]->nPort;
	}

	int nDestClientPort(const int nDest)
	{
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			return DestList[nDest]->nDestClientPort;
		else
			return -1;
	}

	int nDestClientIndex(const int nDest)
	{
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			return DestList[nDest]->nDestClientIndex;
		else
			return -1;
	}

	int nDestManageIndex(const int nDest)
	{
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			return DestList[nDest]->nManageClientIndex;
		else
			return -1;
	}

	bool bDestClientConnected(const int nDest)
	{
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			return DestList[nDest]->bDestClientConnected;
		else
			return false;
	}

	bool bManageConnected(const int nDest)
	{
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			{
			if (DestList[nDest]->nManageClientIndex>=0)
				return bClientConnected(DestList[nDest]->nManageClientIndex);
			}
		return false;
	}

	String sDestClientStatus(const int nDest, TColor& Col)
	{
		DestList.Pack();
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			{
			Col=DestList[nDest]->ClientStatusCol;
			return DestList[nDest]->sClientStatus;
			}
		else
			return "";
	}

	String sGetManageStatus(const int nDest, TColor& Col)
	{
		DestList.Pack();
		if ((nDest<DestList.nGetCount())&&(nDest>=0))
			{
			Col=DestList[nDest]->ManageStatusCol;
			String s="";
			DestInstance D;
			D=*DestList[nDest];
			s=D.sManageStatus;
			return s;
			}
		else
			return "";
	}

	void SetEncrypt(const bool bEnc, const bool bDefaultPW, const String _sPW, const JAES::KEY_SIZE _KeySize)
	{
		bEncrypt=bEnc;
		if (bDefaultPW)
			sPW="";
		else
			sPW=_sPW;
		KeySize=_KeySize;
   }

	JTime GetLastTimeSync_utc()
	{
		return LastTimeSync_utc;
	}

	JTime GetLastTimeUpdate_utc()
	{
		return LastTimeUpdate_utc;
	}

	bool bDestReady(const int nDest)
	{
		return DestList[nDest]->nDestClientPort>=0;
	}

	int nDestCnt()
	{
      return DestList.nGetCount();
	}

   bool bFreshManageConnected(const int nDest);

   int nTXAIS_D_Cnt(const int nDest)   {return DestList[nDest]->nTXAIS_D_Cnt;    }
   int nTXAIS_S_Cnt(const int nDest)   {return DestList[nDest]->nTXAIS_S_Cnt;    }
   int nTXRadar_S_Cnt(const int nDest) {return DestList[nDest]->nTXRadar_S_Cnt;  }
   int nTXRadar_T_Cnt(const int nDest) {return DestList[nDest]->nTXRadar_T_Cnt;  }
   int nTXIFF_T_Cnt(const int nDest)   {return DestList[nDest]->nTXIFF_T_Cnt;    }
	int nTXSysTrack_Cnt(const int nDest){return DestList[nDest]->nTXSysTrack_Cnt; }
	int nTXUser_Cnt(const int nDest)    {return DestList[nDest]->nTXUser_Cnt;     }
	int nTXAlarm_Cnt(const int nDest)   {return DestList[nDest]->nTXAlarm_Cnt;    }
	int nTXErrorCnt(const int nDest)    {return DestList[nDest]->nTXErrorCnt;     }
   int nTotalTXCnt(const int nDest)    {return DestList[nDest]->nTotalTXCnt;     }

   void ClearTotalTXCnt(const int nDest)
   {
      DestList[nDest]->nTotalTXCnt=0;
	}

	int nGetPortRXCnt() const {return nPortRXCnt;}
	int nGetTimeRXCnt() const {return nTimeRXCnt;}

	int nGetTXBufErrCnt() const {return nTXBufErrCnt;}
	int nGetMaxTXBuf() const {return nMaxTXBuf;}

	bool bUpdateTimeRX();
	VistaTimeMessage GetTimeMsg()
	{
      return LastTimeMsg;
   }




/*
	void SetTimeOffs(const int nOffs_sec) {nTimeOffs_sec=nOffs_sec;}
	int nGetTimeOffs_sec() const	{return nTimeOffs_sec;}
*/

//	bool bTXSingleKeepAlive(Cst4_KeepAlive& Msg, const int nDest);
//	bool bTXKeepAlive(const int nSourceID, const int nDest=-1);



};

#endif
