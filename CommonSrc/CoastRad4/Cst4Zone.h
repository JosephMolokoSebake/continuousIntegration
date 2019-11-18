//---------------------------------------------------------------------------

#ifndef Cst4ZoneH
#define Cst4ZoneH
//---------------------------------------------------------------------------

#include "JList.h"
#include "JDDGraph.h"
#include "JFile.h"
#include "JMessageBar.h"
#include "DispDefs.h"

class Cst4Zone
{

   struct ZoneDef
   {
      LatLong LL;
      Pixel Pix;

      ZoneDef()
      {
         LL.Invalidate();
         Pix.Invalidate();
      }

   };

   JList<ZoneDef> Def;

   TColor Color;
   bool bDisp;

   double dMinLat, dMaxLat;


public:

	Cst4Zone(const String sFile="");

   ~Cst4Zone();

   void AddPoint(const LatLong& LL);

   JFile::FILE_ERROR Read(const String sFile);

   void TransformLL(DDGraph* pTrans);

   void Draw(HDC DC);

   void SetColor(const TColor Col)
   {
      Color=Col;
   }

   void SetDisp(const bool bDisplay)
   {
      bDisp=bDisplay;
   }

   double dGetMinLat() const
   {
      return dMinLat;
   }

   double dGetMaxLat() const
   {
      return dMaxLat;
   }

   int nGetSegments()
   {
      return Def.nGetCount();  //NB last one is a return to origin
   }

   LatLong GetLL(const int n)
   {
      return Def[n]->LL;
   }

   int nFindIntersects(const double dLat, double* pdLong);


};


class Cst4Zones
{

   JList<Cst4Zone> Zone;

   struct Intersection
   {
      int nZone;
      double dLong;
   };

   struct IntersectStruct
   {
      int nCnt;
      Intersection* pInter;

      IntersectStruct()
      {
         nCnt=0;
         pInter=NULL;
      }

      IntersectStruct(IntersectStruct& I)
      {
         nCnt=I.nCnt;
         if ((I.pInter)&&(I.nCnt>0))
            {
            pInter=new Intersection[nCnt];
            memcpy(pInter,I.pInter,nCnt*sizeof(Intersection));
            }
         else
            pInter=NULL;
      }

      ~IntersectStruct()
      {
         delete[] pInter;
      }

      void AddIntersects(const int nZone, const double* pdLong, const int nInter)
      {
         if (pInter)
            {
            Intersection* pTemp=new Intersection[nCnt];
            memcpy(pTemp,pInter,nCnt*sizeof(Intersection));
            delete[] pInter;
            pInter=new Intersection[nInter+nCnt];
            memcpy(pInter,pTemp,nCnt*sizeof(Intersection));
            delete[] pTemp;
            for (int i=nCnt; i<nInter+nCnt; i++)
               {
               pInter[i].nZone=nZone;
               pInter[i].dLong=pdLong[i-nCnt];
               }
            nCnt+=nInter;
            }
         else
            {
            pInter=new Intersection[nInter];
            for (int i=0; i<nInter; i++)
               {
               pInter[i].nZone=nZone;
               pInter[i].dLong=pdLong[i];
               }
            nCnt=nInter;
            }
      }

      void SortLong()
      {
         if ((pInter)&&(nCnt>1))
            {
            for (int i=0; i<nCnt-1; i++)
               {
               for (int j=0; j<nCnt-2; j++)
                  {
                  if (pInter[j].dLong>pInter[j+1].dLong)
                     {
                     Intersection Temp=pInter[j];
                     pInter[j]=pInter[j+1];
                     pInter[j+1]=Temp;
                     }
                  }
               }
            }
      }

   };

   enum
   {
      MAX_INTERSECT  =  6000*180,
   };

   IntersectStruct* pIntersect;

   void ToggleZone(const int nZ, Cst4AlarmBits& Zone)
   {
      if (Zone.bSet(nZ))
         Zone.Clear(nZ);
      else
         Zone.Set(nZ);
   }

public:

   Cst4Zones();
   ~Cst4Zones();

   void Init()
   {
      Zone.Clear();
      delete[] pIntersect;
      pIntersect=new IntersectStruct[MAX_INTERSECT];
   }

   void Add(const String sFile);

   void Add(Cst4Zone& Z);

   void Add(const LatLong* pLL, const int nPoints, const bool _bDisp, const TColor _Color);

   void SetColor(const int nZone, const TColor Col)
   {
      Zone[nZone]->SetColor(Col);
   }

   void SetDisplay(const int nZone, const bool bDisp)
   {
      Zone[nZone]->SetDisp(bDisp);
   }


   void TransformLL(DDGraph* pTrans);
   void Draw(HDC DC);

   void CalcIntersects(TMessageBarForm* pMB=NULL);
   JFile::FILE_ERROR StoreIntersects(const String sFile);
   JFile::FILE_ERROR ReadIntersects(const String sFile);

   Cst4AlarmBits InZones(const LatLong& LL);


};



#endif
