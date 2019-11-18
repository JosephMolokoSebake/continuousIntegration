//---------------------------------------------------------------------------

#ifndef Cst4TWSH
#define Cst4TWSH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "JIniFile.h"
#include "JFile.h"

//======================================================

struct Cst4_TWSSetupStruct
{
//Filter
	double dAlpha;
	int nMAFilter;

//Target
	int nMaxSpeed_kts;

//Acquisition
	int nAcqMissedAbort;
	int nAcqIncMin;
	int nAcqIncScans;
	int nAcqDecMin;
	int nAcqDecScans;

//Solid
	int nSolidMissedAbort;
	int nSolidIncMin;
	int nSolidIncScans;
	int nSolidDecMin;
	int nSolidDecScans;

//Health
	int nMinHealth;
	int nMaxHealth;

//Radar Errors
	double dRaErr_m;
	double dBeErr_deg;

//Scan Correlation
	bool bCorr;
	int nCorrScans;
	int nCorrMin;

	bool bShowSolidOnly;

	Cst4_TWSSetupStruct()
	{
		dAlpha=0.1;
		nMAFilter=20;
		nMaxSpeed_kts=50;
		bCorr=false;
		nCorrScans=5;
		nCorrMin=3;
		nMinHealth=15;
		nMaxHealth=30;

		nAcqMissedAbort=3;
		nAcqIncMin=1;
		nAcqIncScans=1;
		nAcqDecMin=1;
		nAcqDecScans=1;

		nSolidMissedAbort=6;
		nSolidIncMin=1;
		nSolidIncScans=1;
		nSolidDecMin=1;
		nSolidDecScans=1;

		dRaErr_m=300.0;
		dBeErr_deg=1.5;

		bShowSolidOnly=true;

	}

	void Assign(const Cst4_TWSSetupStruct& T)
	{
		dAlpha=T.dAlpha;
		nMaxSpeed_kts=T.nMaxSpeed_kts;
		nMAFilter=T.nMAFilter;
		bCorr=T.bCorr;
		nCorrScans=T.nCorrScans;
		nCorrMin=T.nCorrMin;
		nMinHealth=T.nMinHealth;
		nMaxHealth=T.nMaxHealth;

		nAcqMissedAbort=T.nAcqMissedAbort;
		nAcqIncMin=T.nAcqIncMin;
		nAcqIncScans=T.nAcqIncScans;
		nAcqDecMin==T.nAcqDecMin;
		nAcqDecScans=T.nAcqDecScans;

		nSolidMissedAbort=T.nSolidMissedAbort;
		nSolidIncMin=T.nSolidIncMin;
		nSolidIncScans=T.nSolidIncScans;
		nSolidDecMin==T.nSolidDecMin;
		nSolidDecScans=T.nSolidDecScans;

		dRaErr_m=T.dRaErr_m;
		dBeErr_deg=T.dBeErr_deg;

		bShowSolidOnly=T.bShowSolidOnly;
	}


	Cst4_TWSSetupStruct& operator = (const Cst4_TWSSetupStruct& T)
	{
		Assign(T);
		return *this;
	}

	void Store(JIniFile* pIni, const String sHeading);
	void Read(JIniFile* pIni, const String sHeading);

	double dSqr(const double dV) const
	{
		return dV*dV;
	}

	double dATan_rad(const double dy, const double dx) const
	{
		if (fabs(dx)<1.0)
			{
			if (dy<0.0)
				return M_PI*3.0/2.0;	//270 deg
			return M_PI/2.0;			//90 deg
			}
		return atan2(dy,dx);
	}

	double dNormalise_rad(double dBe_rad) const
	{
		if (dBe_rad>M_PI*2.0)
			return dBe_rad-M_PI*2.0;
		if (dBe_rad<0.0)
			return dBe_rad+M_PI*2.0;
		return dBe_rad;
	}

	//Additional distance in tracking gate
	double dCalcSpeedRadius_m(const int nHealth, const double dDeltaT_sec)  const
	{
		double dExpandFact;
		if (nHealth<=nMinHealth)
			dExpandFact=1.0;
		else if (nHealth>=nMaxHealth)
			dExpandFact=0.0;
		else
			dExpandFact=1.0-((nHealth-nMinHealth)*1.0)/(nMaxHealth-nMinHealth);
		return 2.0*nMaxSpeed_kts*dDeltaT_sec*dExpandFact;
	}

	double dCalcErrRadius_m(const double dTargetRa_m) const
	{
		return sqrt(dSqr(2.0*dRaErr_m)+dSqr(2.0*dTargetRa_m*dBeErr_deg*M_PI/180.0));
	}

	double dCalcMaxRadius_m(const int nHealth, const double dDeltaT_sec, const double dTargetRa_m) const
	{
		return dCalcErrRadius_m(dTargetRa_m)+dCalcSpeedRadius_m(nHealth,dDeltaT_sec);
	}

	bool bInRaGate(const double dRa, const double dRa1, const double dRa2) const	//units not used
	{
		return (dRa>=dRa1)&&(dRa<=dRa2);
	}

	bool bInBeGate(const double dBe, const double dBe1, const double dBe2) const	//units not used
	{
		if (dBe2>dBe1)
			return (dBe>=dBe1)&&(dBe<=dBe2);
		else
			return (dBe>=dBe1)||(dBe<=dBe2);
	}

	bool bInErrorGate(const XYPos& Predict, const XYPos& Actual) const;

	bool bInGate(const XYPos& Predict, const XYPos& Actual, const double dSpeedRadius_m, const double dMaxRadius_m, double& dDist_m);

	bool bInGate(const XYPos& Predict, const XYPos& Actual, const double dSpeedRadius_m, const double dMaxRadius_m)
	{
		double dDist_m;	//not used
		return bInGate(Predict,Actual,dSpeedRadius_m,dMaxRadius_m,dDist_m);
	}



};

//*******************************************************************************************

class Cst4_TWS_History
{

	DWORD dwHist;
	BYTE ucIncScans;
	BYTE ucDecScans;
	BYTE ucIncMin;
	BYTE ucDecMin;
	BYTE ucMaxLen;
	BYTE ucLen;

	int nMissedAbort;
	int nMissed;
	int nMaxHealth;

	JFile* pTFil;


public:

	Cst4_TWS_History()
	{
		ucIncScans=0;
		ucDecScans=0;
		dwHist=0;
		ucLen=0;
		nMissed=0;
		pTFil=NULL;
	}

	void Init(	const int nIncMin, const int nIncScans,
					const int nDecMin, const int nDecScans,
					const int nAbortCnt, const int _nMaxHealth, JFile* pFil)
	{
		ucIncScans=(BYTE)nIncScans;
		ucIncMin=(BYTE)nIncMin;
		ucDecScans=(BYTE)nDecScans;
		ucDecMin=(BYTE)nDecMin;
		if (ucIncScans>ucDecScans)
			ucMaxLen=ucIncScans;
		else
			ucMaxLen=ucDecScans;
		nMissedAbort=nAbortCnt;
		nMaxHealth=_nMaxHealth;
		dwHist=0;
		ucLen=0;
		nMissed=0;
		pTFil=pFil;
	}

	void AddScan()
	{
		dwHist<<=1;
		dwHist|=0x00000001;
		if (ucLen<ucMaxLen)
			++ucLen;
		nMissed=0;
	}

	void SkipScan()
	{
		dwHist<<=1;
		if (ucLen<ucMaxLen)
			++ucLen;
		++nMissed;
	}

	void UpdateHealth(int& nHealth)
	{
		BYTE ucIncTot=0;
		BYTE ucDecTot=0;
		int nIncr=0;
		if (ucLen>=ucIncScans)
			{
			for (BYTE uc=0; uc<ucIncScans; uc++)
				if (dwHist&(1<<uc))
					++ucIncTot;
			if ((ucIncTot>=ucIncMin)&&(nHealth<nMaxHealth))
				nIncr=1;
			}
		else
			{
			if (dwHist&0x00000001)
				nIncr=1;
			}
		if ((nIncr==0)&&(nHealth>0))
			{
			if (ucLen>=ucDecScans)
				{
				for (BYTE uc=0; uc<ucDecScans; uc++)
					if (dwHist&(1<<uc))
						++ucDecTot;
				if (ucDecTot<ucDecMin)
				nIncr=-1;
				}
			else
				{
				if (!(dwHist&0x00000001))
					nIncr=-1;
				}
			}
      nHealth+=nIncr;
      if (pTFil)
      	{
         String s;
         if (nIncr==1)
				s.printf(L"     -- Health Incr: %d/%d >= %d)",ucIncTot,ucIncScans,ucIncMin);
         else if (nIncr==-1)
            s.printf(L"     -- Health Decr: %d/%d < %d",ucDecTot,ucDecScans,ucDecMin);
         else
         	s.printf(L"     -- Health Unchanged (incr = %d/%d, decr = %d/%d)",ucIncTot,ucIncScans,ucDecTot,ucDecScans);
         pTFil->TextLine(s);
         }
	}

	bool bAbort() const
	{
		return (nMissed>=nMissedAbort);
	}

	String sStatus()
	{
		String s;
		s.printf(L"   Inc: %2d Dec: %2d  Missed: %2d  ",ucIncScans,ucDecScans,nMissed);

		String sH="";
		for (int i=0; i<16; i++)
			{
			if ((1<<i)&dwHist)
				sH="1"+sH;
			else
				sH="0"+sH;
			}
		return s+sH;
	}

};

//*******************************************************************************************

class Cst4_TWS_Track
{

	JFile* pTFil;

	Cst4_TWSSetupStruct* pSetup;
	Transform* pFlatTrans;
	Transform* pRoundTrans;

	Cst4_TWS_History SolidHist;
	Cst4_TWS_History AcqHist;

	WORD wSource;

	int nTrackID;
	Cst4_Radar_Track* pTrackMsg;

	double dBeta;
	double dMaxVel_mps;

	XYPos StartXY,PredXY,VelXY,SmoothedXY;
	double dPredRa_m;

//	Polar PredPol;

	double dScanTime_utc_sec;
	int nScanIndex;

	double dDeltaT_sec;

	double dSpeedRadius_m;
	double dMaxRadius_m;
	double dMinDist_m;
	XYPos InXY;

	int nCount;
	int nValid;
	double dPrevTime_utc_sec;
	int nHealth;
	bool bSolid;
	bool bSmoothValid;

	bool bForceAbort;

	XYPos* pMABuf;
	double* pdMATime_utc_sec;
	XYPos* pMAVel;
	int nMACnt;
	int nMAIndex;
	XYPos MATot;
	XYPos MAVelTot;

	void Next_utc(const double dTime_utc_sec, const XYPos& XY);
	void NextPos_utc(const double dTime_utc_sec, const XYPos& XY, int& nCurrentID);
	void NextPos_utc(const double dTime_utc_sec);

	double dSqr(const double dV) const
	{
		return dV*dV;
	}

	double dMAFilter_utc_sec(const XYPos& XY, const XYPos& Vel, const double dT_utc_sec, XYPos& XY_Filt, XYPos& Vel_Filt);


public:

	Cst4_TWS_Track(Cst4_TWSSetupStruct* _pSetup, Transform* _pFlatTrans, Transform* _pRoundTrans, const XYPos& PlotXY, const double dT_utc_sec, const WORD _wSource, const int nScan);

	~Cst4_TWS_Track()
	{
		delete[] pMAVel;
		delete[] pdMATime_utc_sec;
		delete[] pMABuf;
		delete pTrackMsg;
		delete pTFil;
	}

	void Assign(const Cst4_TWS_Track& M)
	{


//      Setup=M.Setup;
	}

	Cst4_TWS_Track& operator = (const Cst4_TWS_Track& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_TWS_Track(const Cst4_TWS_Track& M)
	{
		Assign(M);
	}

	void ResetFilters()
	{
		VelXY=XYPos(0.0,0.0);
		delete[] pdMATime_utc_sec;
		delete[] pMABuf;
		delete[] pMAVel;
		pMABuf=new XYPos[pSetup->nMAFilter];
		pdMATime_utc_sec=new double[pSetup->nMAFilter];
		pMAVel=new XYPos[pSetup->nMAFilter];
		MATot=XYPos(0.0,0.0);
		MAVelTot=XYPos(0.0,0.0);
		nMACnt=0;
		nMAIndex=0;
	}

	void InitScan_utc(const double dT_utc_sec, const int nScan)
	{
		dScanTime_utc_sec=dT_utc_sec;
		nScanIndex=nScan;
		dMinDist_m=1e10;
		InXY.Invalidate();
		dSpeedRadius_m=pSetup->dCalcSpeedRadius_m(nHealth,dScanTime_utc_sec-dPrevTime_utc_sec);
		dMaxRadius_m=pSetup->dCalcErrRadius_m(dPredRa_m)+dSpeedRadius_m;
	}

	bool bAddPlot(const XYPos XY)
	{
		if (pSetup->bInGate(PredXY,XY,dSpeedRadius_m,dMaxRadius_m))
			{
			InXY=XY;
			return true;
			}
		else
			{
			InXY.Invalidate();
			return false;
			}
	}

	double dPlotMinDist_m() const
	{
		return dMinDist_m;
	}

	void CalcTrack(int& nCurrentID);

	Cst4_Radar_Track& GetTrackMsg()
	{
      return *pTrackMsg;
   }

   Cst4_Radar_Track* pGetTrackMsg()
   {
      return pTrackMsg;
   }

	bool bAborted() const
	{
		if (bSolid)
			return (SolidHist.bAbort() || (nHealth<=0) || bForceAbort);
		else
			return (AcqHist.bAbort() || bForceAbort) || (nHealth<0);
	}

	bool bSolidTrack() const
	{
		return bSolid;
   }

	int nGetHealth() const
   {
      return nHealth; 
   }

   int nGetTrackID() const
	{
		return nTrackID;
	}

	bool bValidID() const
	{
		return (nTrackID>=0);
	}

	XYPos GetPredXY() const
	{
      return PredXY;
   }

   void SetMaxRadius(const double dMax_m)
   {
      dMaxRadius_m=dMax_m;
   }

   double dGetMaxRadius() const
   {
      return dMaxRadius_m;
   }

};

//*******************************************************************************************

class Cst4_TWS_Plot
{

	XYPos XY;
	double dMinDist_m;
	Cst4_TWS_Track* pTrack;
	bool bUsed;
	bool bValid;

	double dSqr(const double dV) const
	{
		return dV*dV;
	}

	double dGetDist_m(const XYPos& XY1) const
	{
		return sqrt(dSqr(XY.dx-XY1.dx)+dSqr(XY.dy-XY1.dy));
	}


public:

	Cst4_TWS_Plot()
	{
		Init();
	}

	void SetXY(const XYPos& _XY)
	{
		XY=_XY;
		bValid=true;
	}

	XYPos GetXY() const
	{
		return XY;
	}

	void AddTrack(Cst4_TWS_Track* pNewTrack, const double dMaxRadius_m);

	void Init()
	{
		pTrack=NULL;
		dMinDist_m=1e10;
		bUsed=false;
		bValid=false;
	}

	Cst4_TWS_Track* pGetTrack()
	{
		return pTrack;
	}

	double dGetMinDist_m() const
	{
		return dMinDist_m;
	}

   void SetUsed(const bool _bUsed)
   {
      bUsed=_bUsed;
   }

   bool bGetUsed() const
   {
      return bUsed;
   }

	void SetValid(const bool _bValid)
	{
      bValid=_bValid;
   }

	bool bGetValid() const
	{
		return bValid;
   }

};

//*******************************************************************************************

class Cst4_TWS_Scan   /* : public Transform*/
{

	JFile* pTestFile;
//	TRANSFORM_TYPE TransformType;
	Cst4_TWSSetupStruct Setup;
	JList<Cst4_TWS_Track> TWS;
	JList<Cst4_Radar_Track> Tracks;
	int nCurrentID;

	Transform FlatTrans;
	Transform RoundTrans;

	struct HistScan
	{
		int nSize;
		double dT_utc_sec;
		XYPos* pXY;

		HistScan()
		{
			pXY=NULL;
		}

		~HistScan()
		{
			delete[] pXY;
		}

		void Assign(HistScan& T)
		{
			delete[] pXY;
			nSize=T.nSize;
			if (nSize>0)
				{
				pXY=new XYPos[nSize];
				memcpy(pXY,T.pXY,nSize*sizeof(XYPos));
				}
			else
				pXY=NULL;
			dT_utc_sec=T.dT_utc_sec;
		}

		HistScan& operator = (HistScan& T)
		{
			Assign(T);
			return *this;
		}

		HistScan(HistScan& T)
		{
			Assign(T);
		}

		void Init_utc(const double _dT_utc_sec, const int _nSize)
		{
			nSize=_nSize;
			delete[] pXY;
			pXY=new XYPos[nSize];
			dT_utc_sec=_dT_utc_sec;
		}

	}* pHist;

	int nMaxHistScans;
	int nHistScanCnt;
	int nHistScanIndex;

	int nScanCnt;

	double dStop_utc_sec;

	void Correlate(Cst4_Radar_Scan& Scan);


public:

	Cst4_TWS_Scan(Cst4_TWSSetupStruct& _Setup,/* const TRANSFORM_TYPE _TransformType, */ const int nFirstID);

	~Cst4_TWS_Scan()
	{
		delete[] pHist;
		delete pTestFile;
	}

	void Assign(Cst4_TWS_Scan& T)
	{
		Setup=T.Setup;
		TWS=T.TWS;
		delete[] pHist;
		nMaxHistScans=T.nMaxHistScans;
		if (T.nMaxHistScans>0)
			{
			pHist=new HistScan[nMaxHistScans];
			for (int i=0; i<T.nHistScanCnt; i++)
				pHist[i]=T.pHist[i];
			}
		nHistScanCnt=T.nHistScanCnt;
		nHistScanIndex=T.nHistScanIndex;
		nScanCnt=T.nScanCnt;
		Tracks=T.Tracks;
	}

	Cst4_TWS_Scan& operator = (Cst4_TWS_Scan& T)
	{
		Assign(T);
		return *this;
	}

	Cst4_TWS_Scan(Cst4_TWS_Scan& T)
	{
		Assign(T);
	}

   void NewSetup(Cst4_TWSSetupStruct& _Setup)
   {
      Setup=_Setup;
   }

   void Update(Cst4_Radar_Scan& IScan);

   int nGetTrackCount()
   {
      return Tracks.nGetCount();
   }

   Cst4_Radar_Track& GetTrackMsg(const int n)
   {
      return *Tracks[n];
   }

   Cst4_Radar_Track* pGetTrackMsg(const int n)
   {
      return Tracks[n];
   }

};





#endif
