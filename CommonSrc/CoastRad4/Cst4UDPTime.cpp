//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4UDPTime.h"
#include "JCommaText.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4UDPTime::Cst4UDPTime(TComponent* pOwner, const WORD _wOwnID, const bool _bAutoTimeUpdate, const bool bEnableRX)
{
	pUdpLock=new TCriticalSection;
	wOwnID=_wOwnID;
	bAutoTimeUpdate=_bAutoTimeUpdate;
	pUdpClient=new TIdUDPClient(pOwner);

	if (bEnableRX)
		{
		pUdpServer=new TIdUDPServer(pOwner);
		pUdpServer->OnUDPRead=IdUDPServerUDPRead;
		}
	else
		pUdpServer=NULL;

	pLogFil=NULL;

	Reset();

	if (pUdpServer)
		{
		pUdpServer->DefaultPort=CST4_UDP_TIME_PORT;
		try
			{
			pUdpServer->Active=true;
			}
		catch(...)
			{
			pUdpServer->Active=false;
			delete pUdpServer;
			pUdpServer=NULL;
			ShowError("UDP Port Conflict:  Cannot Run Multiple Time Server Applications on a Single PC","Time Synchronisation Error");
			}
		}
}

Cst4UDPTime::~Cst4UDPTime()
{
	if (pUdpClient)
		{
		pUdpClient->Active=false;
		delete pUdpClient;
		}
	if (pUdpServer)
		{
		pUdpServer->Active=false;
		delete pUdpServer;
		}
	delete pUdpLock;
	pUdpLock=NULL;
	DeleteLogFil();
}

void Cst4UDPTime::Reset()
{
	LastRXTime_utc.Invalidate();
	LastTXTime_utc.Invalidate();
	nRequestCnt=0;
	nRXTimeCnt=0;
	nTXTimeCnt=0;
	LastGPSUpdateTime_utc.Invalidate();
	dwLastGPSUpdateTime_utc_sec=0;
	Cst4Manager::Init(false); //Do not save messages
}

void Cst4UDPTime::CreateLogFil(const String sLogFileName)
{
	DeleteLogFil();
	pLogFil=new JFile('O',JFile::ASCII_TYPE);
	pLogFil->Create(sLogFileName);
	pLogFil->TextLine(JTime(true,true).sDateTime(false,false,true)+" - Log File Created");
}

void Cst4UDPTime::DeleteLogFil()
{
	if (pLogFil)
		{
		pLogFil->TextLine(JTime(true,true).sDateTime(false,false,true)+" - Log File Closed");
		delete pLogFil;
		pLogFil=NULL;
		}
}

DWORD Cst4UDPTime::dwConvertAddr(const String sAddr)
{
	CommaText Txt(sAddr,true,'.');
	if (Txt.nGetCount()==4)
		{
		DWORD adw[4];
		try
			{
			adw[3]=Txt.nGetVal(0);
			adw[2]=Txt.nGetVal(1);
			adw[1]=Txt.nGetVal(2);
			adw[0]=Txt.nGetVal(3);
			return (adw[3]<<24)|(adw[2]<<16)|(adw[1]<<8)|adw[0];
			}
		catch(...)
			{
			return 0;
			}
		}
	return 0;	//Invalid
}

String Cst4UDPTime::sConvertAddr(const DWORD dwAddr)
{
	String s;
	s.printf(L"%d.%d.%d.%d",(dwAddr>>24)&0xFF,(dwAddr>>16)&0xFF,(dwAddr>>8)&0xFF,dwAddr&0xFF);
	return s;
}

void Cst4UDPTime::SendData(void* pData,const int nSize,const String sRemoteIP, const bool bBroadcast)
{
	if ((!sRemoteIP.IsEmpty()) || (bBroadcast))
	{
		pUdpLock->Acquire();
		try
		{
			if (nSize>0 && pData != NULL)
			{
				TIdBytes IdBuf;
				BYTE* pucData=(BYTE*)pData;
				for (int i=0;i<nSize;i++)
					AppendByte(IdBuf,pucData[i]);
				if (bBroadcast)
					pUdpClient->Broadcast(IdBuf,CST4_UDP_TIME_PORT);
				else
					pUdpClient->SendBuffer(sRemoteIP,CST4_UDP_TIME_PORT,IdBuf);
			}
		}
		catch (...)
		{
		}
		pUdpLock->Release();
	}
}

void Cst4UDPTime::SendData(void* pData,const int nSize,const String sRemoteIP)
{
	SendData(pData,nSize,sRemoteIP,false);
}

void Cst4UDPTime::BroadcastData(void* pData,const int nSize)
{
	SendData(pData,nSize,"",true);
}


void __fastcall Cst4UDPTime::IdUDPServerUDPRead(TIdUDPListenerThread */*AThread*/, TBytes AData,TIdSocketHandle *ABinding)
{
	if (AData.Length>0)
		RxByteFifo.nPush(AData);
	pUdpLock->Acquire();
	sSenderIP=ABinding->PeerIP;
	pUdpLock->Release();
	int nRXCnt=nReadData(aucRXBuf,RXBUF_SIZE);
	for (int i=0; i<nRXCnt; i++)
		{
		CST4_MSG_TYPE MsgType=Cst4Manager::AddRXByte(aucRXBuf[i]);
		switch(MsgType)
			{
			case CST4_REQUEST_MSG:
				Req=Cst4Manager::GetRequest();
				if (Req.dwGetRequest()==CST4_REQUEST_TIME_SYNC)
					{
					if (pLogFil)
						pLogFil->TextLine(Req.Time_utc().sDateTime(false,false,true)+" - Time Request Received From "+sSenderIP);
					SendCurrentTime(sSenderIP);
					}
				Cst4Manager::ClearMsg();
				break;
			case CST4_RESPONSE_MSG:
				Resp=Cst4Manager::GetResponse();
				if (Resp.dwGetRequest()==CST4_REQUEST_TIME_SYNC)
					{
					pUdpLock->Acquire();
					LastRXTime_utc=Resp.Time_utc();
					if (LastRXTime_utc.bValid() && bAutoTimeUpdate)
						{
						if (Resp.nGetInfoSize()==(int)sizeof(JTime))
							{
							Resp.nGetInfo((BYTE*)&LastGPSUpdateTime_utc,sizeof(LastGPSUpdateTime_utc));
							if (LastGPSUpdateTime_utc.bValid())
								dwLastGPSUpdateTime_utc_sec=LastGPSUpdateTime_utc.dwTotalSec();
							else
								dwLastGPSUpdateTime_utc_sec=0;
							}
						LastRXTime_utc.bUpdatePCTime();
						SetLastCst4TimeUpdate(LastRXTime_utc);
						++nRXTimeCnt;
						if (pLogFil)
							{
							pLogFil->TextLine(LastRXTime_utc.sDateTime(false,false,true)+" - Time Received From "+sSenderIP);
							if (LastGPSUpdateTime_utc.bValid())
								pLogFil->TextLine("   Last GPS Update: "+LastGPSUpdateTime_utc.sDateTime(false,false,true));
							else
								pLogFil->TextLine("   No Last GPS Update Available");
							}
						}
					pUdpLock->Release();
					}
				Cst4Manager::ClearMsg();
				break;
			default:	;
			}
		}
}

int Cst4UDPTime::nReadData(BYTE* pucData,const int nSize)
{
	int nBytesRead=0;
	if (RxByteFifo.nGetSize()>0)
	{
		BYTE uc;
		while (nBytesRead <= nSize  && RxByteFifo.nPop(&uc) >= 0 )
			pucData[nBytesRead++]=uc;
	}
	return nBytesRead;
}

void Cst4UDPTime::SendGPSTime(const String sIPAddr, const JTime& GPSTime_utc)
{
	Cst4_Response Msg(GPSTime_utc,wOwnID,CST4_REQUEST_TIME_SYNC);
	LastGPSUpdateTime_utc=GPSTime_utc;
	LastGPSUpdateTime_utc.bUpdatePCTime();	//Update PC Time
	dwLastGPSUpdateTime_utc_sec=LastGPSUpdateTime_utc.dwTotalSec();
	Msg.SetInfo((BYTE*)&LastGPSUpdateTime_utc,sizeof(LastGPSUpdateTime_utc));
	BYTE aucBuf[1024];
	int nSize=Msg.nCreateTXBuf(aucBuf);
	SendData(aucBuf,nSize,sIPAddr);
	if (pLogFil)
		pLogFil->TextLine(GPSTime_utc.sDateTime(false,false,true)+" - GPS Time Sent To "+sIPAddr);
	pUdpLock->Acquire();
	LastTXTime_utc=GPSTime_utc;
	sDestAddr=sIPAddr;
	++nTXTimeCnt;
	pUdpLock->Release();
}

void Cst4UDPTime::SendTime(const String sIPAddr, const JTime& T_utc)
{
	Cst4_Response Msg(T_utc,wOwnID,CST4_REQUEST_TIME_SYNC);
	Msg.SetInfo((BYTE*)&LastGPSUpdateTime_utc,sizeof(LastGPSUpdateTime_utc));
	BYTE aucBuf[1024];
	int nSize=Msg.nCreateTXBuf(aucBuf);
	SendData(aucBuf,nSize,sIPAddr);
	if (pLogFil)
		{
		pLogFil->TextLine(T_utc.sDateTime(false,false,true)+" - Time Sent To "+sIPAddr);
		if (LastGPSUpdateTime_utc.bValid())
			pLogFil->TextLine("   Last GPS Update: "+LastGPSUpdateTime_utc.sDateTime(false,false,true));
		else
			pLogFil->TextLine("   No Last GPS Update Available");
		}
	pUdpLock->Acquire();
	LastTXTime_utc=T_utc;
	sDestAddr=sIPAddr;
	++nTXTimeCnt;
	pUdpLock->Release();
}


void Cst4UDPTime::SendCurrentTime(const String sIPAddr)
{
	SendTime(sIPAddr,JTime(true,true));
}

void Cst4UDPTime::BroadcastTime(const JTime& T_utc)
{
	Cst4_Response Msg(T_utc,wOwnID,CST4_REQUEST_TIME_SYNC);
	Msg.SetInfo((BYTE*)&LastGPSUpdateTime_utc,sizeof(LastGPSUpdateTime_utc));
	BYTE aucBuf[1024];
	int nSize=Msg.nCreateTXBuf(aucBuf);
	BroadcastData(aucBuf,nSize);
	if (pLogFil)
		{
		pLogFil->TextLine(T_utc.sDateTime(false,false,true)+" - Time Broadcast To All");
		if (LastGPSUpdateTime_utc.bValid())
			pLogFil->TextLine("   Last GPS Update: "+LastGPSUpdateTime_utc.sDateTime(false,false,true));
		else
			pLogFil->TextLine("   No Last GPS Update Available");
		}
	pUdpLock->Acquire();
	LastTXTime_utc=T_utc;
	++nTXTimeCnt;
	pUdpLock->Release();
}


void Cst4UDPTime::BroadcastCurrentTime()
{
	BroadcastTime(JTime(true,true));
}

void Cst4UDPTime::RequestTime(const String sIPAddr)
{
	JTime T_utc(true,true);
	Cst4_Request Msg(T_utc,wOwnID,CST4_REQUEST_TIME_SYNC);
	BYTE aucBuf[1024];
	int nTXSize=Msg.nCreateTXBuf(aucBuf);
	SendData(aucBuf,nTXSize,sIPAddr);
	if (pLogFil)
		pLogFil->TextLine(T_utc.sDateTime(false,false,true)+" - Time Request Sent To "+sIPAddr);
	++nRequestCnt;
}


