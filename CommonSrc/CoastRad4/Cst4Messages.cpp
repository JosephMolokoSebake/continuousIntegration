//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Messages.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JFile::FILE_ERROR Cst4_MessageBase::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=pF->TextLine("");
	if (!E) E=pF->TextLine("**********************************");
	String s;
	if (!E) E=pF->TextLine(sGetMessageDescription((CST4_MSG_TYPE)(dwStatus&CST4_TYPE_MASK)));
	if (g_pSiteDefs)
		s=g_pSiteDefs->sGetDescrFromID(wSource());
	else
		s.printf(L"0x%04X",wSource());
	if (!E) E=pF->TextLine("   Source: "+s);
	JTime T=Time_utc();
	if (T.bAllValid())
		s="   Time: "+T.sDateTime(false,false,true);
	else
		s="   Time: INVALID";
	if (!E) E=pF->TextLine(s);
	s.printf(L"%d",dwSize());
	if (!E) E=pF->TextLine("   Message Size: "+s+" bytes");
	return E;
}

String Cst4_MessageBase::sExtract()
{
	String s="\n\r**********************************\n\r";
   String sV;
	s=s+sGetMessageDescription((CST4_MSG_TYPE)(dwStatus&CST4_TYPE_MASK))+"\n\r";
	if (g_pSiteDefs)
		sV=g_pSiteDefs->sGetDescrFromID(wSource());
	else
		sV.printf(L"0x%04X",wSource());
	s=s+"   Source: "+sV+"\n\r";
	JTime T=Time_utc();
	if (T.bAllValid())
		sV="   Time: "+T.sDateTime(false,false,true);
	else
		sV="   Time: INVALID";
	s=s+sV+"\n\r";
	sV.printf(L"%d",dwSize());
	s=s+"   Message Size: "+sV+" bytes"+"\n\r";
	return s;
}

JFile::FILE_ERROR Cst4_AIS_Dynamic::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	String s;

	if (!E)
		{
		if (bSat_Source())
			E=pF->TextLine("Satellite Data");
		else
			E=pF->TextLine("AIS Site Data");
		}

	s.printf(L"   MMSI: %d",nGetMMSI());
	if (!E) E=pF->TextLine(s);
	if (bSpeedCourseValid())
		{
		s.printf(L"   Speed: %1.1f kts",dSpeed_kts());
		if (!E) E=pF->TextLine(s);
		s.printf(L"   Course: %1.1f deg",dCourse_deg());
		if (!E) E=pF->TextLine(s);
		}
	else
		{
		if (!E) E=pF->TextLine("   Speed and Course Invalid");
		}
	LatLong LL=GetLL();
	if (LL.bValid())
		{
		if (!E) E=pF->TextLine("   Position: "+LL.sLL());
		}
	else
		{
		if (!E) E=pF->TextLine("   Position Invalid");
		}
	return E;
}


String Cst4_AIS_Dynamic::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	String sV;
	sV.printf(L"   MMSI: %d\n\r",nGetMMSI());
	s=s+sV;
	if (bSpeedCourseValid())
		{
		sV.printf(L"   Speed: %1.1f kts\n\r",dSpeed_kts());
		s=s+sV;
		sV.printf(L"   Course: %1.1f deg\n\r",dCourse_deg());
		s=s+sV;
		}
	else
		s=s+"   Speed and Course Invalid\n\r";
	LatLong LL=GetLL();
	if (LL.bValid())
		s=s+"   Position: "+LL.sLL()+"\n\r";
	else
		s=s+"   Position Invalid\n\r";
	return s;
}


JFile::FILE_ERROR Cst4_AIS_Static::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	String s;
	s.printf(L"   MMSI: %d",nGetMMSI());
	if (!E) E=pF->TextLine(s);
	int nIMO;
	String sCallsign;
	String sName;
	String sType;
	String sDest;

//	String sName=sBasicInfo(nType,nIMO,sCallsign);

	AISMsgClass AISMsg;

	AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
	if (AISMsg.bIMOValid())
		nIMO=AISMsg.nGetIMO();
	else
		nIMO=-1;
	if (AISMsg.bCallSignValid())
		sCallsign=AISMsg.sGetCallSign();
	else
		sCallsign="N/A";
	if (AISMsg.bShipNameValid())
		sName=AISMsg.sGetShipName();
	else
		sName="N/A";
	if (AISMsg.bShipAndCargoTypeValid())
		sType=AISMsg.sGetShipAndCargoType();
	else
		sType="N/A";
  if (AISMsg.bDestinationValid())
		sDest=AISMsg.sGetDestination();
  else
		sDest="N/A";

	if (!E) E=pF->TextLine("   IMO: "+String(nIMO));
	if (!E) E=pF->TextLine("   Name: "+sName);
	if (!E) E=pF->TextLine("   Callsign: "+sCallsign);
	if (!E) E=pF->TextLine("   Type: "+sType);
	if (!E) E=pF->TextLine("   Destination: "+sDest);

	return E;
}

String Cst4_AIS_Static::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	String sV;
	sV.printf(L"   MMSI: %d\n\r",nGetMMSI());
   s=s+sV;
	int nIMO;
	String sCallsign;
	String sName;
	String sType;
	String sDest;
	AISMsgClass AISMsg;
	AISMsg.SetExtraInfoBuf(pucStaticData,wStaticSize);
	if (AISMsg.bIMOValid())
		nIMO=AISMsg.nGetIMO();
	else
		nIMO=-1;
	if (AISMsg.bCallSignValid())
		sCallsign=AISMsg.sGetCallSign();
	else
		sCallsign="N/A";
	if (AISMsg.bShipNameValid())
		sName=AISMsg.sGetShipName();
	else
		sName="N/A";
	if (AISMsg.bShipAndCargoTypeValid())
		sType=AISMsg.sGetShipAndCargoType();
	else
		sType="N/A";
  if (AISMsg.bDestinationValid())
		sDest=AISMsg.sGetDestination();
  else
		sDest="N/A";

	s=s+"   IMO: "+String(nIMO)+"\n\r";
	s=s+"   Name: "+sName+"\n\r";
	s=s+"   Callsign: "+sCallsign+"\n\r";
	s=s+"   Type: "+sType+"\n\r";
	s=s+"   Destination: "+sDest+"\n\r";

	return s;

}


JFile::FILE_ERROR Cst4_Radar_Scan::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	if (bValid())
		{
		LatLong LL=GetRadarLL();
		if (LL.bValid())
			{
			if (!E) E=pF->TextLine("   Radar Position: "+LL.sLL());
			}
		else
			{
			if (!E) E=pF->TextLine("   Radar Position Invalid");
			}
		if (!E) E=pF->TextLine("   Number of Plots: "+String(nGetPlotCnt()));
		Cst4Plot* p;
		String s;
		Plot.GoFirst();
		int nP=0;
		while(!Plot.bLast() && !E)
			{
			p=Plot.pNext();
			s.printf(L"     #%4d  %6.0f  %6.2f",++nP,p->dRa_m(),p->dBe_deg());
			E=pF->TextLine(s);
			}
		}
	else
		{
		if (!E) E=pF->TextLine("   Invalid Scan");
		}
	return E;
}

String Cst4_Radar_Scan::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	String sV;
	if (bValid())
		{
		LatLong LL=GetRadarLL();
		if (LL.bValid())
			s=s+("   Radar Position: "+LL.sLL())+"\n\r";
		else
			s=s+"   Radar Position Invalid\n\r";
		s=s+"   Number of Plots: "+String(nGetPlotCnt())+"\n\r";
		Cst4Plot* p;
		String s;
		Plot.GoFirst();
		int nP=0;
		while(!Plot.bLast())
			{
			p=Plot.pNext();
			sV.printf(L"     #%4d  %6.0f  %6.2f\n\r",++nP,p->dRa_m(),p->dBe_deg());
			}
		}
	else
		s=s+"   Invalid Scan\n\r";
	return s;
}

JFile::FILE_ERROR Cst4_Radar_Track::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	if (bValid())
		{
      String s;
      s.printf(L"  Raw Track ID: %d  (%08XH)",dwRawID(),dwRawID());
      if (!E) E=pF->TextLine(s);
		if (bCyclope())
			{
			if (!E) E=pF->TextLine("   System Type: CYCLOPE");
			}
		else if (bSAR())
			{
			if (!E) E=pF->TextLine("   System Type: SAR");
			}
		else
			{
			if (!E) E=pF->TextLine("   System Type: RADAR");
			if (!E) E=pF->TextLine("   Track Number: "+String(nID()));
			if (!E) E=pF->TextLine("   Track Quality: "+String(ucQuality()));
			}
		LatLong LL=GetLL();
		if (LL.bValid())
			{
			if (!E) E=pF->TextLine("   Position: "+LL.sLL());
			}
		else
			{
			if (!E) E=pF->TextLine("   Position Invalid");
			}
		s.printf(L"   Speed: %1.1f kts",dSpeed_kts());
		if (!E) E=pF->TextLine(s);
		s.printf(L"   Course: %1.1f deg",dCourse_deg());
		if (!E) E=pF->TextLine(s);
		}
	else
		{
		if (!E) E=pF->TextLine("   Invalid Track");
		}
	return E;
}

String Cst4_Radar_Track::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	String sV;
	if (bValid())
		{
      sV.printf(L"   Raw Track ID: %d  %08X",nID(),nID());
      s=s+sV+"\n\r";
		if (bCyclope())
			s=s+"   System Type: CYCLOPE";
		else if (bSAR())
			s=s+"   System Type: SAR";
		else
			{
			s=s+"   System Type: RADAR";
			s=s+"   Track Number: "+String(nID())+"\n\r";
			s=s+"   Track Quality: "+String(ucQuality())+"\n\r";
			}
		LatLong LL=GetLL();
		if (LL.bValid())
			s=s+"   Position: "+LL.sLL()+"\n\r";
		else
			s=s+"   Position Invalid"+"\n\r";
		sV.printf(L"   Speed: %1.1f kts\n\r",dSpeed_kts());
		s=s+sV;
		sV.printf(L"   Course: %1.1f deg\n\r",dCourse_deg());
		s=s+sV;
		}
	else
		{
		s=s+"   Invalid Track\n\r";
		}
	return s;

}

JFile::FILE_ERROR Cst4_IFF_Track::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	if (!E) E=pF->TextLine("   Squawk Code: "+String(wID()));
	if (!E) E=pF->TextLine("   Track Number: "+String(wTrack()));
	if (!E) E=pF->TextLine("   Position: "+GetLL().sLL());
	if (!E) E=pF->TextLine("   Height: "+String(nHeight_ft())+" ft");
	return E;
}

String Cst4_IFF_Track::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	s=s+"   Squawk Code: "+String(wID())+"\n\r";
	s=s+"   Track Number: "+String(wTrack())+"\n\r";
	s=s+"   Position: "+GetLL().sLL()+"\n\r";
	s=s+"   Height: "+String(nHeight_ft())+" ft\n\r";
	return s;
}


JFile::FILE_ERROR Cst4_ADSB::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_IFF_Track::Extract(pF);
	if (!E) E=pF->TextLine("   Flight ID: "+String(nFlightID));
	if (!E) E=pF->TextLine("   Callsign: "+sCallsign);
	if (!E) E=pF->TextLine("   Ground Speed: "+String(nGetSpeed_kts())+" kts");
	String sV;
	sV.printf(L"   Course: %1.1f deg",dGetCourse_deg());
	if (!E) E=pF->TextLine(sV);
	return E;
}

String Cst4_ADSB::sExtract()
{
	String s=Cst4_IFF_Track::sExtract();
	s=s+"   Flight ID: "+String(nFlightID)+"\n\r";
	s=s+"   Callsign: "+sCallsign+"\n\r";
	s=s+"   Ground Speed: "+String(nGetSpeed_kts())+" kts\n\r";
	String sV;
	sV.printf(L"   Course: %1.1f deg",dGetCourse_deg());
	s=s+sV+"\n\r";
	return s;
}



JFile::FILE_ERROR Cst4_SysTrack::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	SYSTRACK_ACTION Action=GetAction();
	switch(Action)
		{
		case SYSTRACK_CREATE:	E=pF->TextLine("  Action: Create"); break;
		case SYSTRACK_UPDATE:	E=pF->TextLine("  Action: Update"); break;
		case SYSTRACK_DELETE:	E=pF->TextLine("  Action: Delete"); break;
		case SYSTRACK_DISTR:		E=pF->TextLine("  Action: Distribute"); break;
		}
	if (!E) E=pF->TextLine("   Track Number: "+String(nGetTrackNumber()));
	if (!E) E=pF->TextLine("   Description: "+sDescr);
	String s;
	if (g_pSiteDefs)
		s=g_pSiteDefs->sGetDescrFromID(Origin.dwID);
	else
		s.printf(L"0x%04X",Origin.dwID);
	if (!E) E=pF->TextLine("   Origin: "+s);
	if (!E) E=pF->TextLine("           Seat: "+String(Origin.ucSeat));
	if (!E) E=pF->TextLine("           Time: "+Origin.T_utc.Get_loc().sDateTime(false,false,true));
	if (g_pSiteDefs)
		s=g_pSiteDefs->sGetDescrFromID(Update.dwID);
	else
		s.printf(L"0x%04X",Origin.dwID);
	if (!E) E=pF->TextLine("   Last Update: "+s);
	if (!E) E=pF->TextLine("                Seat: "+String(Update.ucSeat));
	if (!E) E=pF->TextLine("                Time: "+Update.T_utc.Get_loc().sDateTime(false,false,true));
	if (!E) E=pF->TextLine("   Position: "+GetLastPos().sLL());
	s.printf(L"%1.1f kts, %1.1f deg",dLastSpeed_kts(),dLastCourse_deg());
	if (!E) E=pF->TextLine("   Vector: "+s);

	if (!E) E=pF->TextLine("   Classification: "+sGetShipClassification(GetClassification()));

	if (bAISAvail())
		{
		if (!E) E=pF->TextLine("   AIS AVAILABLE");
		if (!E) E=pF->TextLine("   AIS MMSI: "+String(nGetMMSI()));
		}
	if (bRadarAvail())
		{
		if (!E) E=pF->TextLine("   RADAR AVAILABLE");
		if (g_pSiteDefs)
			s=g_pSiteDefs->sGetDescrFromID(Radar.dwID);
		else
			s.printf(L"0x%04X",Radar.dwID);
		if (!E) E=pF->TextLine("   Radar Source: "+s);
		if (!E) E=pF->TextLine("   Radar Track: "+String(Radar.nTrack));
		}

	if (!E) E=pF->TextLine("   "+sComments);
	return E;
}

String Cst4_SysTrack::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
   String sV;
	SYSTRACK_ACTION Action=GetAction();
	switch(Action)
		{
		case SYSTRACK_CREATE:	s=s+"  Action: Create\n\r"; break;
		case SYSTRACK_UPDATE:	s=s+"  Action: Update\n\r"; break;
		case SYSTRACK_DELETE:	s=s+"  Action: Delete\n\r"; break;
		case SYSTRACK_DISTR:		s=s+"  Action: Distribute\n\r"; break;
		}
	s=s+"   Track Number: "+String(nGetTrackNumber())+"\n\r";
	s=s+"   Description: "+sDescr+"\n\r";
	if (g_pSiteDefs)
		sV=g_pSiteDefs->sGetDescrFromID(Origin.dwID);
	else
		sV.printf(L"0x%04X",Origin.dwID);
	s=s+"   Origin: "+sV+"\n\r";
	s=s+"           Seat: "+String(Origin.ucSeat)+"\n\r";
	s=s+"           Time: "+Origin.T_utc.Get_loc().sDateTime(false,false,true)+"\n\r";
	if (g_pSiteDefs)
		sV=g_pSiteDefs->sGetDescrFromID(Update.dwID);
	else
		sV.printf(L"0x%04X",Origin.dwID);
	s=s+"   Last Update: "+sV+"\n\r";
	s=s+"                Seat: "+String(Update.ucSeat)+"\n\r";
	s=s+"                Time: "+Update.T_utc.Get_loc().sDateTime(false,false,true)+"\n\r";
	s=s+"   Position: "+GetLastPos().sLL()+"\n\r";
	sV.printf(L"%1.1f kts, %1.1f deg",dLastSpeed_kts(),dLastCourse_deg());
	s=s+"   Vector: "+sV+"\n\r";

	s=s+"   Classification: "+sGetShipClassification(GetClassification())+"\n\r";

	if (bAISAvail())
		{
		s=s+"   AIS AVAILABLE"+"\n\r";
		s=s+"   AIS MMSI: "+String(nGetMMSI())+"\n\r";
		}
	if (bRadarAvail())
		{
		s=s+"   RADAR AVAILABLE\n\r";
		if (g_pSiteDefs)
			sV=g_pSiteDefs->sGetDescrFromID(Radar.dwID);
		else
			sV.printf(L"0x%04X",Radar.dwID);
		s=s+"   Radar Source: "+sV+"\n\r";
		s=s+"   Radar Track: "+String(Radar.nTrack)+"\n\r";
		}

	s=s+"   "+sComments;
	return s;
}

JFile::FILE_ERROR Cst4_User_Msg::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	String s;
	switch(dwGetType())
		{
		case CST4_USER_INS:			  	s="INS Data";					break;
		case CST4_USER_TRACKER:		  	s="Tracking Radar Data";	break;
		case CST4_USER_WEATHER:		  	s="Weather Information";	break;
		case CST4_USER_AIS_STATUS:	  	s="AIS Status";				break;
		case CST4_USER_SITE_DEFS:	  	s="Site Definition Data";	break;
		case CST4_USER_SITE_UPDATE:  	s="Site Update Data";		break;
		case CST4_USER_SOURCE_STATUS:	s="New Data Sources";		break;
		case CST4_USER_WIND:			  	s="Wind Information";   	break;
		case CST4_USER_AIS_ALT:		  	s="Alternative AIS Data";	break;
		case CST4_USER_KEEP_ALIVE:	  	s="Keep-Alive Message";		break;
		default:								s="Unknown Message";			break;
		}
	if (!E) E=pF->TextLine("   Message Type: "+s);
	DWORD dw=dwPayloadSize(false);
	s.printf(L"   Payload Size: %d",dw);
	if (!E) E=pF->TextLine(s);
	return E;
}

String Cst4_User_Msg::sExtract()
{
	String s=Cst4_MessageBase::sExtract();
	String sV;
	switch(dwGetType())
		{
		case CST4_USER_INS:			  	sV="INS Data";					break;
		case CST4_USER_TRACKER:		  	sV="Tracking Radar Data";	break;
		case CST4_USER_WEATHER:		  	sV="Weather Information";	break;
		case CST4_USER_AIS_STATUS:	  	sV="AIS Status";				break;
		case CST4_USER_SITE_DEFS:	  	sV="Site Definition Data";	break;
		case CST4_USER_SITE_UPDATE:  	sV="Site Update Data";		break;
		case CST4_USER_SOURCE_STATUS:	sV="New Data Sources";		break;
		case CST4_USER_WIND:			  	sV="Wind Information";   	break;
		case CST4_USER_AIS_ALT:		  	sV="Alternative AIS Data";	break;
		case CST4_USER_KEEP_ALIVE:	  	sV="Keep-Alive Message";	break;
		default:								sV="Unknown Message";		break;
		}
	s=s+"   Message Type: "+sV+"\n\r";
	DWORD dw=dwPayloadSize(false);
	sV.printf(L"   Payload Size: %d\n\r",dw);
	s=s+sV;
	return s;
}

int Cst4_SysTrack::nGetPictInFolder(const String sFolder, const DWORD dwUpdate_utc_sec)
{
	int nPictCnt=0;
	WIN32_FIND_DATA FileData;
   String sTrack;
   sTrack.printf(L"%04d",nTrack);
	String sMask=IncludeTrailingBackslash(sFolder)+sTrack+"*.jpg";
	HANDLE FileHandle=::FindFirstFile(sMask.c_str(),&FileData);
	if (FileHandle!=INVALID_HANDLE_VALUE)
		{
		do
			{
			if ((FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
         	{
				String sF=String(FileData.cFileName);
            JTime T;
            bool bValid;
            FILETIME FileTime=FileData.ftCreationTime;
            SYSTEMTIME SysTime_utc;
            ::FileTimeToSystemTime(&FileTime,&SysTime_utc);
            T=JTime(SysTime_utc);
            if (dwUpdate_utc_sec>0)
            	{
               DWORD dwT__utc_sec=T.dwTotalSec();
               bValid=(dwT__utc_sec>dwUpdate_utc_sec);
               }
            else
            	bValid=true;
				if (bValid)
            	{
               String s=sFileNewExt(sFileNameOnly(sF),"");
               s=s.SubString(5,s.Length()-4);
               aPict[nPictCnt].sDescr=s;

//               char cType=sF[6];
               sF=IncludeTrailingBackslash(sFolder)+sF;
               if (nPictCnt<MAX_PICT_NUM)
                  {
                  aPict[nPictCnt].sFile=sF;
//                  switch(cType)
//                     {
//                     case 'N':
//                     case 'n':
//                        aPict[nPictCnt].sType="NARROW";
//                        break;
//                     case 'W':
//                     case 'w':
//                        aPict[nPictCnt].sType="WIDE";
//                        break;
//                     case 'F':
//                     case 'f':
//                        aPict[nPictCnt].sType="FLIR";
//                        break;
//                     default:
//                        aPict[nPictCnt].sType="UNKNOWN";
//                     }
                  aPict[nPictCnt].T=T;
                  ++nPictCnt;
                  }
               }
            }
			}
		while(::FindNextFile(FileHandle,&FileData));
		}
	::FindClose(FileHandle);
   return nPictCnt;
 }
