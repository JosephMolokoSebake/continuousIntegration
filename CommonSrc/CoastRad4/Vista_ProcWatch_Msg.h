//---------------------------------------------------------------------------

#ifndef Vista_ProcWatch_MsgH
#define Vista_ProcWatch_MsgH

#include "Cst4Messages.h"
#include "VistaClient.h"



#pragma option push -a4    //Set compiler to DWORD alignment


//---------------------------------------------------------------------------

class Vista_ProcWatch_TX
{
   VistaClient* pClient;

   int nTXCnt;
   int nSiteID;
   String sSiteDescr;
   bool bTX_OK;
   bool bClientStatusChanged;
   String sClientStatus;
   TColor ClientStatusCol;

   DWORD dwTXInt_sec;
   DWORD dwNextTX_utc_sec;

public:

   Vista_ProcWatch_TX(TComponent* pOwnerForm, const int _nSiteID, const int nTXInt_sec=10);
   Vista_ProcWatch_TX(TComponent* pOwnerForm, const String _sSiteDescr, const int nTXInt_sec=10);
   ~Vista_ProcWatch_TX();

  	bool bTX(const JTime& T_utc, const bool bOn=true);
	bool bTX(const DWORD dwT_utc_sec, const bool bOn=true);
   bool bTX(const bool bOn=true);

   void InitTX();
   bool bCheckTX(const DWORD dwT_utc_sec, const bool bOn=true);
   bool bCheckTX(const JTime& T_utc, const bool bOn=true);
   bool bCheckTX(const bool bOn=true);

   int nGetTXCnt() const
   {
      return nTXCnt;
   }

   String sGetClientStatus() const
   {
      return sClientStatus;
   }

   TColor GetClientStatusColor() const
   {
      return ClientStatusCol;
   }

};

//************************************************************************************

class Vista_ProcWatch_RX
{

   struct RXStruct
   {
      int nID;
      String sDescr;
      DWORD dwLast_utc_sec;
      int nCnt;


      RXStruct()
      {
         nID=-1;
         sDescr="";
         dwLast_utc_sec=0;
         nCnt=0;
      }

   };

   JList<RXStruct> RXList;
   TCriticalSection* pLock;
   String sRebootNow;

public:

   Vista_ProcWatch_RX();
   ~Vista_ProcWatch_RX();

   void Init()
   {
	   pLock->Acquire();
      RXList.Clear();
   	pLock->Release();
   }

   void RXMsg(const int nID, const DWORD dwT_utc_sec, const String sSiteDescr="");

   void ClearSource(const int nSourceID);

   int nGetSourceCnt() const
   {
      return RXList.nGetCount();
   }

   int nGetSourceOffset(const int nID);

   String sGetSourceInfo(const int n)
   {
      return RXList[n]->sDescr;
   }

	int nGetSourceCount(const int n)
   {
      return RXList[n]->nCnt;
   }

	int nGetSourceLeft_sec(const int n, const DWORD dwCurrent_utc_sec, const DWORD dwTimeout_sec)
   {
      int nLeft_sec=dwCurrent_utc_sec-RXList[n]->dwLast_utc_sec;
      nLeft_sec=(int)dwTimeout_sec-nLeft_sec;
      return nLeft_sec;
   }

   String sWatchLine(const int n, const DWORD dwCurrent_utc_sec, const DWORD dwTimeout_sec)
   {
      String s=sGetSourceInfo(n)+"\t";
      String sVal;
      sVal.printf(L"%8d",nGetSourceCount(n));
      s=s+sVal;
      int nSourceLeft_sec=nGetSourceLeft_sec(n,dwCurrent_utc_sec,dwTimeout_sec);
      if (nSourceLeft_sec<=0)
         sRebootNow=RXList[n]->sDescr;
      sVal.printf(L"%4d",nSourceLeft_sec);
      s=s+sVal;
      return s;
   }

   String sReboot() const
   {
   	return sRebootNow;
   }

};

#pragma option pop    //Set compiler back to default alignment


#endif
