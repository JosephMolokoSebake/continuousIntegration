//---------------------------------------------------------------------------
#ifndef Cst4DataServerH
#define Cst4DataServerH

#include "LJTcpServerSocket.h"
#include "Cst4Manager.h"
#include "VistaEncrypt.h"
#include "VistaAlarmMessages.h"

//---------------------------------------------------------------------------



class Cst4DataServer : public LJTcpServerSockets
{


	int nRXBlock;

	class Cst4ServerInstance : public Cst4Manager,public VistaEncrypt
	{
		int nPort;
		int nServerIndex;
		DWORD dwLastData_utc_sec;
		JTime CreateTime_utc;
		int nTotalMsgCnt;
		String sDescr;
		int nRXBufErrCnt;
		int nMaxRXBuf;
		int nRXBytes;


	public:

		Cst4ServerInstance() : Cst4Manager(),VistaEncrypt()
		{
			Cst4Manager::Init(false);
			VistaEncrypt::Init();
			nPort=-1;
			nServerIndex=-1;
			CreateTime_utc=JTime(true,true);
			dwLastData_utc_sec=CreateTime_utc.dwTotalSec();
			nTotalMsgCnt=0;
			sDescr="";
		}

		Cst4ServerInstance(const int _nPort, const int _nServerIndex, const String _sDescr) : Cst4Manager()
		{
			Cst4Manager::Init(false);
			VistaEncrypt::Init();
			nPort=_nPort;
			nServerIndex=_nServerIndex;
			CreateTime_utc=JTime(true,true);
			dwLastData_utc_sec=CreateTime_utc.dwTotalSec();
			nTotalMsgCnt=0;
			sDescr=_sDescr;
			nRXBufErrCnt=0;
			nMaxRXBuf=0;
			nRXBytes=0;
		}

		String sGetDescr() const
		{
			return sDescr;
		}

		JTime GetCreateTime_utc() const
		{
			return CreateTime_utc;
		}

		int nGetPort() const
		{
			return nPort;
		}

		int nGetIndex() const
		{
			return nServerIndex;
		}

		void SetLastData_utc(const DWORD dwSec)
		{
			dwLastData_utc_sec=dwSec;
		}

		DWORD dwGetLastData_utc_sec() const
		{
			return dwLastData_utc_sec;
		}

		void SetDataAge(const DWORD dwAge)
		{


      }

		void AddMsgCnt(const int nMsgCnt)
		{
			nTotalMsgCnt+=nMsgCnt;
		}

		int nGetTotalMsgCnt() const
		{
			return nTotalMsgCnt;
		}

	int nGetRXBufErrCnt() const
	{
		return nRXBufErrCnt;
	}

	void UpdateMaxRXBuf(const int nRXSize)
	{
      nRXBytes+=nRXSize;
		if (nRXSize>nMaxRXBuf)
			nMaxRXBuf=nRXSize;
	}


	int nGetMaxRXBuf() const
	{
		return nMaxRXBuf;
	}

	int nGetRXBytes() const
	{
		return nRXBytes;
	}




	};

//**** End class Cst4ServerInstance

	TComponent* pOwner;

	TCriticalSection* pServerLock;

	enum
	{
		MAX_RX_SIZE    = 1024*1024*10,
	};

	BYTE aucRXData[MAX_RX_SIZE];
   int nMaxTotalRXBuf;

	bool bServerChanged;

	bool bDecrypt;
	String sPW;
	JAES::KEY_SIZE KeySize;

	JList<Cst4ServerInstance> ServerList;
	DWORD dwMaxDormant_sec;

	JList<Cst4_AIS_Dynamic> AIS_D;
	JList<Cst4_AIS_Static> AIS_S;
	JList<Cst4_Radar_Scan> Radar_S;
	JList<Cst4_Radar_Track> Radar_T;
	JList<Cst4_IFF_Track> IFF_T1;
	JList<Cst4_ADSB> ADSB_T;
	JList<Cst4_SysTrack> Sys_T;
	JList<Cst4_User_Msg> User_M;
	JList<VistaAlarmTrigger> Alarm_T;

   JList<Cst4_Response> ResponseList;

	struct RequestListStruct
	{
      DWORD dwRequest;
      WORD wSourceSite;
      int nPort;

      RequestListStruct()
      {
      }

      RequestListStruct(const DWORD _dwRequest, const WORD _wSourceSite, const int _nPort=-1)
      {
         dwRequest=_dwRequest;
         wSourceSite=_wSourceSite;
         nPort=_nPort;
      }

	};

	JList<RequestListStruct> RequestList;

	int nPortTXCnt;
	int nPortErrCnt;
	int nTimeTXCnt;
	Cst4Manager RequestRX;

	String sLogBase;

	bool bTXData(Cst4ServerInstance* pS, BYTE* pucData, const int nSize);
	int nCheckServer(Cst4ServerInstance* pS);
	int nCheckServerDecrypt(Cst4ServerInstance* pS);

	int nLastServerPort;

public:

	Cst4DataServer(TComponent* _pOwner, const int _nMaxDormant_min, const bool bEnableDebugging);

	~Cst4DataServer();

	int nCreateServer(const int nSiteID);

	int nServerCount()
	{
		return ServerList.nGetCount();
	}

	bool bUpdate();

	void PurgeServerList(const DWORD dwEmpty_sec);

	int nFindOldestDataAge();

	int nGetServerListCount()
	{
		return ServerList.nGetCount();
	}

	int nAIS_D_Count()
	{
		return AIS_D.nGetCount();
	}

	Cst4_AIS_Dynamic& GetAIS_D(const int n)
	{
      if (!AIS_D.bPacked())
         AIS_D.Pack();
      return *AIS_D[n];
   }

   Cst4_AIS_Dynamic* pAIS_D(const int n)
   {
      if (!AIS_D.bPacked())
         AIS_D.Pack();
      return AIS_D[n];
   }

   void ClearAIS_D()
   {
      AIS_D.Clear();
   }

   int nAIS_S_Count()
   {
      return AIS_S.nGetCount();
   }

   void ClearAIS_S()
   {
      AIS_S.Clear();
   }

   Cst4_AIS_Static& GetAIS_S(const int n)
   {
      if (!AIS_S.bPacked())
         AIS_S.Pack();
      return *AIS_S[n];
   }

   Cst4_AIS_Static* pAIS_S(const int n)
   {
		if (!AIS_S.bPacked())
         AIS_S.Pack();
      return AIS_S[n];
   }

	int nRadar_S_Count()
   {
      return Radar_S.nGetCount();
   }

   Cst4_Radar_Scan& GetRadar_S(const int n)
   {
      if (!Radar_S.bPacked())
         Radar_S.Pack();
      return *Radar_S[n];
   }

   Cst4_Radar_Scan* pRadar_S(const int n)
   {
      if (!Radar_S.bPacked())
         Radar_S.Pack();
      return Radar_S[n];
   }

   void ClearRadar_S()
   {
      Radar_S.Clear();
   }

   int nRadar_T_Count()
   {
      return Radar_T.nGetCount();
   }

   Cst4_Radar_Track& GetRadar_T(const int n)
   {
      if (!Radar_T.bPacked())
         Radar_T.Pack();
      return *Radar_T[n];
	}

	Cst4_Radar_Track* pRadar_T(const int n)
	{
		if (!Radar_T.bPacked())
			Radar_T.Pack();
		return Radar_T[n];
	}

	void ClearRadar_T()
	{
		Radar_T.Clear();
	}

	int nIFF_T1_Count()
	{
		return IFF_T1.nGetCount();
	}

	Cst4_IFF_Track& GetIFF_T1(const int n)
	{
		if (!IFF_T1.bPacked())
			IFF_T1.Pack();
		return *IFF_T1[n];
	}

	Cst4_IFF_Track* pIFF_T1(const int n)
	{
		if (!IFF_T1.bPacked())
			IFF_T1.Pack();
		return IFF_T1[n];
	}

	void ClearIFF_T1()
	{
		IFF_T1.Clear();
	}

	int nADSB_T_Count()
	{
		return ADSB_T.nGetCount();
	}


	Cst4_ADSB& GetADSB_T(const int n)
	{
		if (!ADSB_T.bPacked())
			ADSB_T.Pack();
		return *ADSB_T[n];
	}

	Cst4_ADSB* pADSB_T(const int n)
	{
		if (!ADSB_T.bPacked())
			ADSB_T.Pack();
		return ADSB_T[n];
	}

	void ClearADSB_T()
	{
		ADSB_T.Clear();
	}

	int nAlarmTrig_Count()
	{
		return Alarm_T.nGetCount();
	}

	VistaAlarmTrigger& GetAlarmTrig(const int n)
	{
		if (!Alarm_T.bPacked())
			Alarm_T.Pack();
		return *Alarm_T[n];
	}

	VistaAlarmTrigger* pGetAlarmTrig(const int n)
	{
		if (!Alarm_T.bPacked())
			Alarm_T.Pack();
      return Alarm_T[n];
   }

	void ClearAlarmTrig()
	{
      Alarm_T.Clear();
   }

	int nSysTrack_Count()
	{
		return Sys_T.nGetCount();
   }

	Cst4_SysTrack& GetSys_T(const int n)
	{
		if (!Sys_T.bPacked())
			Sys_T.Pack();
		return *Sys_T[n];
	}

	Cst4_SysTrack* pSys_T(const int n)
	{
		if (!Sys_T.bPacked())
			Sys_T.Pack();
		return Sys_T[n];
	}

	void ClearSysTrack()
	{
		Sys_T.Clear();
	}

   int nUser_Count()
   {
      return User_M.nGetCount();
   }

   Cst4_User_Msg& User(const int n)
   {
      if (!User_M.bPacked())
         User_M.Pack();
      return *User_M[n];
   }

   Cst4_User_Msg* pUser(const int n)
   {
      if (!User_M.bPacked())
			User_M.Pack();
      return User_M[n];
   }

   void ClearUser()
   {
      User_M.Clear();
	}

//	bool bKeepAliveRXed() const
//	{
//		return bKeepAlive;
//	}
//
//	void ClearKeepAlive()
//	{
//		bKeepAlive=false;
//	}

	int nProcessRequests(const BYTE* pucRXData, const int nRXSize, const int nOwnID);
   int nGetResponse(const int n, BYTE* pucTXData);
   Cst4_Response* pGetResponse(const int n);

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

   bool bGetServerChanged()
   {
   	bool bRet=bServerChanged;
      bServerChanged=false;
      return bRet;
   }

   int nGetServerIndex(const int nServer)
   {
   	if (nServer<ServerList.nGetCount())
      	return ServerList[nServer]->nGetIndex();
      else
      	return -1;
   }

   int nGetServerMsgCnt(const int nServer)
   {
   	if (nServer<ServerList.nGetCount())
	      return ServerList[nServer]->nGetTotalMsgCnt();
      else
      	return 0;
   }

	int nGetServerRXBytes(const int nServer)
	{
		if (nServer<ServerList.nGetCount())
			return ServerList[nServer]->nGetRXBytes();
      else
      	return 0;
   }

   String sGetServerDescr(const int nServer)
   {
   	if (nServer<ServerList.nGetCount())
	      return ServerList[nServer]->sGetDescr();
      else
			return "";
   }

   JTime GetServerCreateTime_utc(const int nServer)
   {
   	if (nServer<ServerList.nGetCount())
	      return ServerList[nServer]->GetCreateTime_utc();
      else
      	return JTime(true);
   }

   int nGetLastServerPort() const
   {
      return nLastServerPort;
   }

   int nGetPortCnt() const {return nPortTXCnt;}
   int nGetPortErrCnt() const {return nPortErrCnt;}
   int nGetTimeTXCnt() const {return nTimeTXCnt;}
   int nGetPortTXCnt() const {return nPortTXCnt;}

//	int nGetKeepAliveCount()
//	{
//		int nCnt=0;
//		for (int i=0; i<ServerList.nGetCount(); i++)
//			nCnt+=ServerList[i]->nGetKeepAliveCnt();
//		return nCnt;
//	}

	void SetLogBase(const String sBase)
	{
		sLogBase=sBase;
		StoreLogInfo("Opened Data Server");
	}

	void StoreLogInfo(const String sInfo);

	int nGetMaxTotalRXBuf() const
	{
		return nMaxTotalRXBuf;
	}

	void SetDecrypt(const bool bDec, const bool bDefaultPW, const String _sPW, const JAES::KEY_SIZE _KeySize)
	{
		bDecrypt=bDec;
		if (bDefaultPW)
			sPW="";
		else
			sPW=_sPW;
		KeySize=_KeySize;
	}

	int nGetDecryptBlockCnt1()
	{
		int nCnt=0;
		pServerLock->Acquire();
		ServerList.GoFirst();
		while(!ServerList.bLast())
			nCnt+=ServerList.pNext()->nGetDecryptBlockCnt();
		pServerLock->Release();
		return nCnt;
	}

	int nGetDecryptErrCnt1()
	{
		int nCnt=0;
		pServerLock->Acquire();
		ServerList.GoFirst();
		while(!ServerList.bLast())
			nCnt+=ServerList.pNext()->nGetDecryptErrCnt();
		pServerLock->Release();
		return nCnt;
	}

	bool bTXVistaTime(VistaTimeMessage Msg);

};

#endif
