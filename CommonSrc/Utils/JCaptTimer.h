//---------------------------------------------------------------------------

#ifndef JCaptTimerH
#define JCaptTimerH
//---------------------------------------------------------------------------

class CaptTimer
{

   TCriticalSection* pLock;
   TTimer* pTimer;

   int nCnt;
   bool bStopTimer;

   void __fastcall TimerTimer(TObject *Sender);

public:
   CaptTimer(TComponent* pOwnerForm, const int nInterval_ms);
   ~CaptTimer();

   void Enable();

   void Disable();

   int nGetCnt()
   {
      if (bEnabled()&&(pLock))
         {
         int n;
         pLock->Acquire();
         n=nCnt;
         nCnt=0;
         pLock->Release();
         return n;
         }
      return 0;
   }

   bool bEnabled() const
   {
      if (pLock)
         {
         bool bEn;
         pLock->Acquire();
         bEn=pTimer->Enabled;
         pLock->Release();
         return bEn;
         }
      return false;
   }

};

#endif
