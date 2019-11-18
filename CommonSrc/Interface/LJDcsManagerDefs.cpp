//---------------------------------------------------------------------------
#include "jpch.h"


#pragma hdrstop

#include "LJDcsManagerDefs.h"
#include "Cst4Manager.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)


void DCSStatusClass::SetCurrentStatus(DCS_STATUS_TYPE Stat,Cst4Manager* pRX)
{
	String sStat="";
	CurrStat=Stat;
	switch (CurrStat)
	{
		case DCS_STAT_IDLE:
		{
			StateUp=DCS_STAT_CAPTURE;
			StateDown=DCS_STAT_CLOSE_EXE;
			sStat="Idle";
			bPing=false;
			break;
		}
		case DCS_STAT_CAPTURE:
		{
			StateUp=DCS_STAT_UNDEFINED;
			StateDown=DCS_STAT_IDLE;
			sStat="Capture Started";
			bPing=false;
			break;
		}
		case DCS_STAT_CLOSE_EXE:
		{
			StateUp=DCS_STAT_UNDEFINED;
			StateDown=DCS_STAT_SHUTDOWN;
			sStat="Program Closed";
			bPing=true;
			dwLastPingResponse=::GetTickCount();
			break;
		}
		case DCS_STAT_SHUTDOWN:
		{
			StateUp=DCS_STAT_UNDEFINED;
			StateDown=DCS_STAT_UNDEFINED;
			sStat="Shutting Down";
			break;
		}
		case DCS_STAT_OFF:
		{
			StateUp=DCS_STAT_UNDEFINED;
			StateDown=DCS_STAT_UNDEFINED;
			sStat="System Off";
			bPing=false;
			break;
		}
		case DCS_STAT_ERROR:
		{
			StateUp=DCS_STAT_ERROR;
			StateDown=DCS_STAT_ERROR;
			sStat="Error";
			bPing=false;
			break;
		}
		default: sStat="Undefined";
	}
	if (pRX)
	{
		if (pRX->GetResponse().nGetInfoSize() > 0)
		{
			int nInfoSize=pRX->GetResponse().nGetInfoSize();
			BYTE* puc=new BYTE[nInfoSize];
			if (pRX->GetResponse().nGetInfo(puc,nInfoSize) > 0)
			{
				sStat=String(AnsiString((char*)puc,nInfoSize));
			}
			delete[] puc;
		}
	}
	sStatus=sStat;
}

String DCSStatusClass::sGetNextState(const DCS_STATUS_TYPE Stat) const
{
	String s="";
	switch (Stat)
	{
		case DCS_STAT_UNDEFINED:		s="";						break;
		case DCS_STAT_IDLE:				s="Stop Capture";		break;
		case DCS_STAT_CAPTURE: 			s="Start Capture";	break;
		case DCS_STAT_CLOSE_EXE:		s="Close Program";  	break;
		case DCS_STAT_SHUTDOWN:			s="Shutdown";  		break;
		case DCS_STAT_ERROR:				s="Error"; 				break;
		default: ;
	}
	return s;
}

void DCSStatusClass::TXRequest(TIdUDPServer* pServer,const DCS_STATUS_TYPE Req)
{
	if (pServer)
	{
		if (pServer->Active)
		{
			Cst4_Request Msg;
			Msg.Set(JTime(true),nCst4SiteID,Req);
			BYTE aucBuf[1024];
			int nTXSize=Msg.nCreateTXBuf(aucBuf);
			TIdBytes IdBuf;
			for (int i=0;i<nTXSize;i++)
				AppendByte(IdBuf,aucBuf[i]);
			if (nTXSize>0)
			{
				try
				{
					pServer->SendBuffer(sUDPIP,nUDPPort,IdBuf);
				}
				catch (...)
				{
				}
			}
		}
	}

}


void DCSStatusClass::ShutdownSystem()
{
	if (pOwner != NULL)
	{
		int nClient=TcpShutDownClients.nAddClient(pOwner,sUDPIP,CST4_PROCWATCH_PORT);
		if (nClient >= 0)
		{
			TcpShutDownClients.ConnectClient(nClient);
			Cst4_Request Req(JTime(true),nCst4SiteID,CST4_REQUEST_SHUTDOWN);
			BYTE aucBuf[1024];
			int nBufSize=Req.nCreateTXBuf(aucBuf);
			int n=0;
			bool bOK=false;
			do
			{
				Application->ProcessMessages();
				if (TcpShutDownClients.bClientConnected(nClient))
					bOK=TcpShutDownClients.bSendData(nClient,aucBuf,nBufSize);
			}
			while((++n<10) && (!bOK));
			TcpShutDownClients.DisconnectClient(nClient);
		}
	}
}

