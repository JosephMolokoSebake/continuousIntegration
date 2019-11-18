//---------------------------------------------------------------------------

#ifndef LJUdpDcsClientH
#define LJUdpDcsClientH

#include "LJDcsManagerDefs.h"

#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdUDPBase.hpp"
#include "IdUDPClient.hpp"
#include "IdUDPServer.hpp"

#include "Cst4Manager.h"
//#include <SyncObjs.hpp>

//---------------------------------------------------------------------------

// ============================================================================
/*
	TYPICAL USAGE:
	==============
	- Create LJUdpDcsClient class with own "Cst4 Site Identifier" and IP address of "DCS Manager"
	- Periodically (every 1 second, say) poll bNewRequestReceived() as follows:
			if (LJUdpDcsClient->bNewRequestReceived(DcsReq))
			{
				switch (DcsReq)
				{
					case DCS_STAT_CAPTURE:
					{
						// START CAPTURE...
						break;
					}
					case DCS_STAT_IDLE:
					{
						// STOP CAPTURE...
						break;
					}
					case DCS_STAT_CLOSE_EXE:
					{
						// CLOSE PROGRAM...
						break;
					}
					default: ;
				}
			}
	- to report a DCS action performed (Start Capture / Stop Capture / Close / Error), call SetAndTxStatus(), e.g.
			on START CAPTURE:
				SetAndTxStatus(DCS_STAT_CAPTURE);
			on ERROR CONDITION:
				SetAndTxStatus(DCS_STAT_ERROR,"Communications Interface Failure");

*/
// ============================================================================



class LJUdpDcsClient  // Thread Safe Class
{
	TIdUDPClient* pUDPClient;
	TCriticalSection* pUdpLock;
	int nCst4SiteID;
	String sDCSManagerIP;

	void __fastcall UdpSocketClientConnect(TObject *Sender);
	void __fastcall IdUDPClientStatus(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
	void __fastcall IdUDPClientConnected(TObject *Sender);
	void __fastcall IdUDPClientDisconnected(TObject *Sender);

	DWORD dwLastStatusTxTick;
	AnsiString sStatus;
	DCS_STATUS_TYPE Status;

	bool bProcessRxData(TBytes IdByteData,DCS_STATUS_TYPE& NewDcsReq);
	Cst4Manager RX;

	void Init();
	void TXStatus();


// ============================================================================
// copy contructor and assignment-operator declared private to prevent class copy

	LJUdpDcsClient(const LJUdpDcsClient& L)
	{
	};

	LJUdpDcsClient& operator = (const LJUdpDcsClient& L)
	{
		return *this;
	};

// ============================================================================

public:
	LJUdpDcsClient(TComponent* pOwner,const int _nCst4SiteID,const String _sDCSManagerIP);
	~LJUdpDcsClient();

	void SetAndTxStatus(const DCS_STATUS_TYPE _Status,const String _sStatus="")
	{
		Status=_Status;
		sStatus=AnsiString(_sStatus);
		TXStatus();
	}

	bool bNewRequestReceived(DCS_STATUS_TYPE& NewDcsReq); // to be polled every 1 sec or so
};


#endif
