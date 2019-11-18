//---------------------------------------------------------------------------

#ifndef VistaClientH
#define VistaClientH
//---------------------------------------------------------------------------

#include "LJTcpClientSocket.h"
#include "Cst4Manager.h"
#include "VistaEncrypt.h"

class VistaClient : public LJTcpClientSockets
{

	int nIdx;

	enum CONNECT_STATE
	{
		CONNECT_OFF,
		CONNECT_WAITING,
		CONNECT_FAILED,
		CONNECT_OK,
	} ConnectState;

	DWORD dwNext_Connect_ms;

/****    TRANSMIT    ******************************************************/

	JList<Cst4_AIS_Dynamic> TX_AIS_D;
	JList<Cst4_AIS_Static> TX_AIS_S;
	JList<Cst4_Radar_Scan> TX_Radar_S;
	JList<Cst4_Radar_Track> TX_Radar_T;
	JList<Cst4_IFF_Track> TX_IFF_T1;
	JList<Cst4_ADSB> TX_ADSB_T;
	JList<Cst4_SysTrack> TX_Sys_T;
	JList<Cst4_User_Msg> TX_User_M;
	JList<VistaAlarmCfg> TX_AlarmCfgList;
	JList<VistaAlarmTrigger> TX_AlarmTrigList;
//	JList<VistaAlarmAck> TX_AlarmAckList;
	VistaTimeMessage TX_TimeMsg;

	int nTXMsgAvail;

	enum
	{
		MAX_TX_BUF_SIZE   = 1024*1024*10,
		MAX_RX_BUF_SIZE 	= 1024*1024*10,
	};

	BYTE aucTXBuf[MAX_TX_BUF_SIZE];
	int nTXBufErrCnt;
	int nMaxTXBuf;

   VistaEncrypt* pDec;
	bool bEncrypt;
	String sPW;
   JAES::KEY_SIZE KeySize;

	String sStatus;
	TColor StatusCol;
	int nTotalTXErrorCnt;
	int nTotalTXCnt;



/****    RECEIVE    ******************************************************/


	BYTE aucRXData[MAX_RX_BUF_SIZE];

	JList<Cst4_AIS_Dynamic> RX_AIS_D;
	JList<Cst4_AIS_Static> RX_AIS_S;
	JList<Cst4_Radar_Scan> RX_Radar_S;
	JList<Cst4_Radar_Track> RX_Radar_T;
	JList<Cst4_IFF_Track> RX_IFF_T;
	JList<Cst4_ADSB> RX_ADSB_T;
	JList<Cst4_SysTrack> RX_Sys_T;
	JList<Cst4_User_Msg> RX_User_M;

	JList<VistaAlarmCfg> RX_AlarmCfgList;
	JList<VistaAlarmTrigger> RX_AlarmTrigList;
   VistaTimeMessage RX_TimeMsg;

	bool bUpdateRX_Normal();
	bool bUpdateRX_Decrypt();

public:

	VistaClient();

	~VistaClient();

	bool bInitTX(TComponent* pOwnerForm, const String sAddr, const int nPort);

	bool bVerifyConnection(bool& _bChanged, String& _sStatus, TColor& _Col, const bool bImmediate=false);

	void AddTXMessage(Cst4_AIS_Dynamic& Msg);
	void AddTXMessage(Cst4_AIS_Static& Msg);
	void AddTXMessage(Cst4_Radar_Scan& Msg);
	void AddTXMessage(Cst4_Radar_Track& Msg);
	void AddTXMessage(Cst4_IFF_Track& Msg);
	void AddTXMessage(Cst4_ADSB& Msg);
	void AddTXMessage(Cst4_SysTrack& Msg);
	void AddTXMessage(Cst4_User_Msg& Msg);

	void AddTXMessage(VistaAlarmCfg& Msg);
	void AddTXMessage(VistaAlarmTrigger& Msg);
	void AddTXMessage(VistaTimeMessage& Msg);

	void ClearTXMsg();

//********
	int nTXAIS_D_Count()	{	return TX_AIS_D.nGetCount();	}

	Cst4_AIS_Dynamic& GetTXAIS_D(const int n)
	{
		if (!TX_AIS_D.bPacked())
			TX_AIS_D.Pack();
		return *TX_AIS_D[n];
	}

	Cst4_AIS_Dynamic* pTXAIS_D(const int n)
	{
		if (!TX_AIS_D.bPacked())
			TX_AIS_D.Pack();
		return TX_AIS_D[n];
	}

	void ClearTXAIS_D()
	{
		TX_AIS_D.Clear();
	}

//********

	int nTXAIS_S_Count()	{	return TX_AIS_S.nGetCount();	}

	void ClearTXAIS_S()
	{
		TX_AIS_S.Clear();
	}

	Cst4_AIS_Static& GetTXAIS_S(const int n)
	{
		if (!TX_AIS_S.bPacked())
			TX_AIS_S.Pack();
		return *TX_AIS_S[n];
	}

	Cst4_AIS_Static* pTXAIS_S(const int n)
	{
		if (!TX_AIS_S.bPacked())
			TX_AIS_S.Pack();
		return TX_AIS_S[n];
	}

//********

	int nTXRadar_S_Count(){	return TX_Radar_S.nGetCount();}

	Cst4_Radar_Scan& GetTXRadar_S(const int n)
	{
		if (!TX_Radar_S.bPacked())
			TX_Radar_S.Pack();
		return *TX_Radar_S[n];
	}

	Cst4_Radar_Scan* pTXRadar_S(const int n)
	{
		if (!TX_Radar_S.bPacked())
			TX_Radar_S.Pack();
		return TX_Radar_S[n];
	}

	void ClearTXRadar_S()
	{
		TX_Radar_S.Clear();
	}

//********

	int nTXRadar_T_Count(){	return TX_Radar_T.nGetCount();}

	Cst4_Radar_Track& GetTXRadar_T(const int n)
	{
		if (!TX_Radar_T.bPacked())
			TX_Radar_T.Pack();
		return *TX_Radar_T[n];
	}

	Cst4_Radar_Track* pTXRadar_T(const int n)
	{
		if (!TX_Radar_T.bPacked())
			TX_Radar_T.Pack();
		return TX_Radar_T[n];
	}

	void ClearTXRadar_T()
	{
		TX_Radar_T.Clear();
	}

//********

	int nTXIFF_T_Count1(){return TX_IFF_T1.nGetCount();}

	Cst4_IFF_Track& GetTXIFF_T1(const int n)
	{
		if (!TX_IFF_T1.bPacked())
			TX_IFF_T1.Pack();
		return *TX_IFF_T1[n];
	}

	Cst4_IFF_Track* pTXIFF_T1(const int n)
	{
		if (!TX_IFF_T1.bPacked())
			TX_IFF_T1.Pack();
		return TX_IFF_T1[n];
	}

	void ClearTXIFF_T1()
	{
		TX_IFF_T1.Clear();
	}

	int nTXADSB_T_Count(){return TX_ADSB_T.nGetCount();}

	Cst4_ADSB& GetTXADSB_T(const int n)
	{
		if (!TX_ADSB_T.bPacked())
			TX_ADSB_T.Pack();
		return *TX_ADSB_T[n];
	}

	Cst4_IFF_Track* pTXADSB_T(const int n)
	{
		if (!TX_ADSB_T.bPacked())
			TX_ADSB_T.Pack();
		return TX_ADSB_T[n];
	}

	void ClearTXADSB_T()
	{
		TX_ADSB_T.Clear();
	}



//********

	int nTXSysTrack_Count(){return TX_Sys_T.nGetCount();}

	Cst4_SysTrack& GetTXSys_T(const int n)
	{
		if (!TX_Sys_T.bPacked())
			TX_Sys_T.Pack();
		return *TX_Sys_T[n];
	}

	Cst4_SysTrack* pTXSys_T(const int n)
	{
		if (!TX_Sys_T.bPacked())
			TX_Sys_T.Pack();
		return TX_Sys_T[n];
	}

	void ClearTXSysTrack()
	{
		TX_Sys_T.Clear();
	}

//********

	int nTXUser_Count(){	return TX_User_M.nGetCount();	}

	Cst4_User_Msg& TXUser(const int n)
	{
		if (!TX_User_M.bPacked())
			TX_User_M.Pack();
		return *TX_User_M[n];
	}

	Cst4_User_Msg* pTXUser(const int n)
	{
		if (!TX_User_M.bPacked())
			TX_User_M.Pack();
		return TX_User_M[n];
	}

	void ClearTXUser()
	{
		TX_User_M.Clear();
	}

//********

	int nTXAlarmCfg_Count()	{return TX_AlarmCfgList.nGetCount();}

	VistaAlarmCfg& TXAlarmCfg(const int n)
	{
		if (!TX_AlarmCfgList.bPacked())
			TX_AlarmCfgList.Pack();
		return *TX_AlarmCfgList[n];
	}

	VistaAlarmCfg* pTXAlarmCfg(const int n)
	{
		if (!TX_AlarmCfgList.bPacked())
			TX_AlarmCfgList.Pack();
		return TX_AlarmCfgList[n];
	}

	void ClearTXAlarmCfg()
	{
		TX_AlarmCfgList.Clear();
	}

//********

	int nTXAlarmTrig_Count(){return TX_AlarmTrigList.nGetCount();}

	VistaAlarmTrigger& TXAlarmTrig(const int n)
	{
		if (!TX_AlarmTrigList.bPacked())
			TX_AlarmTrigList.Pack();
		return *TX_AlarmTrigList[n];
	}

	VistaAlarmTrigger* pTXAlarmTrig(const int n)
	{
		if (!TX_AlarmTrigList.bPacked())
			TX_AlarmTrigList.Pack();
		return TX_AlarmTrigList[n];
	}

	void ClearTXAlarmTrig()
	{
		TX_AlarmTrigList.Clear();
	}

//********


	bool bTXMessages();

	bool bTXBuffer(const void* pBuf, const int nSize);

	String sGetStatus() const
	{
		return sStatus;
	}

	TColor GetStatusCol() const
	{
		return StatusCol;
	}

	void ShowStatus(TEdit* pEd)
	{
		pEd->Text=sStatus;
		pEd->Color=StatusCol;
	}

	int nGetTotalTXCnt() const
	{
		return nTotalTXCnt;
	}
	int nGetTotalTXErrorCnt() const
	{
		return nTotalTXErrorCnt;
	}


/****    RECEIVE    ******************************************************/

private:

	Cst4Manager RXManage;
	DWORD dwLastRXData_utc_sec;
	int nRXMsgCnt;
	int nSysRX_Cnt;
public:


	bool bInitRX(const bool bEnc=false, const bool bDefaultPW=false, const String _sPW="", const JAES::KEY_SIZE _KeySize=JAES::KEY_128_BITS);

	bool bUpdateRX();
	int nUpdateRawRX(BYTE* pucData, const int nMaxSize);

//********
	int nRXAIS_D_Count()	{	return RX_AIS_D.nGetCount();	}

	Cst4_AIS_Dynamic& GetRXAIS_D(const int n)
	{
		if (!RX_AIS_D.bPacked())
			RX_AIS_D.Pack();
		return *RX_AIS_D[n];
	}

	Cst4_AIS_Dynamic* pRXAIS_D(const int n)
	{
		if (!RX_AIS_D.bPacked())
			RX_AIS_D.Pack();
		return RX_AIS_D[n];
	}

	void ClearRXAIS_D()
	{
		RX_AIS_D.Clear();
	}

//********

	int nRXAIS_S_Count()	{	return RX_AIS_S.nGetCount();	}

	void ClearRXAIS_S()
	{
		RX_AIS_S.Clear();
	}

	Cst4_AIS_Static& GetRXAIS_S(const int n)
	{
		if (!RX_AIS_S.bPacked())
			RX_AIS_S.Pack();
		return *RX_AIS_S[n];
	}

	Cst4_AIS_Static* pRXAIS_S(const int n)
	{
		if (!RX_AIS_S.bPacked())
			RX_AIS_S.Pack();
		return RX_AIS_S[n];
	}

//********

	int nRadar_RXRadar_S_Count(){	return RX_Radar_S.nGetCount();}

	Cst4_Radar_Scan& GetRXRadar_S(const int n)
	{
		if (!RX_Radar_S.bPacked())
			RX_Radar_S.Pack();
		return *RX_Radar_S[n];
	}

	Cst4_Radar_Scan* pRXRadar_S(const int n)
	{
		if (!RX_Radar_S.bPacked())
			RX_Radar_S.Pack();
		return RX_Radar_S[n];
	}

	void ClearRXRadar_S()
	{
		RX_Radar_S.Clear();
	}

//********

	int nRXRadar_T_Count(){	return RX_Radar_T.nGetCount();}

	Cst4_Radar_Track& GetRXRadar_T(const int n)
	{
		if (!RX_Radar_T.bPacked())
			RX_Radar_T.Pack();
		return *RX_Radar_T[n];
	}

	Cst4_Radar_Track* pRXRadar_T(const int n)
	{
		if (!RX_Radar_T.bPacked())
			RX_Radar_T.Pack();
		return RX_Radar_T[n];
	}

	void ClearRXRadar_T()
	{
		RX_Radar_T.Clear();
	}

//********

	int nRXIFF_T_Count1(){return RX_IFF_T.nGetCount();}

	Cst4_IFF_Track& GetRXIFF_T(const int n)
	{
		if (!RX_IFF_T.bPacked())
			RX_IFF_T.Pack();
		return *RX_IFF_T[n];
	}

	Cst4_IFF_Track* pRXIFF_T(const int n)
	{
		if (!RX_IFF_T.bPacked())
			RX_IFF_T.Pack();
		return RX_IFF_T[n];
	}

	void ClearRXIFF_T1()
	{
		RX_IFF_T.Clear();
	}

	Cst4_ADSB& GetRXADSB_T(const int n)
	{
		if (!RX_ADSB_T.bPacked())
			RX_ADSB_T.Pack();
		return *RX_ADSB_T[n];
	}

	Cst4_ADSB* pRXADSB_T(const int n)
	{
		if (!RX_ADSB_T.bPacked())
			RX_ADSB_T.Pack();
		return RX_ADSB_T[n];
	}

	void ClearRXADSB_T()
	{
		RX_ADSB_T.Clear();
	}

//********

	int nRXSysTrack_Count(){return RX_Sys_T.nGetCount();}

	Cst4_SysTrack& GetRXSys_T(const int n)
	{
		if (!RX_Sys_T.bPacked())
			RX_Sys_T.Pack();
		return *RX_Sys_T[n];
	}

	Cst4_SysTrack* pRXSys_T(const int n)
	{
		if (!RX_Sys_T.bPacked())
			RX_Sys_T.Pack();
		return RX_Sys_T[n];
	}

	void ClearRXSysTrack()
	{
		RX_Sys_T.Clear();
	}

//********

	int nRXUser_Count(){	return RX_User_M.nGetCount();	}

	Cst4_User_Msg& RXUser(const int n)
	{
		if (!RX_User_M.bPacked())
			RX_User_M.Pack();
		return *RX_User_M[n];
	}

	Cst4_User_Msg* pRXUser(const int n)
	{
		if (!RX_User_M.bPacked())
			RX_User_M.Pack();
		return RX_User_M[n];
	}

	void ClearRXUser()
	{
		RX_User_M.Clear();
	}

//********

	int nRXAlarmCfg_Count()	{return RX_AlarmCfgList.nGetCount();}

	VistaAlarmCfg& RXAlarmCfg(const int n)
	{
		if (!RX_AlarmCfgList.bPacked())
			RX_AlarmCfgList.Pack();
		return *RX_AlarmCfgList[n];
	}

	VistaAlarmCfg* pRXAlarmCfg(const int n)
	{
		if (!RX_AlarmCfgList.bPacked())
			RX_AlarmCfgList.Pack();
		return RX_AlarmCfgList[n];
	}

	void ClearRXAlarmCfg()
	{
		RX_AlarmCfgList.Clear();
	}

//********

	int nRXAlarmTrig_Count(){return RX_AlarmTrigList.nGetCount();}

	VistaAlarmTrigger& RXAlarmTrig(const int n)
	{
		if (!RX_AlarmTrigList.bPacked())
			RX_AlarmTrigList.Pack();
		return *RX_AlarmTrigList[n];
	}

	VistaAlarmTrigger* pRXAlarmTrig(const int n)
	{
		if (!RX_AlarmTrigList.bPacked())
			RX_AlarmTrigList.Pack();
		return RX_AlarmTrigList[n];
	}

	void ClearRXAlarmTrig()
	{
		RX_AlarmTrigList.Clear();
	}

//********

	VistaTimeMessage& GetRXTimeMsg()
	{
		return RX_TimeMsg;
	}

	void ClearRXTimeMsg()
	{
		RX_TimeMsg.Invalidate();
	}

	void ClearAllRX()
	{
		ClearRXAIS_D();
		ClearRXAIS_S();
		ClearRXRadar_S();
		ClearRXRadar_T();
		ClearRXIFF_T1();
		ClearRXADSB_T();
		ClearRXSysTrack();
		ClearRXUser();
		ClearRXAlarmCfg();
		ClearRXAlarmTrig();
//		ClearRXAlarmAck();
		ClearRXTimeMsg();
	}

	bool bSendRXWatchdogMsg();

	void SetEncrypt(const bool bEnc, const bool bDefaultPW, const String _sPW, const JAES::KEY_SIZE _KeySize)
	{
		bEncrypt=bEnc;
		if (bDefaultPW)
			sPW="";
		else
			sPW=_sPW;
		KeySize=_KeySize;
   }

	int nGetDecryptBlockCnt() const
	{
		if (pDec)
			return pDec->nGetDecryptBlockCnt();
		else
			return 0;
	}

	int nGetDecryptErrCnt() const
	{
		if (pDec)
			return pDec->nGetDecryptErrCnt();
		else
			return 0;
	}

	bool bTXRaw(const void* pData, const int nSize);

   bool bConnectOff() const
   {
      return (ConnectState==CONNECT_OFF);
   }

};
#endif
