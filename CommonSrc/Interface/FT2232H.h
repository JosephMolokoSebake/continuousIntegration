//---------------------------------------------------------------------------

#ifndef FT2232HH
#define FT2232HH
//---------------------------------------------------------------------------

#include "ftd2xx.h"
#include "USB_FT_Devices.h"


class FT2232H
{

public:

	enum FT_MODE
	{
		RESET_MODE				= 0x00,
		ASYNC_BITBANG_MODE	= 0x01,
		MPSSE_MODE				= 0x02,
		SYNC_BITBANG_MODE		= 0x04,
		MCU_HOST_MODE			= 0x08,
		FAST_ISO_SERIAL  		= 0x10,
		CBUS_BITBANG_MODE 	= 0x20,	//FT232H only
		SYNC_FIFO_MODE  		= 0x40,
	};

private:

	enum
	{
		CMD_STUFF_BYTE	= 0x55,
	};


	FT_HANDLE hFT;
	FT_STATUS Status;

	int nDev;
	int nDevSerial;

	int nTest;
	bool bStreaming;

	bool bOpenDevice(const int nDevice, const int nSerial, const char cPort);	//cPort is 'A' or 'B'

	bool bWriteData(void* pData, const int nSize);

public:

	FT2232H(const int nDevice, const int nSerial=-1, const char cPort='A');

	~FT2232H();

	void Close()
	{
		if (hFT)
			{
			::FT_Close(hFT);
			hFT=NULL;
			}
	}

	bool bOK() const  //Device Ready
	{
		return (hFT!=NULL);
	}

	bool bSetMode(const FT_MODE Mode, const BYTE ucIOMask=0xFF);

	bool bWriteAllData(void* pData, const int nSize);

	BYTE ucGetBitStatus()
	{
		BYTE uc;
		::FT_GetBitMode(hFT,&uc);
		return uc;
	}

	void PurgeAll()
	{
		if (hFT)
			::FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
	}

	void PurgeRX()
	{
		if (hFT)
			::FT_Purge(hFT,FT_PURGE_RX);
	}

	void PurgeTX()
	{
		if (hFT)
			::FT_Purge(hFT,FT_PURGE_TX);
	}

	bool bWriteData(const int nCmd, void* pData, const int nSize);

	int nReadData(void* pData, const int nMaxSize);

	bool bServiceStatus();		//This writes Setup and receives Status
	bool bUpdateSetup();		//This writes Setup data

	int nGetDevSerial() const
	{
		return nDevSerial;
	}

	String sGetDriverVersion();
	String sGetLibraryVersion();

	void SetStream(const bool bEn)
	{
		bStreaming=bEn;
		nTest=0;
	}

	String sGetDevDescr();

};

#endif
