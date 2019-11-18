//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Stats.h"
#include "JUtils.h"
//#include "Config.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

JFile::FILE_ERROR Cst4SiteStats::ReadOldFormat(JFile* pFil)
{
	JFile::FILE_ERROR E=pFil->Read(&wSiteID,sizeof(wSiteID));
	if (!E) E=pFil->Read(&dwFirstMsgTime_utc_sec,sizeof(dwFirstMsgTime_utc_sec));
	if (!E) E=pFil->Read(&dwLastMsgTime_utc_sec,sizeof(dwLastMsgTime_utc_sec));
	if (!E) E=pFil->Read(&dwTotalMsgCnt,sizeof(dwTotalMsgCnt));
	if (!E) E=pFil->Read(aMin,sizeof(aMin));
	DWORD adwDummy[16];
	if (!E) E=pFil->Read(adwDummy,sizeof(adwDummy));
	return E;
}

JFile::FILE_ERROR Cst4SiteStats::Read(JFile* pFil)
{
   JFile::FILE_ERROR E=pFil->Read(&wSiteID,sizeof(wSiteID));
	if (!E) E=pFil->Read(&dwFirstMsgTime_utc_sec,sizeof(dwFirstMsgTime_utc_sec));
	if (!E) E=pFil->Read(&dwLastMsgTime_utc_sec,sizeof(dwLastMsgTime_utc_sec));
	if (!E) E=pFil->Read(&dwTotalMsgCnt,sizeof(dwTotalMsgCnt));
	if (!E) E=pFil->Read(aMin,sizeof(aMin));
	sDescr="";
	int n;
	if (!E) E=pFil->Read(&n,sizeof(n));
	if ((!E)&&(n>0))
		{
		char* psz=new char[n+1];
		E=pFil->Read(psz,n);
		if (!E)
			{
			psz[n]=0;
			sDescr=String(psz);
			}
		delete[] psz;
		sDescr=g_pSiteDefs->sGetDescrFromID(wGetID());
		}
	return E;
}

	int Cst4SiteStats::nGetPackedSize()
	{
   	int n=0;
		n+=sizeof(wSiteID);
		n+=sizeof(dwFirstMsgTime_utc_sec);
		n+=sizeof(dwLastMsgTime_utc_sec);
		n+=sizeof(dwTotalMsgCnt);
		n+=sizeof(aMin);
		AnsiString as=sDescr;
		int nS=as.Length();
		n+=sizeof(nS);
		n+=nS;
		return n;
	}


void Cst4SiteStats::Pack(BYTE* pucPacked, int& n)
{
	memcpy(&pucPacked[n],&wSiteID,sizeof(wSiteID)); 	n+=sizeof(wSiteID);
	memcpy(&pucPacked[n],&dwFirstMsgTime_utc_sec,sizeof(dwFirstMsgTime_utc_sec));	n+=sizeof(dwFirstMsgTime_utc_sec);
	memcpy(&pucPacked[n],&dwLastMsgTime_utc_sec,sizeof(dwLastMsgTime_utc_sec));	n+=sizeof(dwLastMsgTime_utc_sec);
	memcpy(&pucPacked[n],&dwTotalMsgCnt,sizeof(dwTotalMsgCnt));	n+=sizeof(dwTotalMsgCnt);
	memcpy(&pucPacked[n],aMin,sizeof(aMin));	n+=sizeof(aMin);
	AnsiString as=sDescr;
	int nS=as.Length();
	memcpy(&pucPacked[n],&nS,sizeof(nS));	n+=sizeof(nS);
	if (nS>0)
		{
		memcpy(&pucPacked[n],as.c_str(),nS);	n+=nS;
		}
}

void Cst4SiteStats::Unpack(const BYTE* pucPacked, int& n)
{
	memcpy(&wSiteID,&pucPacked[n],sizeof(wSiteID)); 	n+=sizeof(wSiteID);
	memcpy(&dwFirstMsgTime_utc_sec,&pucPacked[n],sizeof(dwFirstMsgTime_utc_sec));	n+=sizeof(dwFirstMsgTime_utc_sec);
	memcpy(&dwLastMsgTime_utc_sec,&pucPacked[n],sizeof(dwLastMsgTime_utc_sec));	n+=sizeof(dwLastMsgTime_utc_sec);
	memcpy(&dwTotalMsgCnt,&pucPacked[n],sizeof(dwTotalMsgCnt));	n+=sizeof(dwTotalMsgCnt);
	memcpy(aMin,&pucPacked[n],sizeof(aMin));	n+=sizeof(aMin);
	AnsiString as;
	int nS;
	memcpy(&nS,&pucPacked[n],sizeof(nS));	n+=sizeof(nS);
	if (nS>0)
		{
		char *sz=new char[nS+1];
		memcpy(sz,&pucPacked[n],nS);	n+=nS;
		as=AnsiString(sz);
		delete[] sz;
		}
	else
		as="";
	sDescr=as;
}

JFile::FILE_ERROR Cst4SiteStats::Store(JFile* pFil)
{
	JFile::FILE_ERROR E=pFil->Write(&wSiteID,sizeof(wSiteID));
	if (!E) E=pFil->Write(&dwFirstMsgTime_utc_sec,sizeof(dwFirstMsgTime_utc_sec));
	if (!E) E=pFil->Write(&dwLastMsgTime_utc_sec,sizeof(dwLastMsgTime_utc_sec));
	if (!E) E=pFil->Write(&dwTotalMsgCnt,sizeof(dwTotalMsgCnt));
	if (!E) E=pFil->Write(aMin,sizeof(aMin));
	AnsiString as=sDescr;
	int n=as.Length();
	if (!E) E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(as.c_str(),n);
	return E;
}

void Cst4SiteStats::AddMsg_utc(const JTime& T_utc)
{
	AddMsg_utc(T_utc.dwTotalSec());
}

void Cst4SiteStats::AddMsg_utc(const DWORD dwSec_utc)
{
	++dwTotalMsgCnt;
	dwLastMsgTime_utc_sec=dwSec_utc;
	if (dwFirstMsgTime_utc_sec==0)
		dwFirstMsgTime_utc_sec=dwSec_utc;
	int nMin_utc=dwSec_utc/60;
	int nIdx=nMin_utc%(24*60);
	if (aMin[nIdx].nMin_utc==nMin_utc)
		++aMin[nIdx].nCnt;
	else
		{
		aMin[nIdx].nCnt=1;
		aMin[nIdx].nMin_utc=nMin_utc;
		}
}

DWORD Cst4SiteStats::dwGetMsgCnt_utc(const JTime& T_utc, const int nMinutes)
{
	int nLastMin_utc=T_utc.dwTotalSec()/60;
	int nFirstMin_utc=nLastMin_utc-nMinutes;
	DWORD dwCnt=0;
	for (int i=0; i<24*60; i++)
		{
		if ((aMin[i].nMin_utc>=nFirstMin_utc)&&(aMin[i].nMin_utc<=nLastMin_utc))
			dwCnt+=aMin[i].nCnt;
		}
	return dwCnt;
}

DWORD Cst4SiteStats::dwGetMsgCnt_utc(const DWORD dwT_utc_sec, const int nMinutes)
{
	int nLastMin_utc=dwT_utc_sec/60;
	int nFirstMin_utc=nLastMin_utc-nMinutes;
	DWORD dwCnt=0;
	for (int i=0; i<24*60; i++)
		{
		if ((aMin[i].nMin_utc>=nFirstMin_utc)&&(aMin[i].nMin_utc<=nLastMin_utc))
			dwCnt+=aMin[i].nCnt;
		}
	return dwCnt;
}

//*********************************************************************************

Cst4Stats::Cst4Stats()
{
//	SiteDefs.bLoadSites();
	for (int i=0; i<65536; i++)
		apSite[i]=NULL;
}

Cst4Stats::~Cst4Stats()
{
	for (int i=0; i<65536; i++)
		delete apSite[i];
}

int Cst4Stats::nGetPackSize()
{
	int n=0;
	DWORD dwVer;
	n+=sizeof(dwVer);
/*	int nCnt=0;
	for (int i=0; i<65536; i++)
		if (apSite[i])
			++nCnt;
	n+=sizeof(nCnt);
*/
	n+=sizeof(int);
	for (int i=0; i<65536; i++)
		if (apSite[i])
			n+=apSite[i]->nGetPackedSize();
	return n;
}


int Cst4Stats::nPack(BYTE* pucPacked)
{
	int n=0;
	DWORD dwVer=SITESTATS_VERSION;
	memcpy(&pucPacked[n],&dwVer,sizeof(dwVer));
	n+=sizeof(dwVer);
	int nCnt=0;
	for (int i=0; i<65536; i++)
		if (apSite[i])
			++nCnt;
	memcpy(&pucPacked[n],&nCnt,sizeof(nCnt));
	n+=sizeof(nCnt);
	for (int i=0; i<65536; i++)
		if (apSite[i])
			apSite[i]->Pack(pucPacked,n);
	return n;
}

void Cst4Stats::Store(const String sFile)
{
	DWORD dwVer=SITESTATS_VERSION;
	JFile Fil('O');
	JFile::FILE_ERROR E=Fil.Create(sFile);
	if (!E)
		E=Fil.Write(&dwVer,sizeof(dwVer));
	int nCnt=0;
	for (int i=0; i<65536; i++)
		if (apSite[i])
			++nCnt;
	if (!E)
		E=Fil.Write(&nCnt,sizeof(nCnt));
	for (int i=0; (i<65536)&&(!E); i++)
		if (apSite[i])
			E=apSite[i]->Store(&Fil);
	Fil.ShowError(E);
}

void Cst4Stats::Unpack(const BYTE* pucPacked)
{
	int n=0;
	for (int i=0; i<65536; i++)
		{
		delete apSite[i];
		apSite[i]=NULL;
		}
	DWORD dwVer;
	memcpy(&dwVer,&pucPacked[n],sizeof(dwVer));	n+=sizeof(dwVer);
	int nCnt;
	memcpy(&nCnt,&pucPacked[n],sizeof(nCnt));		n+=sizeof(nCnt);
	if (dwVer>=(DWORD)SITESTATS_MIN_VERSION)
		{
		for (int i=0; (i<nCnt); i++)
			{
			Cst4SiteStats Stat;
			Stat.Unpack(pucPacked,n);
			apSite[Stat.wGetID()]=new Cst4SiteStats(Stat);
			}
		}
}


void Cst4Stats::Read(const String sFile)
{
	if (bFileExist(sFile))
		{
		for (int i=0; i<65536; i++)
			{
			delete apSite[i];
			apSite[i]=NULL;
			}
		JFile Fil('I');
		JFile::FILE_ERROR E=Fil.Open(sFile);
		DWORD dwVer;
		if (!E)
			E=Fil.Read(&dwVer,sizeof(dwVer));
		int nCnt;
		if (!E)
			E=Fil.Read(&nCnt,sizeof(nCnt));
		if (dwVer<(DWORD)SITESTATS_MIN_VERSION)
			{
			String sDescr;
			for (int i=0; (i<nCnt)&&(!E); i++)
				{
				Cst4SiteStats Stat;
				E=Stat.ReadOldFormat(&Fil);
				sDescr=g_pSiteDefs->sGetDescrFromID(Stat.wGetID());
				Stat.SetDescr(sDescr);
				apSite[Stat.wGetID()]=new Cst4SiteStats(Stat);
				}
			}
		else
			{
			for (int i=0; (i<nCnt)&&(!E); i++)
				{
				Cst4SiteStats Stat;
				E=Stat.Read(&Fil);
				apSite[Stat.wGetID()]=new Cst4SiteStats(Stat);
				}
			}
		Fil.ShowError(E,10);
		}
}


