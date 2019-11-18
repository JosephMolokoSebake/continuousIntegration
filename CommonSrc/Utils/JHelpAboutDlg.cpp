//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JHelpAboutDlg.h"
#include "JUtils.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJHelpAboutForm *JHelpAboutForm;
//---------------------------------------------------------------------------
__fastcall TJHelpAboutForm::TJHelpAboutForm(TComponent* Owner)
   : TForm(Owner)
{
	sAltDescr;"";
}
//---------------------------------------------------------------------------

void __fastcall TJHelpAboutForm::FormShow(TObject *)
{
	DWORD dwHandle;
	TCHAR szDescr[256];
	int nMinWidth=243;
	String s=ParamStr(0);
	DWORD dwSize=::GetFileVersionInfoSize(s.c_str(),&dwHandle);
	if (dwSize>0)
		{
		TCHAR* pcVer=new TCHAR[dwSize];
		void* pVer;
		UINT uLen;
		bool bRet=::GetFileVersionInfo(s.c_str(),dwHandle,dwSize,pcVer);
		if (bRet)
			bRet=::VerQueryValue(pcVer,L"\\",&pVer,&uLen);
		if (bRet)
         {
         VS_FIXEDFILEINFO VerInfo;
         memcpy(&VerInfo,pVer,sizeof(VS_FIXEDFILEINFO));
         s.printf(L"%u.%u.%u",VerInfo.dwFileVersionMS>>16,VerInfo.dwFileVersionMS&0x0000FFFF,VerInfo.dwFileVersionLS>>16);
         VerEd->Text=s;
         s.printf(L"%02u",VerInfo.dwFileVersionLS&0x0000FFFF);
         BuildEd->Text=s;
         if (VerInfo.dwFileFlags&VS_FF_PRERELEASE)
            {
            DebugLab->Show();
            nMinWidth+=40;
            }
         }
		if (bRet)
			bRet=::VerQueryValue(pcVer,L"\\StringFileInfo\\1C0904E4\\FileDescription",&pVer,&uLen);
		if (bRet)
			{

			if (sAltDescr.IsEmpty())
				{
				wcsncpy(szDescr,(TCHAR*)pVer,255);
				DescrLab->Caption=szDescr;
				}
			else
				{
				DescrLab->Caption=sAltDescr;
				sAltDescr="";
				}
			}
		if (bRet)
			bRet=::VerQueryValue(pcVer,L"\\StringFileInfo\\1C0904E4\\InternalName",&pVer,&uLen);
		if (bRet)
			{
			wcsncpy(szDescr,(TCHAR*)pVer,255);
			RefEd->Text=szDescr;
			}
		if (bRet)
			bRet=::VerQueryValue(pcVer,TEXT("\\StringFileInfo\\1C0904E4\\LegalCopyright"),&pVer,&uLen);
		if (bRet)
			{
			wcsncpy(szDescr,(TCHAR*)pVer,255);
			CopyEd->Text=szDescr;
			}
		if (!bRet)
			ShowLastError("Help About Dialog");
		delete[] pcVer;
		}
	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(DescrLab);
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label3);
		g_Dict.Translate(Label4);
		g_Dict.Translate(Label5);
	#endif
	int nWidth=DescrLab->Width;
	if (nWidth<nMinWidth) nWidth=nMinWidth;
	ClientWidth=64+nWidth;
   MainPan->Width=ClientWidth-22;
   MainBev->Width=MainPan->Width-40;
   MainPan->SetFocus();
}
//---------------------------------------------------------------------------

void ShowHelpAbout(TComponent* pOwner)
{
	JHelpAboutForm=new TJHelpAboutForm(pOwner);
	JHelpAboutForm->ShowModal();
	delete JHelpAboutForm;
}

