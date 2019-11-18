//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JFileDlg.h"
#include "JInfoDlg.h"
#include "JIniFile.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJFileForm *JFileForm;

//---------------------------------------------------------------------------
__fastcall TJFileForm::TJFileForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TJFileForm::FormCreate(TObject *)
{
	sDir=L"";
	sCurrentMask="*.*";
	sTitle=sGet("Select File");
	nFilterIndex=0;
	Sort=DATE_DOWN_SORT;
	bCheckExist=false;
	pDirHistory=new TStringList;
	pFav=new TStringList;
	ReadSettings();
	sFileName="";
}
//---------------------------------------------------------------------------
void __fastcall TJFileForm::FormShow(TObject *)
{
	DlgState=NM_DIALOG_OPEN;
	ShowHeading();
	MaskList.Pack();
	MaskCombo->Clear();
	for (int i=0; i<MaskList.nGetCount(); i++)
		MaskCombo->Items->Add(MaskList[i]->sDescr);
	if (nFilterIndex>=MaskList.nGetCount())
		nFilterIndex=0;
	MaskCombo->ItemIndex=nFilterIndex;
	MaskCombo->Enabled=(MaskCombo->Items->Count>1);
	if (MaskList.nGetCount()>0)
		sCurrentMask=MaskList[nFilterIndex]->sMask;
	else
      sCurrentMask="*.*";
	if ((sDir.IsEmpty())||(!bDirExist(sDir)))
		sDir=sGetDefaultDir();
	ReadDir();
	ShowFiles();
	FileEd->Text=sFileNameOnly(sFileName);
	OKBut->Enabled=!FileEd->Text.IsEmpty();
	FavList->Hide();
	Caption=sTitle;
	if (bCheckExist)
		FileEd->SetFocus();
	else
		CancelBut->SetFocus();

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(TreeBut);
		g_Dict.Translate(RootBut);
		g_Dict.Translate(FavBut);
		g_Dict.Translate(Label3);
		g_Dict.Translate(NameLab);
		g_Dict.Translate(TypeLab);
	#endif


}
//---------------------------------------------------------------------------
void __fastcall TJFileForm::FormClose(TObject *,
		TCloseAction &Action)
{
	if (Action==Forms::caHide)
		DlgState=NM_DIALOG_CANCEL;
	if (ModalResult==mrOk)
		{
		sFileName=IncludeTrailingBackslash(sDir)+sFileNameOnly(FileEd->Text);
		UpdateDirHistory(ExcludeTrailingBackslash(sDir));
		StoreSettings();
		}
	else
		sFileName=L"";
}
//---------------------------------------------------------------------------
void __fastcall TJFileForm::FormDestroy(TObject *)
{
   delete pFav;
   delete pDirHistory;
}
//---------------------------------------------------------------------------

String TJFileForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}

void __fastcall TJFileForm::RootButClick(TObject *)
{
   String s=sDir;
   bool bNetwork=(s.Pos(L"\\\\")==1);
   int nSlash=0;
   int nNum;
   if (bNetwork)
      nNum=4;
   else
      nNum=1;
   int nSize=0;
   for (int i=1; (i<=s.Length())&&(nSize==0); i++)
      {
      if (s[i]=='\\')
         {
         nSlash++;
         if (nSlash==nNum)
            nSize=i;
         }
      }
   if ((nSlash==nNum)&&(nNum<s.Length()))
      {
		sDir=s.SubString(1,nSize);
      ReadDir();
      ShowFiles();
      }
}
//---------------------------------------------------------------------------
void __fastcall TJFileForm::TreeButClick(TObject *)
{
   sDir=sSelectFolder(this,sDir);
   ReadDir();
   ShowFiles();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::FileGridClick(TObject *)
{
   FavList->Hide();
   int n=FileGrid->Row;
   if (n==0)
      {
      ShowHeading();
      }
   else
      {
      if (n>=nLastDirRow)
         {
         DirEntryStruct* pE=FileEntryList[n-nLastDirRow];
//         FileEd->Text=IncludeTrailingBackslash(sDir)+pE->sFile;
         FileEd->Text=pE->sFile;
         if (!pE->sExt.IsEmpty())
            FileEd->Text=FileEd->Text+"."+pE->sExt;
         OKBut->Enabled=true;
         }
      else
			FileEd->Text=L"";
      }
}

//---------------------------------------------------------------------------

void __fastcall TJFileForm::ReadDir()
{
   Screen->Cursor=crHourGlass;
	ShowDirCombo(sDir);
   FileEntryList.Clear();
   DirEntryList.Clear();
   WIN32_FIND_DATA FileData;
	String sFullMask=IncludeTrailingBackslash(sDir)+"*.*";
   HANDLE hFile=::FindFirstFile(sFullMask.c_str(),&FileData);
   if (hFile!=INVALID_HANDLE_VALUE)
      {
      do
         {
   		if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0)
            {
            DirEntryStruct* pE=new DirEntryStruct;
            pE->bDir=true;
            if (bGetDirInfo(FileData,pE))
               DirEntryList.nAdd(pE);
            else
               delete pE;
            }
         }
      while(::FindNextFile(hFile,&FileData));
      ::FindClose(hFile);
      }
   DirEntryList.Pack();
   sFullMask=IncludeTrailingBackslash(sDir)+sCurrentMask;
   String sFullExt=ExtractFileExt(sFullMask);
   bool bAll=(sFullExt.Pos(L"*")>0)||(sFullExt.Pos(L"?")>0);
   hFile=::FindFirstFile(sFullMask.c_str(),&FileData);
   if (hFile!=INVALID_HANDLE_VALUE)
      {
		do
         {
   		if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
            {
				String sExt=ExtractFileExt(FileData.cFileName);
				String sFullExt1=sFullExt;
				if ((sFullExt1.CompareIC(sExt)==0)||(bAll))
					{
					DirEntryStruct* pE=new DirEntryStruct;
               pE->bDir=false;
               if (bGetDirInfo(FileData,pE))
                  FileEntryList.nAdd(pE);
               else
                  delete pE;
               }
            }
         }
      while(::FindNextFile(hFile,&FileData));
      ::FindClose(hFile);
      }
   FileEntryList.Pack();
   SortFiles(false);
   Screen->Cursor=crDefault;
}

void __fastcall TJFileForm::ShowFiles(const bool bSetRow)
{
   int nLines=DirEntryList.nGetCount()+FileEntryList.nGetCount()+1;
   if (nLines<2) nLines=2;
   FileGrid->RowCount=nLines;
   if (nLines==2)
      {
      for (int i=0; i<4; i++)
         FileGrid->Cells[i][1]=L"";
      }
   int nR=1;
   for (int i=0; i<DirEntryList.nGetCount(); i++)
      {
      ShowEntry(DirEntryList[i],nR);
      nR++;
      }
   nLastDirRow=nR;
   for (int i=0; i<FileEntryList.nGetCount(); i++)
      {
      ShowEntry(FileEntryList[i],nR);
      nR++;
      }
   if (bSetRow)
      FileGrid->Row=1;
}

void __fastcall TJFileForm::FileGridDblClick(TObject *)
{
   int n=FileGrid->Row;
   if (n>0)
      {
      DirEntryStruct* pE;
      if (n<nLastDirRow)
         {
         pE=DirEntryList[n-1];
         if (pE->sFile==L"..")
            {
            sDir=ExcludeTrailingBackslash(sDir);
            for (int i=sDir.Length(); i>=1; i--)
               if (sDir[i]=='\\')
                  {
						sDir=sDir.SubString(1,i);
                  i=0;
                  }
            }
         else
            {
            sDir=IncludeTrailingBackslash(sDir)+pE->sFile;
            if (!pE->sExt.IsEmpty())
               sDir=sDir+"."+pE->sExt;
            }
         sDir=IncludeTrailingBackslash(sDir);
         ReadDir();
         ShowFiles();
         }
      else
         {
         pE=FileEntryList[n-nLastDirRow];
         FileEd->Text=pE->sFile;
         if (!pE->sExt.IsEmpty())
            FileEd->Text=FileEd->Text+"."+pE->sExt;
         if (bSelValid())
            {
            sFileName=IncludeTrailingBackslash(sDir)+sFileNameOnly(FileEd->Text);
            UpdateDirHistory(ExcludeTrailingBackslash(sDir));
            StoreSettings();
            DlgState=NM_DIALOG_OK;
            ModalResult=mrOk;
            }
         }
      }
}
//---------------------------------------------------------------------------

/*
String __fastcall TJFileForm::sSelectDir()
{
   LPITEMIDLIST pidlRoot=NULL;
   BROWSEINFO bi={0};
   LPMALLOC pMalloc=NULL;
   char szName[1024];
   SHGetMalloc(&pMalloc);
   bi.hwndOwner=Handle;
   bi.pidlRoot=pidlRoot;
   bi.pszDisplayName=szName;
   bi.lpszTitle=L"Select a folder";
   bi.ulFlags=BIF_RETURNONLYFSDIRS;
   bi.lpfn=NULL;
   bi.lParam=0;
   LPITEMIDLIST pidlSelected=SHBrowseForFolder(&bi);
   if(pidlRoot)
      pMalloc->Free(pidlRoot);
   pMalloc->Release();
   if (pidlSelected)
      SHGetPathFromIDList(pidlSelected,szName);
   else
      szName[0]=0;
   return IncludeTrailingBackslash(String(szName));
}
*/

bool __fastcall TJFileForm::bGetDirInfo(const WIN32_FIND_DATA& FileData, DirEntryStruct* pE)
{
   if (FileData.cFileName[0]!='.')
      {
		wchar_t szDrive[MAXDRIVE];
		wchar_t szDir[MAXDIR];
		wchar_t szFile[MAXFILE];
		wchar_t szExt[MAXEXT];
		::_wfnsplit(FileData.cFileName,szDrive,szDir,szFile,szExt);
		pE->sFile=szFile;
      if (szExt[0])
         pE->sExt=&szExt[1];
      else
         pE->sExt=L"";
      pE->dwSize=FileData.nFileSizeLow;
      ::FileTimeToLocalFileTime(&FileData.ftLastWriteTime,&pE->FT);
      ::FileTimeToSystemTime(&pE->FT,&pE->T);
      return true;
      }
   else
      {
      if (FileData.cFileName[1]=='.')
         {
         pE->sFile=L"..";
         pE->sExt=L"";
         pE->dwSize=0;
         memset(&pE->T,0,sizeof(pE->T));
         return true;
         }
      else
         return false;
      }
}

void __fastcall TJFileForm::ShowEntry(const DirEntryStruct* pE, const int nR)
{
   String s;
   s.printf(L"%02u:%02u:%02u  %02u/%02u/%04u",pE->T.wHour,pE->T.wMinute,pE->T.wSecond,
                                             pE->T.wDay,pE->T.wMonth,pE->T.wYear);
   FileGrid->Cells[0][nR]=s;
   FileGrid->Cells[2][nR]=pE->sExt;
   if (pE->bDir)
      {
      FileGrid->Cells[1][nR]=L"["+pE->sFile+"]";
      FileGrid->Cells[3][nR]=L"<DIR>";
      }
   else
      {
      FileGrid->Cells[1][nR]=pE->sFile;
      if (pE->dwSize>99999999)
         FileGrid->Cells[3][nR]=String(pE->dwSize/(1024*1024))+" MB";
      else if (pE->dwSize>99999)
         FileGrid->Cells[3][nR]=String(pE->dwSize/1024)+" kB";
      else
         FileGrid->Cells[3][nR]=String(pE->dwSize);
      }
}

void __fastcall TJFileForm::ShowDirCombo(const String sDir)
{
   DirCombo->Items->Assign(pDirHistory);
   int nIndex=pDirHistory->IndexOf(ExcludeTrailingBackslash(sDir));
   if (nIndex==-1)
      nIndex=DirCombo->Items->Add(ExcludeTrailingBackslash(sDir));
   DirCombo->ItemIndex=nIndex;
}

void __fastcall TJFileForm::UpdateDirHistory(const String sDir)
{
   int nIndex=pDirHistory->IndexOf(sDir);
   if (nIndex>=0)
      pDirHistory->Delete(nIndex);
   if (pDirHistory->Count>=20)
      pDirHistory->Delete(0); //Delete oldest one
   pDirHistory->Add(sDir);
}

void __fastcall TJFileForm::DirComboChange(TObject *)
{
	sDir=DirCombo->Items->Strings[DirCombo->ItemIndex];
   ReadDir();
   ShowFiles();   
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::StoreSettings()
{

   JIniFile Ini(ChangeFileExt(Application->ExeName,L".JFileIni"),true);
   Ini.Write(L"DEFAULTS",L"Folder",sDir);
   Ini.Write(L"File Name",sFileName);
   Ini.Write(L"Title",sTitle);
   Ini.Write(L"Filter Index",nFilterIndex);
   Ini.Write(L"Sort",Sort);
   Ini.Write(L"WINDOW",L"Width",Width);
   Ini.Write(L"Height",Height);

   Ini.Write(L"HISTORY",L"Number",pDirHistory->Count);
   for (int i=0; i<pDirHistory->Count; i++)
		Ini.Write(L"Dir"+String(i),pDirHistory->Strings[i]);

   Ini.Write(L"FAVORITES",L"Number",pFav->Count);
   for (int i=0; i<pFav->Count; i++)
		Ini.Write(L"Dir"+String(i),pFav->Strings[i]);

}

void __fastcall TJFileForm::ReadSettings()
{
	JIniFile Ini(ChangeFileExt(Application->ExeName,".JFileIni"),false,false);
   Ini.Read(L"DEFAULTS",L"Folder",sDir,L"");
   Ini.Read(L"File Name",sFileName,String(L""));
   Ini.Read(L"Title",sTitle,String(L"Select File"));
   Ini.Read(L"Filter Index",nFilterIndex,0);
   Sort=(SORT_TYPE)Ini.nRead(L"Sort",DATE_DOWN_SORT);
   int n;
   Ini.Read(L"WINDOW",L"Width",n,589);
   Width=n;
   Ini.Read(L"Height",n,406);
   Height=n;
   pDirHistory->Clear();
   Ini.Read(L"HISTORY",L"Number",n,0);
   for (int i=0; i<n; i++)
      {
      String s;
      Ini.Read(L"Dir"+String(i),s,String(L""));
      if ((!s.IsEmpty())&&(pDirHistory->IndexOf(s)==-1))
         pDirHistory->Add(s);
      }

   pFav->Clear();
   Ini.Read(L"FAVORITES",L"Number",n,0);
   for (int i=0; i<n; i++)
      {
      String s;
      Ini.Read(L"Dir"+String(i),s,String(L""));
      if (!s.IsEmpty())
         pFav->Add(s);
      }
}

void __fastcall TJFileForm::MaskComboChange(TObject *)
{
   nFilterIndex=MaskCombo->ItemIndex;
   if (nFilterIndex>=0)
      {
      sCurrentMask=MaskList[nFilterIndex]->sMask;
      ReadDir();
      ShowFiles();
      }
   else
      sCurrentMask=L"*.*";
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::AddMask(const String sDescr, const String sMask)
{
   MaskList.nAdd(new MaskStruct(sDescr,sMask));
   MaskList.Pack();
}

String __fastcall TJFileForm::sGetMask()
{
	return MaskList[nFilterIndex]->sDescr;
}

void __fastcall TJFileForm::ShowHeading()
{
	FileGrid->Cells[0][0]=sGet("Date");
	FileGrid->Cells[1][0]=sGet("Name");
	FileGrid->Cells[2][0]=sGet("Ext");
	FileGrid->Cells[3][0]=sGet("Size");
	switch(Sort)
		{
		case DATE_DOWN_SORT:
			FileGrid->Cells[0][0]=String(sGet("Date")+" (-)");
			break;
		case DATE_UP_SORT:
			FileGrid->Cells[0][0]=String(sGet("Date")+" (+)");
			break;
		case NAME_DOWN_SORT:
			FileGrid->Cells[1][0]=String(sGet("Name")+" (-)");
			break;
		case NAME_UP_SORT:
			FileGrid->Cells[1][0]=String(sGet("Name")+" (+)");
			break;
		case EXT_DOWN_SORT:
			FileGrid->Cells[2][0]=String(sGet("Ext")+" (-)");
			break;
		case EXT_UP_SORT:
			FileGrid->Cells[2][0]=String(sGet("Ext")+" (+)");
			break;
		case SIZE_DOWN_SORT:
			FileGrid->Cells[3][0]=String(sGet("Size")+" (-)");
			break;
		case SIZE_UP_SORT:
			FileGrid->Cells[3][0]=String(sGet("Size")+" (+)");
			break;
		default: ;
		}
}


void __fastcall TJFileForm::FileGridMouseDown(TObject *,
		TMouseButton Button, TShiftState, int X, int Y)
{
	bTopRow=false;
	if (Button==mbLeft)
		{
		int nCol,nRow;
		FileGrid->MouseToCell(X,Y,nCol,nRow);
		if (nRow==0)
			{
			bTopRow=true;
			switch(nCol)
				{
				case 0:
					if (Sort==DATE_DOWN_SORT)
						Sort=DATE_UP_SORT;
					else
						Sort=DATE_DOWN_SORT;
					break;
				case 1:
					if (Sort==NAME_DOWN_SORT)
						Sort=NAME_UP_SORT;
					else
						Sort=NAME_DOWN_SORT;
					break;
				case 2:
					if (Sort==EXT_DOWN_SORT)
						Sort=EXT_UP_SORT;
					else
						Sort=EXT_DOWN_SORT;
					break;
				case 3:
					if (Sort==SIZE_DOWN_SORT)
						Sort=SIZE_UP_SORT;
					else
						Sort=SIZE_DOWN_SORT;
					break;
				default: ;
				}
			ShowHeading();
			SortFiles(true);
			ShowFiles(false);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::SortFiles(const bool bPreserveSel)
{
	int nIndex=-1;
	if (bPreserveSel)
		{
		int n=FileGrid->Row;
		if (n>=nLastDirRow)
			{
			nIndex=n-nLastDirRow;
			for (int i=0; i<FileEntryList.nGetCount(); i++)
				FileEntryList[i]->bSel=false;
			FileEntryList[nIndex]->bSel=true;
			}
		}

	for (int i=0; i<FileEntryList.nGetCount(); i++)
		FileEntryList[i]->Sort=Sort;
	FileEntryList.Sort();

	FileGrid->Row=1;
	if (nIndex!=-1)
		{
		for (int i=0; (i<FileEntryList.nGetCount())&&(nIndex!=-1); i++)
			{
			if (FileEntryList[i]->bSel)
				{
				FileGrid->Row=i+nLastDirRow;
				nIndex=-1;
				}
			}
		}
}

/*
int __fastcall TJFileForm::SortFunction(void *a, void *b)
{
	DirEntryStruct* pDa=(DirEntryStruct*)a;
	DirEntryStruct* pDb=(DirEntryStruct*)b;
	switch(Sort)
		{
		case DATE_DOWN_SORT:
			return -::CompareFileTime(&pDa->FT,&pDb->FT);
		case DATE_UP_SORT:
			return ::CompareFileTime(&pDa->FT,&pDb->FT);
		case NAME_DOWN_SORT:
			return -pDa->sFile.Compare(pDb->sFile);
		case NAME_UP_SORT:
			return pDa->sFile.Compare(pDb->sFile);
		case EXT_DOWN_SORT:
			return -pDa->sExt.Compare(pDb->sExt);
		case EXT_UP_SORT:
			return pDa->sExt.Compare(pDb->sExt);
		case SIZE_DOWN_SORT:
			if (pDa->dwSize==pDb->dwSize)
				return 0;
			else if (pDa->dwSize>pDb->dwSize)
				return -1;
			else
				return 1;
		case SIZE_UP_SORT:
			if (pDa->dwSize==pDb->dwSize)
				return 0;
			else if (pDa->dwSize>pDb->dwSize)
				return 1;
			else
				return -1;
		default:
			return 0;
		}
}
*/

void __fastcall TJFileForm::FavButClick(TObject *)
{
	FavList->Items->Clear();
	FavList->Items->Add(sGet("Add Current Folder"));
	FavList->Items->Add(sGet("Delete Current Folder"));
	FavList->Items->Add("---------------------  "+sGet("Cancel")+"  ---------------------");
	for (int i=0; i<pFav->Count; i++)
		FavList->Items->Add(pFav->Strings[i]);
	FavList->Show();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::FormResize(TObject *)
{

	FileGrid->Height=ClientHeight-124+27;
	OKBut->Top=ClientHeight-89+27;
	CancelBut->Top=ClientHeight-57+27;
	NameLab->Top=ClientHeight-85+27;
	TypeLab->Top=ClientHeight-53+27;
	FileEd->Top=ClientHeight-89+27;
	MaskCombo->Top=ClientHeight-57+27;

   DirCombo->Width=ClientWidth-144+8;
   TreeBut->Left=ClientWidth-101+8;
   RootBut->Left=ClientWidth-77+8;
   FavBut->Left=ClientWidth-37+8;
   OKBut->Left=ClientWidth-89+8;
   CancelBut->Left=ClientWidth-89+8;
	FileEd->Width=ClientWidth-180+8;
	MaskCombo->Width=ClientWidth-192+8;

   FileGrid->Width=ClientWidth-20+8;
   FileGrid->ColWidths[1]=ClientWidth-271+8;

   FavList->Left=ClientWidth-273+8;

}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::FavListDblClick(TObject *)
{
   int n=FavList->ItemIndex;
   int nIndex;
   switch(n)
      {
      case 0:
         pFav->Add(ExcludeTrailingBackslash(sDir));
         break;
      case 1:
         nIndex=pFav->IndexOf(ExcludeTrailingBackslash(sDir));
         if (nIndex>=0)
            pFav->Delete(nIndex);
         else
				ShowInfo(sGet("Current Folder Not In List"),sGet("List Error"));
         break;
      case 2:
         break;
      default:
			sDir=pFav->Strings[n-3];
         ReadDir();
         ShowFiles();
      }
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::DirComboClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::FormClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::FileEdClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::MaskComboClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::NameLabClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::TypeLabClick(TObject *)
{
   FavList->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TJFileForm::OKButClick(TObject *)
{
   if (bSelValid())
      {
//      sFileName=FileEd->Text;
      sFileName=IncludeTrailingBackslash(sDir)+sFileNameOnly(FileEd->Text);
      UpdateDirHistory(ExcludeTrailingBackslash(sDir));
      StoreSettings();
      DlgState=NM_DIALOG_OK;
      ModalResult=mrOk;
      }
}
//---------------------------------------------------------------------------

bool __fastcall TJFileForm::bSelValid()
{
   if (ExtractFileExt(FileEd->Text).IsEmpty())
      {
      String sExt=ExtractFileExt(sCurrentMask);
      if ((sExt.Pos(L"*")<1)&&(sExt.Pos(L"?")<1))
         FileEd->Text=sFileNewExt(FileEd->Text,sExt);
      }
/*
   if (ExtractFilePath(FileEd->Text).IsEmpty())
      FileEd->Text=IncludeTrailingBackslash(sDir)+FileEd->Text;
*/
   if (bCheckExist)
      {
      String sFile=IncludeTrailingBackslash(sDir)+FileEd->Text;
//      String sFile=FileEd->Text;
      if (bFileExist(sFile))
         {
			sFile=sFile+sGet(" already exists.")+"\n"+sGet("Replace this file ?");
			return bAskYes(sFile,Caption);
			}
		else
         return true;
      }
   else
      return true;
}

void __fastcall TJFileForm::FileEdChange(TObject *)
{
   OKBut->Enabled=true;
}
//---------------------------------------------------------------------------

TJFileForm::MaskStruct::MaskStruct(const String sD, const String sM)
{
   sDescr=sD;
   sMask=sM;
}

String sSelectOpenFile(const String sMaskDescr, const String sMask, const String sTitle, const String sFolder)
{
   TJFileForm* pF=new TJFileForm(0);
   pF->Position=poMainFormCenter;
   pF->AddMask(sMaskDescr,sMask);
   if (!sTitle.IsEmpty())
      pF->sTitle=sTitle;
   if (!sFolder.IsEmpty())
      pF->sDir=sFolder;
   pF->ShowModal();
   String sFile=pF->sFileName;
   delete pF;
   return sFile;
}

String sSelectSaveFile(const String sMaskDescr, const String sMask, const String sTitle, const String sFolder)
{
   TJFileForm* pF=new TJFileForm(0);
   pF->Position=poMainFormCenter;
   pF->AddMask(sMaskDescr,sMask);
   if (!sTitle.IsEmpty())
      pF->sTitle=sTitle;
   if (!sFolder.IsEmpty())
      pF->sDir=sFolder;
   pF->CheckExist();
   pF->ShowModal();
   String sFile=pF->sFileName;
   delete pF;
   return sFile;
}

String sSelectSaveFileDef(const String sMaskDescr, const String sMask, const String sTitle, const String sDefaultFile)
{
   TJFileForm* pF=new TJFileForm(0);
   pF->Position=poMainFormCenter;
   pF->AddMask(sMaskDescr,sMask);
   pF->sTitle=sTitle;
   String sFolder=sFileDirOnly(sDefaultFile);
   if (!sFolder.IsEmpty())
      pF->sDir=sFolder;
   pF->sFileName=sFileNameOnly(sDefaultFile);
   pF->CheckExist();
   pF->ShowModal();
	String sFile=pF->sFileName;
   delete pF;
   return sFile;
}

void __fastcall TJFileForm::CancelButClick(TObject *)
{
   sFileName=L"";
   DlgState=NM_DIALOG_CANCEL;
   ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

