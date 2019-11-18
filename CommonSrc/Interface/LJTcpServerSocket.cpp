/********************** CONFIGURATION HEADER ****************************************
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
 *				- Added bool bServerListening to prevent Open() attempts by one socket
 *      V1.05          LJVW 13/08/2009
 *				- Changed receive buffer size to 1MB
 *      V1.06          LJVW 20/11/2009
 *				- Added debug info
 *				- Changed bSendData() to only TX once, even if Winsock buffer full
 *      V1.07          LJVW 23/11/2009
 *				- Added base class to encapsulate calls common to LJTcpClientSocket
 *      V1.08          LJVW 27/01/2010
 *				- Added option to close idle client socket connections (no data received for more than 5 minutes)
 *				- Added option to display "No Data" status if no data received after a predefined period
 *				- Added GetStatus() method to retrieve socket status and corresponding TEdit colour
 *      V1.09          LJVW 29/01/2010
 *				- Bug Fixes
 *      V1.10          LJVW 25/02/2010
 *				- Changed bSendData() to only send on last opened connection
 *      V1.11          LJVW 11/08/2010
 *				- Converted to UniCode
 *      V1.12          LJVW 29/07/2011
 *				- Made Remote Client Port Available for Status indications
 *
/**********************************************************************************/

#include "jpch.h"

#pragma hdrstop

#include "LJTcpServerSocket.h"
#include "JTime.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

LJSingleServerSocket::LJSingleServerSocket(TComponent* _pOwnerForm, const int _nPort,const bool _bEnableDebugging,const bool _bAutoCloseIdleConnections,const int _nNoDataDisplayTimeOut_Min,const int _nMaxRxBuffSize)
			: LJSingleSocketBaseClass(_bEnableDebugging,TCP_SERVER_VERSION,true,"",_nPort,_nMaxRxBuffSize)
{
   bEnableDebugging=_bEnableDebugging;
   pOwnerForm=_pOwnerForm;
   nPort=_nPort;
   pServerSocket=NULL;
   bAutoCloseIdleConnections=_bAutoCloseIdleConnections;
   nNoDataDisplayTimeOut_Min=_nNoDataDisplayTimeOut_Min;
   bListen=false;
	SetRemoteIP("");
	UpdateStatus(SOCKET_STATUS_OFF);
   ServerSocketDataAges.Clear();
   dwLastDataTick=0;
}

void LJSingleServerSocket::DestroyServerSocket()
{
   try
   {
      if (pServerSocket)
      {
         pServerSocket->Socket->Lock();
         pServerSocket->Socket->Close();
         pServerSocket->Socket->Unlock();
         WriteToDebugLog("DestroyServerSocket()","127.0.0.1",nPort);
      }
   }
   catch (...)
   {
   }
   UpdateStatus(SOCKET_STATUS_OFF);
   delete pServerSocket;
   pServerSocket=NULL;
   ServerSocketDataAges.Clear();
// bServerListening=false;
}

void LJSingleServerSocket::CloseSocket()
{
   WriteToDebugLog("CloseSocket()","127.0.0.1",nPort);
   DestroyServerSocket();
   bListen=false;
}

LJSingleServerSocket::~LJSingleServerSocket()
{
   WriteToDebugLog("~LJSingleServerSocket()","127.0.0.1",nPort);
   CloseSocket();
//   ClearRxFifo();
}


void LJSingleServerSocket::ListenForSocketConnect()
{
   bListen=true;
   ResetServerSocket(true);
}


void LJSingleServerSocket::ResetServerSocket(bool bResetErrorCount)
{
   if (nPort>-1)
   {
      DestroyServerSocket();
      pServerSocket=new TServerSocket(pOwnerForm);
      pServerSocket->Port=nPort;
      pServerSocket->ServerType=stNonBlocking;
      pServerSocket->Tag=0;
      pServerSocket->ThreadCacheSize=10; // default=50
      pServerSocket->OnClientConnect=ServerSocketClientConnect;
      pServerSocket->OnClientDisconnect=ServerSocketClientDisconnect;
      pServerSocket->OnClientError=ServerSocketClientError;
      pServerSocket->OnClientRead=ServerSocketClientRead;
      pServerSocket->OnClientWrite=ServerSocketClientWrite;
      pServerSocket->OnGetSocket=ServerSocketGetSocket;
      pServerSocket->OnGetThread=ServerSocketGetThread;
      pServerSocket->OnListen=ServerSocketListen;
      pServerSocket->OnThreadEnd=ServerSocketThreadEnd;
      pServerSocket->OnThreadStart=ServerSocketThreadStart;
      if (bResetErrorCount)
         nSocketErrorCount=0;
      if (bListen)
         DoListen();//pServerSocket->Open();
   }
}

void LJSingleServerSocket::DoListen()
{
   try
   {
      if (pServerSocket)
      {
         if (!pServerSocket->Active) // bServerListening) // if not already called Open() method
         {
            WriteToDebugLog("DoListen()","127.0.0.1",nPort);
            pServerSocket->Active=false;
            pServerSocket->Open();
         }
         else
         {
            pServerSocket->Socket->Lock();
            int nActiveConn=pServerSocket->Socket->ActiveConnections;
            pServerSocket->Socket->Unlock();
            if (nActiveConn==0)
               UpdateStatus(SOCKET_STATUS_WAITING);
            else
            {
               if (bAutoCloseIdleConnections)
               {
                  pServerSocket->Socket->Lock();
                  CheckSocketDataAge(); // close idle socket connections
                  pServerSocket->Socket->Unlock();
               }
               if (nNoDataDisplayTimeOut_Min>0)
               {
                  if (!bDataAgeTimedOut)
                  {
                     bDataAgeTimedOut=::GetTickCount()-dwLastDataTick>DWORD(nNoDataDisplayTimeOut_Min*60*1000);
                     if (bDataAgeTimedOut)
                        UpdateStatus(SOCKET_STATUS_NO_DATA);
                     else
                        UpdateStatus(SOCKET_STATUS_CONNECTED);
                  }
               }
               else
                  UpdateStatus(SOCKET_STATUS_CONNECTED);
            }
         }
      }
   }
   catch(...)
   {
      DestroyServerSocket();
   }

}



void LJSingleServerSocket::CheckSocketConnected()
{
   if (bListen)
   {
      if (pServerSocket==NULL)
         ResetServerSocket();
      else
         DoListen();
   }
}

void LJSingleServerSocket::UpdateSocketDataAge(const int nSocketHandle)
{
   ServerSocketDataAges.GoFirst();
   bool bFound=false;
   while (!ServerSocketDataAges.bLast()&&!bFound)
   {
      SocketDataAgeStruct* pS=ServerSocketDataAges.pNext();
      if (pS)
      {
         if (pS->nSocketHandle==nSocketHandle)
         {
            bFound=true;
            pS->DataTimeStamp.GetCurrentTime();
         }
      }
   }
}

void LJSingleServerSocket::AddSocketDataAgeStruct(const int nSocketHandle)
{
   SocketDataAgeStruct* pS;
   pS=new SocketDataAgeStruct;
   pS->nSocketHandle=nSocketHandle;
   pS->DataTimeStamp.GetCurrentTime();
   ServerSocketDataAges.nAdd(pS);
}

void LJSingleServerSocket::DeleteSocketDataAgeStruct(const int nSocketHandle)
{
   ServerSocketDataAges.GoFirst();
   bool bFound=false;
   while (!ServerSocketDataAges.bLast()&&!bFound)
   {
      SocketDataAgeStruct* pS=ServerSocketDataAges.pNext();
      if (pS)
			if (pS->nSocketHandle==nSocketHandle)
            bFound=true;
   }
   if (bFound)
   {
      ServerSocketDataAges.pPrev();
      ServerSocketDataAges.Delete();
   }
}

void LJSingleServerSocket::CheckSocketDataAge()
{
   ServerSocketDataAges.GoFirst();
   double dSec=JTime(true).dTotalSec();
   //int nSocketHandle;
   while (!ServerSocketDataAges.bLast())
   {
      SocketDataAgeStruct* pS=ServerSocketDataAges.pGetCurrent();
      if (pS)
      {
         if (DWORD(dSec-pS->DataTimeStamp.dTotalSec())>SERVER_SOCKET_CLOSE_IDLE_CONNECTION_TIMEOUT)
				CloseServerConnection(pS->nSocketHandle); // Corresponding entry in ServerSocketDataAges also deleted by this action
         else
            ServerSocketDataAges.pNext();
      }
   }
}

void LJSingleServerSocket::CloseServerConnection(const int nSocketHandle)
{
   bool bFound=false;
   for (int i=0;i<pServerSocket->Socket->ActiveConnections&&!bFound;i++)
   {
      if (pServerSocket->Socket->Connections[i]->SocketHandle==nSocketHandle)
      {
         bFound=true;
         try
         {
            WriteToDebugLog("CloseServerConnection() Idle Connection Closed",pServerSocket->Socket->Connections[i]->RemoteAddress,pServerSocket->Socket->Connections[i]->RemotePort);
            pServerSocket->Socket->Connections[i]->Close();
            UpdateStatus(SOCKET_STATUS_IDLE_CONN_CLOSED);
         }
         catch(...)
         {
         }
      }
   }
}


bool LJSingleServerSocket::bSendData(BYTE* pucData,const int nSize)
{
   bool bOK=false;
   try
   {
      if (pServerSocket)
      {
         if (pServerSocket->Socket->Connected&&pServerSocket->Socket->ActiveConnections>0)
         {
            pServerSocket->Socket->Lock();
				int nBytesQueued=pServerSocket->Socket->Connections[pServerSocket->Socket->ActiveConnections-1]->SendBuf(pucData,nSize); //pCurrentSocket->SendBuf(pucData,nSize); //pServerSocket->Socket->SendBuf(pucData,nSize);
            if (nBytesQueued==-1) // check if WinSock buffer full
            {
					WriteToDebugLog("bSendData() WinSock Buffer Full",pServerSocket->Socket->RemoteAddress,pServerSocket->Socket->RemotePort);
					nSocketErrorCount++;
				}
				else // TX Succeeded
					bOK=true;
				pServerSocket->Socket->Unlock();
			}
		}
	}
	catch(...)
	{
		bOK=false;
		if (pServerSocket)
			pServerSocket->Socket->Unlock();
		DestroyServerSocket();
		nSocketErrorCount++;
	}
	return bOK;
}




void __fastcall LJSingleServerSocket::ServerSocketAccept(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
	pServerSocket->Socket->Lock();
	UpdateStatus(SOCKET_STATUS_ACCEPT);
// bServerListening=true;
	pServerSocket->Socket->Unlock();
}

void __fastcall LJSingleServerSocket::ServerSocketClientConnect(TObject */*Sender*/,TCustomWinSocket *Socket)
{
	pServerSocket->Socket->Lock();
	SetRemoteIP(Socket->RemoteAddress);
	UpdateStatus(SOCKET_STATUS_CONNECTED);//("Connected ("+Socket->RemoteAddress+":"+String(Socket->RemotePort)+")");
	WriteToDebugLog("SocketConnected()",Socket->RemoteAddress,Socket->RemotePort);
	AddSocketDataAgeStruct(Socket->SocketHandle);
	dwLastDataTick=::GetTickCount();
	bDataAgeTimedOut=false;
	pServerSocket->Socket->Unlock();
}

void __fastcall LJSingleServerSocket::ServerSocketClientDisconnect(TObject */*Sender*/,TCustomWinSocket *Socket)
{
   pServerSocket->Socket->Lock();
	SetRemoteIP(Socket->RemoteAddress);
	UpdateStatus(SOCKET_STATUS_DISCONNECTED);//("Disconnected ("+Socket->RemoteAddress+":"+String(Socket->RemotePort)+")");
	WriteToDebugLog("SocketDisconnected()",Socket->RemoteAddress,Socket->RemotePort);
	DeleteSocketDataAgeStruct(Socket->SocketHandle);
	pServerSocket->Socket->Unlock();
}

void __fastcall LJSingleServerSocket::ServerSocketClientError(TObject */*Sender*/,TCustomWinSocket *Socket,TErrorEvent ErrorEvent,int &ErrorCode)
{
	pServerSocket->Socket->Lock();
	SetRemoteIP(Socket->RemoteAddress);
	UpdateStatus(SOCKET_STATUS_ERROR);//("Error ("+Socket->RemoteAddress+":"+String(Socket->RemotePort)+")");
//   bServerListening=false;
   nSocketErrorCount++;
   WriteToDebugLog("SocketError() "+sGetError(ErrorEvent,ErrorCode),Socket->RemoteAddress,Socket->RemotePort);
   ErrorCode=0;
   try
   {
      Socket->Close();
      DeleteSocketDataAgeStruct(Socket->SocketHandle);
   }
   catch(...)
	{
	}
	pServerSocket->Socket->Unlock();
}

void __fastcall LJSingleServerSocket::ServerSocketClientRead(TObject */*Sender*/,TCustomWinSocket *Socket)
{
	Socket->Lock();
	UpdateSocketDataAge(Socket->SocketHandle);
	dwLastDataTick=::GetTickCount();
	bDataAgeTimedOut=false;
	Socket->Unlock();
	ReadSocketData(Socket);
}

void __fastcall LJSingleServerSocket::ServerSocketClientWrite(TObject */*Sender*/,TCustomWinSocket */*Socket*/)
{
}

void __fastcall LJSingleServerSocket::ServerSocketGetSocket(TObject */*Sender*/,int /*Socket*/,TServerClientWinSocket *&/*ClientSocket*/)
{
}

void __fastcall LJSingleServerSocket::ServerSocketGetThread(TObject */*Sender*/,TServerClientWinSocket */*ClientSocket*/,TServerClientThread *&/*SocketThread*/)
{
}

void __fastcall LJSingleServerSocket::ServerSocketListen(TObject */*Sender*/,TCustomWinSocket *Socket)
{
	pServerSocket->Socket->Lock();
	nPort=Socket->LocalPort; // if nPort was 0, Windows auto-assign a new port which is stored in Socket->LocalPort
	UpdateStatus(SOCKET_STATUS_WAITING);
//   bServerListening=true;
	WriteToDebugLog("SocketListening()","127.0.0.1",nPort);
	pServerSocket->Socket->Unlock();
}

void __fastcall LJSingleServerSocket::ServerSocketThreadEnd(TObject* /*Sender*/,TServerClientThread* /*Thread*/)
{
}

void __fastcall LJSingleServerSocket::ServerSocketThreadStart(TObject* /*Sender*/,TServerClientThread* /*Thread*/)
{
}


/* *************************************************************************** */


LJTcpServerSockets::LJTcpServerSockets()
{
	bEnableDebugging=false;
	nCheckConnect=1;
	bAutoCloseIdleConnections=true;
	nNoDataDisplayTimeOut_Min=5;
	Init();
}
LJTcpServerSockets::LJTcpServerSockets(const int _nCheckConnect,const bool _bEnableDebugging,const bool _bAutoCloseIdleConnections,const int _nNoDataDisplayTimeOut_Min)
{
	bEnableDebugging=_bEnableDebugging;
	nCheckConnect=_nCheckConnect;
	bAutoCloseIdleConnections=_bAutoCloseIdleConnections;
	nNoDataDisplayTimeOut_Min=_nNoDataDisplayTimeOut_Min;
	Init();
}

LJTcpServerSockets::~LJTcpServerSockets()
{
	Clear();
	delete pWatchDogTimer;
	delete pServerSocketsLock;
}

void LJTcpServerSockets::Init()
{
	pServerSocketsLock=new TCriticalSection;
	pWatchDogTimer=NULL;
	bStopTimer=true;
	bInTimer=false;
	ServerSockets.Clear();
}


int LJTcpServerSockets::nAddServer(TComponent* _pOwnerForm,const int _nPort,const int _nMaxRxBuffSize)
{
	int nCount;
	pOwnerForm=_pOwnerForm;
	LJSingleServerSocket* pSocket=new LJSingleServerSocket(pOwnerForm,_nPort,bEnableDebugging,bAutoCloseIdleConnections,nNoDataDisplayTimeOut_Min,_nMaxRxBuffSize);
	pServerSocketsLock->Acquire();
	nCount=ServerSockets.nAdd(pSocket);
	pServerSocketsLock->Release();
	return nCount;
}

bool LJTcpServerSockets::bClientConnected(const int nServerIdx)
{
	bool bConn=false;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		bConn=ServerSockets[nServerIdx]->bClientConnected();
	}
	pServerSocketsLock->Release();
	return bConn;
}

int LJTcpServerSockets::nGetServerPort(const int nServerIdx)
{
	int nServerPort=-1;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		nServerPort=ServerSockets[nServerIdx]->nGetServerPort();
		if (pWatchDogTimer==NULL)
			StartTimer();
	}
	pServerSocketsLock->Release();
	return nServerPort;
}

int LJTcpServerSockets::nGetActiveConnections(const int nServerIdx)
{
	int nActiveConn=-1;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		nActiveConn=ServerSockets[nServerIdx]->nGetActiveConnections();
	}
	pServerSocketsLock->Release();
	return nActiveConn;
}

void LJTcpServerSockets::ListenForSocketConnect(const int nServerIdx)
{
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		ServerSockets[nServerIdx]->ListenForSocketConnect();
		if (pWatchDogTimer==NULL)
			StartTimer();
	}
	pServerSocketsLock->Release();
}

void LJTcpServerSockets::CloseSocket(const int nServerIdx)
{
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		ServerSockets[nServerIdx]->CloseSocket();
	}
	pServerSocketsLock->Release();
	CheckTimerNeeded();
}


void LJTcpServerSockets::CheckTimerNeeded()
{
	bool bNeeded=false;
	pServerSocketsLock->Acquire();
	ServerSockets.GoFirst();
	while (!ServerSockets.bLast()&&!bNeeded)
	{
		LJSingleServerSocket* pSocket=ServerSockets.pNext();
		if (pSocket)
			if (pSocket->bListening())
				bNeeded=true;
	}
	pServerSocketsLock->Release();
	if (!bNeeded)
		StopTimer();
}


void LJTcpServerSockets::CheckConnect()
{
   pServerSocketsLock->Acquire();
	ServerSockets.GoFirst();
   nTotalErrorCount=0;
	while (!ServerSockets.bLast())
	{
		LJSingleServerSocket* pSocket=ServerSockets.pNext();
		if (pSocket)
		{
			pSocket->CheckSocketConnected();
			nTotalErrorCount+=pSocket->nGetSocketErrorCount();
		}
	}
	pServerSocketsLock->Release();
}


void LJTcpServerSockets::StopTimer()
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

void LJTcpServerSockets::StartTimer()
{
	if (pWatchDogTimer)
		StopTimer();
	pWatchDogTimer=new TTimer(pOwnerForm);
	pWatchDogTimer->Enabled=false;
	pWatchDogTimer->Interval=300; //nCheckConnect*1000;
	pWatchDogTimer->OnTimer=WatchDogTimerTimer;
	bStopTimer=false;
	bInTimer=false;
	dwPrevWatchDogTick=0;
	pWatchDogTimer->Enabled=true;
}


bool LJTcpServerSockets::bSendData(const int nServerIdx,BYTE* pucData,const int nSize)
{
	bool bOK=false;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		bOK=ServerSockets[nServerIdx]->bSendData(pucData,nSize);
	}
	pServerSocketsLock->Release();
	return bOK;
}


int LJTcpServerSockets::nReadData(const int nServerIdx,BYTE* pucData,const int nMaxSize)
{
	int nBytesRead=0;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		nBytesRead=ServerSockets[nServerIdx]->nReadData(pucData,nMaxSize);
	}
	pServerSocketsLock->Release();
	return nBytesRead;
}

bool LJTcpServerSockets::bReadByte(const int nServerIdx,BYTE& uc)
{
	return (nReadData(nServerIdx,&uc,1)==1);
}



void __fastcall LJTcpServerSockets::WatchDogTimerTimer(TObject */*Sender*/)
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

void LJTcpServerSockets::Clear()
{
	StopTimer();
	pServerSocketsLock->Acquire();
	ServerSockets.Pack();
	ServerSockets.Clear();
	pServerSocketsLock->Release();
}

int LJTcpServerSockets::nGetStatusChanged()
{
	int nFirstSocketStatusChanged=-1;
	pServerSocketsLock->Acquire();
	if (ServerSockets.nGetCount()>0)
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		for (int i=0;i<ServerSockets.nGetCount()&&nFirstSocketStatusChanged<0;i++)
			if (ServerSockets[i]->bGetStatusChanged())
				nFirstSocketStatusChanged=i;
	}
	pServerSocketsLock->Release();
	return nFirstSocketStatusChanged;
}


bool LJTcpServerSockets::bGetStatusChanged(const int nServerIdx)
{
	bool bStatChanged=false;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		bStatChanged=ServerSockets[nServerIdx]->bGetStatusChanged();
	}
	pServerSocketsLock->Release();
	return bStatChanged;
}



String LJTcpServerSockets::sGetStatus(const int nServerIdx,const bool bShowRemoteIP)
{
	String sStatus="Off";
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		if (bShowRemoteIP)
			sStatus=ServerSockets[nServerIdx]->sGetStatus(ServerSockets[nServerIdx]->sGetRemoteClientIP());
		else
			sStatus=ServerSockets[nServerIdx]->sGetStatus("");
	}
	pServerSocketsLock->Release();
	return sStatus;
}


void LJTcpServerSockets::GetStatus(const int nServerIdx,String& sStatus,TColor& StatusEditColour,const bool bShowRemoteIP)
{
	sStatus="";
	StatusEditColour=clBlack;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		if (bShowRemoteIP)
			ServerSockets[nServerIdx]->GetStatus(sStatus,StatusEditColour,ServerSockets[nServerIdx]->sGetRemoteClientIP());
		else
			ServerSockets[nServerIdx]->GetStatus(sStatus,StatusEditColour,"");
	}
	pServerSocketsLock->Release();
}


int LJTcpServerSockets::nGetSocketErrorCount(const int nServerIdx)
{
	int nErrors=-1;
	pServerSocketsLock->Acquire();
	if (nServerIdx<ServerSockets.nGetCount())
	{
		if (!ServerSockets.bPacked())
			ServerSockets.Pack();
		nErrors=ServerSockets[nServerIdx]->nGetSocketErrorCount();
	}
	pServerSocketsLock->Release();
	return nErrors;
}

int LJTcpServerSockets::nGetTotalSocketErrorCount()
{
	int nErrors=-1;
	pServerSocketsLock->Acquire();
	if (ServerSockets.nGetCount()>0)
		nErrors=nTotalErrorCount;
	pServerSocketsLock->Release();
	return nErrors;
}

