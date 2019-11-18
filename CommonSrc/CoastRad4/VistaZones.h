//---------------------------------------------------------------------------

#ifndef VistaZonesH
#define VistaZonesH
//---------------------------------------------------------------------------

#include "VistaZoneMatrix.h"


class VistaZones
{

   JList<ZoneMatrix> InsideList;
   JList<ZoneMatrix> OutsideList;

   int nTotalCnt;
	int nValidCnt;
   int nBlockedCnt;


public:

   VistaZones();
   ~VistaZones();

   void Init();

   bool bAddInsideZone(const String sFil);
   bool bAddOutsideZone(const String sFil);

	bool bValidLL(const LatLong& LL);

  	int nGetTotalCnt() 	const	{ return nTotalCnt;  	}
	int nGetValidCnt() 	const	{ return nValidCnt;  	}
   int nGetBlockedCnt()	const	{ return  nBlockedCnt;  }



};


#endif
