//---------------------------------------------------------------------------

#ifndef LJUdpSocketH
#define LJUdpSocketH


#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdUDPBase.hpp"
#include "IdUDPClient.hpp"



#include "IdUDPServer.hpp"
#include "JByteFifo.h"

#include <SyncObjs.hpp>

//---------------------------------------------------------------------------

// ============================================================================
/*
	TYPICAL USAGE:
	==============

*/
// ============================================================================



class LJUdpSocket  // Thread Safe Class
{
	TIdUDPClient* pUdpClient;	// used for transmissions
	TIdUDPServer* pUdpServer;	// used for listening for client data transmissions
	JByteFifo RxByteFifo;  	// thread safe
	TCriticalSection* pUdpLock;
	String sRemoteIP;
	int nRemotePort;
	String sSenderIP;				// used to store IP address of peer from which last transmission was received
	int nSenderPort;           // used to store port number of peer from which last transmission was received


//	void __fastcall UdpSocketClientConnect(TObject *Sender);
//	void __fastcall IdUDPClientStatus(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
//	void __fastcall IdUDPClientConnected(TObject *Sender);
//	void __fastcall IdUDPClientDisconnected(TObject *Sender);

	void __fastcall IdUDPServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData,TIdSocketHandle *ABinding);

// ============================================================================
// copy contructor and assignment-operator declared private to prevent class copy

	LJUdpSocket(const LJUdpSocket& L);
	LJUdpSocket& operator = (const LJUdpSocket& L);

// ============================================================================

	void SendData(void* pData,const int nSize,const String _sRemoteIP,const int _nRemotePort,const bool bBroadcast); // does the actual sending


public:
	LJUdpSocket(TComponent* pOwner,const String _sIPAddress="",const int _nPort=-1);
	~LJUdpSocket();

	void Reset(const String _sRemoteIP,const int _nRemotePort);
	void SendData(void* pData,const int nSize); // sends data to IP and port set in Constructor or Reset()
	void SendData(void* pData,const int nSize,const String _sRemoteIP,const int _nRemotePort);
	void BroadcastData(void* pData,const int nSize,const int _nRemotePort=-1); // broadcast to port set in Constructor or Reset() if = -1

	void Listen(const int _nPort)
	{
		if (pUdpServer)
		{
			pUdpServer->DefaultPort=_nPort;
			pUdpServer->Active=true;
		}
	}

	int nReadData(BYTE* pucData,const int nSize);

	void GetLastSender(String& _sSenderIP,int& _nSenderPort) // returns IP address and Port number from which transmission was received
	{
		pUdpLock->Acquire();
		_sSenderIP=sSenderIP;
		_nSenderPort=nSenderPort;
		pUdpLock->Release();
	}

	void GetLastSender(String& _sSenderIP) // returns IP address and Port number from which transmission was received
	{
		pUdpLock->Acquire();
		_sSenderIP=sSenderIP;
		pUdpLock->Release();
	}

	void SetActive(const bool bActive)
	{
		if (pUdpServer)
			pUdpServer->Active=bActive;
	}

};


#endif
