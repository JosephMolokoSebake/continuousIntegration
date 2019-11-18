//---------------------------------------------------------------------------

#ifndef VistaZoneMatrixH
#define VistaZoneMatrixH
//---------------------------------------------------------------------------

#include "JList.h"
#include "Cst4Messages.h"

class ZoneMatrix  : public Cst4_MessageUtils
{

   JFile* pTestFil;

   double dLatPrecision;


	JList<LatLong> WayList;
	LatLong MinLL,MaxLL;
	int nLatCnt;

	struct LatSeg
	{
		int nSegCnt;
		double* pdLong;

		LatSeg()
		{
			nSegCnt=0;
			pdLong=NULL;
		}

		~LatSeg()
		{
			delete[] pdLong;
		}


	}* pLatSeg;

	double dGetWayLong(const LatLong& LL1, const LatLong& LL2, const double dLat_deg);

	bool bBetween(const double dB, const double d1, const double d2) const
	{
		if ((dB<d1)&&(dB<d2))
			return false;
		if ((dB>d1)&&(dB>d2))
			return false;
		return true;
	}

public:

	ZoneMatrix()
	{
		pTestFil=NULL;
//      pTestFil=new JFile('O',JFile::ASCII_TYPE);
//      pTestFil->Create("ZoneMatrix.txt");


		pLatSeg=NULL;
		Init();
	}

	~ZoneMatrix()
	{
		delete[] pLatSeg;
      delete pTestFil;
	}

	void Init()
	{
		nLatCnt=0;
		WayList.Clear();
		MinLL=LatLong(1e10,1e10);
		MaxLL=LatLong(-1e10,-1e10);
		delete[] pLatSeg;
		pLatSeg=NULL;
		WayList.Pack();
	}

	bool bCreateZone(const String sFil);

	void AddWayPoint(const LatLong& LL)
	{
		if (LL.dLat<MinLL.dLat)
      	MinLL.dLat=LL.dLat;
		if (LL.dLong<MinLL.dLong)
      	MinLL.dLong=LL.dLong;
		if (LL.dLat>MaxLL.dLat)
      	MaxLL.dLat=LL.dLat;
		if (LL.dLong>MaxLL.dLong)
      	MaxLL.dLong=LL.dLong;
		WayList.nAdd(new LatLong(LL));
	}

	bool bCalcMatrix();

	bool bInZone(const LatLong& LL);

};

#endif
