/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	CoastRad Power Watchdog Interface
 *		PROJECT			C
 *		ACTIVITY       1
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows XP Pro SP1
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *      1.00: Initial Version 13-02-2004
************************************************************************/



//---------------------------------------------------------------------------
#ifndef ResigUSBH
#define ResigUSBH
//---------------------------------------------------------------------------

#include "FTD2XX.h"

enum RESIG_SETUP
{
   RES_STDBY_LED     = 0x001,
   RES_nAZ_TB        = 0x002,
   RES_nAZ_VECT      = 0x004,
   RES_nSERVO_TB     = 0x008,
   RES_nEL_VECT      = 0x010,
   RES_RELAY_ON      = 0x020,
   RES_XMT           = 0x040,
   RES_XMT_INH_LED   = 0x080,
   RES_AZ_OUT_EN     = 0x100,
};

enum RESIG_STATUS
{
   STAT_RELAY_ON     = 0x01,
   STAT_OVLD         = 0x02,
   STAT_nATTEN_POS   = 0x04,
   STAT_nNORMAL_POS  = 0x08,
   STAT_MODINT       = 0x10,
   STAT_DUTY         = 0x20,
   STAT_nREADY       = 0x40,
   STAT_TIMEOUT      = 0x80,
};

enum RESIG_ANTSTATUS
{
   ANTSTAT_SW_LOC    = 0x01,
   ANTSTAT_AZ_ERR    = 0x10,
   ANTSTAT_EL_ERR    = 0x20,
   ANTSTAT_XMT_LOC   = 0x80,
};


class ResigUSB
{

   double dAzOffs;
   double dElOffs;

   enum USB_CMD
   {
      DATA_CMD       = 0,
      RD_STATUS_CMD  = 1,
      RD_AZ_CMD      = 2,
      WR_AZ_CMD      = 3,
      WR_SETUP_CMD   = 4,
      LOOPBACK_CMD   = 5,
      RESET_CMD      = 7,
   };

   FT_HANDLE hFT;
   FT_STATUS Status;

   WORD wSetup;

   bool bTXEnabled;

   int nAntXmtLocCnt;

   void PurgeAll()  //Delete all data in RX and TX FIFOs
   {
      Status=FT_Purge(hFT,FT_PURGE_RX|FT_PURGE_TX);
   }

   void PurgeTX()   //Delete all data in TX FIFO
   {
      Status=FT_Purge(hFT,FT_PURGE_TX);
   }

   void PurgeRX() //Delete all data in RX FIFO
   {
      Status=FT_Purge(hFT,FT_PURGE_RX);
   }

   void Write1(const USB_CMD Cmd, const BYTE ucData)
   {
      if (hFT)
         {
         DWORD dwTXed;
         BYTE uc=Cmd<<5;
         uc|=(ucData&0x1F);
         FT_Write(hFT,&uc,1,&dwTXed);
         }
   }

   void Write2(const USB_CMD Cmd, const WORD wData)
   {
      if (hFT)
         {
         DWORD dwTXed;
         BYTE auc[2];
         auc[0]=Cmd<<5;
         auc[0]|=(wData&0x1F);
         auc[1]=(wData>>5)&0x1F;
         FT_Write(hFT,auc,2,&dwTXed);
         }
   }

   BYTE ucRead(const USB_CMD Cmd)
   {
      BYTE ucData;
      if (hFT)
         {
         DWORD dwTXed,dwRXed;
         BYTE uc=Cmd<<5;
         FT_Write(hFT,&uc,1,&dwTXed);
         FT_Read(hFT,&ucData,1,&dwRXed);
         }
      else
         ucData=0;
      return ucData;
   }

   WORD wRead(const USB_CMD Cmd)
   {
      WORD wData;
      if (hFT)
         {
         DWORD dwTXed,dwRXed;
         BYTE uc=Cmd<<5;
         FT_Write(hFT,&uc,1,&dwTXed);
         FT_Read(hFT,&wData,2,&dwRXed);
         }
      else
         wData=0;
      return wData&0x03FF;
   }

   bool bLoopback(const BYTE uc)
   {
      BYTE ucData;
      if (hFT)
         {
         DWORD dwTXed,dwRXed;
         BYTE ucWr=LOOPBACK_CMD<<5;
         ucWr|=(uc&0x1F);
         FT_Write(hFT,&ucWr,1,&dwTXed);
         FT_Read(hFT,&ucData,1,&dwRXed);
         ucData&=0x1F;
         }
      else
         ucData=0;
      return (ucData==uc);
   }


public:

   ResigUSB(const int nDevNum=-1);
   ~ResigUSB();

   bool bOK() const  //Device Ready
   {
      return (hFT!=NULL);
   }

   bool bLoopbackOK();  //Loopback passed

   void Setup(const WORD _wSetup);

   BYTE ucReadStatus();

   BYTE ucReadAntStatus();

   void Test();

   void Enable_TX(const bool bEn);

   void TX_On();
   void TX_Off();


	void SetAz(const double dDeg);

	void SetEl(const double dDeg);

	double dGetAz(WORD& uData);




};

#endif
