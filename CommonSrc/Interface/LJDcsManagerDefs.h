//---------------------------------------------------------------------------

#ifndef LJDcsManagerDefsH
#define LJDcsManagerDefsH

#include "Cst4Defs.h"
#include "Cst4Manager.h"
#include "LJTcpClientSocket.h"
#include "LJTcpSocketDefs.h"

#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdUDPBase.hpp"
#include "IdUDPServer.hpp"
#include "IdIcmpClient.hpp"

//---------------------------------------------------------------------------

#pragma option push -bi-    //Do not treat enums as ints

enum DCS_STATUS_TYPE
{
	DCS_STAT_IDLE					=CST4_REQUEST_ODCS_START,
	DCS_STAT_CAPTURE				=CST4_REQUEST_ODCS_START+1,
	DCS_STAT_CLOSE_EXE			=CST4_REQUEST_ODCS_START+2,
	DCS_STAT_SHUTDOWN				=CST4_REQUEST_ODCS_START+3,
	DCS_STAT_OFF					=CST4_REQUEST_ODCS_START+4,
	DCS_STAT_ERROR					=CST4_REQUEST_ODCS_START+5,
	DCS_STAT_UNDEFINED         =CST4_REQUEST_ODCS_START+6,
};

class DCSStatusClass
{
	enum
	{
		NO_UPDATES = 5, // no of columns to update on DCS Manager StringGrid
	};

	TComponent* pOwner;
	DWORD dwLastPingResponse;

	int nUpdate;
	DCS_STATUS_TYPE CurrStat;
	DCS_STATUS_TYPE StateUp;
	DCS_STATUS_TYPE StateDown;

	String sUDPIP;
	int nUDPPort;
	int nCst4SiteID;
	String sStatus;
	bool bPing;

	String sGetNextState(const DCS_STATUS_TYPE Stat) const;
	void TXRequest(TIdUDPServer* pServer,const DCS_STATUS_TYPE Req);
	LJTcpClientSockets TcpShutDownClients;

public:
	void ReqStateUp(TIdUDPServer* pServer)
	{
		TXRequest(pServer,StateUp);
	}

	DCS_STATUS_TYPE GetDemoteState() const
	{
		return StateDown;
	}

	void ReqStateDown(TIdUDPServer* pServer)
	{
		if (StateDown == DCS_STAT_SHUTDOWN)
			ShutdownSystem();
		else
		   TXRequest(pServer,StateDown);
	}

	void ResetUpdate()
	{
		nUpdate=NO_UPDATES;
	}
	bool bGetUpdate()
	{
		return (nUpdate-- > 0);
	}

	void SetCurrentStatus(const DCS_STATUS_TYPE Stat,Cst4Manager* pRX);

	DCS_STATUS_TYPE GetCurrentStatus() const
	{
		return CurrStat;
	}
	String sGetCurrentStatus() const
	{
		return sStatus;
	}

	String sGetNextStateDown() const
	{
		return sGetNextState(StateDown);
	}
	String sGetNextStateUp() const
	{
		return sGetNextState(StateUp);
	}

	void SetUDPPort(const int nPort)
	{
		nUDPPort=nPort;
	}
	int nGetUDPPort() const
	{
		return nUDPPort;
	}

	void SetUDPIP(const String sIP)
	{
		sUDPIP=sIP;
	}
	String sGetUDPIP() const
	{
		return sUDPIP;
   }

	void SetCst4SiteID(const int nID)
	{
		nCst4SiteID=nID;
	}

	int nGetCst4SiteID() const
	{
      return nCst4SiteID;
	}

	void SetPing(const bool _bPing)
	{
		bPing=_bPing;
	}

	bool bGetPing() const
	{
		return bPing;
   }

	DCSStatusClass(const DCSStatusClass& L) // copy constructor
	{
		*this=L;
	};

	bool bCheckSystemOff(const bool bPingReply)
	{
		bool bSysOff=false;
		if (bPingReply)
			dwLastPingResponse=::GetTickCount();
		else if (::GetTickCount() - dwLastPingResponse > 5000)
		{
			SetCurrentStatus(DCS_STAT_OFF,NULL);
			ResetUpdate();
			bSysOff=true;
		}
		return bSysOff;
	}

	void ShutdownSystem(); // use ProcWatch on remote PC to execute Shutdown


	DCSStatusClass& operator = (const DCSStatusClass& L)
	{
		dwLastPingResponse=L.dwLastPingResponse;
		nUpdate=L.nUpdate;
		CurrStat=L.CurrStat;
		StateUp=L.StateUp;
		StateDown=L.StateDown;
		sUDPIP=L.sUDPIP;
		nUDPPort=L.nUDPPort;
		nCst4SiteID=L.nCst4SiteID;
		sStatus=L.sStatus;
		bPing=L.bPing;
		return *this;
	};

	DCSStatusClass(TComponent* _pOwner=NULL)
	{
      pOwner=_pOwner;
		nUpdate=NO_UPDATES;
		bPing=false;
		sUDPIP="";
		nCst4SiteID=-1;
	};
	~DCSStatusClass()
	{
	};
};




#pragma option pop


#endif
