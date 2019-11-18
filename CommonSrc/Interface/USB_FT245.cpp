/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General FT245 USB Interface
 *		PROJECT			C1892
 *		ACTIVITY       1
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP Pro SP2
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      1.00: Initial Version 20-03-2007
************************************************************************/

//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "USB_FT245.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USB_FT245::USB_FT245(const int _nDevice, const int nSerialNum, const bool bVerbose)
{
   pLock=new TCriticalSection;
   hFT=NULL;
   nDevice=_nDevice;
	int nI=-1;
	nSerialFound=-1;
	int nDev;
	String sDevType;
	sDevType.printf(L"%04X",nDevice);
	nDevFound=-1;
	String sDescr=sGetDescr(nDevice);

	DWORD dwDev;
	Status=FT_CreateDeviceInfoList(&dwDev);
	nDev=dwDev;
	if (Status==FT_OK)
		{
      for (int i=0; i<nDev; i++)
         {

         DWORD dwFlags,dwType,dwID,dwLocID;
			char acSerial[32];
			char acDescr[256];
			FT_HANDLE hFT1;
			Status=FT_GetDeviceInfoDetail(i,&dwFlags,&dwType,&dwID,&dwLocID,acSerial,acDescr,&hFT1);
			if (Status==FT_OK)
				{
				if ((dwFlags&0x00000001)==0)  //Skip if device is already open
					{
					acSerial[8]=0;
					AnsiString sBuf=String(acSerial).SubString(1,4);
					sscanf(("0x"+sBuf).c_str(),"%x",&nDevFound);
					if ((sBuf==sDevType)||(nDevice==USB_ANY_DEV))
						{
						sBuf=AnsiString(acSerial).SubString(5,4);
						sscanf(("0x"+sBuf).c_str(),"%x",&nSerialFound);
						if (nSerialNum==-1)
							{
                     //Use any device Serial Number
                     nI=i;    //Use this device
                     i=nDev;  //Stop loop
                     }
                  else
                     {
                     //Check device Serial Number
                     AnsiString sSerial;
                     sSerial.printf("%04X",nSerialNum);
                     if (sBuf==sSerial)
                        {
                        nI=i;    //Use this device
                        i=nDev;  //Stop loop
                        }
                     }
                  }
               }
            }
         }
      }
   if (nI>=0)
      {
      Status=FT_Open(nI,&hFT);
      if (Status==FT_OK)
         Status=FT_ResetDevice(hFT);
      if (Status==FT_OK)
         Status=PurgeAll();
      if (Status!=FT_OK)
         {
         hFT=NULL;
         if (bVerbose)
				ShowError("Error Opening Device","Error in "+sGetDescr(nDevice));
			}
		::SleepEx(100,false);
		}
	else
		{
		hFT=NULL;
		if (bVerbose)
			{
			if (nDevFound>=0)
				{
				if (nSerialFound>=0)
					ShowError("Incorrect Device Serial Number",sGetDescr(nDevice)+" Error");
				else
					ShowError("Required Device: "+sGetDescr(nDevice)+"\nFound Device: "+sGetDescr(nDevFound),"Required Device Not Found");
				}
			else
				ShowError("No Compatible USB Devices Found",sGetDescr(nDevice)+" Error");
			}
		else
			{
			if (nDevFound>=0)
				{
				if (nSerialFound>=0)
					nShowError("Incorrect Device Serial Number",sGetDescr(nDevice)+" Error",10);
				else
					nShowError("Required Device: "+sGetDescr(nDevice)+"\nFound Device: "+sGetDescr(nDevFound),"Required Device Not Found",10);
				}
			else
				nShowError("No Compatible USB Devices Found",sGetDescr(nDevice)+" Error",10);
			}
		}
}

USB_FT245::~USB_FT245()
{
	if (hFT)
		{
		Status=FT_Close(hFT);
		if (Status!=FT_OK)
			{
         }
      }
   delete pLock;
}

String USB_FT245::sGetDriverVersion()
{
	String s;
	DWORD dwDriverVer;
	Status=FT_GetDriverVersion(hFT,&dwDriverVer);
	if (Status==FT_OK)
		s.printf(L"%02d.%02d.%02d",(dwDriverVer>>16)&0xFF,(dwDriverVer>>8)&0xFF,dwDriverVer&0xFF);
	else
		s="Error";
	return s;
}

String USB_FT245::sGetLibraryVersion()
{
	String s;
	DWORD dwVer;
	FT_STATUS Status=FT_GetLibraryVersion(&dwVer);
	if (Status==FT_OK)
		s.printf(L"%02d.%02d.%02d",(dwVer>>16)&0xFF,(dwVer>>8)&0xFF,dwVer&0xFF);
	else
		s="Error Reading DLL Version";
	return s;
}


FT245_Detect::FT245_Detect()
{
	DevList.Clear();
	int nDev;
	FT_STATUS Status=FT_ListDevices(&nDev,NULL,FT_LIST_NUMBER_ONLY);
	if (Status==FT_OK)
		{
		::SleepEx(100,false);
		for (int i=0; i<nDev; i++)
			{
			char acBuf[16];
			Status=FT_ListDevices((PVOID)i,acBuf,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
			if (Status==FT_OK)
				{
				int nDevFound=-1;
				int nSerialFound=-1;
				acBuf[8]=0;
				AnsiString sBuf=String(acBuf).SubString(1,4);
				sscanf(("0x"+sBuf).c_str(),"%x",&nDevFound);
				sBuf=AnsiString(acBuf).SubString(5,4);
				sscanf(("0x"+sBuf).c_str(),"%x",&nSerialFound);
				DevListStruct* pDev=new DevListStruct;
				pDev->nID=nDevFound;
				pDev->nSerial=nSerialFound;
				DevList.nAdd(pDev);
				}
			}
		}
	DevList.Pack();
}

String FT245_Detect::sGetDriverVersion(FT_HANDLE h)
{
	String s;
	FT_STATUS Status;
	bool bOwnHandle;
	if (!h)
		{
		Status=FT_Open(0,&h);
		bOwnHandle=true;
		}
	else
		{
		Status=FT_OK;
		bOwnHandle=false;
		}
	if (Status==FT_OK)
		{
		DWORD dwDriverVer;
		Status=FT_GetDriverVersion(h,&dwDriverVer);
		if (Status==FT_OK)
			s.printf(L"%02d.%02d.%02d",(dwDriverVer>>16)&0xFF,(dwDriverVer>>8)&0xFF,dwDriverVer&0xFF);
		if (bOwnHandle)
			FT_Close(h);
		}
	else
		s="Error Reading Driver Version";
	return s;
}

String FT245_Detect::sGetLibraryVersion()
{
	String s;
	DWORD dwVer;
	FT_STATUS Status=FT_GetLibraryVersion(&dwVer);
	if (Status==FT_OK)
		s.printf(L"%02d.%02d.%02d",(dwVer>>16)&0xFF,(dwVer>>8)&0xFF,dwVer&0xFF);
	else
		s="Error Reading DLL Version";
	return s;
}

String FT245_Detect::sGetDescr(const int nDev, const int nSerial)
{
	IMT_USB_Device Dev;
	return Dev.sGetDescr(nDev,nSerial);
}

String FT245_Detect::sGetDescrFromIndex(const int nDevListIndex)
{
	if (nDevListIndex<DevList.nGetCount())
		return sGetDescr(DevList[nDevListIndex]->nID,-1);
	return "Unknown Device";
}


