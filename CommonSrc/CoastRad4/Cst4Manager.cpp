//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Manager.h"
#include "JFile.h"
#include "JUtils.h"
#include "JMemFile.h"
#include "Cst4UserDefs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


Cst4Manager::Cst4Manager()
{
//	pDebugFil=NULL;
//   pDebugFil=new JFile('O',JFile::ASCII_TYPE);
//  	pDebugFil->Create("VistaHub Debug "+JTime(true).sFileName("csv"));

//	pTestFil=NULL;
//	if (!sTestName.IsEmpty())
//		{
//		pTestFil=new JFile('O',JFile::ASCII_TYPE);
//		pTestFil->Create(sTestName+"_"+JTime(true).sFileName("txt"));
//		}
//	pucRXBuf=new BYTE[MAX_RX_SIZE+4];
	nErrorCnt=0;
Init(false);
}

Cst4Manager::~Cst4Manager()
{
//   delete pTestFil;
//	delete[] pucRXBuf;
//   pucRXBuf=NULL;
//   delete pDebugFil;
}

void Cst4Manager::ClearMsg()
{
	Header.Init();
	AIS_D.Clear();
	AIS_S.Clear();
	Radar_S.Clear();
	Radar_T.Clear();
	IFF_T.Clear();
	ADSB_T.Clear();
	Sys_T.Clear();
	User.Clear();
	AlarmCfgList.Clear();
	AlarmTrigList.Clear();
}

void Cst4Manager::Init(const bool _bSaveMessages)
{
	ClearMsg();
	nRXOffs=-4;
	nRXSize=0;
//	nKeepAliveCnt=0;
	TimeMsg.Invalidate();
	bSaveMessages=_bSaveMessages;
}

CST4_MSG_TYPE Cst4Manager::AddRXByte(const BYTE uc)
{
//   if (pTestFil)
//      {
//      static int nIndex=0;
//      String s;
//      if (uc>=32)
//         s.printf(L"%5d %4d %c  %02X",nIndex++,nRXOffs,uc,uc);
//      else
//         s.printf(L"%5d %4d    %02X",nIndex++,nRXOffs,uc);
//      pTestFil->TextLine(s);
//      }

	CST4_MSG_TYPE RXType;
	switch(nRXOffs)
		{
		case -4:
			if (uc=='C') nRXOffs=-3;   break;
		case -3:
			if (uc=='S') nRXOffs=-2;   break;
		case -2:
			if (uc=='T') nRXOffs=-1;   break;
		case -1:
			if (uc=='4') nRXOffs=0;     break;
		case 0:
			aucRXStatus[nRXOffs++]=uc; break;
		case 1:
			aucRXStatus[nRXOffs++]=uc; break;
		case 2:
			aucRXStatus[nRXOffs++]=uc; break;
		case 3:
			aucRXStatus[nRXOffs++]=uc;
			memcpy(&dwRXStatus,aucRXStatus,4);
			RXType=(CST4_MSG_TYPE)(dwRXStatus&Cst4_MessageBase::CST4_TYPE_MASK);
			if ((RXType>CST4_NO_MSG)&&(RXType<CST4_MSG_CNT))
				{
				nRXSize=(dwRXStatus&Cst4_MessageBase::CST4_STAT_SIZE_MASK)>>Cst4_MessageBase::CST4_STAT_SIZE_OFFS;
//				if (pTestFil)
//					pTestFil->TextLine("RX Size = "+String(nRXSize));
				if (nRXSize>0)
					{
					memcpy(aucRXBuf,&dwRXStatus,4);
					}
				else
					{
					nRXOffs=-4;
					++nErrorCnt;
					}
				}
			else
				{
				nRXOffs=-4;
				++nErrorCnt;
				}
			break;
		default:
			if (nRXOffs<MAX_RX_SIZE)
				aucRXBuf[nRXOffs++]=uc;
			else
				{
				//Max buffer size exceeded - skip message
				nRXOffs=-4;
				}
			if (nRXOffs>=nRXSize)
				{
				nRXOffs=-4;
//				if (pTestFil)
//					pTestFil->TextLine("** RX Finished **");
				return AddPackedMessage(aucRXBuf,nRXSize);
				}
		}
	return CST4_NO_MSG;
}

void Cst4Manager::AddMessage(const Cst4_AIS_Dynamic& Msg)
{
	AIS_D.nAdd(new Cst4_AIS_Dynamic(Msg));
	Header.dwAIS_D_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_AIS_Static& Msg)
{
	AIS_S.nAdd(new Cst4_AIS_Static(Msg));
	Header.dwAIS_S_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_Radar_Scan& Msg)
{
	Radar_S.nAdd(new Cst4_Radar_Scan(Msg));
	Header.dwRadar_S_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_Radar_Track& Msg)
{
	Radar_T.nAdd(new Cst4_Radar_Track(Msg));
	Header.dwRadar_T_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_IFF_Track& Msg)
{
	IFF_T.nAdd(new Cst4_IFF_Track(Msg));
	Header.dwIFF_T_Size1+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_ADSB& Msg)
{
	ADSB_T.nAdd(new Cst4_ADSB(Msg));
	Header.dwADSB_T_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_SysTrack& Msg)
{
	Sys_T.nAdd(new Cst4_SysTrack(Msg));
	Header.dwSysTrack_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const Cst4_User_Msg& Msg)
{
	User.nAdd(new Cst4_User_Msg(Msg));
	Header.dwUser_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const VistaAlarmCfg& Msg)
{
	AlarmCfgList.nAdd(new VistaAlarmCfg(Msg));
	Header.dwAlarmCfg_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

void Cst4Manager::AddMessage(const VistaAlarmTrigger& Msg)
{
	AlarmTrigList.nAdd(new VistaAlarmTrigger(Msg));
	Header.dwAlarmTrig_Size+=Msg.dwSize();
	Header.T2_utc=Msg.Time_utc();
	if (!Header.T1_utc.bValid())
		Header.T1_utc=Header.T2_utc;
}

//void Cst4Manager::AddMessage(const VistaAlarmAck& Msg)
//{
//	AlarmAckList.nAdd(new VistaAlarmAck(Msg));
//	Header.dwAlarmAck_Size+=Msg.dwSize();
//	Header.T2_utc=Msg.Time_utc();
//	if (!Header.T1_utc.bValid())
//		Header.T1_utc=Header.T2_utc;
//}


CST4_MSG_TYPE Cst4Manager::AddPackedMessage(const BYTE* pucBuf, const int nMaxSize)
{
	DWORD dwStatus;
	memcpy(&dwStatus,pucBuf,4);

   String s;
   s.printf(L"%6d,",nTXNum);

	Cst4_AIS_Dynamic* pT;

	CST4_MSG_TYPE MsgType=(CST4_MSG_TYPE)(dwStatus&Cst4_MessageBase::CST4_TYPE_MASK);
	DWORD dwSize=(dwStatus&Cst4_MessageBase::CST4_STAT_SIZE_MASK)>>Cst4_MessageBase::CST4_STAT_SIZE_OFFS;
	switch(MsgType)
		{
		case CST4_DYNAMIC_AIS_MSG:
//         if (pDebugFil)
//         	{
//            pDebugFil->TextLine(s+"AIS D");
//            }
			if (bSaveMessages)
				{
				AIS_D.nAdd(new Cst4_AIS_Dynamic(pucBuf,nMaxSize));
				Header.T2_utc=AIS_D.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwAIS_D_Size+=dwSize;
				}
			else
				AIS_Dynamic.Assign(Cst4_AIS_Dynamic(pucBuf,nMaxSize));
			break;
		case CST4_STATIC_AIS_MSG:
//         if (pDebugFil)
//         	pDebugFil->TextLine(s+"AIS S");
			if (bSaveMessages)
				{
				AIS_S.nAdd(new Cst4_AIS_Static(pucBuf,nMaxSize));
				Header.T2_utc=AIS_S.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwAIS_S_Size+=dwSize;
				}
			else
				AIS_Static.Assign(Cst4_AIS_Static(pucBuf,nMaxSize));
			break;
		case CST4_RADAR_SCAN_MSG:
//         if (pDebugFil)
//            pDebugFil->TextLine(s+"Radar S");
			if (bSaveMessages)
				{
				Radar_S.nAdd(new Cst4_Radar_Scan(pucBuf,nMaxSize));
				Header.T2_utc=Radar_S.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwRadar_S_Size+=dwSize;
				}
			else
				{
				Cst4_Radar_Scan S(pucBuf,nMaxSize);
				Radar_Scan.Assign(S);
				}
			break;
		case CST4_RADAR_TRACK_MSG:
//if (pDebugFil)
//            pDebugFil->TextLine(s+"Radar T");
			if (bSaveMessages)
				{
				Radar_T.nAdd(new Cst4_Radar_Track(pucBuf,nMaxSize));
				Header.T2_utc=Radar_T.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwRadar_T_Size+=dwSize;
				}
			else
				{
				Cst4_Radar_Track T(pucBuf,nMaxSize);
				Radar_Track.Assign(T);
				}
			break;
		case CST4_IFF_TRACK_MSG:
//			if (pDebugFil)
//				pDebugFil->TextLine(s+"IFF");
			if (bSaveMessages)
				{
				IFF_T.nAdd(new Cst4_IFF_Track(pucBuf,nMaxSize));
				Header.T2_utc=IFF_T.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwIFF_T_Size1+=dwSize;
				}
			else
				{
				Cst4_IFF_Track T(pucBuf,nMaxSize);
				IFF_Track.Assign(T);
				}
			break;
		case VISTA_ADSB_MSG:
//			if (pDebugFil)
//				pDebugFil->TextLine(s+"ADSB");
			if (bSaveMessages)
				{
				ADSB_T.nAdd(new Cst4_ADSB(pucBuf,nMaxSize));
				Header.T2_utc=ADSB_T.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwADSB_T_Size+=dwSize;
				}
			else
				{
				Cst4_ADSB T(pucBuf,nMaxSize);
				ADSB_Track.Assign(T);
				}
			break;
		case CST4_SYSTRACK_MSG:
//			if (pDebugFil)
//				pDebugFil->TextLine(s+"SysTrack");
			if (bSaveMessages)
				{
				Sys_T.nAdd(new Cst4_SysTrack(pucBuf,nMaxSize));
				Header.T2_utc=Sys_T.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwSysTrack_Size+=dwSize;
				}
			else
				{
				Cst4_SysTrack T(pucBuf,nMaxSize);
				SysTrack.Assign(T);
				}
			break;
		case CST4_USER_MSG:
			if (bSaveMessages)
				{
				User.nAdd(new Cst4_User_Msg(pucBuf,nMaxSize));
				Header.T2_utc=User.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwUser_Size+=dwSize;
				}
			else
				{
				Cst4_User_Msg U(pucBuf,nMaxSize);
				User_Msg.Assign(U);
				}
			break;
		case VISTA_ALARM_CFG_MSG:
			if (bSaveMessages)
				{
				AlarmCfgList.nAdd(new VistaAlarmCfg(pucBuf,nMaxSize));
				Header.T2_utc=AlarmCfgList.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwAlarmCfg_Size+=dwSize;
				}
			else
				{
				VistaAlarmCfg V(pucBuf,nMaxSize);
				AlarmCfg.Assign(V);
				}
			break;
		case VISTA_ALARM_TRIGGER_MSG:
			if (bSaveMessages)
				{
				AlarmTrigList.nAdd(new VistaAlarmTrigger(pucBuf,nMaxSize));
				Header.T2_utc=AlarmTrigList.pGetCurrent()->Time_utc();
				if (!Header.T1_utc.bValid())
					Header.T1_utc=Header.T2_utc;
				Header.dwAlarmTrig_Size+=dwSize;
				}
			else
				{
				VistaAlarmTrigger V(pucBuf,nMaxSize);
				AlarmTrig.Assign(V);
				}
			break;
		case CST4_REQUEST_MSG:
			Request=Cst4_Request(pucBuf,nMaxSize);
			break;
		case CST4_RESPONSE_MSG:
			Response=Cst4_Response(pucBuf,nMaxSize);
			break;
		case VISTA_TIME_MSG:
			TimeMsg=VistaTimeMessage(pucBuf,nMaxSize);
			break;
		case CST4_BITE_MSG:
			BITEInfo=Cst4_BITE_Info(pucBuf,nMaxSize);
			break;
//		case CST4_KEEPALIVE_MSG:
//			++nKeepAliveCnt;
//			break;
		default:
      	++nErrorCnt;
		}
	return MsgType;
}


bool Cst4Manager::bSaveToFile(const String sFile)
{
	Header.dwDataVersion=CST4_DATA_VERSION;
	Header.dwSWVersion=dwGetExeVersion();;
	Header.nAIS_D_cnt=AIS_D.nGetCount();
	Header.nAIS_S_cnt=AIS_S.nGetCount();
	Header.nRadar_S_cnt=Radar_S.nGetCount();
	Header.nRadar_T_cnt=Radar_T.nGetCount();
	Header.nIFF_T_cnt1=IFF_T.nGetCount();
	Header.nADSB_T_cnt=ADSB_T.nGetCount();
	Header.nSysTrack_cnt=Sys_T.nGetCount();
	Header.nUser_cnt=User.nGetCount();
	Header.nAlarmCfg_cnt=AlarmCfgList.nGetCount();
	Header.nAlarmTrig_cnt=AlarmTrigList.nGetCount();
	JFile Fil('O');
   JFile::FILE_ERROR E=Fil.Create(sFile);
   if (!E)
      E=Fil.Write(&Header,sizeof(Header));
   int nOffs;
   BYTE* pucBuf;
   if ((!E)&&(Header.dwAIS_D_Size>0))
      {
      AIS_D.Pack();
      pucBuf=new BYTE[Header.dwAIS_D_Size];
      nOffs=0;
      for (int i=0; i<AIS_D.nGetCount(); i++)
         AIS_D[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   if ((!E)&&(Header.dwAIS_S_Size>0))
      {
      AIS_S.Pack();
      pucBuf=new BYTE[Header.dwAIS_S_Size];
      nOffs=0;
      for (int i=0; i<AIS_S.nGetCount(); i++)
         AIS_S[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   if ((!E)&&(Header.dwRadar_S_Size>0))
      {
      Radar_S.Pack();
      pucBuf=new BYTE[Header.dwRadar_S_Size];
      nOffs=0;
      for (int i=0; i<Radar_S.nGetCount(); i++)
         Radar_S[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   if ((!E)&&(Header.dwRadar_T_Size>0))
      {
      Radar_T.Pack();
      pucBuf=new BYTE[Header.dwRadar_T_Size];
      nOffs=0;
      for (int i=0; i<Radar_T.nGetCount(); i++)
         Radar_T[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   if ((!E)&&(Header.dwIFF_T_Size1>0))
      {
      IFF_T.Pack();
      pucBuf=new BYTE[Header.dwIFF_T_Size1];
      nOffs=0;
      for (int i=0; i<IFF_T.nGetCount(); i++)
         IFF_T[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
		}
	if ((!E)&&(Header.dwADSB_T_Size>0))
		{
		ADSB_T.Pack();
		pucBuf=new BYTE[Header.dwADSB_T_Size];
		nOffs=0;
		for (int i=0; i<ADSB_T.nGetCount(); i++)
			ADSB_T[i]->Pack(pucBuf,nOffs);
		E=Fil.Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	if ((!E)&&(Header.dwSysTrack_Size>0))
		{
		Sys_T.Pack();
		pucBuf=new BYTE[Header.dwSysTrack_Size];
		nOffs=0;
		for (int i=0; i<Sys_T.nGetCount(); i++)
			Sys_T[i]->Pack(pucBuf,nOffs);
		E=Fil.Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	if ((!E)&&(Header.dwUser_Size>0))
      {
      User.Pack();
      pucBuf=new BYTE[Header.dwUser_Size];
      nOffs=0;
      for (int i=0; i<User.nGetCount(); i++)
         User[i]->Pack(pucBuf,nOffs);
      E=Fil.Write(pucBuf,nOffs);
      delete[] pucBuf;
      }
   if ((!E)&&(Header.dwAlarmCfg_Size>0))
      {
		AlarmCfgList.Pack();
		pucBuf=new BYTE[Header.dwAlarmCfg_Size];
		nOffs=0;
		for (int i=0; i<AlarmCfgList.nGetCount(); i++)
			AlarmCfgList[i]->Pack(pucBuf,nOffs);
		E=Fil.Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	if ((!E)&&(Header.dwAlarmTrig_Size>0))
		{
		AlarmTrigList.Pack();
		pucBuf=new BYTE[Header.dwAlarmTrig_Size];
		nOffs=0;
		for (int i=0; i<AlarmTrigList.nGetCount(); i++)
			AlarmTrigList[i]->Pack(pucBuf,nOffs);
		E=Fil.Write(pucBuf,nOffs);
		delete[] pucBuf;
		}
	if (!E)
		E=Fil.Close();
	return (E==JFile::F_NO_ERROR);
}

bool Cst4Manager::bReadFromFile(const String sFile)
{
	JMemFile Fil;
	JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
	if (!E)
		{
		Init(true);
		bool bOK=Fil.bRead(&Header,sizeof(FileHeaderStruct));
		BYTE* pucBuf=Fil.pucGetBuffer();
		int nBufSize=(int)Fil.dwGetSize();
		int nOffs=sizeof(Header);
		if ((bOK)&&(Header.nAIS_D_cnt>0))
			{
			for (int i=0; (i<Header.nAIS_D_cnt)&&(bOK); i++)
				{
				Cst4_AIS_Dynamic* pMsg=new Cst4_AIS_Dynamic;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					AIS_D.nAdd(pMsg);
				}
			AIS_D.Pack();
			}
		if ((bOK)&&(Header.nAIS_S_cnt>0))
			{
			for (int i=0; (i<Header.nAIS_S_cnt)&&(bOK); i++)
				{
				Cst4_AIS_Static* pMsg=new Cst4_AIS_Static;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					AIS_S.nAdd(pMsg);
				}
			AIS_S.Pack();
			}
		if ((bOK)&&(Header.nRadar_S_cnt>0))
			{
			for (int i=0; (i<Header.nRadar_S_cnt)&&(bOK); i++)
				{
				Cst4_Radar_Scan* pMsg=new Cst4_Radar_Scan;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					Radar_S.nAdd(pMsg);
				}
			Radar_S.Pack();
			}
		if ((bOK)&&(Header.nRadar_T_cnt>0))
			{
			for (int i=0; (i<Header.nRadar_T_cnt)&&(bOK); i++)
				{
				Cst4_Radar_Track* pMsg=new Cst4_Radar_Track;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					Radar_T.nAdd(pMsg);
				}
         Radar_T.Pack();
         }
		if ((bOK)&&(Header.nIFF_T_cnt1>0))
			{
			for (int i=0; (i<Header.nIFF_T_cnt1)&&(bOK); i++)
				{
				Cst4_IFF_Track* pMsg=new Cst4_IFF_Track;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					IFF_T.nAdd(pMsg);
				}
			IFF_T.Pack();
			}
		if ((bOK)&&(Header.nADSB_T_cnt>0))
			{
			for (int i=0; (i<Header.nADSB_T_cnt)&&(bOK); i++)
				{
				Cst4_ADSB* pMsg=new Cst4_ADSB;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					ADSB_T.nAdd(pMsg);
				}
			ADSB_T.Pack();
			}
		if ((bOK)&&(Header.nSysTrack_cnt>0))
			{
			for (int i=0; (i<Header.nSysTrack_cnt)&&(bOK); i++)
				{
				Cst4_SysTrack* pMsg=new Cst4_SysTrack;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					Sys_T.nAdd(pMsg);
				}
			Sys_T.Pack();
			}
      if ((bOK)&&(Header.nUser_cnt>0))
         {
         for (int i=0; (i<Header.nUser_cnt)&&(bOK); i++)
            {
            Cst4_User_Msg* pMsg=new Cst4_User_Msg;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					User.nAdd(pMsg);
				}
			User.Pack();
			}
		if ((bOK)&&(Header.nAlarmCfg_cnt>0))
			{
			for (int i=0; (i<Header.nAlarmCfg_cnt)&&(bOK); i++)
				{
				VistaAlarmCfg* pMsg=new VistaAlarmCfg;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					AlarmCfgList.nAdd(pMsg);
				}
			AlarmCfgList.Pack();
			}
		if ((bOK)&&(Header.nAlarmTrig_cnt>0))
			{
			for (int i=0; (i<Header.nAlarmTrig_cnt)&&(bOK); i++)
				{
				VistaAlarmTrigger* pMsg=new VistaAlarmTrigger;
				bOK=pMsg->bUnpack(pucBuf,nOffs,nBufSize);
				if (bOK)
					AlarmTrigList.nAdd(pMsg);
				}
			AlarmTrigList.Pack();
			}
		return bOK;
		}
	else
		return false;
}

int Cst4Manager::nGetMsgCnt(const CST4_MSG_TYPE MsgType) const
{
	switch(MsgType)
		{
		case CST4_DYNAMIC_AIS_MSG: 	return AIS_D.nGetCount();
		case CST4_STATIC_AIS_MSG:  	return AIS_S.nGetCount();
		case CST4_RADAR_SCAN_MSG:  	return Radar_S.nGetCount();
		case CST4_RADAR_TRACK_MSG: 	return Radar_T.nGetCount();
		case CST4_IFF_TRACK_MSG:   	return IFF_T.nGetCount();
		case VISTA_ADSB_MSG:   			return ADSB_T.nGetCount();
		case CST4_SYSTRACK_MSG:    	return Sys_T.nGetCount();
		case CST4_USER_MSG:        	return User.nGetCount();
		case VISTA_ALARM_CFG_MSG:  	return AlarmCfgList.nGetCount();
		case VISTA_ALARM_TRIGGER_MSG:	return AlarmTrigList.nGetCount();
		default: 							return 0;
		}
}

int Cst4Manager::nGetTXSize()
{
	int nTXSize=0;
   AIS_D.GoFirst();
   while(!AIS_D.bLast())
      nTXSize+=(int)AIS_D.pNext()->dwCalcSize()+4;
   AIS_S.GoFirst();
	while(!AIS_S.bLast())
      nTXSize+=(int)AIS_S.pNext()->dwCalcSize()+4;
   Radar_S.GoFirst();
   while(!Radar_S.bLast())
      nTXSize+=(int)Radar_S.pNext()->dwCalcSize()+4;
   Radar_T.GoFirst();
   while(!Radar_T.bLast())
      nTXSize+=(int)Radar_T.pNext()->dwCalcSize()+4;
   IFF_T.GoFirst();
   while(!IFF_T.bLast())
      nTXSize+=(int)IFF_T.pNext()->dwCalcSize()+4;
	ADSB_T.GoFirst();
	while(!ADSB_T.bLast())
      nTXSize+=(int)ADSB_T.pNext()->dwCalcSize()+4;
	Sys_T.GoFirst();
	while(!Sys_T.bLast())
		nTXSize+=(int)Sys_T.pNext()->dwCalcSize()+4;
	User.GoFirst();
	while(!User.bLast())
		nTXSize+=(int)User.pNext()->dwCalcSize()+4;

	AlarmCfgList.GoFirst();
	while(!AlarmCfgList.bLast())
		nTXSize+=(int)AlarmCfgList.pNext()->dwCalcSize()+4;

	AlarmTrigList.GoFirst();
	while(!AlarmTrigList.bLast())
		nTXSize+=(int)AlarmTrigList.pNext()->dwCalcSize()+4;

	return nTXSize;
}

void Cst4Manager::PackTX(BYTE* pucTXBuf)
{
	int nOffs=0;

	AIS_D.GoFirst();
	while(!AIS_D.bLast())
		nOffs+=AIS_D.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	AIS_S.GoFirst();
	while(!AIS_S.bLast())
		nOffs+=AIS_S.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	Radar_S.GoFirst();
	while(!Radar_S.bLast())
		nOffs+=Radar_S.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	Radar_T.GoFirst();
	while(!Radar_T.bLast())
		nOffs+=Radar_T.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	IFF_T.GoFirst();
	while(!IFF_T.bLast())
		nOffs+=IFF_T.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	ADSB_T.GoFirst();
	while(!ADSB_T.bLast())
		nOffs+=ADSB_T.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	Sys_T.GoFirst();
	while(!Sys_T.bLast())
		nOffs+=Sys_T.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	User.GoFirst();
	while(!User.bLast())
		nOffs+=User.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	AlarmCfgList.GoFirst();
	while(!AlarmCfgList.bLast())
		nOffs+=AlarmCfgList.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

	AlarmTrigList.GoFirst();
	while(!AlarmTrigList.bLast())
		nOffs+=AlarmTrigList.pNext()->nCreateTXBuf(&pucTXBuf[nOffs]);

}

