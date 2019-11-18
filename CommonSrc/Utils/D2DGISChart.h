//---------------------------------------------------------------------------

#ifndef D2DGISChartH
#define D2DGISChartH
//---------------------------------------------------------------------------

#include "D2DTransGraph.h"
#include "JMessageBar.h"
#include "JList.h"
#include "JFile.h"
#include "JMemFile.h"

//**************************************************************************************

class EEZPolygon
{

   LatLong* pLL;
   POINT* pPnt;
   int nCnt;

   TColor Col;
   bool bFill;


public:

   EEZPolygon()
   {
      pLL=NULL;
      pPnt=NULL;
      nCnt=0;
   }

   ~EEZPolygon()
   {
      delete[] pPnt;
      delete[] pLL;
   }

   void Clear()
   {
      delete[] pPnt;
      pPnt=NULL;
      delete[] pLL;
      pLL=NULL;
      nCnt=0;
   }

   int nSetup(const String sFil, const TColor _Col, const bool _bFill);

   void TransLL(TransGraph* pTrans);

   void Draw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer);

   int nGetCnt() const
   {
      return nCnt;
   }

};

class ZonePolygon
{

   LatLong* pLL;
   POINT* pPnt;
   int nCnt;

   TColor Col;


public:

	ZonePolygon()
   {
      pLL=NULL;
      pPnt=NULL;
      nCnt=0;
   }

	~ZonePolygon()
   {
      delete[] pPnt;
      delete[] pLL;
   }

   void Clear()
   {
      delete[] pPnt;
      pPnt=NULL;
      delete[] pLL;
      pLL=NULL;
      nCnt=0;
   }

   int nSetup(const String sFil, const TColor _Col);

   void TransLL(TransGraph* pTrans);

   void Draw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer);

   int nGetCnt() const
   {
      return nCnt;
   }

};



class GISChart
{

   enum
   {
      GIS_CHART_VERSION = 1000,
   };

   struct LLRegion
   {
      LatLong MinLL;
      LatLong MaxLL;

      LLRegion()
      {
         MinLL.Invalidate();
         MaxLL.Invalidate();
      }

      void Assign(const LLRegion& R)
      {
         MinLL=R.MinLL;
         MaxLL=R.MaxLL;
      }

      LLRegion(const LLRegion& R)
      {
         Assign(R);
      }

      LLRegion& operator = (const LLRegion& R)
      {
         Assign(R);
         return *this;
      }

      void Normalise360(double& dLong)
      {
         if (dLong<0.0)
            dLong+=360.0;
      }

      void Normalise180(double& dLong)
      {
         if (dLong>180.0)
            dLong-=360.0;
      }

      void AddLL(LatLong LL)
      {
         if (fabs(LL.dLong)>90.0)
            Normalise360(LL.dLong);
         if (MinLL.bValid())
            {
            if (LL.dLat>MaxLL.dLat)
               MaxLL.dLat=LL.dLat;
            if (LL.dLat<MinLL.dLat)
               MinLL.dLat=LL.dLat;
            if (LL.dLong>MaxLL.dLong)
               MaxLL.dLong=LL.dLong;
            if (LL.dLong<MinLL.dLong)
               MinLL.dLong=LL.dLong;
            }
         else
            {
            MinLL=LL;
            MaxLL=LL;
            }
      }

      bool bContains(const LatLong& LL) const
      {
/*
         if ((LL.dLat>MaxLL.dLat)||(LL.dLat<MinLL.dLat))
            return false;
         if (MinLL.dLong*MaxLL.dLong<0.0)
            {
            //Straddles DateLine
            if ((LL.dLong>TopLeft_LL.dLong)||(LL.dLong<BottomRight_LL.dLong))
               return false;
            }
         else if ((LL.dLong<MinLL.dLong)||(LL.dLong>MaxLL.dLong))
            return false;
*/
         return true;
      }

      bool bIntersect(const LLRegion& R) const
      {

         return true;
		}



   };


	class GISPolygon
   {

      int nCnt;
      LatLong MinLL,MaxLL;
      LatLong* pLL;
      XYPos* pXY;
      POINT* pPnt;
      bool bValid;
      int nBlockNum;

      int nIndex;

      int nEEZ_pix;


      void UpdateMinMax(const Pixel& Pix, Pixel& MinPix, Pixel& MaxPix)
      {
         if (Pix.nx<MinPix.nx)
            MinPix.nx=Pix.nx;
         if (Pix.ny<MinPix.ny)
            MinPix.ny=Pix.ny;
         if (Pix.nx>MaxPix.nx)
            MaxPix.nx=Pix.nx;
         if (Pix.ny>MaxPix.ny)
            MaxPix.ny=Pix.ny;
      }


   public:

		GISPolygon()
		{
			pLL=NULL;
			pXY=NULL;
			pPnt=NULL;
			nBlockNum=-1;
		}

		GISPolygon(const BYTE* pucBuf, const int nPntCnt);

		~GISPolygon()
      {
         delete[] pLL;
         delete[] pXY;
         delete[] pPnt;
      }

      int nGetCount() const
      {
         return nCnt;
      }

      LatLong& GetLL(const int n) const
      {
         return pLL[n];
      }

      int nGetSize() const
      {
         return sizeof(int)*2*nCnt;
      }

      int nPackLL(BYTE* pucData);
      void UnpackLL(const BYTE* pucData, const int nPntCnt);

      void Init(const int nPntCnt)
      {
         delete[] pPnt;
         delete[] pXY;
         delete[] pLL;
         nCnt=nPntCnt;
         pLL=new LatLong[nCnt];
         pXY=new XYPos[nCnt];
         pPnt=new POINT[nCnt];
         MinLL=LatLong(1e10,1e10);
         MaxLL=LatLong(-1e10,-1e10);
         bValid=false;
         nIndex=0;
      }

      void AddLL(const LatLong& LL)
      {
         if (nIndex<nCnt)
            pLL[nIndex++]=LL;
         if (LL.dLat<MinLL.dLat)
            MinLL.dLat=LL.dLat;
         if (LL.dLong<MinLL.dLong)
            MinLL.dLong=LL.dLong;
         if (LL.dLat>MaxLL.dLat)
            MaxLL.dLat=LL.dLat;
         if (LL.dLong>MaxLL.dLong)
            MaxLL.dLong=LL.dLong;
      }

      void Invalidate()
      {
         bValid=false;
      }

      bool bGetValid() const
      {
         return bValid;
      }

      bool bIntersect(const LatLong& DispMinLL, const LatLong& DispMaxLL) const
      {
         if (MaxLL.dLat<DispMinLL.dLat)
            return false;
         if (MinLL.dLat>DispMaxLL.dLat)
            return false;
         if (MaxLL.dLong<DispMinLL.dLong)
            return false;
         if (MinLL.dLong>DispMaxLL.dLong)
            return false;
         return true;
      }

		void DrawFill(D2DDraw* pDraw,DWG_LAYER_TYPE Layer);
		void DrawBorder(D2DDraw* pDraw,DWG_LAYER_TYPE Layer);

		void DrawEEZ(D2DDraw* pDraw,DWG_LAYER_TYPE Layer, const TColor Col);
		void DrawZone(D2DDraw* pDraw,DWG_LAYER_TYPE Layer, const TColor Col);


		void TransLL(TransGraph* pTrans, const LatLong& ScreenMin, const LatLong& ScreenMax);

		LatLong GetMinLL()   {return MinLL;}
      LatLong GetMaxLL()   {return MaxLL;}

      void SetBlock(const int nB) {nBlockNum=nB;}
      int nGetBlock() const {return nBlockNum;}

   };

	JList<GISPolygon> PolyList;

   LatLong ClipMin,ClipMax;

   struct BlockStruct
   {
      int nPolyCnt;
      int* pnPolygon;

      BlockStruct()
      {
         pnPolygon=NULL;
      }

      ~BlockStruct()
      {
         delete[] pnPolygon;
      }

      void Set(const int* pnPoly, const int nCnt)
      {
         nPolyCnt=nCnt;
         delete[] pnPolygon;
         if (nPolyCnt>0)
            {
            pnPolygon=new int[nPolyCnt];
            memcpy(pnPolygon,pnPoly,nPolyCnt*sizeof(int));
            }
         else
            pnPolygon=NULL;
      }

      JFile::FILE_ERROR Store(JFile* pFil)
      {
         JFile::FILE_ERROR E=pFil->Write(&nPolyCnt,sizeof(nPolyCnt));
         if ((!E)&&(nPolyCnt>0))
            E=pFil->Write(pnPolygon,nPolyCnt*sizeof(int));
         return E;
      }

      bool bRead(JMemFile* pFil)
      {
         bool bOK=pFil->bRead(&nPolyCnt,sizeof(nPolyCnt));
         if (bOK)
            {
            delete[] pnPolygon;
            if (nPolyCnt>0)
               {
               pnPolygon=new int[nPolyCnt];
               bOK=pFil->bRead(pnPolygon,nPolyCnt*sizeof(int));
               }
            else
               pnPolygon=NULL;
            }
         return bOK;
      }

   }* pBlock;

   enum
   {
      MAX_BLOCKS  = 360*180,
   };

   int* pnValidPolygons;
   int nValidPolyCount;

   void Normalise(LatLong& LL)
   {
      if (LL.dLat>90.0)
         LL.dLat-=180.0;
      if (LL.dLong>180.0)
         LL.dLong-=360.0;
   }

   void CheckMinMax(LatLong& MinLL, LatLong& MaxLL)
   {
      if (MinLL.dLat>MaxLL.dLat)
         {
         double dT=MinLL.dLat;
         MinLL.dLat=MaxLL.dLat;
         MaxLL.dLat=dT;
         }
      if (MinLL.dLong>MaxLL.dLong)
         {
         double dT=MinLL.dLong;
         MinLL.dLong=MaxLL.dLong;
         MaxLL.dLong=dT;
         }
   }

   int nGetBlockLat(const double dLat) const
   {
      int nLat=floor(dLat+90.0);
      if (nLat<0)
         nLat=0;
      else if (nLat>179)
         nLat=179;
      return nLat;
   }

   int nGetBlockLong(const double dLong) const
   {
      int nLong=floor(dLong+180.0);
      if (nLong<0)
         nLong=0;
      else if (nLong>359)
         nLong=359;
      return nLong;
   }

   int nGetBlockIndex(const int nLat, const int nLong) const
   {
      return nLat*360+nLong;
   }

   int nGetBlockIndex(const LatLong& LL) const
   {
      return nGetBlockIndex(nGetBlockLat(LL.dLat),nGetBlockLong(LL.dLong));
   }

   void GetBlockExtents(const int nB, LatLong& MinLL, LatLong& MaxLL)
   {
      int nLong=(nB%360)-180;
      int nLat=(nB/360)-90;
      MinLL.dLat=nLat;
      MinLL.dLong=nLong;
      MaxLL.dLat=MinLL.dLat+0.999999;
      MaxLL.dLong=MinLL.dLong+0.999999;
      CheckMinMax(MinLL,MaxLL);
   }

   void GenerateBlocks(TMessageBarForm* pMsgBar);

   int nGetMaxPolyCount(const LatLong& MinLL, const LatLong& MaxLL);
   int nGetValidPolygons(int* pnIndex, const LatLong& MinLL, const LatLong& MaxLL);


public:
	GISChart();
	~GISChart();

   EEZPolygon EEZ;
   ZonePolygon Zone;

   void SetClip(const LatLong& _ClipMin, const LatLong& _ClipMax)
   {
      ClipMin=_ClipMin;
		ClipMax=_ClipMax;
		if (ClipMin.dLat>ClipMax.dLat)
			{
			double d=ClipMin.dLat;
			ClipMin.dLat=ClipMax.dLat;
			ClipMax.dLat=d;
			}
	}

   int nImport(const String sFile, const bool bClip, TMessageBarForm* pMsgBar=NULL);
   bool bRead(const String sFile, TMessageBarForm* pMsgBar=NULL);
   bool bStore(const String sFile, TMessageBarForm* pMsgBar=NULL);

	void RegenPixels(TransGraph* pTrans, const LatLong& MinLL, const LatLong& MaxLL);

   bool bDraw(D2DDraw* pDraw,DWG_LAYER_TYPE Layer,const int nW,const int nH,const TColor ChartCol,const TColor BackCol,const bool bFill, const bool bStandAlone, const bool bFillBack);


	bool bProximity(const LatLong& LL, const int nProx_m);




};




#endif
