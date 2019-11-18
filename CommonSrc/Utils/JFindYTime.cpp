//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JFindYTime.h"
#include "JFileDlg.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

FindYTime::FindYTime(const int _nPar, const double _dMaxTimeDiff)
{
   nPar=_nPar;
   dMaxTimeDiff=_dMaxTimeDiff;
}

FindYTime::~FindYTime()
{
}

bool FindYTime::bOpenFile(const String sTitle)
{
   String sT;
   if (sTitle.IsEmpty())
      sT=L"Select File (Value(s) vs Time)";
   else
      sT=sTitle;
   String sFile=sSelectOpenFile(L"Text Files (*.txt)",L"*.txt",sT);
   if (!sFile.IsEmpty())
      return bReadFile(sFile);
   else
      return false;
}

bool FindYTime::bReadFile(const String sFile)
{
   nIndex=0;
   V.Clear();
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFile);
   if (!E)
      {
      while(!E)
         {
         char szSt[256];
         E=Fil.Read(szSt,256);
         if (!E)
            {
            ValStruct VI;
            int n=sscanf(szSt,"%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&VI.dT,&VI.adV[0],&VI.adV[1],&VI.adV[2],&VI.adV[3],&VI.adV[4],&VI.adV[5],&VI.adV[6],&VI.adV[7],&VI.adV[8]);
            if (n>=nPar+1)
               {
               ValStruct* p=new ValStruct;
               *p=VI;
               V.nAdd(p);
               }
            }
         }
      V.Pack();
      }
   else
      Fil.ShowError(E);
   return (V.nGetCount()>0);
}

int FindYTime::nFindYClosest(const double dT)
{
   int n=-1;
   for (int i=nIndex; i<V.nGetCount(); i++)
      {
      if (fabs(dT-V[i]->dT)<=dMaxTimeDiff)
         {
         n=i;
         i=V.nGetCount();
         }
      }
   if (n>=0)
      {
      double dMinT=dT-dMaxTimeDiff;
      double dMaxT=dT+dMaxTimeDiff;
      double dMin=1e20;
      int nFound=-1;
      int nTest=n;
      int nMin=0;
      while(nTest>=0)
         {
         nMin=nTest;
         double dDiff=fabs(dT-V[nTest]->dT);
         if (dDiff<dMin)
            {
            dMin=dDiff;
            nFound=nTest;
            }
         if (V[nTest]->dT<dMinT)
            nTest=-1;            //stop search
         else
            --nTest;
         }
      nTest=n;
      while(nTest<V.nGetCount())
         {
         double dDiff=fabs(dT-V[nTest]->dT);
         if (dDiff<dMin)
            {
            dMin=dDiff;
            nFound=nTest;
            }
         if (V[nTest]->dT>dMaxT)
            nTest=V.nGetCount(); //stop search
         else
            ++nTest;
         }
      n=nFound;
      nIndex=nMin;
      if (nIndex<0)
         nIndex=0;
      }
   return n;
}

int FindYTime::nFindY(const double dT, const bool bClosest)
{
   if ((dT<V[0]->dT-1.0)||(dT>V[V.nGetCount()-1]->dT+1.0))
      return -1;
   else
      {
      if (bClosest)
         return nFindYClosest(dT);
      else
         {
         int nIncr;
         if (dT>V[nIndex]->dT)
            nIncr=1;
         else
            nIncr=-1;
         int n=-1;
         int i=nIndex;
         while((i>-1)&&(i<V.nGetCount()))
            {
            double dSec=V[i]->dT;
            if (nIncr>0)
               {
               if (dSec>=dT)
                  {
                  n=i;
                  i=-1;
                  }
               }
            else
               {
               if (dSec<=dT)
                  {
                  if (i<V.nGetCount()-1)
                     n=i+1;
                  else
                     n=i;
                  i=-1;
                  }
               }
            if (n>0)
               {
/*
               if (bClosest)
                  {
                  int nStart=n-1;
                  if (nStart<0)
                     nStart=0;
                  int nStop=n+1;
                  if (nStop>=V.nGetCount())
                     nStop=V.nGetCount()-1;
                  double dMin=1e10;
                  for (int ii=nStart; ii<=nStop; ii++)
                     {
                     double dDiff=fabs(dT-V[ii]->dT);
                     if (dDiff<dMin)
                        {
                        dMin=dDiff;
                        n=ii;
                        }
                     }
                  }
*/
               }
            if (n>=0)
               {
               if (n>0)
                  {
                  if ((fabs(dT-V[n]->dT)>dMaxTimeDiff)||(fabs(dT-V[n-1]->dT)>dMaxTimeDiff))
                     n=-1;
                  }
               else
                  {
                  if (fabs(dT-V[n]->dT)>dMaxTimeDiff)
                     n=-1;
                  }
               if (n>=0)
                  nIndex=n;
               }
            else
               i+=nIncr;
            }
         return n;
         }
      }
}

bool FindYTime::bFindY(const double dT, double* pdY, const bool bInterpol)
{
   int n2=nFindY(dT,!bInterpol);  //find value with time > T
   if (bInterpol)
      {
      if (n2>0)
         {
         for (int i=0; i<nPar; i++)
            pdY[i]=dInterpol(V[n2-1]->dT,V[n2-1]->adV[i],V[n2]->dT,V[n2]->adV[i],dT);
         }
      else if (n2==0)
         {
         for (int i=0; i<nPar; i++)
            pdY[i]=V[n2]->adV[i];
         }
      else
         return false;
      }
   else
      {
      if (n2>=0)
         {
         for (int i=0; i<nPar; i++)
            pdY[i]=V[n2]->adV[i];
         }
      else
         return false;
      }
   return true;
}

double FindYTime::dInterpol(const double dx1, const double dy1,const double dx2, const double dy2, const double dx) const
{
   double dDeltaX=dx2-dx1;
   if (fabs(dDeltaX)<1e-9)
      return dy1;
   else
      return dy1+(dy2-dy1)*(dx-dx1)/dDeltaX;
}


