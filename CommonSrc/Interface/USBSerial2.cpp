//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "USBSerial2.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


USBSerial2::USBSerial2(const int _nBaud, const char _cParity, const int _nDataBits, const int _nStopBits, const bool _b232, const int nDevNum)
{
   ::InitializeCriticalSection(&CriticalSection);
   hFT=NULL;

   int nDev;
   int nI=-1;
   Status=FT_ListDevices(&nDev,NULL,FT_LIST_NUMBER_ONLY);
   if (Status==FT_OK)
      {
      for (int i=0; i<nDev; i++)
         {
         char acBuf[16];
         Status=FT_ListDevices((PVOID)i,acBuf,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
         if (Status==FT_OK)
            {
            char acType[16],acNum[16];
            memcpy(acType,acBuf,4);
            acType[4]=0;
            memcpy(acNum,&acBuf[4],4);
            acNum[4]=0;
            int nType,nNum;
				if (sscanf(acType,"%d",&nType)==1)
					{
               if (sscanf(acNum,"%d",&nNum)!=1)
                  nNum=-1;
               }
            else
               nType=-1;
            if (nType==1)  //USB Serial
               {
               if (nDevNum>=0)
                  {
                  if (nNum==nDevNum)
                     {
                     nI=i;
                     i=nDev;  //Device found -> stop loop
                     }
                  }
               else
                  {
                  nI=i;
                  i=nDev;  //Device found -> stop loop
                  }
               }
            }
         }
      }
   if (nI>=0)
      {
      Status=FT_Open(nI,&hFT);
      if (Status==FT_OK)
         Setup(_nBaud,_cParity,_nDataBits,_nStopBits,_b232);
      else
         hFT=NULL;
      }
}

USBSerial2::USBSerial2(const int nDevNum)
{
   ::InitializeCriticalSection(&CriticalSection);
   hFT=NULL;

   int nDev;
   int nI=-1;
   Status=FT_ListDevices(&nDev,NULL,FT_LIST_NUMBER_ONLY);
   if (Status==FT_OK)
      {
      for (int i=0; i<nDev; i++)
         {
         char acBuf[16];
         Status=FT_ListDevices((PVOID)i,acBuf,FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
         if (Status==FT_OK)
            {
            char acType[16],acNum[16];
            memcpy(acType,acBuf,4);
            acType[4]=0;
            memcpy(acNum,&acBuf[4],4);
            acNum[4]=0;
            int nType,nNum;
				if (sscanf(acType,"%d",&nType)==1)
					{
               if (sscanf(acNum,"%x",&nNum)!=1)
                  nNum=-1;
               }
            else
               nType=-1;
            if (nType==1)  //USB Serial
               {
               if (nDevNum>=0)
                  {
                  if (nNum==nDevNum)
                     {
                     nI=i;
                     i=nDev;  //Device found -> stop loop
                     }
                  }
               else
                  {
                  nI=i;
                  i=nDev;  //Device found -> stop loop
                  }
               }
            }
         }
      }
   if (nI>=0)
      {
      Status=FT_Open(nI,&hFT);
      if (Status!=FT_OK)
         hFT=NULL;
      }
}

USBSerial2::~USBSerial2()
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

void USBSerial2::Setup(const int _nBaud, const char _cParity, const int _nDataBits, const int _nStopBits, const bool _b232)
{
   if (hFT)
      {
      Status=FT_SetBaudRate(hFT,_nBaud);
      if (Status==FT_OK)
         {
         BYTE ucLen,ucStop,ucParity;
         switch(_cParity)
            {
            case 'E':   ucParity=FT_PARITY_EVEN;   break;
            case 'O':   ucParity=FT_PARITY_ODD;    break;
            case 'M':   ucParity=FT_PARITY_MARK;   break;
            case 'S':   ucParity=FT_PARITY_SPACE;  break;
            default:    ucParity=FT_PARITY_NONE;
            }
         switch(_nDataBits)
            {
            case 7:  ucLen=FT_BITS_7;           break;
            default: ucLen=FT_BITS_8;
            }
         switch(_nStopBits)
            {
            case 2:  ucStop=FT_STOP_BITS_2;     break;
            default: ucStop=FT_STOP_BITS_1;
            }
         Status=FT_SetDataCharacteristics(hFT,ucLen,ucStop,ucParity);
         }
      if (Status==FT_OK)
         {
         if (_b232)
            Status=FT_ClrDtr(hFT);
         else
            Status=FT_SetDtr(hFT);
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

int USBSerial2::nRXData(BYTE* pucData, const int nMaxSize)
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

int USBSerial2::nTXData(BYTE* pucData, const int nSize)
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
