//---------------------------------------------------------------------------
#ifndef Cst4UDPH
#define Cst4UDPH
//---------------------------------------------------------------------------

#include "LJUdpSocket.h"
#include "Cst4Manager.h"


class Cst4UDP : public LJUdpSocket, public Cst4Manager
{

	WORD wOwnID;

	int nMaxRXBuf;
	BYTE* pucRXBuf;

	bool bBroadcast;

	int nTotalRXMessages;

	DWORD dwConvertAddr(const String sAddr);
	String sConvertAddr(const DWORD dwAddr);

public:

	Cst4UDP(TComponent* pOwner, const WORD _wOwnID);

	~Cst4UDP();

	void InitTX(const int nPort, const String sAddr="", const WORD _wOwnID=0xFFFF);
	int nTXMessages();
	void SendRXWatchdogMsg();
	void SendTXWatchdogMsg();
	void SendNotifyMsg();
	void SendRebootMsg();

	void InitRX(const int nPort, const int nMaxBufSize=1024*1024);
	int nUpdateRX();				//Returns number of messages received

	int nGetTotalRXMessages() 		const	{return nTotalRXMessages;   	}

	DWORD dwGetSourceAddr();

};


#endif
