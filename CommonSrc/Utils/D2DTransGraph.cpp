//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JTransform.h"

#include "D2DTransGraph.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


TransGraph::TransGraph(TPanel* pPanel):D2DDraw(pPanel)
{
	pMetafile=NULL;
	pMetaCanvas=NULL;
	SetScreenSize(SizeI(pPanel->Width,pPanel->Height));
	bPrinting=false;
	bMeta=false;
	bFrozen=false;
	bUsePrintColor=false;
	Init();
}

TransGraph::~TransGraph()
{
	delete pMetafile;
	delete pMetaCanvas;
}


void TransGraph::Init()
{
   nZoomNum=0;
   aCoordCenter[0]=LatLong(-34.0,18.0);
   aCoordSize[0]=SizeD(1.0,1.0);
   SetLLView(aCoordCenter[0],aCoordSize[0]);
   SetLLRef(LatLong(-34.0,18.0));
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}

void TransGraph::SetScreenSize(const SizeI Size)
{
   ScreenSize=Size;
   ScreenCenter=Pixel(Size.nW/2,Size.nH/2);
   Max.Left=-32767;
   Max.Right=32767;
   Max.Top=-32767;
   Max.Bottom=32767;
}

Pixel TransGraph::XY2Pix(const XYPos& Pos)
{
	int nx=(Pos.dx-ZoomCenter.dx)*Factor.dW+ScreenCenter.nx;
	int ny=ScreenCenter.ny-(Pos.dy-ZoomCenter.dy)*Factor.dH;
   if (nx<-10000)
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

XYPos TransGraph::Pix2XY(const Pixel& tp)
{
   double dx=(tp.nx-ScreenCenter.nx)/Factor.dW+ZoomCenter.dx;
   double dy=(ScreenCenter.ny-tp.ny)/Factor.dH+ZoomCenter.dy;
   return XYPos(dx,dy);
}

Polar TransGraph::Pix2Pol(const Pixel& pt)
{
   return XY2Pol(Pix2XY(pt));
}

LatLong TransGraph::Pix2LL(const Pixel& tp)
{
   return XY2LL(Pix2XY(tp));
}

Pixel TransGraph::Pol2Pix(const Polar& Pol)
{
   return XY2Pix(Pol2XY(Pol));
}

Pixel TransGraph::LL2Pix(const LatLong& LL)
{
   return XY2Pix(LL2XY(LL));
}

void TransGraph::SetZoomPos(const LatLong& Center, const SizeD& Size)
{
   nZoomNum=0;
   aCoordCenter[nZoomNum]=Center;
   aCoordSize[nZoomNum]=Size;
   SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}

void TransGraph::GetZoomPos(LatLong& Center, SizeD& Size)
{
   Center=aCoordCenter[nZoomNum];
   Size=aCoordSize[nZoomNum];
}

void TransGraph::SetZoomPos(const LatLong& Center, const double dWidth_deg)
{
	SetZoomPos(Center,SizeD(dWidth_deg,0.0));
}

void TransGraph::GetZoomPos(LatLong& Center, double& dWidth_deg)
{
	Center=aCoordCenter[nZoomNum];
	dWidth_deg=dGetLLWidth();
}


void TransGraph::Pan(const int nx, const int ny, const bool bAdd2Prev)
{
   LatLong Center=Pix2LL(Pixel(nx,ny));
   if (bAdd2Prev)
   	{
      if (nZoomNum<MAX_ZOOMS-1)
         ++nZoomNum;
      else
         {
         for (int i=0; i<nZoomNum; i++)
            {
            aCoordCenter[i]=aCoordCenter[i+1];
            aCoordSize[i]=aCoordSize[i+1];
            }
         }
      aCoordCenter[nZoomNum]=Center;
      aCoordSize[nZoomNum]=aCoordSize[nZoomNum-1];
      SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
      FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
      }
   else
   	{
      SetLLView(Center,aCoordSize[nZoomNum]);
      FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
      }
}

void TransGraph::Pan(const Pixel& Pix, const bool bAdd2Prev)
{
   Pan(Pix.nx,Pix.ny);
}

void TransGraph::Zoom(const LatLong& Center, const SizeD& Size)
{
   if (nZoomNum<MAX_ZOOMS-1)
      nZoomNum++;
   else
      {
      for (int i=0; i<nZoomNum; i++)
         {
         aCoordCenter[i]=aCoordCenter[i+1];
         aCoordSize[i]=aCoordSize[i+1];
         }
      }
   aCoordSize[nZoomNum]=Size;
   aCoordCenter[nZoomNum]=Center;
   SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}

void TransGraph::ZoomIn(const int nx, const int ny)
{
   LatLong Center=Pix2LL(Pixel(nx,ny));
   if (nZoomNum<MAX_ZOOMS-1)
      nZoomNum++;
   else
      {
      for (int i=0; i<nZoomNum; i++)
         {
         aCoordCenter[i]=aCoordCenter[i+1];
         aCoordSize[i]=aCoordSize[i+1];
         }
      }
   aCoordSize[nZoomNum]=aCoordSize[nZoomNum-1];
   aCoordCenter[nZoomNum]=Center;
   aCoordSize[nZoomNum].dW/=2.0;
   aCoordSize[nZoomNum].dH/=2.0;
   SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}

void TransGraph::ZoomIn(const Pixel& Pix)
{
   ZoomIn(Pix.nx,Pix.ny);
}

void TransGraph::ZoomOut(const int nx, const int ny)
{
   LatLong Center=Pix2LL(Pixel(nx,ny));
   if (nZoomNum<MAX_ZOOMS-1)
      nZoomNum++;
   else
      {
      for (int i=0; i<nZoomNum; i++){
         aCoordCenter[i]=aCoordCenter[i+1];
         aCoordSize[i]=aCoordSize[i+1];
         }
      }
   aCoordCenter[nZoomNum]=Center;
   aCoordSize[nZoomNum]=aCoordSize[nZoomNum-1];
   aCoordSize[nZoomNum].dW*=2.0;
   aCoordSize[nZoomNum].dH*=2.0;
   SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}

void TransGraph::ZoomOut(const Pixel& Pix)
{
   ZoomOut(Pix.nx,Pix.ny);
}

void TransGraph::ZoomWin(const Pixel& Start, const Pixel& End)
{
   LatLong Center=Pix2LL(Pixel((Start.nx+End.nx)/2,(Start.ny+End.ny)/2));
   LatLong StartC=Pix2LL(Start);
   LatLong EndC=Pix2LL(End);
   if (nZoomNum<MAX_ZOOMS-1)
      nZoomNum++;
   else
      {
      for (int i=0; i<nZoomNum; i++)
         {
         aCoordCenter[i]=aCoordCenter[i+1];
         aCoordSize[i]=aCoordSize[i+1];
         }
      }
   aCoordCenter[nZoomNum]=Center;
   aCoordSize[nZoomNum]=SizeD(fabs(StartC.dLong-EndC.dLong),fabs(StartC.dLat-EndC.dLat));
   SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
   FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
}


void TransGraph::PrevZoom()
{
   if (nZoomNum>0)
      {
      nZoomNum--;
      SetLLView(aCoordCenter[nZoomNum],aCoordSize[nZoomNum]);
      FixAspect(ScreenSize.nW,ScreenSize.nH,Factor,ZoomCenter);
      }
}
/*
void TransGraph::SetPen(TPaintBox* pPB,const TColor Color)
{
	if (pC_D2D)
		pC_D2D->Pen->Color=GetColor(Color);
	else
		pPB->Canvas->Pen->Color=GetColor(Color);
}

void TransGraph::SetPen(TPaintBox* pPB,const TColor Color, const int nWidth)
{
	if (pC_D2D)
	{
		pC_D2D->Pen->Color=GetColor(Color);
		pC_D2D->Pen->Width=nWidth;
	}
	else
	{
		pPB->Canvas->Pen->Color=GetColor(Color);
		pPB->Canvas->Pen->Width=nWidth;
   }
}

void TransGraph::SetPen(TPaintBox* pPB,const TColor Color, const int nWidth, const TPenStyle Style)
{
	if (pC_D2D)
	{
		pC_D2D->Pen->Color=GetColor(Color);
		pC_D2D->Pen->Width=nWidth;
		pC_D2D->Pen->Style=Style;
	}
	else
	{
		pPB->Canvas->Pen->Color=GetColor(Color);
		pPB->Canvas->Pen->Width=nWidth;
		pPB->Canvas->Pen->Style=Style;
	}
}

void TransGraph::SetMode(TPaintBox* pPB,const TPenMode Mode)
{
	if (pC_D2D)
	{
		//pC_D2D->Pen->StrokeStyle?? NB cannot be used when "Style" is used
	}
	else
		pPB->Canvas->Pen->Mode=Mode;
}

void TransGraph::SetBrush(TPaintBox* pPB,const TColor Color)
{
	if (pC_D2D)
		pC_D2D->Brush->Color=GetColor(Color);
	else
		pPB->Canvas->Brush->Color=GetColor(Color);
}

void TransGraph::SetBrush(TPaintBox* pPB,const TColor Color, const TBrushStyle Style)
{
	if (pC_D2D)
	{
		pC_D2D->Brush->Color=GetColor(Color);
		pC_D2D->Brush->Style=Style;
	}
	else
	{
		pPB->Canvas->Brush->Color=GetColor(Color);
		pPB->Canvas->Brush->Style=Style;
	}
}

void TransGraph::SetFont(TPaintBox* pPB,const String sName, const int nSize, const TColor Color)
{
	if (pC_D2D)
	{
		pC_D2D->Font->Name=sName;
		pC_D2D->Font->Size=0;
		pC_D2D->Font->Height=-(nSize*13)/10;
		pC_D2D->Font->Color=GetColor(Color);
	}
	else
	{
		pPB->Canvas->Font->Name=sName;
		pPB->Canvas->Font->Size=0;
		pPB->Canvas->Font->Height=-(nSize*13)/10;
		pPB->Canvas->Font->Color=GetColor(Color);
	}

}

void TransGraph::SetFont(TPaintBox* pPB,TFont* pFont)
{
	if (pC_D2D)
		pC_D2D->Font->Assign(pFont);
	else
		pPB->Canvas->Font->Assign(pFont);
}

void TransGraph::MoveTo(TPaintBox* pPB,const int& nX,const int& nY)
{
	if (pC_D2D)
		pC_D2D->MoveTo(nX,nY);
	else
		pPB->Canvas->MoveTo(nX,nY);
}
void TransGraph::MoveTo(TPaintBox* pPB,const Pixel& pt)
{
	MoveTo(pPB,pt.nx,pt.ny);
}

void TransGraph::MoveTo(TPaintBox* pPB,const XYPos& XY)
{
	MoveTo(pPB,XY2Pix(XY));
}

void TransGraph::LineTo(TPaintBox* pPB,const int& nX,const int& nY)
{
	if (pC_D2D)
		pC_D2D->LineTo(nX,nY);
	else
		pPB->Canvas->LineTo(nX,nY);

}
void TransGraph::LineTo(TPaintBox* pPB,const Pixel& pt)
{
	LineTo(pPB,pt.nx,pt.ny);
}

void TransGraph::LineTo(TPaintBox* pPB,const XYPos& XY)
{
   LineTo(pPB,XY2Pix(XY));
}

void TransGraph::Dot(TPaintBox* pPB,const Pixel& pt, const int nSize)
{
	if (pC_D2D)
		pC_D2D->Ellipse(pt.nx-nSize,pt.ny-nSize,pt.nx+nSize,pt.ny+nSize);
	else
		pPB->Canvas->Ellipse(pt.nx-nSize,pt.ny-nSize,pt.nx+nSize,pt.ny+nSize);
}

void TransGraph::Dot(TPaintBox* pPB,const XYPos& XY, const int nSize)
{
   Dot(pPB,XY2Pix(XY),nSize);
}

LatLong TransGraph::GetLatLong(const Pixel pt)
{
   return XY2LL(Pix2XY(pt));
}
  */
/* !!! NOT CURRENTLY SUPPORTED !!! LJVW 2012/02/21
void TransGraph::SetPrint(const bool bPrint, const int nLeft, const int nTop, const int nWidth, const int nHeight, const bool bCentered)
{
	bPrinting=bPrint;
	if (bPrinting)
		{
		pPrevCanvas=pC;
		pC=Printer()->Canvas;

		HDC DC=pC->Handle;

		::SetMapMode(DC,MM_ANISOTROPIC);
		::SetWindowExtEx(DC,ScreenSize.nW,ScreenSize.nH,NULL);

		int nXDPI=::GetDeviceCaps(DC,LOGPIXELSX);
		int nYDPI=::GetDeviceCaps(DC,LOGPIXELSY);

		int nL,nT;
		if (bCentered)
			{
			int nW=(::GetDeviceCaps(DC,PHYSICALWIDTH)*25.4)/nXDPI;
			int nH=(::GetDeviceCaps(DC,PHYSICALHEIGHT)*25.4)/nYDPI;
			nL=(nW-nWidth)/2;
			nT=(nH-nHeight)/2;
			}
		else
			{
			nL=nLeft;
			nT=nTop;
			}

		int nXOffs=::GetDeviceCaps(DC,PHYSICALOFFSETX);
		double dXOffs=(nXOffs*25.4)/nXDPI;  //Offset in mm
		dXOffs=nL-dXOffs;                //Required Offset in mm
		int nXPixOffset=(dXOffs/25.4)*nXDPI;

		int nYOffs=::GetDeviceCaps(DC,PHYSICALOFFSETY);
		double dYOffs=(nYOffs*25.4)/nYDPI;  //Offset in mm
		dYOffs=nT-dYOffs;                 //Required Offset in mm
		int nYPixOffset=(dYOffs/25.4)*nYDPI;

		int nPixWidth=(nWidth/25.4)*nXDPI;
		int nPixHeight=(nHeight/25.4)*nYDPI;

		::SetViewportExtEx(DC,nPixWidth,nPixHeight,NULL);
		::SetViewportOrgEx(DC,nXPixOffset,nYPixOffset,NULL);

		hClipRgn=::CreateRectRgn(nXPixOffset,nYPixOffset,nXPixOffset+nPixWidth,nYPixOffset+nPixHeight);
		::SelectClipRgn(DC,hClipRgn);
		}
	else
		{
		HDC DC=pC->Handle;
		::SelectClipRgn(DC,NULL);
		::DeleteObject(hClipRgn);

		pC=pPrevCanvas;
		}
}
LJVW 2012/02/21 */

Polar TransGraph::GetPol(const Pixel& Pt1, const Pixel& Pt2)
{
	return Transform::GetPol(Pix2XY(Pt1),Pix2XY(Pt2));
}
/* !!! NOT CURRENTLY SUPPORTED !!! LJVW 2012/02/21
void TransGraph::SetMetaFile(const bool bM)
{
	bMeta=bM;
	if (bMeta)
		{
		delete pMetafile;
		pMetafile=new TMetafile;
		pMetafile->Enhanced=true;
      pMetafile->Width=610;
      pMetafile->Height=394;
      delete pMetaCanvas;
      pMetaCanvas=new TMetafileCanvas(pMetafile,0);
      pPrevCanvas=pC;
      pC=pMetaCanvas;
      HDC DC=pC->Handle;
      ::SetMapMode(DC,MM_ANISOTROPIC);
      ::SetWindowExtEx(DC,ScreenSize.nW,ScreenSize.nH,NULL);
      ::SetViewportExtEx(DC,610,394,NULL);
      ::SetViewportOrgEx(DC,0,0,NULL);
      }
   else
      {
      delete pMetaCanvas; //This will transfer the data to the metafile
      pMetaCanvas=NULL;
      Clipboard()->Assign(pMetafile);
      delete pMetafile;
      pMetafile=NULL;
      pC=pPrevCanvas;
		}
}
LJVW 2012/02/21 */

/*TColor TransGraph::GetColor(const TColor Color)
{
   if (bUsePrintColor)
      {
      int n=0;
      TColor PrintColor=clBlack;
      bool bFound=false;
      while((n<48)&&(!bFound))
         {
         if (Color==aDispColor[n])
            {
            bFound=true;
            PrintColor=aPrintColor[n];
            }
         else
            n++;
         }
      return PrintColor;
      }
   else
      return Color;
}
	 */
