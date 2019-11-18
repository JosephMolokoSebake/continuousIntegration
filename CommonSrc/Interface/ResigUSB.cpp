//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "ResigUSB.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


ResigUSB::ResigUSB(const int nDevNum)
{
   hFT=NULL;

   wSetup=RES_nAZ_TB | RES_nAZ_VECT | RES_nSERVO_TB | RES_nEL_VECT;
   bTXEnabled=false;

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
            if ((acBuf[0]=='M')&&(acBuf[1]=='T'))
               {
               nI=i;
               i=nDev;
               }
            }
         }
      }
   if (nI>=0)
      {
      Status=FT_Open(nI,&hFT);
      if (Status==FT_OK)
         {
         PurgeAll();
         Enable_TX(false);
         }
      if (Status!=FT_OK)
         hFT=NULL;
      }
   else
      hFT=NULL;
   dAzOffs=-51.0;
   dElOffs=0.0;
   nAntXmtLocCnt=1000;  //start local
}

ResigUSB::~ResigUSB()
{
   if (hFT)
      {
      Status=FT_Close(hFT);
      if (Status!=FT_OK)
         {
         }
      }
}

bool ResigUSB::bLoopbackOK()
{
   PurgeRX();       //Delete all pending data in the RX FIFO
   Write1(RESET_CMD,0);

   if (bLoopback(0x15))
      return bLoopback(0x0A);
   return false;
}

void ResigUSB::Setup(const WORD _wSetup)
{
   Write2(WR_SETUP_CMD,_wSetup);
}

BYTE ResigUSB::ucReadStatus()
{
   BYTE uc=ucRead(RD_STATUS_CMD);
   return uc;
}

BYTE ResigUSB::ucReadAntStatus()
{
   wSetup&=(~RES_nSERVO_TB);
   Setup(wSetup);
   WORD uData=wRead(RD_AZ_CMD);
   wSetup|=RES_nSERVO_TB;
   Setup(wSetup);
   //ANT XMT Local bit flickers if on remote
   if ((uData&ANTSTAT_XMT_LOC)==0)
      nAntXmtLocCnt=0;
   else
      {
      if (++nAntXmtLocCnt<25)
         uData&=(~ANTSTAT_XMT_LOC); //Set low until high for longer than 500 ms
      else
         nAntXmtLocCnt=25;
      }
   return (BYTE)(uData&0x00FF);
}

void ResigUSB::Test()
{
   Write2(WR_SETUP_CMD,0x0055);
   WORD w=ucRead(RD_STATUS_CMD);
   if (w==0)
      w=0;

}

void ResigUSB::Enable_TX(const bool bEn)
{
   if (!bEn)
      wSetup&=(~RES_XMT_INH_LED);
   else
      wSetup|=RES_XMT_INH_LED;
   bTXEnabled=bEn;
   Setup(wSetup);
}

void ResigUSB::TX_On()
{
   if (bTXEnabled)
      {
      //NB. check if TX in REMOTE mode
      wSetup|=RES_XMT;
		Setup(wSetup);
      }
}

void ResigUSB::TX_Off()
{
   wSetup&=(~RES_XMT);
   Setup(wSetup);
}

void ResigUSB::SetAz(const double dDeg)
{
	double dAngle=dDeg;
	if (dAngle<0.0)
		dAngle+=360.0;
	else
		if (dAngle>=360.0) dAngle-=360.0;
	WORD uData=(WORD)floor((dAngle/0.3516+0.5));

   wSetup&=(~RES_nAZ_VECT);
   wSetup|=RES_AZ_OUT_EN;
   Setup(wSetup);
	Write2(WR_AZ_CMD,uData);
   wSetup|=RES_nAZ_VECT;
   wSetup&=(~RES_AZ_OUT_EN);
   Setup(wSetup);
}

void ResigUSB::SetEl(const double dDeg)
{
	double dAngle=dDeg;  //+dElOffset;
   if (dAngle>25.0)
      dAngle=25.0;
   else if (dAngle<-25.0)
      dAngle=-25.0;
	if (dAngle<0.0)
		dAngle+=360.0;
	else
		if (dAngle>=360.0) dAngle-=360.0;
	WORD uData=(WORD)(floor(dAngle/0.3516)+0.5);
   wSetup&=(~RES_nEL_VECT);
   wSetup|=RES_AZ_OUT_EN;
   Setup(wSetup);
	Write2(WR_AZ_CMD,uData);
   wSetup|=RES_nEL_VECT;
   wSetup&=(~RES_AZ_OUT_EN);
   Setup(wSetup);
}

double ResigUSB::dGetAz(WORD& uData)
{
   wSetup&=(~RES_nAZ_TB);
   Setup(wSetup);
   uData=wRead(RD_AZ_CMD);
   wSetup|=RES_nAZ_TB;
   Setup(wSetup);

//Swap bits on read   
   WORD u=uData;
   uData=0;
   for (int i=0; i<10; i++)
      {
      if (u&(1<<i))
         uData|=(0x200>>i);
      }

	double dDeg=uData*0.3516;
	dDeg+=dAzOffs;
	if (dDeg<0.0)
		dDeg+=360.0;
	else if (dDeg>=360.0)
      dDeg-=360.0;
	return dDeg;

}



