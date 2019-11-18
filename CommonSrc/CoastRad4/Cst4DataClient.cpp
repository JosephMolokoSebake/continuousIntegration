//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4DataClient.h"
#include "Cst4UserDefs.h"
#include "VistaEncrypt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Cst4DataClient::AddMessage(Cst4_AIS_Dynamic& Msg, const int nDest)
{
   if (DestList.nGetCount()>0)
      {
      if (nDest>=0)
         DestList[nDest]->AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
      else
         {
         for (int i=0; i<DestList.nGetCount(); i++)
            DestList[i]->AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
         }
      ++nMsgAvail;
      }
}

void Cst4DataClient::AddMessage(Cst4_AIS_Static& Msg, const int nDest)
{
   if (DestList.nGetCount()>0)
      {
      if (nDest>=0)
         DestList[nDest]->AIS_S.nAdd(new Cst4_AIS_Static(Msg));
      else
         {
         for (int i=0; i<DestList.nGetCount(); i++)
            DestList[i]->AIS_S.nAdd(new Cst4_AIS_Static(Msg));
         }
      ++nMsgAvail;
      }
}

void Cst4DataClient::AddMessage(Cst4_Radar_Scan& Msg, const int nDest)
{
   if (DestList.nGetCount()>0)
      {
      if (nDest>=0)
         DestList[nDest]->Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
      else
         {
         for (int i=0; i<DestList.nGetCount(); i++)
            DestList[i]->Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
         }
      ++nMsgAvail;
      }
}

void Cst4DataClient::AddMessage(Cst4_Radar_Track& Msg, const int nDest)
{
   if (DestList.nGetCount()>0)
      {
      if (nDest>=0)
         DestList[nDest]->Radar_T.nAdd(new Cst4_Radar_Track(Msg));
      else
         {
         for (int i=0; i<DestList.nGetCount(); i++)
            DestList[i]->Radar_T.nAdd(new Cst4_Radar_Track(Msg));
         }
      ++nMsgAvail;
      }
}

void Cst4DataClient::AddMessage(Cst4_IFF_Track& Msg, const int nDest)
{
	if (DestList.nGetCount()>0)
		{
		if (nDest>=0)
			DestList[nDest]->IFF_T.nAdd(new Cst4_IFF_Track(Msg));
		else
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				DestList[i]->IFF_T.nAdd(new Cst4_IFF_Track(Msg));
			}
		++nMsgAvail;
		}
}

void Cst4DataClient::AddMessage(Cst4_ADSB& Msg, const int nDest)
{
	if (DestList.nGetCount()>0)
		{
		if (nDest>=0)
			DestList[nDest]->ADSB_T.nAdd(new Cst4_ADSB(Msg));
		else
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				DestList[i]->ADSB_T.nAdd(new Cst4_ADSB(Msg));
			}
		++nMsgAvail;
		}
}

void Cst4DataClient::AddMessage(Cst4_SysTrack& Msg, const int nDest)
{
	if (DestList.nGetCount()>0)
		{
		if (nDest>=0)
			DestList[nDest]->Sys_T.nAdd(new Cst4_SysTrack(Msg));
		else
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				DestList[i]->Sys_T.nAdd(new Cst4_SysTrack(Msg));
			}
      ++nMsgAvail;
      }
}

void Cst4DataClient::AddMessage(Cst4_User_Msg& Msg, const int nDest)
{
	if (DestList.nGetCount()>0)
		{
		if (nDest>=0)
			DestList[nDest]->User_M.nAdd(new Cst4_User_Msg(Msg));
		else
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				DestList[i]->User_M.nAdd(new Cst4_User_Msg(Msg));
			}
		++nMsgAvail;
		}
}

void Cst4DataClient::AddMessage(VistaAlarmTrigger& Msg, const int nDest)
{
	if (DestList.nGetCount()>0)
		{
		if (nDest>=0)
			DestList[nDest]->Alarm_T.nAdd(new VistaAlarmTrigger(Msg));
		else
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				DestList[i]->Alarm_T.nAdd(new VistaAlarmTrigger(Msg));
			}
		++nMsgAvail;
		}
}




/*
void Cst4DataClient::WriteDebug(const BYTE* pucData, const int nSize, const int nTXBlockNum, JFile* pDebugFil)
{
	String s;
	CST4_MSG_TYPE MsgType;
	int n=0;
	for (int i=0; i<nSize; i++)
		{
		++n;
		MsgType=TXDebug.AddRXByte(pucData[i],NULL);
		int nTXCnt=TXDebug.nGetTXNum();
		s.printf(L"%5d,%5d,%5d,",nTXCnt,nTXBlockNum,n);
		switch(MsgType)
			{
			case CST4_DYNAMIC_AIS_MSG:
				pDebugFil->TextLine(s+"AIS D");
				break;
			case CST4_STATIC_AIS_MSG:
				pDebugFil->TextLine(s+"AIS S");
				break;
			case CST4_RADAR_SCAN_MSG:
				pDebugFil->TextLine(s+"Radar S");
				break;
			case CST4_RADAR_TRACK_MSG:
				pDebugFil->TextLine(s+"Radar T");
				break;
			case CST4_REQUEST_MSG:
				pDebugFil->TextLine(s+"Request");
				break;
			case CST4_RESPONSE_MSG:
				pDebugFil->TextLine(s+"Response");
				break;
			case CST4_BITE_MSG:
				pDebugFil->TextLine(s+"BITE");
				break;
			case CST4_IFF_TRACK_MSG:
				pDebugFil->TextLine(s+"IFF");
				break;
			case CST4_SAR_MSG_OBSOLETE:
				pDebugFil->TextLine(s+"SAR");
				break;
			case CST4_RADAR_TRACK_INFO_MSG:
				break;
			case CST4_USER_MSG:
				if (TXDebug.pGetUser()->dwGetType()==CST4_USER_PICTUREBLOCK)
					{
					int nSize=TXDebug.pGetUser()->nGetPayloadSize();
					if (nSize>0)
						{
						Cst4_Block_Data Block;
						Block.UnPack((BYTE*)TXDebug.pGetUser()->pGetPayload());
						int nTotal=Block.nGetTotalBlocks();
						int nBlockNum=Block.nGetBlockNum();
						pDebugFil->TextLine(s+"JPEG "+String(nBlockNum+1)+"/"+String(nTotal));
						}
					}
				else
					{
					pDebugFil->TextLine(s+"USER "+String(TXDebug.pGetUser()->dwGetType()));
					}
				break;
			case CST4_SYSTRACK_MSG:
				pDebugFil->TextLine(s+"SysTrack");
				break;
			}
		}



}
*/


bool Cst4DataClient::bTXSingleDest(const int nDest)
{
	bool bOK=true;
	if ((nDest>=0)&&(nDest<DestList.nGetCount()))
		{
		DestInstance* pD=DestList[nDest];
		pD->nTotalTXCnt=0;
		if (pD->nDestClientIndex>=0)
			{
			int nTXSize=0;
			bool bTXBufOK=true;
			int nMaxBuf=MAX_TX_BUF_SIZE-65536;

			pD->AIS_D.GoFirst();
			while(!pD->AIS_D.bLast() && bTXBufOK)
				{
				nTXSize+=pD->AIS_D.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}

			pD->AIS_S.GoFirst();
			while(!pD->AIS_S.bLast() && bTXBufOK)
				{
				nTXSize+=pD->AIS_S.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}

			pD->Radar_S.GoFirst();
			while(!pD->Radar_S.bLast() && bTXBufOK)
				{
				nTXSize+=pD->Radar_S.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}


			pD->Radar_T.GoFirst();
			while(!pD->Radar_T.bLast() && bTXBufOK)
				{
				nTXSize+=pD->Radar_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}


			pD->IFF_T.GoFirst();
			while(!pD->IFF_T.bLast() && bTXBufOK)
				{
				nTXSize+=pD->IFF_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}


			pD->Sys_T.GoFirst();
			while(!pD->Sys_T.bLast() && bTXBufOK)
				{
				nTXSize+=pD->Sys_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}


			pD->User_M.GoFirst();
			while(!pD->User_M.bLast() && bTXBufOK)
				{
				nTXSize+=pD->User_M.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
				if (nTXSize>nMaxBuf)
					bTXBufOK=false;
				}

			pD->Alarm_T.GoFirst();
			while(!pD->Alarm_T.bLast() && bTXBufOK)
				{
				nTXSize+=pD->Alarm_T.pNext()->nCreateTXBuf(&aucTXBuf[nTXSize]);
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
//				if (pDebugFil)
//					WriteDebug(aucTXBuf,nTXSize,nTXBlockNum,pDebugFil);

				if (bSendData(pD->nDestClientIndex,aucTXBuf,nTXSize))
					{
					pD->UpdateTXCount();
					pD->bDestClientConnected=true;
					pD->nConnectedTXErrorCnt=0;
					}
				else
					{
					bOK=false;
//					if (pDebugFil)
//						pDebugFil->TextLine(",,,TX ERROR");
					++pD->nTXErrorCnt;
					if (pD->nConnectedTXErrorCnt>=0)
						{
						if (++pD->nConnectedTXErrorCnt>3000) 	//about 5 min
							pD->bDestClientConnected=false;
						}
					}
				}
			GetStatus(pD->nDestClientIndex,pD->sClientStatus,pD->ClientStatusCol);
			}
		else
			bOK=false;
		pD->ClearMsg();
		}
	return bOK;
}


bool Cst4DataClient::bTXMessages(const int nDest)
{
	bool bOK=true;
	if (nMsgAvail>0)
		{
		if (nDest>=0)
			bOK=bTXSingleDest(nDest);
		else
			{
			if (DestList.nGetCount()>0)
				{
				for (int i=0; i<DestList.nGetCount(); i++)
					{
					if (!bTXSingleDest(i))
						bOK=false;
					}
				}
			else
				bOK=false;
			}
		nMsgAvail=0;
		}
	return bOK;
}

/*
bool Cst4DataClient::bTXSingleKeepAlive(Cst4_KeepAlive& Msg, const int nDest)
{
	int nTXSize=Msg.nCreateTXBuf(aucTXBuf);
	bool bOK=true;
	if ((nDest>=0)&&(nDest<DestList.nGetCount()))
		{
		DestInstance* pD=DestList[nDest];
		pD->nTotalTXCnt=0;
		if (pD->nDestClientIndex>=0)
			{
			if (nTXSize>0)
				{
				if (bSendData(pD->nDestClientIndex,aucTXBuf,nTXSize))
					{
					pD->UpdateTXCount();
					pD->bDestClientConnected=true;
					pD->nConnectedTXErrorCnt=0;
					}
				else
					{
					bOK=false;
					++pD->nTXErrorCnt;
					if (pD->nConnectedTXErrorCnt>=0)
						{
						if (++pD->nConnectedTXErrorCnt>3000) 	//about 5 min
							pD->bDestClientConnected=false;
						}
					}
				}
			GetStatus(pD->nDestClientIndex,pD->sClientStatus,pD->ClientStatusCol);
			}
		pD->ClearMsg();
		}
	return bOK;
}



bool Cst4DataClient::bTXKeepAlive(const int nSourceID, const int nDest)
{
	bool bOK=true;
	Cst4_KeepAlive Msg(JTime(true,true),nSourceID);
	if (nDest>=0)
		bOK=bTXSingleKeepAlive(Msg,nDest);
	else
		{
		if (DestList.nGetCount()>0)
			{
			for (int i=0; i<DestList.nGetCount(); i++)
				{
				if (!bTXSingleKeepAlive(Msg,i))
					bOK=false;
				}
			}
		else
			bOK=false;
		}
	return bOK;
}
*/

bool Cst4DataClient::bRequestDestPort_utc(const int nDest, DWORD dwT_utc_sec)
{
	bool bOK=false;
	if ((nDest>=0)&&(nDest<DestList.nGetCount()))
		{
		DestInstance* pDest=DestList[nDest];
		if (pDest->nManageClientIndex>=0)
			{
			if (dwT_utc_sec==0)
				dwT_utc_sec=JTime(true,true).dwTotalSec();
			DWORD dwT_loc_sec=dwCst4ConvertToLocal(dwT_utc_sec);
			if (!bClientConnected(pDest->nManageClientIndex))
				{
				if (dwT_loc_sec>=pDest->dwNextRequest_loc_sec)
					{
					pDest->dwNextRequest_loc_sec=dwT_loc_sec+10;
					ConnectClient(pDest->nManageClientIndex);
					DWORD dwT_ms=::GetTickCount();
					while(!bClientConnected(pDest->nManageClientIndex)&&(::GetTickCount()-dwT_ms<100)&&(!bAbortAll))
						{
						Application->ProcessMessages();
						}
					}
				}
			else
				pDest->dwNextRequest_loc_sec=dwT_loc_sec+10;
			if (bClientConnected(pDest->nManageClientIndex))
				{
				Cst4_Request Msg(JTime(true),pDest->wOwnSite,CST4_REQUEST_SERVER_PORT);
				BYTE aucBuf[1024];
				int nTXSize=Msg.nCreateTXBuf(aucBuf);
				if (bSendData(pDest->nManageClientIndex,aucBuf,nTXSize))
					{
					++pDest->nRequestsPending;
					bOK=true;
					}
				}
			GetStatus(pDest->nManageClientIndex,pDest->sManageStatus,pDest->ManageStatusCol);
			}
		}
	return bOK;
}

int Cst4DataClient::nCreateDestination(const String sIPAddr, const int nManagePort, const WORD wOwnSite)
{
	DestInstance* pDest;
	pDest=new DestInstance;
	pDest->sIPAddr=sIPAddr;
	pDest->wOwnSite=wOwnSite;
	pDest->nManageClientIndex=nAddClient(pOwner,sIPAddr,nManagePort);
	pDest->nDestClientPort=-1;
	if (pDest->nManageClientIndex>=0)
		{
		ConnectClient(pDest->nManageClientIndex);
		DWORD dwT=::GetTickCount();
		while(!bClientConnected(pDest->nManageClientIndex)&&(::GetTickCount()-dwT<100)&&(!bAbortAll))
			{
			Application->ProcessMessages();
			}
		pDest->bManageConnected=bClientConnected(pDest->nManageClientIndex);
		}
	else
		pDest->bManageConnected=false;
	GetStatus(pDest->nManageClientIndex,pDest->sManageStatus,pDest->ManageStatusCol);
	pDest->nRequestsPending=0;
	DestList.nAdd(pDest);
	DestList.Pack();
	int nDest=DestList.nGetCount()-1;
	bRequestDestPort_utc(nDest);
	return nDest;
}

bool Cst4DataClient::bFreshManageConnected(const int nDest)
{
	if ((nDest<DestList.nGetCount())&&(nDest>=0))
		{
		DestInstance* pD=DestList[nDest];
		if (pD->nManageClientIndex>=0)
			{
			bool bConn=bClientConnected(pD->nManageClientIndex);
			if (bGetStatusChanged(pD->nManageClientIndex))
				GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
			if (bConn && !pD->bManageConnected)
				{
				pD->bManageConnected=bConn;
				return true;
				}
			else
				pD->bManageConnected=bConn;
			}
		}
	return false;
}


bool Cst4DataClient::bRequestDestTime_utc(const int nDest, DWORD dwT_utc_sec)
{
	bool bOK=false;
	if ((nDest<DestList.nGetCount())&&(nDest>=0))
		{
		DestInstance* pD=DestList[nDest];
		if (pD->nManageClientIndex>=0)
			{
			if (dwT_utc_sec==0)
				dwT_utc_sec=JTime(true,true).dwTotalSec();
			DWORD dwT_loc_sec=dwCst4ConvertToLocal(dwT_utc_sec);
			if (!bClientConnected(pD->nManageClientIndex))
            {
				if (dwT_loc_sec>=pD->dwNextRequest_loc_sec)
					{
					pD->dwNextRequest_loc_sec=dwT_loc_sec+10;
					ConnectClient(pD->nManageClientIndex);
					DWORD dwT_ms=::GetTickCount();
					while(!bClientConnected(pD->nManageClientIndex)&&(::GetTickCount()-dwT_ms<100)&&(!bAbortAll))
						{
						Application->ProcessMessages();
						}
               }
            }
         else
            pD->dwNextRequest_loc_sec=dwT_loc_sec+10;
         if (bClientConnected(pD->nManageClientIndex))
				{
				JTime T_utc=JTime(true,true);
				Cst4_Request Msg(T_utc,pD->wOwnSite,CST4_REQUEST_TIME_SYNC);
				BYTE aucBuf[1024];
				int nTXSize=Msg.nCreateTXBuf(aucBuf);
				if (bSendData(pD->nManageClientIndex,aucBuf,nTXSize))
					++pD->nRequestsPending;
/*
				if (g_pDebugTimeFil)
					{
					g_pDebugTimeFil->TextLine("TIME REQUESTED @ "+T_utc.sDateTime(false,false,true));
					g_pDebugTimeFil->TextLine("  UTC - LOCAL = "+String(g_nCst4TimeBias_hours)+" hours");
					g_pDebugTimeFil->TextLine("");
					}
*/
				}
			GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
			}
		}
	return bOK;
}

bool Cst4DataClient::bRequestSources(const int nDest, DWORD dwT_utc_sec)
{
	bool bOK=false;
	if ((nDest<DestList.nGetCount())&&(nDest>=0))
		{
		DestInstance* pD=DestList[nDest];
		if (pD->nManageClientIndex>=0)
			{
			if (dwT_utc_sec==0)
				dwT_utc_sec=JTime(true,true).dwTotalSec();
			DWORD dwT_loc_sec=dwCst4ConvertToLocal(dwT_utc_sec);
			if (!bClientConnected(pD->nManageClientIndex))
				{
				if (dwT_loc_sec>=pD->dwNextRequest_loc_sec)
					{
					pD->dwNextRequest_loc_sec=dwT_loc_sec+10;
					ConnectClient(pD->nManageClientIndex);
					DWORD dwT_ms=::GetTickCount();
					while(!bClientConnected(pD->nManageClientIndex)&&(::GetTickCount()-dwT_ms<100)&&(!bAbortAll))
						{
						Application->ProcessMessages();
						}
					}
				}
			else
				pD->dwNextRequest_loc_sec=dwT_loc_sec+10;
			if (bClientConnected(pD->nManageClientIndex))
				{
				JTime T_utc=JTime(true,true);
				Cst4_Request Msg(T_utc,pD->wOwnSite,CST4_REQUEST_SOURCE_UPDATE);
				BYTE aucBuf[1024];
				int nTXSize=Msg.nCreateTXBuf(aucBuf);
				if (bSendData(pD->nManageClientIndex,aucBuf,nTXSize))
					++pD->nRequestsPending;
				}
			GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
			}
		}
	return bOK;
}

bool Cst4DataClient::bSendRXWatchdogMsg(const int nDest)
{
	bool bOK=false;
	if ((nDest<DestList.nGetCount())&&(nDest>=0))
		{
		DestInstance* pD=DestList[nDest];
		if (pD->nManageClientIndex>=0)
			{
			if (bClientConnected(pD->nManageClientIndex))
				{
				Cst4_Request Msg(JTime(true),pD->wOwnSite,CST4_REQUEST_RX_WATCHDOG);
				BYTE aucBuf[1024];
				int nTXSize=Msg.nCreateTXBuf(aucBuf);
            bOK=bSendData(pD->nManageClientIndex,aucBuf,nTXSize);
				}
			GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
			}
      }
   return bOK;
}

bool Cst4DataClient::bSendTXWatchdogMsg(const int nDest)
{
	bool bOK=false;
	if ((nDest<DestList.nGetCount())&&(nDest>=0))
		{
		DestInstance* pD=DestList[nDest];
		if (pD->nManageClientIndex>=0)
			{
			if (bClientConnected(pD->nManageClientIndex))
				{
				Cst4_Request Msg(JTime(true),pD->wOwnSite,CST4_REQUEST_TX_WATCHDOG);
				BYTE aucBuf[1024];
				int nTXSize=Msg.nCreateTXBuf(aucBuf);
            bOK=bSendData(pD->nManageClientIndex,aucBuf,nTXSize);
				}
			GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
			}
      }
   return bOK;
}

int Cst4DataClient::nServiceResponse()
{
	RequestList.Clear();
	DestList.GoFirst();
	int nDest=0;
	while(!DestList.bLast())
		{
		DestInstance* pD=DestList.pNext();
		if (pD->nRequestsPending>0)
			{
			if (pD->nManageClientIndex>=0)
				{
				BYTE aucRXBuf[1024];
				int nRXSize=nReadData(pD->nManageClientIndex,aucRXBuf,1024);
				if (nRXSize>0)
					{
					for (int i=0; i<nRXSize; i++)
						{
						if (pD->ResponseRX.AddRXByte(aucRXBuf[i])==CST4_RESPONSE_MSG)
							{
							String s;
							int nPort;
							Cst4_Response Response;
							Response=pD->ResponseRX.GetResponse();
							switch(Response.dwGetRequest())
								{
								case CST4_REQUEST_SERVER_PORT:
									nPort=*(int*)Response.pucGetInfo();
									if (nPort!=pD->nDestClientPort)
										{
										pD->nDestClientPort=nPort;
										if (pD->nDestClientIndex>=0)
											DisconnectClient(pD->nDestClientIndex);
										pD->nDestClientIndex=nAddClient(pOwner,pD->sIPAddr,pD->nDestClientPort);
										if (pD->nDestClientIndex>=0)
											{
											ConnectClient(pD->nDestClientIndex);
											DWORD dwT_ms=::GetTickCount();
											while(!bClientConnected(pD->nDestClientIndex)&&(::GetTickCount()-dwT_ms<100)&&(!bAbortAll))
												{
												Application->ProcessMessages();
												}
											}
										}
									pD->nTXErrorCnt=0;
									pD->nConnectedTXErrorCnt=-1;
									pD->bDestClientConnected=true;
									++nPortRXCnt;
									RequestList.nAdd(new RequestListStruct(nDest,CST4_REQUEST_SERVER_PORT,Response.wSource(),nPort));
									break;
								case CST4_REQUEST_TIME_SYNC:
									LastTimeSync_utc=Response.Time_utc();
									++nTimeRXCnt;
									RequestList.nAdd(new RequestListStruct(nDest,CST4_REQUEST_TIME_SYNC,Response.wSource()));
/*
									if (g_pDebugTimeFil)
										{
										String s;
										s.printf(L"TIME RECEIVED FROM SITE #%04X",Response.wSource());
										g_pDebugTimeFil->TextLine(s);
										g_pDebugTimeFil->TextLine("  Current PC Time: "+JTime(true,true).sDateTime(false,false,true));
										g_pDebugTimeFil->TextLine("  UTC - LOCAL = "+String(g_nCst4TimeBias_hours)+" hours");
										g_pDebugTimeFil->TextLine("  Received Time: "+LastTimeSync_utc.sDateTime(false,false,true));
										}
*/
									if (LastTimeSync_utc.bUpdatePCTime())
										{
/*
										if (g_pDebugTimeFil)
											g_pDebugTimeFil->TextLine("  New Current PC Time: "+JTime(true,true).sDateTime(false,false,true));
*/
										}
									else
										{
/*
										if (g_pDebugTimeFil)
											g_pDebugTimeFil->TextLine("  !!!!!! TIME UPDATE FAILED !!!!!!");
*/
										}
/*
									if (g_pDebugTimeFil)
										g_pDebugTimeFil->TextLine("");
*/
									if (Response.nGetInfoSize()>0)
										{
										Cst4Time T;
										int nOffs=0;
										T.Unpack(Response.pucGetInfo(),nOffs);
										if (nOffs>0)
											{
											LastTimeUpdate_utc=T.Get_utc();
											SetLastCst4TimeUpdate(LastTimeUpdate_utc);
											}
										else
											LastTimeUpdate_utc.Invalidate();
										}
									else
										LastTimeUpdate_utc.Invalidate();
									break;
								case CST4_REQUEST_SOURCE_UPDATE:
									break;
								case CST4_REQUEST_SITE_UPDATE:
									break;
								default: ;
								}
                     --pD->nRequestsPending;
                     }
                  }
					}
				GetStatus(pD->nManageClientIndex,pD->sManageStatus,pD->ManageStatusCol);
				}
         }
      ++nDest;
      }
   RequestList.Pack();
   return RequestList.nGetCount();
}

bool Cst4DataClient::bUpdateTimeRX()
{
	LastTimeMsg.Invalidate();
	BYTE aucRXBuf[1024];
	DestInstance* pD;
	DestList.GoFirst();
	while((!DestList.bLast()) && (!LastTimeMsg.bValid()))
		{
		pD=DestList.pNext();
		int nRXSize=nReadData(pD->nDestClientIndex,aucRXBuf,1024);
		if (nRXSize>0)
			{
			for (int i=0; i<nRXSize; i++)
				{
				if (pD->TimeRX.AddRXByte(aucRXBuf[i])==VISTA_TIME_MSG)
					LastTimeMsg=pD->TimeRX.GetTimeMsg();
				}
			}
		}
	return LastTimeMsg.bValid();
}




