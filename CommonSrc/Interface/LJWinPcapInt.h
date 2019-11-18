//---------------------------------------------------------------------------

#ifndef LJWinPcapIntH
#define LJWinPcapIntH

#define WPCAP
#define HAVE_REMOTE     // to access newest WinPcap features
#include "pcap.h"
#include "JList.h"
#include "JTime.h"
#include "JFile.h"
#include <SyncObjs.hpp>

#define DAYS_FROM_1970_TO_1990 7304	// 1 January 1970 to and NOT including 1 January 1990

//---------------------------------------------------------------------------

JTime GetPacketTime_UTC(const struct pcap_pkthdr* pPcapHeader)
{
	JTime T;
	T.SetTotalSec(pPcapHeader->ts.tv_sec-(DAYS_FROM_1970_TO_1990*86400));
	T.AddSec(pPcapHeader->ts.tv_usec * 0.000001);
	T.SetUTC();
	return T;
}


class LJWinPcapDataClass
{
	int nSize;
	JTime T_UTC;
	BYTE* pucData;

public:
	BYTE* pGetData() const
	{
		return pucData;
	}

	int nGetDataSize() const
	{
		return nSize;
   }

	void Assign(const LJWinPcapDataClass& _C)
	{
		nSize=_C.nSize;
		T_UTC=_C.T_UTC;
		delete[] pucData;
		pucData=new BYTE[nSize];
		memcpy(pucData,_C.pucData,nSize);
	}

	LJWinPcapDataClass& operator = (const LJWinPcapDataClass& _C)
	{
      Assign(_C);
		return *this;
	};

	LJWinPcapDataClass(const LJWinPcapDataClass& _C)  // Copy constructor
	{
		*this=_C;
	};

	void Invalidate()
	{
		nSize=0;
		delete[] pucData;
		pucData=NULL;
		nSize=0;
	}

	LJWinPcapDataClass()
	{
		pucData=NULL;
		Invalidate();
	}

	void SetData(const struct pcap_pkthdr* pPcapHeader,const BYTE* pucPcapData)
	{
		T_UTC=GetPacketTime_UTC(pPcapHeader);
		pucData=new BYTE[pPcapHeader->caplen];
		memcpy(pucData,pucPcapData,pPcapHeader->caplen);
		nSize=pPcapHeader->caplen;
	}

	LJWinPcapDataClass(const struct pcap_pkthdr* pPcapHeader,const BYTE* pucPcapData)
	{
		SetData(pPcapHeader,pucPcapData);
	}

	~LJWinPcapDataClass()
	{
		delete[] pucData;
	}
};


class LJWinPcapIntClass : TThread
{
	pcap_if_t* pPcap_Devices;
	pcap_if_t* pPcap_Current_Device;
	pcap_t* pPcap_Adapter_Handle;
	char acErrorBuf[PCAP_ERRBUF_SIZE];
	int nAdaptersFound;
	String sVersion;
	void FreeAllDevices();
	void Reset();
	bool bTestCapture;
   JFile* pDebFil;
	bool bAbortCaptureThread;
	void __fastcall Execute();
	void __fastcall Terminate();

	String sCompileFilter(const String sFilter); //,const bool bUDP,const bool bTCP,const bool bInbound,const bool bOutbound);
	String sParseFilter(const String sFilter);
	void StorePacket(const struct pcap_pkthdr* pPcapHeader, const BYTE* pucData);

	void StorePacketTmpLst(const struct pcap_pkthdr* pPcapHeader, const BYTE* pucData);; // for testing purposes only

	// Packet Storage Related
	CRITICAL_SECTION DataLock;
	JList<String>sTmpLst; // for testing purposes only
	JList<LJWinPcapDataClass>MsgFifo;


/* ******************** PREVENT CLASS COPY ********************************** */
	LJWinPcapIntClass& operator = (const LJWinPcapIntClass& _S);
	LJWinPcapIntClass(const LJWinPcapIntClass& _S); // Copy constructor
/* ******************** PREVENT CLASS COPY ********************************** */

public:
	int nGetFriendlyDeviceNames(TStringList* pslDescr);
	bool bGetDevices(TStringList* pslDev=NULL,TStringList* pslDevDescr=NULL);
	bool bSelectAdapter(const int nAdapter);
	bool bSetFilter(const String sFilterIPs);
	bool bStartCapture();
	void StopCapture();
	int nGetPacketCount()
	{
		int n=0;
		EnterCriticalSection(&DataLock);
		n=MsgFifo.nGetCount();
		LeaveCriticalSection(&DataLock);
		return n;
	}

	bool bGetNextPacket(LJWinPcapDataClass* pPacket)
	{
		bool bOK=false;
		pPacket->Invalidate();
		EnterCriticalSection(&DataLock);
		if (MsgFifo.nGetCount()>0)
		{
			pPacket->Assign(*MsgFifo.pFirst());
			MsgFifo.GoFirst();
			MsgFifo.Delete();
			bOK=true;
		}
		LeaveCriticalSection(&DataLock);
		return bOK;
   }

	String GetWinPcapVersion()
	{
		return sVersion;
	}

	bool bStartTestCapture(); // for testing purposes only
	String sGetDataTmpLst() // for testing purposes only
	{
		EnterCriticalSection(&DataLock);
		String s="";
		if (sTmpLst.nGetCount()>0)
		{
			//s=String(sTmpLst.nGetCount())+" *******************\r\n";
			sTmpLst.GoFirst();
			while (!sTmpLst.bLast())
			{
				String* ps=sTmpLst.pNext();
				if (ps)
					if (!ps->IsEmpty())
						s += (*ps) + "\r\n";
			}
		}
		sTmpLst.Clear();
		LeaveCriticalSection(&DataLock);
		return s;
	}

	__fastcall LJWinPcapIntClass();
   __fastcall ~LJWinPcapIntClass();

};
#endif
