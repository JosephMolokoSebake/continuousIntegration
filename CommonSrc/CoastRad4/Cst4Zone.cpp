//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Zone.h"
#include "JMemFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4Zone::Cst4Zone(const String sFile)
{
   if (!sFile.IsEmpty())
      Read(sFile);
}

Cst4Zone::~Cst4Zone()
{
}

void Cst4Zone::AddPoint(const LatLong& LL)
{
   if (LL.dLat<dMinLat) dMinLat=LL.dLat;
   if (LL.dLat>dMaxLat) dMaxLat=LL.dLat;
   ZoneDef* pZ=new ZoneDef;
   pZ->LL=LL;
   Def.nAdd(pZ);
   Def.Pack();
}

JFile::FILE_ERROR Cst4Zone::Read(const String sFile)
{
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFile);
   Def.Clear();
   char szSt[256];
   dMinLat=1e10;
   dMaxLat=-1e10;
   LatLong FirstLL;
   FirstLL.Invalidate();
   while(!E)
      {
      E=Fil.Read(szSt,256);
      if (!E)
         {
         double dLat,dLong;
         int n=sscanf(szSt,"%lf%lf",&dLat,&dLong);
         if (n==2)
            {
            AddPoint(LatLong(dLat,dLong));
            if (!FirstLL.bValid())
               FirstLL=LatLong(dLat,dLong);
            }
         }
      }
   if (FirstLL.bValid())
      AddPoint(FirstLL);   //Close polygon
   Def.Pack();
   return E;
}

void Cst4Zone::TransformLL(DDGraph* pTrans)
{
   for (int i=0; i<Def.nGetCount(); i++)
      Def[i]->Pix=pTrans->LL2Pix(Def[i]->LL);
}

void Cst4Zone::Draw(HDC DC)
{
   if ((Def.nGetCount()>1)&&(bDisp))
      {
      HPEN hPen=::CreatePen(PS_SOLID,1,Color);
      HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
      POINT* pPnt=new POINT[Def.nGetCount()];
      for (int i=0; i<Def.nGetCount(); i++)
         {
         pPnt[i].x=Def[i]->Pix.nx;
         pPnt[i].y=Def[i]->Pix.ny;
         }
      ::Polyline(DC,pPnt,Def.nGetCount());
      delete[] pPnt;
      ::SelectObject(DC,hOldPen);
      ::DeleteObject(hPen);
      }
}

int Cst4Zone::nFindIntersects(const double dLat, double* pdLong)
{
   int n=0;
   LatLong LL1,LL2;
   for (int i=0; i<Def.nGetCount(); i++)
      {
      LL1=Def[i]->LL;
      if (i==Def.nGetCount()-1)
         LL2=Def[0]->LL;
      else
         LL2=Def[i+1]->LL;
      if (LL1.dLat>LL2.dLat)
         {
         LatLong TempLL=LL1;
         LL1=LL2;
         LL2=TempLL;
         }
      if ((dLat>=LL1.dLat)&&(dLat<=LL2.dLat))
         {
         double dDeltaLat=LL2.dLat-LL1.dLat; //Always positive
         if (dDeltaLat>1e-10)
            pdLong[n++]=LL1.dLong+(LL2.dLong-LL1.dLong)*(dLat-LL1.dLat)/dDeltaLat;
         else
            pdLong[n++]=(LL1.dLong+LL2.dLong)/2.0;
         }
      }
   if (n>1)
      {
      //Bubble Sort
      for (int i=0; i<n-1; i++)
         for (int j=0; j<n-2; j++)
            {
            if (pdLong[j]>pdLong[j+1])
               {
               double dTemp=pdLong[j];
               pdLong[j]=pdLong[j+1];
               pdLong[j+1]=dTemp;
               }
            }
      }
   return n;
}

Cst4Zones::Cst4Zones()
{
   pIntersect=new IntersectStruct[MAX_INTERSECT];
}

Cst4Zones::~Cst4Zones()
{
   delete[] pIntersect;
}

void Cst4Zones::Add(const String sFile)
{
   Cst4Zone* pZ=new Cst4Zone(sFile);
   Zone.nAdd(pZ);
   Zone.Pack();
}

void Cst4Zones::Add(Cst4Zone& Z)
{
   Cst4Zone* pZ=new Cst4Zone;
   *pZ=Z;
   Zone.nAdd(pZ);
   Zone.Pack();
}

void Cst4Zones::TransformLL(DDGraph* pTrans)
{
   for (int i=0; i<Zone.nGetCount(); i++)
      Zone[i]->TransformLL(pTrans);
}

void Cst4Zones::Draw(HDC DC)
{
   for (int i=0; i<Zone.nGetCount(); i++)
      Zone[i]->Draw(DC);
}

void Cst4Zones::CalcIntersects(TMessageBarForm* pMB)
{
   if (pMB)
      {
      pMB->SetCaption(L"Calculating Zone Intersects...",true);
      pMB->SetProgress(0);
      }
   bool bAborted=false;
   int nPrev=-1;
   for (int nI=0; (nI<MAX_INTERSECT)&&(!bAborted); nI++)
      {
      double dLat=(nI*180.0)/MAX_INTERSECT-90.0;
      for (int nZ=0; nZ<Zone.nGetCount(); nZ++)
         {
         if ((dLat>=Zone[nZ]->dGetMinLat())&&(dLat<=Zone[nZ]->dGetMaxLat()))
            {
            double* pdLong=new double[Zone[nZ]->nGetSegments()];
            int nInter=Zone[nZ]->nFindIntersects(dLat,pdLong);
            if (nInter>0)
               pIntersect[nI].AddIntersects(nZ,pdLong,nInter);
            delete[] pdLong;
            }
         if (pMB)
            {
            int n=(nI*100)/MAX_INTERSECT;
            if (n!=nPrev)
               {
               pMB->SetProgress(n);
               nPrev=n;
               bAborted=pMB->bAbort;
               }
            }
         }
      }
   for (int nI=0; nI<MAX_INTERSECT; nI++)
      {
      if (pIntersect[nI].nCnt>0)
         pIntersect[nI].SortLong();
      }
   if (pMB)
      pMB->SetProgress(100);
}

JFile::FILE_ERROR Cst4Zones::StoreIntersects(const String sFile)
{
   JFile Fil('O');
   JFile::FILE_ERROR E=Fil.Create(sFile);
   for (int i=0; (i<MAX_INTERSECT)&&(!E); i++)
      {
      if (pIntersect[i].nCnt>0)
         {
         E=Fil.Write(&i,sizeof(i));
         if (!E)
            E=Fil.Write(&pIntersect[i].nCnt,sizeof(pIntersect[i].nCnt));
         if (!E)
            E=Fil.Write(pIntersect[i].pInter,sizeof(Intersection)*pIntersect[i].nCnt);
         }
      }
   return E;
}

JFile::FILE_ERROR Cst4Zones::ReadIntersects(const String sFile)
{
   JMemFile Fil;
   JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
   if (!E)
      {
      delete[] pIntersect;
      pIntersect=new IntersectStruct[MAX_INTERSECT];
      bool bOK=true;
      while(bOK)
         {
         int nI,nCnt;
         bOK=Fil.bRead(&nI,sizeof(nI));
         if (bOK)
            bOK=Fil.bRead(&nCnt,sizeof(nCnt));
         if ((bOK)&&(nCnt>0))
            {
            pIntersect[nI].nCnt=nCnt;
            pIntersect[nI].pInter=new Intersection[nCnt];
            bOK=Fil.bRead(pIntersect[nI].pInter,sizeof(Intersection)*nCnt);
            }
         }
      }
   return E;

}

Cst4AlarmBits Cst4Zones::InZones(const LatLong& LL)
{
   int nI=(LL.dLat+90.0)*(MAX_INTERSECT/180.0);
   if (nI<0)
      nI=0;
   else if (nI>MAX_INTERSECT-1)
      nI=MAX_INTERSECT-1;
   Cst4AlarmBits Zones;
   if (pIntersect[nI].nCnt>0)
      {
      for (int i=0; i<pIntersect[nI].nCnt; i++)
         {
         if (LL.dLong>=pIntersect[nI].pInter[i].dLong)
            ToggleZone(pIntersect[nI].pInter[i].nZone,Zones);
         }
      }
   return Zones;
}

