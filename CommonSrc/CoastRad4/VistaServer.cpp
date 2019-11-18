//---------------------------------------------------------------------------
#include <jpch.h>

#pragma hdrstop

#include "VistaServer.h"
#include "VistaEncrypt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


	bool VistaServer::bInitRX(TComponent* pOwnerForm, const int nPort)
	{
		delete pDec;
		pDec=NULL;
		if (bEncrypt)
			{
			pDec=new VistaEncrypt;
			pDec->SetPW(sPW,KeySize);
         }
		ClearAllRX();
      ClearTXMsg();
		nTotalTXCnt=0;
		nTotalTXErrorCnt=0;
		dwNextListen_ms=0;
		ConnectState=CONNECT_OFF;
		Manage.Init(false);
		nRXMsgCnt=0;
		nIdx=nAddServer(pOwnerForm,nPort);
      bool bChanged;
		bool bOK=bVerifyConnection(bChanged,sStatus,StatusCol,true);
		return bOK;
	}

	bool VistaServer::bVerifyConnection(bool& _bChanged, String& _sStatus, TColor& _Col, const bool bImmediate)
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
				GetStatus(nIdx,_sStatus,_Col,false);
				}
			}
		else
			{
			if (bImmediate)
				ConnectState=CONNECT_OFF;
			DWORD dwT_ms;
			switch(ConnectState)
				{
				case CONNECT_OFF:
					ListenForSocketConnect(nIdx);
					dwNextListen_ms=::timeGetTime()+8000+random(5000);
					ConnectState=CONNECT_WAITING;
					_bChanged=true;
					GetStatus(nIdx,_sStatus,_Col,false);
					break;
				case CONNECT_WAITING:
					dwT_ms=::timeGetTime();
					if (dwT_ms>dwNextListen_ms)
						{
						ListenForSocketConnect(nIdx);
						dwNextListen_ms=dwT_ms+8000+random(5000);
						_bChanged=true;
						GetStatus(nIdx,_sStatus,_Col,false);
						}
					break;
				case CONNECT_OK:
					_bChanged=true;
					GetStatus(nIdx,_sStatus,_Col,false);
					ConnectState=CONNECT_WAITING;
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


	bool VistaServer::bUpdateRX()
	{
		if (pDec)
			return bUpdateRX_Decrypt();
		else
			return bUpdateRX_Normal();
	}

	bool VistaServer::bUpdateRX_Normal()
	{
      Cst4_ADSB M;
		int nPrevRXMsgCnt=nRXMsgCnt;
		int nRXBytes=nReadData(nIdx,aucRXData,MAX_RX_SIZE);
		for (int i=0; i<nRXBytes; i++)
			{
			CST4_MSG_TYPE MsgType=Manage.AddRXByte(aucRXData[i]);
			switch(MsgType)
				{
				case CST4_DYNAMIC_AIS_MSG:
					RX_AIS_D.nAdd(new Cst4_AIS_Dynamic(*Manage.pGetAISDynamic()));
					++nRXMsgCnt;
					break;
				case CST4_STATIC_AIS_MSG:
					RX_AIS_S.nAdd(new Cst4_AIS_Static(*Manage.pGetAISStatic()));
					++nRXMsgCnt;
					break;
				case CST4_RADAR_SCAN_MSG:
					RX_Radar_S.nAdd(new Cst4_Radar_Scan(*Manage.pGetRadarScan()));
					++nRXMsgCnt;
					break;
				case CST4_RADAR_TRACK_MSG:
					RX_Radar_T.nAdd(new Cst4_Radar_Track(*Manage.pGetRadarTrack()));
					++nRXMsgCnt;
					break;
				case CST4_IFF_TRACK_MSG:
					RX_IFF_T1.nAdd(new Cst4_IFF_Track(*Manage.pGetIFFTrack()));
					++nRXMsgCnt;
					break;
				case VISTA_ADSB_MSG:
					M=*Manage.pGetADSBTrack();
					RX_ADSB_T.nAdd(new Cst4_ADSB(M));
					++nRXMsgCnt;
					break;
				case CST4_SYSTRACK_MSG:
					RX_Sys_T.nAdd(new Cst4_SysTrack(*Manage.pGetSysTrack()));
					++nRXMsgCnt;
					break;
				case CST4_USER_MSG:
					RX_User_M.nAdd(new Cst4_User_Msg(*Manage.pGetUser()));
					++nRXMsgCnt;
					break;
				case VISTA_ALARM_CFG_MSG:
					RX_AlarmCfgList.nAdd(new VistaAlarmCfg(*Manage.pGetAlarmCfg()));
					++nRXMsgCnt;
					break;
				case VISTA_ALARM_TRIGGER_MSG:
					RX_AlarmTrigList.nAdd(new VistaAlarmTrigger(*Manage.pGetAlarmTrigger()));
					++nRXMsgCnt;
					break;
//				case VISTA_ALARM_ACK_MSG:
//					RX_AlarmAckList.nAdd(new VistaAlarmAck(*Manage.pGetAlarmAck()));
//					++nRXMsgCnt;
//					break;
				case VISTA_TIME_MSG:
					RX_TimeMsg=Manage.GetTimeMsg();
					++nRXMsgCnt;
					break;
				default: ;
				}
			}
		GetStatus(nIdx,sStatus,StatusCol,false);
		return (nPrevRXMsgCnt!=nRXMsgCnt);
	}

	bool VistaServer::bUpdateRX_Decrypt()
	{
		int nPrevRXMsgCnt=nRXMsgCnt;
		int nRXBytes=nReadData(nIdx,aucRXData,MAX_RX_SIZE);
		int nPlainSize;
		for (int n=0; n<nRXBytes; n++)
			{
			nPlainSize=pDec->nAddDecryptByte(aucRXData[n]);
			if (nPlainSize>0)
				{
				for (int i=0; i<nPlainSize; i++)
					{
					CST4_MSG_TYPE MsgType=Manage.AddRXByte(pDec->ucGetPlainByte(i));
					switch(MsgType)
						{
						case CST4_DYNAMIC_AIS_MSG:
							RX_AIS_D.nAdd(new Cst4_AIS_Dynamic(*Manage.pGetAISDynamic()));
							++nRXMsgCnt;
							break;
						case CST4_STATIC_AIS_MSG:
							RX_AIS_S.nAdd(new Cst4_AIS_Static(*Manage.pGetAISStatic()));
							++nRXMsgCnt;
							break;
						case CST4_RADAR_SCAN_MSG:
							RX_Radar_S.nAdd(new Cst4_Radar_Scan(*Manage.pGetRadarScan()));
							++nRXMsgCnt;
							break;
						case CST4_RADAR_TRACK_MSG:
							RX_Radar_T.nAdd(new Cst4_Radar_Track(*Manage.pGetRadarTrack()));
							++nRXMsgCnt;
							break;
						case CST4_IFF_TRACK_MSG:
							RX_IFF_T1.nAdd(new Cst4_IFF_Track(*Manage.pGetIFFTrack()));
							++nRXMsgCnt;
							break;
						case VISTA_ADSB_MSG:
							RX_ADSB_T.nAdd(new Cst4_ADSB(*Manage.pGetADSBTrack()));
							++nRXMsgCnt;
							break;
						case CST4_SYSTRACK_MSG:
							RX_Sys_T.nAdd(new Cst4_SysTrack(*Manage.pGetSysTrack()));
							++nRXMsgCnt;
							break;
						case CST4_USER_MSG:
							RX_User_M.nAdd(new Cst4_User_Msg(*Manage.pGetUser()));
							++nRXMsgCnt;
							break;
						case VISTA_ALARM_CFG_MSG:
							RX_AlarmCfgList.nAdd(new VistaAlarmCfg(*Manage.pGetAlarmCfg()));
							++nRXMsgCnt;
							break;
						case VISTA_ALARM_TRIGGER_MSG:
							RX_AlarmTrigList.nAdd(new VistaAlarmTrigger(*Manage.pGetAlarmTrigger()));
							++nRXMsgCnt;
							break;
//						case VISTA_ALARM_ACK_MSG:
//							RX_AlarmAckList.nAdd(new VistaAlarmAck(*Manage.pGetAlarmAck()));
//							++nRXMsgCnt;
//							break;
						case VISTA_TIME_MSG:
							RX_TimeMsg=Manage.GetTimeMsg();
							++nRXMsgCnt;
							break;
						default: ;
						}
					}
				}
			}
		GetStatus(nIdx,sStatus,StatusCol,false);
		return (nPrevRXMsgCnt!=nRXMsgCnt);
	}

/****    TRANSMIT    ******************************************************/

void VistaServer::ClearTXMsg()
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

void VistaServer::AddTXMessage(Cst4_AIS_Dynamic& Msg)
{
	TX_AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_AIS_Static& Msg)
{
	TX_AIS_S.nAdd(new Cst4_AIS_Static(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_Radar_Scan& Msg)
{
	TX_Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_Radar_Track& Msg)
{
	TX_Radar_T.nAdd(new Cst4_Radar_Track(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_IFF_Track& Msg)
{
	TX_IFF_T1.nAdd(new Cst4_IFF_Track(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_ADSB& Msg)
{
	TX_ADSB_T.nAdd(new Cst4_ADSB(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_SysTrack& Msg)
{
	TX_Sys_T.nAdd(new Cst4_SysTrack(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(Cst4_User_Msg& Msg)
{
	TX_User_M.nAdd(new Cst4_User_Msg(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(VistaAlarmCfg& Msg)
{
	TX_AlarmCfgList.nAdd(new VistaAlarmCfg(Msg));
	++nTXMsgAvail;
}

void VistaServer::AddTXMessage(VistaAlarmTrigger& Msg)
{
	TX_AlarmTrigList.nAdd(new VistaAlarmTrigger(Msg));
	++nTXMsgAvail;
}

//void VistaServer::AddTXMessage(VistaAlarmAck& Msg)
//{
//	TX_AlarmAckList.nAdd(new VistaAlarmAck(Msg));
//	++nTXMsgAvail;
//}

void VistaServer::AddTXMessage(VistaTimeMessage& Msg)
{
   TX_TimeMsg=Msg;
	++nTXMsgAvail;
}


bool VistaServer::bTXMessages()
{
	bool bOK=true;
	if (nTXMsgAvail>0)
		{
		bOK=false;
		int nTXSize=0;
		bool bTXBufOK=true;
		int nMaxBuf=MAX_TX_BUF_SIZE-65536;
		if (TX_TimeMsg.bValid())
			{
			nTXSize+=TX_TimeMsg.nCreateTXBuf(&aucTXBuf[nTXSize]);
			if (nTXSize>nMaxBuf)
				bTXBufOK=false;
			}
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

//		TX_AlarmAckList.GoFirst();
//		while(!TX_AlarmAckList.bLast() && bTXBufOK)
//			{
//			nTXSize+=TX_AlarmAckList.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
//			if (nTXSize>nMaxBuf)
//				bTXBufOK=false;
//			}

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
	GetStatus(nIdx,sStatus,StatusCol,false);
	return bOK;
}


