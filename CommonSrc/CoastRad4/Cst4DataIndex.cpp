//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4DataIndex.h"
#include "JMemFile.h";
#include "JDCSFile.h";

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4DataIndex::Cst4DataIndex(const String _sIndexFolder, const bool bNewAvail)
{
	pTestFil=NULL;
   sLogFile="";

	pFileLB=NULL;
	pProgBar=NULL;
	bAborted=false;
	SearchPar=SEARCH_NONE;
	memset(aSourceCnt,0,sizeof(aSourceCnt));
	sIndexFolder=IncludeTrailingBackslash(_sIndexFolder);
	bCreateDir(sIndexFolder,true);
	if (bNewAvail)
		{
		String sTempFolder=sIndexFolder+"Temp\\";
      bCreateDir(sTempFolder,true);
      JTime T(true);
      String sFile1,sFile2;
      WORD wMonth=T.Get().wMonth;
      WORD wYear=T.Get().wYear;
      sFile1.printf(L"%04d_%02d",wYear,wMonth);  //This month
      --wMonth;
      if (wMonth==0)
         --wYear;
      sFile2.printf(L"%04d_%02d",wYear,wMonth);  //Last month
      WIN32_FIND_DATA FileData;
      String sMask=sTempFolder+"*.Cst4IDX";
      HANDLE FileHandle=::FindFirstFile(sMask.c_str(),&FileData);
      if (FileHandle!=INVALID_HANDLE_VALUE)
         {
         do
            {
            if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
               {
               String sTempFile=sTempFolder+String(FileData.cFileName);
               String sDestFile=sIndexFolder+String(FileData.cFileName);
               if ((sTempFile.Pos(sFile1)>0)||(sTempFile.Pos(sFile2)>0))
                  {
                  //File is from this month or last month
                  //Check if archive flag is set
                  if (::GetFileAttributes(sTempFile.c_str())&FILE_ATTRIBUTE_ARCHIVE)
                     {
                     //Copy file to index folder
							int nCancel=FALSE;
							::CopyFileEx(sTempFile.c_str(),sDestFile.c_str(),NULL,NULL,&nCancel,0);
							::SetFileAttributes(sTempFile.c_str(),FILE_ATTRIBUTE_NORMAL);  //Clear archive flag
							}
						}
					else
                  {
                  //Move file to index folder
                  ::MoveFileEx(sTempFile.c_str(),sDestFile.c_str(),MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
                  }
               }
            Application->ProcessMessages();
            }
         while(::FindNextFile(FileHandle,&FileData));
         }
      ::FindClose(FileHandle);
      }
}

Cst4DataIndex::~Cst4DataIndex()
{
	delete pTestFil;
}

bool Cst4DataIndex::bOpenIndexFile_utc(const JTime& T_utc)
{
	sIndexFile.printf(L"%04d_%02d.Cst4IDX",T_utc.Get().wYear,T_utc.Get().wMonth);
   sIndexFile=sIndexFolder+sIndexFile;
   bool bOK;
   if (bFileExist(sIndexFile))
      bOK=bOpenIndexFile(sIndexFile);
   else
      {
      IndexList.Clear();
      Header.Init();
      bOK=true;
      }

/*
	delete pTestFil;
	pTestFil=new JFile('O',JFile::ASCII_TYPE);
	pTestFil->Create(sIndexFile+".txt");
*/
	return bOK;
}

bool Cst4DataIndex::bOpenIndexFile(const String sFile)
{
   IndexList.Clear();
   JMemFile Fil;
   if (Fil.ReadBuffer(sFile)==JFile::F_NO_ERROR)
      {
      bool bOK=Fil.bRead(&Header,sizeof(Header));
      BYTE* pucBuf=Fil.pucGetBuffer();
      int nOffs=sizeof(Header);
      for (int i=0; (i<Header.nFiles)&&(bOK); i++)
         IndexList.nAdd(new IndexEntry(pucBuf,nOffs));
      IndexList.Pack();
      return bOK;
      }
   else
      return false;
}

bool Cst4DataIndex::bSaveIndexFile(const String sFile)
{
   JFile Fil('O');
   JFile::FILE_ERROR E=Fil.Create(sFile);
   if (!E)
      E=Fil.Write(&Header,sizeof(Header));
	if (!E)
      {
		int nOffs=0;
      IndexList.GoFirst();
      while(!IndexList.bLast())
         nOffs+=IndexList.pNext()->nGetSize();
      BYTE* pucBuf=new BYTE[nOffs];
      nOffs=0;
      IndexList.GoFirst();
      while(!IndexList.bLast())
         {
         IndexEntry* pEntry=IndexList.pNext();
         pEntry->Pack(pucBuf,nOffs);
         }
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   return (E==JFile::F_NO_ERROR);
}

bool Cst4DataIndex::bFileInIndex(const String sFile)
{
   String sF=sFileNameOnly(sFile);
	for (int i=0; i<Header.nFiles; i++)
		{
		if (sF.Pos(sGetIndexEntryFile(i))!=0)
         return true;
		}
	return false;
}


bool Cst4DataIndex::bUpdateRT_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile)
{
	IndexList.nAdd(new IndexEntry(T1_utc,T2_utc,sDataFile));
	++Header.nFiles;
	if (!Header.T1_utc.bValid())
		{
		Header.T1_utc=T1_utc;
      Header.dwT1_utc_sec=T1_utc.dwTotalSec();
      }
	Header.T2_utc=T2_utc;
	Header.dwT2_utc_sec=T2_utc.dwTotalSec();
	JDCSFile Fil(sIndexFile,true,true);
	bool bOK=Fil.bOK();
	if (bOK)
		bOK=Fil.bWrite(&Header,sizeof(Header));
	if (bOK)
		{
		int nOffs=0;
		IndexList.GoFirst();
		while(!IndexList.bLast())
			nOffs+=IndexList.pNext()->nGetSize();
		BYTE* pucBuf=new BYTE[nOffs];
		nOffs=0;
		IndexList.GoFirst();
		while(!IndexList.bLast() && bOK)
			{
			IndexEntry* pEntry=IndexList.pNext();
			pEntry->Pack(pucBuf,nOffs);
			}
		bOK=Fil.bWrite(pucBuf,nOffs);
		delete[] pucBuf;
		}
   return bOK;
}

bool Cst4DataIndex::bUpdateRTIndex_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile)
{
	bool bOK=false;
	if (T1_utc.bValid() && T2_utc.bValid())
		{
		if (T1_utc.Get().wMonth!=T2_utc.Get().wMonth)
			{
			if (bOpenIndexFile_utc(T1_utc))
				{
				int nMonthDays=T1_utc.nMonthDays();
				JTime TT2_utc;
				TT2_utc.SetDate(nMonthDays,T1_utc.Get().wMonth,T1_utc.Get().wYear,true);
				TT2_utc.SetTime(23,59,59,999);
				bOK=bUpdateRT_utc(T1_utc,TT2_utc,sDataFile);
				}
			if (bOpenIndexFile_utc(T2_utc))
				{
				JTime TT1_utc;
				TT1_utc.SetDate(1,T2_utc.Get().wMonth,T2_utc.Get().wYear,true);
				TT1_utc.SetTime(0,0,0,0);
				bOK=bUpdateRT_utc(TT1_utc,T2_utc,sDataFile);
				}

			}
      else
         {
			bOK=bOpenIndexFile_utc(T1_utc);
         if (bOK)
            bOK=bUpdateRT_utc(T1_utc,T2_utc,sDataFile);
         }
      }
   return bOK;
}

bool Cst4DataIndex::bRebuild_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile)
{
   IndexList.GoFirst();
   while(!IndexList.bLast())
      {
      if (IndexList.pNext()->sDataFile==sDataFile)
         return true;
      }
	IndexEntry* pEntry=new IndexEntry(T1_utc,T2_utc,sDataFile);
	IndexList.nAdd(pEntry);
	++Header.nFiles;
	if (!Header.T1_utc.bValid())
		{
		Header.T1_utc=T1_utc;
		Header.dwT1_utc_sec=T1_utc.dwTotalSec();
		}
	else if (pEntry->dwT1_utc_sec<Header.dwT1_utc_sec)
		{
		Header.T1_utc=pEntry->T1_utc;
		Header.dwT1_utc_sec=pEntry->dwT1_utc_sec;
		}
	if (!Header.T2_utc.bValid())
		{
		Header.T2_utc=T2_utc;
		Header.dwT2_utc_sec=T2_utc.dwTotalSec();
		}
	else if (pEntry->dwT2_utc_sec>Header.dwT2_utc_sec)
		{
		Header.T2_utc=pEntry->T2_utc;
		Header.dwT2_utc_sec=pEntry->dwT2_utc_sec;
		}
	JFile Fil('O');
	JFile::FILE_ERROR E=Fil.Create(sIndexFile);
	if (!E)
		E=Fil.Write(&Header,sizeof(Header));
   if (!E)
      {
      int nOffs=0;
      IndexList.GoFirst();
      while(!IndexList.bLast())
         nOffs+=IndexList.pNext()->nGetSize();
      BYTE* pucBuf=new BYTE[nOffs];
      nOffs=0;
      IndexList.GoFirst();
      while(!IndexList.bLast())
         {
         IndexEntry* pEntry=IndexList.pNext();
         pEntry->Pack(pucBuf,nOffs);
         }
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   return (E==JFile::F_NO_ERROR);
}


void Cst4DataIndex::RebuildIndex_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile)
{
	if (T1_utc.Get().wMonth!=T2_utc.Get().wMonth)
		{
		if (bOpenIndexFile_utc(T1_utc))
			{
			int nMonthDays=T1_utc.nMonthDays();
			JTime TT2_utc;
			TT2_utc.SetDate(nMonthDays,T1_utc.Get().wMonth,T1_utc.Get().wYear,true);
			TT2_utc.SetTime(23,59,59,999);
			bRebuild_utc(T1_utc,TT2_utc,sDataFile);
			}
		if (bOpenIndexFile_utc(T2_utc))
			{
			JTime TT1_utc;
			TT1_utc.SetDate(1,T2_utc.Get().wMonth,T2_utc.Get().wYear,true);
			TT1_utc.SetTime(0,0,0,0);
			TT1_utc.SetUTC();
			bRebuild_utc(TT1_utc,T2_utc,sDataFile);
			}
		}
	else
		{
		if (bOpenIndexFile_utc(T1_utc))
			bRebuild_utc(T1_utc,T2_utc,sDataFile);
		}

}

bool Cst4DataIndex::bStaticPresent(const Cst4_AIS_Static* pStat)
{
	int nMMSI=pStat->nGetMMSI();
	Cst4_AIS_Static* pS;
	AIS_S.GoFirst();
	while(!AIS_S.bLast())
		{
		pS=AIS_S.pNext();
		if (pS->nGetMMSI()==nMMSI)
			{
			bool bReplace=true;
			JTime T_prev=pS->Time_utc();
			JTime T_new=pStat->Time_utc();
			if (T_prev.bValid() && T_new.bValid())
				bReplace=(T_new.dwTotalSec()>T_prev.dwTotalSec());
			if (bReplace)
				*pS=*pStat;
			return true;
			}
      }
   return false;
}

int Cst4DataIndex::nImportFile_utc(const String sFile, const DWORD dwT1_utc_sec, const DWORD dwT2_utc_sec, const bool bCreateVesselList)
{
	int nMsg=0;
	Cst4DataFile Fil;

	if (Fil.bReadFile(sDataFolder+sFile,true))
      {
		JTime FromT_utc,ToT_utc;
		Fil.GetTimeSpan_utc(FromT_utc,ToT_utc);
		if (FromT_utc.dwTotalSec()<DataFrom_utc.dwTotalSec())
			DataFrom_utc=FromT_utc;
		if (ToT_utc.dwTotalSec()>DataTo_utc.dwTotalSec())
			DataTo_utc=ToT_utc;
		int nTotalMsg=	 Fil.nDynamicCnt()
							+Fil.nStaticCnt()
							+Fil.nScanCnt()
							+Fil.nTrackCnt()
							+Fil.nIFFCnt()
							+Fil.nADSBCnt()
							+Fil.nSysTrackCnt()
							+Fil.nUserCnt();
		int nPrevPerc=-1;
      if (pProgBar)
         {
         pProgBar->Position=0;
         Application->ProcessMessages();
         }

      Cst4_AIS_Static* pStat;
      for (int i=0; i<Fil.nStaticCnt(); i++)
			{
			pStat=Fil.pStatic(i);
			if (!bStaticPresent(pStat))
				{
            AIS_S.nAdd(new Cst4_AIS_Static(*pStat));
            ++aSourceCnt[pStat->wSource()].anCnt[CST4_STATIC_AIS_MSG];
            if (bCreateVesselList)
               AddVessel(pStat);
            if (SearchPar!=SEARCH_NONE)
               CheckSearch(pStat);
            ++nMsg;
            if (pProgBar)
               {
               int nPerc=(nMsg*100)/nTotalMsg;
               if (nPerc!=nPrevPerc)
                  {
                  nPrevPerc=nPerc;
                  pProgBar->Position=nPerc;
                  Application->ProcessMessages();
                  }
               }
            }
         }

      Cst4_AIS_Dynamic* pD;
      for (int i=0; i<Fil.nDynamicCnt(); i++)
         {
         pD=Fil.pDynamic(i);
			if (bValidTime_utc(pD->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
            {
            AIS_D.nAdd(new Cst4_AIS_Dynamic(*pD));
            ++aSourceCnt[pD->wSource()].anCnt[CST4_DYNAMIC_AIS_MSG];
            if (bCreateVesselList)
               AddVessel(pD);
            if (SearchPar!=SEARCH_NONE)
               CheckSearch(pD);
            ++nMsg;
            if (pProgBar)
               {
               int nPerc=(nMsg*100)/nTotalMsg;
               if (nPerc!=nPrevPerc)
                  {
                  nPrevPerc=nPerc;
                  pProgBar->Position=nPerc;
                  Application->ProcessMessages();
                  }
               }
            }
         }
      Cst4_Radar_Scan* pScan;
      for (int i=0; i<Fil.nScanCnt(); i++)
         {
         pScan=Fil.pScan(i);
			if (bValidTime_utc(pScan->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
            Radar_S.nAdd(new Cst4_Radar_Scan(*pScan));
            ++aSourceCnt[pScan->wSource()].anCnt[CST4_RADAR_SCAN_MSG];
            ++nMsg;
            if (pProgBar)
               {
               int nPerc=(nMsg*100)/nTotalMsg;
               if (nPerc!=nPrevPerc)
                  {
                  nPrevPerc=nPerc;
                  pProgBar->Position=nPerc;
                  Application->ProcessMessages();
                  }
               }
            }
         }
      Cst4_Radar_Track* pT;
      for (int i=0; i<Fil.nTrackCnt(); i++)
         {
         pT=Fil.pTrack(i);
			if (bValidTime_utc(pT->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
				Radar_T.nAdd(new Cst4_Radar_Track(*pT));
				++aSourceCnt[pT->wSource()].anCnt[CST4_RADAR_TRACK_MSG];
				++nMsg;
				if (pProgBar)
					{
					int nPerc=(nMsg*100)/nTotalMsg;
					if (nPerc!=nPrevPerc)
						{
						nPrevPerc=nPerc;
						pProgBar->Position=nPerc;
						Application->ProcessMessages();
						}
					}
				}
			}
		Cst4_IFF_Track* pIFF;
		for (int i=0; i<Fil.nIFFCnt(); i++)
			{
			pIFF=Fil.pIFF(i);
			if (bValidTime_utc(pIFF->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
				IFF_T.nAdd(new Cst4_IFF_Track(*pIFF));
				++aSourceCnt[pIFF->wSource()].anCnt[CST4_IFF_TRACK_MSG];
				++nMsg;
				if (pProgBar)
					{
					int nPerc=(nMsg*100)/nTotalMsg;
					if (nPerc!=nPrevPerc)
						{
						nPrevPerc=nPerc;
						pProgBar->Position=nPerc;
						Application->ProcessMessages();
						}
					}
				}
			}

		Cst4_ADSB* pADSB;
		for (int i=0; i<Fil.nADSBCnt(); i++)
			{
			pADSB=Fil.pADSB(i);
			if (bValidTime_utc(pADSB->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
				ADSB_T.nAdd(new Cst4_ADSB(*pADSB));
				++aSourceCnt[pADSB->wSource()].anCnt[VISTA_ADSB_MSG];
				++nMsg;
				if (pProgBar)
					{
					int nPerc=(nMsg*100)/nTotalMsg;
					if (nPerc!=nPrevPerc)
						{
						nPrevPerc=nPerc;
						pProgBar->Position=nPerc;
						Application->ProcessMessages();
						}
					}
				}
			}

		Cst4_SysTrack* pSysTrack;
		for (int i=0; i<Fil.nSysTrackCnt(); i++)
			{
			pSysTrack=Fil.pSysTrack(i);
			if (bValidTime_utc(pSysTrack->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
				Sys_T.nAdd(new Cst4_SysTrack(*pSysTrack));
				++aSourceCnt[pSysTrack->wSource()].anCnt[CST4_SYSTRACK_MSG];
				++nMsg;
				if (pProgBar)
					{
					int nPerc=(nMsg*100)/nTotalMsg;
					if (nPerc!=nPrevPerc)
						{
						nPrevPerc=nPerc;
						pProgBar->Position=nPerc;
						Application->ProcessMessages();
						}
					}
				}
			}
		Cst4_User_Msg* pUser;
		for (int i=0; i<Fil.nUserCnt(); i++)
			{
			pUser=Fil.pUser(i);
			if (bValidTime_utc(pUser->dwTime_sec_utc(),dwT1_utc_sec,dwT2_utc_sec))
				{
				User_M.nAdd(new Cst4_User_Msg(*pUser));
				++aSourceCnt[pUser->wSource()].anCnt[CST4_USER_MSG];
				++nMsg;
				if (pProgBar)
					{
					int nPerc=(nMsg*100)/nTotalMsg;
					if (nPerc!=nPrevPerc)
						{
						nPrevPerc=nPerc;
						pProgBar->Position=nPerc;
						Application->ProcessMessages();
						}
					}
				}
			}
		}
	if (pProgBar)
		{
		pProgBar->Position=pProgBar->Max;
		Application->ProcessMessages();
		}
	return nMsg;
}

int Cst4DataIndex::nImport_utc(const JTime& T1_utc, const JTime& T2_utc, const bool bCreateVesselList, TMessageBarForm* pMsgBar, const bool /*bPackData*/)
{
	DWORD dwT1_utc_sec=T1_utc.dwTotalSec();
	DWORD dwT2_utc_sec=T2_utc.dwTotalSec();
	IndexEntry* pEntry;
	int nMsg=0;
	int nF=0;
	if (pMsgBar)
		pMsgBar->SetProgress(0);
	IndexList.GoFirst();
	while((!IndexList.bLast())&&(!bAborted))
		{
		pEntry=IndexList.pNext();
		if ((pEntry->dwT1_utc_sec<dwT2_utc_sec)&&(pEntry->dwT2_utc_sec>dwT1_utc_sec))
			{
			if (pTestFil)
				{
				String s;
				s=pEntry->sDataFile+"    "+JTime(pEntry->dwT1_utc_sec).sDateTime()+"  "+JTime(pEntry->dwT2_utc_sec).sDateTime();
				pTestFil->TextLine(s);
				}

			if (pFileLB)
            {
            pFileLB->Items->Add(pEntry->sDataFile);
            pFileLB->ItemIndex=pFileLB->Items->Count-1;
            Application->ProcessMessages();
            }
         nMsg+=nImportFile_utc(pEntry->sDataFile,dwT1_utc_sec,dwT2_utc_sec,bCreateVesselList);
         }
      if (pMsgBar)
         pMsgBar->SetProgress(++nF,IndexList.nGetCount());
      }
   if (pMsgBar)
      pMsgBar->SetProgress(100);
   return nMsg;
}

int Cst4DataIndex::nImportData_utc(const String _sDataFolder, const JTime& T1_utc, const JTime& T2_utc, const bool bCreateVesselList, TMessageBarForm* pMsgBar, const bool bPackData)
{
	memset(aSourceCnt,0,sizeof(aSourceCnt));
	VesselList.Clear();
	SearchList.Clear();
	sDataFolder=IncludeTrailingBackslash(_sDataFolder);
	bAborted=false;
	AIS_D.Clear();
	AIS_S.Clear();
	Radar_S.Clear();
	Radar_T.Clear();
	IFF_T.Clear();
	ADSB_T.Clear();
	Sys_T.Clear();
	User_M.Clear();
	DataFrom_utc=T1_utc;
	DataTo_utc=T2_utc;
	int nTotalMsgCnt=0;
	if (DataFrom_utc.Get().wMonth!=DataTo_utc.Get().wMonth)
		{
		if (bOpenIndexFile_utc(DataFrom_utc))
			{
			int nMonthDays=DataFrom_utc.nMonthDays();
			JTime TT2_utc;	//Last time/date in month
			TT2_utc.SetDate(nMonthDays,DataFrom_utc.Get().wMonth,DataFrom_utc.Get().wYear,true);
			TT2_utc.SetTime(23,59,59,999);
			nTotalMsgCnt+=nImport_utc(DataFrom_utc,TT2_utc,bCreateVesselList,pMsgBar,bPackData);
			}
		if (!bAborted)
			{
			if (bOpenIndexFile_utc(T2_utc))
				{
				JTime TT1_utc;
				TT1_utc.SetDate(1,T2_utc.Get().wMonth,T2_utc.Get().wYear,true);
				TT1_utc.SetTime(0,0,0,0);
				nTotalMsgCnt+=nImport_utc(TT1_utc,T2_utc,bCreateVesselList,pMsgBar,bPackData);
				}
			}
		}
	else
		{
		if (bOpenIndexFile_utc(T1_utc))
			nTotalMsgCnt+=nImport_utc(T1_utc,T2_utc,bCreateVesselList,pMsgBar,bPackData);
		}

	StoreLog(sLogFile,"Data Import Completed");

	VesselList.Pack();
	SearchList.Pack();
	memset(anMsgCnt,0,sizeof(anMsgCnt));
	if (bPackData)
		AIS_D.Pack();
	anMsgCnt[CST4_DYNAMIC_AIS_MSG]=AIS_D.nGetCount();
	StoreLog(sLogFile,"  Total AIS_D = "+String(AIS_D.nGetCount()));
	if (bPackData)
		AIS_S.Pack();
	anMsgCnt[CST4_STATIC_AIS_MSG]=AIS_S.nGetCount();
	StoreLog(sLogFile,"  Total AIS_S = "+String(AIS_S.nGetCount()));
	if (bPackData)
		Radar_S.Pack();
	anMsgCnt[CST4_RADAR_SCAN_MSG]=Radar_S.nGetCount();
	StoreLog(sLogFile,"  Total Radar_S = "+String(Radar_S.nGetCount()));
	if (bPackData)
		Radar_T.Pack();
	anMsgCnt[CST4_RADAR_TRACK_MSG]=Radar_T.nGetCount();
	StoreLog(sLogFile,"  Total Radar_T = "+String(Radar_T.nGetCount()));
	if (bPackData)
		IFF_T.Pack();
	anMsgCnt[CST4_IFF_TRACK_MSG]=IFF_T.nGetCount();
	StoreLog(sLogFile,"  Total IFF = "+String(IFF_T.nGetCount()));
	if (bPackData)
		ADSB_T.Pack();
	anMsgCnt[VISTA_ADSB_MSG]=ADSB_T.nGetCount();
	StoreLog(sLogFile,"  Total ADS-B = "+String(ADSB_T.nGetCount()));
	if (bPackData)
		Sys_T.Pack();
	anMsgCnt[CST4_SYSTRACK_MSG]=Sys_T.nGetCount();
	if (bPackData)
		User_M.Pack();
	anMsgCnt[CST4_USER_MSG]=User_M.nGetCount();
	StoreLog(sLogFile,"  Total User = "+String(User_M.nGetCount()));
	StoreLog(sLogFile," ");
	return nTotalMsgCnt;
}

void Cst4DataIndex::AddVessel(const int nMMSI, const int nIMO, const String sName)
{
   VesselList.GoFirst();
   bool bFound=false;
   VesselStruct* pV;
   String s=sName.Trim();
   while((!VesselList.bLast())&&(!bFound))
      {
      pV=VesselList.pNext();
      if (pV->nMMSI==nMMSI)
         {
         if (pV->sName.IsEmpty())
            {
            if (!s.IsEmpty())
               pV->sName=s;
            }
         if (pV->nIMO<=0)
            {
            if (nIMO>0)
               pV->nIMO=nIMO;
            }
         bFound=true;
         }
      }
   if (!bFound)
      {
      pV=new VesselStruct;
      pV->nMMSI=nMMSI;
      pV->nIMO=nIMO;
      pV->sName=s;
      pV->bSel=true;
      pV->bUseColor=false;
      pV->Color=clDefault;
      VesselList.nAdd(pV);
      }
}


void Cst4DataIndex::AddVessel(Cst4_AIS_Static* pStatic)
{
   int nMMSI=pStatic->nGetMMSI();
   if (nMMSI>0)
      {
      int nType,nIMO;
      String sName=pStatic->sBasicInfo(nType,nIMO);
      AddVessel(nMMSI,nIMO,sName);
      }
}

void Cst4DataIndex::AddVessel(Cst4_AIS_Dynamic* pDynamic)
{
	int nMMSI=pDynamic->nGetMMSI();
	if (nMMSI>0)
		AddVessel(nMMSI,-1,L"");
}

void Cst4DataIndex::SortVesselName()
{
	for (int i=0; i<VesselList.nGetCount(); i++)
		VesselList[i]->SortPar=VESSEL_SORT_NAME;
	VesselList.Sort();
}

void Cst4DataIndex::SortVesselMMSI()
{
	for (int i=0; i<VesselList.nGetCount(); i++)
		VesselList[i]->SortPar=VESSEL_SORT_MMSI;
	VesselList.Sort();
}

void Cst4DataIndex::SortVesselIMO()
{
	for (int i=0; i<VesselList.nGetCount(); i++)
		VesselList[i]->SortPar=VESSEL_SORT_IMO;
	VesselList.Sort();
}

//Check Static first, then Dynamic
void Cst4DataIndex::CheckSearch(Cst4_AIS_Static* pStatic)
{
	int nMMSI=pStatic->nGetMMSI();
	int nIMO;
	String sShipName,sCallsign;
	if (pStatic->bCst3Msg())
		{
      int nType;
      pStatic->nGetCst3Info(nIMO,sShipName,sCallsign,nType);
      }
   else
      {
      AISMsgClass AIS;
      AIS.SetExtraInfoBuf(pStatic->pucGetData(),pStatic->wGetDataSize());
      sShipName=pStatic->sShipName();
      if (AIS.bCallSignValid())
         sCallsign=AIS.sGetCallSign();
      else
         sCallsign=L"";
      if (AIS.bShipNameValid())
         sShipName=AIS.sGetShipName();
      else
         sShipName=L"";
//      nIMO=AIS.nGetIMO();
      }
   String s;
   int nMMSIFound=-1;
   switch(SearchPar)
      {
      case SEARCH_NAME:
         sShipName=sShipName.UpperCase();
         if (sShipName.Pos(sSearchName)>0)
            nMMSIFound=nMMSI;
         break;
      case SEARCH_CALLSIGN:
         sCallsign=sCallsign.UpperCase();
         if (sCallsign.Pos(sSearchCall)>0)
            nMMSIFound=nMMSI;
         break;
      case SEARCH_MMSI:
         if (nSearchMMSI==pStatic->nGetMMSI())
            nMMSIFound=pStatic->nGetMMSI();
         break;
      case SEARCH_IMO:
         if (nSearchIMO==pStatic->nIMO())
            nMMSIFound=pStatic->nGetMMSI();
         break;
      default: ;
      }
   if (nMMSIFound>0)
      {
      SearchList.GoFirst();
      while((!SearchList.bLast())&&(nMMSIFound>0))
         {
         if (SearchList.pNext()->Stat.nGetMMSI()==nMMSIFound)
            nMMSIFound=-1;   //Already in list
         }
      }
   if (nMMSIFound>0)
      {
      SearchInfoStruct* pS=new SearchInfoStruct;
      pS->nMMSI=nMMSI;
      pS->Stat=*pStatic;
      SearchList.nAdd(pS);
      }
}

//Check Static first, then Dynamic
void Cst4DataIndex::CheckSearch(Cst4_AIS_Dynamic* pDynamic)
{
   int nMMSI=pDynamic->nGetMMSI();
   if (SearchPar==SEARCH_MMSI)
      {
      int nMMSIFound=-1;
      if (nSearchMMSI==nMMSI)
         {
         SearchList.GoFirst();
         while((!SearchList.bLast())&&(nMMSI>0))
            {
            if (SearchList.pNext()->nMMSI==nMMSIFound)
               nMMSI=-1;   //Already in list
            }
         if (nMMSI>0)
            {
            SearchInfoStruct* pS=new SearchInfoStruct;
            pS->nMMSI=nMMSI;
            SearchList.nAdd(pS);
            }
         }
      }
   SearchList.GoFirst();
   SearchInfoStruct* pS;
   SearchInfoStruct* pFound=NULL;
   while((!SearchList.bLast())&&(!pFound))
      {
      pS=SearchList.pNext();
      if (pS->nMMSI==nMMSI)
         pFound=pS;
      }
   if (pFound)
      {
      ++pFound->nHits;
      ++pFound->anSource[pDynamic->wSource()];
      if (!pFound->D1.bValid())
         {
         pFound->D1=*pDynamic;
         pFound->D2=*pDynamic;
         }
      else
         {
			DWORD dwT_sec=pDynamic->Time_utc().dwTotalSec();
			if (dwT_sec<pFound->D1.Time_utc().dwTotalSec())
				pFound->D1=*pDynamic;
			if (dwT_sec>pFound->D2.Time_utc().dwTotalSec())
				pFound->D2=*pDynamic;
         }
      }
}

bool Cst4DataIndex::bRemoveFile(const String sFile)
{
   bool bOK=false;
   String sDataFile=sFileNameOnly(sFile);
   WIN32_FIND_DATA FileData;
   String sMask=sIndexFolder+"*.Cst4IDX";
	HANDLE FileHandle=::FindFirstFile(sMask.c_str(),&FileData);
   if (FileHandle!=INVALID_HANDLE_VALUE)
      {
      do
         {
         if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
            {
            String sIndexFile=sIndexFolder+String(FileData.cFileName);
            if (bOpenIndexFile(sIndexFile))
               {
               bool bFound=false;
               for (int i=0; (i<IndexList.nGetCount() &&!bFound); i++)
						{
						if (AnsiString(IndexList[i]->sDataFile).AnsiCompareIC(sDataFile)==0)
                     {
                     --Header.nFiles;
                     IndexList.Delete(i);
                     bFound=true;
                     }
                  }
               if (bFound)
                  {
                  bOK=true;
                  if (IndexList.nGetCount()>0)
                     {
                     IndexList.Pack();
							Header.dwT1_utc_sec=0xFFFFFFFF;
							Header.dwT2_utc_sec=0;
                     for (int i=0; i<IndexList.nGetCount(); i++)
                        {
								if (IndexList[i]->T1_utc.dwTotalSec()<Header.dwT1_utc_sec)
									{
									Header.dwT1_utc_sec=IndexList[i]->T1_utc.dwTotalSec();
									Header.T1_utc=IndexList[i]->T1_utc;
									}
								if (IndexList[i]->T2_utc.dwTotalSec()>Header.dwT2_utc_sec)
									{
									Header.dwT2_utc_sec=IndexList[i]->T2_utc.dwTotalSec();
									Header.T2_utc=IndexList[i]->T2_utc;
									}
                        }
                     bSaveIndexFile(sIndexFile);
                     }
                  else
                     DeleteFile(sIndexFile);
                  }
               }
            }
         }
      while(::FindNextFile(FileHandle,&FileData));
      }
   ::FindClose(FileHandle);
   return bOK;
}
