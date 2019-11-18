//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Associate.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Cst4Associate::AddMsg(const Cst4_AIS_Dynamic& Msg)
{
	AISList.GoFirst();
	bool bFin=false;
	Cst4_AIS_Dynamic* pMsg;
	while((!AISList.bLast())&&(!bFin))
		{
		pMsg=AISList.pNext();
		if (pMsg->nGetMMSI()==Msg.nGetMMSI())
			{
			AISList.GoPrev();
			AISList.Delete();
			bFin=true;
			}
		}
	AISList.nAdd(new Cst4_AIS_Assoc(Msg));
	AISList.Pack();
}

void Cst4Associate::Consolidate_utc(const DWORD dwSec_utc)
{
	DWORD dwFirstSec_utc=dwSec_utc-60;
	AISList.GoFirst();
//	bool bFin=false;
	Cst4_AIS_Dynamic* pMsg;
	while(!AISList.bLast())
		{
		pMsg=AISList.pNext();
		DWORD dwT_sec_utc=pMsg->dwTime_sec_utc();
		if (dwT_sec_utc<dwFirstSec_utc)
			{
			AISList.GoPrev();
			AISList.Delete();
			}
//		else
//			bFin=true;
		}
	AISList.Pack();
}

void Cst4Associate::UpdateMsg(Cst4_Radar_Track* pMsg)
{
	bool bAssoc=false;
	if (pMsg->ucQuality()>=20)
		{
		double dMinDist_m=1e10;
		int nMinAIS=-1;
		Cst4_Radar_Assoc AMsg(*pMsg);
		for (int i=0; i<AISList.nGetCount(); i++)
			{
			if (AISList[i]->bGridClose(AMsg))
				{
				double dDist_m=AMsg.dDist_m(AISList[i]->GetAdjLL());
				if (dDist_m<dMinDist_m)
					{
					dMinDist_m=dDist_m;
					nMinAIS=i;
					}
				}
			}
		if (dMinDist_m<2000.0)
			{
			if (fabs(pMsg->dSpeed_kts()-AISList[nMinAIS]->dSpeed_kts())<5.0);
				{
				if (AISList[nMinAIS]->dSpeed_kts()>5.0)
					{
					double dCourseDiff_deg=fabs(pMsg->dCourse_deg()-AISList[nMinAIS]->dCourse_deg());
					if (dCourseDiff_deg>180.0)
						dCourseDiff_deg=360.0-dCourseDiff_deg;
					if (dCourseDiff_deg<90.0)
						bAssoc=true;
					}
				else
					bAssoc=true;	//Speed very low -> ignore course
				}
			}
		}
	pMsg->SetAssociated(bAssoc);
}
