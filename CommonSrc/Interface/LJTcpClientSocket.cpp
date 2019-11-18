/****************************** CONFIGURATION HEADER ****************************************
 *
 *   MODULE TITLE       TCP/IP Server Sockets Container
 *
 *   FILE NAME          LJTcpServerSocket.cpp
 *   COMPILER           Borland C++ Builder 6
 *   PROCESSOR          Pentium IV
 *   OS                 MS Windows XP SP3
 *   PROGRAMMER         Jacques van Wyk
 *
 *  ABSTRACT
 *      V1.00          LJVW 10/01/2008
 *                              - Initial Version
 *      V1.01          LJVW 07/08/2008
 *                              - Fixed stopping of Watchdog Timer in StopTimer()
 *      V1.02          LJVW 11/08/2008
 *                              - Enabled polling of change of socket status
 *      V1.03          LJVW 13/05/2009
 *				- Added Error Checking on Data TX in bSendData()
 *      V1.04          LJVW 29/07/2009
 *				- Added bool bClientConnecting to prevent Open() attempts by one socket
 *      V1.05          LJVW 11/08/2009
 *				- Added Socket.Close() to close socket in OnClientSocketError()
 *				- Added support for saving info to debug logfile
 *      V1.06          LJVW 13/08/2009
 *				- Changed receive buffer size to 1MB
 *      V1.07          LJVW 20/11/2009
 *				- Added extra debug info
 *				- Changed bSendData() to only TX once, even if Winsock buffer full
 *      V1.08          LJVW 23/11/2009
 *				- Added base class to encapsulate calls common to LJTcpServerSocket
 *      V1.09          LJVW 27/01/2010
 *				- Added GetStatus() method to retrieve socket status and corresponding TEdit colour
 *				- Fixed reconnection logic after socket error encountered
 *      V1.10          LJVW 29/01/2010
 *				- Bug Fixes
 *      V1.11          LJVW 11/08/2010
 *				- Converted to UniCode
 *      V1.12          LJVW 29/07/2011
 *				- Recompiled to work with update LJTcpSocketDefs.cpp
 *
/****************************** CONFIGURATION HEADER *****************************************/

//---------------------------------------------------------------------------

#include "jpch.h"

#pragma hdrstop

#include "LJTcpClientSocket.h"
#include "JTime.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

LJSingleClientSocket::LJSingleClientSocket(TComponent* _pOwnerForm,const String _sIPAddress,const int _nPort,const bool _bEnableDebugging,const int _nWinSockBufSize,const int _nMaxRxBuffSize)
			: LJSingleSocketBaseClass(_bEnableDebugging,TCP_CLIENT_VERSION,false,_sIPAddress,_nPort,_nMaxRxBuffSize)
{
	pOwnerForm=_pOwnerForm;
   nPort=_nPort;
   sIPAddress=_sIPAddress;
   bEnableDebugging=_bEnableDebugging;
   nWinSockBufSize=_nWinSockBufSize;
   pClientSocket=NULL;
   bConnectClient=false;
   bClientConnecting=false;
   bInDoConnect=false;
   dwLastDisconnect=0;
}

void LJSingleClientSocket::DestroyClientSocket()
{
   if (pClientSocket)
   {
      try
      {
         WriteToClientDebugLog("DestroyClientSocket()");
         pClientSocket->Socket->Lock();
         pClientSocket->Close();
         pClientSocket->Socket->Unlock();
      }
      catch(...)
      {
      }
      delete pClientSocket;
      pClientSocket=NULL;
   }
   UpdateStatus(SOCKET_STATUS_OFF);
   bClientConnecting=false;
	dwLastDisconnect=0;
	dwStartConnectAttemp=0;
	bPrevTXFailed=false;
}

LJSingleClientSocket::~LJSingleClientSocket()
{
   WriteToClientDebugLog("~LJSingleClientSocket()");
   DestroyClientSocket();
   ClearRxFifo();
}



void LJSingleClientSocket::ConnectClientSocket()
{
	bConnectClient=true;
   ResetClientSocket(true);
}


void LJSingleClientSocket::DisconnectClientSocket()
{
   bConnectClient=false;
   WriteToClientDebugLog("DisconnectClientSocket()");
   DestroyClientSocket();
   ClearRxFifo();
}


void LJSingleClientSocket::ResetClientSocket(bool bResetErrorCount)
{
	if (!sIPAddress.IsEmpty()&&nPort>-1)
   {
      WriteToClientDebugLog("ResetClientSocket()");
      DestroyClientSocket();
      pClientSocket=new TClientSocket(pOwnerForm);
      pClientSocket->Port=nPort;
      pClientSocket->Address=sIPAddress;
      pClientSocket->OnLookup=ClientSocketLookup;
      pClientSocket->OnConnecting=ClientSocketConnecting;
      pClientSocket->OnConnect=ClientSocketConnect;
      pClientSocket->OnDisconnect=ClientSocketDisconnect;
      pClientSocket->OnRead=ClientSocketRead;
      pClientSocket->OnWrite=ClientSocketWrite;
      pClientSocket->OnError=ClientSocketError;
      pClientSocket->ClientType=ctNonBlocking;
      if (bResetErrorCount)
         nSocketErrorCount=0;
      if (bConnectClient)
         DoConnect();
   }
}

bool LJSingleClientSocket::bSetWinSockBufSize(const int _nSize)
{
   bool bOK=false;
   int nSize;
   if (_nSize<=-1)
      nSize=8192; // Windows Default
   else
      nSize=_nSize;
   if (pClientSocket->Socket->Connected&&nSize>=0)
   {
		if (::setsockopt((SOCKET)pClientSocket->Socket->SocketHandle,SOL_SOCKET,SO_SNDBUF,(char*)&nSize,4)==0);
      {
         bOK=true;
         WriteToClientDebugLog("Winsock Buffer Size [bytes] = "+String(nSize));
      }
   }
   return bOK;
}

void LJSingleClientSocket::DoConnect()
{
	if (!bInDoConnect)
	{
		bInDoConnect=true;
		bool bResetSocket=false;
		try
		{
			if (pClientSocket)
			{
				pClientSocket->Socket->Lock();
				if (!pClientSocket->Active)//->Socket->Connected)
				{
					if (!bClientConnecting) // if not already called Open() method
					{
						WriteToClientDebugLog("DoConnect()");
						pClientSocket->Open();
						dwStartConnectAttemp=::GetTickCount();
					}
				}
/*
				if (!pClientSocket->Socket->Connected)
				{
					if (::abs(::GetTickCount()-dwStartConnectAttemp)>30000) //10*60*1000) // socket active, trying to connect but NOT connected
					{
						if (bConnectClient)
						{
							bResetSocket=true;
							WriteToClientDebugLog("DoConnect() Timeout [10 minutes]");
							ShowMessage("timeout");
						}
					}
				}*/
				pClientSocket->Socket->Unlock();
			}
			else
				bResetSocket=true;
		}
		catch (...)
		{
			pClientSocket->Socket->Unlock();
			WriteToClientDebugLog("DoConnect(Exception Caught)");
			bResetSocket=true;
		}

		if (bResetSocket)
			DestroyClientSocket();
		bInDoConnect=false;
   }
}

void LJSingleClientSocket::CheckSocketConnected()
{
   if (bConnectClient)
   {
      if (pClientSocket==NULL)
         ResetClientSocket();
      else
         DoConnect();
   }
}

bool LJSingleClientSocket::bSendData(BYTE* pucData,const int nSize)
{
   bool bOK=false;
   try
   {
      if (pClientSocket)
      {
         if (pClientSocket->Socket->Connected)
         {
            pClientSocket->Socket->Lock();
            int nBytesQueued=pClientSocket->Socket->SendBuf(pucData,nSize);
            if (nBytesQueued==-1) // WinSock buffer full
            {
					WriteToClientDebugLog("bSendData() WinSock Buffer Full");
					nSocketErrorCount++;
					bPrevTXFailed=true;
				}
				else // TX Succeeded
				{
					bOK=true;
					if (bPrevTXFailed)
					{
						bPrevTXFailed=false;
						WriteToClientDebugLog("bSendData() WinSock Buffer TX Restored");
					}
            }
            pClientSocket->Socket->Unlock();
         }
      }
   }
   catch(...)
   {
      bOK=false;
      if (pClientSocket)
         pClientSocket->Socket->Unlock();
      WriteToClientDebugLog("bSendData(Exception Caught) TX Failed");
      DestroyClientSocket();
      nSocketErrorCount++;
   }
   return bOK;
}



void __fastcall LJSingleClientSocket::ClientSocketConnect(TObject */*Sender*/,TCustomWinSocket *Socket)
{
	pClientSocket->Socket->Lock();
	UpdateStatus(SOCKET_STATUS_CONNECTED);
	bClientConnecting=false;
	WriteToClientDebugLog("SocketConnected()");
	bSetWinSockBufSize(nWinSockBufSize);
	sLocalIP=Socket->LocalAddress;
	pClientSocket->Socket->Unlock();
}

void __fastcall LJSingleClientSocket::ClientSocketError(TObject */*Sender*/,TCustomWinSocket *Socket,TErrorEvent ErrorEvent,int &ErrorCode)
{
   pClientSocket->Socket->Lock();
   UpdateStatus(SOCKET_STATUS_ERROR);
   WriteToClientDebugLog("SocketError() "+sGetError(ErrorEvent,ErrorCode));
   nSocketErrorCount++;
   ErrorCode=0;
   try
   {
      Socket->Close();
   }
   catch (...)
   {
   }
   bClientConnecting=false;
   pClientSocket->Socket->Unlock();
}

void __fastcall LJSingleClientSocket::ClientSocketDisconnect(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
   pClientSocket->Socket->Lock();
   UpdateStatus(SOCKET_STATUS_DISCONNECTED);
   DWORD dwNow=::GetTickCount();
   if (dwNow-dwLastDisconnect>500) // to ensure multiple disconnect events on single disconnect action do not cause multiple DoConnect() attempts
   {
      dwLastDisconnect=dwNow;
      bClientConnecting=false; // triggers DoConnect() on next timer event
      WriteToClientDebugLog("SocketDisconnected()");
   }
   else
      WriteToClientDebugLog("SocketDisconnected(Skipped Multiple Disconnect)");
   pClientSocket->Socket->Unlock();
}

void __fastcall LJSingleClientSocket::ClientSocketConnecting(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
   pClientSocket->Socket->Lock();
	UpdateStatus(SOCKET_STATUS_CONNECTING);
	bClientConnecting=true;
//	dwStartConnectAttemp=::GetTickCount();
	WriteToClientDebugLog("SocketConnecting()");
	pClientSocket->Socket->Unlock();
}

void __fastcall LJSingleClientSocket::ClientSocketLookup(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
	pClientSocket->Socket->Lock();
	UpdateStatus(SOCKET_STATUS_LOOKUP);
	bClientConnecting=true;
//	dwStartConnectAttemp=::GetTickCount();
	pClientSocket->Socket->Unlock();
}

void __fastcall LJSingleClientSocket::ClientSocketRead(TObject */*Sender*/,TCustomWinSocket *Socket)
{
	ReadSocketData(Socket);
}

void __fastcall LJSingleClientSocket::ClientSocketWrite(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
}


String LJSingleClientSocket::sGetLocalIPAddr()
{
	String s="";
	pClientSocket->Socket->Lock();
	s=sLocalIP;
	pClientSocket->Socket->Unlock();
   return s;
}


/* *************************************************************************** */



LJTcpClientSockets::LJTcpClientSockets()
{
   bEnableDebugging=false;
   nCheckConnect=1;  // check for TCP errors every 1 seconds
   nWinSockBufSize=8192; // Windows default
   Init();
}
LJTcpClientSockets::LJTcpClientSockets(const int _nCheckConnectSec,const bool _bEnableDebugging,const int _nWinSockBufSize)
{
   bEnableDebugging=_bEnableDebugging;
   nCheckConnect=_nCheckConnectSec;
   nWinSockBufSize=_nWinSockBufSize;
   Init();
}

LJTcpClientSockets::~LJTcpClientSockets()
{
//   StopTimer();
   Clear();
   delete pWatchDogTimer;
   delete pClientSocketsLock;
}

void LJTcpClientSockets::Init()
{
   pClientSocketsLock=new TCriticalSection;
   pWatchDogTimer=NULL;
   bStopTimer=true;
   bInTimer=false;
   ClientSockets.Clear();
}


int LJTcpClientSockets::nAddClient(TComponent* _pOwnerForm,const String _sIPAddress,const int _nPort,const int _nMaxRxBuffSize)
{
	int nCount;
	pOwnerForm=_pOwnerForm;
	LJSingleClientSocket* pSocket=new LJSingleClientSocket(pOwnerForm,_sIPAddress,_nPort,bEnableDebugging,nWinSockBufSize,_nMaxRxBuffSize);
	pClientSocketsLock->Acquire();
	nCount=ClientSockets.nAdd(pSocket);
	pClientSocketsLock->Release();
	return nCount;
}

bool LJTcpClientSockets::bClientConnected(const int nClientIdx)
{
	bool bConn=false;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		bConn=ClientSockets[nClientIdx]->bClientConnected();
	}
	pClientSocketsLock->Release();
	return bConn;
}

void LJTcpClientSockets::ConnectClient(const int nClientIdx)
{
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		ClientSockets[nClientIdx]->ConnectClientSocket();
		if (pWatchDogTimer==NULL)
			StartTimer();
	}
	pClientSocketsLock->Release();
}

void LJTcpClientSockets::DisconnectClient(const int nClientIdx)
{
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		ClientSockets[nClientIdx]->DisconnectClientSocket();
	}
	pClientSocketsLock->Release();
}

void LJTcpClientSockets::DisconnectClientSockets()
{
	pClientSocketsLock->Acquire();
	if (!ClientSockets.bPacked())
		ClientSockets.Pack();
	for (int i=0;i<ClientSockets.nGetCount();i++)
		ClientSockets[i]->DisconnectClientSocket();
	pClientSocketsLock->Release();
}


void LJTcpClientSockets::CheckConnect()
{
	pClientSocketsLock->Acquire();
	ClientSockets.GoFirst();
	nTotalErrorCount=0;
	while (!ClientSockets.bLast())
	{
		LJSingleClientSocket* pSocket=ClientSockets.pNext();
		if (pSocket)
		{
			pSocket->CheckSocketConnected();
			nTotalErrorCount+=pSocket->nGetSocketErrorCount();
		}
	}
	pClientSocketsLock->Release();
}

void LJTcpClientSockets::StopTimer()
{
	bStopTimer=true;
	if (pWatchDogTimer)
	{
		DWORD dwBeginStop=::GetTickCount();
		while (pWatchDogTimer->Enabled&&::GetTickCount()-dwBeginStop<60000) // wait for timer event to complete, but not longer than 60 sec
			Application->ProcessMessages();
		pWatchDogTimer->Enabled=false; // just to be sure
		delete pWatchDogTimer;
		pWatchDogTimer=NULL;
	}
	bInTimer=false;
}

void LJTcpClientSockets::StartTimer()
{
	if (pWatchDogTimer)
		StopTimer();
	pWatchDogTimer=new TTimer(pOwnerForm);
	pWatchDogTimer->Enabled=false;
	pWatchDogTimer->Interval=300;//nCheckConnect*1000;
	pWatchDogTimer->OnTimer=WatchDogTimerTimer;
	bStopTimer=false;
	dwPrevWatchDogTick=0;
	pWatchDogTimer->Enabled=true;
}


bool LJTcpClientSockets::bSendData(const int nClientIdx,BYTE* pucData,const int nSize)
{
	bool bOK=false;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		bOK=ClientSockets[nClientIdx]->bSendData(pucData,nSize);
	}
	pClientSocketsLock->Release();
	return bOK;
}


int LJTcpClientSockets::nReadData(const int nClientIdx,BYTE* pucData,const int nMaxSize)
{
	int nBytesRead=0;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		nBytesRead=ClientSockets[nClientIdx]->nReadData(pucData,nMaxSize);
	}
	pClientSocketsLock->Release();
	return nBytesRead;
}

bool LJTcpClientSockets::bReadByte(const int nClientIdx,BYTE& uc)
{
	return (nReadData(nClientIdx,&uc,1)==1);
}



void __fastcall LJTcpClientSockets::WatchDogTimerTimer(TObject */*Sender*/)
{
	if (!bInTimer)
	{
		bInTimer=true;
		if (bStopTimer)
			pWatchDogTimer->Enabled=false;
		else
		{
			DWORD dwNowTick=::GetTickCount();
			if (::abs((int)(dwNowTick-dwPrevWatchDogTick))>nCheckConnect*1000)
			{
				dwPrevWatchDogTick=dwNowTick;
				CheckConnect();
			}
		}
		bInTimer=false;
	}
}

void LJTcpClientSockets::Clear()
{
	StopTimer();
	pClientSocketsLock->Acquire();
	ClientSockets.Clear();
	pClientSocketsLock->Release();
}

int LJTcpClientSockets::nGetStatusChanged()
{
	int nFirstSocketStatusChanged=-1;
	pClientSocketsLock->Acquire();
	if (ClientSockets.nGetCount()>0)
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		for (int i=0;i<ClientSockets.nGetCount()&&nFirstSocketStatusChanged<0;i++)
			if (ClientSockets[i]->bGetStatusChanged())
				nFirstSocketStatusChanged=i;
	}
	pClientSocketsLock->Release();
	return nFirstSocketStatusChanged;
}

bool LJTcpClientSockets::bGetStatusChanged(const int nClientIdx)
{
	bool bStatChanged=false;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		bStatChanged=ClientSockets[nClientIdx]->bGetStatusChanged();
	}
	pClientSocketsLock->Release();
	return bStatChanged;
}


String LJTcpClientSockets::sGetStatus(const int nClientIdx)
{
   bStatusOff=true;
	String sStatus="Off";
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
		{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		sStatus=ClientSockets[nClientIdx]->sGetStatus("");
	   bStatusOff=false;
		}
	pClientSocketsLock->Release();
   if (bStatusOff)
      {
      bStatusOff=true;
      }
	return sStatus;
}

SOCKET_STATUS LJTcpClientSockets::GetStatus(const int nClientIdx)
{
	SOCKET_STATUS Stat=SOCKET_STATUS_OFF;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		Stat=ClientSockets[nClientIdx]->GetStatus();
	}
	pClientSocketsLock->Release();
	return Stat;
}

void LJTcpClientSockets::GetStatus(const int nClientIdx,String& sStatus,TColor& StatusEditColour)
{
	sStatus="";
	StatusEditColour=clBlack;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		ClientSockets[nClientIdx]->GetStatus(sStatus,StatusEditColour,"");
	}
	pClientSocketsLock->Release();
}


int LJTcpClientSockets::nGetSocketErrorCount(const int nClientIdx)
{
	int nErrors=-1;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		nErrors=ClientSockets[nClientIdx]->nGetSocketErrorCount();
	}
	pClientSocketsLock->Release();
	return nErrors;
}

int LJTcpClientSockets::nGetTotalSocketErrorCount()
{
	int nErrors=-1;
	pClientSocketsLock->Acquire();
	if (ClientSockets.nGetCount()>=0)
		nErrors=nTotalErrorCount;
	pClientSocketsLock->Release();
	return nErrors;
}

bool LJTcpClientSockets::bSetWinSockBufSize(const int nClientIdx,const int nSize)
{
	bool bOK=false;
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		bOK=ClientSockets[nClientIdx]->bSetWinSockBufSize(nSize);
	}
	pClientSocketsLock->Release();
	return bOK;

}


String LJTcpClientSockets::sGetLocalIPAddr(int nClientIdx)
{
	String s="";
	pClientSocketsLock->Acquire();
	if (nClientIdx<ClientSockets.nGetCount())
	{
		if (!ClientSockets.bPacked())
			ClientSockets.Pack();
		s=ClientSockets[nClientIdx]->sGetLocalIPAddr();
	}
	pClientSocketsLock->Release();
	return s;
}

