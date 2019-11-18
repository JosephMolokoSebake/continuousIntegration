//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "VistaClient.h"
#include "VistaEncrypt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


VistaClient::VistaClient() : LJTcpClientSockets()
{
	nIdx=-1;
	nTXMsgAvail=0;
	dwNext_Connect_ms=0;
	ConnectState=CONNECT_OFF;
	bEncrypt=false;
	sPW="";
	pDec=NULL;
}

VistaClient::~VistaClient()
{
	if (nIdx>=0)
		DisconnectClient(nIdx);
	delete pDec;
}


/****    TRANSMIT    ******************************************************/

bool VistaClient::bInitTX(TComponent* pOwnerForm, const String sAddr, const int nPort)
{
	bool bOK=false;
	nTotalTXCnt=0;
	nTotalTXErrorCnt=0;
	nIdx=nAddClient(pOwnerForm,sAddr,nPort);
	if (nIdx>=0)
		{
		bool bChanged;
		bOK=bVerifyConnection(bChanged,sStatus,StatusCol,true);
		}
	ClearTXMsg();
	GetStatus(nIdx,sStatus,StatusCol);
	return bOK;
}

void VistaClient::ClearTXMsg()
{
	nTXMsgAvail=0;
	TX_AIS_D.Clear();
	TX_AIS_S.Clear();
	TX_Radar_S.Clear();
	TX_Radar_T.Clear();
	TX_IFF_T1.Clear();
	TX_ADSB_T.Clear();
	TX_Sys_T.Clear();
	TX_User_M.Clear();
	TX_AlarmCfgList.Clear();
	TX_AlarmTrigList.Clear();
//	TX_AlarmAckList.Clear();
	TX_TimeMsg.Invalidate();
}

bool VistaClient::bVerifyConnection(bool& _bChanged, String& _sStatus, TColor& _Col, const bool bImmediate)
{
	_bChanged=false;
	if (nIdx>=0)
		{
		bool bOK=bClientConnected(nIdx);
		if (bOK)
			{
			if (ConnectState!=CONNECT_OK)
				{
				_bChanged=true;
				ConnectState=CONNECT_OK;
				GetStatus(nIdx,_sStatus,_Col);
				}
			}
		else
			{
			if (bImmediate)
				ConnectState=CONNECT_OFF;
			DWORD dwT_ms;
			switch(ConnectState)
				{
				case CONNECT_OK:
					ConnectState=CONNECT_WAITING;
					_bChanged=true;
					GetStatus(nIdx,_sStatus,_Col);
					break;
				case CONNECT_OFF:
					ConnectClient(nIdx);
					dwNext_Connect_ms=::timeGetTime()+8000+random(5000);
					ConnectState=CONNECT_WAITING;
					_bChanged=true;
					GetStatus(nIdx,_sStatus,_Col);
					break;
				case CONNECT_WAITING:
					dwT_ms=::timeGetTime();
					if (dwT_ms>dwNext_Connect_ms)
						{
						ConnectClient(nIdx);
						dwNext_Connect_ms=dwT_ms+8000+random(5000);
						_bChanged=true;
						GetStatus(nIdx,_sStatus,_Col);
						}
					break;
				default: ;
				}
			}
		return bOK;
		}
	_sStatus="Connection Off";
	_Col=cl3DLight;
	_bChanged=true;
	ConnectState=CONNECT_OFF;
	return false;
}

void VistaClient::AddTXMessage(Cst4_AIS_Dynamic& Msg)
{
	TX_AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_AIS_Static& Msg)
{
	TX_AIS_S.nAdd(new Cst4_AIS_Static(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_Radar_Scan& Msg)
{
	TX_Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_Radar_Track& Msg)
{
	TX_Radar_T.nAdd(new Cst4_Radar_Track(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_IFF_Track& Msg)
{
	TX_IFF_T1.nAdd(new Cst4_IFF_Track(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_ADSB& Msg)
{
	TX_ADSB_T.nAdd(new Cst4_ADSB(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_SysTrack& Msg)
{
	TX_Sys_T.nAdd(new Cst4_SysTrack(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(Cst4_User_Msg& Msg)
{
	TX_User_M.nAdd(new Cst4_User_Msg(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(VistaAlarmCfg& Msg)
{
	TX_AlarmCfgList.nAdd(new VistaAlarmCfg(Msg));
	++nTXMsgAvail;
}

void VistaClient::AddTXMessage(VistaAlarmTrigger& Msg)
{
	TX_AlarmTrigList.nAdd(new VistaAlarmTrigger(Msg));
	++nTXMsgAvail;
}

//void VistaClient::AddTXMessage(VistaAlarmAck& Msg)
//{
//	TX_AlarmAckList.nAdd(new VistaAlarmAck(Msg));
//	++nTXMsgAvail;
//}

void VistaClient::AddTXMessage(VistaTimeMessage& Msg)
{
	TX_TimeMsg=Msg;
	++nTXMsgAvail;
}


bool VistaClient::bTXMessages()
{
	bool bOK=true;
	if (nTXMsgAvail>0)
		{
		bOK=false;
		int nTXSize=0;
		bool bTXBufOK=true;
		int nMaxBuf=MAX_TX_BUF_SIZE-65536;

		if (TX_TimeMsg.bValid() && bTXBufOK)
			nTXSize+=TX_TimeMsg.nCreateTXBuf(&aucTXBuf[nTXSize]);

		TX_AIS_D.GoFirst();
		while(!TX_AIS_D.bLast() && bTXBufOK)
				{
				nTXSize+=TX_AIS_D.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}

		TX_AIS_S.GoFirst();
		while(!TX_AIS_S.bLast() && bTXBufOK)
			{
			nTXSize+=TX_AIS_S.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_Radar_S.GoFirst();
		while(!TX_Radar_S.bLast() && bTXBufOK)
			{
			nTXSize+=TX_Radar_S.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_Radar_T.GoFirst();
		while(!TX_Radar_T.bLast() && bTXBufOK)
			{
			nTXSize+=TX_Radar_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_IFF_T1.GoFirst();
		while(!TX_IFF_T1.bLast() && bTXBufOK)
			{
			nTXSize+=TX_IFF_T1.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_ADSB_T.GoFirst();
		while(!TX_ADSB_T.bLast() && bTXBufOK)
			{
			nTXSize+=TX_ADSB_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_Sys_T.GoFirst();
		while(!TX_Sys_T.bLast() && bTXBufOK)
			{
			nTXSize+=TX_Sys_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
//			Cst4Manager M;
//			for (int i=0; i<nTXSize; i++)
//				M.AddRXByte(aucTXBuf[i]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_User_M.GoFirst();
		while(!TX_User_M.bLast() && bTXBufOK)
			{
			nTXSize+=TX_User_M.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_AlarmCfgList.GoFirst();
		while(!TX_AlarmCfgList.bLast() && bTXBufOK)
			{
			nTXSize+=TX_AlarmCfgList.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		TX_AlarmTrigList.GoFirst();
		while(!TX_AlarmTrigList.bLast() && bTXBufOK)
			{
			nTXSize+=TX_AlarmTrigList.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}

		if (nTXSize>nMaxTXBuf)
			nMaxTXBuf=nTXSize;

		if (!bTXBufOK)
			++nTXBufErrCnt;

		if (nTXSize>0)
			{

			if (bEncrypt)
				{
				VistaEncrypt Enc;
				Enc.SetPW(sPW,KeySize);
				nTXSize=Enc.nEncrypt(aucTXBuf,nTXSize);
				}


			bOK=bSendData(nIdx,aucTXBuf,nTXSize);
			if (bOK)
				{
				nTotalTXCnt+=nTXMsgAvail;
				}
			else
				{
				bOK=false;
				nTotalTXErrorCnt+=nTXMsgAvail;;
				}
			}
		ClearTXMsg();
		}
	GetStatus(nIdx,sStatus,StatusCol);
	return bOK;
}

	bool VistaClient::bTXBuffer(const void* pBuf, const int nSize)
	{
		if (nSize>0)
			{
			memcpy(aucTXBuf,pBuf,nSize);
			return bSendData(nIdx,aucTXBuf,nSize);
			}
		return false;
	}



/****    RECEIVE    ******************************************************/

	bool VistaClient::bInitRX(const bool bEnc, const bool bDefaultPW, const String _sPW, const JAES::KEY_SIZE _KeySize)
	{
		bEncrypt=bEnc;
		if (bDefaultPW)
			sPW="";
		else
			sPW=_sPW;
		KeySize=_KeySize;
		delete pDec;
		pDec=NULL;
		if (bEncrypt)
			{
			pDec=new VistaEncrypt;
         pDec->SetPW(sPW,KeySize);
			}
		ClearAllRX();
		RXManage.Init(false);
		nRXMsgCnt=0;
		bool bChanged;
		bool bOK=bVerifyConnection(bChanged,sStatus,StatusCol,true);
		return bOK;
	}


	int VistaClient::nUpdateRawRX(BYTE* pucData, const int nMaxSize)
	{
		int nRXBytes=nReadData(nIdx,pucData,nMaxSize);
		return nRXBytes;
	}


	bool VistaClient::bUpdateRX()
	{
		if (bEncrypt)
			return bUpdateRX_Decrypt();
		else
			return bUpdateRX_Normal();
	}

	bool VistaClient::bUpdateRX_Normal()
	{
		int nPrevRXMsgCnt=nRXMsgCnt;
		int nRXBytes=nReadData(nIdx,aucRXData,MAX_RX_BUF_SIZE);
		for (int i=0; i<nRXBytes; i++)
			{
			CST4_MSG_TYPE MsgType=RXManage.AddRXByte(aucRXData[i]);
			switch(MsgType)
				{
				case CST4_DYNAMIC_AIS_MSG:
					RX_AIS_D.nAdd(new Cst4_AIS_Dynamic(*RXManage.pGetAISDynamic()));
					++nRXMsgCnt;
					break;
				case CST4_STATIC_AIS_MSG:
					RX_AIS_S.nAdd(new Cst4_AIS_Static(*RXManage.pGetAISStatic()));
					++nRXMsgCnt;
					break;
				case CST4_RADAR_SCAN_MSG:
					RX_Radar_S.nAdd(new Cst4_Radar_Scan(*RXManage.pGetRadarScan()));
					++nRXMsgCnt;
					break;
				case CST4_RADAR_TRACK_MSG:
					RX_Radar_T.nAdd(new Cst4_Radar_Track(*RXManage.pGetRadarTrack()));
					++nRXMsgCnt;
					break;
				case CST4_IFF_TRACK_MSG:
					RX_IFF_T.nAdd(new Cst4_IFF_Track(*RXManage.pGetIFFTrack()));
					++nRXMsgCnt;
					break;
				case VISTA_ADSB_MSG:
					RX_ADSB_T.nAdd(new Cst4_ADSB(*RXManage.pGetADSBTrack()));
					++nRXMsgCnt;
					break;
				case CST4_SYSTRACK_MSG:
					RX_Sys_T.nAdd(new Cst4_SysTrack(*RXManage.pGetSysTrack()));
					++nRXMsgCnt;
					break;
				case CST4_USER_MSG:
					RX_User_M.nAdd(new Cst4_User_Msg(*RXManage.pGetUser()));
					++nRXMsgCnt;
					break;
				case VISTA_ALARM_CFG_MSG:
					RX_AlarmCfgList.nAdd(new VistaAlarmCfg(*RXManage.pGetAlarmCfg()));
					++nRXMsgCnt;
					break;
				case VISTA_ALARM_TRIGGER_MSG:
					RX_AlarmTrigList.nAdd(new VistaAlarmTrigger(*RXManage.pGetAlarmTrigger()));
					++nRXMsgCnt;
					break;
//				case VISTA_ALARM_ACK_MSG:
//					RX_AlarmAckList.nAdd(new VistaAlarmAck(*RXManage.pGetAlarmAck()));
//					++nRXMsgCnt;
//             break;
				case VISTA_TIME_MSG:
					RX_TimeMsg=RXManage.GetTimeMsg();
					++nRXMsgCnt;
					break;
				default: ;
				}
			}
		GetStatus(nIdx,sStatus,StatusCol);
		return (nPrevRXMsgCnt!=nRXMsgCnt);
	}

	bool VistaClient::bUpdateRX_Decrypt()
	{
		int nPrevRXMsgCnt=nRXMsgCnt;
		int nRXBytes=nReadData(nIdx,aucRXData,MAX_RX_BUF_SIZE);
		int nPlainSize;
		for (int n=0; n<nRXBytes; n++)
			{
			nPlainSize=pDec->nAddDecryptByte(aucRXData[n]);
		if (nPlainSize>0)
				{
				for (int i=0; i<nPlainSize; i++)
					{
						CST4_MSG_TYPE MsgType=RXManage.AddRXByte(pDec->ucGetPlainByte(i));
					switch(MsgType)
						{
						case CST4_DYNAMIC_AIS_MSG:
							RX_AIS_D.nAdd(new Cst4_AIS_Dynamic(*RXManage.pGetAISDynamic()));
							++nRXMsgCnt;
							break;
						case CST4_STATIC_AIS_MSG:
							RX_AIS_S.nAdd(new Cst4_AIS_Static(*RXManage.pGetAISStatic()));
							++nRXMsgCnt;
							break;
						case CST4_RADAR_SCAN_MSG:
							RX_Radar_S.nAdd(new Cst4_Radar_Scan(*RXManage.pGetRadarScan()));
							++nRXMsgCnt;
							break;
						case CST4_RADAR_TRACK_MSG:
							RX_Radar_T.nAdd(new Cst4_Radar_Track(*RXManage.pGetRadarTrack()));
							++nRXMsgCnt;
							break;
						case CST4_IFF_TRACK_MSG:
							RX_IFF_T.nAdd(new Cst4_IFF_Track(*RXManage.pGetIFFTrack()));
							++nRXMsgCnt;
							break;
						case VISTA_ADSB_MSG:
							RX_ADSB_T.nAdd(new Cst4_ADSB(*RXManage.pGetADSBTrack()));
							++nRXMsgCnt;
							break;
						case CST4_SYSTRACK_MSG:
							RX_Sys_T.nAdd(new Cst4_SysTrack(*RXManage.pGetSysTrack()));
							++nRXMsgCnt;
							break;
						case CST4_USER_MSG:
							RX_User_M.nAdd(new Cst4_User_Msg(*RXManage.pGetUser()));
							++nRXMsgCnt;
							break;
						case VISTA_ALARM_CFG_MSG:
							RX_AlarmCfgList.nAdd(new VistaAlarmCfg(*RXManage.pGetAlarmCfg()));
							++nRXMsgCnt;
							break;
						case VISTA_ALARM_TRIGGER_MSG:
							RX_AlarmTrigList.nAdd(new VistaAlarmTrigger(*RXManage.pGetAlarmTrigger()));
							++nRXMsgCnt;
							break;
//						case VISTA_ALARM_ACK_MSG:
//							RX_AlarmAckList.nAdd(new VistaAlarmAck(*RXManage.pGetAlarmAck()));
//							++nRXMsgCnt;
//							break;
						default: ;
						}
					}
				}
			}
		GetStatus(nIdx,sStatus,StatusCol);
		return (nPrevRXMsgCnt!=nRXMsgCnt);
	}



bool VistaClient::bSendRXWatchdogMsg()
{
	bool bChanged;
	String sStatus;
	TColor StatusCol;
	if (bVerifyConnection(bChanged,sStatus,StatusCol))
		{
		Cst4_Request Msg(JTime(true),0,CST4_REQUEST_RX_WATCHDOG);
		BYTE aucBuf[1024];
		int nTXSize=Msg.nCreateTXBuf(aucBuf);
		return bSendData(nIdx,aucBuf,nTXSize);
		}
	return false;
}


bool VistaClient::bTXRaw(const void* pData, const int nSize)
{
   return bSendData(nIdx,(BYTE*)pData,nSize);

}






