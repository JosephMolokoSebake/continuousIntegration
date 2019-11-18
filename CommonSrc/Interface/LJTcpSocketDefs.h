//---------------------------------------------------------------------------

#ifndef LJTcpSocketDefsH
#define LJTcpSocketDefsH

#include <System.Win.ScktComp.hpp>
#include "JList.h"
#include "JFile.h"
#include "JByteFifo.h"

//---------------------------------------------------------------------------

#pragma option push -bi    //Treat enums as ints


enum
{
	//MAX_TCP_RX_BUFFER_SIZE								= 1024*64, // 64k
	SERVER_SOCKET_CLOSE_IDLE_CONNECTION_TIMEOUT	= 5*60,	 // Close idle connection to server socket after N seconds
};

enum SOCKET_STATUS
{
	SOCKET_STATUS_OFF,
	SOCKET_STATUS_CONNECTING,
	SOCKET_STATUS_CONNECTED,
	SOCKET_STATUS_DISCONNECTED,
	SOCKET_STATUS_WAITING,
	SOCKET_STATUS_ACCEPT,
	SOCKET_STATUS_ERROR,
	SOCKET_STATUS_LOOKUP,
	SOCKET_STATUS_NO_DATA,
	SOCKET_STATUS_IDLE_CONN_CLOSED,
};

#pragma option pop    		//Set compiler back to default enum type

class LJSingleSocketBaseClass
{
	JByteFifo RxByteFifo;  // thread safe
	int nMaxRxBuffSize;

	DWORD dwTotalTime_nReadData;
	DWORD dwTotalTime_ReadSocketData;

	//JList<BYTE>RxBytes;
	//TCriticalSection* pRxDataLock;

	SOCKET_STATUS SocketStatus;
	bool bStatusChanged;
	TCriticalSection* pStatusLock;

	JFile* pDebugFil;
	JFile::FILE_ERROR DebugFilError;
	TCriticalSection* pDebugFileLock;

	int nGetSocketRxBufSize(TCustomWinSocket *Socket);

	SOCKET_STATUS GetAndClearStatus();

	String sRemoteIP;

protected:
	void ReadSocketData(TCustomWinSocket *Socket);
	void ClearRxFifo();

	void WriteToDebugLog(const String sMsg,const String sIPAddress,const int nPort);


	void SetRemoteIP(const String _sRemoteIP)
	{
		sRemoteIP=_sRemoteIP;
	}
	String sGetRemoteIP() const
	{
		return sRemoteIP;
	}



	void UpdateStatus(const SOCKET_STATUS _SocketStatus);

	int nSocketErrorCount;

//************************************************************************************
	// Copy and Assign Contructors declared (and NOT DEFINED) as private to ensure that they are NOT USED
	LJSingleSocketBaseClass& operator = (const LJSingleSocketBaseClass& _B);
	LJSingleSocketBaseClass(const LJSingleSocketBaseClass& _B);  // Copy constructor
	void Assign(const LJSingleSocketBaseClass& _B);
//************************************************************************************


public:
	LJSingleSocketBaseClass(const bool bEnableDebugging,const String sVer,const bool bServer,const String sIPAddress,const int nPort,const int _nMaxRxBuffSize);
	~LJSingleSocketBaseClass();


	int nGetSocketErrorCount();
	String sGetStatus(const String sRemoteClientIP);
	SOCKET_STATUS GetStatus();
	void GetStatus(String& sStatus,TColor& StatusEditColour,const String sRemoteClientIP);
	bool bGetStatusChanged();
	int nReadData(BYTE* pucData,const int nSize);

	static String sGetError(const TErrorEvent ErrorEvent,const int ErrorCode);
	static void GetStatusFromEnum(const SOCKET_STATUS Stat,String& sStatus,TColor& StatusEditColour,const String sRemoteClientIP);
	static void SetStatusFromEnum(const SOCKET_STATUS Stat,TEdit* StatusEdit,const String sRemoteClientIP)
	{
		String s;
		TColor Col;
		GetStatusFromEnum(Stat,s,Col,sRemoteClientIP);
		StatusEdit->Text=s;
		StatusEdit->Color=Col;
	};

};

#endif
