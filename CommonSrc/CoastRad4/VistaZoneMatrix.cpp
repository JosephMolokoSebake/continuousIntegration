//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "VistaZoneMatrix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


double ZoneMatrix::dGetWayLong(const LatLong& LL1, const LatLong& LL2, const double dLat_deg)
{
	if (bBetween(dLat_deg,LL1.dLat,LL2.dLat))
		{
		double dDeltaLat=LL2.dLat-LL1.dLat;
		if (fabs(dDeltaLat)<1e-9)
			return LL1.dLong;
		double dDeltaLong=LL2.dLong-LL1.dLong;
		if (fabs(dDeltaLong)<1e-9)
			return LL2.dLong;    //Changed from invalid - JGT 30/07/2019
//			return -1e10;   //invalid
		double dLong=LL1.dLong+(dLat_deg-LL1.dLat)*dDeltaLong/dDeltaLat;
		return dLong;
		}
	else
		return -1e10;   //invalid
}

bool ZoneMatrix::bCreateZone(const String sFil)
{
   bool bOK=false;
   if (bFileExist(sFil))
   	{
      JFile Fil('I',JFile::ASCII_TYPE);
      JFile::FILE_ERROR E=Fil.Open(sFil);
      if (!E)
         {
         LatLong FirstLL;
         FirstLL.Invalidate();
         LatLong LL;
         char szSt[256];
         int n;
         while(!E)
            {
            E=Fil.Read(szSt,255);
            if (!E)
               {
               n=sscanf(szSt,"%lf%lf",&LL.dLat,&LL.dLong);
               if (n==2)
                  {
                  AddWayPoint(LL);
                  if (!FirstLL.bValid())
                     FirstLL=LL;
                  }
            	}
            }
         if (FirstLL.bValid())
	         AddWayPoint(FirstLL);   //Close the zone by adding the first waypoint at the end
         bOK=bCalcMatrix();
         }
   	}
   return bOK;
}



bool ZoneMatrix::bCalcMatrix()
{
	WayList.Pack();
	double dLatDiff=fabs(MaxLL.dLat-MinLL.dLat);
//   dLatPrecision=60000.0;

   if (dLatDiff<0.1)
	   dLatPrecision=1000000.0;
   else if (dLatDiff<1.0)
	   dLatPrecision=100000.0;
   else
	   dLatPrecision=60000.0;


	nLatCnt=dLatDiff*dLatPrecision+1.5;

   String s;
	if (pTestFil)
		{
		pTestFil->TextLine("Min: "+MinLL.sLL());
		pTestFil->TextLine("Max: "+MaxLL.sLL());
		s.printf(L"Lat Difference = %3.3f",dLatDiff);
		pTestFil->TextLine(s);
		s.printf(L"LatCnt = %d",nLatCnt);
		pTestFil->TextLine(s);
		}
	if (nLatCnt>0)
		{
		pLatSeg=new LatSeg[nLatCnt];
		double adLong[100];
		int nSeg;
		for (int n=0; n<nLatCnt; n++)
			{
			nSeg=0;
			double dLat=MinLL.dLat+n/dLatPrecision;
			for (int nWay=0; nWay<WayList.nGetCount()-1; nWay++)
				{
				double dLong=dGetWayLong(*WayList[nWay],*WayList[nWay+1],dLat);
				if (dLong>-1e9)
					adLong[nSeg++]=dLong;
				}
			if (nSeg>1)
				{
				//Bubble sort
				double dT;
				for (int i=0; i<nSeg; i++)
					{
					for (int j=0; j<nSeg-1; j++)
						{
						if (adLong[j]>adLong[j+1])
							{
							dT=adLong[j];
							adLong[j]=adLong[j+1];
							adLong[j+1]=dT;
							}
						}
					}
				}
			pLatSeg[n].nSegCnt=nSeg;
			if (nSeg>1)
				{
				pLatSeg[n].pdLong=new double[nSeg];
				for (int i=0; i<nSeg; i++)
					pLatSeg[n].pdLong[i]=adLong[i];

				if (pTestFil)
					{
					String sV;
					sV.printf(L"%6d  %14.8f:",n,dLat);
					for (int i=0; i<nSeg; i++)
						{
						s.printf(L"  %14.8f",adLong[i]);
						sV=sV+s;
						}
               pTestFil->TextLine(sV);
					}
				}
			}
		return true;
		}
	return false;
}

bool ZoneMatrix::bInZone(const LatLong& LL)
{
	String sV;
	if (bBetween(LL.dLat,MinLL.dLat,MaxLL.dLat)&& bBetween(LL.dLong,MinLL.dLong,MaxLL.dLong))
		{
		int n=(LL.dLat-MinLL.dLat)*dLatPrecision;
      if (pTestFil)
         {
         sV.printf(L"%5d  ",n);
         sV=sV+LL.sLL();
         }
		if ((n>=0)&&(n<nLatCnt))
			{
			if (pLatSeg[n].pdLong)
				{
				int nSeg=0;
				while(nSeg<pLatSeg[n].nSegCnt)
					{
					if (bBetween(LL.dLong,pLatSeg[n].pdLong[nSeg],pLatSeg[n].pdLong[nSeg+1]))
						{
						if (pTestFil)
							{
							sV=sV+"  ****** INSIDE";
							pTestFil->TextLine(sV);
							}
						return true;
						}
					nSeg+=2;
					}
				}
			}
		if (pTestFil)
			{
			sV=sV+"  OUT";
			pTestFil->TextLine(sV);
			}
		}
	return false;
}



