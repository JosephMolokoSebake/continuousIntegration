/************** CONFIGURATION HEADER ************************************
MODULE      General Navigational Transformations
COMPILER    Borland C++ Builder 6
PROCESSOR   Pentium IV
OS				MS Windows XP
PROGRAMMER  Johan Theron
CHANGE HISTORY
   1. Version 1.0.0.00  : Initial Version JGT 05/02/2007

************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JTransform.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

String LatLong::sLL() const
{
	return sLat()+" "+sLong();
}

String LatLong::sLat(const bool bGPSFormat) const
{
	if (bValidLat())
		{
		char c;
		if (dLat<0.0) c='S'; else c='N';
		double dL=fabs(dLat);
		int nDeg=floor(dL);
		double dMin=(dL-floor(dL))*60.0;
		int nMin=floor(dMin);
		int nFrac=floor((dMin-floor(dMin))*1000.0+0.5);
		if (nFrac>999)
			{
			nFrac=0;
			nMin++;
			}
		String s;
		if (bGPSFormat)
			return s.sprintf(L"%02d%02d.%03d,%c",nDeg,nMin,nFrac,c);
		else
			return s.sprintf(L"%02d:%02d.%03d%c",nDeg,nMin,nFrac,c);
		}
	else
		return "";
}

String LatLong::sLong(const bool bGPSFormat) const
{
	if (bValidLong())
		{
		char c;
		if (dLong<0.0) c='W'; else c='E';
		double dL=fabs(dLong);
		int nDeg=floor(dL);
		double dMin=(dL-nDeg)*60.0;
		int nMin=floor(dMin);
		int nFrac=floor((dMin-nMin)*1000.0+0.5);
		if (nFrac>999)
			{
			nFrac=0;
			nMin++;
			}
		String s;
		if (bGPSFormat)
			return s.sprintf(L"%02d%02d.%03d,%c",nDeg,nMin,nFrac,c);
		else
			return s.sprintf(L"%02d:%02d.%03d%c",nDeg,nMin,nFrac,c);
		}
	else
		return "";
}

int LatLong::nGetLatDeg() const
{
	return floor(fabs(dLat));
}

double LatLong::dGetLatMinutes() const
{
   double dL=fabs(dLat);
   double dMin=(dL-floor(dL))*60.0;
   return dMin;
}

char LatLong::cGetLatHemi() const
{
   if (dLat<0.0)
      return 'S';
   else
      return 'N';
}

int LatLong::nGetLongDeg() const
{
   return floor(fabs(dLong));
}

double LatLong::dGetLongMinutes() const
{
   double dL=fabs(dLong);
   int nDeg=floor(dL);
   double dMin=(dL-nDeg)*60.0;
   return dMin;
}

char LatLong::cGetLongHemi() const
{
	if (dLong<0.0)
		return 'W';
	else
		return 'E';
}

Transform::Mercator::Mercator()
{
	dRefLat_deg=0.0;
	dRefLong_deg=0.0;

	dXRadius_m=6378137;
	dYRadius_m=6378137;
}

Transform::Mercator::~Mercator()
{

}

void Transform::Mercator::LL2XY(const double dLat_deg, const double dLong_deg, double& dx_m, double& dy_m)
{
	//Standard Mercator
	double dx_rad=dDeg2Rad(dLong_deg-dRefLong_deg);
	if (dx_rad>M_PI)
		dx_rad-=2.0*M_PI;
	double dL_deg=dLat_deg;
	if (dL_deg>85.0)
		dL_deg=85.0;
	else if (dL_deg<-85.0)
		dL_deg=-85.0;
	double dy_rad=log(tan(M_PI/4.0+dDeg2Rad(dL_deg)/2.0));
	if (dy_rad>M_PI)
		dy_rad-=2.0*M_PI;
	dx_m=dx_rad*dXRadius_m;
	dy_m=dy_rad*dYRadius_m;
}

void Transform::Mercator::XY2LL(const double dx_m, const double dy_m, double &dLat_deg, double& dLong_deg)
{
	double dx_rad=dx_m/dXRadius_m;
	double dy_rad=dy_m/dYRadius_m;
	dLong_deg=dRad2Deg(dx_rad)+dRefLong_deg;
	double dE=exp(dy_rad);
	double dLat_rad=2.0*atan(dE)-M_PI/2.0;
	dLat_deg=dRad2Deg(dLat_rad);
}




Transform::Transform()
{
	TransType=ROUND_TRANS;
	Projection=SPHERICAL;
	pMerc=NULL;
	dDeg2Rad=M_PI/180.0;
	dRad2M=6372850.0;
	dDeg2M=111227.2;
	LLCenter=LatLong(0.0,0.0);
	SetLLRef(LatLong(0.0,0.0));
	LLSize=SizeD(1.0,1.0);
	SetRadarPos(LatLong(0.0,0.0),0.0);
	bValidTrans=false;
}

void Transform::SetLLRef(const LatLong& Pos)
{
	LLRef=Pos;
	dCosLatRef=cos((90.0-Pos.dLat)*dDeg2Rad);  //All Lat measured from North Pole
	dSinLatRef=sin((90.0-Pos.dLat)*dDeg2Rad);
}

void Transform::SetRadarPos(const LatLong& Pos, const double dHeight_m)
{
	RadarPos=Pos;
	dRadarHeight_m=dHeight_m;
	dCosLatRad=cos((90.0-Pos.dLat)*dDeg2Rad);  //All Lat measured from North Pole
	dSinLatRad=sin((90.0-Pos.dLat)*dDeg2Rad);

	const long double ldA_m=6378137.0;
	long double ldB_m=6378137.0+dRadarHeight_m;
	ldCosRulePar1=ldSqr(ldA_m)+ldSqr(ldB_m);
	ldCosRulePar2=ldA_m*ldB_m*2.0;

	bValidTrans=true;

}

double Transform::dGetGroundRa_m(const long double ldRa_m) const
{
	long double ldTheta=ldACOS((ldCosRulePar1-ldSqr(ldRa_m))/ldCosRulePar2);
	return 6378137.0*ldTheta;
/*
	long double ldCosTheta=(ldCosRulePar1-ldSqr(ldRa_m))/ldCosRulePar2;
	const long double ldA_m=6378137.0;
	const long double ldC_m=2.0*ldSqr(ldA_m)-2.0*ldSqr(ldA_m)*ldCosTheta;
	return sqrtl(ldC_m);
*/
}

double Transform::dGetWidth_km()
{
	double dDeg2MLong=dDeg2M*cos(LLCenter.dLat*dDeg2Rad);
   return LLSize.dW*dDeg2MLong*0.001;
}

double Transform::dGetSize_deg(const double dLat, const double dWidth_km)
{
	double dDeg2MLong=dDeg2M*cos(dLat*dDeg2Rad);
   return dWidth_km/(dDeg2MLong*0.001);
}

void Transform::FixAspect(const int nWidth, const int nHeight, SizeD& Factor, XYPos& ZoomCenter)
{
	double dDeg2MLong=dDeg2M*cos(LLCenter.dLat*dDeg2Rad);
   double dx=LLSize.dW*dDeg2MLong;
   double dy=LLSize.dH*dDeg2M;
   double dXYRatio;
   if (dy<1e-10)
      dXYRatio=1000.0;
   else
      dXYRatio=dx/dy;
   double dPixelRatio=(nWidth*1.0)/nHeight;
   if (dXYRatio>dPixelRatio)
      {
      dy=dx/dPixelRatio;
      LLSize.dH=dy/dDeg2M;
      }
   else
      {
      dx=dy*dPixelRatio;
      LLSize.dW=dx/dDeg2MLong;
      }
   ZoomCenter=LL2XY(LLCenter);
   Factor.dW=nWidth/(LLSize.dW*dDeg2MLong);
   Factor.dH=nHeight/(LLSize.dH*dDeg2M);
}

LatLong Transform::Pol2LL(const Polar& Pol)
{
   if (TransType==FLAT_TRANS)
      {
      double dx=Pol.dRa*sin(Pol.dBe*dDeg2Rad);
      double dy=Pol.dRa*cos(Pol.dBe*dDeg2Rad);
      double dLat=dy/dDeg2M+RadarPos.dLat;
      double dAvLat=(dLat+RadarPos.dLat)/2.0;
      double dLong=RadarPos.dLong+dx/(dDeg2M*cos(dAvLat*dDeg2Rad));
      return LatLong(dLat,dLong);
      }
   else
      {
		double dRt=Pol.dRa/dRad2M;  //in Rad
      double dCosRt=cos(dRt);
      double dSinRt=sin(dRt);
      double dCosLatT=dCosRt*dCosLatRad+dSinRt*dSinLatRad*cos(Pol.dBe*dDeg2Rad);
      double dLatT=dACOS(dCosLatT);
      double dCosDeltaLong=(dCosRt-dCosLatT*dCosLatRad)/(sin(dLatT)*dSinLatRad);
      double dDeltaLong;
      if (Pol.dBe>180.0)
         dDeltaLong=-dACOS(dCosDeltaLong);
      else
         dDeltaLong=dACOS(dCosDeltaLong);
      double dLongT=RadarPos.dLong+dDeltaLong/dDeg2Rad;
      dLatT/=dDeg2Rad;
      return LatLong(90.0-dLatT,dLongT);
      }
}

Polar Transform::LL2Pol(const LatLong& LL)
{
   if (TransType==FLAT_TRANS)
      {
      double dAvLat=(LL.dLat+RadarPos.dLat)/2.0;
      double dy=dDeg2M*(LL.dLat-RadarPos.dLat);
      double dx=dDeg2M*cos(dAvLat*dDeg2Rad)*(LL.dLong-RadarPos.dLong);
      double dRa=sqrt(dx*dx+dy*dy);
      double dBe;
      if (fabs(dx)>1e-9)
         dBe=atan2(dx,dy)/dDeg2Rad;
      else if (dy<0.0)
         dBe=180.0;
      else
         dBe=0.0;
      if (dBe<0.0) dBe+=360.0;
      return Polar(dRa,dBe);
      }
   else
      {
      double dCosLatT=cos((90.0-LL.dLat)*dDeg2Rad);
      double dSinLatT=sin((90.0-LL.dLat)*dDeg2Rad);
		double dCosDeltaLong=cos((RadarPos.dLong-LL.dLong)*dDeg2Rad);
		double dCosRa=dCosLatRad*dCosLatT+dSinLatRad*dSinLatT*dCosDeltaLong;
		double dRa=dACOS(dCosRa);
		double dBe;
		if (dRa<1e-15)
			dBe=0.0;
		else
			{
			double dCosBe=(dCosLatT-dCosRa*dCosLatRad)/(sin(dRa)*dSinLatRad);
			dBe=dACOS(dCosBe)/dDeg2Rad;
			}
		dRa*=dRad2M;
		if (LL.dLong<RadarPos.dLong)
			dBe=360.0-dBe;
		return Polar(dRa,dBe);
		}
}

XYPos Transform::LL2XY(const LatLong& LL)
{
	XYPos XY;
	if (pMerc)
		{
		pMerc->LL2XY(LL.dLat,LL.dLong,XY.dx,XY.dy);
		}
	else
		{
		if (TransType==FLAT_TRANS)
			{
			XY.dy=(LL.dLat-LLRef.dLat)*dDeg2M;
			double dAvLat=(LL.dLat+LLRef.dLat)/2.0;
			double dK=cos(dAvLat*dDeg2Rad);
			XY.dx=(LL.dLong-LLRef.dLong)*dDeg2M*dK;
			}
		else
			{
			XY.dy=-(LLRef.dLat-LL.dLat)*dDeg2M;
			double dSinLatT=sin((90.0-LL.dLat)*dDeg2Rad);
			double dCosLatT=cos((90.0-LL.dLat)*dDeg2Rad);
			double dDeltaLong=(LLRef.dLong-LL.dLong)*dDeg2Rad;
			double dCosX=dCosLatT*dCosLatT+dSinLatT*dSinLatT*cos(dDeltaLong);
			XY.dx=dACOS(dCosX)*dRad2M;
			if (LL.dLong<LLRef.dLong)
				XY.dx*=-1.0;
			}
		}
	return XY;
}

LatLong Transform::XY2LL(const XYPos& XY)
{
	LatLong LL;
	if (pMerc)
		{
		pMerc->XY2LL(XY.dx,XY.dy,LL.dLat,LL.dLong);

		}
	else
		{
		if (TransType==FLAT_TRANS)
			{
			LL.dLat=LLRef.dLat+XY.dy/dDeg2M;
			double dAvLat=(LL.dLat+LLRef.dLat)/2.0;
			double dK=cos(dAvLat*dDeg2Rad);
			LL.dLong=LLRef.dLong+XY.dx/(dDeg2M*dK);
			}
		else
			{
			LL.dLat=LLRef.dLat+XY.dy/dDeg2M;
			double dCosLatT=cos((90.0-LL.dLat)*dDeg2Rad);
			double dSinLatT=sin((90.0-LL.dLat)*dDeg2Rad);
			double dCosDeltaLong=(cos(XY.dx/dRad2M)-dSqr(dCosLatT))/dSqr(dSinLatT);
			double dDeltaLong;
			if (XY.dx<0.0)
				dDeltaLong=-dACOS(dCosDeltaLong);
			else
				dDeltaLong=dACOS(dCosDeltaLong);
			LL.dLong=LLRef.dLong+dDeltaLong/dDeg2Rad;
			}
		}
	return LL;
}

Polar Transform::XY2Pol(const XYPos& XY)
{
	return LL2Pol(XY2LL(XY));
}

XYPos Transform::Pol2XY(const Polar& Pol)
{
	return LL2XY(Pol2LL(Pol));
}

XYPos Transform::GetXY(const LatLong& LL1, const LatLong& LL2)
{
	if (TransType==FLAT_TRANS)
		{
		XYPos XY1=LL2XY(LL1);
		XYPos XY2=LL2XY(LL2);
		return XYPos(XY2.dx-XY1.dx,XY2.dy-XY1.dy);
		}
	else
		{
		double dy=-(LL1.dLat-LL2.dLat)*dDeg2M;
		double dSinLatT=sin((90.0-LL2.dLat)*dDeg2Rad);
		double dCosLatT=cos((90.0-LL2.dLat)*dDeg2Rad);
		double dDeltaLong=(LL1.dLong-LL2.dLong)*dDeg2Rad;
		double dCosX=dSqr(dCosLatT)+dSqr(dSinLatT)*cos(dDeltaLong);
		double dx=dACOS(dCosX);
		if (LL2.dLong<LL1.dLong)
			dx*=-1.0;
		dx*=dRad2M;
		return XYPos(dx,dy);
		}
}

XYPos Transform::GetXY(const XYPos& XY1, const XYPos& XY2)
{
   return GetXY(XY2LL(XY1),XY2LL(XY2));
}

Polar Transform::GetPol(const LatLong& LL1, const LatLong& LL2)
{
   if (TransType==FLAT_TRANS)
		{
      XYPos XY=GetXY(LL1,LL2);
      double dRa=sqrt(XY.dx*XY.dx+XY.dy*XY.dy);
      double dBe;
      if (fabs(XY.dx)>1e-9)
         dBe=atan2(XY.dx,XY.dy)/dDeg2Rad;
      else if (XY.dy<0.0)
         dBe=180.0;
      else
         dBe=0.0;
      if (dBe<0.0) dBe+=360.0;
      return Polar(dRa,dBe);
      }
   else
      {
      double dCosLatT=cos((90.0-LL2.dLat)*dDeg2Rad);
      double dSinLatT=sin((90.0-LL2.dLat)*dDeg2Rad);
      double dCosLatR=cos((90.0-LL1.dLat)*dDeg2Rad);
      double dSinLatR=sin((90.0-LL1.dLat)*dDeg2Rad);
      double dCosDeltaLong=cos((LL1.dLong-LL2.dLong)*dDeg2Rad);
      double dCosRa=dCosLatR*dCosLatT+dSinLatR*dSinLatT*dCosDeltaLong;
      double dRa=dACOS(dCosRa);
      double dBe;
      if (dRa<1e-15)
         dBe=0.0;
      else{
         double dCosBe=(dCosLatT-dCosRa*dCosLatR)/(sin(dRa)*dSinLatR);
         dBe=dACOS(dCosBe)/dDeg2Rad;
         }
      dRa*=dRad2M;
      if (LL2.dLong<LL1.dLong)
         dBe=360.0-dBe;
      return Polar(dRa,dBe);
      }
}

Polar Transform::GetPol(const XYPos& XY1, const XYPos& XY2)
{
   return GetPol(XY2LL(XY1),XY2LL(XY2));
}

double Transform::dGetDist(const XYPos& P1, const XYPos& P2)
{
   return sqrt(dSqr(P1.dx-P2.dx)+dSqr(P1.dy-P2.dy));
}

double Transform::dGetDist(const LatLong& L1, const LatLong& L2)
{
   return dGetDist(LL2XY(L1),LL2XY(L2));
}

double Transform::dGetNormDist(const XYPos& LP1, const XYPos& LP2, const XYPos& P, double& dFraction)
{
// Determines the distance from point P to the line through LP1 and LP2
// If dFraction > 1, then the closest point is outside the segment on LP2's side
// If dFraction < 0, then the closest point is outside the segment on LP1's side
   double dDX=P.dx-LP1.dx;
   double dDY=P.dy-LP1.dy;
   double dDX2=LP2.dx-LP1.dx;
   double dDY2=LP2.dy-LP1.dy;
   double dL=sqrt(dSqr(dDX)+dSqr(dDY));
   if (dL<0.0001) return 0.0;
   double dL2=sqrt(dSqr(dDX2)+dSqr(dDY2));
   if (dL2<0.0001) return 0.0;
   //Take dot product between two vectors to determine the angle between them
   double dCosThet=(dDX*dDX2+dDY*dDY2)/(dL*dL2);
   double dSinThet;
   if (dCosThet>1.0)
      if (dCosThet<1.001)
			dSinThet=0.0;
		else
			dSinThet=0.0;  //Error
	else
		dSinThet=sqrt(1.0-dSqr(dCosThet));
	double dDist=dSinThet*dL;
	dFraction=(dCosThet*dL)/dL2;
	return dDist;
}

double Transform::dGetMinDist(const XYPos& LP1, const XYPos& LP2, const XYPos& P)
{
// Determines the minimum distance from point P to the line from LP1 to LP2
	double dDX=P.dx-LP1.dx;
	double dDY=P.dy-LP1.dy;
	double dDX2=LP2.dx-LP1.dx;
	double dDY2=LP2.dy-LP1.dy;
	double dL=sqrt(dSqr(dDX)+dSqr(dDY));
	if (dL<0.0001) return 0.0;
	double dL2=sqrt(dSqr(dDX2)+dSqr(dDY2));
	if (dL2<0.0001) return 0.0;
	//Take dot product between two vectors to determine the angle between them
	double dCosThet=(dDX*dDX2+dDY*dDY2)/(dL*dL2);
	double dSinThet;
	if (dCosThet>1.0)
		if (dCosThet<1.001)
			dSinThet=0.0;
		else
			dSinThet=0.0;  //Error
	else
		dSinThet=sqrt(1.0-dSqr(dCosThet));
// If dFraction > 1, then the closest point is outside the segment on LP2's side
// If dFraction < 0, then the closest point is outside the segment on LP1's side
	double dFraction=(dCosThet*dL)/dL2;
	double dDist;
	if (dFraction<0.0)
		dDist=dL;   //Distance from P to LP1
	else if (dFraction>1.0)
		dDist=sqrt(dSqr(P.dx-LP2.dx)+dSqr(P.dy-LP2.dy)); //Distance from P to LP2
	else
		dDist=dSinThet*dL;   //Distance from P to line
	return dDist;
}

double Transform::dDistance2(const LatLong& LL1,const LatLong& LL2)
{
	double dTheta=LL1.dLong-LL2.dLong;
	double dTheta_rad=dTheta*M_PI/180.0;
	double dLat1_rad=LL1.dLat*M_PI/180.0;
//	double dLong1_rad=LL1.dLong*M_PI/180.0;
	double dLat2_rad=LL2.dLat*M_PI/180.0;
//	double dLong2_rad=LL2.dLong*M_PI/180.0;
	double dDist=sin(dLat1_rad)*sin(dLat2_rad)+cos(dLat1_rad)*cos(dLat2_rad)*cos(dTheta_rad);
	double dDist_rad=dACOS(dDist);
	double dDist_deg = dDist_rad*180.0/M_PI;
	dDist_deg = dDist_deg*60.0*1.1515;
	double dDist_m=dDist_deg*1609.344;  //in m
	return dDist_m;
}

