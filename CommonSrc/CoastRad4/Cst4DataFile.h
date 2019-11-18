//---------------------------------------------------------------------------

#ifndef Cst4DataFileH
#define Cst4DataFileH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "VistaAlarmMessages.h"
#include "JMemFile.h"
#include "JUtils.h"

class Cst4DataFile
{

	enum
	{
//		CST4_DATA_VERSION = 10100, 	//Pre XE2 (Header Size = 1608)
//		CST4_DATA_VERSION = 10200,		//XE2 version (with error -> Header Size=1592)
//		CST4_DATA_VERSION = 10201,    //Fixed XE2 version (Header Size = 1608)
//		CST4_DATA_VERSION = 10202,    //Replace SAR messages with SysTrack Messages
//		CST4_DATA_VERSION = 10203,    //Add support for Alarm messages
//		CST4_DATA_VERSION = 10204,    //Remove support for Alarm Acknowledge messages
		CST4_DATA_VERSION = 10205,    //Add support for ADSB messages
	};

#pragma option push -a4    //Set compiler to DWORD alignment

	struct HeaderStruct
	{
		DWORD dwDataVersion;
		DWORD dwSWVersion;
		JTime T1_utc;
		JTime T2_utc;

		DWORD dwAIS_D_Size;
		DWORD dwAIS_S_Size;
		DWORD dwRadar_S_Size;
		DWORD dwRadar_T_Size;

		int nAIS_D_cnt;
		int nAIS_S_cnt;
		int nRadar_S_cnt;
		int nRadar_T_cnt;

		int anAISSourceCnt_10000[256]; //Keep in for compatibility with version 10000

		DWORD dwIFF_T_Size;
		int nIFF_T_cnt;

		DWORD dwSysT_Size; //New in ver 10202
		int nSysT_cnt;     //New in ver 10202

		DWORD dwUser_Size;
		int nUser_cnt;

		DWORD dwAlarmT_Size;     //New in ver 10203
		int nAlarmT_cnt;         //New in ver 10203

		DWORD dwAlarmCfg_Size;   //New in ver 10203
		int nAlarmCfg_cnt;       //New in ver 10203

		DWORD dwADSB_T_Size;      //New in ver 10205
		int nADSB_T_cnt;          //New in ver 10205

		DWORD adwDummy[122];

		void Init()
		{
			memset(this,0,sizeof(HeaderStruct));
			T1_utc.Invalidate();
			T2_utc.Invalidate();
		}

		HeaderStruct()
		{
			Init();
		}

		void PackSources(const int* pnSourceCnt, const int nMaxSources);
		void UnpackSources(int* pnSourceCnt, const int nMaxSources);

		int nRead(JFile* pFil)
		{
			JFile::FILE_ERROR E=pFil->Read(this,1592);
			if (!E)
				{
				if (dwDataVersion!=10200)
					{
					memset(adwDummy,0,sizeof(adwDummy));
					if (dwDataVersion>=10204)
						return 1632;
					else
						return 1608;
					}
				else
					return 1592;
				}
			return 0;
		}

		int nRead(JMemFile* pFil)
		{
			if (pFil->bRead(this,1592))
				{
				if (dwDataVersion!=10200)
					{
					memset(adwDummy,0,sizeof(adwDummy));
					if (dwDataVersion>=10204)
						return 1632;
					else
						return 1608;
					}
				else
					return 1592;
				}
			return 0;
		}


	} Cst4Header;

#pragma option pop    //Set compiler back to default alignment

	enum
   {
   	FATAL_ERROR_CNT	= 100,
   };

   int anSourceCnt[65536];

   JList<Cst4_AIS_Dynamic> AIS_D;
   JList<Cst4_AIS_Static> AIS_S;
   JList<Cst4_Radar_Scan> Radar_S;
   JList<Cst4_Radar_Track> Radar_T;
	JList<Cst4_IFF_Track> IFF_T;
	JList<Cst4_ADSB> ADSB_T;
	JList<Cst4_SysTrack> Sys_T;
	JList<Cst4_User_Msg> User_M;
	JList<VistaAlarmTrigger> Alarm_T;
	JList<VistaAlarmCfg> Alarm_Cfg;

	DWORD dwT1_utc;
	DWORD dwT2_utc;

	void CheckTime_utc(const JTime& T_utc)
	{
		if (T_utc.bValid())
			{
			DWORD dwT_utc=T_utc.dwTotalSec();
			if (dwT_utc<dwT1_utc)
				{
				Cst4Header.T1_utc=T_utc;
				dwT1_utc=dwT_utc;
				}
			if (dwT_utc>dwT2_utc)
				{
				Cst4Header.T2_utc=T_utc;
				dwT2_utc=dwT_utc;
				}
			}
	}

   struct MMSIStruct
   {
      int nMMSI;
		double dT1_utc_sec;
		double dT2_utc_sec;
		int nCnt;
		bool abSource2[65536];
	};

	JFile* pTestFil;

	String sFileName;

	int nDiscMsgCnt;

	void CheckMinMax(const double dT_sec, double& dT1_sec, double& dT2_sec, const double dMin_sec, const double dMax_sec, const WORD wSource);
	bool bVerifyHeader(const String sFile);

	JFile::FILE_ERROR SaveAIS_D(JFile* pFil);
	JFile::FILE_ERROR SaveAIS_S(JFile* pFil);
	JFile::FILE_ERROR SaveRadar_S(JFile* pFil);
	JFile::FILE_ERROR SaveRadar_T(JFile* pFil);
	JFile::FILE_ERROR SaveIFF_T1(JFile* pFil);
	JFile::FILE_ERROR SaveADSB_T(JFile* pFil);
	JFile::FILE_ERROR SaveSys_T(JFile* pFil);
	JFile::FILE_ERROR SaveUser_M(JFile* pFil);
	JFile::FILE_ERROR SaveAlarm_T(JFile* pFil);
	JFile::FILE_ERROR SaveAlarm_Cfg(JFile* pFil);

	bool bReadAIS_D(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadAIS_S(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadRadar_S(	const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadRadar_T(	const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadIFF_T1(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadADSB_T(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadSys_T(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadUser_M(		const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadAlarm_T(	const BYTE* pucBuf,	const int nBufSize, int& nOffs);
	bool bReadAlarm_Cfg(	const BYTE* pucBuf,	const int nBufSize, int& nOffs);



public:

	Cst4DataFile(const String _sFileName="");
	~Cst4DataFile();

	void Init();

	String sGetFileName()
	{
		return sFileName;
	}

	void AddMessage(Cst4_AIS_Dynamic& Msg)
	{
		AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwAIS_D_Size+=Msg.dwCalcSize();
		++anSourceCnt[Msg.wSource()];
	}

	DWORD dwGetAIS_D_Size() const
	{
		return Cst4Header.dwAIS_D_Size;
	}

	void AddMessage(Cst4_AIS_Static& Msg)
	{
		AIS_S.nAdd(new Cst4_AIS_Static(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwAIS_S_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_Radar_Scan& Msg)
	{
		Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwRadar_S_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_Radar_Track& Msg)
	{
		Radar_T.nAdd(new Cst4_Radar_Track(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwRadar_T_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_IFF_Track& Msg)
	{
		IFF_T.nAdd(new Cst4_IFF_Track(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwIFF_T_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_ADSB& Msg)
	{
		ADSB_T.nAdd(new Cst4_ADSB(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwADSB_T_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_SysTrack& Msg)
	{
		Sys_T.nAdd(new Cst4_SysTrack(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwSysT_Size+=Msg.dwCalcSize();
	}

	void AddMessage(Cst4_User_Msg& Msg)
	{
		User_M.nAdd(new Cst4_User_Msg(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwUser_Size+=Msg.dwCalcSize();
		++anSourceCnt[Msg.wSource()];
	}

	void AddMessage(VistaAlarmTrigger& Msg)
	{
		Alarm_T.nAdd(new VistaAlarmTrigger(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwAlarmT_Size+=Msg.dwCalcSize();
		++anSourceCnt[Msg.wSource()];
	}

	void AddMessage(VistaAlarmCfg& Msg)
	{
		Alarm_Cfg.nAdd(new VistaAlarmCfg(Msg));
		CheckTime_utc(Msg.Time_utc());
		Cst4Header.dwAlarmCfg_Size+=Msg.dwCalcSize();
		++anSourceCnt[Msg.wSource()];
	}



	bool bSaveFile(const String sFile="", const bool bSortMessages=false, const bool bClearData=true);

	void GetTimeSpan_utc(JTime& T1_utc, JTime& T2_utc)
	{
		T1_utc=Cst4Header.T1_utc.ReturnUTC(g_nCst4TimeBias_hours); 	//always check for UTC
		T2_utc=Cst4Header.T2_utc.ReturnUTC(g_nCst4TimeBias_hours);  //always check for UTC
	}

	DWORD dwFrom_utc_sec() const
	{
		return Cst4Header.T1_utc.ReturnUTC(g_nCst4TimeBias_hours).dwTotalSec();
	}

	DWORD dwTo_utc_sec() const
	{
		return Cst4Header.T2_utc.ReturnUTC(g_nCst4TimeBias_hours).dwTotalSec();
	}

	double dFrom_utc_sec() const
	{
		return Cst4Header.T1_utc.ReturnUTC(g_nCst4TimeBias_hours).dTotalSec();
	}

	double dTo_utc_sec() const
	{
		return Cst4Header.T2_utc.ReturnUTC(g_nCst4TimeBias_hours).dTotalSec();
	}

	void GetTimeSpan_utc(DWORD& dwT1_utc_sec, DWORD& dwT2_utc_sec)
	{
		dwT1_utc_sec=dwFrom_utc_sec();
		dwT2_utc_sec=dwTo_utc_sec();
	}

	JTime FirstTime_utc() const
	{
		return Cst4Header.T1_utc.ReturnUTC(g_nCst4TimeBias_hours);
	}

	JTime LastTime_utc() const
	{
		return Cst4Header.T2_utc.ReturnUTC(g_nCst4TimeBias_hours);
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

	bool bReadFile(const String sFile, const bool bCheckHeader);
//	bool bReadFile(const String sFile, const bool bCheckHeader, const String sLogFile, const bool bRegenTimeSpan=false);

	bool bReadFileHeader(const String sFile);

	int nTotalDataCnt() const
	{
		return AIS_D.nGetCount()
				+AIS_S.nGetCount()
				+Radar_S.nGetCount()
				+Radar_T.nGetCount()
				+IFF_T.nGetCount()
				+ADSB_T.nGetCount()
				+Sys_T.nGetCount()
				+User_M.nGetCount()
				+Alarm_T.nGetCount()
				+Alarm_Cfg.nGetCount();
	}

	int nTotalHeaderCnt() const
	{
		return Cst4Header.nAIS_D_cnt
					+Cst4Header.nAIS_S_cnt
					+Cst4Header.nRadar_S_cnt
					+Cst4Header.nRadar_T_cnt
					+Cst4Header.nIFF_T_cnt
					+Cst4Header.nADSB_T_cnt
					+Cst4Header.nSysT_cnt
					+Cst4Header.nUser_cnt
					+Cst4Header.nAlarmT_cnt
					+Cst4Header.nAlarmCfg_cnt;
	}

	int nGetSourceCnt(const WORD wSource) const
	{
		return anSourceCnt[wSource];
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
	Cst4_ADSB* pADSB(const int n)         		{return ADSB_T[n];             }
	Cst4_ADSB& ADSB(const int n)          		{return *ADSB_T[n];            }

	int nSysTrackCnt()                        {return Sys_T.nGetCount();    }
	Cst4_SysTrack* pSysTrack(const int n)   	{return Sys_T[n];             }
	Cst4_SysTrack& SysTrack(const int n)    	{return *Sys_T[n];            }

	int nUserCnt()                         	{return User_M.nGetCount();   }
	Cst4_User_Msg* pUser(const int n)   		{return User_M[n];            }
	Cst4_User_Msg& User(const int n)    		{return *User_M[n];           }

	int nAlarmTCnt()                         	{return Alarm_T.nGetCount();   }
	VistaAlarmTrigger* pAlarmT(const int n)	{return Alarm_T[n];            }
	VistaAlarmTrigger& AlarmT(const int n) 	{return *Alarm_T[n];           }

	int nAlarmCfgCnt()                       	{return Alarm_Cfg.nGetCount(); }
	VistaAlarmCfg* pAlarmCfg(const int n) 		{return Alarm_Cfg[n];          }
	VistaAlarmCfg& AlarmCfg(const int n)  		{return *Alarm_Cfg[n];         }


	int nConsolidateAISTargets();

	void Append(Cst4DataFile& F, const DWORD dwT1_utc_sec, const DWORD dwT2_utc_sec);

	bool bRegenTimeSpan(const String sFile, const DWORD dwMin_utc_sec, const DWORD dwMax_utc_sec);

	String sGetHeaderInfo();
	void ExtractText(const String sFile);

   bool bAborted;

};


#endif
