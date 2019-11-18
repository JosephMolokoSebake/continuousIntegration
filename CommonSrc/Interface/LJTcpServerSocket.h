//---------------------------------------------------------------------------

#ifndef LJTcpServerSocketH
#define LJTcpServerSocketH
//---------------------------------------------------------------------------
#include <System.Win.ScktComp.hpp>
#include "JList.h"
#include "JFile.h"
#include "JTime.h"
#include "LJTcpSocketDefs.h"

#define TCP_SERVER_VERSION "TCPServer Version 1.13"

//---------------------------------------------------------------------------

class LJSingleServerSocket : public LJSingleSocketBaseClass  // do not use this class directly - rather use LJTcpServerSockets
{
   bool bEnableDebugging;
   bool bAutoCloseIdleConnections;
   int nNoDataDisplayTimeOut_Min;
   TServerSocket* pServerSocket;
   bool bListen;
	TComponent* pOwnerForm;
	int nPort;
	DWORD dwLastDataTick;
	bool bDataAgeTimedOut;

	void ResetServerSocket(bool bResetErrorCount=false); // re-initialise socket => connect if bListen

	void __fastcall ServerSocketAccept(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientConnect(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientDisconnect(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientError(TObject *Sender,TCustomWinSocket *Socket,TErrorEvent ErrorEvent,int &ErrorCode);
	void __fastcall ServerSocketClientRead(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketClientWrite(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketGetSocket(TObject *Sender,int Socket,TServerClientWinSocket *&ClientSocket);
	void __fastcall ServerSocketGetThread(TObject *Sender,TServerClientWinSocket *ClientSocket,TServerClientThread *&SocketThread);
	void __fastcall ServerSocketListen(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ServerSocketThreadEnd(TObject *Sender,TServerClientThread *Thread);
	void __fastcall ServerSocketThreadStart(TObject *Sender,TServerClientThread *Thread);

	void DestroyServerSocket();
	void DoListen();

	void UpdateSocketDataAge(const int nSocketHandle);
	void AddSocketDataAgeStruct(const int nSocketHandle);
	void DeleteSocketDataAgeStruct(const int nSocketHandle);
	void CheckSocketDataAge();
	void CloseServerConnection(const int nSocketHandle);

	struct SocketDataAgeStruct
	{
		int nSocketHandle;
		JTime DataTimeStamp;
	};

	JList<SocketDataAgeStruct>ServerSocketDataAges;

//************************************************************************************
	// Copy and Assign Contructors declared (and NOT DEFINED) as private to ensure that they are NOT USED
	void Assign(LJSingleServerSocket& _S);
	LJSingleServerSocket& operator = (LJSingleServerSocket& _S);

	LJSingleServerSocket(LJSingleServerSocket& _S); // Copy constructor
//************************************************************************************

public:
	LJSingleServerSocket(TComponent* _pOwnerForm,const int _nPort,const bool bEnableDebugging,const bool _bAutoCloseIdleConnections,const int _nNoDataDisplayTimeOut_Min,const int _nMaxRxBuffSize);
	~LJSingleServerSocket();

	int nGetServerPort() const
	{
		return nPort;
	}

	String sGetRemoteClientIP() const
	{
		return sGetRemoteIP();
	}

	int nGetActiveConnections() const
	{
		 int nActiveConn=-1;
		 if (pServerSocket)
			 if (pServerSocket->Socket)
			 {
				 pServerSocket->Socket->Lock();
				 nActiveConn=pServerSocket->Socket->ActiveConnections;
				 pServerSocket->Socket->Unlock();
			 }
		 return nActiveConn;
	}

	void ListenForSocketConnect();
	void CloseSocket();

	void CheckSocketConnected();

	bool bSendData(BYTE* pucData,const int nSize);

	bool bClientConnected() const
	{
		if (pServerSocket)
			return (pServerSocket->Socket->ActiveConnections>0);
		else
			return false;
	}

	bool bListening()
	{
		return bListen;
	}
};



class LJTcpServerSockets
{

private:
   JList<LJSingleServerSocket>ServerSockets;
   TCriticalSection* pServerSocketsLock;

   TComponent* pOwnerForm;

   TTimer* pWatchDogTimer;
   void __fastcall WatchDogTimerTimer(TObject *Sender);
   bool bInTimer;
   bool bStopTimer;

   bool bEnableDebugging;
   bool bAutoCloseIdleConnections;
   int nNoDataDisplayTimeOut_Min;


   void CheckConnect();
   void StopTimer();
   void StartTimer();

   int nCheckConnect;

   void Init();

   int nTotalErrorCount;

   void CheckTimerNeeded();

   DWORD dwPrevWatchDogTick;

public:
   LJTcpServerSockets(); // check for connection every 1 sec, no debugging, Autoclose idle connections, No data display timeout=5 min
   LJTcpServerSockets(const int _nCheckConnect,const bool _bEnableDebugging,const bool _bAutoCloseIdleConnections,const int _nNoDataDisplayTimeOut_Min);
   			// nCheckConnect:		check for connection every n seconds - reset socket if none found
   			// bEnableDebugging:		Write Debug Log to File
   			// bAutoCloseIdleConnections:	Close Idle Connections After SERVER_SOCKET_CLOSE_IDLE_CONNECTION_TIMEOUT minutes
   			// nNoDataDisplayTimeOut_Min:	Update Server status to "No Data" after n minutes of no data received
   ~LJTcpServerSockets();

	int nAddServer(TComponent* _pOwnerForm,const int _nPort=0,const int _nMaxRxBuffSize=0); // Add listening server on specified port, OR auto-assigned by Windows of Port=0. Returns index of added server in servers list
	int nGetServerPort(const int nServerIdx);
	int nGetActiveConnections(const int nServerIdx);
   bool bClientConnected(const int nServerIdx);

   int nGetCount() const
   {
      int nCount=0;
      pServerSocketsLock->Acquire();
      nCount=ServerSockets.nGetCount();
      pServerSocketsLock->Release();
      return nCount;

   }

   void ListenForSocketConnect(const int nServerIdx);
   void CloseSocket(const int nServerIdx); // ALL sockets closed on call to Clear()

   bool bSendData(const int nServerIdx,BYTE* pucData,const int nSize);
   int nReadData(const int nServerIdx,BYTE* pucData,const int nMaxSize);
   bool bReadByte(const int nServerIdx,BYTE& uc);

   int nGetStatusChanged();
   bool bGetStatusChanged(const int nServerIdx);
   String sGetStatus(const int nServerIdx,const bool bShowRemoteIP);
   void GetStatus(const int nServerIdx,String& sStatus,TColor& StatusEditColour,const bool bShowRemoteIP);

	void GetStatusFromEnum(const SOCKET_STATUS Stat,String& sStatus,TColor& StatusEditColour,const String sRemoteClientIP)
	{
      LJSingleServerSocket::GetStatusFromEnum(Stat,sStatus,StatusEditColour,sRemoteClientIP);
   }


   int nGetSocketErrorCount(const int nServerIdx);
   int nGetTotalSocketErrorCount();

   void Clear();

};



#endif

