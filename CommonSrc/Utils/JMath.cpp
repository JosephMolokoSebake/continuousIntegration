/** CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	SciPlot Statistics Classes
 *		PROJECT			Aegis/C0953
 *		ACTIVITY       8
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium III
 *		OS					MS Windows NT4/Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *			V3.00		Initial Version	JGT 12/09/2000
 ************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JMath.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

MovingAverage::MovingAverage(const int nLength)
{
	nSize=nLength;
	pdVal=new double[nSize];
   Reset();
}

MovingAverage::~MovingAverage()
{
	delete[] pdVal;
}

double MovingAverage::dFilter(const double dVal)
{
	if (nNum<nSize)
		nNum++;
	else
		dTotal-=pdVal[nIndex];
	pdVal[nIndex]=dVal;
	dTotal+=dVal;
	if (nIndex<nSize-1)
		nIndex++;
	else
		nIndex=0;
   dLastVal=dTotal/nNum;
	return dLastVal;
}

double MovingAverage::dFilter()
{
	if (nNum>0)
      {
		nNum--;
		dTotal-=pdVal[nIndex];
      }
	if (nIndex<nSize-1)
		nIndex++;
	else
		nIndex=0;
   dLastVal=dTotal/nNum;
	return dLastVal;
}






