//---------------------------------------------------------------------------
#ifndef JDDGraphH
#define JDDGraphH

#include "JTransform.h"
#include "JList.h"


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

class DDGraph : public Transform
{

   Pixel ScreenCenter;
   TRect Max;
   SizeI PrevSize;
   TCanvas* pPrevCanvas;

   bool bFrozen;


protected:

   struct ZoomDefStruct
   {
      LatLong Center;
      SizeD Size;
   };

   JList<ZoomDefStruct> ZoomList;

   SizeI ScreenSize;
   XYPos ZoomCenter;
   SizeD Factor;

   enum
   {
      SCREEN_DEVICE  = 0,
      PRINT_DEVICE   = 1,
      META_DEVICE    = 2} Device;

   void SetScreenSize(const SizeI Size);


public:

   DDGraph(const int nW, const int nH);
   ~DDGraph();

   void Init();

   XYPos Pix2XY(const Pixel& tp);
   Pixel XY2Pix(const XYPos& Pos);

   Polar Pix2Pol(const Pixel& pt);
   LatLong Pix2LL(const Pixel& tp);

   Pixel Pol2Pix(const Polar& Pol);

   Pixel LL2Pix(const LatLong& LL);

   Polar GetPol(const Pixel& Pt1, const Pixel& Pt2);
   XYPos GetXY(const Pixel& Pt1, const Pixel& Pt2);

   void SetCurrentZoom();
   void SetCurrentZoom(const LatLong& Center, const SizeD& Size);
   void SetZoomPos(const LatLong& Center, const SizeD& Size);
   void SetZoomPos(const LatLong& Center, const double dDiagonal);
   void GetZoomPos(LatLong& Center, SizeD& Size);
   void ZoomOut(const int nx, const int ny);
   void ZoomIn(const int nx, const int ny);
   void ZoomWin(const Pixel Start, const Pixel End);
	void ZoomNew(const LatLong& Center, const SizeD& Size);
	void Pan(const LatLong& LL, const bool bAdd2Prev=true);
	void Pan(const int nx, const int ny, const bool bAdd2Prev=true);
   void PrevZoom();

   int nGetPrevZooms() const
   {
      return ZoomList.nGetCount()-1;
	}

	//Clear all previous zooms - keep present one
	void ClearPrevZooms()
	{
		ZoomDefStruct* pZ=new ZoomDefStruct;
		*pZ=*ZoomList.pLast();
		ZoomList.Clear();
      ZoomList.nAdd(pZ);
	}

   LatLong GetLatLong(const Pixel pt);

   SizeI GetScreenSize() const
   {
      return ScreenSize;
   }

   Pixel GetScreenCenter() const
   {
      return ScreenCenter;
   }

   LatLong GetZoomCenter()
   {
      return ZoomList.pLast()->Center;
   }

   SizeD GetZoomSize()
   {
      return ZoomList.pLast()->Size;
   }

   void Freeze(){
      bFrozen=false;
   }

   void Thaw(){
      bFrozen=false;
   }

   bool bGetFrozen() const{
      return bFrozen;
   }

   HFONT CreateWinFont(HDC hDC, TFont* pFont, HFONT* phOldFont=NULL);

};

//---------------------------------------------------------------------------
#endif
