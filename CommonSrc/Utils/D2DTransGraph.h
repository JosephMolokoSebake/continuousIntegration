//---------------------------------------------------------------------------
#ifndef D2DTransGraphH
#define D2DTransGraphH

#include "JTransform.h"
#include "JD2DDraw.h"


#define MAX_ZOOMS    20


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

class TransGraph : public Transform, public D2DDraw
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
	TransGraph(TPanel* pPanel);
	~TransGraph();

	void Init();


   /*
	void Draw(TPaintBox* pPB,Graphics::TBitmap* pBM);

	void SetPen(TPaintBox* pPB,const TColor Color);
	void SetPen(TPaintBox* pPB,const TColor Color, const int nWidth);
	void SetPen(TPaintBox* pPB,const TColor Color, const int nWidth, const TPenStyle Style);
	void SetMode(TPaintBox* pPB,const TPenMode Mode);

	void SetBrush(TPaintBox* pPB,const TColor Color);
	void SetBrush(TPaintBox* pPB,const TColor Color, const TBrushStyle Style);

	void SetFont(TPaintBox* pPB,const String sName, const int nSize, const TColor Color);
	void SetFont(TPaintBox* pPB,TFont* pFont);

	void MoveTo(TPaintBox* pPB,const int& nX,const int& nY);
	void MoveTo(TPaintBox* pPB,const Pixel& pt);
	void MoveTo(TPaintBox* pPB,const XYPos& XY);
   void LineTo(TPaintBox* pPB,const int& nX,const int& nY);
	void LineTo(TPaintBox* pPB,const Pixel& pt);
	void LineTo(TPaintBox* pPB,const XYPos& XY);


	void Dot(TPaintBox* pPB,const Pixel& pt, const int nSize);
	void Dot(TPaintBox* pPB,const XYPos& XY, const int nSize);*/


	XYPos Pix2XY(const Pixel& tp);
	Pixel XY2Pix(const XYPos& Pos);

	Polar Pix2Pol(const Pixel& pt);
	LatLong Pix2LL(const Pixel& tp);

	Pixel Pol2Pix(const Polar& Pol);

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
	void Pan(const int nx, const int ny, const bool bAdd2Prev=true);
	void Pan(const Pixel& Pix, const bool bAdd2Prev=true);
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

	/*
	TColor GetColor(const TColor Color);
	void MapColor(const int n, const TColor DispColor, const TColor PrintColor)
	{
		aDispColor[n]=DispColor;
		aPrintColor[n]=PrintColor;
	}
    */
};

//---------------------------------------------------------------------------
#endif
