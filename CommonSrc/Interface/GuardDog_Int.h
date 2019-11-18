//---------------------------------------------------------------------------

#ifndef GuardDog_intH
#define GuardDog_intH
//---------------------------------------------------------------------------

#include "USB_FT245.h"
#include "USBSerial.h"
#include "NMEADecode.h"
#include "JTime.h"
#include "JTransform.h"

enum GD_INTERFACE
{
	GD_USB_INT,
	GD_RS232_INT,
	GD_RS422_INT,
};


class GuardDog_Int
{

	enum GD_CMD
	{
		RESET_CMD		= 0,
		SETUP_CMD		= 1,
		GPS_WR_CMD		= 2,
		GPS_RD_CMD		= 3,
		TRIGGER_0_CMD 	= 4,
		TRIGGER_1_CMD 	= 5,
		TRIGGER_2_CMD 	= 6,
		STATUS_CMD		= 7,
	};

	enum GD_SETUP
	{
		SETUP_WR				= 0,
		LOOPBACK_WR			= 1,
		GPS_BAUD_WR			= 2,
		WATCH_TIME_0_WR	= 3,
		WATCH_TIME_1_WR	= 4,
		WATCH_TIME_2_WR	= 5,
		WATCH_DELAY_0_WR 	= 6,
		WATCH_DELAY_1_WR 	= 7,
		WATCH_DELAY_2_WR 	= 8,
	};

	enum GD_SETUP_BITS
	{
		ENABLE_GPS			= 0,
		SEL_EXT_GPS			= 1,
		ENABLE_WATCHDOG_0	= 2,
		ENABLE_WATCHDOG_1	= 3,
		ENABLE_WATCHDOG_2	= 4,
		OVERRIDE_RELAY_0	= 5,
		OVERRIDE_RELAY_1	= 6,
		OVERRIDE_RELAY_2	= 7,
		GSM_REPORT			= 8,
	};



	enum GD_STATUS_BYTE
	{
		LOOPBACK_RD_LSB	= 0,
		LOOPBACK_RD_MSB	= 1,
		WATCHCNT_0_RD_LSB	= 2,
		WATCHCNT_0_RD_MSB	= 3,
		WATCHCNT_1_RD_LSB	= 4,
		WATCHCNT_1_RD_MSB	= 5,
		WATCHCNT_2_RD_LSB	= 6,
		WATCHCNT_2_RD_MSB	= 7,
		GPS_AVAIL_LSB		= 8,
		GPS_AVAIL_MSB		= 9,
		STATUS_LSB			= 10,
		STATUS_MSB			= 11,
		UTC_TIME_B0			= 12,
		UTC_TIME_B1			= 13,
		UTC_TIME_B2			= 14,
		UTC_TIME_B3			= 15,
		UTC_DATE_LSB		= 16,
		UTC_DATE_MSB		= 17,
		FW_VERSION_LSB		= 18,
		FW_VERSION_MSB	 	= 19,
		GPS_LAT_B0			= 20,
		GPS_LONG_B0			= 24,
		GPS_HEIGHT_B0		= 28,
		UTC_MSGCNT_B0 		= 32,
	};

	enum GD_STATUS_BITS
	{
		GSM_OVERRIDE_STAT	= 0,
		SEL_INT_0 			= 1,
		SEL_INT_1 			= 2,
		UTC_VALID_TIME		= 3,
		UTC_VALID_DATE		= 4,
		UTC_VALID_LEAPSEC	= 5,

	};

	enum
	{
		STUFF_BYTE	= 0x55,
   };


	USB_FT245* pUSB;
	USBSerial* pSerial;


	NMEADecode NMEA;


	WORD awSetup[16];
	BYTE aucStatus[64];

	int nCreateUBXMsg(const BYTE ucHead0, const BYTE ucHead1, const BYTE ucID0, const BYTE ucID1, const WORD wSize, const BYTE* pucPayload, const bool bStuff, BYTE* pucMsg);

	void SendCmd(const GD_CMD Cmd);

public:

	GuardDog_Int(GD_INTERFACE Interface, const int nSerial);
	~GuardDog_Int();

	bool bOK()
	{
      return (pUSB!=NULL)||(pSerial!=NULL);
   }

   void ResetAll();

   void UpdateSetup();

	void SetSetup(const int n, const WORD w)
	{
		awSetup[n]=w;
   }

	void SetSetupBit(const int n, const int nBitOffs)
	{
		awSetup[n]|=(1<<nBitOffs);
   }

	void ClearSetupBit(const int n, const int nBitOffs)
	{
		awSetup[n]&=(~(1<<nBitOffs));
	}

	void SetSetupBits(const int n, const int nStartOffs, const int nBits, const WORD wVal)
	{
		for (int i=0; i<nBits; i++)
			{
			if ((wVal>>i)&0x0001)
				SetSetupBit(n,i+nStartOffs);
			else
				ClearSetupBit(n,i+nStartOffs);
			}

	}

	bool bUpdateStatus();

	BYTE ucGetStatus(const int n) const
	{
		return aucStatus[n];
	}

	bool bGetStatusBit(const int n, const int nBitOffs) const
	{
		return ((aucStatus[n]>>nBitOffs)&0x01)!=0;
	}

	WORD wGetStatus(const int n) const
	{
		WORD w=aucStatus[n+1];
		w<<=8;
		w|=aucStatus[n];
		return w;
	}

	DWORD dwGetStatus(const int n) const
	{
		DWORD dw;
		memcpy(&dw,&aucStatus[n],sizeof(dw));
		return dw;
	}

	int nGetStatus(const int n) const
	{
		int nV;
		memcpy(&nV,&aucStatus[n],sizeof(nV));
		return nV;
	}

	bool bLoopbackOK();

	int nGetFWVersion() const
	{
		return (int)wGetStatus(FW_VERSION_LSB);
	}

	void EnableGPS(const bool bEn, const int nBaud=9600);

	int nReadGPSData(BYTE* pucBuf, const int nMaxBytes);

	void SetGPSNMEARate(const String sMsg, const int nRate);
	void SetGPSUBXMsg(const BYTE ucMsgClass, const BYTE ucMsgID, const int nRate);

	void StartWatchdog(const int nWatchID, const int nWatch_sec, const int nDelay_sec);
	void StopWatchdog(const int nWatchID);
	void TriggerWatchdog(const int nWatchID);
	int nReadWatchCount(const int nWatchID, bool& bResetTime);
	int nGetWatchCount(const int nWatchID, bool& bResetTime);	//Needs external bUpdateStatus

	void TestTimeDecode();

	JTime ReadGPSTime();

	DWORD dwReadGPSPar(JTime& T, LatLong& LL, double& dHeight_m, bool& bTimeValid, bool& bDateValid, bool& bLeapValid);

	bool bGetGSMOverride() const
	{
		return bGetStatusBit(STATUS_LSB,GSM_OVERRIDE_STAT);
   }


};


#endif
