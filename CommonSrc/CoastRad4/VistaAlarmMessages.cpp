//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "VistaAlarmMessages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool VistaAlarm_VesselStruct::bParseLine(const String sLine)
{
	sName="";
	nIMO=-1;
	nMMSI=-1;
	CommaText Txt(sLine);
	if (Txt.nGetCount()>0)
		{
		try
			{
			nMMSI=Txt.nGetVal(0);
			}
		catch(...)
			{
			nMMSI=-1;
			}
		}
	if (Txt.nGetCount()>1)
		{
		AnsiString s1=Txt.sGetPar(1);
		sName=s1.Trim();
		}
	if (Txt.nGetCount()>2)
		{
		try
			{
			nIMO=Txt.nGetVal(2);
			}
		catch(...)
			{
			nIMO=-1;
			}
		}
	return (nMMSI!=-1);
}

String VistaAlarm_VesselStruct::sListBoxLine()
{
	String s="",sV;
	if (nMMSI>=0)
		sV.printf(L"%08d  ",nMMSI);
	else
		sV="          ";
	s=s+sV;
	if (nIMO>=0)
		sV.printf(L"%08d  ",nIMO);
	else
		sV="          ";
	s=s+sV;
	if (!sName.IsEmpty())
		s=s+sName;
	return s;
}

bool Vista_ZoneDef_Data::bCalcMatrix()
{
	delete pMatrix;
	pMatrix=new ZoneMatrix;
	for (int i=0; i<nGetWayPointCount(); i++)
		pMatrix->AddWayPoint(GetWayPoint(i));
	bool bOK=pMatrix->bCalcMatrix();
	if (!bOK)
		{
		delete pMatrix;
		pMatrix=NULL;
		}
   return bOK;
}

bool Vista_ZoneDef_Data::bInZone(const LatLong& LL)
{
	if (!pMatrix)
		bCalcMatrix();
	if (pMatrix)
		return pMatrix->bInZone(LL);
	else
		return false;

}




