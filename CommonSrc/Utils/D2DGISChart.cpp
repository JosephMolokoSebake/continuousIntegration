//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "D2DGISChart.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


GISChart::GISPolygon::GISPolygon(const BYTE* pucBuf, const int nPntCnt)
{
   pLL=NULL;
   pXY=NULL;
   pPnt=NULL;

   UnpackLL(pucBuf,nPntCnt);
}


void GISChart::GISPolygon::DrawFill(D2DDraw* pDraw,DWG_LAYER_TYPE Layer)
{
   if (bValid)
	  pDraw->DrawPolygonRegion(Layer,pPnt,nCnt);
}

void GISChart::GISPolygon::DrawBorder(D2DDraw* pDraw,DWG_LAYER_TYPE Layer)
{
	if (bValid)
		pDraw->DrawPolyLine(Layer,pPnt,nCnt);
}

void GISChart::GISPolygon::DrawEEZ(D2DDraw* pDraw,DWG_LAYER_TYPE Layer, const TColor Col)
{
   if (bValid)
   	{
      pDraw->SetBrush(Layer,Col,bsSolid);
      pDraw->Ellipse(Layer,pPnt[0].x,pPnt[0].y,nEEZ_pix,nEEZ_pix,true);
   	}
}


int GISChart::GISPolygon::nPackLL(BYTE* pucData)
{
   int nOffs=0;
   int nL;
   for (int i=0; i<nCnt; i++)
      {
      nL=pLL[i].dLat*60000.0;
      memcpy(&pucData[nOffs],&nL,sizeof(nL));
      nOffs+=sizeof(nL);
      nL=pLL[i].dLong*60000.0;
      memcpy(&pucData[nOffs],&nL,sizeof(nL));
      nOffs+=sizeof(nL);
      }
   return nOffs;
}

void GISChart::GISPolygon::UnpackLL(const BYTE* pucData, const int nPntCnt)
{
   int nOffs=0;
   if (nPntCnt>0)
      {
      Init(nPntCnt);
      int nLat,nLong;
      for (int i=0; i<nCnt; i++)
         {
         memcpy(&nLat,&pucData[nOffs],sizeof(nLat));
         nOffs+=sizeof(nLat);
         memcpy(&nLong,&pucData[nOffs],sizeof(nLong));
         nOffs+=sizeof(nLong);
         AddLL(LatLong(nLat/60000.0,nLong/60000.0));
         }
      }
}

/*
#ifdef __DDGRAPH
void GISChart::GISPolygon::TransLL(DDGraph* pTrans, const LatLong& ScreenMin, const LatLong& ScreenMax)
{
   bValid=bIntersect(ScreenMin,ScreenMax);
   if (bValid)
      {
      Pixel Pix;
      Pixel MaxPix(-100000000,-100000000),MinPix(100000000,100000000);
      for (int i=0; i<nCnt; i++)
         {
         pXY[i]=pTrans->LL2XY(pLL[i]);
         Pix=pTrans->XY2Pix(pXY[i]);
         pPnt[i].x=Pix.nx;
         pPnt[i].y=Pix.ny;
         UpdateMinMax(Pix,MinPix,MaxPix);
         }
      if ((MaxPix.nx-MinPix.nx<2)&&(MaxPix.ny-MinPix.ny<2))
         bValid=false;
      }

//   bValid=true;
}

#else*/
void GISChart::GISPolygon::TransLL(TransGraph* pTrans, const LatLong& ScreenMin, const LatLong& ScreenMax)
{
	bValid=bIntersect(ScreenMin,ScreenMax);
	if (bValid)
		{
		Pixel Pix;
		Pixel MaxPix(-100000000,-100000000),MinPix(100000000,100000000);
		for (int i=0; i<nCnt; i++)
			{
			pXY[i]=pTrans->LL2XY(pLL[i]);
			Pix=pTrans->XY2Pix(pXY[i]);
			pPnt[i].x=Pix.nx;
			pPnt[i].y=Pix.ny;
			UpdateMinMax(Pix,MinPix,MaxPix);
			}
		if ((MaxPix.nx-MinPix.nx<2)&&(MaxPix.ny-MinPix.ny<2))
			bValid=false;

      double dEEZ_m=200*1852.0;
      XYPos XY;
      XY.dx=pXY[0].dx+dEEZ_m;
      XY.dy=pXY[0].dy;
      Pixel p1=pTrans->XY2Pix(pXY[0]);
      Pixel p2=pTrans->XY2Pix(XY);

      nEEZ_pix=abs(p1.nx-p2.nx);

		}
}

//#endif

//**************************************************************************************


GISChart::GISChart()
{
	PolyList.Clear();
	pBlock=new BlockStruct[MAX_BLOCKS];
	pnValidPolygons=NULL;
	nValidPolyCount=0;
}

GISChart::~GISChart()
{
   delete[] pnValidPolygons;
   delete[] pBlock;
}

int GISChart::nImport(const String sFile, const bool bClip, TMessageBarForm* pMsgBar)
{
   if (pMsgBar)
      {
      pMsgBar->Show();
      pMsgBar->SetCaption(L"Importing Chart ("+sFileNameOnly(sFile)+") ...");
      }
   PolyList.Clear();
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFile);
   char szSt[1024];
   GISPolygon* pBlock=NULL;
   LatLong LL;
   while(!E)
      {
      E=Fil.Read(szSt,1023);
      if (!E)
         {
         if ((szSt[0]=='P')&&(szSt[1]==' '))
            {
            szSt[0]=' ';
            szSt[1]=' ';
            int nCnt;
				int n=sscanf(szSt,"%d",&nCnt);
				if ((n==1)&&(nCnt>0))
					{
               if (pBlock)
                  {
                  if (bClip)
                     {
                     if (pBlock->bIntersect(ClipMin,ClipMax))
                        PolyList.nAdd(pBlock);
                     else
                        delete pBlock;
                     }
                  else
                     PolyList.nAdd(pBlock);
                  }
               pBlock=new GISPolygon;
               pBlock->Init(nCnt);
               }
            }
         else if ((szSt[0]=='C')&&(szSt[1]==' '))
            {
            szSt[0]=' ';
            szSt[1]=' ';
            int n=sscanf(szSt,"%lf%lf",&LL.dLong,&LL.dLat);
            if (n==2)
               {
               if (pBlock)
                  pBlock->AddLL(LL);
               }
            }
         }
      }
   if (pBlock)
      {
      if (bClip)
         {
         if (pBlock->bIntersect(ClipMin,ClipMax))
            PolyList.nAdd(pBlock);
         else
            delete pBlock;
         }
      else
         PolyList.nAdd(pBlock);
      }
   PolyList.Pack();
   if (pMsgBar)
      pMsgBar->SetProgress(100);
   GenerateBlocks(pMsgBar);
   return PolyList.nGetCount();
}

void GISChart::GenerateBlocks(TMessageBarForm* pMsgBar)
{
   if (pMsgBar)
      {
      pMsgBar->Show();
      pMsgBar->SetCaption(L"Generating 1 degree blocks ...");
      pMsgBar->SetProgress(0);
      }
   LatLong MinLL,MaxLL;
   int nCnt;
   int* pnPoly=new int[PolyList.nGetCount()];
   for (int nB=0; nB<MAX_BLOCKS; nB++)
      {
      if (pMsgBar)
         pMsgBar->SetProgress(nB,MAX_BLOCKS);
      nCnt=0;
      GetBlockExtents(nB,MinLL,MaxLL);
      for (int i=0; i<PolyList.nGetCount(); i++)
         {
         if (PolyList[i]->bIntersect(MinLL,MaxLL))
            pnPoly[nCnt++]=i;
         }
      pBlock[nB].Set(pnPoly,nCnt);
      }
   delete[] pnPoly;
   if (pMsgBar)
      pMsgBar->SetProgress(100);
}

int GISChart::nGetMaxPolyCount(const LatLong& MinLL, const LatLong& MaxLL)
{
   int nCnt=0;
   int nLat1=nGetBlockLat(MinLL.dLat);
   int nLat2=nGetBlockLat(MaxLL.dLat);
   if (nLat2<nLat1)
      {
      int nT=nLat2;
      nLat2=nLat1;
      nLat1=nT;
      }
   int nLong1=nGetBlockLong(MinLL.dLong);
   int nLong2=nGetBlockLong(MaxLL.dLong);
   int nB;
   for (int nLat=nLat1; nLat<=nLat2; nLat++)
      {
      if (nLong2>=nLong1)
         {
         for (int nLong=nLong1; nLong<=nLong2; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            nCnt+=pBlock[nB].nPolyCnt;
            }
         }
      else
         {
         for (int nLong=nLong1; nLong<360; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            nCnt+=pBlock[nB].nPolyCnt;
            }
         for (int nLong=0; nLong<=nLong2; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            nCnt+=pBlock[nB].nPolyCnt;
            }
         }
      }
   return nCnt;
}

int GISChart::nGetValidPolygons(int* pnIndex, const LatLong& MinLL, const LatLong& MaxLL)
{
   bool* pbUsed=new bool[PolyList.nGetCount()];
   memset(pbUsed,false,PolyList.nGetCount());
   int nLat1=nGetBlockLat(MinLL.dLat);
   int nLat2=nGetBlockLat(MaxLL.dLat);
   if (nLat2<nLat1)
      {
      int nT=nLat2;
      nLat2=nLat1;
      nLat1=nT;
      }
   int nLong1=nGetBlockLong(MinLL.dLong);
   int nLong2=nGetBlockLong(MaxLL.dLong);
   int nB;
   for (int nLat=nLat1; nLat<=nLat2; nLat++)
      {
      if (nLong2>=nLong1)
         {
         for (int nLong=nLong1; nLong<=nLong2; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            for (int nP=0; nP<pBlock[nB].nPolyCnt; nP++)
               pbUsed[pBlock[nB].pnPolygon[nP]]=true;
            }
         }
      else
         {
         for (int nLong=nLong1; nLong<360; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            for (int nP=0; nP<pBlock[nB].nPolyCnt; nP++)
               pbUsed[pBlock[nB].pnPolygon[nP]]=true;
            }
         for (int nLong=0; nLong<=nLong2; nLong++)
            {
            nB=nGetBlockIndex(nLat,nLong);
            for (int nP=0; nP<pBlock[nB].nPolyCnt; nP++)
               pbUsed[pBlock[nB].pnPolygon[nP]]=true;
            }
         }
      }
   int nCnt=0;
   for (int i=0; i<PolyList.nGetCount(); i++)
      if (pbUsed[i])
         pnIndex[nCnt++]=i;
   delete[] pbUsed;
   return nCnt;
}

bool GISChart::bRead(const String sFile, TMessageBarForm* pMsgBar)
{
   PolyList.Clear();
   JMemFile Fil;
   if (pMsgBar)
      {
      pMsgBar->Show();
		pMsgBar->SetCaption("Reading Chart ("+sFileNameOnly(sFile)+") ...");
      }
   JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
   if (!E)
      {
      int nVer,nCnt;
      bool bOK=Fil.bRead(&nVer,sizeof(nVer));
      if (bOK)
         bOK=Fil.bRead(&nCnt,sizeof(nCnt));
      if ((bOK)&&(nCnt>0))
			{
			int* pnPolyCnt=new int[nCnt];
			bOK=Fil.bRead(pnPolyCnt,nCnt*sizeof(int));
			for (int i=0; (i<nCnt)&&(bOK); i++)
				{
				if (pMsgBar)
					pMsgBar->SetProgress((i*100)/nCnt);
				if (pnPolyCnt[i]>0)
					{
					BYTE* pucBuf=new BYTE[pnPolyCnt[i]*sizeof(int)*2];
					bOK=Fil.bRead(pucBuf,pnPolyCnt[i]*sizeof(int)*2);
					if (bOK)
						PolyList.nAdd(new GISPolygon(pucBuf,pnPolyCnt[i]));
					delete[] pucBuf;
					}
				}
			delete[] pnPolyCnt;
			}
		if (pMsgBar)
			pMsgBar->SetProgress(100);
		PolyList.Pack();
		for (int i=0; (i<MAX_BLOCKS)&&(bOK); i++)
         bOK=pBlock[i].bRead(&Fil);
      if (!bOK)
         nShowError(L"Error Reading: "+sFile,L"GIS Chart File Error",30);
      return bOK;
      }
   else
      {
      nShowError(L"Error Opening: "+sFile,L"GIS Chart File Error",30);
      return false;
      }
}

bool GISChart::bStore(const String sFile, TMessageBarForm* pMsgBar)
{
   JFile Fil('O');
   JFile::FILE_ERROR E=Fil.Create(sFile);
   if (!E)
      {
      int nVer=GIS_CHART_VERSION;
      E=Fil.Write(&nVer,sizeof(nVer));
      }
   if (!E)
      {
      int nCnt=PolyList.nGetCount();
      E=Fil.Write(&nCnt,sizeof(nCnt));
      }
   if ((!E)&&(PolyList.nGetCount()>0))
      {
      int* pnPolyCnt=new int[PolyList.nGetCount()];
      for (int i=0; i<PolyList.nGetCount(); i++)
         pnPolyCnt[i]=PolyList[i]->nGetCount();
      E=Fil.Write(pnPolyCnt,PolyList.nGetCount()*sizeof(int));
      delete[] pnPolyCnt;
      }
   for (int i=0; (i<PolyList.nGetCount())&&(!E); i++)
      {
      if (pMsgBar)
         pMsgBar->SetProgress(i,PolyList.nGetCount());
      int nSize=PolyList[i]->nGetSize();
      BYTE* pucBuf=new BYTE[nSize];
      nSize=PolyList[i]->nPackLL(pucBuf);
      E=Fil.Write(pucBuf,nSize);
      delete[] pucBuf;
      }
   for (int i=0; (i<MAX_BLOCKS)&&(!E); i++)
      E=pBlock[i].Store(&Fil);
   Fil.ShowError(E);
   return (E==JFile::F_NO_ERROR);
}

/*#ifdef __DDGRAPH
void GISChart::RegenPixels(DDGraph* pTrans, const LatLong& MinLL, const LatLong& MaxLL)
{
   delete[] pnValidPolygons;
   pnValidPolygons=NULL;
   nValidPolyCount=0;
   LatLong Min=MinLL;
   LatLong Max=MaxLL;
   CheckMinMax(Min,Max);
   int nMaxPolyCount=nGetMaxPolyCount(Min,Max);
   if (nMaxPolyCount>0)
      {
      pnValidPolygons=new int[nMaxPolyCount];
      nValidPolyCount=nGetValidPolygons(pnValidPolygons,Min,Max);
      for (int i=0; i<nValidPolyCount; i++)
         PolyList[pnValidPolygons[i]]->TransLL(pTrans,Min,Max);
      }
}
#else*/

void GISChart::RegenPixels(TransGraph* pTrans, const LatLong& MinLL, const LatLong& MaxLL)
{
	delete[] pnValidPolygons;
	pnValidPolygons=NULL;
	nValidPolyCount=0;
	LatLong Min=MinLL;
	LatLong Max=MaxLL;
	CheckMinMax(Min,Max);
	int nMaxPolyCount=nGetMaxPolyCount(Min,Max);
	if (nMaxPolyCount>0)
		{
		pnValidPolygons=new int[nMaxPolyCount];
		nValidPolyCount=nGetValidPolygons(pnValidPolygons,Min,Max);
		for (int i=0; i<nValidPolyCount; i++)
			PolyList[pnValidPolygons[i]]->TransLL(pTrans,Min,Max);
      if (EEZ.nGetCnt()>1)
         EEZ.TransLL(pTrans);
      if (Zone.nGetCnt()>0)
         Zone.TransLL(pTrans);
		}
}
//#endif


bool GISChart::bDraw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer,const int nW,const int nH,const TColor ChartCol,const TColor BackCol,const bool bFill, const bool bStandAlone, const bool bFillBack)
{
	if (bStandAlone)
		pDraw->BeginDraw(Layer);
	if (bFillBack)
		{
		pDraw->SetBrush(Layer,BackCol);
		pDraw->FillRect(Layer,TRect(0,0,nW,nH));
		}

	if (nValidPolyCount>0)
		{

      if (EEZ.nGetCnt()>1)
         EEZ.Draw(pDraw,Layer);

		pDraw->SetBrush(Layer,ChartCol);
		if (bFill)
			{
			for (int i=0; i<nValidPolyCount; i++)
				{
				PolyList[pnValidPolygons[i]]->DrawFill(pDraw,Layer);
				}
			}
		else
			{
			for (int i=0; i<nValidPolyCount; i++)
				PolyList[pnValidPolygons[i]]->DrawBorder(pDraw,Layer);
			}
		// revert to old pen and brush - GDI
//		pDraw->SelectOldAndDeleteNewBrush_GDI(Layer);
//		pDraw->SelectOldAndDeleteNewPen_GDI(Layer);
		}
   if (Zone.nGetCnt()>1)
      Zone.Draw(pDraw,Layer);
	if (bStandAlone)
		return (pDraw->bEndDraw(Layer));
	else
		return true;
}

bool GISChart::bProximity(const LatLong& LL, const int nProx_m)
{
	bool bCloser=false;
//	int nClosest_m=10000000;


	return bCloser;
}

//**************************************************************************************

int EEZPolygon::nSetup(const String sFil, const TColor _Col, const bool _bFill)
{
   Col=_Col;
   bFill=_bFill;
   nCnt=0;
   delete[] pLL;
   pLL=NULL;
//   delete[] pXY;
//   pXY=NULL;
   delete[] pPnt;
   pPnt=NULL;
   JList<LatLong> LL_List;
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFil);
   char szSt[256];
   LatLong LL;
   while(!E)
      {
      E=Fil.Read(szSt,255);
      if (!E)
         {
         int n=sscanf(szSt,"%lf%lf",&LL.dLat,&LL.dLong);
         if ((n==2)&&(LL.bValid()))
            LL_List.nAdd(new LatLong(LL));
         }
      }
   if (LL_List.nGetCount()>0)
      {
	   nCnt=LL_List.nGetCount()+1;
      pLL=new LatLong[nCnt];
//      pXY=new XYPos[nCnt];
      pPnt=new POINT[nCnt];
      LL_List.Pack();
      for (int i=0; i<nCnt-1; i++)
         pLL[i]=*LL_List[i];
      pLL[nCnt-1]=pLL[0];     //Close polygon
      }
   return nCnt;
}

void EEZPolygon::TransLL(TransGraph* pTrans)
{
   Pixel Pix;
   for (int i=0; i<nCnt; i++)
   	{
//      pXY[i]=pTrans->LL2XY(pLL[i]);
//      Pix=pTrans->XY2Pix(pXY[i]);
      Pix=pTrans->LL2Pix(pLL[i]);
      pPnt[i].x=Pix.nx;
      pPnt[i].y=Pix.ny;
   	}
}

void EEZPolygon::Draw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer)
{
	pDraw->SetBrush(Layer,Col);
   if (bFill)
		pDraw->DrawPolygonRegion(Layer,pPnt,nCnt);
   else
		pDraw->DrawPolyLine(Layer,pPnt,nCnt);
}

//**************************************************************************************

int ZonePolygon::nSetup(const String sFil, const TColor _Col)
{
   Col=_Col;
   nCnt=0;
   delete[] pLL;
   pLL=NULL;
   delete[] pPnt;
   pPnt=NULL;
   JList<LatLong> LL_List;
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFil);
   char szSt[256];
   LatLong LL;
   while(!E)
      {
      E=Fil.Read(szSt,255);
      if (!E)
         {
         int n=sscanf(szSt,"%lf%lf",&LL.dLat,&LL.dLong);
         if ((n==2)&&(LL.bValid()))
            LL_List.nAdd(new LatLong(LL));
         }
      }
   if (LL_List.nGetCount()>0)
      {
	   nCnt=LL_List.nGetCount()+1;
      pLL=new LatLong[nCnt];
//      pXY=new XYPos[nCnt];
      pPnt=new POINT[nCnt];
      LL_List.Pack();
      for (int i=0; i<nCnt-1; i++)
         pLL[i]=*LL_List[i];
      pLL[nCnt-1]=pLL[0];     //Close polygon
      }
   return nCnt;
}

void ZonePolygon::TransLL(TransGraph* pTrans)
{
   Pixel Pix;
   for (int i=0; i<nCnt; i++)
   	{
      Pix=pTrans->LL2Pix(pLL[i]);
      pPnt[i].x=Pix.nx;
      pPnt[i].y=Pix.ny;
   	}
}

void ZonePolygon::Draw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer)
{
	pDraw->SetBrush(Layer,Col);
	pDraw->DrawPolyLine(Layer,pPnt,nCnt);
}


