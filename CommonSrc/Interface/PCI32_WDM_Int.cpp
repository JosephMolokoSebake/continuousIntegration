//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop


#include "PCI32_WDM_Int.h"
#include "PCI32_WDMioctl.h"
#include "JUtils.h"
#include "JMessageBar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

PCI32_WDM_Int::PCI32_WDM_Int( const BRIDGE_DEVICE _Bridge,
                              const String _sName,
                              const DWORD dwDriverVersion,
                              GUID ClassGuid,
                              const bool _bVerbose)
{
   Bridge=_Bridge;
   SetBridge();
   sName=_sName;
   hDevice=INVALID_HANDLE_VALUE;
   bVerbose=_bVerbose;
   bDeviceOK=false;
   DWORD dwError;
   hDevice=OpenByInterface(&ClassGuid,0,&dwError);
   if (hDevice==INVALID_HANDLE_VALUE)
      {
      if (bVerbose)
         ShowError(L"ERROR opening device: "+sName);
      }
   else
      bDeviceOK=bCheckDriverVersion(dwDriverVersion);
}

PCI32_WDM_Int::~PCI32_WDM_Int()
{
   bUnloadDriver();
}

void PCI32_WDM_Int::SetBridge()
{
   if (Bridge==PCI_9030)
      {
      dwCfgOffs=PLX_9030_LOC_GPIOC;
      dwCfgVal=PLX_9030_GPIO_VAL;
      dwCLK0=PLX_9030_CFGCLK_0;
      dwCLK1=PLX_9030_CFGCLK_1;
      dwDATA0=PLX_9030_CFGDATA_0;
      dwDATA1=PLX_9030_CFGDATA_1;
      dwNCONFIG0=PLX_9030_NCONFIG_0;
      dwNCONFIG1=PLX_9030_NCONFIG_1;
      dwDONE1=PLX_9030_CFGDONE_1;
      }
   else
      {
      dwCfgOffs=PLX_9050_LOC_USER;
      dwCfgVal=PLX_9050_USER_VAL;
      dwCLK0=PLX_9050_CFGCLK_0;
      dwCLK1=PLX_9050_CFGCLK_1;
      dwDATA0=PLX_9050_CFGDATA_0;
      dwDATA1=PLX_9050_CFGDATA_1;
      dwNCONFIG0=PLX_9050_NCONFIG_0;
      dwNCONFIG1=PLX_9050_NCONFIG_1;
      dwDONE1=PLX_9050_CFGDONE_1;
      }
}

void PCI32_WDM_Int::ErrorMessage(const String sMess, const bool bFatal)
{
   if (bVerbose)
      ShowError(sMess,sName+" Error");
   if (bFatal)
      {
      bUnloadDriver();
      if (bVerbose)
         ShowError(L"The Application Will Exit To The Operating System",sFileNameOnly(Application->ExeName)+": Fatal Error Encountered");
      exit(0);
      }
}

bool PCI32_WDM_Int::bUnloadDriver()
{
   bool bOK=true;
	if (hDevice!=INVALID_HANDLE_VALUE)
      {
		if (!CloseHandle(hDevice))
         {
			ErrorMessage(sError.sprintf(L"ERROR: CloseHandle returns %0x", GetLastError()));
         bOK=false;
   		}
		hDevice=INVALID_HANDLE_VALUE;
	   }
   return bOK;
}

void PCI32_WDM_Int::FlushCfgBuffer()
{
   if (dwCfgBufferIndex>0)
      {
      WriteCfg(dwCfgOffs,pdwCfgBuffer,dwCfgBufferIndex);
      dwCfgBufferIndex=0;
      }
}

void PCI32_WDM_Int::Set9054nCONFIG(const bool bnCFG)
{
   DWORD dwVal=dwReadCfg(PLX_9054_LOC_USER);
   //nCONFIG is inverted in H/W
   if (bnCFG)
      dwVal&=PLX_9054_NCONFIG_0;
   else
      dwVal|=PLX_9054_NCONFIG_1;
   WriteCfg(PLX_9054_LOC_USER,dwVal);
}


bool PCI32_WDM_Int::bConfigDone()
{
   if (Bridge==PCI_9054)
      return ((dwReadCfg(PLX_9054_LOC_USER)&dwDONE1)!=0);
   else
      return ((dwReadCfg(dwCfgOffs)&dwDONE1)!=0);
}

void PCI32_WDM_Int::CfgOut(const bool bNCONFIG, const bool bDCLK, const bool bDATA)
{
   DWORD dwCntrl=dwCfgVal;
	if (bNCONFIG)
   	dwCntrl|=dwNCONFIG1;
   else
   	dwCntrl&=dwNCONFIG0;
	if (bDCLK)
   	dwCntrl|=dwCLK1;
   else
   	dwCntrl&=dwCLK0;
	if (bDATA)
   	dwCntrl|=dwDATA1;
   else
   	dwCntrl&=dwDATA0;
   pdwCfgBuffer[dwCfgBufferIndex]=dwCntrl;
   dwCfgBufferIndex++;
   if (dwCfgBufferIndex==CFG_BUFFER_SIZE)
      FlushCfgBuffer();
}

bool PCI32_WDM_Int::bConfigFlex(const String sFile)
{
   TMessageBarForm* pMB=new TMessageBarForm(0);
   pMB->Show();
   pMB->SetCaption(L"Configuring "+sName+" PLD (L"+sFileNameOnly(sFile)+")");
   JFile fil('I');
   JFile::FILE_ERROR Error=fil.Open(sFile);
   DWORD dwSize=0;
   if (!Error) Error=fil.GetSize(dwSize);
   BYTE* pucCfgData=NULL;
   if (!Error)
      {
      pucCfgData=new BYTE[dwSize];
      Error=fil.Read(pucCfgData,dwSize);
      }
   bool bOK;
   if (Error)
      {
      if (bVerbose)
         fil.ShowError(Error);
      bOK=false;
      }
   else
      {
      bOK=true;
      dwCfgBufferIndex=0;
      pdwCfgBuffer=new DWORD[CFG_BUFFER_SIZE];
      CfgOut(true,false,true);
      FlushCfgBuffer();
      //Set nCONFIG low
      if (Bridge==PCI_9054) Set9054nCONFIG(false);
      CfgOut(false,false,true);
      //Keep nCONFIG low for an additional 20 writes ( ~10 microsec on PCI bus)
      int i;
      for (i=0; i<20; i++)
         CfgOut(false,false,true);
      //set nCONFIG high and wait 20 writes (~10 microsec on PCI bus)
      FlushCfgBuffer();
      if (Bridge==PCI_9054) Set9054nCONFIG(true);
      for (i=0; i<20; i++)
         CfgOut(true,false,true);

      if (Bridge==PCI_9030)
         {
         //Check that CONF_DONE has gone low
         FlushCfgBuffer();
         if (bConfigDone())
            {
            bOK=false;
            ErrorMessage(L"PLD Configuration Initialization Failed");
            }
         }

      DWORD dw=0;
      int j;
      while((dw<dwSize)&&(bOK))
         {
         //Write data byte to device
         WORD uData=pucCfgData[dw];
         for (j=0; j<8; j++)
            {
            bool bData=((uData&0x0001)!=0);
            CfgOut(true,false,bData);
            CfgOut(true,true,bData);
            uData>>=1;		//Shift right
            }
         dw++;
         pMB->SetProgress((dw*100)/dwSize);
         }
      //Clock another 10 times to finish config
      for (i=0; i<10; i++)
         {
         CfgOut(true,true,true);
         CfgOut(true,false,true);
         }
      FlushCfgBuffer();
      delete[] pdwCfgBuffer;
      delete[] pucCfgData;

      if (Bridge!=PCI_9050)
         {
         //Check that CONF_DONE has gone high
         if (!bConfigDone())
            {
            bOK=false;
            ErrorMessage(L"PLD Configuration Confirmation Failed");
            }
         else
            {
            if (Bridge==PCI_9054)
               Write(PLX_9054_CONFIG_ADDR,PLX_9054_EN_APEX);
            }
         }
      }
   pMB->Hide();
   delete pMB;
   return bOK;
}

void PCI32_WDM_Int::WriteCfg(const DWORD dwOffs, DWORD dwData)
{
   WriteCfg(dwOffs,&dwData,1);
}

void PCI32_WDM_Int::WriteCfg(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum)
{
   DWORD dwSize=2+dwNum;
   DWORD* pdwWr=new DWORD[dwSize];
   pdwWr[PCI32_WDM_INDEX_OFFS]=dwOffs;
   pdwWr[PCI32_WDM_NUM_OFFS]=dwNum;
   memcpy(&pdwWr[PCI32_WDM_DATA_OFFS],pdwData,dwNum*sizeof(DWORD));
	ULONG	dwOutNum;
	if (!DeviceIoControl(hDevice,
						      PCI32_WDM_CFG_WR,
						      pdwWr,
						      dwSize*sizeof(DWORD),
						      NULL,
						      0,
						      &dwOutNum,
						      NULL)){
      delete[] pdwWr;
		ErrorMessage(sError.sprintf(L"ERROR: DeviceIoControl PCI32_WDM_CFG_WR returns %0x.",::GetLastError()),true);
      }
   else
      delete[] pdwWr;
}

DWORD PCI32_WDM_Int::dwReadCfg(const DWORD dwOffs)
{
   DWORD adwRd[2];
   adwRd[PCI32_WDM_INDEX_OFFS]=dwOffs;
   adwRd[PCI32_WDM_NUM_OFFS]=1;
	DWORD	dwReadSize,dwData;
	if (!DeviceIoControl(hDevice,
						      PCI32_WDM_CFG_RD,
						      &adwRd,
						      sizeof(adwRd),
						      &dwData,
						      sizeof(dwData),
						      &dwReadSize,
						      NULL)){
		ErrorMessage(sError.sprintf(L"ERROR: DeviceIoControl PCI32_WDM_CFG_RD returns %0x.",::GetLastError()),true);
	   }
   else if (dwReadSize!=sizeof(dwData))
      ErrorMessage(L"Read returns more than one dword",true);
   else
      return dwData;
   return 0;
}

void PCI32_WDM_Int::Write(const DWORD dwOffs, DWORD dwData)
{
   Write(dwOffs,&dwData,1);
}

void PCI32_WDM_Int::Write(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum)
{
   DWORD dwSize=2+dwNum;
   DWORD* pdwWr=new DWORD[dwSize];
   pdwWr[PCI32_WDM_INDEX_OFFS]=dwOffs;
   pdwWr[PCI32_WDM_NUM_OFFS]=dwNum;
   memcpy(&pdwWr[PCI32_WDM_DATA_OFFS],pdwData,dwNum*sizeof(DWORD));
	ULONG	dwOutNum;
	if (!DeviceIoControl(hDevice,
						      PCI32_WDM_IO_WR,
						      pdwWr,
						      dwSize*sizeof(DWORD),
						      NULL,
						      0,
						      &dwOutNum,
						      NULL))
      {
      delete[] pdwWr;
		ErrorMessage(sError.sprintf(L"ERROR: DeviceIoControl PCI32_WDM_IO_WR returns %0x.",::GetLastError()),true);
      }
   else
      delete[] pdwWr;
}

DWORD PCI32_WDM_Int::dwRead(const DWORD dwOffs)
{
   DWORD dwData;
   Read(dwOffs,&dwData,1);
   return dwData;
}

void PCI32_WDM_Int::Read(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum)
{
   DWORD adwRd[2];
   adwRd[PCI32_WDM_INDEX_OFFS]=dwOffs;
   adwRd[PCI32_WDM_NUM_OFFS]=dwNum;
	DWORD	dwReadSize;
	if (!DeviceIoControl(hDevice,
						      PCI32_WDM_IO_RD,
						      &adwRd,
						      sizeof(adwRd),
						      pdwData,
						      dwNum*sizeof(DWORD),
						      &dwReadSize,
						      NULL))
      {
		ErrorMessage(sError.sprintf(L"ERROR: DeviceIoControl PCI32_WDM_IO_RD returns %0x.",::GetLastError()),true);
	   }
   else if (dwReadSize!=dwNum*sizeof(DWORD))
      ErrorMessage(L"Invalid number of dwords returned by Read",true);
}

bool PCI32_WDM_Int::bCheckDriverVersion(const DWORD dwDriverVersion)
{
	bool bVersionOK=false;
	DWORD	dwVersion,dwReadSize;
	if (!DeviceIoControl(hDevice,
						      PCI32_WDM_VERSION,
						      NULL,
						      0,
						      &dwVersion,
						      sizeof(dwVersion),
						      &dwReadSize,
						      NULL))
      {
		ErrorMessage(sError.sprintf(L"ERROR: DeviceIoControl PCI32_WDM_VERSION returns %0x.",::GetLastError()),true);
	   }
   else
      {
		bVersionOK=(dwVersion==dwDriverVersion);
      if (!bVersionOK)
   		ErrorMessage(sError.sprintf(L"Fatal Error: Invalid Driver Version\nRequired Version = %u, Version Found = %u",dwDriverVersion,dwVersion),true);
   	}
   return bVersionOK;
}

HANDLE PCI32_WDM_Int::OpenByInterface(
		GUID* pClassGuid,	// points to the GUID that identifies the interface class
		DWORD instance,	// specifies which instance of the enumerated devices to open
		PDWORD pError		// address of variable to receive error status
		)
{
	HANDLE hDev;
	CDeviceInterfaceClass DevClass(pClassGuid, pError);
	if (*pError!=ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;
	CDeviceInterface DevInterface(&DevClass, instance, pError);
	if (*pError!=ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;
	hDev=CreateFile(
		DevInterface.DevicePath(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hDev==INVALID_HANDLE_VALUE)
		*pError=GetLastError();
	return hDev;
}


