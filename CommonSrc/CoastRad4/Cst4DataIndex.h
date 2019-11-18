//---------------------------------------------------------------------------

#ifndef Cst4DataIndexH
#define Cst4DataIndexH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "Cst4DataFile.h"
#include "JUtils.h"
#include "JMessageBar.h"


class Cst4DataIndex
{

public:

   enum
   {
      INDEX_FORMAT_VERSION = 1000,
   };

   enum VESSEL_SORT_PAR
   {
      VESSEL_SORT_MMSI,
      VESSEL_SORT_IMO,
      VESSEL_SORT_NAME,
   };

   struct VesselStruct
   {
      int nMMSI;
      int nIMO;
      String sName;
      bool bUseColor;
      TColor Color;
      bool bSel;
      VESSEL_SORT_PAR SortPar;

      VesselStruct& operator = (const VesselStruct& C)
      {
         nMMSI=C.nMMSI;
         nIMO=C.nIMO;
         sName=C.sName;
         bUseColor=C.bUseColor;
         Color=C.Color;
         bSel=C.bSel;
         SortPar=C.SortPar;
         return *this;
      }

      VesselStruct(const VesselStruct& C)
      {
         *this=C;
      }

      VesselStruct()
      {
         nMMSI=-1;
         nIMO=-1;
			sName="";
         bUseColor=false;
         Color=clBlack;
         bSel=false;
         SortPar=VESSEL_SORT_MMSI;
      }

      bool operator < (const VesselStruct& T) const
      {
         if (SortPar==VESSEL_SORT_MMSI)
            return nMMSI<T.nMMSI;
         else if (SortPar==VESSEL_SORT_IMO)
            return nIMO<T.nIMO;
         else
         	{
				if (sName.IsEmpty()&& !T.sName.IsEmpty())
               return false;
            else if (!sName.IsEmpty()&& T.sName.IsEmpty())
            	return true;
            else if (sName.IsEmpty()&& T.sName.IsEmpty())
            	return nMMSI<T.nMMSI;
            else
            	return sName.CompareIC(T.sName)<0;
            }
//            return sName.Compare(T.sName)<0;
      }

      bool operator > (const VesselStruct& T) const
      {
         if (SortPar==VESSEL_SORT_MMSI)
            return nMMSI>T.nMMSI;
         else if (SortPar==VESSEL_SORT_IMO)
            return nIMO>T.nIMO;
         else
         	{
				if (sName.IsEmpty()&& !T.sName.IsEmpty())
               return true;
            else if (!sName.IsEmpty()&& T.sName.IsEmpty())
            	return false;
            else if (sName.IsEmpty()&& T.sName.IsEmpty())
            	return nMMSI>T.nMMSI;
            else
            	return sName.CompareIC(T.sName)>0;
            }
//            return sName.Compare(T.sName)>0;
      }



   };

   enum SEARCH_PAR
   {
      SEARCH_NONE,
      SEARCH_NAME,
      SEARCH_CALLSIGN,
      SEARCH_MMSI,
      SEARCH_IMO,
   };

   struct SearchInfoStruct
   {
      int nMMSI;
      Cst4_AIS_Static Stat;
      Cst4_AIS_Dynamic D1;
      Cst4_AIS_Dynamic D2;
      int nHits;
      int anSource[65536];

      SearchInfoStruct& operator = (const SearchInfoStruct& C)
      {
         nMMSI=C.nMMSI;
         Stat=C.Stat;
         D1=C.D1;
         D2=C.D2;
         nHits=C.nHits;
         memcpy(anSource,C.anSource,sizeof(anSource));
         return *this;
      }

      SearchInfoStruct(const SearchInfoStruct& C)
      {
         *this=C;
      }

      SearchInfoStruct()
      {
         nMMSI=-1;
         Stat.Invalidate();
			D1.Invalidate();
			D2.Invalidate();
			nHits=0;
			memset(anSource,0,sizeof(anSource));
		}

	};

private:

	struct IndexHeader
	{
		DWORD dwExeVersion;
		DWORD dwIndexVersion;
		int nFiles;
		JTime T1_utc,T2_utc;
		DWORD dwT1_utc_sec,dwT2_utc_sec;

		void Init()
		{
			dwExeVersion=dwGetExeVersion();
			dwIndexVersion=Cst4DataIndex::INDEX_FORMAT_VERSION;
			nFiles=0;
			T1_utc.Invalidate();
			T2_utc.Invalidate();
			dwT1_utc_sec=0;
			dwT2_utc_sec=0;
		}

		IndexHeader()
		{
			Init();
		}

   };

   struct IndexEntry
   {
		JTime T1_utc,T2_utc;
		DWORD dwT1_utc_sec,dwT2_utc_sec;
		String sDataFile;

		void Unpack(const BYTE* pucBuf, int& nOffs)
		{
			memcpy(&T1_utc,&pucBuf[nOffs],sizeof(T1_utc));
			nOffs+=sizeof(T1_utc);
			memcpy(&T2_utc,&pucBuf[nOffs],sizeof(T2_utc));
			nOffs+=sizeof(T2_utc);
			memcpy(&dwT1_utc_sec,&pucBuf[nOffs],sizeof(dwT1_utc_sec));
			nOffs+=sizeof(dwT1_utc_sec);
			memcpy(&dwT2_utc_sec,&pucBuf[nOffs],sizeof(dwT2_utc_sec));
			nOffs+=sizeof(dwT2_utc_sec);
			int nSize;
			memcpy(&nSize,&pucBuf[nOffs],sizeof(nSize));
			nOffs+=sizeof(nSize);
			if (nSize>0)
				{
				char* pcBuf=new char[nSize+1];
				memcpy(pcBuf,&pucBuf[nOffs],nSize);
				pcBuf[nSize]=0;
				sDataFile=String(pcBuf);
				nOffs+=nSize;
            delete[] pcBuf;
            }
         else
				sDataFile="";
      }

      IndexEntry()
      {
      }

		IndexEntry(const JTime& _T1_utc, const JTime& _T2_utc, const String _sDataFile)
		{
			T1_utc=_T1_utc;
			dwT1_utc_sec=T1_utc.dwTotalSec();
			T2_utc=_T2_utc;
			dwT2_utc_sec=T2_utc.dwTotalSec();
			sDataFile=sFileNameOnly(_sDataFile);
		}

		IndexEntry(const BYTE* pucBuf, int& nOffs)
		{
			Unpack(pucBuf,nOffs);
		}

		int nGetSize() const
		{
			return sizeof(T1_utc)+sizeof(dwT1_utc_sec)+sizeof(T2_utc)+sizeof(dwT2_utc_sec)+sizeof(int)+sDataFile.Length();
		}

		void Pack(BYTE* pucBuf, int& nOffs)
		{
			memcpy(&pucBuf[nOffs],&T1_utc,sizeof(T1_utc));
			nOffs+=sizeof(T1_utc);
			memcpy(&pucBuf[nOffs],&T2_utc,sizeof(T2_utc));
			nOffs+=sizeof(T2_utc);
			memcpy(&pucBuf[nOffs],&dwT1_utc_sec,sizeof(dwT1_utc_sec));
			nOffs+=sizeof(dwT1_utc_sec);
			memcpy(&pucBuf[nOffs],&dwT2_utc_sec,sizeof(dwT2_utc_sec));
			nOffs+=sizeof(dwT2_utc_sec);
			AnsiString s1=sDataFile;
			int nSize=s1.Length();
			memcpy(&pucBuf[nOffs],&nSize,sizeof(nSize));
			nOffs+=sizeof(nSize);
			if (nSize>0)
				{
				memcpy(&pucBuf[nOffs],s1.c_str(),nSize);
				nOffs+=nSize;
				}
		}


	};

	bool bFileInIndex(const String sFile);

   String sLogFile;

	JFile* pTestFil;

	String sIndexFolder;
	String sIndexFile;
	IndexHeader Header;
	JList<IndexEntry> IndexList;



	JList<VesselStruct> VesselList;

	JList<SearchInfoStruct> SearchList;

	SEARCH_PAR SearchPar;
	String sSearchName;
	String sSearchCall;
	int nSearchMMSI;
	int nSearchIMO;

	int anMsgCnt[CST4_MSG_CNT];

	struct SourceCntStruct
	{
		int anCnt[CST4_MSG_CNT];
	} aSourceCnt[65536];

	String sDataFolder;

	TListBox* pFileLB;
	TProgressBar* pProgBar;

	bool bAborted;

	JTime DataFrom_utc,DataTo_utc;

	bool bOpenIndexFile_utc(const JTime& T_utc);

	bool bUpdateRT_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile);

	bool bRebuild_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile);

	int nImport_utc(const JTime& T1_utc, const JTime& T2_utc, const bool bCreateVesselList, TMessageBarForm* pMsgBar, const bool bPackData=true);

	int nImportFile_utc(const String sFile, const DWORD dwT1_utc_sec, const DWORD dwT2_utc_sec, const bool bCreateVesselList);

	bool bValidTime_utc(const DWORD dwT_utc_sec, const DWORD dwT1_utc_sec, const DWORD dwT2_utc_sec) const
	{
		return (dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec);
	}

	bool bStaticPresent(const Cst4_AIS_Static* pStat);

	void AddVessel(const int nMMSI, const int nIMO, const String sName);
	void AddVessel(Cst4_AIS_Static* pStatic);
	void AddVessel(Cst4_AIS_Dynamic* pDynamic);

	void CheckSearch(Cst4_AIS_Static* pStatic);
	void CheckSearch(Cst4_AIS_Dynamic* pDynamic);

public:

	Cst4DataIndex(const String _sIndexFolder, const bool bNewAvail=false);
	~Cst4DataIndex();

	JList<Cst4_AIS_Dynamic> AIS_D;
	JList<Cst4_AIS_Static> AIS_S;
	JList<Cst4_Radar_Scan> Radar_S;
	JList<Cst4_Radar_Track> Radar_T;
	JList<Cst4_IFF_Track> IFF_T;
	JList<Cst4_ADSB> ADSB_T;
	JList<Cst4_SysTrack> Sys_T;
	JList<Cst4_User_Msg> User_M;

	bool bOpenIndexFile(const String sFile);
	bool bSaveIndexFile(const String sFile);

	bool bUpdateRTIndex_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile);

	void RebuildIndex_utc(const JTime& T1_utc, const JTime& T2_utc, const String sDataFile);

	void SetTellBack(TListBox* _pFileLB, TProgressBar* _pProgBar)
	{
		pFileLB=_pFileLB;
		pProgBar=_pProgBar;
	}

	int nImportData_utc(const String _sDataFolder, const JTime& T1_utc, const JTime& T2_utc, const bool bCreateVesselList, TMessageBarForm* pMsgBar, const bool bPackData=true);


	int nGetVesselCnt()
	{
		return VesselList.nGetCount();
	}

	VesselStruct* pGetVessel(const int n)
	{
		return VesselList[n];
	}

	void SortVesselName();
	void SortVesselMMSI();
	void SortVesselIMO();

	void SetSearchOptions(const SEARCH_PAR Par, const String sName, const String sCallsign, const int nMMSI, const int nIMO)
	{
		SearchPar=Par;
		sSearchName=sName.UpperCase().Trim();
		sSearchCall=sCallsign.UpperCase().Trim();
		nSearchMMSI=nMMSI;
		nSearchIMO=nIMO;
	}

	int nGetSearchCnt()
	{
		return SearchList.nGetCount();
	}

	SearchInfoStruct* pGetSearchInfo(const int n)
	{
		return SearchList[n];
	}

	void CreateLogFile()
	{
      sLogFile=sGetExeDir()+"Index Data Log_"+JTime(true).sFileName("txt");
	}

	String sGetLogFile() const
	{
		return sLogFile;
	}

	void StoreLog(const String sFileName, const String s)
	{
		if (!sFileName.IsEmpty())
			{
			JFile::FILE_ERROR E;
			JFile Fil('B',JFile::ASCII_TYPE);
			if (!bFileExist(sFileName))
				E=Fil.Create(sFileName);
			else
				E=Fil.Open(sFileName);
			if (!E)
				E=Fil.AppendTextLine(s);
			}
	}

	void SetAbort()
	{
		bAborted=true;
	}

	void GetDataTimeSpan_utc(JTime& T1_utc, JTime& T2_utc)
	{
		T1_utc=DataFrom_utc;
		T2_utc=DataTo_utc;
	}

	int nMsgCnt(const CST4_MSG_TYPE MsgType) const
	{
		return anMsgCnt[MsgType];
	}

	int nDynamicCnt()                         {return AIS_D.nGetCount();    }
	Cst4_AIS_Dynamic* pDynamic(const int n)   {return AIS_D[n];             }
	Cst4_AIS_Dynamic& Dynamic(const int n)    {return *AIS_D[n];            }

	int nStaticCnt()                          {return AIS_S.nGetCount();    }
	Cst4_AIS_Static* pStatic(const int n)     {return AIS_S[n];             }
	Cst4_AIS_Static& Static(const int n)      {return *AIS_S[n];            }

	int nScanCnt()                            {return Radar_S.nGetCount();  }
	Cst4_Radar_Scan* pScan(const int n)       {return Radar_S[n];           }
	Cst4_Radar_Scan& Scan(const int n)        {return *Radar_S[n];          }

	int nTrackCnt()                           {return Radar_T.nGetCount();  }
	Cst4_Radar_Track* pTrack(const int n)     {return Radar_T[n];           }
	Cst4_Radar_Track& Track(const int n)      {return *Radar_T[n];          }

	int nIFFCnt()                             {return IFF_T.nGetCount();    }
	Cst4_IFF_Track* pIFF(const int n)         {return IFF_T[n];             }
	Cst4_IFF_Track& IFF(const int n)          {return *IFF_T[n];            }

	int nADSBCnt()                             {return ADSB_T.nGetCount();    }
	Cst4_ADSB* pADSB(const int n)         		 {return ADSB_T[n];             }
	Cst4_ADSB& ADSB(const int n)          		  {return *ADSB_T[n];            }

	int nSysTrackCnt()                        {return Sys_T.nGetCount();    }
	Cst4_SysTrack* pSysTrack(const int n)   	{return Sys_T[n];             }
	Cst4_SysTrack& SysTrack(const int n)    	{return *Sys_T[n];            }

	int nUserCnt()                            {return User_M.nGetCount();   }
	Cst4_User_Msg* pUser(const int n)        	{return User_M[n];            }
	Cst4_User_Msg& User(const int n)         	{return *User_M[n];           }

	int nGetSourceCnt(const CST4_MSG_TYPE MsgType, const WORD wSource) const
	{
		return aSourceCnt[wSource].anCnt[MsgType];
	}

	int nGetIndexCount()
	{
		return IndexList.nGetCount();
	}

	String sGetIndexEntryFile(const int n)
	{
		return IndexList[n]->sDataFile;
	}

	void GetIndexEntryTimeSpan(const int n, JTime& T1_utc, JTime& T2_utc)
	{
		T1_utc=IndexList[n]->T1_utc;
		T2_utc=IndexList[n]->T2_utc;
	}

	void DeleteIndexEntry(const int n)
	{
		IndexList.Delete(n);
		--Header.nFiles;
		IndexList.Pack();
	}

	void AddIndexEntry_utc(const int sDataFile, const JTime& T1_utc, const JTime& T2_utc)
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
	}

	bool bRemoveFile(const String sFile);



};

struct NewFileListStruct
{
   char szName[256];
   JTime T1_utc,T2_utc;

   NewFileListStruct()
   {
      szName[0]=0;
		T1_utc.Invalidate();
		T2_utc.Invalidate();
	}

	NewFileListStruct(const String sName, const JTime& _T1_utc, const JTime& _T2_utc)
	{
		strncpy(szName,AnsiString(sName).c_str(),255);
		szName[255]=0;
		T1_utc=_T1_utc;
		T2_utc=_T2_utc;
	}

};


#endif
