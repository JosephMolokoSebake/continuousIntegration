//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "JCaptTimer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CaptTimer::CaptTimer(TComponent* pOwnerForm, const int nInterval_ms)
{
   pLock=new TCriticalSection;
	pTimer=new TTimer(pOwnerForm);
	pTimer->Enabled=false;
	pTimer->Interval=nInterval_ms;
	pTimer->OnTimer=TimerTimer;
	bStopTimer=false;
   Enable();

}

CaptTimer::~CaptTimer()
{
   Disable();
   delete pTimer;
   delete pLock;
   pLock=NULL;
}

void CaptTimer::Enable()
{
	bStopTimer=false;
   nCnt=0;
   pTimer->Enabled=true;
}

void CaptTimer::Disable()
{
   pLock->Acquire();
   bStopTimer=true;
   pLock->Release();
   DWORD dwT_ms=::GetTickCount();
   do
      {
      Application->ProcessMessages();
      }
   while((pTimer->Enabled)&&(::GetTickCount()-dwT_ms<1000));
   pTimer->Enabled=false;   //Make sure!
}

void __fastcall CaptTimer::TimerTimer(TObject *Sender)
{
   pLock->Acquire();
   if (bStopTimer)
      pTimer->Enabled=false;
   else
	   ++nCnt;
   pLock->Release();
}

