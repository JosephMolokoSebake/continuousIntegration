//---------------------------------------------------------------------------

#ifndef JFindYTimeH
#define JFindYTimeH
//---------------------------------------------------------------------------

#include "JList.h"

class FindYTime
{

   int nPar;
   int nIndex;

   struct ValStruct
   {
      double dT;
      double adV[9];
   };

   JList<ValStruct> V;

   double dMaxTimeDiff;

   double dInterpol(const double dx1, const double dy1,const double dx2, const double dy2, const double dx) const;

   int nFindYClosest(const double dT);

public:

   FindYTime(const int _nPar, const double _dMaxTimeDiff);
   ~FindYTime();

   bool bOpenFile(const String sTitle=L"");

   bool bReadFile(const String sFile);

   int nFindY(const double dT, const bool bClosest);
   bool bFindY(const double dT, double* pdY, const bool bInterpol=true);

};


#endif
