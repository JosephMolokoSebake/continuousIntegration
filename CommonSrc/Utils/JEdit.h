//---------------------------------------------------------------------------

#ifndef JEditH
#define JEditH
//---------------------------------------------------------------------------

#include "jpch.h"
#include "JTransform.h"
#include "JList.h"
#include "JTime.h"

class JEdit
{

   enum DATA_TYPE
   {
      INT_TYPE,
      DOUBLE_TYPE,
   };

	struct EditStruct
	{
		String sDescr;
		TEdit* pEd1;
		TEdit* pEd2;
//      TDateTimePicker* pDatePick;
//      TDateTimePicker* pTimePick;
      double dMin;
      double dMax;
      int nDeci;
      int nMin;
      int nMax;
      DATA_TYPE DataType;

		EditStruct()
		{
			sDescr="";
			pEd1=NULL;
         pEd2=NULL;
//         pDatePick=NULL;
//         pTimePick=NULL;
         dMin=-10000000.0;
         dMax=10000000.0;
         nDeci=2;
         nMin=-999999;
         nMax=999999;
         DataType=INT_TYPE;
      }

      bool bCheckValid(const double d, const int nParCnt);


	};

   struct PanelStruct
   {
      String sDescr;
      TPanel* pPan;

      PanelStruct()
      {
         sDescr="";
         pPan=NULL;
      }

   };


   JList<EditStruct> EditList;

   JList<PanelStruct> PanList;

   bool bDataValid;

   int nLastIdx;

   int nInvalid;
   double dInvalid;

	void SetVal(TEdit* pEd, const double d, const int nDeci);
  	void SetLL(TEdit* pLatEd, TEdit* pLongEd, const LatLong LL);

   int nFindIndex(TEdit* pEd1, const bool bVerbose=true);
   int nFindIndex(TEdit* pEd1, TEdit* pEd2, const bool bVerbose=true);

   EditStruct* pCheckIndex(const int nIdx, const String sDescr);

public:

	JEdit();
	~JEdit();

	void Init()
	{
      nLastIdx=-1;
		EditList.Clear();
		bDataValid=true;
      PanList.Clear();
	}

	void SetValid()
	{
      bDataValid=true;
   }

	bool bValid()
	{
      if (!bDataValid)
         {
         SetValid();
         return false;
         }
      else
      	return true;
	}

   int nGetInvalid() const
   {
      return nInvalid;
   }

   double dGetInvalid() const
   {
      return dInvalid;
   }

	//Set ReadOnly Values
	void SetValRO(TEdit* pEd,const int n);
	void SetValRO(TEdit* pEd,const DWORD dw);
	void SetValRO(TEdit* pEd,const double d, const int nDeci=-1);
   void SetLLRO(TEdit* pLatEd, TEdit* pLongEd, const LatLong LL);
	void SetTimeRO(TEdit* pTimeEd, const JTime& T);
	void SetTimeRO(TEdit* pTimeEd, const int& nT_ms);
	void SetDateRO(TEdit* pDateEd, const JTime& T);
	void SetDateTimeRO(TEdit* pDateEd, TEdit* pTimeEd, const JTime& T);
   void SetBinRO(TEdit* pEd, const int nVal, const int nBits=8);
   void SetHexRO(TEdit* pEd, const int nVal, const int nBits=8);

   //Integers
	int nSetVal(TEdit* pEd, const int n, const String sDescr, const int nMin=-9999, const int nMax=9999);
	int nSetVal(const String sDescr, TEdit* pEd, const int n);
	void SetVal(const int nIdx, const int nVal);

   int nGetVal(const int nIdx);   //Use Min/Max values from SetLimits()
	int nGetVal(const int nIdx, const int nMin, const int nMax);
   void GetVal(const int nIdx, int& nVal);   //Use Min/Max values from SetLimits()
	void GetVal(const int nIdx, int& nVal, const int nMin, const int nMax);

   int nGetVal(TEdit* pEd);        //Use Min/Max values from SetLimits()
	int nGetVal(TEdit* pEd, const int nMin, const int nMax);
	void GetVal(TEdit* pEd, int& nVal);
	void GetVal(TEdit* pEd, int& nVal, int nMin, const int nMax);


   //Double
	int nSetVal(TEdit* pEd, const double d, const int nDeci, const String sDescr, const double dMin=-1e9, const double dMax=1e9);
	int nSetVal(const String sDescr, TEdit* pEd, const double d, const int nDeci);
	void SetVal(const int nIdx, const double dVal, const int nDeci);

   double dGetVal(const int nIdx); //Use Min/Max values from SetLimits()
	double dGetVal(const int nIdx, const double dMin, const double dMax);

	double dGetVal(TEdit* pEd);       //Use Min/Max values from SetLimits()
	double dGetVal(TEdit* pEd, const double dMin, const double dMax);
	void GetVal(TEdit* pEd, double& dVal, const double dMin, const double dMax);
	void GetVal(TEdit* pEd, double& dVal);       //Use Min/Max values from SetLimits()

   //LatLong
  	int nSetLL(const String sDescr, TEdit*pLatEd, TEdit*pLongEd, const LatLong LL);
	void SetLL(const int nIdx, const LatLong LL);

	LatLong GetLL(const int nIdx);
	void GetLL(const int nIdx, LatLong& LL);

   LatLong GetLL(TEdit* pLatEd, TEdit* pLongEd);
	void GetLL(TEdit* pLatEd, TEdit* pLongEd, LatLong& LL);

   //DateTime
   void SetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime, JTime T);
   void SetDate(TDateTimePicker* pDate,JTime T);
   void SetTime(TDateTimePicker* pTime, JTime T);

   JTime GetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime);
   void GetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime, JTime& T);
   JTime GetDate(TDateTimePicker* pDate);
   void GetDate(TDateTimePicker* pDate, JTime& T);
   JTime GetTime(TDateTimePicker* pTime);
   void GetTime(TDateTimePicker* pTime, JTime& T);

   void SetLimits(TEdit* pEd, const String sDescr, const double dMin, const double dMax);
   void SetLastLimits(const String sDescr, const double dMin, const double dMax);
   void SetLastLimits(const double dMin, const double dMax);

   void SetPanel(const String sDescr, TPanel* pPan, TColor Col);
   void SelectPanel(TPanel* pPan, const TColor BackCol=clBlack);
   TColor GetColor(TPanel* pPan);




};

#endif
