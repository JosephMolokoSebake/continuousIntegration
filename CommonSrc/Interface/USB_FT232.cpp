//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "USB_FT232.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USB_FT232::USB_FT232(const int _nBaud, const char _ucParity, const int _nDataBits, const int _nStopBits, const int _nDevice, const int nSerialNum)
{
   ::InitializeCriticalSection(&CriticalSection);
   hFT=NULL;

   nDevice=_nDevice;
   String sDevType;
   sDevType.printf(L"%04X",nDevice);
   bool bFTFound=false;
   bool bDevOK=false;
   int nDev;
   int nDevFound=-1;
   int nI=-1;
   String sDescr=sGetDescr(nDevice);
   Status=FT_ListDevices(&nDev,NULL,FT_LIST_NUMBER_ONLY);
   if (Status==FT_OK)
      {
      for (int i=0; i<nDev; i++)
         {
         bFTFound=true;
         char acBuf[16];
         Status=FT_ListDevices((PVOID)i,acBuf,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
         if (Status==FT_OK)
            {
            acBuf[8]=0;
            String sBuf=String(acBuf).SubString(1,4);
            swscanf((L"0x"+sBuf).c_str(),L"%x",&nDevFound);
            if ((sBuf==sDevType)||(nDevice==USB_ANY_DEV))
               {
               bDevOK=true;   //Correct Device Type
               if (nSerialNum==-1)
                  {
                  //Use any device Serial Number
                  nI=i;    //Use this device
                  i=nDev;  //Stop loop
                  }
               else
                  {
                  //Check device Serial Number
                  String sSerial;
                  sSerial.printf(L"%04X",nSerialNum);
                  sBuf=String(acBuf).SubString(5,4);
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

   bool bVerbose=true;      

   if (nI>=0)
      {
      Status=FT_Open(nI,&hFT);
      if (Status==FT_OK)
         Status=FT_ResetDevice(hFT);
      if (Status==FT_OK)
         Setup(_nBaud,_ucParity,_nDataBits,_nStopBits);
      if (Status!=FT_OK)
         {
         hFT=NULL;
         if (bVerbose)
            ShowError(L"Error Opening Device",L"Error in "+sDescr);
         }
      ::SleepEx(100,false);
      }
   else
      {
      hFT=NULL;
      if (bVerbose)
         {
         if (bFTFound)
            {
            if (bDevOK)
               ShowError(L"Incorrect Device Serial Number",sDescr+" Error");
            else
               {
               String sFound=sGetDescr(nDevFound);
               ShowError(L"Required Device: "+sDescr+"\nFound Device: "+sFound,L"Required Device Not Found");
               }
            }
         else
            ShowError(L"No Compatible USB Devices Found",sDescr+" Error");
         }
      }
}

USB_FT232::~USB_FT232()
{
   if (hFT)
      {
      Status=FT_Close(hFT);
      if (Status!=FT_OK)
         {
         }
      }
   ::DeleteCriticalSection(&CriticalSection);
}

void USB_FT232::Setup(const int _nBaud, const char _ucParity, const int _nDataBits, const int _nStopBits)
{
   if (hFT)
      {
//      DWORD dwDiv=0;
//      Status=FT_SetDivisor(hFT,dwDiv);
      Status=FT_SetBaudRate(hFT,_nBaud);
      if (Status==FT_OK)
         {
         BYTE ucLen,ucStop,ucParity;
         switch(_ucParity)
            {
            case 'E':   ucParity=FT_PARITY_EVEN;   break;
            case 'O':   ucParity=FT_PARITY_ODD;    break;
            case 'M':   ucParity=FT_PARITY_MARK;   break;
            case 'S':   ucParity=FT_PARITY_SPACE;  break;
            default:
               ucParity=FT_PARITY_NONE;
            }
         switch(_nDataBits)
            {
            case 7:  ucLen=FT_BITS_7;   break;
            default: ucLen=FT_BITS_8;
            }
         switch(_nStopBits)
            {
            case 2:  ucStop=FT_STOP_BITS_2;   break;
            default: ucStop=FT_STOP_BITS_1;
            }
         Status=FT_SetDataCharacteristics(hFT,ucLen,ucStop,ucParity);
         }
      if (Status==FT_OK)
         Status=FT_SetFlowControl(hFT,FT_FLOW_NONE,0,0);
      if (Status==FT_OK)
         Status=FT_ClrRts(hFT);
      if (Status==FT_OK)
         Status=FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
      if (Status!=FT_OK)
         {
         }
      }
}

int USB_FT232::nRXData(BYTE* pucData, const int nMaxSize)
{
   if (hFT)
      {
      DWORD dwToRX,dwRXed=0;
      Status=FT_GetQueueStatus(hFT,&dwToRX);
      if (Status==FT_OK)
         {
         if ((int)dwToRX>nMaxSize)
            dwToRX=nMaxSize;
         if (dwToRX>0)
            {
            Status=FT_Read(hFT,pucData,dwToRX,&dwRXed);
            if (Status!=FT_OK)
               {
               dwRXed=0;   //Error
               }
            }
         }
      else
         dwRXed=0;
      return (int)dwRXed;
      }
   return 0;
}

int USB_FT232::nTXData(BYTE* pucData, const int nSize)
{
   if (hFT)
      {
      DWORD dwTXed;
      Status=FT_Write(hFT,pucData,nSize,&dwTXed);
      if (Status!=FT_OK)
         dwTXed=0;
      return (int)dwTXed;
      }
   return 0;
}

String USB_FT232::sGetDescr(const int nDev)
{
   int nD=nDev;
   if (nD==-1)
      nD=nDevice;
   switch(nD)
      {
      case USB_SERIAL2_DEV    : return "USB Serial Interface";
      case USB_WATCHDOG_DEV   : return "USB Power Watchdog";
      case USB_RESIG_DEV      : return "RESIG USB Interface";
      case USB_PROPMON_DEV    : return "Propulsion Monitor Interface";
      case USB_SERIAL3_DEV    : return "Enhanced USB Serial Interface";
      case USB_RHUBARB_DEV    : return "RHUBARB USB Interface";
      case USB_PRINTER_DEV    : return "USB Printer Emulator";
      case USB_DOSTAL_DEV     : return "USB DOSTAL Target Extractor";
      case USB_AGGR_DEV       : return "Aggregate Interface";
      case USB_TIME_DEV       : return "USB Time Distribution Interface";
      case USB_SERIAL31_DEV   : return "Enhanced USB Serial (ver 3.1) Interface";
      case USB_PROPMON3_DEV   : return "Propulsion Monitor Interface";
      case USB_SERIAL3_4CH_DEV: return "USB Serial 4 Channel Interface";
      default                 : return "Unknown Device";
      }
}

