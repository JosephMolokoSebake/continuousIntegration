//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "JGISChart.h"
#include "JUtils.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------

#pragma package(smart_init)


GISChart::GISPolygon::GISPolygon(const BYTE* pucBuf, const int nPntCnt)
{
   pLL=NULL;
   pXY=NULL;
   pPnt=NULL;
   
   UnpackLL(pucBuf,nPntCnt);
}


void GISChart::GISPolygon::DrawFill(HDC DC, HBRUSH hFillBrush)
{
   if (bValid)
      {
      HRGN hRgn=::CreatePolygonRgn(pPnt,nCnt,ALTERNATE);
//      HRGN hRgn=::CreatePolygonRgn(pPnt,nCnt,WINDING);
      ::SelectObject(DC,hRgn);
      ::FillRgn(DC,hRgn,hFillBrush);
      ::DeleteObject(hRgn);
      }
}

void GISChart::GISPolygon::DrawBorder(HDC DC)
{
   if (bValid)
      ::Polyline(DC,pPnt,nCnt);
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

#else
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
      }
}

#endif


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
#ifdef _CST4_DICT
		pMsgBar->SetCaption(g_Dict.sTranslate("Importing Chart")+" ("+sFileNameOnly(sFile)+") ...");
#else
		pMsgBar->SetCaption("Importing Chart ("+sFileNameOnly(sFile)+") ...");
#endif
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
#ifdef _CST4_DICT
		pMsgBar->SetCaption(g_Dict.sTranslate("Generating 1 degree blocks")+" ...");
#else
		pMsgBar->SetCaption("Generating 1 degree blocks ...");
#endif
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
#ifdef _CST4_DICT
		pMsgBar->SetCaption(g_Dict.sTranslate("Reading Chart")+" "+sFileNameOnly(sFile)+") ...");
#else
		pMsgBar->SetCaption("Reading Chart ("+sFileNameOnly(sFile)+") ...");
#endif
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
			{
#ifdef _CST4_DICT
			nShowError(g_Dict.sTranslate("Error Reading")+": "+sFile,g_Dict.sTranslate("GIS Chart File Error"),30);
#else
			nShowError(L"Error Reading: "+sFile,"GIS Chart File Error",30);
#endif
			}
		return bOK;
		}
	else
		{
#ifdef _CST4_DICT
		nShowError(g_Dict.sTranslate("Error Opening")+": "+sFile,g_Dict.sTranslate("GIS Chart File Error"),30);
#else
		nShowError(L"Error Opening: "+sFile,"GIS Chart File Error",30);
#endif
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

#ifdef __DDGRAPH
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

#else
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
      }
}

#endif

void GISChart::Draw(HDC DC, const TColor Col, const bool bFill)
{
   if (nValidPolyCount>0)
      {
      HBRUSH hBrush=::CreateSolidBrush(Col);
      HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
      HPEN hPen=::CreatePen(PS_SOLID,1,Col);
      HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
      if (bFill)
         {
         for (int i=0; i<nValidPolyCount; i++)
            {
//            PolyList[pnValidPolygons[i]]->DrawBorder(DC);
            PolyList[pnValidPolygons[i]]->DrawFill(DC,hBrush);
            }
         }
      else
         {
         for (int i=0; i<nValidPolyCount; i++)
            PolyList[pnValidPolygons[i]]->DrawBorder(DC);
         }
      ::SelectObject(DC,hOldPen);
      ::DeleteObject(hPen);
      ::SelectObject(DC,hOldBrush);
      ::DeleteObject(hBrush);
      }
}

