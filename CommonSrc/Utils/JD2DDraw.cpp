//---------------------------------------------------------------------------
#include <jpch.h>

#pragma hdrstop

#include "JD2DDraw.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


D2DDraw::D2DDraw(TPanel* pPanel)
{
	nWidth=pPanel->Width;
	nHeight=pPanel->Height;
   CurrentLayer=DWG_LAYER_CANVAS;
	pPanel->Caption="";
	pPanel->BevelInner=bvNone;
	pPanel->BevelOuter=bvNone;
	pPanel->BevelKind=bkNone;
	for (int i=0;i<2;i++)
		{
		pBM_WIC_D2D[i]=NULL;
		pWIC_RT_D2D[i]=NULL;
//		pBM_GDI[i]=NULL;
		abBMDrawStarted_D2D[i]=false;
		}
	pWICFactory=NULL;
	pWICBitmapLock=NULL;
	for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
		paSolidBrush_WIC_BM_D2D[i]=NULL;
	for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
		paSolidBrush_D2D[i]=NULL;
	pdwWICBMData=new DWORD*[nHeight];
	for (int y=0;y<nHeight;y++)
		{
		pdwWICBMData[y]=new DWORD[nWidth];
		}
	pucWicBMData=NULL;
	pC_D2D=NULL;
//	pPB_GDI=NULL;
	pDWFactory_D2D=NULL;
	bool bDirect2DSupported;
   try
      {
      pC_D2D=new TDirect2DCanvas(pPanel->Handle);
      bDirect2DSupported=pC_D2D->Supported();
      }
   catch(...)
      {
       bDirect2DSupported=false;
      }
	if (!bDirect2DSupported)
		{
		delete pC_D2D;
		pC_D2D=NULL;
		}
	else
		{
		D2D1_PIXEL_FORMAT PixFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE);
		RTProp_D2D=D2D1::RenderTargetProperties();
		RTProp_D2D.pixelFormat=PixFormat;

      //Use Aliased mode for everything to get maximum drawing speed
      pC_D2D->RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		pC_D2D->RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
//		pC_D2D->RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_FORCE_DWORD);
//		pC_D2D->RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		pC_D2D->RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// Create DirectWrite Factory
		if ((DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),reinterpret_cast<IUnknown**>(&pDWFactory_D2D)))!=S_OK)
			pDWFactory_D2D=NULL;
		// Create D2D Layer Brushes
		for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
			paSolidBrush_D2D[i]=new LJBrush_D2D(pC_D2D->RenderTarget,clWhite,pDWFactory_D2D,nWidth,nHeight);
		// create back/front D2D bitmaps
		D2D_CreateBitmapRenderTargets();
		}
}

D2DDraw::~D2DDraw()
{
	if (pC_D2D)
		{
		delete pC_D2D;
		pC_D2D=NULL;
		}
	for (int i=0;i<2;i++)
		{
		if (pWIC_RT_D2D[i])
			{
			pWIC_RT_D2D[i]->Release();
			pWIC_RT_D2D[i]=NULL;
			}
		if (pBM_WIC_D2D[i])
			{
			pBM_WIC_D2D[i]->Release();
			pBM_WIC_D2D[i]=NULL;
			}
		}
	if (pWICFactory)
		{
		pWICFactory->Release();
		pWICFactory=NULL;
		}
	if (pWICBitmapLock)
		{
		pWICBitmapLock->Release();
		pWICBitmapLock=NULL;
		}
	if (pdwWICBMData)
		{
		for (int y=0;y<nHeight;y++)
			{
			delete[] pdwWICBMData[y];
			pdwWICBMData[y]=NULL;
			}
		}
	for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
		{
		if (paSolidBrush_WIC_BM_D2D[i]!=NULL)
			{
			delete paSolidBrush_WIC_BM_D2D[i];
			paSolidBrush_WIC_BM_D2D[i]=NULL;
			}
		if (paSolidBrush_D2D[i]!=NULL)
			{
			delete paSolidBrush_D2D[i];
			paSolidBrush_D2D[i]=NULL;
			}
		}
	if (pDWFactory_D2D)
		{
		pDWFactory_D2D->Release();
		pDWFactory_D2D=NULL;
		}
}

bool D2DDraw::bD2D_CreateWICFactory()
{
	bool bOK=true;
	if (pWICFactory == NULL)
	{
		if (CoCreateInstance(CLSID_WICImagingFactory, NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&pWICFactory)) != S_OK)
			bOK=false;
	}
	if (!bOK)
	{
		pWICFactory=NULL;
	}
	return bOK;
}

void D2DDraw::D2D_CreateBitmapRenderTargets()
{
	if (pC_D2D)
	{
		if (bD2D_CreateWICFactory())
		{
			ID2D1Factory* pD2DFactory;
			pC_D2D->RenderTarget->GetFactory(&pD2DFactory);
			HRESULT hr=S_OK;
			for (int i=0;i<2;i++)
			{
				if (pBM_WIC_D2D[i]) // release WIC Bitmap
					pBM_WIC_D2D[i]->Release();
				if (pWIC_RT_D2D[i]) // release WIC Render Target
					pWIC_RT_D2D[i]->Release();
				hr=pWICFactory->CreateBitmap(nWidth,nHeight,GUID_WICPixelFormat32bppBGR,WICBitmapCacheOnLoad,&pBM_WIC_D2D[i]);
				if (hr==S_OK)
				{

					hr=pD2DFactory->CreateWicBitmapRenderTarget(pBM_WIC_D2D[i],RTProp_D2D,&pWIC_RT_D2D[i]);
					if (hr!=S_OK)
						pWIC_RT_D2D[i]=NULL;
				}
				else
					pBM_WIC_D2D[i]=NULL;
			}
			if (pBM_WIC_D2D[0] && hr==S_OK)
				for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
					paSolidBrush_WIC_BM_D2D[i]=new LJBrush_D2D(pWIC_RT_D2D[0],clWhite,pDWFactory_D2D,nWidth,nHeight);
		}
	}
}

void D2DDraw::BeginDraw(const DWG_LAYER_TYPE Layer)
{
	if (Layer==DWG_LAYER_CANVAS)
	{
		if (pC_D2D)
			pC_D2D->BeginDraw();
	}
	else
	{
		if (pWIC_RT_D2D[Layer])
		{
			pWIC_RT_D2D[Layer]->BeginDraw();
			abBMDrawStarted_D2D[Layer]=true;
		}
	}
}

bool D2DDraw::bEndDraw(DWG_LAYER_TYPE Layer)
{
	HRESULT hr=S_OK;
	if (Layer==DWG_LAYER_CANVAS)
	{
		if (pC_D2D)
			pC_D2D->EndDraw();
	}
	else
	{
		if (pWIC_RT_D2D[Layer])
		{
			hr=pWIC_RT_D2D[Layer]->EndDraw();
			abBMDrawStarted_D2D[Layer]=false;
		}
	}
	return (hr==S_OK);
}


/*
void D2DDraw::DrawGDIBitMap(TPaintBox* pPB,Graphics::TBitmap* pBM)
{
	if (pBM)
	{
		if (pC_D2D)
			pC_D2D->Draw(0,0,pBM);
		else
			pPB->Canvas->Draw(0,0,pBM);
	}
}
*/

bool D2DDraw::bDrawBitMap(DWG_LAYER_TYPE DestLayer,DWG_LAYER_TYPE SrcLayer)
{
	HRESULT hr=S_OK;
	if (DestLayer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D && pBM_WIC_D2D[SrcLayer])
			{
			ID2D1Bitmap* pBM_D2D=NULL;
			hr=pC_D2D->RenderTarget->CreateBitmapFromWicBitmap(pBM_WIC_D2D[SrcLayer],NULL,&pBM_D2D);
			if (hr==S_OK)
				pC_D2D->RenderTarget->DrawBitmap(pBM_D2D,D2D1::RectF(0.f,0.f,nWidth,nHeight));
			pBM_D2D->Release();
			}
		}
	else // NOTE: if destination layer is bitmap, source layer must also be bitmap
		{
		hr=S_FALSE;
		if (pWIC_RT_D2D[SrcLayer] && pWIC_RT_D2D[DestLayer])
			{
			ID2D1Bitmap* pBM_D2D=NULL;
			hr=pWIC_RT_D2D[DestLayer]->CreateBitmapFromWicBitmap(pBM_WIC_D2D[SrcLayer],NULL,&pBM_D2D);
			if (hr==S_OK)
				pWIC_RT_D2D[DestLayer]->DrawBitmap(pBM_D2D,D2D1::RectF(0.f,0.f,nWidth,nHeight));
			pBM_D2D->Release();
			}
		}
	return (hr==S_OK);
}

void D2DDraw::SetPen(DWG_LAYER_TYPE Layer,const TColor Col,const int nWidth,const TPenStyle PenStyle)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->SetColor(Col);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->SetWidth(nWidth);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->SetStrokeStyle(PenStyle);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->SetColor(Col);
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->SetWidth(nWidth);
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->SetStrokeStyle(PenStyle);
			}
      }
}

void D2DDraw::SetBrush(DWG_LAYER_TYPE Layer,const TColor Col,const TBrushStyle BrushStyle)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			//pC_D2D->Brush->Style=BrushStyle;			// NOTE: Setting this to anything other than bsSolid/bsClear will cause an exception - not supported for TDirect2DCanvas
			//pC_D2D->Brush->Color=GetColor(Col);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_BRUSH]->SetStrokeStyle(BrushStyle);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_BRUSH]->SetColor(Col);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->SetStrokeStyle(BrushStyle);
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->SetColor(Col);
			}
		}
}

void D2DDraw::DrawPolygonRegion(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			ShowMessage("To be implemented");
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH])
			{
			Check_BM_RT_D2D_BeginDrawCalled(Layer);
			//D2D1_FILL_MODE_ALTERNATE
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->DrawPolygon_D2D(pWIC_RT_D2D[Layer],pPnt,nCnt,true);
			}
		}
}

void D2DDraw::DrawPolyLine(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			ShowMessage("To be implemented");
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH])
			{
			Check_BM_RT_D2D_BeginDrawCalled(Layer);
			//D2D1_FILL_MODE_ALTERNATE
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->DrawPolygon_D2D(pWIC_RT_D2D[Layer],pPnt,nCnt,false);
			}
		}
}

void D2DDraw::FillRect(DWG_LAYER_TYPE Layer,const TRect R)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			//pC_D2D->FillRectangle(D2D1::RectF(R.left,R.top,R.right,R.bottom));
			paSolidBrush_D2D[D2D_BRUSH_TYPE_BRUSH]->FillRect(pC_D2D->RenderTarget,R);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH])
			{
			//pWIC_RT_D2D[Layer]->FillRectangle(D2D1::RectF(R.left,R.top,R.right,R.bottom),paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->Get());
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->FillRect(pWIC_RT_D2D[Layer],R);
			}
		}
}

void D2DDraw::FillRect(DWG_LAYER_TYPE Layer,const TRect R,const TColor Col,const TBrushStyle BrushStyle)
{
	SetBrush(Layer,Col,BrushStyle);
	FillRect(Layer,R);
}


void D2DDraw::Check_BM_RT_D2D_BeginDrawCalled(const DWG_LAYER_TYPE Layer)
{
	if (pWIC_RT_D2D[Layer])
	{
		if (!abBMDrawStarted_D2D[Layer])
		{
			BeginDraw(Layer);
			abBMDrawStarted_D2D[Layer]=true;
		}
	}
}

/*
void D2DDraw::CreatePolygon_D2D(DWG_LAYER_TYPE Layer,POINT* pPnt,const int nCnt,const bool bFill)
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

	ID2D1Factory* pD2DFactory;
	pC_D2D->RenderTarget->GetFactory(&pD2DFactory);
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
	  if (paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH])
	  {
			if (bFill)
				pWIC_RT_D2D[Layer]->FillGeometry(pg,paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->Get());
			else
				pWIC_RT_D2D[Layer]->DrawGeometry(pg,paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->Get(),1);
	  }
	  pg->Release();
	  delete[] pt;
	}
}
*/

void D2DDraw::Rectangle(DWG_LAYER_TYPE Layer,const int nX1,const int nY1,const int nX2,const int nY2, const bool bFill)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->DrawRect(pC_D2D->RenderTarget,TRect(nX1,nY1,nX2,nY2));
         if (bFill)
				paSolidBrush_D2D[D2D_BRUSH_TYPE_BRUSH]->FillRect(pC_D2D->RenderTarget,TRect(nX1,nY1,nX2,nY2));
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->DrawRect(pWIC_RT_D2D[Layer],TRect(nX1,nY1,nX2,nY2));
         if (bFill)
				paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->FillRect(pWIC_RT_D2D[Layer],TRect(nX1,nY1,nX2,nY2));
			}
		}
}

void D2DDraw::Ellipse(DWG_LAYER_TYPE Layer,const int nRadiusX,const int nRadiusY,const int nCenterX,const int nCenterY, const bool bFill)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->DrawEllipse(pC_D2D->RenderTarget,nRadiusX,nRadiusY,nCenterX,nCenterY);
			if (bFill)
				paSolidBrush_D2D[D2D_BRUSH_TYPE_BRUSH]->FillEllipse(pC_D2D->RenderTarget,nRadiusX,nRadiusY,nCenterX,nCenterY);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->DrawEllipse(pWIC_RT_D2D[Layer],nRadiusX,nRadiusY,nCenterX,nCenterY);
         if (bFill)
				paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_BRUSH]->FillEllipse(pWIC_RT_D2D[Layer],nRadiusX,nRadiusY,nCenterX,nCenterY);
			}
		}
}



void D2DDraw::TextOutW(DWG_LAYER_TYPE Layer,const int nX,const int nY,const String s)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->SetXY(nX,nY);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->DrawTextLayout(pC_D2D->RenderTarget,s);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->SetXY(nX,nY);
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->DrawTextLayout(pWIC_RT_D2D[Layer],s);
			}
		}
}

/*
HRESULT Direct2DUtility::GetWICFactory(IWICImagingFactory** factory)
{
	static ComPtr<IWICImagingFactory> m_pWICFactory;
	HRESULT hr = S_OK;
	if (nullptr == m_pWICFactory)
	{
		hr = CoCreateInstance(
				CLSID_WICImagingFactory, nullptr,
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory) );
	}
	if (SUCCEEDED(hr))
	{
		hr = AssignToOutputPointer(factory, m_pWICFactory);
	}
	return hr;
}
*/

void D2DDraw::LockBitmap(DWG_LAYER_TYPE Layer)
{
	if (pBM_WIC_D2D[Layer])
	{
		WICRect rcLock = {0,0,nWidth,nHeight};
		if (pWICBitmapLock)
			pWICBitmapLock->Release();
		HRESULT hr = pBM_WIC_D2D[Layer]->Lock(&rcLock,WICBitmapLockWrite,&pWICBitmapLock);
		UINT cbBufferSize=0;
		// Retrieve a pointer to the pixel data.
		if (hr==S_OK)
		{
			hr = pWICBitmapLock->GetDataPointer(&cbBufferSize,&pucWicBMData);
			if (hr==S_OK && pucWicBMData)
			{
				for (int x=0;x<nWidth;x++)
				{
					for (int y=0;y<nHeight;y++)
					{
						DWORD dw;
						memcpy(&dw,&pucWicBMData[4*((y*nWidth)+x)],4);
						pdwWICBMData[y][x]=dw;
					}
				}
			}
		}
	}
}


void D2DDraw::UnlockBitmap()
{
	if (pucWicBMData)
	{
		for (int x=0;x<nWidth;x++)
			for (int y=0;y<nHeight;y++)
			{
				DWORD dw=pdwWICBMData[y][x];
				memcpy(&pucWicBMData[4*((y*nWidth)+x)],&dw,4);
				//pdwWICBMData[y][x]=puc[(y*nWidth)+x];
			}
	}
	if (pWICBitmapLock)
	{
		pWICBitmapLock->Release();
		pWICBitmapLock=NULL;
	}
}


DWORD* D2DDraw::pdwBitMapScanLine(DWG_LAYER_TYPE Layer,const int nY)
{
	DWORD* pdwPix;
	if (pC_D2D)
		pdwPix=pdwWICBMData[nY];
	return pdwPix;
}

void D2DDraw::SetFont(DWG_LAYER_TYPE Layer,TFont* pFont)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->SetFont(pFont);
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->SetFont(pFont);
		}
}

void D2DDraw::DrawText(DWG_LAYER_TYPE Layer,const String sTxt,const int nX,const int nY)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->SetXY(nX,nY);
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->DrawText(pC_D2D->RenderTarget,sTxt);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->SetXY(nX,nY);
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->DrawText(pWIC_RT_D2D[Layer],sTxt);
			}
		}
}

void D2DDraw::MoveTo(DWG_LAYER_TYPE Layer,const int& nX,const int& nY)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
				paSolidBrush_D2D[i]->SetXY(nX,nY);
         }
		}
	else
		{
      if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			for (int i=0;i<D2D_BRUSH_TYPE_MAX_COUNT;i++)
				paSolidBrush_WIC_BM_D2D[i]->SetXY(nX,nY);
			}
		}
}

void D2DDraw::LineTo(DWG_LAYER_TYPE Layer,const int& nX,const int& nY)
{
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_PEN]->DrawLine(pC_D2D->RenderTarget,nX,nY);
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN]->DrawLine(pWIC_RT_D2D[Layer],nX,nY);
			}
		}
}

void D2DDraw::LineTo(DWG_LAYER_TYPE Layer,const Pixel& pt)
{
	LineTo(Layer,pt.nx,pt.ny);
}

int D2DDraw::nTextWidth(DWG_LAYER_TYPE Layer,const String s) const
{
	int nWidth=0;
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->SetText(s);
			nWidth=paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->nGetTextWidth();
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->SetText(s);
			nWidth=paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->nGetTextWidth();
			}
		}
	return nWidth;
}


int D2DDraw::nTextHeight(DWG_LAYER_TYPE Layer,const String s) const
{
	int nHeight=0;
	if (Layer==DWG_LAYER_CANVAS)
		{
		if (pC_D2D)
			{
			paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->SetText(s);
			nHeight=paSolidBrush_D2D[D2D_BRUSH_TYPE_TEXT]->nGetTextHeight();
			}
		}
	else
		{
		if (pWIC_RT_D2D[Layer] && paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_PEN])
			{
			paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->SetText(s);
			nHeight=paSolidBrush_WIC_BM_D2D[D2D_BRUSH_TYPE_TEXT]->nGetTextHeight();
			}
		}
	return nHeight;
}

TColor D2DDraw::GetColor(const TColor Color)
{
	return Color;
}

