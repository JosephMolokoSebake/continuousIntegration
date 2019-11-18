//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "NMEADlg.h"
#include "JFileDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TNMEAForm *NMEAForm;
//---------------------------------------------------------------------------
__fastcall TNMEAForm::TNMEAForm(TComponent* Owner)
   : TForm(Owner)
{

   pRawFil=NULL;
   bAllText=false;
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::FormShow(TObject *Sender)
{
   ClearButClick(Sender);
   CloseBut->SetFocus();
   bStopTimer=false;

   pRawFil=new JFile('O');
   pRawFil->Create(L"RAW_DATA_"+JTime(true).sFileName(L"txt"));

   sRX=L"";
   Timer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::FormClose(TObject *, TCloseAction &)
{
   bStopTimer=true;
   while(Timer->Enabled)
      Application->ProcessMessages();
   delete pRawFil;
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::ClearButClick(TObject *)
{
   Memo->Clear();
   SentenceEd->SetVal(Memo->Lines->Count);
   StoreBut->Enabled=(Memo->Lines->Count>0);
   ClearBut->Enabled=(Memo->Lines->Count>0);
   if (pUsb)
      pUsb->Reset();
   else if (pCom)
      pCom->Reset();
   else if (pCommsTD)
      {
      Decode.Reset();
      pCommsTD->ResetUART();
      }
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::StoreButClick(TObject *Sender)
{
   bStopTimer=true;
   while(Timer->Enabled)
      Application->ProcessMessages();
   String sFile=sSelectSaveFile(L"Text Files (*.txt)",L"*.txt",L"Specify Log File");
   if (!sFile.IsEmpty())
      {
      JFile Fil('O',JFile::ASCII_TYPE);
      JFile::FILE_ERROR E=Fil.Create(sFile);
      for (int i=0; (i<Memo->Lines->Count)&&(!E); i++)
			E=Fil.TextLine(Memo->Lines->Strings[i]);
      Fil.ShowError(E);
      }
	ClearButClick(Sender);
   bStopTimer=false;
   Timer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::TimerTimer(TObject *)
{
   String sSec;
   sSec.printf(L"%9.3f   ",JTime(true).dSec());
   if (pUsb)
      {
      if (bAllText)
         {
         char acRXData[4096];
         int nRX=pUsb->nRXUARTData(acRXData,sizeof(acRXData));

         pRawFil->Write(acRXData,nRX);

         for (int i=0; i<nRX; i++)
            {
            switch(acRXData[i])
               {
               case '\r':
                  if (!sRX.IsEmpty())
                     Memo->Lines->Add(sSec+sRX);
                  sRX=L"";
                  break;
               case 2:
                  if (!sRX.IsEmpty())
                     Memo->Lines->Add(sSec+sRX);
                  Memo->Lines->Add(sSec+"****** Start of Message ******");
                  sRX=L"";
                  break;
               default:
                  sRX=sRX+acRXData[i];
               }
            }
         }
      else
         {
         pUsb->nUpdate();
         do
            {
            sRX=pUsb->sGetSentence();
            if (!sRX.IsEmpty())
               Memo->Lines->Add(sSec+sRX);
            }
         while(!sRX.IsEmpty());
         CheckEd->SetVal(pUsb->nGetChecksumErrors());
         }
      }
   else if (pCommsTD)
      {
      BYTE* pucRX=new BYTE[8192];
      int nSize=pCommsTD->nRXUARTData(pucRX,8192);
      if (nSize>0)
         pRawFil->Write(pucRX,nSize);
      if (bAllText)
         {
         String s=L"";
         for (int i=0; i<nSize; i++)
            {
            if (pucRX[i]=='\r')
               {
               if (!s.IsEmpty())
                  Memo->Lines->Add(sSec+s);
               s=L"";
               }
            else
               s=s+pucRX[i];
            }
         }
      else
         {
         for (int i=0; i<nSize; i++)
            Decode.AddByte(pucRX[i]);
         delete[] pucRX;
         do
            {
            sRX=Decode.sGetSentence();
            if (!sRX.IsEmpty())
               Memo->Lines->Add(sSec+sRX);
            }
         while(!sRX.IsEmpty());
         CheckEd->SetVal(Decode.nGetChecksumErrors());
         }
      }
   else if (pCom)
      {
      do
         {
         sRX=pCom->sGetSentence();
         if (!sRX.IsEmpty())
            Memo->Lines->Add(sSec+sRX);
         }
      while(!sRX.IsEmpty());
      CheckEd->SetVal(pCom->nGetChecksumErrors());
      }
   SentenceEd->SetVal(Memo->Lines->Count);
   StoreBut->Enabled=(Memo->Lines->Count>0);
   ClearBut->Enabled=(Memo->Lines->Count>0);
   if (bStopTimer)
      Timer->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TNMEAForm::FormCreate(TObject *)
{
   pCommsTD=NULL;
   pUsb=NULL;
   pCom=NULL;
}
//---------------------------------------------------------------------------
