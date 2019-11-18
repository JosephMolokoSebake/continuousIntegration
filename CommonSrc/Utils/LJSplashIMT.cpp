//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "LJSplashIMT.h"
#include "JUtils.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplashForm *SplashForm;
//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner)
	: TForm(Owner)
{
	nSplashTime=3000;
	nMonitor=0;
	sSplashFile="";
	sTitle="";
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormShow(TObject *)
{
	if ((nMonitor==1)&&(Screen->MonitorCount>1))
		Left=Screen->Monitors[1]->Left+(Screen->Monitors[1]->Width-Width)/2;
	else
		Left=(Screen->Monitors[0]->Width-Width)/2;
	Top=(Screen->Monitors[0]->Height-Height)/2;
	DWORD dwHandle;
	String s,sTmp;
	TCHAR szDescr[256];
	s=ParamStr(0);
	DWORD dwSize=::GetFileVersionInfoSize(s.c_str(),&dwHandle);
	if (dwSize>0)
	{
		TCHAR* pcVer=new TCHAR[dwSize];
		void* pVer;
		UINT uLen;
		bool bRet=::GetFileVersionInfo(s.c_str(),dwHandle,dwSize,pcVer);
		if (bRet)
			{
			bRet=::VerQueryValue(pcVer,L"\\StringFileInfo\\1C0904E4\\FileDescription",&pVer,&uLen);		//Try RSA
			if (!bRet)
				bRet=::VerQueryValue(pcVer,L"\\StringFileInfo\\040904E4\\FileDescription",&pVer,&uLen);	//Try USA
			}
		if (bRet)
		{
			::wcsncpy(szDescr,(TCHAR*)pVer,255);
			labelCaption->Caption=szDescr;
		}

		if (!sTitle.IsEmpty())
			labelCaption->Caption=sTitle;

		String sVersion="Version";
		String sBuild="Build";
		String sBeta="BETA";
		String sRef="Reference";
		String sCopy="Copyright";

		#ifdef _CST4_DICT
			g_Dict.Translate(this);	//Form Caption
			sVersion=g_Dict.sTranslate(sVersion);
			sBuild=g_Dict.sTranslate(sBuild);
			sBeta=g_Dict.sTranslate(sBeta);
			sRef=g_Dict.sTranslate(sRef);
			sCopy=g_Dict.sTranslate(sCopy);
		#endif

		if (bRet)
			bRet=::VerQueryValue(pcVer,L"\\",&pVer,&uLen);
		if (bRet)
		{
			VS_FIXEDFILEINFO VerInfo;
			memcpy(&VerInfo,pVer,sizeof(VS_FIXEDFILEINFO));
			sTmp.printf(L"%u.%u.%u",VerInfo.dwFileVersionMS>>16,VerInfo.dwFileVersionMS&0x0000FFFF,VerInfo.dwFileVersionLS>>16);
			s=sVersion+" "+sTmp;
			sTmp.printf(L"%02u",VerInfo.dwFileVersionLS&0x0000FFFF);
			s+=" "+sBuild+" "+sTmp;
			if (VerInfo.dwFileFlags&VS_FF_PRERELEASE)
				s+=sBeta;
			labelVer->Caption=s;
		}
		if (bRet)
			{
			bRet=::VerQueryValue(pcVer,TEXT("\\StringFileInfo\\1C0904E4\\InternalName"),&pVer,&uLen);
			if (!bRet)
				bRet=::VerQueryValue(pcVer,TEXT("\\StringFileInfo\\040904E4\\InternalName"),&pVer,&uLen);
			}
		if (bRet)
		{
			wcsncpy(szDescr,(TCHAR*)pVer,255);
			labelRef->Caption=sRef+" "+String(szDescr);
		}
		if (bRet)
			{
			bRet=::VerQueryValue(pcVer,TEXT("\\StringFileInfo\\1C0904E4\\LegalCopyright"),&pVer,&uLen);
			if (!bRet)
				bRet=::VerQueryValue(pcVer,TEXT("\\StringFileInfo\\040904E4\\LegalCopyright"),&pVer,&uLen);
			}
		if (bRet)
			{
			wcsncpy(szDescr,(TCHAR*)pVer,255);
			labelCopy->Caption=sCopy+" "+String(szDescr);
			}
		if (!bRet)
			ShowLastError(L"Splash Screen");
		delete[] pcVer;
	}
	if (!sSplashFile.IsEmpty())
		if (bFileExist(sSplashFile))
			imageSplash->Picture->LoadFromFile(sSplashFile);
	labelCaption->Left=(Width-labelCaption->Width)/2;
	labelCopy->Left=(Width-labelCopy->Width)/2;
	labelVer->Left=(Width-labelVer->Width)/2;
	labelRef->Left=(Width-labelRef->Width)/2;

   PB->Repaint();
   Application->ProcessMessages();
//   Repaint();

	if (nSplashTime>0)
		timerSplash->Interval=nSplashTime;
	timerSplash->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::timerSplashTimer(TObject *)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::imageSplashClick(TObject *)
{
	Close();
}
//---------------------------------------------------------------------------

void TSplashForm::SetSplashFile(const String _sSplashFile)
{
   sSplashFile=_sSplashFile;
}

void __fastcall TSplashForm::PBPaint(TObject */*Sender*/)
{
	TCanvas* pC=PB->Canvas;
	pC->Pen->Width=5;
	pC->Pen->Style=psSolid;
	pC->Pen->Color=clBlack;
	pC->Brush->Style=bsClear;
   TRect R;
   R.left=0;
   R.right=PB->Width;
   R.top=0;
   R.bottom=PB->Height;
	pC->Rectangle(R);

}
//---------------------------------------------------------------------------

void __fastcall TSplashForm::PBClick(TObject */*Sender*/)
{
	Close();
}
//---------------------------------------------------------------------------

void ShowIMTSplash(const int nDelay_sec, const String sTitle)
{
	SplashForm=new TSplashForm(0);
	SplashForm->SetSplashTime(nDelay_sec*1000);
	SplashForm->SetTitle(sTitle);
	SplashForm->ShowModal();
	delete SplashForm;
}

void __fastcall TSplashForm::FormKeyPress(TObject */*Sender*/, System::WideChar &/*Key*/)

{

	Close();
}
//---------------------------------------------------------------------------

