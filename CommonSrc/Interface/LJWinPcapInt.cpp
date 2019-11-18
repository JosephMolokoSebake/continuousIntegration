//---------------------------------------------------------------------------
#include <jpch.h>

#pragma hdrstop

#include "LJWinPcapInt.h"
#include "LJUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall LJWinPcapIntClass::LJWinPcapIntClass() : TThread(true)
{
	bTestCapture=false;
	pPcap_Devices=NULL;
	pPcap_Current_Device=NULL;
	pPcap_Adapter_Handle=NULL;
	memset(acErrorBuf,0,PCAP_ERRBUF_SIZE);
	Reset();
	nAdaptersFound=-1;
	sVersion="";
	bAbortCaptureThread=true;
	InitializeCriticalSection(&DataLock);
	MsgFifo.Clear();
	pDebFil=NULL;
#ifdef _DEBUG
	pDebFil=new JFile('O',JFile::ASCII_TYPE);
	pDebFil->Create("Debug LJWinPcapInt.txt");
#endif

}

__fastcall LJWinPcapIntClass::~LJWinPcapIntClass()
{
	bAbortCaptureThread=true;
	Terminate();
	Reset();
	DeleteCriticalSection(&DataLock);
	MsgFifo.Clear();
	if (pDebFil)
	   delete pDebFil;
}

void LJWinPcapIntClass::FreeAllDevices()
{
	pcap_freealldevs(pPcap_Devices);
}

void LJWinPcapIntClass::Reset()
{
	FreeAllDevices();
	pPcap_Devices=NULL;
	pPcap_Current_Device=NULL;
	pPcap_Adapter_Handle=NULL;
}

int LJWinPcapIntClass::nGetFriendlyDeviceNames(TStringList* pslDescr)
{
	int n=0;
	if (pslDescr != NULL)
	{
		pslDescr->Clear();
		TStringList* pslDev=new TStringList;
		if (bGetDevices(pslDev,pslDescr))
		{
			n=nAdaptersFound;
			for (int i=0;i<nAdaptersFound;i++)
			{
				String s=pslDescr->Strings[i];
				if (s == "No Description Available")
					s=pslDev->Strings[i];
				int nPos=s.Pos("rpcap://");
				if (nPos > 0)
					s=s.Delete(1,8);
				nPos=s.Pos("Network adapter '");
				if (nPos > 0)
					s=s.Delete(1,17);
				nPos=s.Pos("' on local host");
				if (nPos > 0)
					s=s.Delete(nPos,15);
				pslDescr->Strings[i]=s;
			}
		}
	}
	return n;
}

bool LJWinPcapIntClass::bGetDevices(TStringList* pslDev,TStringList* pslDevDescr)
{
	nAdaptersFound=0;
	FreeAllDevices();
	if (pslDev)
		pslDev->Clear();
	if (pslDevDescr)
		pslDevDescr->Clear();
	// Retrieve the device list from the local machine
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &pPcap_Devices, acErrorBuf) == -1)
	{
		nShowErrorMsg(String(acErrorBuf),"WinPcap Device Listing Failed");
		Reset();
		return false;
	}
	for (pPcap_Current_Device = pPcap_Devices; pPcap_Current_Device!=NULL; pPcap_Current_Device=pPcap_Current_Device->next)
	{
		nAdaptersFound++;
		if (pslDev)
			pslDev->Add(String(pPcap_Current_Device->name));
		if (pslDevDescr)
		{
			if (pPcap_Current_Device->description)
				pslDevDescr->Add(String(pPcap_Current_Device->description));
			else
				pslDevDescr->Add("No Description Available");
		}
	}
	if (nAdaptersFound == 0)
	{
		nShowErrorMsg("No Interfaces Found. Make Sure WinPcap Is Installed.","WinPCap Device Listing Failed");
		Reset();
		return false;
	}
	sVersion=String(pcap_lib_version());
	return true;
}

bool LJWinPcapIntClass::bSelectAdapter(const int nAdapter)
{
	if (nAdaptersFound==-1) // bGetDevices not yet executed
      bGetDevices();
	if (nAdaptersFound>0 && nAdapter>=0 && nAdapter<nAdaptersFound && pPcap_Devices!=NULL)
	{
		//Jump to the selected adapter
      int i;
		for (pPcap_Current_Device=pPcap_Devices,i=0;i<nAdapter;pPcap_Current_Device=pPcap_Current_Device->next,i++);

		// Open the device
		if ((pPcap_Adapter_Handle=pcap_open(pPcap_Current_Device->name,          // name of the device
								65536,            // portion of the packet to capture
														// 65536 guarantees that the whole packet will be captured on all the link layers
								PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
								1000,             // read timeout
								NULL,             // authentication on the remote machine
								acErrorBuf        // error buffer
								) ) == NULL)
		{
			nShowErrorMsg("Unable to open the adapter. "+String(pPcap_Current_Device->name)+" is not supported by WinPcap.","WinPCap Adapter Error");
			Reset();
			return false;
		}
		else
		{
#ifdef _DEBUG
			nShowInfoMsg(String(pPcap_Current_Device->description)+" selected.");
#endif
			return true;
		}
	}
	else
	{
		nShowErrorMsg("Invalid Adapter Selected","WinPCap Adapter Selection Failed");
		return false;
	}
}

String LJWinPcapIntClass::sCompileFilter(const String sFilter) //,const bool bUDP,const bool bTCP,const bool bInbound,const bool bOutbound)
{
	if (sFilter.IsEmpty())// && bUDP && bTCP && bInbound && bOutbound)
		return "";
	else
	{
		// ******************************************************
		// UDP/TCP/Inbound/Outbound filtering NOT YET IMPLEMENTED
		// ******************************************************
		String sParsedFilter="";
		//ShowMessage("UDP ONLY!!!");
		//sParsedFilter+="ip proto \\udp and dst port 3455";
		//sParsedFilter+="ip proto \\udp and ip host "+sParseFilter(sFilter);// and ip host "+sParseFilter(sFilter);
		sParsedFilter+="ip host "+sParseFilter(sFilter);
		return sParsedFilter;
	}
}

String LJWinPcapIntClass::sParseFilter(const String sFilter)
{
	String sParsedFilter=sFilter;
	while (sParsedFilter.Pos(";") > 0)
	{
		int n=sParsedFilter.Pos(";");
		sParsedFilter=sParsedFilter.Delete(n,1);
		sParsedFilter=sParsedFilter.Insert(" or ",n);
	}
	while (sParsedFilter.Pos(",") > 0)
	{
		int n=sParsedFilter.Pos(",");
		sParsedFilter=sParsedFilter.Delete(n,1);
		sParsedFilter=sParsedFilter.Insert(" or ",n);
	}
	return sParsedFilter;
}


bool LJWinPcapIntClass::bSetFilter(const String sFilterIPs) //,const bool bUDP,const bool bTCP,const bool bInbound,const bool bOutbound)
{
	if (pPcap_Current_Device != NULL && pPcap_Adapter_Handle != NULL)
	{
		bpf_int32 nNetMask;
		if (pPcap_Current_Device->addresses != NULL) // Retrieve the mask of the first address of the interface
		  nNetMask=((struct sockaddr_in *)(pPcap_Current_Device->addresses->netmask))->sin_addr.S_un.S_addr;
		else
		  nNetMask=0xffffff; // If the interface is without an address we suppose it to be in a C class network

		nNetMask=0xffffff;

		// Compile the filter
		struct bpf_program fcode;
		String sFilter=sCompileFilter(sFilterIPs);//,bUDP,bTCP,bInbound,bOutbound);
		if (pcap_compile(pPcap_Adapter_Handle,&fcode,AnsiString(sFilter).c_str(),1,nNetMask) < 0)
		{
			nShowErrorMsg("Unable to compile the packet filter.","WinPCap Filter Setup Error");
			Reset();
			return false;
		}
		if (pcap_setfilter(pPcap_Adapter_Handle,&fcode) < 0) // Set the filter
		{
			nShowErrorMsg("Error setting the filter.","WinPCap Filter Setup Error");
			Reset();
			return false;
		}
	}
	else
		return false;
	return true;

}

bool LJWinPcapIntClass::bStartTestCapture()
{
	bTestCapture=true;
	return bStartCapture();
}

bool LJWinPcapIntClass::bStartCapture()
{
	if (pPcap_Adapter_Handle != NULL)
	{
		if (bTestCapture)
			nShowInfoMsg("Starting Test Capture Mode!!","Test Capture");
		TThread::Resume();
		return true;
	}
	else
		return false;
}


void LJWinPcapIntClass::StopCapture()
{
	Terminate();
}


 // **************************************************************************************************
// !!! NB FOR TESTING PURPOSES ONLY !!!
void LJWinPcapIntClass::StorePacketTmpLst(const struct pcap_pkthdr* pPcapHeader, const BYTE* /*pucData*/)
{
	if (!TThread::Terminated)
	{
		struct tm ltime;
		char timestr[16];
		time_t local_tv_sec;

		EnterCriticalSection(&DataLock);
		JTime T=GetPacketTime_UTC(pPcapHeader);
		String s;
		s.printf(L"%s caplen %d len %d",T.sDateTime(false,true),pPcapHeader->caplen,pPcapHeader->len);
		String* ps=new String(s);
		sTmpLst.nAdd(ps);
		LeaveCriticalSection(&DataLock);
	}
}
// !!! NB FOR TESTING PURPOSES ONLY !!!
 // **************************************************************************************************


void LJWinPcapIntClass::StorePacket(const struct pcap_pkthdr* pPcapHeader, const BYTE* pucData)
{
	if (!TThread::Terminated)
	{
		EnterCriticalSection(&DataLock);
		// store raw data
		LJWinPcapDataClass* pP=new LJWinPcapDataClass();
		pP->SetData(pPcapHeader,pucData);
		MsgFifo.nAdd(pP);
		LeaveCriticalSection(&DataLock);
	}
}


void __fastcall LJWinPcapIntClass::Execute()
{
	int nRet;
	struct pcap_pkthdr* pHeader;
	const u_char* pPkt_data;
	while (!TThread::Terminated)
	{
		if (pPcap_Adapter_Handle!=NULL)
		{
			if ((nRet = pcap_next_ex(pPcap_Adapter_Handle,&pHeader,&pPkt_data)) >= 0)  // retrieve next packet
			{
				switch (nRet)
				{
					case -2:
					{
						nShowErrorMsg("EOF was reached reading from an offline capture: "+String(pcap_geterr(pPcap_Adapter_Handle)),"WinPCap Capture Error");
						Terminate();
						break;
					}
					case -1:
					{
						nShowErrorMsg("Error reading the packets: "+String(pcap_geterr(pPcap_Adapter_Handle)),"WinPCap Capture Error");
						Terminate();
						break;
					}
					case 0: // timeout elapsed
						/*if (pDebFil)
							pDebFil->TextLine("Timeout Elapsed");*/
						break;
					case 1:
					{
						if (!bTestCapture)
							StorePacket(pHeader,pPkt_data);
						else
							StorePacketTmpLst(pHeader,pPkt_data);
						/*
						if (pDebFil)
						{
							static int nCnt=0;
							static int nTotalBytes=0;
							nTotalBytes+=(pHeader->caplen-42);
							String s;
							s.printf(L"%d\t%d\t%d",++nCnt,nTotalBytes,pHeader->caplen-42);
							pDebFil->TextLine(s);
						}*/
						break;
					}
					default: ;
				}
			}
		}
	}
}

void __fastcall LJWinPcapIntClass::Terminate()
{
   TThread::Terminate();                            
}


