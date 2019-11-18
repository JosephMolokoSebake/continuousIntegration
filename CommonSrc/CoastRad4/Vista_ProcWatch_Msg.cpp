//---------------------------------------------------------------------------

#include "jpch.h"

#pragma hdrstop

#include "Vista_ProcWatch_Msg.h"
#include "Cst4SiteDefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Vista_ProcWatch_TX::Vista_ProcWatch_TX(TComponent* pOwnerForm, const int _nSiteID, const int nTXInt_sec)
{
   nTXCnt=0;
   nSiteID=_nSiteID;
   sSiteDescr="";
   dwTXInt_sec=(WORD)nTXInt_sec;
	sClientStatus="Connecting ...";
   ClientStatusCol=clYellow;
   pClient=new VistaClient;
   pClient->bInitTX(pOwnerForm,"127.0.0.1",VISTA_PROCWATCH_PORT);   //ProcWatch always resides on the local PC
   DWORD dwT_ms=::GetTickCount();
   do
      Application->ProcessMessages();
   while(::GetTickCount()-dwT_ms<1000);
   bTX_OK=pClient->bVerifyConnection(bClientStatusChanged,sClientStatus,ClientStatusCol);
   dwNextTX_utc_sec=0;
}

Vista_ProcWatch_TX::Vista_ProcWatch_TX(TComponent* pOwnerForm, const String _sSiteDescr, const int nTXInt_sec)
{
   nTXCnt=0;
   randomize();
   nSiteID=random(65536);
   sSiteDescr=_sSiteDescr;
   dwTXInt_sec=(WORD)nTXInt_sec;
	sClientStatus="Connecting ...";
   ClientStatusCol=clYellow;
   pClient=new VistaClient;
   pClient->bInitTX(pOwnerForm,"127.0.0.1",VISTA_PROCWATCH_PORT);   //ProcWatch always resides on the local PC
   DWORD dwT_ms=::GetTickCount();
   do
      Application->ProcessMessages();
   while(::GetTickCount()-dwT_ms<1000);
   bTX_OK=pClient->bVerifyConnection(bClientStatusChanged,sClientStatus,ClientStatusCol);
   dwNextTX_utc_sec=0;
}

Vista_ProcWatch_TX::~Vista_ProcWatch_TX()
{
   delete pClient;
}

bool Vista_ProcWatch_TX::bTX(const JTime& T_utc, const bool bOn)
{
   bTX_OK=pClient->bVerifyConnection(bClientStatusChanged,sClientStatus,ClientStatusCol);
   if (bTX_OK)
      {
      Cst4_User_Msg Msg;
      char szDescr[512];
      int nL=0;
      if (!sSiteDescr.IsEmpty())
         {
         AnsiString as=sSiteDescr;
         strcpy(szDescr,as.c_str());
         nL=as.Length();
         szDescr[nL]=0;
         ++nL;
         }
      DWORD dwType;
      if (bOn)
         dwType=CST4_USER_PROCWATCH_ON;
      else
         dwType=CST4_USER_PROCWATCH_OFF;
      if (nL>0)
      	Msg.Set(T_utc,nSiteID,dwType,szDescr,nL);
      else
      	Msg.Set(T_utc,nSiteID,dwType,NULL,0);
      pClient->AddTXMessage(Msg);
      bTX_OK=pClient->bTXMessages();
      if (bTX_OK)
         ++nTXCnt;
      }
   return bTX_OK;
}

bool Vista_ProcWatch_TX::bTX(const DWORD dwT_utc_sec, const bool bOn)
{
	JTime T_utc;
   T_utc.SetTotalSec(dwT_utc_sec);
   return bTX(T_utc,bOn);
}

bool Vista_ProcWatch_TX::bTX(const bool bOn)
{
   return bTX(JTime(true,true),bOn);
}

void Vista_ProcWatch_TX::InitTX()
{
   dwNextTX_utc_sec=0;
}

bool Vista_ProcWatch_TX::bCheckTX(const DWORD dwT_utc_sec, const bool bOn)
{
   if (dwT_utc_sec>=dwNextTX_utc_sec)
      {
      dwNextTX_utc_sec=dwT_utc_sec+dwTXInt_sec;
      return bTX(dwT_utc_sec,bOn);
      }
   return false;
}

bool Vista_ProcWatch_TX::bCheckTX(const JTime& T_utc, const bool bOn)
{
  	DWORD dwT_utc_sec=T_utc.dwTotalSec();
   return bCheckTX(dwT_utc_sec,bOn);
}

bool Vista_ProcWatch_TX::bCheckTX(const bool bOn)
{
   return bCheckTX(JTime(true,true),bOn);
}

//************************************************************************************

Vista_ProcWatch_RX::Vista_ProcWatch_RX()
{
   pLock=new TCriticalSection;
   sRebootNow="";
}

Vista_ProcWatch_RX::~Vista_ProcWatch_RX()
{
   delete pLock;
}

void Vista_ProcWatch_RX::RXMsg(const int nID, const DWORD dwT_utc_sec, const String sSiteDescr)
{
   pLock->Acquire();
   RXStruct* pRX=NULL;
   RXStruct* p;
   RXList.GoFirst();
   while(!RXList.bLast() && !pRX)
      {
      p=RXList.pNext();
      if (p->nID==nID)
         pRX=p;
      }
   if (!pRX)
      {
      pRX=new RXStruct;
	   pRX->nID=nID;
      if (sSiteDescr.IsEmpty())
	      pRX->sDescr=sGetCst4SiteNameOnly(nID);
      else
         pRX->sDescr=sSiteDescr;
	   RXList.nAdd(pRX);
	   RXList.Pack();
      }
	pRX->dwLast_utc_sec=dwT_utc_sec;
   ++pRX->nCnt;
   pLock->Release();
}

void Vista_ProcWatch_RX::ClearSource(const int nSourceID)
{
   pLock->Acquire();
   bool bFound=false;
   RXList.GoFirst();
   while(!RXList.bLast() && !bFound)
      {
      if (RXList.pNext()->nID==nSourceID)
         {
         bFound=true;
         RXList.DeletePrev();
         }
      }
   RXList.Pack();
   pLock->Release();
}

int Vista_ProcWatch_RX::nGetSourceOffset(const int nID)
{
   int nOffs=-1;
   int n=0;
   RXList.GoFirst();
   while(!RXList.bLast() && (nOffs==-1))
      {
      if (RXList.pNext()->nID==nID)
         nOffs=n;
      else
      	n++;
      }
   return nOffs;

}




