/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	CoastRad DirectDraw Graphics
 *		PROJECT			CoastRad C1152
 *		ACTIVITY       3
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *
************************************************************************///---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JDDGraph.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


DDGraph::DDGraph(const int nW, const int nH)
{
   SetScreenSize(SizeI(nW,nH));
   Device=SCREEN_DEVICE;
   bFrozen=false;
   Init();
}

DDGraph::~DDGraph()
{
}

void DDGraph::Init()
{
   SetZoomPos(LatLong(-34.0,18.0),SizeD(1.0,1.0));
   SetLLRef(LatLong(-34.0,18.0));
}

void DDGraph::SetScreenSize(const SizeI Size)
{
   ScreenSize=Size;
   ScreenCenter=Pixel(Size.nW/2,Size.nH/2);
   Max.Left=-32767;
   Max.Right=32767;
   Max.Top=-32767;
   Max.Bottom=32767;
}

Pixel DDGraph::XY2Pix(const XYPos& Pos)
{
	int nx=(Pos.dx-ZoomCenter.dx)*Factor.dW+ScreenCenter.nx;
	int ny=ScreenCenter.ny-(Pos.dy-ZoomCenter.dy)*Factor.dH;
	if (nx<Max.Left)
		nx=Max.Left;
	else if (nx>Max.Right)
		nx=Max.Right;
	if (ny<Max.Top)
		ny=Max.Top;
	else if (ny>Max.Bottom)
		ny=Max.Bottom;
	return Pixel(nx,ny);
}

XYPos DDGraph::Pix2XY(const Pixel& tp)
{
   double dx=(tp.nx-ScreenCenter.nx)/Factor.dW+ZoomCenter.dx;
   double dy=(ScreenCenter.ny-tp.ny)/Factor.dH+ZoomCenter.dy;
   return XYPos(dx,dy);
}

Polar DDGraph::Pix2Pol(const Pixel& pt)
{
   return XY2Pol(Pix2XY(pt));
}

LatLong DDGraph::Pix2LL(const Pixel& tp)
{
   return XY2LL(Pix2XY(tp));
}

Pixel DDGraph::Pol2Pix(const Polar& Pol)
{
   return XY2Pix(Pol2XY(Pol));
}

Pixel DDGraph::LL2Pix(const LatLong& LL)
{
   return XY2Pix(LL2XY(LL));
}

void DDGraph::SetCurrentZoom()
{
   if (ZoomList.nGetCount()>0)
      {
      SetLLView(ZoomList.pLast()->Center,ZoomList.pLast()->Size);
      FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
      }
}

void DDGraph::SetCurrentZoom(const LatLong& Center, const SizeD& Size)
{
   if (ZoomList.nGetCount()>0)
      {
      ZoomList.pLast()->Center=Center;
      ZoomList.pLast()->Size=Size;
      SetCurrentZoom();
      }
}

void DDGraph::SetZoomPos(const LatLong& Center, const SizeD& Size)
{
   ZoomList.Clear();
   ZoomDefStruct* pZ=new ZoomDefStruct;
   pZ->Center=Center;
   pZ->Size=Size;
   ZoomList.nAdd(pZ);
   SetCurrentZoom();
}

void DDGraph::SetZoomPos(const LatLong& Center, const double dDiagonal)
{
   SetZoomPos(Center,SizeD(dDiagonal*0.8,0.0));
}

void DDGraph::ZoomNew(const LatLong& Center, const SizeD& Size)
{
   ZoomDefStruct* pZ=new ZoomDefStruct;
   pZ->Center=Center;
   pZ->Size=Size;
   ZoomList.nAdd(pZ);
   SetCurrentZoom();
}

void DDGraph::GetZoomPos(LatLong& Center, SizeD& Size)
{
   Center=ZoomList.pLast()->Center;
   Size=ZoomList.pLast()->Size;
}

void DDGraph::Pan(const LatLong& LL, const bool bAdd2Prev)
{
	if (bAdd2Prev)
		{
		ZoomDefStruct* pZ=new ZoomDefStruct;
		pZ->Center=LL;
		pZ->Size=ZoomList.pLast()->Size;
		ZoomList.nAdd(pZ);
		SetCurrentZoom();
		}
	else
		{
		SetLLView(LL,ZoomList.pLast()->Size);
		FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
		}
}

void DDGraph::Pan(const int nx, const int ny, const bool bAdd2Prev)
{
	LatLong Center=Pix2LL(Pixel(nx,ny));
	Pan(Center,bAdd2Prev);
}

void DDGraph::ZoomIn(const int nx, const int ny)
{
   LatLong Center=Pix2LL(Pixel(nx,ny));
   ZoomDefStruct* pZ=new ZoomDefStruct;
   pZ->Center=Center;
   pZ->Size=ZoomList.pLast()->Size;
   pZ->Size.dW/=2.0;
   pZ->Size.dH/=2.0;
   ZoomList.nAdd(pZ);
   SetCurrentZoom();
}

void DDGraph::ZoomOut(const int nx, const int ny)
{
   LatLong Center=Pix2LL(Pixel(nx,ny));
   ZoomDefStruct* pZ=new ZoomDefStruct;
   pZ->Center=Center;
   pZ->Size=ZoomList.pLast()->Size;
   pZ->Size.dW*=2.0;
   pZ->Size.dH*=2.0;
   ZoomList.nAdd(pZ);
   SetCurrentZoom();
}

void DDGraph::ZoomWin(const Pixel Start, const Pixel End)
{
   if ((abs(Start.nx-End.nx)>2)&&(abs(Start.ny-End.ny)>2))
      {
      LatLong Center=Pix2LL(Pixel((Start.nx+End.nx)/2,(Start.ny+End.ny)/2));
      LatLong StartC=Pix2LL(Start);
      LatLong EndC=Pix2LL(End);
      ZoomDefStruct* pZ=new ZoomDefStruct;
      pZ->Center=Center;
      pZ->Size=SizeD(fabs(StartC.dLong-EndC.dLong),fabs(StartC.dLat-EndC.dLat));
      if (pZ->Size.dW<0.001) pZ->Size.dW=0.001; //about 10 m
      if (pZ->Size.dH<0.001) pZ->Size.dH=0.001; //about 10 m
      ZoomList.nAdd(pZ);
      SetCurrentZoom();
      }
}


void DDGraph::PrevZoom()
{
   if (ZoomList.nGetCount()>1)
      {
      ZoomList.Delete(ZoomList.nGetCount()-1);
      SetCurrentZoom();
      }
}

LatLong DDGraph::GetLatLong(const Pixel pt)
{
   return XY2LL(Pix2XY(pt));
}

Polar DDGraph::GetPol(const Pixel& Pt1, const Pixel& Pt2)
{                                                                
   return Transform::GetPol(Pix2XY(Pt1),Pix2XY(Pt2));
}

HFONT DDGraph::CreateWinFont(HDC hDC, TFont* pFont, HFONT* phOldFont)
{
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
   if (phOldFont)
		*phOldFont=(HFONT)::SelectObject(hDC,hFont);
   else
      ::SelectObject(hDC,hFont);
//   ::SetTextColor(hDC,Config.GetColor(pFont->Color));
   ::SetTextColor(hDC,pFont->Color);
   return hFont;
}

