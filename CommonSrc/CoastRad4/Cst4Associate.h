//---------------------------------------------------------------------------

#ifndef Cst4AssociateH
#define Cst4AssociateH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"

class Cst4_Radar_Assoc : public Cst4_Radar_Track, public Cst4Grid
{

	LatLong Adj_LL;

	double dSqr(const double dV) const
	{
		return dV*dV;
	}


public:

	Cst4_Radar_Assoc()
	{
		Adj_LL.Invalidate();
	}

	void Assign(const Cst4_Radar_Assoc& T)
	{
		Cst4_Radar_Track::Assign(T);
		Cst4Grid::Assign(T);
		Adj_LL=T.Adj_LL;
	}

	Cst4_Radar_Assoc(const Cst4_Radar_Assoc& T)
	{
		Assign(T);
	}

	Cst4_Radar_Assoc(const Cst4_Radar_Track& T)
	{
		Cst4_Radar_Track::Assign(T);
		Adj_LL=GetLL();
		SetGridLL(Adj_LL);
		Adj_LL.dLong*=cos(Adj_LL.dLat*M_PI/180.0);	//Adjust Long for latitude
	}

	Cst4_Radar_Assoc& operator = (const Cst4_Radar_Assoc& T)
	{
		Assign(T);
		return *this;
	}

	~Cst4_Radar_Assoc()
	{
	}

	double dDist_m(const LatLong& AIS_Adj_LL) const
	{
		return sqrt(dSqr(Adj_LL.dLat-AIS_Adj_LL.dLat)+dSqr(Adj_LL.dLong-AIS_Adj_LL.dLong))*111227.2;
	}

};

class Cst4_AIS_Assoc : public Cst4_AIS_Dynamic, public Cst4Grid
{
	LatLong Adj_LL;

public:

	Cst4_AIS_Assoc()
	{
		Adj_LL.Invalidate();
	}

	void Assign(const Cst4_AIS_Assoc& T)
	{
		Cst4_AIS_Dynamic::Assign(T);
		Cst4Grid::Assign(T);
		Adj_LL=T.Adj_LL;
	}

	Cst4_AIS_Assoc(const Cst4_AIS_Assoc& T)
	{
		Assign(T);
	}

	Cst4_AIS_Assoc(const Cst4_AIS_Dynamic& D)
	{
		Cst4_AIS_Dynamic::Assign(D);
		Adj_LL=GetLL();
		SetGridLL(Adj_LL);
		Adj_LL.dLong*=cos(Adj_LL.dLat*M_PI/180.0);	//Adjust Long for latitude
	}

	Cst4_AIS_Assoc& operator = (const Cst4_AIS_Assoc& T)
	{
		Assign(T);
		return *this;
	}

	~Cst4_AIS_Assoc()
	{
	}

	LatLong GetAdjLL() const
	{
		return Adj_LL;
	}

};



class Cst4Associate
{

	JList<Cst4_AIS_Assoc> AISList;

public:

	void Init()
	{
		AISList.Clear();
	}

	Cst4Associate()
	{
		Init();
	}

	~Cst4Associate()
	{
	}

	void Assign(const Cst4Associate& A)
	{
		AISList=A.AISList;
		AISList.Pack();
	}

	Cst4Associate(const Cst4Associate& A)
	{
		Assign(A);
	}

	Cst4Associate& operator = (const Cst4Associate& A)
	{
		Assign(A);
		return *this;
	}

	void AddMsg(const Cst4_AIS_Dynamic& Msg);

	void Consolidate_utc(const DWORD dwSec_utc);

	void UpdateMsg(Cst4_Radar_Track* pMsg);

	int nGetAISCnt() const
	{
		return AISList.nGetCount();
	}


};


#endif
