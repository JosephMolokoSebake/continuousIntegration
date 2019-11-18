//---------------------------------------------------------------------------
#ifndef JTransGraphH
#define JTransGraphH

#include "JTransform.h"

#include <Vcl.Graphics.hpp>


#define MAX_ZOOMS    20

/*

*** Pixel is now defined in Transform.h (JGT 06/05/2003) ***

struct Pixel
{

   int nx;
   int ny;

   Pixel()
   {
      nx=0;
      ny=0;
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

   POINT ToPoint()
   {
      POINT p;
      p.x=nx;
      p.y=ny;
      return p;
   }

   void Invalidate()
   {
      nx=-100000;
   }

   bool bValid() const
   {
      return (nx!=-100000);
   }

};
*/

struct SizeI
{

   int nW;
   int nH;

   SizeI()
   {
      nW=0;
      nH=0;
   }

   SizeI(const double _dW, const double _dH)
   {
      nW=_dW;
      nH=_dH;
   }

   SizeI(const int _nW, const int _nH)
   {
      nW=_nW;
      nH=_nH;
   }

   SizeI(const SizeI& s)
   {
      nW=s.nW;
      nH=s.nH;
   }

};

class TransGraph : public Transform
{

   Pixel ScreenCenter;
   TRect Max;
   SizeI PrevSize;
   TCanvas* pPrevCanvas;

   TMetafile* pMetafile;
   TMetafileCanvas* pMetaCanvas;

   bool bFrozen;

   TColor aDispColor[48];
   TColor aPrintColor[48];
   bool bUsePrintColor;

protected:

   LatLong aCoordCenter[MAX_ZOOMS];
   SizeD aCoordSize[MAX_ZOOMS];
   int nZoomNum;
   SizeI ScreenSize;
   XYPos ZoomCenter;
   SizeD Factor;

   bool bPrinting;
   bool bMeta;

   HRGN hClipRgn;

   void SetScreenSize(const SizeI Size);

public:

   TCanvas* pC;

   void MapColor(const int n, const TColor DispColor, const TColor PrintColor)
   {
      aDispColor[n]=DispColor;
      aPrintColor[n]=PrintColor;
   }

   void SetPen(const TColor Color);
   void SetPen(const TColor Color, const int nWidth);
   void SetPen(const TColor Color, const int nWidth, const TPenStyle Style);
   void SetMode(const TPenMode Mode);

   void MoveTo(const Pixel& pt);
   void LineTo(const Pixel& pt);
   void MoveTo(const XYPos& XY);
   void LineTo(const XYPos& XY);

   void SetBrush(const TColor Color);
   void SetBrush(const TColor Color, const TBrushStyle Style);

   void Dot(const Pixel& pt, const int nSize);
   void Dot(const XYPos& XY, const int nSize);

   void SetFont(const String sName, const int nSize, const TColor Color);
   void SetFont(TFont* pFont);

   TransGraph(TCanvas* pCanvas, const int nW, const int nH);
   ~TransGraph();

   void Init();

   XYPos Pix2XY(const Pixel& tp);
   Pixel XY2Pix(const XYPos& Pos);

   Polar Pix2Pol(const Pixel& pt);
   LatLong Pix2LL(const Pixel& tp);

	Pixel Pol2Pix(const Polar& Pol);

	Pixel Pol2Pix(const double dRa_m, const double dBe_deg)
	{
		return Pol2Pix(Polar(dRa_m,dBe_deg));
	}

   Pixel LL2Pix(const LatLong& LL);

   Polar GetPol(const Pixel& Pt1, const Pixel& Pt2);
	XYPos GetXY(const Pixel& Pt1, const Pixel& Pt2);

	void SetZoomPos(const LatLong& Center, const SizeD& Size);
	void GetZoomPos(LatLong& Center, SizeD& Size);
	void SetZoomPos(const LatLong& Center, const double dWidth_deg);
	void GetZoomPos(LatLong& Center, double& dWidth_deg);
   void ZoomOut(const Pixel& Pix);
   void ZoomOut(const int nx, const int ny);
   void ZoomIn(const int nx, const int ny);
   void ZoomIn(const Pixel& Pix);
   void ZoomWin(const Pixel& Start, const Pixel& End);
   void Zoom(const LatLong& Center, const SizeD& Size);
   void Pan(const int nx, const int ny);
   void Pan(const Pixel& Pix);
   void PrevZoom();

   int nGetPrevZooms() const
   {
      return nZoomNum;
   }

   double dGetPixelRatio() const
   {
      return (ScreenSize.nW*1.0)/ScreenSize.nH;
   }


   LatLong GetLatLong(const Pixel pt);

   void UsePrintColor(const bool bUse)
   {
      bUsePrintColor=bUse;
   }

   void SetPrint(const bool bP, const int nLeft, const int nTop, const int nWidth, const int nHeight, const bool bCentered);

   bool bGetPrinting() const
   {
      return bPrinting;
   }

   SizeI GetScreenSize() const
   {
      return ScreenSize;
   }

   Pixel GetScreenCenter() const
   {
      return ScreenCenter;
   }

   void SetMetaFile(const bool bM);

   LatLong GetZoomCenter() const
   {
      return aCoordCenter[nZoomNum];
   }

   SizeD GetZoomSize() const
   {
      return aCoordSize[nZoomNum];
   }

   void Freeze()
   {
      bFrozen=false;
   }

   void Thaw()
   {
      bFrozen=false;
   }

   bool bGetFrozen() const
   {
      return bFrozen;
   }

   TColor GetColor(const TColor Color);


};

//---------------------------------------------------------------------------
#endif
