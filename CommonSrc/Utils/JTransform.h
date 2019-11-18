//---------------------------------------------------------------------------
#ifndef JTransformH
#define JTransformH
//---------------------------------------------------------------------------

#include "Math.h"

struct LatLong
{

	double dLong;
	double dLat;

	void Invalidate()
	{
		dLat=-1e10;
		dLong=-1e10;
	}

	LatLong()
	{
		Invalidate();
	}

   LatLong(const double _dLat, const double _dLong)
   {
      dLat=_dLat;
      dLong=_dLong;
   }

   LatLong& operator = (const LatLong& L)
   {
      dLat=L.dLat;
      dLong=L.dLong;
      return *this;
   }

   bool operator != (const LatLong& L) const
   {
      return (fabs(dLat-L.dLat)>0.0000001)||(fabs(dLong-L.dLong)>0.0000001);
   }

   LatLong(const LatLong& L)
   {
      *this=L;
   }

   String sLL() const;
	String sLat(const bool bGPSFormat=false) const;
   String sLong(const bool bGPSFormat=false) const;

   bool bValid() const
   {
      return bValidLat() && bValidLong();
   }

   bool bValidLat() const
   {
      return (fabs(dLat)<100.0);
   }

   bool bValidLong() const
   {
      return (fabs(dLong)<361.0);
   }

   int nGetLatDeg() const;
   double dGetLatMinutes() const;
   char cGetLatHemi() const;
   int nGetLongDeg() const;
   double dGetLongMinutes() const;
   char cGetLongHemi() const;


};

struct SizeD
{

   double dW;
   double dH;

   SizeD()
   {
      dW=0.0;
      dH=0.0;
   }

   SizeD(const double _dW, const double _dH)
   {
      dW=_dW;
      dH=_dH;
   }

   SizeD(const SizeD& s)
   {
      dW=s.dW;
      dH=s.dH;
   }

};

struct Polar
{

   double dRa;
   double dBe;

   Polar()
   {
      dBe=0;
      dRa=0;
   }

   Polar(const double _dRa, const double _dBe)
   {
      dBe=_dBe;
      dRa=_dRa;
   }

   Polar(const Polar& Pol)
   {
      dRa=Pol.dRa;
      dBe=Pol.dBe;
   }

   void Invalidate()
   {
      dRa=-1000.0;
   }

   bool bValid() const
   {
      return (dRa>-999.0);
   }

};


struct XYPos
{

	double dx;		//m
	double dy;     //m

	XYPos()
   {
		dx=0;
		dy=0;
	}

	XYPos(const double _dx, const double _dy)
   {
		dx=_dx;
		dy=_dy;
	}

	XYPos(const XYPos& XY)
   {
		dx=XY.dx;
		dy=XY.dy;
	}

   void Invalidate()
   {
      dx=-1e200;
      dy=-1e200;
   }

   bool bValid() const
   {
      return (dx>-1e199);
   }

	friend XYPos operator + (const XYPos& P1, const XYPos& P2)
   {
		return XYPos(P1.dx +P2.dx,P1.dy+P2.dy);
	}

	friend XYPos operator - (const XYPos& P1, const XYPos& P2)
   {
		 return XYPos(P1.dx-P2.dx,P1.dy-P2.dy);
	}

	XYPos& operator += (const XYPos& Pos)
   {
		dx+=Pos.dx;
		dy+=Pos.dy;
		return *this;
	}

	XYPos& operator -= (const XYPos& Pos)
   {
		dx-=Pos.dx;
		dy-=Pos.dy;
		return *this;
	}

	friend XYPos  operator * (const double __re_val1, const XYPos& __z2)
   {
		return XYPos(__z2.dx*__re_val1,__z2.dy*__re_val1);
	}

	friend XYPos  operator/(const XYPos& __z1, const double __re_val2)
   {
		return XYPos(__z1.dx/__re_val2,__z1.dy/__re_val2);
	}

};

struct Pixel
{

   int nx;
   int ny;

   Pixel()
   {
      nx=0xFFFFFFFF;
      ny=0xFFFFFFFF;
   }

   Pixel(const double _dx, const double _dy)
   {
      nx=_dx;
      ny=_dy;
   }

   Pixel(const int _nx, const int _ny)
   {
      nx=_nx;
      ny=_ny;
   }

   Pixel(const Pixel& pt)
   {
      nx=pt.nx;
      ny=pt.ny;
   }

   Pixel(const POINT& pt)
   {
      nx=pt.x;
      ny=pt.y;
   }

   Pixel& operator = (const Pixel& p)
   {
      nx=p.nx;
      ny=p.ny;
      return *this;
   }

   Pixel& operator = (const POINT& p)
   {
      nx=p.x;
      ny=p.y;
      return *this;
   }

   bool bEqual(const Pixel& p) const
   {
      return (nx==p.nx)&&(ny==p.ny);
   }


   POINT GetPoint() const
   {
      POINT p;
      p.x=nx;
      p.y=ny;
      return p;
   }

   POINT ToPoint()
   {
      POINT p;
      p.x=nx;
      p.y=ny;
      return p;
   }

   void Invalidate()
   {
      nx=0xFFFFFFFF;
   }

   bool bValid() const
   {
      return ((DWORD)nx!=0xFFFFFFFF);
   }

   void Set(const int _nx, const int _ny)
   {
      nx=_nx;
		ny=_ny;
	}


};

class Transform
{

	class Mercator
	{

		double dXRadius_m,dYRadius_m;

		double dRefLat_deg,dRefLong_deg;

		double dDeg2Rad(const double dDeg) const
		{
			return dDeg*M_PI/180.0;
		}

		double dRad2Deg(const double dRad) const
		{
			return dRad*180.0/M_PI;
		}

	public:

		Mercator();
		~Mercator();

		void SetRef(const double dLat_deg, const double dLong_deg)
		{
			dRefLat_deg=dLat_deg;
			dRefLong_deg=dLong_deg;
		}

		void LL2XY(const double dLat_deg, const double dLong_deg, double& dx_m, double& dy_m);
		void XY2LL(const double dx_m, const double dy_m, double &dLat_deg, double& dLong_deg);


	};

public:

	enum PROJECTION{SPHERICAL,MERCATOR};
	enum TRANSFORM_TYPE{FLAT_TRANS,ROUND_TRANS};

private:

	Mercator* pMerc;

	TRANSFORM_TYPE TransType;
	PROJECTION Projection;

	double dDeg2Rad;
	double dRad2M;
	double dDeg2M;

	LatLong LLRef;
	LatLong LLCenter;
	SizeD LLSize;
	LatLong RadarPos;
	double dRadarHeight_m;
	double dCosLatRad;
	double dSinLatRad;
	double dCosLatRef;
	double dSinLatRef;

	long double ldCosRulePar1;
	long double ldCosRulePar2;

	bool bValidTrans;

	double dACOS(const double dV) const
	{
		if (dV>0.9999999999999999)
			return 0.0;
		else if (dV<-0.9999999999999999)
			return M_PI;
		else
			return acos(dV);
	}

	long double ldACOS(const long double ldV) const
	{
		if (ldV>0.9999999999999999)
			return 0.0;
		else if (ldV<-0.9999999999999999)
			return M_PI;
		else
			return acosl(ldV);
	}

public:

	Transform();

	~Transform()
	{
		delete pMerc;
	}

	void Invalidate()
	{
		bValidTrans=false;
	}

	bool bValid() const
	{
		return bValidTrans;
	}

	void SetProjection(const PROJECTION Project, const LatLong& LL)
	{
		Projection=Project;
		if (Projection==MERCATOR)
			{
			if (!pMerc)
				pMerc=new Mercator;
			pMerc->SetRef(LL.dLat,LL.dLong);
			}
		else
			{
			delete pMerc;
			pMerc=NULL;
			}
	}

	void FixAspect(const int nWidth, const int nHeight, SizeD& Factor, XYPos& ZoomCenter);

	double dSqr(const double dV) const
	{
		return dV*dV;
	}

	long double ldSqr(const long double ldV) const
	{
		return ldV*ldV;
	}

   //All Polar conversions are relative to radar position
   Polar LL2Pol(const LatLong& LL);
   Polar XY2Pol(const XYPos& XY);

	LatLong Pol2LL(const Polar& Pol);
	LatLong Pol2LL(const double dRa_m, const double dAz_deg)
	{
		return Pol2LL(Polar(dRa_m,dAz_deg));
	}

	LatLong XY2LL(const XYPos& XY);

	XYPos Pol2XY(const Polar& Pol);
	XYPos Pol2XY(const double dRa_m, const double dAz_deg)
	{
		return Pol2XY(Polar(dRa_m,dAz_deg));
	}
	XYPos LL2XY(const LatLong& LL);

   Polar GetPol(const LatLong& LL1, const LatLong& LL2);
   Polar GetPol(const XYPos& XY1, const XYPos& XY2);

   XYPos GetXY(const LatLong& LL1, const LatLong& LL2);
   XYPos GetXY(const XYPos& XY1, const XYPos& XY2);

   void SetLLRef(const LatLong& Pos);

   void SetLLView(const LatLong& Center, const SizeD& Size)
   {
		LLCenter=Center;
      LLSize=Size;
   }

   double dGetLLWidth() const
   {
      return LLSize.dW;
   }

   double dGetDist(const XYPos& P1, const XYPos& P2);

	double dGetDist(const LatLong& L1, const LatLong& L2);

   void SetRadarPos(const LatLong& Pos, const double dHeight_m=0.0);

   LatLong GetRadarPos() const
   {
      return RadarPos;
   }

	double dGetRadarHeight_m() const
	{
      return dRadarHeight_m;
   }

	double dGetGroundRa_m(const long double ldRa_m) const;

   void SetTransformType(const TRANSFORM_TYPE Trans)
   {
      TransType=Trans;
   }

   double dGetNormDist(const XYPos& LP1, const XYPos& LP2, const XYPos& P, double& dFraction);

   double dGetMinDist(const XYPos& LP1, const XYPos& LP2, const XYPos& P);

   double dGetWidth_km();
   double dGetSize_deg(const double dLat, const double dWidth_km);

	double dDistance2(const LatLong& LL1,const LatLong& LL2);


};

#endif
