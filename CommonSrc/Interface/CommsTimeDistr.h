//---------------------------------------------------------------------------

#ifndef CommsTimeDistrH
#define CommsTimeDistrH
//---------------------------------------------------------------------------

#include "CommsTimeDistr_101.h"
#include "CommsTimeDistr_200.h"
#include "JUtils.h"

class CommsTimeDistr
{

   int nVersion;
   CommsTimeDistr_101* p101;
   CommsTimeDistr_200* p200;

public:

   CommsTimeDistr(const int nVer)
   {
      p101=NULL;
      p200=NULL;
      nVersion=nVer;
      if (nVer==101)
         p101=new CommsTimeDistr_101;
      else if (nVer==200)
         p200=new CommsTimeDistr_200;
      else if (nVer==201)
         p200=new CommsTimeDistr_200("Comms_TimeDistr_201.rbf");
   }


   ~CommsTimeDistr()
   {
      delete p101;
      delete p200;
   }

   int nGetVer() const
   {
      return nVersion;
   }

   bool bLoopBackOK()
   {
      if (p101)
         return p101->bLoopBackOK();
      else if (p200)
         return p200->bLoopBackOK();
      else
         return false;
   }

   bool bOK() const
   {
      if (p101)
         return p101->bOK();
      else if (p200)
         return p200->bOK();
      else
         return false;
   }

   void ResetAll()
   {
      if (p101)
         p101->ResetAll();
      else if (p200)
         p200->ResetAll();
   }


   void ResetTD()
   {
      if (p101)
         p101->ResetTD();
      else if (p200)
         p200->ResetTD();
   }


   void StartTDCapt()
   {
      if (p101)
         p101->StartTDCapt();
      else if (p200)
         p200->StartTDCapt();
   }

   void StopTDCapt()
   {
      if (p101)
         p101->StopTDCapt();
      else if (p200)
         p200->StopTDCapt();
   }

   int nReadTDBuffer()
   {
      if (p101)
         return p101->nReadTDBuffer();
//      else if (p200)
//         return p200->nReadTDBuffer();
      else
         return 0; 
   }

   int nExtractTDMsg(BYTE* pucMsg, const int nMaxSize, bool& bCRCOK)
   {
      if (p101)
         return p101->nExtractTDMsg(pucMsg,nMaxSize,bCRCOK);
//      else if (p200)
//         return p200->nExtractTDMsg(pucMsg,nMaxSize,bCRCOK);
      else
         return 0;
   }

   bool bTimeValid()
   {
      if (p101)
         return p101->bTimeValid();
      else if (p200)
         return p200->bTimeValid();
      else
         return false;
   }

   int nReadTimeMS()
   {
      if (p101)
         return p101->nReadTimeMS();
      else if (p200)
         return p200->nReadTimeMS();
      else
         return 0;
   }

   bool bTDClockPresent()
   {
      if (p101)
         return p101->bTDClockPresent();
      else if (p200)
         return p200->bTDClockPresent();
      else
         return false;
   }

   int nGetTDCRCErrors() const
   {
      if (p101)
         return p101->nGetTDCRCErrors();
      else if (p200)
         return p200->nGetTDCRCErrors();
      else
         return 0;
   }

	JTime ReadTime_loc()
	{
		if (p101)
			return p101->ReadTime();
		else if (p200)
         return p200->ReadTime(false);
      else
         return JTime(true);
   }

   bool bWaitForTime()
   {
      if (p101)
         return p101->bWaitForTime();
      else if (p200)
         return p200->bWaitForTime();
      else
         return false;
   }

   LatLong ReadLL()
   {
      if (p101)
         return p101->ReadLL();
      else if (p200)
         return p200->ReadLL();
      else
         return LatLong(0.0,0.0);
   }

   LatLong ReadLL(double& dSpeed, double& dCourse)
   {
      if (p101)
         return p101->ReadLL(dSpeed,dCourse);
      else if (p200)
         {
         dSpeed=0.0;
         dCourse=0.0;
         return p200->ReadLL();
         }
      else
         {
         dSpeed=0.0;
         dCourse=0.0;
         return LatLong(0.0,0.0);
         }
   }

   int nReadTDMessages()
   {
      if (p101)
         return p101->nReadTDMessages();
      else if (p200)
         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
      return 0;
   }

   JTime GetZDATime() const
   {
      if (p101)
         return p101->GetZDATime();
      else if (p200)
         return p200->ReadTime(true);  //UTC Time/Date
//         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
      return JTime(true);
   }

   int nGetZDAMessages() const
   {
      if (p101)
         return p101->nGetZDAMessages();
      else if (p200)
         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
      return 0;
   }

   POSDataStruct GetPOSData()
   {
      if (p101)
         return p101->GetPOSData();
      else if (p200)
         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
      POSDataStruct TP;
      return TP;
   }

   int nGetPOSMessages() const
   {
      if (p101)
         return p101->nGetPOSMessages();
      else if (p200)
         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
      return 0;
   }

   int nReadTimeCRCErrors()
   {
      if (p101)
         return p101->nReadTimeCRCErrors();
      else if (p200)
         return p200->nGetTDCRCErrors();
      else
         return 0;
   }

   void ShowGPSStatus(TComponent* Owner, const int nHeadBaud=9600, const TIMEDISTR_UART_CHAN HeadChan=TD_UART_NONE);

   void ExtractGGAData(GGADataStruct& Data)
   {
      if (p200)
         p200->ExtractGGAData(Data);
      else
         ShowError("Cannot access this option from this version of CommsTimeDistr","CommsTimeDistr");
   }


//***********************************************************

   void ResetUART()
   {
      if (p101)
         p101->ResetUART();
      else if (p200)
         p200->ResetUART();
   }

   void SetupUART(const int nBaud, const TIMEDISTR_UART_CHAN Chan)
   {
      if (p101)
         p101->SetupUART(nBaud,Chan);
      else if (p200)
         p200->SetupUART(nBaud,Chan);
   }

   void StartUART()
   {
      if (p101)
         p101->StartUART();
      else if (p200)
         p200->StartUART();
   }

   void StopUART()
   {
      if (p101)
         p101->StopUART();
      else if (p200)
         p200->StopUART();
   }

   int nRXUARTData(BYTE* pucData, const int nMaxSize)
   {
      if (p101)
         return p101->nRXUARTData(pucData,nMaxSize);
      else if (p200)
         return p200->nRXUARTData(pucData,nMaxSize);
      else
         return 0;
   }

   int nTXUARTData(const BYTE* pucData, const int nSize)
   {
      if (p101)
         return p101->nTXUARTData(pucData,nSize);
      else if (p200)
         return p200->nTXUARTData(pucData,nSize);
      else
         return 0;
   }

   void TestInt(const int nDiv)
   {
      if (p101)
         p101->TestInt(nDiv);
      else if (p200)
         p200->TestInt(nDiv);
   }

//***********************************************************

   void ResetINS()
   {
      if (p101)
         p101->ResetINS();
      else if (p200)
         p200->ResetINS();
   }

   void SetupINS(const int nBaud, const int nFrameSize)
   {
      if (p101)
         p101->SetupINS(nBaud,nFrameSize);
      else if (p200)
         p200->SetupINS(nBaud,nFrameSize);
   }

   void StartINS(const bool bGPSTime)
   {
      if (p101)
         p101->StartINS(bGPSTime);
      else if (p200)
         p200->StartINS(bGPSTime);
   }

   void StopINS()
   {
      if (p101)
         p101->StopINS();
      else if (p200)
         p200->StopINS();
   }

   int nRXINSData(BYTE* pucData, const int nMaxSize)
   {
      if (p101)
         return p101->nRXINSData(pucData,nMaxSize);
      else if (p200)
         return p200->nRXINSData(pucData,nMaxSize);
      else
         return 0;
   }

//***********************************************************

   void ResetORT()
   {
      if (p101)
         p101->ResetORT();
      else if (p200)
         p200->ResetORT();
   }


   void StartORTCapt()
   {
      if (p101)
         p101->StartORTCapt();
      else if (p200)
         p200->StartORTCapt();
   }

   void StopORTCapt()
   {
      if (p101)
         p101->StopORTCapt();
      else if (p200)
         p200->StopORTCapt();
   }

   int nReadORTBuffer()
   {
      if (p101)
         return p101->nReadORTBuffer();
      else if (p200)
         return p200->nReadORTBuffer();
      else
         return 0;
   }

   int nExtractORTMsg(void* pMsg, JTime& T, const int nMaxSize, bool& bCRCOK, WORD& uRXCRC)
   {
      if (p101)
         return p101->nExtractORTMsg(pMsg,T,nMaxSize,bCRCOK,uRXCRC);
      else if (p200)
         return p200->nExtractORTMsg(pMsg,T,nMaxSize,bCRCOK,uRXCRC);
      else
         return 0;
   }

   void StartORTSim(const void* pData, const int nSize)
   {
      if (p101)
         p101->StartORTSim(pData,nSize);
      else if (p200)
         p200->StartORTSim(pData,nSize);
   }

   void StopORTSim()
   {
      if (p101)
         p101->StopORTSim();
      else if (p200)
         p200->StopORTSim();
   }

   bool bORTClockPresent() const
   {
      if (p101)
         return p101->bORTClockPresent();
      else if (p200)
         return p200->bORTClockPresent();
      else
         return false;
   }

   int nGetORTCRCErrors() const
   {
      if (p101)
         return p101->nGetORTCRCErrors();
      else if (p200)
         return p200->nGetORTCRCErrors();
      else
         return 0;
   }


//***********************************************************

   void SetMRRIncrDec(  const int nACPNum,
                        const double dARP,
                        const bool bInvertACP,
                        const bool bInvertNRP,
                        const double dNRPFilter,
                        const double dACPFilter)
   {
      if (p101)
         p101->SetMRRIncrDec(nACPNum,dARP,bInvertACP,bInvertNRP,dNRPFilter,dACPFilter);
      else if (p200)
         p200->SetMRRIncrDec(nACPNum,dARP,bInvertACP,bInvertNRP,dNRPFilter,dACPFilter);
   }

   void EnableMRR(const bool bEn, const bool bSim=false)
   {
      if (p101)
         p101->EnableMRR(bEn,bSim);
      else if (p200)
         p200->EnableMRR(bEn,bSim);
   }

   double dReadMRRAngle(int& nMaxCount, double& dARP)
   {
      if (p101)
         return p101->dReadMRRAngle(nMaxCount,dARP);
      else if (p200)
         return p200->dReadMRRAngle(nMaxCount,dARP);
      else
         return 0.0;
   }

   void ReadMRRWatchdogs(bool& bACPWatch, bool& bNRPWatch)
   {
      if (p101)
         p101->ReadMRRWatchdogs(bACPWatch,bNRPWatch);
      else if (p200)
         p200->ReadMRRWatchdogs(bACPWatch,bNRPWatch);
   }

   int nReadMRRSectorTimes(DWORD* pdwData)    //Max = 256 DWORDS
   {
      if (p101)
         return p101->nReadMRRSectorTimes(pdwData);
      else if (p200)
         return p200->nReadMRRSectorTimes(pdwData);
      else
         return 0;
   }

//***********************************************************

};


#endif
