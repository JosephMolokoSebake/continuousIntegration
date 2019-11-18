//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "FT2232H.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


FT2232H::FT2232H(const int nDevice, const int nSerial, const char cPort)
{
	nDev=nDevice;
	hFT=NULL;
	nDevSerial=0;
	DWORD numDevs;
	FT_CreateDeviceInfoList(&numDevs);
	if (bOpenDevice(nDevice,nSerial,cPort))
		{
		if (Status==FT_OK)
			Status=::FT_ResetDevice(hFT);
		if (Status==FT_OK)
			Status=::FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
		if (Status!=FT_OK)
			hFT=NULL;
		::SleepEx(100,false);
		}
	if (!hFT)
		nShowError("Could not initialise USB interface",sGetDevDescr()+" Error",30);
}

FT2232H::~FT2232H()
{
	Close();
}

bool FT2232H::bOpenDevice(const int nDevice, const int nSerial, const char cPort)
{
	AnsiString saDev;
	saDev.printf("%04X",nDevice);
	AnsiString saFound="";
	DWORD dwDevCnt;
	Status=::FT_ListDevices(&dwDevCnt,NULL,FT_LIST_NUMBER_ONLY);
	if (Status==FT_OK)
		{
		DWORD dw;
		for (dw=0; (dw<dwDevCnt)&&(saFound.IsEmpty()); dw++)
			{
			char acBuf[64];
			Status=::FT_ListDevices((PVOID)dw,acBuf,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
			if (Status==FT_OK)
				{
				acBuf[63]=0;	//Make sure it is zero terminated
				saFound=AnsiString(acBuf);
				AnsiString saDevFound=saFound.SubString(1,4);
				AnsiString saSerialFound=saFound.SubString(5,4);
				try
					{
					nDevSerial=saSerialFound.ToInt();
					}
				catch(...)
					{

					}
				if (saDevFound==saDev)
					{
					if (nSerial>=0)
						{
						AnsiString saSerial;
						saSerial.printf("%04X",nSerial);
						if (saSerialFound!=saSerial)
							saFound="";
						}
					if (!saFound.IsEmpty())
						{
						if (saFound.Length()>0)
							saFound[saFound.Length()]=cPort;
						else
							saFound="";
						}
					}
				else
					saFound="";
				}
			}
		}
	if (!saFound.IsEmpty())

   //
		{
		char szDev[64];
		strcpy(szDev,saFound.c_str());
		Status=::FT_OpenEx((PVOID)szDev,FT_OPEN_BY_SERIAL_NUMBER,&hFT);
		if (Status==FT_OK)
			return true;
		else
			hFT=NULL;
		}
	return false;
}

bool FT2232H::bSetMode(const FT_MODE Mode, const BYTE ucIOMask)
{
	BYTE ucLatencyTimer = 2;
	BYTE ucMode=RESET_MODE;
	Status=::FT_SetBitMode(hFT,ucIOMask,ucMode);	//Reset Mode
		Status=::FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
	::SleepEx(10,false);
	if (Status==FT_OK)
		{
		ucMode=Mode;
//      BYTE ucMask=0x00;
		switch(Mode)
			{
			case SYNC_FIFO_MODE:
				Status=::FT_SetBitMode(hFT,ucIOMask,ucMode);
				if (Status==FT_OK)
					Status=::FT_SetLatencyTimer(hFT,ucLatencyTimer);
				if (Status==FT_OK)
					Status=::FT_SetUSBParameters(hFT,0x10000,0x10000);
				if (Status==FT_OK)
					Status=::FT_SetFlowControl(hFT,FT_FLOW_RTS_CTS,0,0);
				if (Status!=FT_OK)
					nShowError("Could Not Set USB SyncFIFO Mode",sGetDevDescr()+" Error",30);
				break;
			case ASYNC_BITBANG_MODE:
			case SYNC_BITBANG_MODE:
				Status=::FT_SetBitMode(hFT,ucIOMask,ucMode);
				if (Status==FT_OK)
					Status=::FT_SetBaudRate(hFT,230400);	//57600x16 = 1.08us update period
				if (Status==FT_OK)
					Status=::FT_SetFlowControl(hFT,FT_FLOW_NONE,0,0);
				if (Status!=FT_OK)
					nShowError("Could Not Set USB BitBang Mode",sGetDevDescr()+" Error",30);
				break;
			case RESET_MODE:
				//Already reset
				break;
			default:	;
			}
		}
	else
		nShowError("Could Not Reset USB Mode",sGetDevDescr()+" Error",30);
	if (Status==FT_OK)
		Status=::FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
	return (Status==FT_OK);
}

String FT2232H::sGetDriverVersion()
{
	String s="Error";
	DWORD dwDriverVer;
	Status=FT_GetDriverVersion(hFT,&dwDriverVer);
	if (Status==FT_OK)
		s.printf(L"%02d.%02d.%02d",(dwDriverVer>>16)&0xFF,(dwDriverVer>>8)&0xFF,dwDriverVer&0xFF);
	return s;
}

String FT2232H::sGetLibraryVersion()
{
	String s="Error";
	DWORD dwVer;
	FT_STATUS Status=::FT_GetLibraryVersion(&dwVer);
	if (Status==FT_OK)
		s.printf(L"%02d.%02d.%02d",(dwVer>>16)&0xFF,(dwVer>>8)&0xFF,dwVer&0xFF);
	return s;
}

bool FT2232H::bWriteAllData(void* pData, const int nSize)
{
	DWORD dwRXBytes,dwTXBytes,dwEventDWord,dwBytesWritten;
	BYTE* pucData=(BYTE*)pData;
	DWORD dwOffs=0;
	do
		{
		Status=::FT_GetStatus(hFT,&dwRXBytes,&dwTXBytes,&dwEventDWord);
		}
	while((Status==FT_OK)&&(dwTXBytes>0));
	if (Status==FT_OK)
		{
		do
			{
			Status=::FT_Write(hFT,&pucData[dwOffs],nSize-dwOffs,&dwBytesWritten);
			dwOffs+=dwBytesWritten;
			}
		while((Status==FT_OK)&&(dwOffs<(DWORD)nSize));
		}
	return (Status==FT_OK);
}

bool FT2232H::bWriteData(void* pData, const int nSize)
{
	DWORD dwRXBytes,dwTXBytes,dwEventDWord,dwBytesWritten;
	do
		{
		Status=::FT_GetStatus(hFT,&dwRXBytes,&dwTXBytes,&dwEventDWord);
		}
	while((Status==FT_OK)&&(dwTXBytes>0));
	if (Status==FT_OK)
		Status=::FT_Write(hFT,pData,nSize,&dwBytesWritten);
	return (Status==FT_OK);
}

bool FT2232H::bWriteData(const int nCmd, void* pData, const int nSize)
{
	BYTE* pucData=(BYTE*)pData;
	BYTE* pucTX=new BYTE[nSize*2+9];	//Make provision for byte stuffing and cmd+cmd+3x size
	int nS=0;
	pucTX[nS++]=CMD_STUFF_BYTE;
	pucTX[nS++]=nCmd;
	if (nCmd==CMD_STUFF_BYTE)
		pucTX[nS++]=CMD_STUFF_BYTE;	//Stuff cmd byte
	BYTE uc=(BYTE)(nSize&0x000000FF);
	pucTX[nS++]=uc;
	if (uc==CMD_STUFF_BYTE)
		pucTX[nS++]=CMD_STUFF_BYTE;	//Stuff cmd byte
	uc=(BYTE)((nSize>>8)&0x000000FF);
	pucTX[nS++]=uc;
	if (uc==CMD_STUFF_BYTE)
		pucTX[nS++]=CMD_STUFF_BYTE;	//Stuff cmd byte
	uc=(BYTE)((nSize>>16)&0x000000FF);
	pucTX[nS++]=uc;
	if (uc==CMD_STUFF_BYTE)
		pucTX[nS++]=CMD_STUFF_BYTE;	//Stuff cmd byte
	for (int i=0; i<nSize; i++)
		{
		pucTX[nS++]=pucData[i];
		if (pucData[i]==CMD_STUFF_BYTE)
			pucTX[nS++]=CMD_STUFF_BYTE;
		}
	bool bOK=bWriteData(pucTX,nS);
	delete[] pucTX;
	return bOK;
}


int FT2232H::nReadData(void* pucData, const int nMaxSize)
{
	DWORD dwRXBytes,dwTXBytes,dwEventDWord,dwBytesReceived=0;
	Status=::FT_GetStatus(hFT,&dwRXBytes,&dwTXBytes,&dwEventDWord);
	if (Status==FT_OK)
		{
		if (dwRXBytes>0)
			{
			if (dwRXBytes>(DWORD)nMaxSize)
				dwRXBytes=nMaxSize;
			Status=::FT_Read(hFT,pucData,dwRXBytes,&dwBytesReceived);
/*
			if (bStreaming)
				{
				for (int i=0; i<(int)dwBytesReceived; i++)
					{
					if (((BYTE*)pucData)[i]!=(BYTE)(nTest&0x000000FF))
						{
						nTest=((BYTE*)pucData)[i];
						}
					++nTest;
					}
				}
*/

			if (Status==FT_OK)
				return dwBytesReceived;
			}
		}
	return 0;
}

String FT2232H::sGetDevDescr()
{
	IMT_USB_Device Dev;
	return Dev.sGetDescr(nDev,0);
}


