//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4DataFile.h"
#include "JUtils.h"
#include "Cst4SiteDefs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Cst4DataFile::HeaderStruct::PackSources(const int* pnSourceCnt, const int nMaxSources)
{
   memset(anAISSourceCnt_10000,0,sizeof(anAISSourceCnt_10000));
   int n=0;
   for (int i=0; i<nMaxSources; i++)
      {
      if (pnSourceCnt[i]>0)
         {
         if (n<254)
            {
            anAISSourceCnt_10000[n++]=i;
            anAISSourceCnt_10000[n++]=pnSourceCnt[i];
            }
         }
      }
}

void Cst4DataFile::HeaderStruct::UnpackSources(int* pnSourceCnt, const int nMaxSources)
{
   memset(pnSourceCnt,0,sizeof(int)*nMaxSources);
   if (dwDataVersion<10100)
      {
      for (int i=0; i<256; i++)
         pnSourceCnt[i]=anAISSourceCnt_10000[i];
      }
   else
      {
      for (int i=0; i<128; i++)
         {
         if (anAISSourceCnt_10000[i*2]>0)
         	{
            int n=anAISSourceCnt_10000[i*2];
            if (n<256)
	            pnSourceCnt[n]=anAISSourceCnt_10000[i*2+1];
            else
            	i=128;
            }
         else
            i=128;
         }
      }
}


Cst4DataFile::Cst4DataFile(const String _sFileName)
{
	sFileName=_sFileName;
	pTestFil=NULL;
   Init();
}

Cst4DataFile::~Cst4DataFile()
{
	delete pTestFil;
}

void Cst4DataFile::Init()
{
	dwT1_utc=0xFFFFFFFF;
	dwT2_utc=0;
	Cst4Header.Init();
	AIS_D.Clear();
	AIS_S.Clear();
	Radar_S.Clear();
	Radar_T.Clear();
	IFF_T.Clear();
	ADSB_T.Clear();
	Sys_T.Clear();
	User_M.Clear();
	Alarm_T.Clear();
//	Alarm_A.Clear();
	bAborted=false;
}

JFile::FILE_ERROR Cst4DataFile::SaveAIS_D(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwAIS_D_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwAIS_D_Size];
		AIS_D.GoFirst();
		while(!AIS_D.bLast())
			AIS_D.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveAIS_S(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwAIS_S_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwAIS_S_Size];
		AIS_S.GoFirst();
		while(!AIS_S.bLast())
			AIS_S.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveRadar_S(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwRadar_S_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwRadar_S_Size];
		Radar_S.GoFirst();
		while(!Radar_S.bLast())
			Radar_S.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveRadar_T(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwRadar_T_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwRadar_T_Size];
		Radar_T.GoFirst();
		while(!Radar_T.bLast())
			Radar_T.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveIFF_T1(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwIFF_T_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwIFF_T_Size];
		IFF_T.GoFirst();
		while(!IFF_T.bLast())
			IFF_T.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveADSB_T(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwADSB_T_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwADSB_T_Size];
		ADSB_T.GoFirst();
		while(!ADSB_T.bLast())
			ADSB_T.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveSys_T(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwSysT_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwSysT_Size];
		Sys_T.GoFirst();
		while(!Sys_T.bLast())
			Sys_T.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveUser_M(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwUser_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwUser_Size];
		User_M.GoFirst();
		while(!User_M.bLast())
			User_M.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveAlarm_T(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwAlarmT_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwAlarmT_Size];
		Alarm_T.GoFirst();
		while(!Alarm_T.bLast())
			Alarm_T.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}

JFile::FILE_ERROR Cst4DataFile::SaveAlarm_Cfg(JFile* pFil)
{
	JFile::FILE_ERROR E=JFile::F_NO_ERROR;
	if (Cst4Header.dwAlarmCfg_Size>0)
		{
		int nOffs=0;
		BYTE* pucBuf=new BYTE[Cst4Header.dwAlarmCfg_Size];
		Alarm_Cfg.GoFirst();
		while(!Alarm_Cfg.bLast())
			Alarm_Cfg.pNext()->Pack(pucBuf,nOffs);
		E=pFil->Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	return E;
}




bool Cst4DataFile::bSaveFile(const String sFile, const bool bSortMessages, const bool bClearData)
{
	Cst4Header.dwDataVersion=CST4_DATA_VERSION;
	Cst4Header.dwSWVersion=dwGetExeVersion();;

	Cst4Header.nAIS_D_cnt=AIS_D.nGetCount();
	Cst4Header.nAIS_S_cnt=AIS_S.nGetCount();
	Cst4Header.nRadar_S_cnt=Radar_S.nGetCount();
	Cst4Header.nRadar_T_cnt=Radar_T.nGetCount();
	Cst4Header.nIFF_T_cnt=IFF_T.nGetCount();
	Cst4Header.nADSB_T_cnt=ADSB_T.nGetCount();
	Cst4Header.nSysT_cnt=Sys_T.nGetCount();
	Cst4Header.nUser_cnt=User_M.nGetCount();
	Cst4Header.nAlarmT_cnt=Alarm_T.nGetCount();
	Cst4Header.nAlarmCfg_cnt=Alarm_Cfg.nGetCount();

	if (bSortMessages)
		{
		AIS_D.Sort();
		AIS_S.Sort();
		Radar_S.Sort();
		Radar_T.Sort();
		IFF_T.Sort();
		ADSB_T.Sort();
		Sys_T.Sort();
		User_M.Sort();
		//AlarmT and AlarmCfg are NOT sorted
		}

	if (!sFile.IsEmpty())
		sFileName=sFile;
	JFile::FILE_ERROR E;
	JFile Fil('O');
	if (sFileName.IsEmpty())
		E=JFile::F_OPEN_ERROR;
	else
		E=Fil.Create(sFileName);
	Cst4Header.PackSources(anSourceCnt,65536);
	if (!E)
		E=Fil.Write(&Cst4Header,sizeof(Cst4Header));

	if (!E) E=SaveAIS_D(&Fil);
	if (!E) E=SaveAIS_S(&Fil);
	if (!E) E=SaveRadar_S(&Fil);
	if (!E) E=SaveRadar_T(&Fil);
	if (!E) E=SaveIFF_T1(&Fil);
	if (!E) E=SaveSys_T(&Fil);
	if (!E) E=SaveUser_M(&Fil);
	if (!E) E=SaveAlarm_T(&Fil);
	if (!E) E=SaveAlarm_Cfg(&Fil);
	if (!E) E=SaveADSB_T(&Fil);

	if (bClearData)
		Init();
	if (!E)
		E=Fil.Close();

	return (E==JFile::F_NO_ERROR);
}

bool Cst4DataFile::bVerifyHeader(const String sFile)
{
	JTime FNT;
	FNT.SetTimeFromFileName(sFile);
	//Convert to UTC if not UTC
	Cst4Header.T1_utc.ConvertToUTC(g_nCst4TimeBias_hours);
	Cst4Header.T2_utc.ConvertToUTC(g_nCst4TimeBias_hours);
	DWORD dwT1_utc_sec=Cst4Header.T1_utc.dwTotalSec();
	DWORD dwT2_utc_sec=Cst4Header.T2_utc.dwTotalSec();
	DWORD dwMin_utc_sec,dwMax_utc_sec;
	if (FNT.bValid())
		{
		DWORD dwFNT=FNT.dwTotalSec();
		dwMin_utc_sec=dwFNT-86400*7;
		dwMax_utc_sec=dwFNT+86400*7;
		}
	else
		{
		dwMin_utc_sec=0;
		dwMax_utc_sec=0xFFFFFFFF;
		}
   bool bOK;
	bool bValidTimeSpan=true;
	if ((dwT1_utc_sec<dwMin_utc_sec)||(dwT1_utc_sec>dwMax_utc_sec))
		bValidTimeSpan=false;
	if ((dwT2_utc_sec<dwMin_utc_sec)||(dwT2_utc_sec>dwMax_utc_sec))
		bValidTimeSpan=false;
	if (bValidTimeSpan)
		{
		JTime T=Cst4Header.T1_utc;
		if ((T.Get().wHour>23)||(T.Get().wMinute>59)||(T.Get().wSecond>59))
			bValidTimeSpan=false;
		T=Cst4Header.T2_utc;
		if ((T.Get().wHour>23)||(T.Get().wMinute>59)||(T.Get().wSecond>59))
			bValidTimeSpan=false;
		}
	if (bValidTimeSpan)
		bOK=true;
	else
		bOK=bRegenTimeSpan(sFile,dwMin_utc_sec,dwMax_utc_sec);
   return bOK;
}

bool Cst4DataFile::bReadFileHeader(const String sFile)
{
	JFile Fil;
	Init();
   bool bOK=true;
	JFile::FILE_ERROR E=Fil.Open(sFile);
	if (!E)
		{
		if (Cst4Header.nRead(&Fil)==0)
      	{
			E=JFile::F_READ_ERROR;
         bOK=false;
         }
		}
	else
   	bOK=false;
	if (!E)
		{
		E=Fil.Close();
		if (!bVerifyHeader(sFile))
			E=JFile::F_FORMAT_ERROR;
		}
	if (E)
   	bOK=false;
   else
		Cst4Header.UnpackSources(anSourceCnt,65536);
	return bOK;
}


void Cst4DataFile::CheckMinMax(const double dT_sec, double& dT1_sec, double& dT2_sec, const double dMin_sec, const double dMax_sec, const WORD wSource)
{
	String s;
	s.printf(L"%04X",wSource);
	if ((dT_sec>dMin_sec)&&(dT_sec<dMax_sec))
		{
		if (dT_sec<dT1_sec)
			dT1_sec=dT_sec;
		if (dT_sec>dT2_sec)
			dT2_sec=dT_sec;
		}
	else
		{
		s="****  "+s;
		}
	if (pTestFil)
		pTestFil->TextLine(s+"   "+JTime(dT_sec).sDateTime());
}

bool Cst4DataFile::bRegenTimeSpan(const String sFile, const DWORD dwMin_utc_sec, const DWORD dwMax_utc_sec)
{
	Cst4DataFile Temp;
	bool bOK;
//	if (Temp.bReadFile(sFile,true))
	if (Temp.bReadFile(sFile,false))
		{
		bOK=true;
		double dMin_utc_sec=dwMin_utc_sec;
		double dMax_utc_sec=dwMax_utc_sec;
		double dT_sec;
		double dT1_sec=1e38;
		double dT2_sec=0.0;
		for (int i=0; i<Temp.nDynamicCnt(); i++)
			CheckMinMax(Temp.pDynamic(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pDynamic(i)->wSource());
		//Do not need to check AIS static
		for (int i=0; i<Temp.nScanCnt(); i++)
			CheckMinMax(Temp.pScan(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pScan(i)->wSource());
		for (int i=0; i<Temp.nTrackCnt(); i++)
			CheckMinMax(Temp.pTrack(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pTrack(i)->wSource());
		for (int i=0; i<Temp.nIFFCnt(); i++)
			CheckMinMax(Temp.pIFF(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pIFF(i)->wSource());
		for (int i=0; i<Temp.nADSBCnt(); i++)
			CheckMinMax(Temp.pADSB(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pADSB(i)->wSource());
		for (int i=0; i<Temp.nSysTrackCnt(); i++)
			CheckMinMax(Temp.pSysTrack(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pSysTrack(i)->wSource());
		for (int i=0; i<Temp.nUserCnt(); i++)
			CheckMinMax(Temp.pUser(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pUser(i)->wSource());
		for (int i=0; i<Temp.nAlarmTCnt(); i++)
			CheckMinMax(Temp.pAlarmT(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pAlarmT(i)->wSource());
//		for (int i=0; i<Temp.nAlarmACnt(); i++)
//			CheckMinMax(Temp.pAlarmA(i)->dTime_sec_utc(),dT1_sec,dT2_sec,dMin_utc_sec,dMax_utc_sec,Temp.pAlarmA(i)->wSource());
		Cst4Header.T1_utc=JTime(dT1_sec,true);
		Cst4Header.T2_utc=JTime(dT2_sec,true);
		}
	else
		bOK=false;
	return bOK;
}


bool Cst4DataFile::bReadAIS_D(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nAIS_D_cnt>0)
		{
		for (int i=0; i<Cst4Header.nAIS_D_cnt; i++)
			{
			Cst4_AIS_Dynamic* pMsg=new Cst4_AIS_Dynamic;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				{
				LatLong LL=pMsg->GetLL();
				bool bValidTime=pMsg->Time_utc().bAllValid();
				if ((LL.dLat<181.0)&&(bValidTime))
					AIS_D.nAdd(pMsg);
				else
					delete pMsg;
				}
			else
				delete pMsg;
			}
		AIS_D.Pack();
		return (AIS_D.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadAIS_S(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nAIS_S_cnt>0)
		{
		for (int i=0; i<Cst4Header.nAIS_S_cnt; i++)
			{
			Cst4_AIS_Static* pMsg=new Cst4_AIS_Static;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				AIS_S.nAdd(pMsg);
			else
				delete pMsg;
			}
		AIS_S.Pack();
		return (AIS_S.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadRadar_S(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nRadar_S_cnt>0)
		{
		for (int i=0; i<Cst4Header.nRadar_S_cnt; i++)
			{
			Cst4_Radar_Scan* pMsg=new Cst4_Radar_Scan;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				Radar_S.nAdd(pMsg);
			else
				delete pMsg;
			}
		Radar_S.Pack();
		return (Radar_S.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadRadar_T(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nRadar_T_cnt>0)
		{
		for (int i=0; i<Cst4Header.nRadar_T_cnt; i++)
			{
			Cst4_Radar_Track* pMsg=new Cst4_Radar_Track;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				Radar_T.nAdd(pMsg);
			else
				delete pMsg;
			}
		Radar_T.Pack();
		return (Radar_T.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadIFF_T1(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nIFF_T_cnt>0)
		{
		for (int i=0; i<Cst4Header.nIFF_T_cnt; i++)
			{
			Cst4_IFF_Track* pMsg=new Cst4_IFF_Track;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				IFF_T.nAdd(pMsg);
			else
				delete pMsg;
			}
		IFF_T.Pack();
		return (IFF_T.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadADSB_T(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nADSB_T_cnt>0)
		{
		for (int i=0; i<Cst4Header.nADSB_T_cnt; i++)
			{
			Cst4_ADSB* pMsg=new Cst4_ADSB;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				ADSB_T.nAdd(pMsg);
			else
				delete pMsg;
			}
		ADSB_T.Pack();
		return (ADSB_T.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadSys_T(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nSysT_cnt>0)
		{
		for (int i=0; i<Cst4Header.nSysT_cnt; i++)
			{
			Cst4_SysTrack* pMsg=new Cst4_SysTrack;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				Sys_T.nAdd(pMsg);
			else
				delete pMsg;
			}
		Sys_T.Pack();
		return (Sys_T.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadUser_M(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nUser_cnt>0)
		{
		for (int i=0; i<Cst4Header.nUser_cnt; i++)
			{
			Cst4_User_Msg* pMsg=new Cst4_User_Msg;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				User_M.nAdd(pMsg);
			else
				delete pMsg;
			}
		User_M.Pack();
		return (User_M.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadAlarm_T(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nAlarmT_cnt>0)
		{
		for (int i=0; i<Cst4Header.nAlarmT_cnt; i++)
			{
			VistaAlarmTrigger* pMsg=new VistaAlarmTrigger;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				Alarm_T.nAdd(pMsg);
			else
				delete pMsg;
			}
		Alarm_T.Pack();
		return (Alarm_T.nGetCount()>0);
		}
	return true;
}

bool Cst4DataFile::bReadAlarm_Cfg(const BYTE* pucBuf,	const int nBufSize, int& nOffs)
{
	if (Cst4Header.nAlarmCfg_cnt>0)
		{
		for (int i=0; i<Cst4Header.nAlarmCfg_cnt; i++)
			{
			VistaAlarmCfg* pMsg=new VistaAlarmCfg;
			if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
				Alarm_Cfg.nAdd(pMsg);
			else
				delete pMsg;
			}
		Alarm_Cfg.Pack();
		return (Alarm_Cfg.nGetCount()>0);
		}
	return true;
}



bool Cst4DataFile::bReadFile(	const String sFile,
										const bool bCheckHeader)
{
	Init();
	bool bOK;
	delete pTestFil;
	pTestFil=NULL;
//	int nErrorCnt=0;
	JMemFile Fil;
	JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
	if (!E)
		{
		Init();
		int nOffs=Cst4Header.nRead(&Fil);
		bOK=(nOffs>0);
		if (bOK)
			{
			if (bCheckHeader)
				{
				bOK=bVerifyHeader(sFile);
				}
			else
				{
				//Convert to UTC if not UTC
				Cst4Header.T1_utc.ConvertToUTC(g_nCst4TimeBias_hours);
				Cst4Header.T2_utc.ConvertToUTC(g_nCst4TimeBias_hours);
				}
			}
		if (bOK)
			Cst4Header.UnpackSources(anSourceCnt,65536);
		BYTE* pucBuf=Fil.pucGetBuffer();
		int nBufSize=(int)Fil.dwGetSize();

		if (bOK) bOK=bReadAIS_D(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadAIS_S(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadRadar_S(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadRadar_T(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadIFF_T1(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadSys_T(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadUser_M(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadAlarm_T(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadAlarm_Cfg(pucBuf,nBufSize,nOffs);
		if (bOK) bOK=bReadADSB_T(pucBuf,nBufSize,nOffs);

		return bOK &&(!bAborted);
		}
	else
		return false;
}


//bool Cst4DataFile::bReadFile(const String sFile, const bool bCheckHeader, const String sLogFile, const bool bRegenTimeSpan)
//{
//	StoreLog(sLogFile,"Opening "+sFile);
//	Init();
//	bool bOK;
//	delete pTestFil;
//	int nErrorCnt=0;
////	pTestFil=new JFile('O',JFile::ASCII_TYPE);
////	pTestFil->Create(sFile+".test");
//	JMemFile Fil;
//	JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
//	if (!E)
//		{
//		double dMinT_utc_sec=1e38;
//		double dMaxT_utc_sec=0.0;
//		double dT_utc_sec;
//		double dFNT_utc_sec=0.0;
//		if (bRegenTimeSpan)
//			{
//			JTime FNT;
//			FNT.SetTimeFromFileName(sFile);
//			if (FNT.bValid())
//				dFNT_utc_sec=FNT.dTotalSec()-60.0; 	//1 minute less than file name time
//			}
//		Init();
//		int nOffs=Cst4Header.nRead(&Fil);
//		bOK=(nOffs>0);
//		if (bOK)
//			{
//			if (bCheckHeader)
//				{
//				StoreLog(sLogFile,"   Verify Header");
//				bOK=bVerifyHeader(sFile);
//				}
//			else
//				{
//				//Convert to UTC if not UTC
//				Cst4Header.T1_utc.ConvertToUTC(g_nCst4TimeBias_hours);
//				Cst4Header.T2_utc.ConvertToUTC(g_nCst4TimeBias_hours);
//				}
//			if (Cst4Header.T1_utc.bAllValid())
//				StoreLog(sLogFile,"   From "+Cst4Header.T1_utc.sDateTime(true,false,true));
//			if (Cst4Header.T2_utc.bAllValid())
//				StoreLog(sLogFile,"   To   "+Cst4Header.T2_utc.sDateTime(true,false,true));
//			}
//		if (bOK)
//			Cst4Header.UnpackSources(anSourceCnt,65536);
//		BYTE* pucBuf=Fil.pucGetBuffer();
//		int nBufSize=(int)Fil.dwGetSize();
//		nDiscMsgCnt=0;
//		if ((bOK)&&(Cst4Header.nAIS_D_cnt>0))
//			{
//			StoreLog(sLogFile,"   AIS_D: "+String(Cst4Header.nAIS_D_cnt));
//			for (int i=0; (i<Cst4Header.nAIS_D_cnt)&&(bOK); i++)
//				{
//				Cst4_AIS_Dynamic* pMsg=new Cst4_AIS_Dynamic;
//				if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
//					{
//					LatLong LL=pMsg->GetLL();
//					bool bValidTime=pMsg->Time_utc().bAllValid();
//					if (bValidTime && bRegenTimeSpan)
//						{
//						dT_utc_sec=pMsg->Time_utc().dTotalSec();
//						if (dT_utc_sec>dFNT_utc_sec)
//							{
//							if (dT_utc_sec<dMinT_utc_sec)
//								dMinT_utc_sec=dT_utc_sec;
//							if (dT_utc_sec>dMaxT_utc_sec)
//								dMaxT_utc_sec=dT_utc_sec;
//							}
//						}
//					if ((LL.dLat<181.0)&&(bValidTime))
//						AIS_D.nAdd(pMsg);
//					else
//						{
//						if (++nErrorCnt>FATAL_ERROR_CNT)
//							bOK=false;
//						WORD w=pMsg->wSource();
//						String s=sGetCst4Site(w)+")";
//						if (!bValidTime)
//							s=s+" Invalid Time";
//						StoreLog(sLogFile,"     Error: "+LL.sLL()+" ("+s);
//						++nDiscMsgCnt;
////						bOK=false;
//						delete pMsg;
//						}
//					}
//				else
//					{
//					if (++nErrorCnt>FATAL_ERROR_CNT)
//						bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w);
//					StoreLog(sLogFile,"      Error: Unpack ("+s+")");
//					++nDiscMsgCnt;
//					delete pMsg;
//					}
//				}
//			AIS_D.Pack();
//			}
//		if ((bOK)&&(Cst4Header.nAIS_S_cnt>0))
//			{
//			StoreLog(sLogFile,"   AIS_S: "+String(Cst4Header.nAIS_S_cnt));
//			for (int i=0; (i<Cst4Header.nAIS_S_cnt)&&(bOK); i++)
//				{
//				Cst4_AIS_Static* pMsg=new Cst4_AIS_Static;
//				if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
//					AIS_S.nAdd(pMsg);
//				else
//					{
//               if (++nErrorCnt>FATAL_ERROR_CNT)
//                  bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w);
//					StoreLog(sLogFile,"      Error: Unpack ("+s+")");
//					++nDiscMsgCnt;
////					bOK=false;
//					delete pMsg;
//					}
//				}
//			AIS_S.Pack();
//			}
//		if ((bOK)&&(Cst4Header.nRadar_S_cnt>0))
//			{
//			StoreLog(sLogFile,"   Radar_S: "+String(Cst4Header.nRadar_S_cnt));
//			for (int i=0; (i<Cst4Header.nRadar_S_cnt)&&(bOK); i++)
//				{
//				Cst4_Radar_Scan* pMsg=new Cst4_Radar_Scan;
//				bool bValid=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
//				bool bValidTime=pMsg->Time_utc().bAllValid();
//            if (bValidTime && bRegenTimeSpan)
//               {
//               dT_utc_sec=pMsg->Time_utc().dTotalSec();
//               if (dT_utc_sec>dFNT_utc_sec)
//                  {
//                  if (dT_utc_sec<dMinT_utc_sec)
//                     dMinT_utc_sec=dT_utc_sec;
//                  if (dT_utc_sec>dMaxT_utc_sec)
//                     dMaxT_utc_sec=dT_utc_sec;
//                  }
//               }
//				if (bValid)
//					Radar_S.nAdd(pMsg);
//				else
//					{
//               if (++nErrorCnt>FATAL_ERROR_CNT)
//                  bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w)+")";
//					if (!bValidTime)
//						s=s+" Invalid Time";
//					StoreLog(sLogFile,"      Error: Unpack ("+s);
//					++nDiscMsgCnt;
////					bOK=false;
//					delete pMsg;
//					}
//				}
//			Radar_S.Pack();
//			}
//		if ((bOK)&&(Cst4Header.nRadar_T_cnt>0))
//			{
//			StoreLog(sLogFile,"   Radar_T: "+String(Cst4Header.nRadar_T_cnt));
//			for (int i=0; (i<Cst4Header.nRadar_T_cnt)&&(bOK); i++)
//				{
//				Cst4_Radar_Track* pMsg=new Cst4_Radar_Track;
//				bool bValid=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
//				bool bValidTime=pMsg->Time_utc().bAllValid();
//				if (bValidTime && bRegenTimeSpan)
//               {
//               dT_utc_sec=pMsg->Time_utc().dTotalSec();
//               if (dT_utc_sec>dFNT_utc_sec)
//                  {
//                  if (dT_utc_sec<dMinT_utc_sec)
//                     dMinT_utc_sec=dT_utc_sec;
//                  if (dT_utc_sec>dMaxT_utc_sec)
//                     dMaxT_utc_sec=dT_utc_sec;
//                  }
//               }
//				if (bValid)
//					Radar_T.nAdd(pMsg);
//				else
//					{
//               if (++nErrorCnt>FATAL_ERROR_CNT)
//                  bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w)+")";
//					if (!bValidTime)
//						s=s+" Invalid Time";
//					StoreLog(sLogFile,"      Error: Unpack ("+s);
//					++nDiscMsgCnt;
////					bOK=false;
//					delete pMsg;
//					}
//				}
//			Radar_T.Pack();
//			}
//		if ((bOK)&&(Cst4Header.nIFF_T_cnt>0))
//			{
//			StoreLog(sLogFile,"   IFF: "+String(Cst4Header.nIFF_T_cnt));
//			for (int i=0; (i<Cst4Header.nIFF_T_cnt)&&(bOK); i++)
//				{
//				Cst4_IFF_Track* pMsg=new Cst4_IFF_Track;
//				bool bValid=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
//				bool bValidTime=pMsg->Time_utc().bAllValid();
//				if (bValidTime && bRegenTimeSpan)
//               {
//               dT_utc_sec=pMsg->Time_utc().dTotalSec();
//               if (dT_utc_sec>dFNT_utc_sec)
//                  {
//                  if (dT_utc_sec<dMinT_utc_sec)
//                     dMinT_utc_sec=dT_utc_sec;
//                  if (dT_utc_sec>dMaxT_utc_sec)
//                     dMaxT_utc_sec=dT_utc_sec;
//                  }
//               }
//				if ((bValid)&&(bValidTime))
//					IFF_T.nAdd(pMsg);
//				else
//					{
//               if (++nErrorCnt>FATAL_ERROR_CNT)
//                  bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w)+")";
//					if (!bValidTime)
//						s=s+" Invalid Time";
//					StoreLog(sLogFile,"      Error: Unpack ("+s);
//					++nDiscMsgCnt;
////					bOK=false;
//					delete pMsg;
//					}
//				}
//			IFF_T.Pack();
//			}
//		if ((bOK)&&(Cst4Header.nSysT_cnt>0))
//			{
//			StoreLog(sLogFile,"   SAR: "+String(Cst4Header.nSysT_cnt));
//			for (int i=0; (i<Cst4Header.nSysT_cnt)&&(bOK)&&(!bAborted); i++)
//				{
//				Cst4_SysTrack* pMsg=new Cst4_SysTrack;
//				if (pMsg->bUnpack(pucBuf,nOffs,nBufSize))
//            	{
//               if (bRegenTimeSpan)
//                  {
//                  dT_utc_sec=pMsg->Time_utc().dTotalSec();
//                  if (dT_utc_sec>dFNT_utc_sec)
//                     {
//                     if (dT_utc_sec<dMinT_utc_sec)
//                        dMinT_utc_sec=dT_utc_sec;
//                     if (dT_utc_sec>dMaxT_utc_sec)
//								dMaxT_utc_sec=dT_utc_sec;
//							}
//						}
//					Sys_T.nAdd(pMsg);
//					}
//				else
//					{
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w);
//					StoreLog(sLogFile,"   Error: Unpack ("+s+")");
//					++nDiscMsgCnt;
//					ShowError(sFile,"Fatal Data File Error (System Track)");
//					bOK=false;
//					delete pMsg;
//					}
//				}
//			Sys_T.Pack();
//			}
//
//		if ((bOK)&&(Cst4Header.nAlarmT_cnt>0))
//			{
//			StoreLog(sLogFile,"   Alarm: "+String(Cst4Header.nAlarmT_cnt));
//			for (int i=0; (i<Cst4Header.nAlarmT_cnt)&&(bOK)&&(!bAborted); i++)
//				{
//				VistaAlarmTrigger* pMsg=new VistaAlarmTrigger;
//				bool bValid=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
//				bool bValidTime=pMsg->Time_utc().bAllValid();
//				if (bValidTime && bRegenTimeSpan)
//					{
//					dT_utc_sec=pMsg->Time_utc().dTotalSec();
//					if (dT_utc_sec>dFNT_utc_sec)
//						{
//						if (dT_utc_sec<dMinT_utc_sec)
//							dMinT_utc_sec=dT_utc_sec;
//						if (dT_utc_sec>dMaxT_utc_sec)
//							dMaxT_utc_sec=dT_utc_sec;
//						}
//					}
//				if ((bValid)&&(bValidTime))
//					Alarm_T.nAdd(pMsg);
//				else
//					{
//					if (++nErrorCnt>FATAL_ERROR_CNT)
//						bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w)+")";
//					if (!bValidTime)
//						s=s+" Invalid Time";
//					StoreLog(sLogFile,"   Error: Unpack (");
//					++nDiscMsgCnt;
//					delete pMsg;
//					}
//				}
//			Alarm_T.Pack();
//			}
//
//		if ((bOK)&&(Cst4Header.nAlarmCfg_cnt>0))
//			{
//			StoreLog(sLogFile,"   Alarm: "+String(Cst4Header.nAlarmCfg_cnt));
//			for (int i=0; (i<Cst4Header.nAlarmCfg_cnt)&&(bOK)&&(!bAborted); i++)
//				{
//				VistaAlarmCfg* pMsg=new VistaAlarmCfg;
//				bool bValid=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
//				bool bValidTime=true;	//pMsg->Time_utc().bAllValid();  Ignore time
//				if (bValidTime && bRegenTimeSpan)
//					{
//					dT_utc_sec=pMsg->Time_utc().dTotalSec();
//					if (dT_utc_sec>dFNT_utc_sec)
//						{
//						if (dT_utc_sec<dMinT_utc_sec)
//							dMinT_utc_sec=dT_utc_sec;
//						if (dT_utc_sec>dMaxT_utc_sec)
//							dMaxT_utc_sec=dT_utc_sec;
//						}
//					}
//				if ((bValid)&&(bValidTime))
//					Alarm_Cfg.nAdd(pMsg);
//				else
//					{
//					if (++nErrorCnt>FATAL_ERROR_CNT)
//						bOK=false;
//					WORD w=pMsg->wSource();
//					String s=sGetCst4Site(w)+")";
//					if (!bValidTime)
//						s=s+" Invalid Time";
//					StoreLog(sLogFile,"   Error: Unpack (");
//					++nDiscMsgCnt;
////					bOK=false;
//					delete pMsg;
//					}
//				}
//			Alarm_Cfg.Pack();
//			}
//
//
//			//		if (nDiscMsgCnt>0)
////			{
////			ShowInfo(String (nDiscarded)+" Messages Discarded",sFile);
////			}
//		if (bRegenTimeSpan)
//      	{
//         Cst4Header.T1_utc=JTime(dMinT_utc_sec,true);
//         Cst4Header.T2_utc=JTime(dMaxT_utc_sec,true);
//         }
//		if (bOK)
//			{
//			StoreLog(sLogFile,"Closing OK "+sFile);
//			StoreLog(sLogFile,"");
//			}
//		else
//			{
//			StoreLog(sLogFile,"Closing Error "+sFile);
//			StoreLog(sLogFile,"");
//			}
//		return bOK &&(!bAborted);
//		}
//	else
//		{
//		StoreLog(sLogFile,"Closing Error "+sFile);
//			StoreLog(sLogFile,"");
//		return false;
//		}
//}



int Cst4DataFile::nConsolidateAISTargets()
{
	int nSources=0;
	for (int i=0; i<65536; i++)
		if (anSourceCnt[i]>0)
			++nSources;
	if ((nSources>1)&&(AIS_D.nGetCount()>0))
		{
		JList<MMSIStruct> MMSIList;
		Cst4_AIS_Dynamic* pD;
		int nMMSI;
		double dT_utc_sec;
		AIS_D.GoFirst();
		while(!AIS_D.bLast())
			{
			pD=AIS_D.pNext();
			nMMSI=pD->nGetMMSI();
			WORD wSource=pD->wSource();
			dT_utc_sec=pD->dTime_sec_utc();
			MMSIList.GoFirst();
			bool bFound=false;
			MMSIStruct* pMMSI;
			while(!MMSIList.bLast()&&(!bFound))
				{
				pMMSI=MMSIList.pNext();
				if (pMMSI->nMMSI==nMMSI)
					{
					bFound=true;
					++pMMSI->nCnt;
					pMMSI->abSource2[wSource]=true;
					if (dT_utc_sec<pMMSI->dT1_utc_sec)
						pMMSI->dT1_utc_sec=dT_utc_sec;
					if (dT_utc_sec>pMMSI->dT2_utc_sec)
						pMMSI->dT2_utc_sec=dT_utc_sec;
					}
				}
			if (!bFound)
				{
				pMMSI=new MMSIStruct;
				pMMSI->nMMSI=nMMSI;
				pMMSI->nCnt=1;
				memset(pMMSI->abSource2,false,sizeof(pMMSI->abSource2));
				pMMSI->abSource2[wSource]=true;
				pMMSI->dT1_utc_sec=dT_utc_sec;
				pMMSI->dT2_utc_sec=dT_utc_sec;
				MMSIList.nAdd(pMMSI);
				}
			}
		MMSIList.Pack();
		if (!AIS_D.bPacked())
			AIS_D.Pack();
		bool* pbValid=new bool[AIS_D.nGetCount()];
		memset(pbValid,true,AIS_D.nGetCount());
		int nDupCnt=0;
		for (int nM=0; nM<MMSIList.nGetCount(); nM++)
			{
			int nMMSISources=0;
			for (int i=0; i<65536; i++)
				if (MMSIList[nM]->abSource2[i])
					++nMMSISources;
			if (nMMSISources>1)
				{
				int nIndexSize=(MMSIList[nM]->dT2_utc_sec-MMSIList[nM]->dT1_utc_sec)*10.0+1.5;

				struct IndexStruct
				{
					int nIdx;
					WORD wSource;
				};

				IndexStruct* pIndex=new IndexStruct[nIndexSize];
				for (int i=0; i<nIndexSize; i++)
					pIndex[i].nIdx=-1;
				int nAISCnt=AIS_D.nGetCount();
				for (int nI=0; nI<nAISCnt; nI++)
					{
					if ((AIS_D[nI]->nGetMMSI()==nMMSI)&&(pbValid[nI]))
						{
						int nOffs=(AIS_D[nI]->dTime_sec_utc()-MMSIList[nM]->dT1_utc_sec)*10.0;
						if ((nOffs>=0)&&(nOffs<nIndexSize))
							{
							if (pIndex[nOffs].nIdx!=-1)
								{
								pbValid[nI]=false;
								++nDupCnt;
								}
							else
								{
								pIndex[nOffs].nIdx=nI;
								pIndex[nOffs].wSource=AIS_D[nI]->wSource();
								}
							}
						}
					}
				for (int i=0; i<nIndexSize; i++)
					{
					int nD=pIndex[i].nIdx;
					if (nD>=0)
						{
						if (pbValid[nD])
							{
							int n1=i-50;
							if (n1<0) n1=0;
							int n2=i+51;
							if (n2>nIndexSize) n2=nIndexSize;
							for (int j=n1; j<n2; j++)
								{
								int nC=pIndex[j].nIdx;
								if (nC>=0)
									{
									if ((pbValid[nC])&&(j!=i))
										{
										if (AIS_D[nD]->bDuplicate(AIS_D[nC],5.0))
											{
											pbValid[nC]=false;
											++nDupCnt;
											}
										}
									}
								}
							}
						}
					}
				delete[] pIndex;
				}
			}
		if (nDupCnt>0)
			{
//			int nAISCnt=AIS_D.nGetCount();
			JList<Cst4_AIS_Dynamic> AIS_Cons;
			AIS_Cons=AIS_D;
			AIS_D.Clear();
			memset(anSourceCnt,0,sizeof(anSourceCnt));
			Cst4Header.dwAIS_D_Size=0;
			int nAISCnt=AIS_Cons.nGetCount();
			AIS_Cons.Pack();
			for (int i=0; i<nAISCnt; i++)
				{
				if (pbValid[i])
					AddMessage(*AIS_Cons[i]);
				}
			AIS_D.Pack();
//         nAISCnt=AIS_D.nGetCount();
			}
		delete[] pbValid;
		return nDupCnt;
		}
	else
		return 0;
}

void Cst4DataFile::Append(Cst4DataFile& F, const DWORD dwT1_utc_sec, const DWORD dwT2_utc_sec)
{
	for (int i=0; i<F.nDynamicCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pDynamic(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.Dynamic(i));
		}
	for (int i=0; i<F.nStaticCnt(); i++)
		{
//		DWORD dwT_utc_sec=F.pStatic(i)->dwTime_sec_utc();
//		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.Static(i));
		}
	for (int i=0; i<F.nScanCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pScan(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.Scan(i));
		}
	for (int i=0; i<F.nTrackCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pTrack(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.Track(i));
		}
	for (int i=0; i<F.nIFFCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pIFF(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.IFF(i));
		}
	for (int i=0; i<F.nADSBCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pADSB(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.ADSB(i));
		}
	for (int i=0; i<F.nSysTrackCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pSysTrack(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.SysTrack(i));
		}
	for (int i=0; i<F.nUserCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pUser(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.User(i));
		}
	for (int i=0; i<F.nAlarmTCnt(); i++)
		{
		DWORD dwT_utc_sec=F.pAlarmT(i)->dwTime_sec_utc();
		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
			AddMessage(F.AlarmT(i));
		}
//	for (int i=0; i<F.nAlarmACnt(); i++)
//		{
//		DWORD dwT_utc_sec=F.pAlarmA(i)->dwTime_sec_utc();
//		if ((dwT_utc_sec>=dwT1_utc_sec)&&(dwT_utc_sec<=dwT2_utc_sec))
//			AddMessage(F.AlarmA(i));
//		}
}


String Cst4DataFile::sGetHeaderInfo()
{
	String s="";
	String sV;
	s=sV.sprintf  (L"Software Version: %d\r\n",Cst4Header.dwSWVersion);
	s=s+sV.sprintf(L"Data Version:     %d\r\n",Cst4Header.dwDataVersion);
	s=s+"Time From "+Cst4Header.T1_utc.sDateTime(false,false,true)+" to "+Cst4Header.T2_utc.sDateTime(false,false,true)+"\r\n";
	s=s+"Dynamic AIS Messages:  "+String(Cst4Header.nAIS_D_cnt)+"\r\n";
	s=s+"Static AIS Messages:   "+String(Cst4Header.nAIS_S_cnt)+"\r\n";
	s=s+"Radar Scan Messages:   "+String(Cst4Header.nRadar_S_cnt)+"\r\n";
	s=s+"Radar Track Messages:  "+String(Cst4Header.nRadar_T_cnt)+"\r\n";
	s=s+"IFF Track Messages:    "+String(Cst4Header.nIFF_T_cnt)+"\r\n";
	s=s+"ADS-B Track Messages:  "+String(Cst4Header.nADSB_T_cnt)+"\r\n";
	s=s+"System Track Messages: "+String(Cst4Header.nSysT_cnt)+"\r\n";
	s=s+"User Messages:         "+String(Cst4Header.nUser_cnt)+"\r\n";
	s=s+"Alarm Trigger Messages:"+String(Cst4Header.nAlarmT_cnt)+"\r\n";
//	s=s+"Alarm Ack Messages		:"+String(Cst4Header.nAlarmA_cnt)+"\r\n";
	s=s+"Alarm Cfg Messages		:"+String(Cst4Header.nAlarmCfg_cnt)+"\r\n";
	if (nDiscMsgCnt>0)
		s=s+"Discarded Messages:   "+String(nDiscMsgCnt);

	s=s+"\r\n";
	s=s+"Data Sources\r\n";

	int anCnt[65536];
	memset(anCnt,0,sizeof(anCnt));
	AIS_D.GoFirst();
	while(!AIS_D.bLast())
		++anCnt[AIS_D.pNext()->wSource()];
	AIS_S.GoFirst();
	while(!AIS_S.bLast())
		++anCnt[AIS_S.pNext()->wSource()];
	Radar_S.GoFirst();
	while(!Radar_S.bLast())
		++anCnt[Radar_S.pNext()->wSource()];
	Radar_T.GoFirst();
	while(!Radar_T.bLast())
		++anCnt[Radar_T.pNext()->wSource()];
	IFF_T.GoFirst();
	while(!IFF_T.bLast())
		++anCnt[IFF_T.pNext()->wSource()];
	while(!ADSB_T.bLast())
		++anCnt[ADSB_T.pNext()->wSource()];
	User_M.GoFirst();
	while(!User_M.bLast())
		++anCnt[User_M.pNext()->wSource()];
	Alarm_T.GoFirst();
	while(!Alarm_T.bLast())
		++anCnt[Alarm_T.pNext()->wSource()];
//	Alarm_A.GoFirst();
//	while(!Alarm_A.bLast())
//		++anCnt[Alarm_A.pNext()->wSource()];
	for (int i=0; i<65536; i++)
		{
		if (anCnt[i]>0)
			s=s+"  "+sGetCst4Site(i)+": "+String(anCnt[i])+" messages\r\n";
		}
	return s;
}

void Cst4DataFile::ExtractText(const String sFile)
{
	JFile F('O',JFile::ASCII_TYPE);
	JFile::FILE_ERROR E=F.Create(sFile);
	for (int i=0; i<nDynamicCnt() && !E; i++)
		E=pDynamic(i)->Extract(&F);
	for (int i=0; i<nStaticCnt() && !E; i++)
		E=pStatic(i)->Extract(&F);
	for (int i=0; i<nScanCnt() && !E; i++)
		E=pScan(i)->Extract(&F);
	for (int i=0; i<nTrackCnt() && !E; i++)
		E=pTrack(i)->Extract(&F);
	for (int i=0; i<nIFFCnt() && !E; i++)
		E=pIFF(i)->Extract(&F);
	for (int i=0; i<nADSBCnt() && !E; i++)
		E=pADSB(i)->Extract(&F);
	for (int i=0; i<nSysTrackCnt() && !E; i++)
		E=pSysTrack(i)->Extract(&F);
	for (int i=0; i<nUserCnt() && !E; i++)
		E=pUser(i)->Extract(&F);
	for (int i=0; i<nAlarmTCnt() && !E; i++)
		E=pAlarmT(i)->Extract(&F);
	F.ShowError(E);
	for (int i=0; i<nAlarmCfgCnt() && !E; i++)
		E=pAlarmCfg(i)->Extract(&F);
	F.ShowError(E);
	if (!E)
		ShowInfo("Messages Extracted to "+sFile,"Extract Messages");


}

