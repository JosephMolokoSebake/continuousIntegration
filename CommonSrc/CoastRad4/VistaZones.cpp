//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "VistaZones.h"
#include "JFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

VistaZones::VistaZones()
{
   Init();

}

VistaZones::~VistaZones()
{


}

void VistaZones::Init()
{
   InsideList.Clear();
   OutsideList.Clear();
   nTotalCnt=0;
	nValidCnt=0;
   nBlockedCnt=0;

}

bool VistaZones::bAddInsideZone(const String sFil)
{
	ZoneMatrix* pZone=new ZoneMatrix;
   if (pZone->bCreateZone(sFil))
   	{
      InsideList.nAdd(pZone);
      return true;
   	}
   delete pZone;
   return false;
}

bool VistaZones::bAddOutsideZone(const String sFil)
{
	ZoneMatrix* pZone=new ZoneMatrix;
   if (pZone->bCreateZone(sFil))
   	{
      OutsideList.nAdd(pZone);
      return true;
   	}
   delete pZone;
   return false;
}

bool VistaZones::bValidLL(const LatLong& LL)
{
   bool bValid;
   if (InsideList.nGetCount()>0)
      {
      bValid=false;
	   InsideList.GoFirst();
   	while(!InsideList.bLast() && !bValid)
      	bValid=InsideList.pNext()->bInZone(LL);
      }
   else
      bValid=true;
   if (bValid)
      {
      OutsideList.GoFirst();
      while(!OutsideList.bLast() && bValid)
         {
         if (OutsideList.pNext()->bInZone(LL))
            bValid=false;
         }
   	}
   ++nTotalCnt;
   if (bValid)
      ++nValidCnt;
   else
      ++nBlockedCnt;
   return bValid;
}

