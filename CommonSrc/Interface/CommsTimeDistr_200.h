//---------------------------------------------------------------------------
#ifndef CommsTimeDistr_200H
#define CommsTimeDistr_200H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Driver for CommsTimeDistr_200 card
 *    FILE NAME		CommsTimeDistr_200
 *		PROJECT			Virtue V/C0771
 *		ACTIVITY       14
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium
 *		OS					MS Windows 2000 SP3
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *    	Software driver for the PCI Bus MSDS Slave Interface.
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 07/09/2002
 ************************************************************************/

#include <Syncobjs.hpp>

#include "PCI32_WDM_Int.h"
#include "JFile.h"
#include "Jcrc.h"
#include "JTime.h"
#include "JTransform.h"

#ifndef TIMEDISTR_GGADATA_DEFINED
#define TIMEDISTR_GGADATA_DEFINED

struct GGADataStruct
{
   bool bDiff;
   int nSat;
   JTime T;          //Time Only
   LatLong LL;
   double dAlt;      // m
   double dHDOP;

   GGADataStruct()
   {
      LL.Invalidate();
   }

   void Invalidate()
   {
      LL.Invalidate();
   }

   bool bValid() const
   {
      return LL.bValid();
   }

};

#endif

#ifndef TIMEDISTR_UART_CHAN_DEFINED
#define TIMEDISTR_UART_CHAN_DEFINED

enum TIMEDISTR_UART_CHAN
{
   TD_UART_NONE      = 0,
   TD_UART_RS422_0   = 1,
   TD_UART_RS422_1   = 2,
   TD_UART_RS422_2   = 3,
   TD_UART_RS422_3   = 4,
   TD_UART_RS232_0   = 5,
   TD_UART_RS232_1   = 6,
};

#endif

class CommsTimeDistr_200:public PCI32_WDM_Int
{

   enum
   {
      CLOCK_FREQ     = 50,    //50 MHz on-board Clock
   };

   enum
   {
      SETUP_WR          = 0,
      BAUD_WR           = 4,
      UART_TX_WR        = 8,
      ORT_SIM_DATA_WR   = 12,
      LOOPBACK_WR       = 28,

      MRR_ACP_MAX_INT_WR   = 16,
      MRR_ACP_DIV_WR       = 20,
      MRR_DEL_WR           = 24,

   };

   enum
   {
      TD2_DATA_RD       = 0,
      TD2_STATUS_RD     = 4,
      UART_STATUS_RD    = 12,
      UART_DATA_RD      = 16,
      INS_ORT_STATUS_RD = 20,
      INS_ORT_DATA_RD   = 24,
      LOOPBACK_RD       = 28,

      MRR_ANGLE_RD      = 16,
      MRR_ARP_RD        = 20,
      MRR_FIFO_RD       = 24,
      MRR_STATUS_RD     = 28,

   };

   //SETUP_WR
   enum
   {
      RESET_TD             = 0x00000001,
      ENABLE_TD            = 0x00000002,

      RESET_UART           = 0x00000004,
      ENABLE_UART          = 0x00000008,
      UART_CHAN_MASK       = 0x00000070,
      UART_CHAN_SHIFT      = 4,

      RESET_INS            = 0x00000080,
      ENABLE_INS           = 0x00000100,
      SEL_INS_TAMS         = 0x00000200,
      INS_FRAME_MASK       = 0x00007C00,
      INS_FRAME_SHIFT      = 10,

      RESET_ORT            = 0x00008000,
      ENABLE_ORT           = 0x00010000,
      RESET_ORT_SIM        = 0x00020000,
      SET_ORT_SIM          = 0x00040000,
      ENABLE_ORT_SIM       = 0x00080000,

      MRR_RESET            = 0x00100000,
      MRR_ENABLE           = 0x00200000,
      MRR_INV_NRP          = 0x00400000,
      MRR_INV_ACP          = 0x00800000,
      MRR_SEL_SIM          = 0x01000000,
      MRR_SELECT           = 0x02000000,

      SEL_TD2_GGA          = 0x04000000,

   };

   //BAUD_WR
   enum
   {
      UART_BAUD_SHIFT      = 0,
      UART_BAUD_MASK       = 0x0000FFFF,
      INS_BAUD_SHIFT       = 16,
      INS_BAUD_MASK        = 0xFFFF0000,
   };


   //MRR_ACP_MAX_INT_WR
   enum
   {
      MRR_MAX_ACP_SHIFT    = 0,
      MRR_MAX_ACP_MASK     = 0x0000FFFF,
      MRR_ACP_INT_SHIFT    = 16,
      MRR_ACP_INT_MASK     = 0xFFFF0000,
   };

   //MRR_ACP_DIV_WR
   enum
   {
      MRR_ACP_DIV_SHIFT    = 0,
      MRR_ACP_DIV_MASK     = 0x00007FFF,
   };

   //MRR_DEL_WR
   enum
   {
      MRR_NRP_DEL_SHIFT    = 0,
      MRR_NRP_DEL_MASK     = 0x00007FFF,
      MRR_ACP_DEL_SHIFT    = 15,
      MRR_ACP_DEL_MASK     = 0x3FFF1000,
   };

   //TD2_STATUS_RD
   enum
   {
      //SEL_TD2_GGA = 0
      TD2_DAY_SHIFT        = 0,
      TD2_DAY_MASK         = 0x0000001F,
      TD2_MONTH_SHIFT      = 5,
      TD2_MONTH_MASK       = 0x0000000F,
      TD2_YEAR_SHIFT       = 9,
      TD2_YEAR_MASK        = 0x0000007F,
      TD2_TIME_ERR_SHIFT   = 16,
      TD2_TIME_ERR_MASK    = 0x00007FFF,
      TD2_CLOCK_PRESENT    = 0x80000000,

      //SEL_TD2_GGA = 1
      TD2_GGASIZE_SHIFT    = 0,
      TD2_GGASIZE_MASK     = 0x00000FFF,
      TD2_GGA_ERR_SHIFT    = 16,
      TD2_GGA_ERR_MASK     = 0x00007FFF,
      TD2_GGA_PRESENT      = 0x80000000,
   };

   //TD2_DATA_RD
   enum
   {
      //SEL_TD2_GGA = 0
      TD2_TIME_MASK        = 0x7FFFFFFF,
      TD2_TIME_VALID       = 0x80000000,

      //SEL_TD2_GGA = 1
      TD2_GGA_MASK         = 0x000000FF,
   };

   //UART_STATUS_RD
   enum
   {
      UART_TX_LEFT_SHIFT   = 0,
      UART_TX_LEFT_MASK    = 0x000003FF,
      UART_COUNT_SHIFT     = 10,
      UART_COUNT_MASK      = 0x00000FFF,
      UART_FIFO_FULL       = 0x00400000,
   };

   //UART_DATA_RD
   enum
   {
      UART_DATA_MASK   = 0x000000FF,
   };

   //INS_ORT_STATUS_RD
   enum
   {
      INS_COUNT_SHIFT     = 0,
      INS_COUNT_MASK      = 0x00000FFF,
      INS_FIFO_FULL       = 0x00001000,
      ORT_COUNT_SHIFT     = 13,
      ORT_COUNT_MASK      = 0x000007FF,
      ORT_FIFO_FULL       = 0x01000000,
      ORT_CLOCK_PRESENT   = 0x02000000,
   };

   //UART_DATA_RD
   enum
   {
      INS_DATA_MASK   = 0x000000FF,
   };

   //First DWORD of Data Frame
   enum
   {
      FRAME_SIZE_MASK   = 0x000001FF,
      FRAME_CRC_OK      = 0x00000200,
      FRAME_CRC_SHIFT   = 16,
   };

   //MRR_ANGLE_RD
   enum
   {
      MRR_ANGLE_SHIFT   = 0,
      MRR_ANGLE_MASK    = 0x0000FFFF,
      MRR_MAX_CNT_SHIFT = 16,
      MRR_MAX_CNT_MASK  = 0x0000FFFF,  //after shift
   };

   //MRR_ARP_RD
   enum
   {
      //all 32 bits
   };

   //MRR_FIFO_RD
   enum
   {
      //all 32 bits
   };

   //MRR_STATUS_RD
   enum
   {
      MRR_FIFO_SIZE_SHIFT  = 0,
      MRR_FIFO_SIZE_MASK   = 0x000000FF,
      MRR_ACP_PRESENT      = 0x00000100,
      MRR_NRP_PRESENT      = 0x00000200,
   };

   DWORD dwSetup;
   DWORD dwBaud;

//TD2 Parameters
   LatLong TD2_PrevLL;
   DWORD dwTD2_PrevLLTime;
   String sTD2_TempGGA;
   String sTD2_GGA;
   int nTD2_CS;
   int nGGAMsgCnt;

   RockCRC* pCRC;

   TCriticalSection* pLock;

   enum
   {
      MAX_ORT_BUFFER_SIZE   = 10000,
   };

   DWORD adwORTBuf[MAX_ORT_BUFFER_SIZE];
   int nORTBufSize;
   int nORTBufOffs;

   BYTE aucORTMsg[1024];
   int nORTMsgSize;
   int nORTMsgOffs;

   WORD uORTCRC;
   bool bORTCRCOK;
   DWORD dwORTTime;
   DWORD dwORTDate;
   bool bORTClkPresent;
   int nORTCRCErrors;

   int nTimeZoneBias_hours;

   TIMEDISTR_UART_CHAN UARTChan;

   WORD uBitReverse(const WORD uData) const;

   bool bCheckCRC(const BYTE* pucMsg, const int nSize) const;

//   JFile* pFil;

   char H2C(const BYTE uc) const
   {
      if (uc<10)
         return uc+'0';
      else
         return (uc-10)+'A';
   }


   void SelTD2_GGA(const bool bGGA)
   {
      if (dwSetup&SEL_TD2_GGA)
         {
         if (!bGGA)
            {
            dwSetup&=(~SEL_TD2_GGA);
            Write(SETUP_WR,dwSetup);
            }
         }
      else
         {
         if (bGGA)
            {
            dwSetup|=SEL_TD2_GGA;
            Write(SETUP_WR,dwSetup);
            }
         }
   }

   String sReadGGA();

   bool bCheckORTCRC(const BYTE* pucMsg, const int nSize) const;
   JTime CalcORTTime(const int nTime, const int nDate);


public:

   CommsTimeDistr_200(const String sRBF="");
   ~CommsTimeDistr_200();

   bool bLoopBackOK();

	int nGetBias_hours() const
	{
		return nTimeZoneBias_hours;
	}

   bool bOK() const
   {
      return bDeviceOK;
   }

   void ResetAll()
   {
      dwSetup|=RESET_TD;
      dwSetup|=RESET_UART;
      dwSetup|=RESET_INS;
      dwSetup|=RESET_ORT;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_TD);
      dwSetup&=(~RESET_UART);
      dwSetup&=(~RESET_INS);
      dwSetup&=(~RESET_TD);
      Write(SETUP_WR,dwSetup);
   }


   void ResetTD()
   {
      dwSetup|=RESET_TD;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_TD);
      Write(SETUP_WR,dwSetup);
   }


   void StartTDCapt();
   void StopTDCapt();

   bool bTimeValid()
   {
      pLock->Acquire();
      SelTD2_GGA(false);
      bool bValid=(dwRead(TD2_DATA_RD)&TD2_TIME_VALID)!=0;
      pLock->Release();
      return bValid;
   }

   int nReadTimeMS()
   {
      pLock->Acquire();
      SelTD2_GGA(false);
      DWORD dwT=dwRead(TD2_DATA_RD);
      pLock->Release();
      if (dwT&TD2_TIME_VALID)
         {
         dwT=(dwT&TD2_TIME_MASK)/10;
         dwT|=TD2_TIME_VALID;
         }
      else
         dwT=0;
      return (int)dwT;
   }

   bool bTDClockPresent()
   {
      pLock->Acquire();
      SelTD2_GGA(false);
      bool bPresent=(dwRead(TD2_STATUS_RD)&TD2_CLOCK_PRESENT)!=0;
      pLock->Release();
      return bPresent;
   }

   int nGetTDCRCErrors()
   {
      pLock->Acquire();
      SelTD2_GGA(false);
      int nErr=(int)((dwRead(TD2_STATUS_RD)>>TD2_TIME_ERR_SHIFT)&TD2_TIME_ERR_MASK);
      pLock->Release();
      return nErr;
   }

   int nGetGGACRCErrors()
   {
      pLock->Acquire();
      SelTD2_GGA(true);
      int nErr=(int)((dwRead(TD2_STATUS_RD)>>TD2_GGA_ERR_SHIFT)&TD2_GGA_ERR_MASK);
      pLock->Release();
      return nErr;
   }

   JTime ReadTime(const bool bUTCTime=false);

   bool bWaitForTime();

   LatLong ReadLL(const int nTimeoutMs=1500);

   String sGetGGA()
   {
      return sTD2_GGA;
   }

   void ExtractGGAData(GGADataStruct& Data);

//***********************************************************

   void ResetUART()
   {
      dwSetup|=RESET_UART;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_UART);
      Write(SETUP_WR,dwSetup);
   }

   void SetupUART(const int nBaud, const TIMEDISTR_UART_CHAN Chan);
   void StartUART();
   void StopUART();
   int nRXUARTData(BYTE* pucData, const int nMaxSize);
   int nTXUARTData(const BYTE* pucData, const int nSize);


   void TestInt(const int nDiv)
   {
      DWORD dw=nDiv;
      dwBaud&=(~UART_BAUD_MASK);
      dwBaud|=(dw<<UART_BAUD_SHIFT);
      Write(BAUD_WR,dwBaud);
      dwSetup|=RESET_UART;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_UART);
      dwSetup|=ENABLE_UART;
      Write(SETUP_WR,dwSetup);
   }

//***********************************************************

   void ResetINS()
   {
      dwSetup|=RESET_INS;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_INS);
      Write(SETUP_WR,dwSetup);
   }

   void SetupINS(const int nBaud, const int nFrameSize);
   void StartINS(const bool bGPSTime);
   void StopINS();
   int nRXINSData(BYTE* pucData, const int nMaxSize);

//***********************************************************

   void ResetORT()
   {
      dwSetup|=RESET_ORT;
      Write(SETUP_WR,dwSetup);
      dwSetup&=(~RESET_ORT);
      Write(SETUP_WR,dwSetup);
   }


   void StartORTCapt();
   void StopORTCapt();
   int nReadORTBuffer();
   int nExtractORTMsg(void* pMsg, JTime& T, const int nMaxSize, bool& bCRCOK, WORD& uRXCRC);
   void StartORTSim(const void* pData, const int nSize);
   void StopORTSim();

   bool bORTClockPresent() const
   {
      return bORTClkPresent;
   }

   int nGetORTCRCErrors() const
   {
      return nORTCRCErrors;
   }


//***********************************************************

   void SetMRRIncrDec(  const int nACPNum,
                        const double dARP,
                        const bool bInvertACP,
                        const bool bInvertNRP,
                        const double dNRPFilter,
                        const double dACPFilter);

   void EnableMRR(const bool bEn, const bool bSim=false);

   double dReadMRRAngle(int& nMaxCount, double& dARP);

   void ReadMRRWatchdogs(bool& bACPWatch, bool& bNRPWatch);

   int nReadMRRSectorTimes(DWORD* pdwData);    //Max = 256 DWORDS

   int nGetGGAMsgCnt() const
   {
      return nGGAMsgCnt;
   }

};

//***********************************************************


#endif
