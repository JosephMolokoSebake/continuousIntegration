#ifndef LJTcpClientSocketH
#define LJTcpClientSocketH
//---------------------------------------------------------------------------
#include <System.Win.ScktComp.hpp>
#include "JList.h"
#include "JFile.h"
#include "LJTcpSocketDefs.h"

#define TCP_CLIENT_VERSION "TCPClient Version 1.12"

//---------------------------------------------------------------------------

class LJSingleClientSocket : public LJSingleSocketBaseClass   // do not use this class directly - rather use LJTcpClientSockets
{
   bool bEnableDebugging;
   TClientSocket* pClientSocket;
   int nWinSockBufSize;
   bool bConnectClient;
   TComponent* pOwnerForm;
	String sIPAddress;
   int nPort;
   bool bClientConnecting;
   bool bInDoConnect;
	DWORD dwLastDisconnect;
	DWORD dwStartConnectAttemp;
	bool bPrevTXFailed;
	String sLocalIP;

	void __fastcall ClientSocketConnect(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ClientSocketError(TObject *Sender,TCustomWinSocket *Socket,TErrorEvent ErrorEvent,int &ErrorCode);
   void __fastcall ClientSocketDisconnect(TObject *Sender,TCustomWinSocket *Socket);
   void __fastcall ClientSocketConnecting(TObject *Sender,TCustomWinSocket *Socket);
	void __fastcall ClientSocketLookup(TObject *Sender,TCustomWinSocket *Socket);
   void __fastcall ClientSocketRead(TObject *Sender,TCustomWinSocket *Socket);
   void __fastcall ClientSocketWrite(TObject *Sender,TCustomWinSocket *Socket);

   void DestroyClientSocket();
   void DoConnect();

	void WriteToClientDebugLog(const String sMsg)
	{
		if (pClientSocket)
			WriteToDebugLog(sMsg,pClientSocket->Address,pClientSocket->Port);
		else
			WriteToDebugLog(sMsg,"999.999.999.999",-1);
	}


//************************************************************************************
   // Copy and Assign Contructors defined as private to ensure that they are NOT USED
   void Assign(LJSingleClientSocket& _S);
	LJSingleClientSocket& operator = (LJSingleClientSocket& _S);

	LJSingleClientSocket(LJSingleClientSocket& _S); // Copy constructor
//************************************************************************************

public:
	LJSingleClientSocket(TComponent* _pOwnerForm,const String _sIPAddress,const int _nPort,const bool _bEnableDebugging,const int _nWinSockBufSize,const int _nMaxRxBuffSize);
	~LJSingleClientSocket();

   bool bSetWinSockBufSize(const int _nSize); // set to -1 to use Windows Default (8192 bytes)

   bool bClientConnected() const
   {
      if (pClientSocket)
         return pClientSocket->Socket->Connected;
      else
         return false;
   }

   void ConnectClientSocket();
   void CheckSocketConnected();
   bool bSendData(BYTE* pucData,const int nSize);
   void DisconnectClientSocket();
	void ResetClientSocket(bool bResetErrorCount=false); // re-initialise socket => connect if bConnectClient
	String sGetLocalIPAddr();

};



class LJTcpClientSockets
{

private:
   JList<LJSingleClientSocket>ClientSockets;
   TCriticalSection* pClientSocketsLock;

   bool bEnableDebugging;
   int nWinSockBufSize;

   TComponent* pOwnerForm;

   TTimer* pWatchDogTimer;
   void __fastcall WatchDogTimerTimer(TObject *Sender);
   bool bInTimer;

   bool bStatusOff;

   void CheckConnect();
   void StopTimer();
   void StartTimer();
   bool bStopTimer;

   int nCheckConnect;
   void Init();

   int nTotalErrorCount;

   DWORD dwPrevWatchDogTick;


public:
   LJTcpClientSockets(); // default Check for connection: 1 seconds, debugging disabled, Winsock Buff Size=8192 => i.e. unchanged
   LJTcpClientSockets(const int _nCheckConnectSec,const bool _bEnableDebugging,const int _nWinSockBufSize); // set to -1 to use Windows Default (8192 bytes)
   ~LJTcpClientSockets();

   void SetCheckConnectTime(const int _nCheckConnectSec)
   {
      nCheckConnect=_nCheckConnectSec;
   };


   int nAddClient(TComponent* _pOwnerForm,const String _sIPAddress,const int _nPort,const int nMaxRxBuffSize=0);
   int nGetCount() const
   {
		int nCount=0;
		pClientSocketsLock->Acquire();
		nCount=ClientSockets.nGetCount();
		pClientSocketsLock->Release();
		return nCount;
	}


   bool bClientConnected(const int nClientIdx);
   void ConnectClient(const int nClientIdx);
   void DisconnectClient(const int nClientIdx);
   void DisconnectClientSockets(); // disconnects all clients


   bool bSendData(const int nClientIdx,BYTE* pucData,const int nSize);
   int nReadData(const int nClientIdx,BYTE* pucData,const int nMaxSize);
   bool bReadByte(const int nClientIdx,BYTE& uc);

	String sGetStatus(const int nClientIdx);
	SOCKET_STATUS GetStatus(const int nClientIdx);
	void GetStatus(const int nClientIdx,String& sStatus,TColor& StatusEditColour);
	bool bGetStatusChanged(const int nClientIdx);
	int nGetStatusChanged();  // returns first socket number for which the status has changed

	void GetStatusFromEnum(const SOCKET_STATUS Stat,String& sStatus,TColor& StatusEditColour)
   {
      LJSingleClientSocket::GetStatusFromEnum(Stat,sStatus,StatusEditColour,"");
   }

   int nGetSocketErrorCount(const int nClientIdx);
   int nGetTotalSocketErrorCount();

   void Clear();

   bool bSetWinSockBufSize(const int nClientIdx,const int nSize); // set to -1 to use Windows Default (8192 bytes)


	String sGetLocalIPAddr(int nClientIdx);

   bool bGetStatusOff() const
   {
      return bStatusOff;
   }

};



#endif

