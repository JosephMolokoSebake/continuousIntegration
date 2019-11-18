//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst5Messages.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JFile::FILE_ERROR Cst5_MessageBase::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=pF->TextLine("");
	if (!E) E=pF->TextLine("**********************************");
	String s;
	if (!E) E=pF->TextLine(sGetMessageDescription((CST4_MSG_TYPE)(dwStatus&CST5_TYPE_MASK)));
	if (g_pSiteDefs)
		s=g_pSiteDefs->sGetDescrFromID(wSource());
	else
		s.printf(L"0x%04X",wSource());
	if (!E) E=pF->TextLine("   Source: "+s);
	JTime T=Time_utc();
	if (T.bAllValid())
		s="   Time: "+T.sDateTime(true,true,true);
	else
		s="   Time: INVALID";
	if (!E) E=pF->TextLine(s);
	s.printf(L"%d",dwSize());
	if (!E) E=pF->TextLine("   Message Size: "+s+" bytes");
	return E;
}

JFile::FILE_ERROR Cst4_AIS_Dynamic::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	String s;
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

/*
JFile::FILE_ERROR Cst5_Image::Extract(JFile* pF)
{
	JFile::FILE_ERROR E=Cst4_MessageBase::Extract(pF);
	String s;
	s.printf(L"   MMSI: %d",nGetMMSI());
	if (!E) E=pF->TextLine(s);
	int nType,nIMO;
	String sCallsign;
	String sName=sBasicInfo(nType,nIMO,sCallsign);
	if (!E) E=pF->TextLine("   IMO: "+String(nIMO));
	if (!E) E=pF->TextLine("   Name: "+sName);
	if (!E) E=pF->TextLine("   Callsign: "+sCallsign);
	return E;
}
*/

