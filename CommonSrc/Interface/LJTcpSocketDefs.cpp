//---------------------------------------------------------------------------

#include "jpch.h"

#pragma hdrstop

#include "LJTcpSocketDefs.h"
#include "JTime.h"
#include "JUtils.h"
//#include "LJUtils.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


LJSingleSocketBaseClass::LJSingleSocketBaseClass(const bool bEnableDebugging,const String sVer,const bool bServer,const String sIPAddress,const int nPort,const int _nMaxRxBuffSize)
{
	pStatusLock=new TCriticalSection;
	sRemoteIP="";
	UpdateStatus(SOCKET_STATUS_OFF);
	nMaxRxBuffSize=_nMaxRxBuffSize;

	//pRxDataLock=NULL;
	//pRxDataLock=new TCriticalSection;
	//RxBytes.Clear();
	dwTotalTime_nReadData=0;
	dwTotalTime_ReadSocketData=0;
	RxByteFifo.Clear();
	if (bEnableDebugging)
	{
		DebugFilError=JFile::F_NO_ERROR;
		pDebugFil=new JFile('B',JFile::ASCII_TYPE);
		JTime T(true);
		String sBaseFileName=IncludeTrailingBackslash(ExtractFilePath(Application->ExeName))+"\\TcpLogs\\"+ExcludeTrailingBackslash(ExtractFileName(Application->ExeName));
		if (!bDirExist("TcpLogs"))
			bCreateDir("TcpLogs",false);
		if (bServer)
			sBaseFileName=sBaseFileName+"_LJTcpServerSocket Debug Log_"+T.sFileName()+"_"+String(nPort);
		else
			sBaseFileName=sBaseFileName+"_LJTcpClientSocket Debug Log_"+T.sFileName()+"_"+sIPAddress+" ["+String(nPort)+"]";
		int nFil=0;
		String sName=sBaseFileName;
		while (bFileExist(sName+".txt"))
			sName.printf(L"%s_%03u",sBaseFileName,++nFil);
		DebugFilError=pDebugFil->Create(sName+".txt");
		if (!DebugFilError)
			DebugFilError=pDebugFil->TextLine("; "+sVer);
		pDebugFileLock=new TCriticalSection;
	}
	else
	{
		pDebugFil=NULL;
		pDebugFileLock=NULL;
	}


}

LJSingleSocketBaseClass::~LJSingleSocketBaseClass()
{
//	delete pRxDataLock;
//	pRxDataLock=NULL;
//	RxBytes.Clear();
//   RxByteFifo.Clear();
	delete pStatusLock;
	pStatusLock=NULL;
	if (pDebugFil)
	{
		pDebugFileLock->Acquire();
		if (!DebugFilError)
		{
			DebugFilError=pDebugFil->Open();
			if (!DebugFilError)
				DebugFilError=pDebugFil->Seek(pDebugFil->dwGetSize());
			if (!DebugFilError)
			{
				DebugFilError=pDebugFil->TextLine("Total Time in nReadData()="+String(dwTotalTime_nReadData));
				DebugFilError=pDebugFil->TextLine("Total Time in ReadSocketData()="+String(dwTotalTime_ReadSocketData));
			}
			if (!DebugFilError)
				DebugFilError=pDebugFil->Close();
		}
		delete pDebugFil;
		pDebugFil=NULL;
	   pDebugFileLock->Release();
	}
	delete pDebugFileLock;
	pDebugFileLock=NULL;
}


int LJSingleSocketBaseClass::nGetSocketRxBufSize(TCustomWinSocket *Socket)
{
	int nWinSockBufSize=-1;
	int nSize=sizeof(nWinSockBufSize);
	if (::getsockopt(Socket->SocketHandle,SOL_SOCKET,SO_RCVBUF,(char*)&nWinSockBufSize,&nSize)==0)
		return nWinSockBufSize;
	else
		return 8192; //default socket rcv buf size in Windows
}


void LJSingleSocketBaseClass::ReadSocketData(TCustomWinSocket *Socket)
{
	DWORD dwStartTick=::GetTickCount();
	int nSocketRxBufSize=nGetSocketRxBufSize(Socket);
	BYTE* pucData=new BYTE[nSocketRxBufSize];
	int nRxSize;
	try
	{
		Socket->Lock();
		nRxSize=Socket->ReceiveBuf(pucData,nSocketRxBufSize);
		Socket->Unlock();
	}
	catch(...)
	{
		nRxSize=0;
		Socket->Unlock();
		nSocketErrorCount++;
		UpdateStatus(SOCKET_STATUS_ERROR);
		WriteToDebugLog("ReadSocketData() Failed",Socket->RemoteAddress,Socket->RemotePort);
	}
	if (nRxSize>0)
	{
		if (nMaxRxBuffSize>0)
		{
			if (nRxSize + RxByteFifo.nGetSize() < nMaxRxBuffSize)
				RxByteFifo.nPush(pucData,nRxSize);
			else
			{
				UpdateStatus(SOCKET_STATUS_ERROR);
				WriteToDebugLog("ReadSocketData() Buffer Overflow: RX Buffer Cleared",Socket->RemoteAddress,Socket->RemotePort);
				nSocketErrorCount++;
			}
		}
		else // internal RX Fifo unbounded
			RxByteFifo.nPush(pucData,nRxSize);
	}
	delete[] pucData;
	dwTotalTime_ReadSocketData+=(::GetTickCount()-dwStartTick);
}

void LJSingleSocketBaseClass::ClearRxFifo()
{
	//pRxDataLock->Acquire();
	RxByteFifo.Clear();
	//RxBytes.Clear();
	//pMSRxFifo->Clear();
	//pRxDataLock->Release();
}


int LJSingleSocketBaseClass::nReadData(BYTE* pucData,const int nSize)
{
	DWORD dwStartTick=::GetTickCount();
	int nBytesRead=0;
	if (RxByteFifo.nGetSize()>0)
	{
		BYTE uc;
		while (nBytesRead <= nSize  && RxByteFifo.nPop(&uc) >= 0 )
			pucData[nBytesRead++]=uc;
	}
	dwTotalTime_nReadData+=(::GetTickCount()-dwStartTick);
	return nBytesRead;
}



void LJSingleSocketBaseClass::UpdateStatus(const SOCKET_STATUS _SocketStatus)
{
	pStatusLock->Acquire();
	if (SocketStatus!=_SocketStatus)
	{
		SocketStatus=_SocketStatus;
		bStatusChanged=true;
	}
	pStatusLock->Release();
}

SOCKET_STATUS LJSingleSocketBaseClass::GetAndClearStatus()
{
	SOCKET_STATUS Stat;
	pStatusLock->Acquire();
	Stat=SocketStatus;
	bStatusChanged=false;
	pStatusLock->Release();
	return Stat;
}


String LJSingleSocketBaseClass::sGetStatus(const String sRemoteClientIP)
{
	String sStat="";
	TColor DummyCol;
	GetStatusFromEnum(GetAndClearStatus(),sStat,DummyCol,sRemoteClientIP); // bStatusChanged set to FALSE
	return sStat;
}

SOCKET_STATUS LJSingleSocketBaseClass::GetStatus()
{
	return GetAndClearStatus();
}

void LJSingleSocketBaseClass::GetStatus(String& sStatus,TColor& StatusEditColour,const String sRemoteClientIP)
{
	GetStatusFromEnum(GetAndClearStatus(),sStatus,StatusEditColour,sRemoteClientIP); // bStatusChanged set to FALSE
}


bool LJSingleSocketBaseClass::bGetStatusChanged()
{
   bool bChanged;
	pStatusLock->Acquire();
   bChanged=bStatusChanged;
   pStatusLock->Release();
   return bChanged;
}

void LJSingleSocketBaseClass::GetStatusFromEnum(const SOCKET_STATUS Stat,String& sStatus,TColor& StatusEditColour,const String sRemoteClientIP) // bStatusChanged UNCHANGED
{
	switch (Stat)
	{
      case SOCKET_STATUS_OFF:
      {
         sStatus="Off";
			StatusEditColour=clBtnFace;
			break;
		}
		case SOCKET_STATUS_CONNECTING:
		{
			sStatus="Connecting...";
			StatusEditColour=clYellow;
			break;
		}
		case SOCKET_STATUS_CONNECTED:
		{
			sStatus="Connected";
			if (!sRemoteClientIP.IsEmpty())
				sStatus+=" ("+sRemoteClientIP+")";
			StatusEditColour=clLime;
			break;
		}
		case SOCKET_STATUS_DISCONNECTED:
		{
			sStatus="Disconnected";
			if (!sRemoteClientIP.IsEmpty())
				sStatus+=" ("+sRemoteClientIP+")";
			StatusEditColour=clWindow;
			break;
		}
		case SOCKET_STATUS_WAITING:
		{
			sStatus="Waiting...";
			StatusEditColour=clYellow;
			break;
		}
		case SOCKET_STATUS_ACCEPT:
		{
			sStatus="Accept";
			StatusEditColour=clYellow;
			break;
		}
		case SOCKET_STATUS_ERROR:
		{
			sStatus="Error";
			if (!sRemoteClientIP.IsEmpty())
				sStatus+=" ("+sRemoteClientIP+")";
			StatusEditColour=clRed;
			break;
		}
		case SOCKET_STATUS_LOOKUP:
		{
			sStatus="Lookup";
			StatusEditColour=clYellow;
			break;
		}
		case SOCKET_STATUS_NO_DATA:
		{
			sStatus="No Data";
			StatusEditColour=clYellow;
			break;
		}
		case SOCKET_STATUS_IDLE_CONN_CLOSED:
		{
			sStatus="Idle Socket Closed";
			if (!sRemoteClientIP.IsEmpty())
				sStatus+=" ("+sRemoteClientIP+")";
			StatusEditColour=clWindow;
			break;
		}
		default:
		{
			StatusEditColour=clBtnFace;
			sStatus="Off";
		}
	}
}


void LJSingleSocketBaseClass::WriteToDebugLog(const String sMsg,const String sIPAddress,const int nPort)
{
	if (pDebugFil&&!DebugFilError)
   {
      JTime T(true);
      String s;
      s.printf(L"%s %-16s:%-5d %s",T.sDateTime(),sIPAddress,nPort,sMsg);
		pDebugFileLock->Acquire();
		DebugFilError=pDebugFil->Open();
		if (!DebugFilError)
			DebugFilError=pDebugFil->Seek(pDebugFil->dwGetSize());
		if (!DebugFilError)
			DebugFilError=pDebugFil->TextLine(s);
		if (!DebugFilError)
			DebugFilError=pDebugFil->Close();
		pDebugFileLock->Release();
	}
}



int LJSingleSocketBaseClass::nGetSocketErrorCount()
{
   int n;
   pStatusLock->Acquire();
   n=nSocketErrorCount;
   pStatusLock->Release();
   return n;
}


String LJSingleSocketBaseClass::sGetError(const TErrorEvent ErrorEvent,const int ErrorCode)
{
   String sErr;
   switch (ErrorEvent)
   {
      case eeGeneral:		sErr="GENERAL ERROR: ";		break;
      case eeSend:		sErr="SEND ERROR: ";    	break;
      case eeReceive:		sErr="RECEIVE ERROR: ";		break;
      case eeConnect:		sErr="CONNECT ERROR: ";		break;
      case eeDisconnect:	sErr="DISCONNECT ERROR: ";	break;
      case eeAccept:		sErr="ACCEPT ERROR: ";		break;
      default: 			sErr="UNKNOWN ERROR: ";
   }
   sErr+=sGetWinError(ErrorCode);
   sErr=sErr.Delete(sErr.Length()-1,2); // remove \r\n
   sErr+=" ("+String(ErrorCode)+")";
   return sErr;
}


