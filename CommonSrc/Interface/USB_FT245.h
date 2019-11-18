/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Resig USB Interface
 *		PROJECT			C1892
 *		ACTIVITY       1
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP Pro SP2
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      1.00: Initial Version 22-02-2006
************************************************************************/



//---------------------------------------------------------------------------
#ifndef USB_FT245H
#define USB_FT245H
//---------------------------------------------------------------------------

#include "FTD2XX.h"
#include "USB_FT_Devices.h"

#include "SyncObjs.hpp"
#include "JList.h"

class FT245_Detect
{

	struct DevListStruct
	{
		int nID;
		int nSerial;
	};

	JList<DevListStruct> DevList;

public:

   FT245_Detect();

   int nDevCount()
   {
      return DevList.nGetCount();
	}

	String sGetDriverVersion(FT_HANDLE h=NULL);

	String sGetLibraryVersion();

   void GetDev(const int n, int& nID, int& nSerial)
   {
      if (n<DevList.nGetCount())
         {
			nID=DevList[n]->nID;
         nSerial=DevList[n]->nSerial;
			}
	}

	String sGetDescr(const int nDev, const int nSerial);
	String sGetDescrFromIndex(const int nDevListIndex);

};


class USB_FT245
{

   TCriticalSection* pLock;

   FT_HANDLE hFT;
   FT_STATUS Status;
	int nDevice;

	WORD wSetup;

   int nSerialFound;
   int nDevFound;


public:

   USB_FT245(const int _nDevice, const int nSerialNum=-1, const bool bVerbose=true);
	~USB_FT245();

	String sGetDriverVersion();

   FT_STATUS GetStatus() const
   {
      return Status;
   }

   void Purge()
   {
      Status=FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
   }

   int nGetDevice()
   {
      return nDevFound;
   }

   int nGetSerialNum()
   {
      return nSerialFound;
   }

   bool bOK() const  //Device Ready
   {
      return (hFT!=NULL);
   }

   FT_STATUS PurgeAll()  //Delete all data in RX and TX FIFOs
   {
      if (hFT)
         Status=FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
      return Status;
   }

   FT_STATUS PurgeTX()   //Delete all data in TX FIFO
   {
      if (hFT)
         Status=FT_Purge(hFT,FT_PURGE_TX);
      return Status;
   }

   FT_STATUS PurgeRX() //Delete all data in RX FIFO
   {
      if (hFT)
         Status=FT_Purge(hFT,FT_PURGE_RX);
      return Status;
   }

   int nWriteUSB(BYTE* pucData, const int nBytes)
   {
      DWORD dwTXed;
      if (hFT)
         Status=FT_Write(hFT,pucData,nBytes,&dwTXed);
      return (int)dwTXed;
   }

   int nReadUSB(BYTE* pucData, const int nBytes)
   {
      DWORD dwRXed;
      if (hFT)
         Status=FT_Read(hFT,pucData,nBytes,&dwRXed);
      return (int)dwRXed;
   }

   int nReadUSBRXSize()
   {
      DWORD dwRXBytes,dwTXBytes,dwEvent;
      if (hFT)
         {
         Status=FT_GetQueueStatus(hFT,&dwRXBytes);
         if (Status!=FT_OK)
            dwRXBytes=0;
         }
      else
         dwRXBytes=0;
      return (int)dwRXBytes;
   }

   int nReadUSBTXSize()
   {
      DWORD dwRXBytes,dwTXBytes,dwEvent;
      if (hFT)
         Status=FT_GetStatus(hFT,&dwRXBytes,&dwTXBytes,&dwEvent);
      else
         dwTXBytes=0;
      return (int)dwTXBytes;
   }


   void Acquire()
	{
      pLock->Acquire();
	}

	void Release()
   {
      pLock->Release();
	}

	String sGetLibraryVersion();


	String sGetDescr(const int nDev=-1,const int nSer=-1)
	{
		IMT_USB_Device Dev;
		if (nDev<0)
			return Dev.sGetDescr(nDevice,nSerialFound);
		else
			return Dev.sGetDescr(nDev,nSer);
	}



};



#endif
