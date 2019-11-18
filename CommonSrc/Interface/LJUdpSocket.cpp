//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "LJUdpSocket.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

LJUdpSocket::LJUdpSocket(TComponent* pOwner,const String _sIPAddress,const int _nPort)
{
	pUdpLock=new TCriticalSection;
	sRemoteIP=_sIPAddress;
	nRemotePort=_nPort;
	pUdpClient=new TIdUDPClient(pOwner);
	pUdpServer=new TIdUDPServer(pOwner);
	pUdpServer->OnUDPRead=IdUDPServerUDPRead;
	Reset(sRemoteIP,nRemotePort);
}

LJUdpSocket::~LJUdpSocket()
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
}

void LJUdpSocket::Reset(const String _sRemoteIP,const int _nRemotePort)
{
	sRemoteIP=_sRemoteIP;
	nRemotePort=_nRemotePort;
}


void LJUdpSocket::SendData(void* pData,const int nSize,const String _sRemoteIP,const int _nRemotePort,const bool bBroadcast)
{
	if ((!_sRemoteIP.IsEmpty()&&_nRemotePort>-1) || (bBroadcast && _nRemotePort>-1))
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
					pUdpClient->Broadcast(IdBuf,_nRemotePort);
				else
					pUdpClient->SendBuffer(_sRemoteIP,_nRemotePort,IdBuf);
			}
		}
		catch (...)
		{
		}
		pUdpLock->Release();
	}
}

void LJUdpSocket::SendData(void* pData,const int nSize,const String _sRemoteIP,const int _nRemotePort)
{
	SendData(pData,nSize,_sRemoteIP,_nRemotePort,false);
}

void LJUdpSocket::SendData(void* pData,const int nSize)
{
	SendData(pData,nSize,sRemoteIP,nRemotePort,false);
}

void LJUdpSocket::BroadcastData(void* pData,const int nSize,const int _nRemotePort)
{
	if (_nRemotePort>-1)
		SendData(pData,nSize,"",_nRemotePort,true);
	else
		SendData(pData,nSize,"",nRemotePort,true);
}


void __fastcall LJUdpSocket::IdUDPServerUDPRead(TIdUDPListenerThread */*AThread*/, TBytes AData,TIdSocketHandle *ABinding)
{
	if (AData.Length>0)
		RxByteFifo.nPush(AData);
	pUdpLock->Acquire();
	sSenderIP=ABinding->PeerIP;
	nSenderPort=ABinding->PeerPort;
	pUdpLock->Release();
}


int LJUdpSocket::nReadData(BYTE* pucData,const int nSize)
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



