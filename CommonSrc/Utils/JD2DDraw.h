//---------------------------------------------------------------------------

#ifndef JD2DDrawH
#define JD2DDrawH

#include <Direct2D.hpp>
#include "JTransform.h"

// as long as D2D1.lib is on the library search path, it should be found
#pragma comment(lib,"D2D1.lib")


//---------------------------------------------------------------------------

/*
	This graphics module attempts target both Direct 2D and GDI graphics. By default, it attempts to use
	Direct2D and reverts to GDI of found to be unsupported. Class access is provided for usage as if it is
	always a Direct2D application, utilising GDI internally when Direct2D is not supported (e.g. on Windows XP).

	Major differences when using Direct 2D instead of GDI:
	- no more HDC, rather Window handle (TPanel->Handle)
	- no more pens or penstyles; rather brushes and strokestyles
	- ...


	Drawing Commands Implementation Pattern:
	[] foo_Drawing_Cmd(DWG_LAYER_TYPE Layer,...,...)
	{
		if (Layer==DWG_LAYER_CANVAS)
		{
			if (pC_D2D)
			{
			}
			else if (pPB_GDI)
			{
			}
		}
		else
		{
			if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			}
			else if (pBM_GDI[Layer])
			{
			}
		}
	}

*/

DWORD dwSwapRGB(DWORD dwCol)
{
	return ((dwCol & 0x000000ff) << 16 | (dwCol & 0x0000FF00) | (dwCol & 0x00FF0000) >> 16);
}


enum DWG_LAYER_TYPE
{
	DWG_LAYER_BACK_BMP		=0,
	DWG_LAYER_FRONT_BMP		=1,
	DWG_LAYER_CANVAS			=2,
};

enum D2D_BRUSH_TYPE
{
	D2D_BRUSH_TYPE_BRUSH			=0,
	D2D_BRUSH_TYPE_PEN 			=1,
	D2D_BRUSH_TYPE_TEXT			=2,

	D2D_BRUSH_TYPE_MAX_COUNT	=3,
	D2D_BRUSH_TYPE_FORCE_DWORD	=0xffffffff
};

class LJBrush_D2D
{
//	ID2D1RenderTarget* pRT;            // not created in this class
	ID2D1Factory* pD2DFactory;         // not created in this class
	ID2D1SolidColorBrush* pSolidBrush; // used for drawing on D2D surface -> Direct2D Canvas RenderTarget in this case
	IDWriteFactory* pDWFactory;    // not created in this class
	IDWriteTextFormat* pTxtFormat1; // recreated every time font properties are changed (SetFont)
	IDWriteTextLayout* pTxtLayout1; // recreated every time font properties are changed (SetFont)
	int nWidth;
	XYPos XY;
	int nClientWidth;
	int nClientHeight;
	ID2D1StrokeStyle* pStrokeStyle;
	String sTxt;
	DWRITE_TEXT_METRICS TxtMetrics; // measured distances of text for TextLayout object

	void SetStrokeStyle(const D2D1_DASH_STYLE DashStyle=D2D1_DASH_STYLE_SOLID)
	{
		if (pStrokeStyle)
			pStrokeStyle->Release();
		D2D1_STROKE_STYLE_PROPERTIES SSProp = D2D1::StrokeStyleProperties();
		SSProp.lineJoin = D2D1_LINE_JOIN_MITER;// JOIN_BEVEL;
		SSProp.startCap = D2D1_CAP_STYLE_ROUND;
		SSProp.endCap = D2D1_CAP_STYLE_ROUND;
		SSProp.dashStyle = DashStyle;
		if (pD2DFactory->CreateStrokeStyle(SSProp,
		0, // dashes
		0, // dash count
		&pStrokeStyle)!=S_OK)
		{
			pStrokeStyle=NULL;
		}
	}

	void CreateTextLayout()
	{
		if (pTxtLayout1)
			pTxtLayout1->Release();
		pDWFactory->CreateTextLayout(sTxt.c_str(),sTxt.Length(),pTxtFormat1,nClientWidth,nClientHeight,&pTxtLayout1);
		pTxtLayout1->GetMetrics(&TxtMetrics);
	}


public:
	//virtual void OnPaint_GDI();

	ID2D1SolidColorBrush* Get() const
	{
		return pSolidBrush;
	}

	LJBrush_D2D(ID2D1RenderTarget* pRT,const TColor Col,IDWriteFactory* _pDWFactory,const int _nClientWidth,const int _nClientHeight)
	{
		pStrokeStyle=NULL;
		pSolidBrush=NULL;
		assert(pRT!=NULL);
		assert(_pDWFactory!=NULL);
		pDWFactory=_pDWFactory;
		pTxtFormat1=NULL;
		pTxtLayout1=NULL;
		pRT->GetFactory(&pD2DFactory);
		nClientWidth=_nClientWidth;
		nClientHeight=_nClientHeight;
		if (pRT->CreateSolidColorBrush(D2D1::ColorF(dwSwapRGB(Col)),&pSolidBrush)!=S_OK)
			pSolidBrush=NULL;
		SetStrokeStyle(psSolid);
		//SetWidth(nWidth);
		SetXY(0,0);
		SetWidth(1);
		sTxt="";
	};

	void SetColor(const TColor Col)
	{
		pSolidBrush->SetColor(D2D1::ColorF(dwSwapRGB(Col)));
	}
	void SetWidth(const int _nWidth)
	{
		nWidth=_nWidth;
	}

	void SetXY(const XYPos _XY)
	{
		XY=_XY;
	}
	void SetXY(const double dX,const double dY)
	{
		XY.dx=dX;
		XY.dy=dY;
	}
	void SetStrokeStyle(const TBrushStyle BrushStyle)
	{
		assert(BrushStyle==bsSolid); // only solid penstyle currently implemented
		SetStrokeStyle();
	}
	void SetStrokeStyle(const TPenStyle PenStyle) // not all VCL penstyles currently implemented
	{
		//assert(PenStyle==psSolid);
		D2D1_DASH_STYLE DashStyle;
		switch (PenStyle)
		{
			case psSolid:			DashStyle=D2D1_DASH_STYLE_SOLID;				break;
			case psDot:				DashStyle=D2D1_DASH_STYLE_DOT;				break;
			case psDash:			DashStyle=D2D1_DASH_STYLE_DASH;				break;
			case psDashDot:		DashStyle=D2D1_DASH_STYLE_DASH_DOT;			break;
			case psDashDotDot:	DashStyle=D2D1_DASH_STYLE_DASH_DOT_DOT;	break;
			default: DashStyle=D2D1_DASH_STYLE_SOLID;
		}
		SetStrokeStyle(DashStyle);
	}

	void SetFont(TFont* pFont)
	{
		// Recreate a DirectWrite text format object - if requirement exists to reuse the SAME text, differently formatted, rather use D2D1TextLayout
		if (pTxtFormat1)
			pTxtFormat1->Release();

/*
   int nWeight;
   if (pFont->Style.Contains(fsBold))
      nWeight=FW_BOLD;
   else
      nWeight=FW_NORMAL;
   HFONT hFont=::CreateFont(-pFont->Size,	// logical height of font
               0,	// logical average character width
               0,	// angle of escapement
               0,	// base-line orientation angle
               nWeight,	// font weight
               pFont->Style.Contains(fsItalic),	   // italic attribute flag
               pFont->Style.Contains(fsUnderline),	// underline attribute flag
               pFont->Style.Contains(fsStrikeOut),	// strikeout attribute flag
               DEFAULT_CHARSET,	   // character set identifier
               OUT_DEFAULT_PRECIS,	// output precision
               CLIP_DEFAULT_PRECIS,	// clipping precision
					PROOF_QUALITY,	      // output quality
					DEFAULT_PITCH|FF_DONTCARE,	// pitch and family
					pFont->Name.c_str()); 	// pointer to typeface name string
*/

		DWRITE_FONT_WEIGHT FontWeight=DWRITE_FONT_WEIGHT_NORMAL;
      ::DWRITE_FONT_STYLE FontStyle=DWRITE_FONT_STYLE_NORMAL;

		if (pFont->Style.Contains(fsBold))
			FontWeight=DWRITE_FONT_WEIGHT_BOLD;

		if (pFont->Style.Contains(fsItalic))
			FontStyle=DWRITE_FONT_STYLE_ITALIC;
/*
		if (pFont->Style.Contains(fsUnderline))
			FontWeight=DWRITE_FONT_WEIGHT_BOLD;
		if (pFont->Style.Contains(fsStrikeOut))
			FontWeight=DWRITE_FONT_WEIGHT_BOLD;
*/

		if ((pDWFactory->CreateTextFormat(pFont->Name.w_str(),
      					NULL,
							FontWeight,
							FontStyle,
							DWRITE_FONT_STRETCH_MEDIUM,
							abs(pFont->Height),//*-1.0, // size in pixels
							L"", //locale
							&pTxtFormat1))!=S_OK)
			pTxtFormat1=NULL;
		// Center the text horizontally and vertically.
		if (pTxtFormat1)
		{
			pTxtFormat1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); // DWRITE_TEXT_ALIGNMENT_CENTER
			pTxtFormat1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR); // DWRITE_PARAGRAPH_ALIGNMENT_CENTER
			SetColor(pFont->Color);
			//pFont->Pitch
		}
	}

	// **** (Begin) Text Layout Functions
	void SetText(const String s)
	{
		if (sTxt!=s)
		{
			sTxt=s;
			CreateTextLayout();
		}
	}

	int nGetTextWidth() const
	{
		return TxtMetrics.width;
	}
	int nGetTextHeight() const
	{
		return TxtMetrics.height;
	}

	void DrawTextLayout(ID2D1RenderTarget* pRT,const String s)
	{
		SetText(s);
		pRT->DrawTextLayout(D2D1::Point2F(XY.dx,XY.dy),pTxtLayout1,pSolidBrush,D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
	// **** (End) Text Layout Functions

	void DrawText(ID2D1RenderTarget* pRT,const String s) // standalone function - i.e. do not mix use with TextLayout functions
	{
		//pRT->DrawTextW(sTxt.c_str(),sTxt.Length(),pTxtFormat,D2D1::RectF(XY.dx,XY.dy,XY.dx+300,XY.dy),pSolidBrush);
		sTxt=s;
		pRT->DrawTextW(sTxt.c_str(),sTxt.Length(),pTxtFormat1,D2D1::RectF(XY.dx,XY.dy,nClientWidth,nClientHeight),pSolidBrush);
	}

	void DrawLine(ID2D1RenderTarget* pRT,const int nX,const int nY)
	{
		pRT->DrawLine(D2D1::Point2F(XY.dx,XY.dy),D2D1::Point2F(nX,nY),pSolidBrush,nWidth,pStrokeStyle);
		SetXY(nX,nY);
	}

	void FillRect(ID2D1RenderTarget* pRT,const TRect R)
	{
		pRT->FillRectangle(D2D1::RectF(R.left,R.top,R.right,R.bottom),pSolidBrush);
	}

	void DrawRect(ID2D1RenderTarget* pRT,const TRect R)
	{
		pRT->DrawRectangle(D2D1::RectF(R.left,R.top,R.right,R.bottom),pSolidBrush,nWidth,pStrokeStyle);
	}

	void DrawEllipse(ID2D1RenderTarget* pRT,const int nCenterX,const int nCenterY,const int nRadiusX,const int nRadiusY)
	{
		pRT->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(nCenterX,nCenterY),nRadiusX,nRadiusY),pSolidBrush,nWidth,pStrokeStyle);
	}

	void FillEllipse(ID2D1RenderTarget* pRT,const int nCenterX,const int nCenterY,const int nRadiusX,const int nRadiusY)
	{
		pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(nCenterX,nCenterY),nRadiusX,nRadiusY),pSolidBrush);
	}

	void DrawPolygon_D2D(ID2D1RenderTarget* pRT,POINT* pPnt,const int nCnt,const bool bFill)
	{
		// http://www.codeproject.com/Articles/35890/Direct2D-Hardware-Acceleration-in-Windows-7-Plotti

		// Convert POINT to D2D1_POINT_2F
		D2D1_POINT_2F* pt =  new D2D1_POINT_2F[nCnt];
		for (int i=0;i<nCnt;i++)
		{
		  pt[i].x = (FLOAT)pPnt[i].x;
		  pt[i].y = (FLOAT)pPnt[i].y;
		}
		ID2D1PathGeometry* pg = NULL;
		ID2D1GeometrySink* pgs = NULL;
		pD2DFactory->CreatePathGeometry(&pg);
		if (pg)
		{
			pg->Open(&pgs);
			if (pgs)
			{
				D2D1_POINT_2F fb;
				fb.x = (FLOAT)pt[0].x;
				fb.y = (FLOAT)pt[0].y;
				D2D1_FIGURE_BEGIN fg;
				if (bFill)
					fg=D2D1_FIGURE_BEGIN_FILLED;
				else
					fg=D2D1_FIGURE_BEGIN_HOLLOW;
				//D2D1_FIGURE_END fe;
				//if (bClose)
				//	 fe = D2D1_FIGURE_END_CLOSED;
				//else
				//	 fe = D2D1_FIGURE_END_OPEN;
				pgs->BeginFigure(fb,fg);
				for (int i=1 ; i<nCnt; i++)
				{
					D2D1_POINT_2F fu;
					fu.x = pt[i].x;
					fu.y = pt[i].y;
					pgs->AddLine(fu);
				}
				pgs->EndFigure(D2D1_FIGURE_END_CLOSED);
				pgs->Close();
				pgs->Release();
			}
			if (bFill)
				pRT->FillGeometry(pg,pSolidBrush);
			else
				pRT->DrawGeometry(pg,pSolidBrush,1);
			pg->Release();
			delete[] pt;
		}
	}

	~LJBrush_D2D()
	{
		if (pSolidBrush)
		{
			pSolidBrush->Release();
			pSolidBrush=NULL;
		}
		if (pStrokeStyle)
		{
			pStrokeStyle->Release();
			pStrokeStyle=NULL;
		}
		if (pTxtFormat1)
		{
			pTxtFormat1->Release();
			pTxtFormat1=NULL;
		}
		if (pTxtLayout1)
		{
			pTxtLayout1->Release();
			pTxtLayout1=NULL;
		}
	};
};


class D2DDraw
{
	int nWidth;
	int nHeight;

	TDirect2DCanvas* pC_D2D;


	DWG_LAYER_TYPE CurrentLayer;

   D2D1_RENDER_TARGET_PROPERTIES RTProp_D2D;

// Bitmaps for Direct2D
	IWICImagingFactory* pWICFactory;
	IWICBitmap* pBM_WIC_D2D[2];  // front and back bitmaps
	IWICBitmapLock* pWICBitmapLock;
	ID2D1RenderTarget* pWIC_RT_D2D[2];
	DWORD** pdwWICBMData;
	BYTE* pucWicBMData;
// Bitmaps for Direct2D

	bool abBMDrawStarted_D2D[2];
	void Check_BM_RT_D2D_BeginDrawCalled(const DWG_LAYER_TYPE Layer);
//	void CreatePolygon_D2D(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt,const bool bFill);

	LJBrush_D2D* paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_MAX_COUNT];
	LJBrush_D2D* paSolidBrush_D2D[D2D_BRUSH_TYPE_MAX_COUNT];

	IDWriteFactory* pDWFactory_D2D;

	void D2D_CreateBitmapRenderTargets();
	bool bD2D_CreateWICFactory();

public:
	D2DDraw(TPanel* pPanel);
	~D2DDraw();

	bool bIsDirect2DSupported() const
	{
		return (pC_D2D!=NULL);
	}

   void SetCurrentLayer(const DWG_LAYER_TYPE Layer)
   {
   	CurrentLayer=Layer;
   }

	// NOTE: all Direct2D drawing calls must be nested within BeginDraw() and EndDraw() calls

	void BeginDraw(const DWG_LAYER_TYPE Layer);
	void BeginDraw(){ 	BeginDraw(CurrentLayer); }


	bool bEndDraw(DWG_LAYER_TYPE Layer); // Direct2D::Present() called, i.e. nothing shown on screen/bitmap up to this point
	bool bEndDraw(){ return bEndDraw(CurrentLayer);	}

	bool bDrawBitMap(DWG_LAYER_TYPE DestLayer,DWG_LAYER_TYPE SrcLayer);

	// Functions to access bitmap pixels directly (e.g. radar video overlay)
	void LockBitmap(DWG_LAYER_TYPE Layer);
	void UnlockBitmap();
	DWORD* pdwBitMapScanLine(DWG_LAYER_TYPE Layer,const int nY);

	// Basic Drawing Functions
	void SetPen(DWG_LAYER_TYPE Layer,const TColor Col,const int nWidth,const TPenStyle PenStyle=psSolid);
	void SetPen(const TColor Col,const int nWidth,const TPenStyle PenStyle=psSolid)
   {
   	SetPen(CurrentLayer,Col,nWidth,PenStyle);
   }


	void SetBrush(DWG_LAYER_TYPE Layer,const TColor Col,const TBrushStyle BrushStyle=bsSolid);
	void SetBrush(const TColor Col,const TBrushStyle BrushStyle=bsSolid)
   {
   	SetBrush(CurrentLayer,Col,BrushStyle);
   }


	void SetFont(DWG_LAYER_TYPE Layer,TFont* pFont);
	void SetFont(TFont* pFont)
   {
   	SetFont(CurrentLayer,pFont);
   }

	void DrawText(DWG_LAYER_TYPE Layer,const String sTxt,const int nX,const int nY);
	void DrawText(const String sTxt,const int nX,const int nY)
   {
   	DrawText(CurrentLayer,sTxt,nX,nY);
   }

	void MoveTo(DWG_LAYER_TYPE Layer,const int& nX,const int& nY);
	void MoveTo(const int& nX,const int& nY)
   {
   	MoveTo(CurrentLayer,nX,nY);
   }

	void LineTo(DWG_LAYER_TYPE Layer,const int& nX,const int& nY);
	void LineTo(const int& nX,const int& nY)
   {
		LineTo(CurrentLayer,nX,nY);
   }

	void LineTo(DWG_LAYER_TYPE Layer,const Pixel& pt);
	void LineTo(const Pixel& pt)
   {
   	LineTo(CurrentLayer,pt);
   }

	int nTextWidth(DWG_LAYER_TYPE Layer,const String s) const;
	int nTextWidth(const String s) const
   {
   	return nTextWidth(CurrentLayer,s);
   }

	int nTextHeight(DWG_LAYER_TYPE Layer,const String s) const;
	int nTextHeight(const String s) const
   {
   	return nTextHeight(CurrentLayer,s);
   }

   SIZE TextSize(DWG_LAYER_TYPE Layer, const String s) const
   {
   	SIZE Size;
      Size.cx=nTextWidth(Layer,s);
      Size.cy=nTextHeight(Layer,s);
      return Size;
   }

   SIZE TextSize(const String s) const
   {
   	return TextSize(CurrentLayer,s);
   }

	void FillRect(DWG_LAYER_TYPE Layer,const TRect R);
	void FillRect(const TRect R)
   {
   	FillRect(CurrentLayer,R);
   }

	void FillRect(DWG_LAYER_TYPE Layer,const TRect R,const TColor Col,const TBrushStyle BrushStyle=bsSolid);
	void FillRect(const TRect R,const TColor Col,const TBrushStyle BrushStyle=bsSolid)
   {
		FillRect(CurrentLayer,R,Col,BrushStyle);
   }

	void Rectangle(DWG_LAYER_TYPE Layer,const int nX1,const int nY1,const int nX2,const int nY2, const bool bFill);
	void Rectangle(const int nX1,const int nY1,const int nX2,const int nY2, const bool bFill)
   {
		Rectangle(CurrentLayer,nX1,nY1,nX2,nY2,bFill);
   }

	void Ellipse(DWG_LAYER_TYPE Layer,const int nCenterX,const int nCenterY,const int nRadiusX,const int nRadiusY, const bool bFill);
	void Ellipse(const int nCenterX,const int nCenterY,const int nRadiusX,const int nRadiusY, const bool bFill)
   {
		Ellipse(CurrentLayer,nCenterX,nCenterY,nRadiusX,nRadiusY,bFill);
   }

	void Ellipse(DWG_LAYER_TYPE Layer,const TRect R, bool bFill)
   {
   	TPoint p=R.CenterPoint();
      int nRad=R.Width()/2;
      Ellipse(Layer,p.x,p.y,nRad,nRad);
   }

	void Ellipse(const TRect R, bool bFill)
   {
		Ellipse(CurrentLayer,R,bFill);
   }


	void TextOutW(DWG_LAYER_TYPE Layer,const int nX,const int nY,const String s);
	void TextOutW(const int nX,const int nY,const String s)
   {
		TextOutW(CurrentLayer,nX,nY,s);
   }

	void DrawPolygonRegion(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt);
	void DrawPolygonRegion(POINT* pPnt,const int nCnt)
   {
		DrawPolygonRegion(CurrentLayer,pPnt,nCnt);
   }

	void DrawPolyLine(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt);
	void DrawPolyLine(POINT* pPnt,const int nCnt)
   {
		DrawPolyLine(CurrentLayer,pPnt,nCnt);
   }

/*
	void DrawEllipse(DWG_LAYER_TYPE Layer,const int nX1,const int nY1,const int nX2,const int nY2);
	void DrawEllipse(const int nX1,const int nY1,const int nX2,const int nY2)
   {
		DrawEllipse(CurrentLayer,nX1,nY1,nX2,nY2);
   }
*/
	TColor GetColor(const TColor Color);
	/*void MapColor(const int n, const TColor DispColor, const TColor PrintColor)
	{
		aDispColor[n]=DispColor;
		aPrintColor[n]=PrintColor;
	}*/


};



#endif
