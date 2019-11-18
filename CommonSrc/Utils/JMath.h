//---------------------------------------------------------------------------
#ifndef JMathH
#define JMathH
//---------------------------------------------------------------------------
class MovingAverage
{

	double* pdVal;
	double dTotal;
	int nNum;
	int nSize;
	int nIndex;
   double dLastVal;

public:

   MovingAverage(const int nLength);
   ~MovingAverage();

   void Reset()
   {
      nNum=0;
      dTotal=0.0;
      nIndex=0;
      dLastVal=0.0;
   }

   double dFilter(const double dVal);

   double dFilter(); //End of filtering (empty filter array)

   double dFilled() const
   {
      return (nNum*1.0)/nSize;
   }

   double dGetLastVal() const
   {
      return dLastVal;
   }

   int nGetNum() const
   {
      return nNum;
   }

};

class Statistics
{

   double dMin;
   double dMax;
   double dSum;
   double dSumSqr;
   int nNum;

public:

	void Reset()
   {
      dMin=1e99;
      dMax=-1e99;
      dSum=0.0;
      dSumSqr=0.0;
      nNum=0;
   }

   Statistics()
   {
   	Reset();
   }

   void Add(const double dVal)
   {
      if (dVal<dMin) dMin=dVal;
      if (dVal>dMax) dMax=dVal;
      dSum+=dVal;
      dSumSqr+=(dVal*dVal);
      nNum++;
   }

   double dGetMin() const
   {
   	return dMin;
   }

   double dGetMax() const
   {
   	return dMax;
   }

   double dGetMean() const
   {
      if (nNum>0)
         return dSum/nNum;
      else
         return 0.0;
   }

   double dGetStdDev() const
   {
      if (nNum>1)
         {
         double dv=(dSumSqr-(dSum*dSum)/nNum)/(nNum-1);
         if (dv>1e-10)
            return sqrt(dv);
         else
            return 0.0;
         }
      else
         return 0.0;
   }

   int nGetNum() const
   {
      return nNum;
   }

   double dGetRMS() const
   {
      if (nNum>0)
         return sqrt(dSumSqr/nNum);
      else
         return 0.0;
   }

};

#endif
