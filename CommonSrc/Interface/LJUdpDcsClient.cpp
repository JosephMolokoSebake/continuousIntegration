//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "LJUdpDcsClient.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

LJUdpDcsClient::LJUdpDcsClient(TComponent* pOwner,const int _nCst4SiteID,const String _sDCSManagerIP)
{
	pUdpLock=new TCriticalSection;
	pUDPClient=new TIdUDPClient(pOwner);
	nCst4SiteID=_nCst4SiteID;
	sDCSManagerIP=_sDCSManagerIP;
	dwLastStatusTxTick=0;
	Init();
}

LJUdpDcsClient::~LJUdpDcsClient()
{
	if (pUDPClient)
	{
		SetAndTxStatus(DCS_STAT_CLOSE_EXE);
		pUDPClient->Active=false;
		delete pUDPClient;
	}
	delete pUdpLock;
	pUdpLock=NULL;
}

void LJUdpDcsClient::Init()
{
	if (pUDPClient)
	{
		pUDPClient->Port=0;
		pUDPClient->Active=true;
		SetAndTxStatus(DCS_STAT_IDLE);
	}
}



bool LJUdpDcsClient::bNewRequestReceived(DCS_STATUS_TYPE& NewDcsReq)
{
	bool bNewReq=false;
	try
	{
		pUdpLock->Acquire();
		if (pUDPClient->Active)
		{
			if (pUDPClient->Binding->Readable(50))
			{
				TIdBytes IdBuf;
				IdBuf.Length=pUDPClient->BufferSize;
				String sPeerIP="";
				WORD wPeerPort=0;
				TIdIPVersion IPVersion=ID_DEFAULT_IP_VERSION;
				int nTimeOut_MSec=IdTimeoutDefault;
				if (pUDPClient->ReceiveBuffer(IdBuf,sPeerIP,wPeerPort,IPVersion,nTimeOut_MSec) > 0)// (IdBuf,IdUDPClient->BufferSize)>0)
					bNewReq=bProcessRxData(IdBuf,NewDcsReq);
			}
		}
		pUdpLock->Release();
	}
	catch (...)
	{
		pUdpLock->Release();
	}
	if (!bNewReq)
	{
		DWORD dwNowTick=::GetTickCount();
		if (dwNowTick-dwLastStatusTxTick>5000)
		{
			TXStatus();
			dwLastStatusTxTick=dwNowTick;
		}
	}
	return bNewReq;
}


bool LJUdpDcsClient::bProcessRxData(TBytes IdByteData,DCS_STATUS_TYPE& NewDcsReq)
{
	bool bOK=false;
	for (int i=0;i<IdByteData.Length;i++)
	{
		CST4_MSG_TYPE MsgType=RX.AddRXByte(IdByteData[i]);
		if (MsgType==CST4_REQUEST_MSG)
		{
			switch (RX.GetRequest().dwGetRequest())
			{
				case DCS_STAT_IDLE:
				case DCS_STAT_CAPTURE:
				case DCS_STAT_CLOSE_EXE:
				{
					NewDcsReq=(DCS_STATUS_TYPE)RX.GetRequest().dwGetRequest();
					bOK=true;
					break;
				}
				default: NewDcsReq=DCS_STAT_IDLE;
			}
		}
	}
	return bOK;
}

void LJUdpDcsClient::TXStatus()
{
	pUdpLock->Acquire();
	if (pUDPClient)
	{
		if (pUDPClient->Active)
		{
			assert(true);
//			Cst4U_Response Msg;
//			Msg.Set(JTime(true),nCst4SiteID,Status,sStatus.c_str(),sStatus.Length());
//			BYTE aucBuf[1024];
//			int nTXSize=Msg.nCreateTXBuf(aucBuf);
//			TIdBytes IdBuf;
//			for (int i=0;i<nTXSize;i++)
//				AppendByte(IdBuf,aucBuf[i]);
//			if (nTXSize>0)
//				pUDPClient->SendBuffer(sDCSManagerIP,CST4_PORT_BASE+6+1,IdBuf);
		}
	}
   pUdpLock->Release();
}


