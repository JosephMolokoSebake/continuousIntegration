//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4DataServer.h"
#include "Cst4Defs.h"
#include "Cst4SiteDefs.h"
#include "JFile.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4DataServer::Cst4DataServer(TComponent* _pOwner, const int _nMaxDormant_min, const bool bEnableDebugging) : LJTcpServerSockets(5,bEnableDebugging,true,1)
{
	pServerLock=new TCriticalSection;

   nRXBlock=0;

   pOwner=_pOwner;
	dwMaxDormant_sec=_nMaxDormant_min*60;
	nPortTXCnt=0;
	nPortErrCnt=0;
	nTimeTXCnt=0;
	nMaxTotalRXBuf=0;
	sLogBase="";
	bServerChanged=false;
	ResponseList.Clear();
	RequestList.Clear();
	RequestRX.Init(false);
//	bKeepAlive=false;
}

Cst4DataServer::~Cst4DataServer()
{
	StoreLogInfo("Closed Data Server");
	delete pServerLock;
}

int Cst4DataServer::nCreateServer(const int nSiteID)
{
	pServerLock->Acquire();
	int nPort=-1;
	int nIndex=nAddServer(pOwner);
	if (nIndex>=0)
		{
		ListenForSocketConnect(nIndex);
		nPort=nGetServerPort(nIndex);
		String sDescr;
		if (g_pSiteDefs)
			sDescr=g_pSiteDefs->sGetDescrFromID(nSiteID);//+" Socket";
		else
			sDescr="";
		ServerList.nAdd(new Cst4ServerInstance(nPort,nIndex,sDescr));
		ServerList.Pack();
		bServerChanged=true;
		StoreLogInfo("Created "+sDescr+", Port="+String(nPort));
		}
	pServerLock->Release();
	return nPort;
}

bool Cst4DataServer::bTXData(Cst4ServerInstance* pS, BYTE* pucData, const int nSize)
{
	int nIndex=pS->nGetIndex();
	return bSendData(nIndex,pucData,nSize);
}

int Cst4DataServer::nCheckServer(Cst4ServerInstance* pS)
{
	pServerLock->Acquire();
	int nMsgCnt=0;
	int nIndex=pS->nGetIndex();
	int nRXSize=nReadData(nIndex,aucRXData,MAX_RX_SIZE);
	pS->UpdateMaxRXBuf(nRXSize);

	++nRXBlock;
	for (int i=0; i<nRXSize; i++)
		{
		CST4_MSG_TYPE MsgType=pS->AddRXByte(aucRXData[i]);
//      int nTXNum=pS->nGetTXNum();
		switch(MsgType)
			{
			case CST4_DYNAMIC_AIS_MSG:
				pS->pGetAISDynamic()->SetServerIndex(nIndex);
				AIS_D.nAdd(new Cst4_AIS_Dynamic(*pS->pGetAISDynamic()));
				++nMsgCnt;
				break;
			case CST4_STATIC_AIS_MSG:
				pS->pGetAISStatic()->SetServerIndex(nIndex);
				AIS_S.nAdd(new Cst4_AIS_Static(*pS->pGetAISStatic()));
				++nMsgCnt;
				break;
			case CST4_RADAR_SCAN_MSG:
				pS->pGetRadarScan()->SetServerIndex(nIndex);
				Radar_S.nAdd(new Cst4_Radar_Scan(*pS->pGetRadarScan()));
				++nMsgCnt;
				break;
			case CST4_RADAR_TRACK_MSG:
				pS->pGetRadarTrack()->SetServerIndex(nIndex);
				Radar_T.nAdd(new Cst4_Radar_Track(*pS->pGetRadarTrack()));
				++nMsgCnt;
				break;
			case CST4_IFF_TRACK_MSG:
				pS->pGetIFFTrack()->SetServerIndex(nIndex);
				IFF_T1.nAdd(new Cst4_IFF_Track(*pS->pGetIFFTrack()));
				++nMsgCnt;
				break;
			case VISTA_ADSB_MSG:
				pS->pGetADSBTrack()->SetServerIndex(nIndex);
				ADSB_T.nAdd(new Cst4_ADSB(*pS->pGetADSBTrack()));
				++nMsgCnt;
				break;
			case VISTA_ALARM_TRIGGER_MSG:
				pS->pGetAlarmTrig()->SetServerIndex(nIndex);
				Alarm_T.nAdd(new VistaAlarmTrigger(*pS->pGetAlarmTrig()));
				++nMsgCnt;
				break;
			case CST4_SYSTRACK_MSG:
				pS->pGetSysTrack()->SetServerIndex(nIndex);
				Sys_T.nAdd(new Cst4_SysTrack(*pS->pGetSysTrack()));
				++nMsgCnt;
				break;
			case CST4_USER_MSG:
				pS->pGetUser()->SetServerIndex(nIndex);
				User_M.nAdd(new Cst4_User_Msg(*pS->pGetUser()));
				++nMsgCnt;
				break;
//			case CST4_KEEPALIVE_MSG:
//
//
//				break;
			default: ;
			}
		}
	pS->AddMsgCnt(nMsgCnt);
	pServerLock->Release();
	return nMsgCnt;
}

int Cst4DataServer::nCheckServerDecrypt(Cst4ServerInstance* pS)
{
	pServerLock->Acquire();
	int nMsgCnt=0;
	int nIndex=pS->nGetIndex();
	int nRXSize=nReadData(nIndex,aucRXData,MAX_RX_SIZE);
	pS->UpdateMaxRXBuf(nRXSize);
	++nRXBlock;

   pS->SetPW(sPW,KeySize);
	int nPlainSize;
	for (int n=0; n<nRXSize; n++)
		{
		nPlainSize=pS->nAddDecryptByte(aucRXData[n]);
		if (nPlainSize>0)
			{
			for (int i=0; i<nPlainSize; i++)
				{
				CST4_MSG_TYPE MsgType=pS->AddRXByte(pS->ucGetPlainByte(i));
				switch(MsgType)
					{
					case CST4_DYNAMIC_AIS_MSG:
						pS->pGetAISDynamic()->SetServerIndex(nIndex);
						AIS_D.nAdd(new Cst4_AIS_Dynamic(*pS->pGetAISDynamic()));
						++nMsgCnt;
						break;
					case CST4_STATIC_AIS_MSG:
						pS->pGetAISStatic()->SetServerIndex(nIndex);
						AIS_S.nAdd(new Cst4_AIS_Static(*pS->pGetAISStatic()));
						++nMsgCnt;
						break;
					case CST4_RADAR_SCAN_MSG:
						pS->pGetRadarScan()->SetServerIndex(nIndex);
						Radar_S.nAdd(new Cst4_Radar_Scan(*pS->pGetRadarScan()));
						++nMsgCnt;
						break;
					case CST4_RADAR_TRACK_MSG:
						pS->pGetRadarTrack()->SetServerIndex(nIndex);
						Radar_T.nAdd(new Cst4_Radar_Track(*pS->pGetRadarTrack()));
						++nMsgCnt;
						break;
					case CST4_IFF_TRACK_MSG:
						pS->pGetIFFTrack()->SetServerIndex(nIndex);
						IFF_T1.nAdd(new Cst4_IFF_Track(*pS->pGetIFFTrack()));
						++nMsgCnt;
						break;
					case VISTA_ADSB_MSG:
						pS->pGetADSBTrack()->SetServerIndex(nIndex);
						ADSB_T.nAdd(new Cst4_ADSB(*pS->pGetADSBTrack()));
						++nMsgCnt;
						break;
					case CST4_SYSTRACK_MSG:
						pS->pGetSysTrack()->SetServerIndex(nIndex);
						Sys_T.nAdd(new Cst4_SysTrack(*pS->pGetSysTrack()));
						++nMsgCnt;
						break;
					case CST4_USER_MSG:
						pS->pGetUser()->SetServerIndex(nIndex);
						User_M.nAdd(new Cst4_User_Msg(*pS->pGetUser()));
						++nMsgCnt;
						break;
					default: ;
					}
				}
			pS->AddMsgCnt(nMsgCnt);
			pServerLock->Release();
			}
		}
	return nMsgCnt;
}


bool Cst4DataServer::bUpdate()
{
	pServerLock->Acquire();
	bool bDataAvail=false;
	DWORD dwutc_sec=JTime(true,true).dwTotalSec();
	ServerList.GoFirst();
	Cst4ServerInstance* pS;
	while(!ServerList.bLast())
		{
		pS=ServerList.pNext();
		if (pS->nGetIndex()>=0)
			{
			if (bDecrypt)
				{
				if (nCheckServerDecrypt(pS)>0)
					{
					int nMax=pS->nGetMaxRXBuf();
					if (nMax>nMaxTotalRXBuf)
						nMaxTotalRXBuf=nMax;
					pS->SetLastData_utc(dwutc_sec);
					bDataAvail=true;
					}
				}
			else
				{
				if (nCheckServer(pS)>0)
					{
					int nMax=pS->nGetMaxRXBuf();
					if (nMax>nMaxTotalRXBuf)
						nMaxTotalRXBuf=nMax;
					pS->SetLastData_utc(dwutc_sec);
					bDataAvail=true;
					}
				}
			}
		}
	pServerLock->Release();
	return bDataAvail;
}

	bool Cst4DataServer::bTXVistaTime(VistaTimeMessage Msg)
	{
	BYTE aucBuf[8192];
   int nTXSize=Msg.nCreateTXBuf(aucBuf);
	pServerLock->Acquire();
	ServerList.GoFirst();
	Cst4ServerInstance* pS;
	bool bOK=true;
	while(!ServerList.bLast())
		{
		pS=ServerList.pNext();
		if (pS->nGetIndex()>=0)
			{
			if (!bTXData(pS,aucBuf,nTXSize))
				bOK=false;
			}
		}
	pServerLock->Release();
   return bOK;
}


//
void Cst4DataServer::PurgeServerList(const DWORD dwEmpty_sec)
{
	pServerLock->Acquire();
	DWORD dwutc_sec=JTime(true,true).dwTotalSec();
	Cst4ServerInstance* pS;
	ServerList.GoFirst();
	while(!ServerList.bLast())
		{
		pS=ServerList.pNext();
		DWORD dwAge_sec=dwutc_sec-pS->dwGetLastData_utc_sec();
		pS->SetDataAge(dwAge_sec);
		if (dwAge_sec>dwEmpty_sec)
			{
			StoreLogInfo("Deleted "+pS->sGetDescr()+", Port="+String(pS->nGetPort()));
			ServerList.GoPrev();
			ServerList.Delete();
			bServerChanged=true;
			}
		}
	ServerList.Pack();
	pServerLock->Release();
}

int Cst4DataServer::nFindOldestDataAge()
{
	int nOldest_sec=0;
	pServerLock->Acquire();
	DWORD dwutc_sec=JTime(true,true).dwTotalSec();
	Cst4ServerInstance* pS;
	ServerList.GoFirst();
	while(!ServerList.bLast())
		{
		pS=ServerList.pNext();
		int nAge_sec=(int)(dwutc_sec-pS->dwGetLastData_utc_sec());
		if (nAge_sec>nOldest_sec)
			nOldest_sec=nAge_sec;
		}
	ServerList.Pack();
	pServerLock->Release();
	return nOldest_sec;
}


int Cst4DataServer::nProcessRequests(const BYTE* pucRXData, const int nRXSize, const int nOwnID)
{
	pServerLock->Acquire();
	RequestList.Clear();
	ResponseList.Clear();
	BYTE aucResponseData[1024];
	int nResponseSize;
	Cst4Time LastUpdateTime_utc;
	for (int i=0; i<nRXSize; i++)
		{
		if (RequestRX.AddRXByte(pucRXData[i])==CST4_REQUEST_MSG)
			{
			DWORD dwRequest=RequestRX.GetRequest().dwGetRequest();
			WORD wSrcSite=RequestRX.GetRequest().wSource();
			JTime RequestTime=RequestRX.GetRequest().Time_utc();
			WORD wTXID;
			if (nOwnID>=0)
				wTXID=(WORD)nOwnID;
			else
				wTXID=wSrcSite;
			int nPort;
			JTime T_utc;
			switch(dwRequest)
				{
				case CST4_REQUEST_SERVER_PORT:
					pServerLock->Release();
					nPort=nCreateServer(wSrcSite);
					pServerLock->Acquire();
					if (nPort>=0)
						{
						ResponseList.nAdd(new Cst4_Response(JTime(true,true),wTXID,dwRequest,&nPort,sizeof(nPort))); //NB Time is utc
						++nPortTXCnt;
						RequestList.nAdd(new RequestListStruct(dwRequest,wSrcSite,nPort));
						}
					else
						++nPortErrCnt;
					nLastServerPort=nPort;
					break;
				case CST4_REQUEST_TIME_SYNC:
					LastUpdateTime_utc=Cst4Time(JTime(g_dwCst4LastUpdate_utc_sec,true));
					nResponseSize=0;
					LastUpdateTime_utc.Pack(aucResponseData,nResponseSize);
					T_utc=JTime(true,true);
					ResponseList.nAdd(new Cst4_Response(T_utc,wTXID,dwRequest,aucResponseData,nResponseSize));	//NB Time is utc
/*
					if (g_pDebugTimeFil)
						{
						String s;
						s.printf(L"TIME REQUEST RECEIVED FROM SITE #%04X",wSrcSite);
						g_pDebugTimeFil->TextLine(s);;
						g_pDebugTimeFil->TextLine("  Time in Request Message: "+RequestTime.sDateTime(false,false,true));
						g_pDebugTimeFil->TextLine("  Responded with Current PC Time: "+T_utc.sDateTime(false,false,true));
						s.printf(L"  UTC - LOCAL = %d hours",g_nCst4TimeBias_hours);
						g_pDebugTimeFil->TextLine(s);;
						g_pDebugTimeFil->TextLine("");
						}
*/
					++nTimeTXCnt;
					RequestList.nAdd(new RequestListStruct(dwRequest,wSrcSite));
					break;
				case CST4_REQUEST_SOURCE_UPDATE:
					break;
				case CST4_REQUEST_SITE_UPDATE:
					break;
				case CST4_REQUEST_RX_WATCHDOG:
					ResponseList.nAdd(new Cst4_Response(JTime(true,true),wTXID,dwRequest)); //NB Time is utc
					RequestList.nAdd(new RequestListStruct(dwRequest,wSrcSite));
					break;
				case CST4_REQUEST_TX_WATCHDOG:
					ResponseList.nAdd(new Cst4_Response(JTime(true,true),wTXID,dwRequest)); //NB Time is utc
					RequestList.nAdd(new RequestListStruct(dwRequest,wSrcSite));
					break;
				default: ;
				}
			}
		}
	if (RequestList.nGetCount()>0)
		{
		ResponseList.Pack();
		RequestList.Pack();
		}
	pServerLock->Release();
	return RequestList.nGetCount();
}

int Cst4DataServer::nGetResponse(const int n, BYTE* pucTXData)
{
	return ResponseList[n]->nCreateTXBuf(pucTXData);
}

Cst4_Response* Cst4DataServer::pGetResponse(const int n)
{
	return ResponseList[n];
}

void Cst4DataServer::StoreLogInfo(const String sInfo)
{
	if (!sLogBase.IsEmpty())
		{
		JTime T(true);
		String sFile;
		sFile.printf(L"%04d%02d%02d",T.Get().wYear,T.Get().wMonth,T.Get().wDay);
		sFile=sLogBase+"_Log_"+sFile+".log";
		JFile::FILE_ERROR E;
		JFile Fil('B',JFile::ASCII_TYPE);
		if (bFileExist(sFile))
			E=Fil.Open(sFile);
		else
			E=Fil.Create(sFile);
		if (!E)
			Fil.AppendTextLine(T.sDateTime(true,false,true)+" "+sInfo);
		}
}

