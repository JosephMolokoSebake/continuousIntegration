//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "GuardDog_int.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



GuardDog_Int::GuardDog_Int(GD_INTERFACE Interface, const int nSerial)
{
	pUSB=NULL;
	pSerial=NULL;

	if (Interface==GD_USB_INT)
		{
		pUSB=new USB_FT245(USB_GUARDDOG_DEV,nSerial);
		if (!pUSB->bOK())
			{
			delete pUSB;
			pUSB=NULL;
			}
		}
	else
		{
		pSerial=new USBSerial(USB_DETECT_DEV,nSerial);
		if (pSerial->bOK())
			{
			pSerial->SetupUART(38400,'N',8,1,(Interface==GD_RS422_INT));
			pSerial->StartUART(Interface==GD_RS422_INT);
			}
		else
			{
			delete pSerial;
			pSerial=NULL;
         }
      }

	NMEA.Reset();
	memset(awSetup,0,sizeof(awSetup));
	if (bOK())
		ResetAll();
}

GuardDog_Int::~GuardDog_Int()
{
	delete pSerial;
	delete pUSB;
}

void GuardDog_Int::SendCmd(const GD_CMD Cmd)
{
	BYTE aucBuf[2];
	aucBuf[0]=STUFF_BYTE;
	aucBuf[1]=Cmd;
	if (pUSB)
		pUSB->nWriteUSB(aucBuf,2);
	else if (pSerial)
		pSerial->nTXUARTData(aucBuf,2);
}


void GuardDog_Int::ResetAll()
{
	SendCmd(RESET_CMD);
}

void GuardDog_Int::UpdateSetup()
{
	BYTE aucBuf[66];	//make provision for byte stuffing
	int n=0;
	aucBuf[n++]=STUFF_BYTE;
	aucBuf[n++]=SETUP_CMD;	//SETUP command
	for (int i=15; i>=0; i--)
		{
		aucBuf[n]=awSetup[i]&0x00FF;     	//LSB
		if (aucBuf[n++]==STUFF_BYTE)
			aucBuf[n++]=STUFF_BYTE;
		aucBuf[n]=(awSetup[i]>>8)&0x00FF;	//MSB
		if (aucBuf[n++]==STUFF_BYTE)
			aucBuf[n++]=STUFF_BYTE;
		}
	if (pUSB)
		pUSB->nWriteUSB(aucBuf,n);
	else if (pSerial)
		pSerial->nTXUARTData(aucBuf,n);
}

bool GuardDog_Int::bUpdateStatus()
{
	SendCmd(STATUS_CMD);
	BYTE aucBuf[72];
	int nRX=0;
	DWORD dT_ms=::GetTickCount();
	do
		{
		if (pUSB)
			nRX+=pUSB->nReadUSB(&aucBuf[nRX],36);
		else if (pSerial)
			nRX+=pSerial->nRXUARTData(&aucBuf[nRX],36);
		}
	while((nRX<36)&&(::GetTickCount()-dT_ms<1000));
	if (nRX>=36)
		{
		for (int i=0; i<36; i++)
			aucStatus[i]=aucBuf[35-i];  //Bytes were read in reverse order
		return true;
		}
	return false;
}

bool GuardDog_Int::bLoopbackOK()
{
	SetSetup(LOOPBACK_WR,0xAAAA);
	UpdateSetup();
	if (bUpdateStatus())
		{
		WORD w=wGetStatus(LOOPBACK_RD_LSB);
		if (w==0xAAAA)
			{
			SetSetup(LOOPBACK_WR,0x5555);
			UpdateSetup();
			if (bUpdateStatus())
				{
				w=wGetStatus(LOOPBACK_RD_LSB);
				if (w==0x5555)
					return true;
				}
			}
		}
	return false;
}

void GuardDog_Int::EnableGPS(const bool bEn, const int nBaud)
{
	WORD wBaud=50e6/(2*nBaud);
	SetSetup(GPS_BAUD_WR,wBaud);
	if (bEn)
		SetSetupBit(SETUP_WR,ENABLE_GPS);
	else
		ClearSetupBit(SETUP_WR,ENABLE_GPS);
	UpdateSetup();
}


int GuardDog_Int::nReadGPSData(BYTE* pucBuf, const int nMaxBytes)
{
	int nRX=0;
	if (bUpdateStatus())
		{
		int nGPSSize=wGetStatus(GPS_AVAIL_LSB);
		if (nGPSSize>nMaxBytes)
			nGPSSize=nMaxBytes;
		if (nGPSSize>0)
			{
			SendCmd(GPS_RD_CMD);
			DWORD dT_ms=::GetTickCount();
			do
				{
				if (pUSB)
					nRX+=pUSB->nReadUSB(pucBuf,nGPSSize);
				else if (pSerial)
					nRX+=pSerial->nRXUARTData(pucBuf,nGPSSize);
				}
			while((nRX<nGPSSize)&&(::GetTickCount()-dT_ms<1000));
			}
		}
	return nRX;
}

void GuardDog_Int::SetGPSNMEARate(const String sMsg, const int nRate)
{
	String sRate=String(nRate);
	String s="PUBX,40,"+sMsg+",0,"+sRate+","+sRate+"0,0,0";
	s=NMEA.sConstructSentence(s);
	int nL=s.Length();
	BYTE* pucBuf=new BYTE[nL*2+2];	//make provision for byte stuffing
	int n=0;
	pucBuf[n++]=STUFF_BYTE;
	pucBuf[n++]=GPS_WR_CMD;	//Write to GPS command

	AnsiString s1=s;
	for (int i=0; i<nL; i++)
		{
		pucBuf[n]=s1[i+1];
		if (pucBuf[n++]==STUFF_BYTE)
			pucBuf[n++]=STUFF_BYTE;
		}
	if (pUSB)
		pUSB->nWriteUSB(pucBuf,n);
	else if (pSerial)
		pSerial->nTXUARTData(pucBuf,n);
}

void GuardDog_Int::SetGPSUBXMsg(const BYTE ucMsgClass, const BYTE ucMsgID, const int nRate)
{
	BYTE aucPayload[32];
	BYTE aucTXMsg[32];
	int n=0;
	aucPayload[n++]=ucMsgClass;
	aucPayload[n++]=ucMsgID;
	aucPayload[n++]=nRate;

	int nTXSize=nCreateUBXMsg(0xB5,0x62,0x06,0x01,n,aucPayload,true,aucTXMsg);
	EnableGPS(true);
	if (pUSB)
		pUSB->nWriteUSB(aucTXMsg,nTXSize);
	else if (pSerial)
		pSerial->nTXUARTData(aucTXMsg,nTXSize);
}

void GuardDog_Int::StartWatchdog(const int nWatchID, const int nWatch_sec, const int nDelay_sec)
{
	SetSetup(WATCH_TIME_0_WR+nWatchID,nWatch_sec);
	SetSetup(WATCH_DELAY_0_WR+nWatchID,nDelay_sec);
	ClearSetupBit(SETUP_WR,OVERRIDE_RELAY_0+nWatchID);
	SetSetupBit(SETUP_WR,ENABLE_WATCHDOG_0+nWatchID);
	UpdateSetup();
}

void GuardDog_Int::StopWatchdog(const int nWatchID)
{
	ClearSetupBit(SETUP_WR,ENABLE_WATCHDOG_0+nWatchID);
	UpdateSetup();
}

void GuardDog_Int::TriggerWatchdog(const int nWatchID)
{
	SendCmd((GD_CMD)(TRIGGER_0_CMD+nWatchID));
}

int GuardDog_Int::nGetWatchCount(const int nWatchID, bool& bResetTime)
{
	int nCnt=(int)wGetStatus(WATCHCNT_0_RD_LSB+nWatchID*2);
	bResetTime=(nCnt&0x00008000)!=0;
	return nCnt&0x00007FFF;
}

int GuardDog_Int::nReadWatchCount(const int nWatchID, bool& bResetTime)
{
	bUpdateStatus();
	return nGetWatchCount(nWatchID,bResetTime);
}

int GuardDog_Int::nCreateUBXMsg(const BYTE ucHead0, const BYTE ucHead1, const BYTE ucID0, const BYTE ucID1, const WORD wSize, const BYTE* pucPayload, const bool bStuff, BYTE* pucMsg)
{
	BYTE aucTmp[1024];
	int n=0;
	aucTmp[n++]=ucHead0;
	aucTmp[n++]=ucHead1;
	aucTmp[n++]=ucID0;
	aucTmp[n++]=ucID1;
	aucTmp[n++]=(BYTE)(wSize&0xFF);
	aucTmp[n++]=(BYTE)((wSize>>8)&0xFF);
	memcpy(&aucTmp[n],pucPayload,wSize);
	n+=(int)wSize;
	BYTE ucCK_A=0;
	BYTE ucCK_B=0;
	for (int i=2; i<n; i++)
		{
		ucCK_A+=aucTmp[i];
		ucCK_B+=ucCK_A;
		}
	aucTmp[n++]=ucCK_A;
	aucTmp[n++]=ucCK_B;
	if (bStuff)
		{
		int nSize=n;
		n=0;
		pucMsg[n++]=STUFF_BYTE;
		pucMsg[n++]=GPS_WR_CMD;	//Write to GPS command
		for (int i=0; i<nSize; i++)
			{
			pucMsg[n]=aucTmp[i];
			if (pucMsg[n++]==STUFF_BYTE)
				pucMsg[n++]=STUFF_BYTE;
			}
		}
	else
		memcpy(pucMsg,aucTmp,n);
	return n;
}

void GuardDog_Int::TestTimeDecode()
{
	BYTE aucPayload[128];
	BYTE aucTXMsg[128];
	int n=0;
//GPS millisec
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
//Accuracy
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
//Nanosec
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;
	aucPayload[n++]=0x00;

	WORD wYear=2010;
	aucPayload[n++]=wYear&0xFF;		//LSB of year
	aucPayload[n++]=(wYear>>8)&0xFF;	//MSB of year
	aucPayload[n++]=10;					//month
	aucPayload[n++]=29;					//day
	aucPayload[n++]=14;					//hour
	aucPayload[n++]=44;					//minute
	aucPayload[n++]=21;					//sec
	aucPayload[n++]=0x07;				//all valid

	int nTXSize=nCreateUBXMsg(0xB5,0x62,0x01,0x21,n,aucPayload,true,aucTXMsg);
	EnableGPS(true);
	if (pUSB)
		pUSB->nWriteUSB(aucTXMsg,nTXSize);
	else if (pSerial)
		pSerial->nTXUARTData(aucTXMsg,nTXSize);
}

JTime GuardDog_Int::ReadGPSTime()
{
	JTime T;
	if (bUpdateStatus())
		{
		DWORD dwTime_100us=dwGetStatus(UTC_TIME_B0);
		if (dwTime_100us&0x80000000)
			{
			WORD wDate=wGetStatus(UTC_DATE_LSB);
			T.SetDCSDateTime(wDate,dwTime_100us);
			T+=3600*2;	//Make SAST
			}
		}
	return T;
}

DWORD GuardDog_Int::dwReadGPSPar(JTime& T, LatLong& LL, double& dHeight_m, bool& bTimeValid, bool& bDateValid, bool& bLeapValid)
{
	if (bUpdateStatus())
		{
		DWORD dwTime_100us=dwGetStatus(UTC_TIME_B0);
		if (dwTime_100us&0x80000000)
			{
			WORD wDate=wGetStatus(UTC_DATE_LSB);
			T.SetDCSDateTime(wDate,dwTime_100us);
			T+=3600*2;	//Make SAST
			}
		bTimeValid=bGetStatusBit(STATUS_LSB,UTC_VALID_TIME);
		bDateValid=bGetStatusBit(STATUS_LSB,UTC_VALID_DATE);
		bLeapValid=bGetStatusBit(STATUS_LSB,UTC_VALID_LEAPSEC);
		int nLat=nGetStatus(GPS_LAT_B0);
		int nLong=nGetStatus(GPS_LONG_B0);
		if ((nLat!=0)&&(nLong!=0))
			{
			int nHgt_mm=nGetStatus(GPS_HEIGHT_B0);
			LL.dLat=nLat*1e-7;
			LL.dLong=nLong*1e-7;
			dHeight_m=nHgt_mm*1e-3;
			}
		else
			{
			LL.Invalidate();
			dHeight_m=0.0;
			}
		return dwGetStatus(UTC_MSGCNT_B0);
		}
	else
		{
		T.Invalidate();
		LL.Invalidate();
		return 0;
		}
}

