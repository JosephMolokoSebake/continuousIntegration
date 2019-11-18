//---------------------------------------------------------------------------

#ifndef Cst4UDPTimeH
#define Cst4UDPTimeH


#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdUDPBase.hpp"
#include "IdUDPClient.hpp"
#include "IdUDPServer.hpp"
#include "JByteFifo.h"
#include "Cst4Manager.h"

//---------------------------------------------------------------------------

class Cst4UDPTime : public Cst4Manager  // Thread Safe Class
{
	TIdUDPClient* pUdpClient;				// used for transmissions
	TIdUDPServer* pUdpServer;				// used for listening for client data transmissions
	JByteFifo RxByteFifo;  					// thread safe
	TCriticalSection* pUdpLock;
	String sSenderIP;							// used to store IP address of peer from which last transmission was received

	enum {  RXBUF_SIZE = 8192};

	BYTE aucRXBuf[RXBUF_SIZE];

	JTime LastRXTime_utc;
	JTime LastTXTime_utc;
	int nRequestCnt;
	int nRXTimeCnt;
	int nTXTimeCnt;

	JTime LastGPSUpdateTime_utc;
	DWORD dwLastGPSUpdateTime_utc_sec;

	bool bAutoTimeUpdate;

	WORD wOwnID;
	Cst4_Request Req;
	Cst4_Response Resp;

	String sDestAddr;

	JFile* pLogFil;


	void __fastcall IdUDPServerUDPRead(TIdUDPListenerThread *AThread, TBytes AData,TIdSocketHandle *ABinding);

// ============================================================================
// copy contructor and assignment-operator declared private to prevent class copy

	Cst4UDPTime(const Cst4UDPTime& L);
	Cst4UDPTime& operator = (const Cst4UDPTime& L);

// ============================================================================

	void Reset();

	void SendData(void* pData,const int nSize,const String sRemoteIP, const bool bBroadcast); // does the actual sending

	void SendData(void* pData,const int nSize,const String sRemoteIP);
	void BroadcastData(void* pData,const int nSize); // broadcast to port set in Constructor or Reset() if = -1
	int nReadData(BYTE* pucData,const int nSize);

	void GetLastSender(String& _sSenderIP) // returns IP address from which transmission was received
	{
		pUdpLock->Acquire();
		_sSenderIP=sSenderIP;
		pUdpLock->Release();
	}

	DWORD dwConvertAddr(const String sAddr);
	String sConvertAddr(const DWORD dwAddr);


public:

	Cst4UDPTime(TComponent* pOwner, const WORD _wOwnID=0, const bool _bAutoTimeUpdate=true, const bool bEnableRX=true);
	~Cst4UDPTime();

	void SetLastGPSUpdateTime(const JTime& T_utc)
	{
		LastGPSUpdateTime_utc=T_utc;
		dwLastGPSUpdateTime_utc_sec=LastGPSUpdateTime_utc.dwTotalSec();
	}

	JTime GetLastGPSUpdateTime_utc()
	{
		return LastGPSUpdateTime_utc;
	}

	int nGetTimeAge_hours(const DWORD dwCurrentTime_utc_sec) const				//Time since last GPS update - up to 7 hours
	{
		if (dwLastGPSUpdateTime_utc_sec>0)
			{
			int nH=(dwCurrentTime_utc_sec-dwLastGPSUpdateTime_utc_sec)/3600;
			if (nH>7)
				nH=7;
			return nH;
			}
		return 7;
	}

	int nGetTimeAge_hours() const				//Time since last GPS update - up to 7 hours
	{
		return nGetTimeAge_hours(JTime(true,true).dwTotalSec());
	}

	void SendCurrentTime(const String sIPAddr);
	void BroadcastCurrentTime();
	void SendTime(const String sIPAddr, const JTime& T_utc);
	void BroadcastTime(const JTime& T_utc);
	void RequestTime(const String sIPAddr);

	void SendGPSTime(const String sIPAddr, const JTime& GPSTime_utc); 	//Also updates PC time

	void CreateLogFil(const String sLogFileName);
	void DeleteLogFil();

	JTime GetLastRXTime_utc(DWORD& dwSourceAddr)
	{
		pUdpLock->Acquire();
		dwSourceAddr=dwConvertAddr(sSenderIP);
		JTime T=LastRXTime_utc;
		LastRXTime_utc.Invalidate();
		pUdpLock->Release();
		return T;
	}

	JTime GetLastRXTime_utc()
	{
		pUdpLock->Acquire();
		JTime T=LastRXTime_utc;
		LastRXTime_utc.Invalidate();
		pUdpLock->Release();
		return T;
	}

	JTime GetLastTXTime_utc(DWORD& dwDestAddr)
	{
		pUdpLock->Acquire();
		dwDestAddr=dwConvertAddr(sDestAddr);
		JTime T=LastTXTime_utc;
		LastTXTime_utc.Invalidate();
		pUdpLock->Release();
		return T;
	}

	JTime GetLastTXTime_utc()
	{
		pUdpLock->Acquire();
		JTime T=LastTXTime_utc;
		LastTXTime_utc.Invalidate();
		pUdpLock->Release();
		return T;
	}

	int nGetRequestCnt() const
	{
		return nRequestCnt;
	}

	int nGetRXTimeCnt() const
	{
		pUdpLock->Acquire();
		int nCnt=nRXTimeCnt;
		pUdpLock->Release();
		return nCnt;
	}

	int nGetTXTimeCnt() const
	{
		pUdpLock->Acquire();
		int nCnt=nTXTimeCnt;
		pUdpLock->Release();
		return nCnt;
	}



};


#endif
