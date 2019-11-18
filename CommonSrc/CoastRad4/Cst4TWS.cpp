//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4TWS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Cst4_TWSSetupStruct::Store(JIniFile* pIni, const String sHeading)
{
	pIni->Write(sHeading,"Alpha",dAlpha);
	pIni->Write("Moving Average Filter",nMAFilter);
	pIni->Write("Max Speed",nMaxSpeed_kts);
	pIni->Write("Abort after missed scans (acquisition)",nAcqMissedAbort);
	pIni->Write("Health Incr Minimum (acquisition)",nAcqIncMin);
	pIni->Write("Health Incr Scans (acquisition)",nAcqIncScans);
	pIni->Write("Health Decr Minimum (acquisition)",nAcqDecMin);
	pIni->Write("Health Decr Scans (acquisition)",nAcqDecScans);
	pIni->Write("Abort after missed scans (solid)",nSolidMissedAbort);
	pIni->Write("Health Incr Minimum (solid)",nSolidIncMin);
	pIni->Write("Health Incr Scans (solid)",nSolidIncScans);
	pIni->Write("Health Decr Minimum (solid)",nSolidDecMin);
	pIni->Write("Health Decr Scans (solid)",nSolidDecScans);
	pIni->Write("Use Scan Correlation",bCorr);
	pIni->Write("Scan Correlation Scans",nCorrScans);
	pIni->Write("Scan Correlation Minimum",nCorrMin);
	pIni->Write("Min Solid Health",nMinHealth);
	pIni->Write("Max Solid Health",nMaxHealth);
	pIni->Write("Radar Error Range",dRaErr_m);
	pIni->Write("Radar Error Bearing",dBeErr_deg);
	pIni->Write("Report Solid Tracks Only",bShowSolidOnly);
}

void Cst4_TWSSetupStruct::Read(JIniFile* pIni, const String sHeading)
{
	pIni->Read(sHeading,"Alpha",dAlpha,0.1);
	pIni->Read("Moving Average Filter",nMAFilter,10);
	pIni->Read("Max Speed",nMaxSpeed_kts,50);
	pIni->Read("Abort after missed scans (acquisition)",nAcqMissedAbort,3);
	pIni->Read("Health Incr Minimum (acquisition)",nAcqIncMin,1);
	pIni->Read("Health Incr Scans (acquisition)",nAcqIncScans,1);
	pIni->Read("Health Decr Minimum (acquisition)",nAcqDecMin,1);
	pIni->Read("Health Decr Scans (acquisition)",nAcqDecScans,1);
	pIni->Read("Abort after missed scans (solid)",nSolidMissedAbort,6);
	pIni->Read("Health Incr Minimum (solid)",nSolidIncMin,1);
	pIni->Read("Health Incr Scans (solid)",nSolidIncScans,1);
	pIni->Read("Health Decr Minimum (solid)",nSolidDecMin,1);
	pIni->Read("Health Decr Scans (solid)",nSolidDecScans,1);
	pIni->Read("Use Scan Correlation",bCorr,false);
	pIni->Read("Scan Correlation Scans",nCorrScans,5);
	pIni->Read("Scan Correlation Minimum",nCorrMin,1);
	pIni->Read("Min Solid Health",nMinHealth,5);
	pIni->Read("Max Solid Health",nMaxHealth,15);
	pIni->Read("Radar Error Range",dRaErr_m,100.0);
	pIni->Read("Radar Error Bearing",dBeErr_deg,1.0);
	pIni->Read("Report Solid Tracks Only",bShowSolidOnly,true);
}

inline bool Cst4_TWSSetupStruct::bInErrorGate(const XYPos& Predict, const XYPos& Actual) const
{
	double dRa_m=sqrt(dSqr(Actual.dx)+dSqr(Actual.dy));
	double dRa1_m=sqrt(dSqr(Predict.dx)+dSqr(Predict.dy));
	double dRa2_m=dRa1_m+dRaErr_m;
	dRa1_m-=dRaErr_m;
	if (!bInRaGate(dRa_m,dRa1_m,dRa2_m))
		return false; 	//outside range error
	double dBe_rad=dNormalise_rad(dATan_rad(Actual.dy,Actual.dx));
	double dBe1_rad=dATan_rad(Predict.dy,Predict.dx);
	double dBe2_rad=dNormalise_rad(dBe1_rad+dBeErr_deg*M_PI/180.0);
	dBe1_rad=dNormalise_rad(dBe1_rad-dBeErr_deg*M_PI/180.0);
	return bInBeGate(dBe_rad,dBe1_rad,dBe2_rad);
}



inline bool Cst4_TWSSetupStruct::bInGate(const XYPos& Predict, const XYPos& Actual, const double dSpeedRadius_m, const double dMaxRadius_m, double& dDist_m)
{
	double dx_m=Actual.dx-Predict.dx;
	double dy_m=Actual.dy-Predict.dy;
	dDist_m=sqrt(dSqr(dx_m)+dSqr(dy_m));
	if (dDist_m>dMaxRadius_m)  //Check if targets are very far apart
		return false;	//targets too far apart
	if (dDist_m<=dSpeedRadius_m)
		return true;   //target within speed radius
	if (dSpeedRadius_m<1.0)
		return bInErrorGate(Predict,Actual); //Check Radar Errors only
	//Get intersection of line between Actual and Predicted on SpeedRadius circle
	double dTheta_rad=dATan_rad(dy_m,dx_m);
	double dx1_m=Predict.dx+dSpeedRadius_m*cos(dTheta_rad);	//x position of intersection
	double dy1_m=Predict.dy+dSpeedRadius_m*sin(dTheta_rad);	//y position of intersection
	return bInErrorGate(XYPos(dx1_m,dy1_m),Actual); 				//Check if edge position can fall in Radar Errors
}

Cst4_TWS_Track::Cst4_TWS_Track(Cst4_TWSSetupStruct* _pSetup, Transform* _pFlatTrans, Transform* _pRoundTrans, const XYPos& PlotXY, const double dT_utc_sec, const WORD _wSource, const int nScan)
{
	memset(this,0,sizeof(Cst4_TWS_Track));
	pMABuf=NULL;
	pdMATime_utc_sec=NULL;
	pMAVel=NULL;
	pSetup=_pSetup;
	pFlatTrans=_pFlatTrans;
	pRoundTrans=_pRoundTrans;

	pTFil=NULL;
//	pTFil=new JFile('O',JFile::ASCII_TYPE);

	if (pTFil)
		{
		static int nF=0;
		String s;
		s.printf(L"OTB Track Test %05d.txt",nF++);
		pTFil->Create(s);
		}

	AcqHist.Init(	pSetup->nAcqIncMin,pSetup->nAcqIncScans,
						pSetup->nAcqDecMin,pSetup->nAcqDecScans,
						pSetup->nAcqMissedAbort,pSetup->nMaxHealth,pTFil);
	SolidHist.Init(pSetup->nSolidIncMin,pSetup->nSolidIncScans,
						pSetup->nSolidDecMin,pSetup->nSolidDecScans,
						pSetup->nSolidMissedAbort,pSetup->nMaxHealth,pTFil);
	pTrackMsg=new Cst4_Radar_Track;
	ResetFilters();
	dPredRa_m=sqrt(dSqr(PlotXY.dx)+dSqr(PlotXY.dy));
	StartXY=PlotXY;
	PredXY=StartXY;
	dDeltaT_sec=-1.0;
	dPrevTime_utc_sec=dT_utc_sec;
	wSource=_wSource;
	nTrackID=-1; 	//will only get ID if Acq track
	dBeta=2.0*(2.0-pSetup->dAlpha)-4.0*sqrt(1.0-pSetup->dAlpha);
	dMaxVel_mps=pSetup->nMaxSpeed_kts/2.0;
	VelXY=XYPos(0.0,0.0);
	nCount=0;
	bSolid=false;
	nHealth=0;
	bSmoothValid=false;

	nScanIndex=nScan;

	if (pTFil)
		{
		String s;
		s.printf(L"INIT %3d X: %6.0f  Y:%6.0f",nScanIndex,StartXY.dx,StartXY.dy);
		pTFil->TextLine(s);
		}

}


void Cst4_TWS_Track::Next_utc(const double dTime_utc_sec, const XYPos& InXY)
{
	if (dPrevTime_utc_sec>=0.0)
		dDeltaT_sec=dTime_utc_sec-dPrevTime_utc_sec;
	if (dDeltaT_sec<0.001)
		dDeltaT_sec=0.001;
	else if (dDeltaT_sec>3600.0)
		dDeltaT_sec=3600.0;
	dPrevTime_utc_sec=dTime_utc_sec;
	if (nCount==0)
		{
		PredXY=InXY;
		StartXY=InXY;
		SmoothedXY=InXY;
		nCount++;
		}
	else
		{
		SmoothedXY.dx=PredXY.dx+pSetup->dAlpha*(InXY.dx-PredXY.dx);
		SmoothedXY.dy=PredXY.dy+pSetup->dAlpha*(InXY.dy-PredXY.dy);
		if (nCount==1)
			{
			VelXY.dx=(InXY.dx-StartXY.dx)/dDeltaT_sec;
			VelXY.dy=(InXY.dy-StartXY.dy)/dDeltaT_sec;
			nCount++;
			}
		else
			{
			VelXY.dx=VelXY.dx+(dBeta/dDeltaT_sec)*(InXY.dx-PredXY.dx);
			VelXY.dy=VelXY.dy+(dBeta/dDeltaT_sec)*(InXY.dy-PredXY.dy);
			}
		double dVel_mps=sqrt(dSqr(VelXY.dx)+dSqr(VelXY.dy));
		if (dVel_mps>dMaxVel_mps)
			{
			double dFact=dMaxVel_mps/dVel_mps;
			VelXY.dx*=dFact;
			VelXY.dy*=dFact;
			}
		PredXY.dx=SmoothedXY.dx+dDeltaT_sec*VelXY.dx;
		PredXY.dy=SmoothedXY.dy+dDeltaT_sec*VelXY.dy;
		}
	dPredRa_m=sqrt(dSqr(PredXY.dx)+dSqr(PredXY.dy));
	bSmoothValid=true;
}

void Cst4_TWS_Track::NextPos_utc(const double dTime_utc_sec, const XYPos& XY, int& nCurrentID)
{
	if (pTFil)
		{
		String s;
		s.printf(L"%3d X: %6.0f  Y:%6.0f  Health: %2d",nScanIndex,XY.dx,XY.dy,nHealth);
		if (bSolid)
			pTFil->TextLine("SOLID "+s);
		else
			pTFil->TextLine("ACQ "+s);
		}
	SolidHist.AddScan();	//Always update SolidHist (to build up history)
	if (bSolid)
		{
		SolidHist.UpdateHealth(nHealth);
		}
	else
		{
		//Acquisition mode
		AcqHist.AddScan();	//AcqHist is only used until solid track
		AcqHist.UpdateHealth(nHealth);
		if (nHealth>=pSetup->nMinHealth)
			{
			bSolid=true;	//never returns to acq mode
			nTrackID=nCurrentID++;	//Assign ID only to solid track - then incr ID
			}
		}

	if (pTFil)
		{
      if (bSolid)
			pTFil->TextLine("    Solid: "+SolidHist.sStatus());
      else
			pTFil->TextLine("    Acq: "+AcqHist.sStatus());
		pTFil->TextLine("");
		}

	Next_utc(dTime_utc_sec,InXY);
}

void Cst4_TWS_Track::NextPos_utc(const double dTime_utc_sec)
{

	if (pTFil)
		{
		String s;
		s.printf(L"*** Memory Track  Scan %3d H: %2d",nScanIndex,nHealth);
		pTFil->TextLine(s);
		}
	SolidHist.SkipScan();	//Always update SolidHist
	if (bSolid)
		SolidHist.UpdateHealth(nHealth);
	else
		{
		//Acquisition mode
		AcqHist.SkipScan();	//AcqHist is only used until solid track
		AcqHist.UpdateHealth(nHealth);
		}
	if (nCount>0)
		Next_utc(dTime_utc_sec,PredXY);
	else
		{
		SmoothedXY=XYPos(0.0,0.0);
		bSmoothValid=false;
		}

	if (pTFil)
		{
		if (bSolid)
			pTFil->TextLine("    Solid: "+SolidHist.sStatus());
		else
			pTFil->TextLine("    Acq: "+AcqHist.sStatus());
		pTFil->TextLine("");
		}


}

double Cst4_TWS_Track::dMAFilter_utc_sec(const XYPos& XY, const XYPos& Vel, const double dT_utc_sec, XYPos& XY_Filt, XYPos& Vel_Filt)
{
	double dT1_utc_sec;
	if (nMACnt<pSetup->nMAFilter)
		{
		if (nMACnt==0)
			dT1_utc_sec=dT_utc_sec;
		else
			dT1_utc_sec=pdMATime_utc_sec[0];
		++nMACnt;
		}
	else
		{
		MATot.dx-=pMABuf[nMAIndex].dx;
		MATot.dy-=pMABuf[nMAIndex].dy;
      MAVelTot.dx-=pMAVel[nMAIndex].dx;
      MAVelTot.dy-=pMAVel[nMAIndex].dy;
		dT1_utc_sec=pdMATime_utc_sec[nMAIndex];
		}
	pMABuf[nMAIndex]=XY;
	pdMATime_utc_sec[nMAIndex]=dT_utc_sec;
	MATot.dx+=XY.dx;
	MATot.dy+=XY.dy;
	MAVelTot.dx+=Vel.dx;
	MAVelTot.dy+=Vel.dy;
	if (nMAIndex<pSetup->nMAFilter-1)
		++nMAIndex;
	else
		nMAIndex=0;
	XY_Filt=XYPos(MATot.dx/nMACnt,MATot.dy/nMACnt);
	Vel_Filt=XYPos(MAVelTot.dx/nMACnt,MAVelTot.dy/nMACnt);
	double dTrackTime_utc_sec=(dT_utc_sec+dT1_utc_sec)/2.0;
	return dTrackTime_utc_sec;
}

void Cst4_TWS_Track::CalcTrack(int& nCurrentID)
{
	if (InXY.bValid())
		NextPos_utc(dScanTime_utc_sec,InXY,nCurrentID);
	else
		NextPos_utc(dScanTime_utc_sec);//Memory track
	XYPos MA_XY,MA_Vel;
	double dT_utc_sec=dMAFilter_utc_sec(SmoothedXY,VelXY,dScanTime_utc_sec,MA_XY,MA_Vel);
	MA_Vel=VelXY;    //Do not use MA filtered Vel
	double dSpeed_kts=sqrt(dSqr(MA_Vel.dx)+dSqr(MA_Vel.dy))*2.0;
	double dCourse_deg;
	if (fabs(MA_Vel.dy)>1e-10)
		dCourse_deg=atan2(MA_Vel.dx,MA_Vel.dy)*180.0/M_PI;
	else
		if (MA_Vel.dx>0.0)
			dCourse_deg=90.0;
		else
			dCourse_deg=270.0;
	if (pSetup->nMAFilter>1)
		{
		//Extrapolate target position to account for MA delay
		if (dDeltaT_sec>0.0)
			{
			double dDelay_sec=dDeltaT_sec*pSetup->nMAFilter/2.0;
			MA_XY.dx+=MA_Vel.dx*dDelay_sec;
			MA_XY.dy+=MA_Vel.dy*dDelay_sec;
			}
		}
	Polar FlatPol=pFlatTrans->XY2Pol(MA_XY);
	LatLong LL=pRoundTrans->Pol2LL(FlatPol);
	if (nTrackID>=0)
		pTrackMsg->Set(JTime(dT_utc_sec,true),nTrackID,LL,dSpeed_kts,dCourse_deg,nHealth,wSource);
}

void Cst4_TWS_Plot::AddTrack(Cst4_TWS_Track* pNewTrack, const double dMaxRadius_m)
{
	XYPos XY=pNewTrack->GetPredXY();
	double dNewDist_m=dGetDist_m(XY);
	if (dNewDist_m<dMaxRadius_m)
		{
		bool bUseNew=true;
		if (pTrack)
			{
			if (pTrack->bSolidTrack() && pNewTrack->bSolidTrack())
				{
				bUseNew=(dNewDist_m<dMinDist_m);	//always take closest track
				}
			else if (pTrack->bSolidTrack() && !pNewTrack->bSolidTrack())
				{
				bUseNew=(dNewDist_m<dMinDist_m/2.0);	//use new track if it is much closer than old track
				}
			else if (!pTrack->bSolidTrack() && pNewTrack->bSolidTrack())
				{
				bUseNew=(dNewDist_m<dMinDist_m*2.0);   //use new track except if it is much further than old track
				}
			else
				{
				bUseNew=(dNewDist_m<dMinDist_m);	//always take closest track
				}
			}
		if (bUseNew)
			{
			dMinDist_m=dNewDist_m;
			pTrack=pNewTrack;
			}
		}
}

Cst4_TWS_Scan::Cst4_TWS_Scan(Cst4_TWSSetupStruct& _Setup,/* const TRANSFORM_TYPE _TransformType, */const int nFirstID)
{
	Setup=_Setup;
//	TransformType=_TransformType;
	nHistScanCnt=0;
	nMaxHistScans=0;
	nHistScanIndex=0;
	pHist=NULL;
	nScanCnt=0;
	nCurrentID=nFirstID;
	pTestFile=NULL;
//		pTestFile=new JFile('O',JFile::ASCII_TYPE);

	if (pTestFile)
		pTestFile->Create("OTB Scan.txt");

}


void Cst4_TWS_Scan::Correlate(Cst4_Radar_Scan& Scan)
{
	if (!pHist)
		{
		nMaxHistScans=Setup.nCorrScans;
		nHistScanIndex=0;
		nHistScanCnt=0;
		pHist=new HistScan[nMaxHistScans];
      }

	int nPlotCnt=Scan.nGetPlotCnt();
	if (nPlotCnt>0)
		{
		HistScan CS;	//Current Scan
		CS.Init_utc(Scan.dTime_sec_utc(),nPlotCnt);
      Polar Pol;
      XYPos XY;
      for (int nP=0; nP<nPlotCnt; nP++)
			{
			Pol=Polar(Scan.pPlot(nP)->dRa_m(),Scan.pPlot(nP)->dBe_deg());
			XY=XYPos(FlatTrans.Pol2XY(Pol));
         CS.pXY[nP]=XY;
         int nPlotTotal=0;
         double dErrRadius_m=Setup.dCalcErrRadius_m(Pol.dRa);
         for (int nS=0; nS<nHistScanCnt; nS++)
            {
            if (nS!=nHistScanIndex)
               {
					double dSpeedRadius_m=Setup.dCalcSpeedRadius_m(0,CS.dT_utc_sec-pHist[nS].dT_utc_sec);
               for (int i=0; i<pHist[nS].nSize; i++)
                  {
                  if (Setup.bInGate(pHist[nS].pXY[i],XY,dSpeedRadius_m,dSpeedRadius_m+dErrRadius_m))
                     {
                     ++nPlotTotal;
                     i=pHist[nS].nSize;	//stop loop
                     }
                  }
               }
            }
         if (nPlotTotal<Setup.nCorrMin)
            Scan.InvalidatePlot(nP) ;
         }
		pHist[nHistScanIndex++]=CS;
		if (nHistScanIndex>=nMaxHistScans)
			nHistScanIndex=0;
		if (nHistScanCnt<nMaxHistScans)
			++nHistScanCnt;
		}
}

void Cst4_TWS_Scan::Update(Cst4_Radar_Scan& IScan)
{
	Cst4_Radar_Scan Scan;
	Scan=IScan;
	if (nScanCnt==0)
		{
/*
		SetTransformType(TransformType);
		SetLLRef(Scan.GetRadarLL());
		SetRadarPos(Scan.GetRadarLL());
*/
		LatLong RadarLL=Scan.GetRadarLL();
		FlatTrans.SetTransformType(Transform::FLAT_TRANS);
		FlatTrans.SetProjection(Transform::SPHERICAL,RadarLL);
		FlatTrans.SetLLRef(RadarLL);
		FlatTrans.SetRadarPos(RadarLL);
		RoundTrans.SetTransformType(Transform::ROUND_TRANS);
		RoundTrans.SetProjection(Transform::SPHERICAL,RadarLL);
		RoundTrans.SetLLRef(RadarLL);
		RoundTrans.SetRadarPos(RadarLL);

		dStop_utc_sec=Scan.dTime_sec_utc()+60.0*60.0;
		}
	++nScanCnt;
	if (Setup.bCorr)
		Correlate(Scan);
	int nScanPlotNum=Scan.nGetPlotCnt();

	double dT_utc_sec=Scan.dTime_sec_utc();
	TWS.GoFirst();
	while(!TWS.bLast())
		TWS.pNext()->InitScan_utc(dT_utc_sec,nScanCnt);
	Cst4_TWS_Plot* pPlot=NULL;
	Cst4_TWS_Track* pT;
	String s;
	if (nScanPlotNum>0)
		{
		pPlot=new Cst4_TWS_Plot[nScanPlotNum];
		XYPos XY;
		double dRa_m,dBe_deg;
		for (int i=0; i<nScanPlotNum; i++)
			{
			if (Scan.pPlot(i)->bValid())
				{
				dRa_m=Scan.pPlot(i)->dRa_m();
				dBe_deg=Scan.pPlot(i)->dBe_deg();
				XY=FlatTrans.Pol2XY(Polar(dRa_m,dBe_deg));
	//			if ((XY.dx>60000.0)&&(XY.dy<108000.0)&&(XY.dy>-35000.0)&&(XY.dy<-23000.0)&&(dT_sec<dStop_sec))
				if (dRa_m>100.0)
					{
					if (pTestFile)
						pTestFile->TextLine(s.sprintf(L"Scan:%4d  Plot:%3d  X:%6.0f  Y:%6.0f",nScanCnt,i,XY.dx,XY.dy));
					pPlot[i].SetXY(XY);
					TWS.GoFirst();
					while(!TWS.bLast())
						{
						pT=TWS.pNext();
						pPlot[i].AddTrack(pT,pT->dGetMaxRadius());
						}
					pT=pPlot[i].pGetTrack();
					if (pT)
						{
						pPlot[i].SetUsed(pT->bAddPlot(XY));
						}
					}
				else
					{
					pPlot[i].SetValid(false);
					}
				}
			}
		}
	//Calculate

	if (nScanPlotNum==0)
		nScanPlotNum=0;

	TWS.GoFirst();
	while(!TWS.bLast())
		TWS.pNext()->CalcTrack(nCurrentID);
//	int nUnUsed=0;
	for (int nP=0; nP<nScanPlotNum; nP++)
		{
		//Initiate tracks on unused plots
		if (!pPlot[nP].bGetUsed() && pPlot[nP].bGetValid())
			{
			Cst4_TWS_Track* pNT;
			pNT=new Cst4_TWS_Track(&Setup,&FlatTrans,&RoundTrans,pPlot[nP].GetXY(),Scan.dTime_sec_utc(),Scan.wSource(),nScanCnt);
			TWS.nAdd(pNT);
//			++nUnUsed;
			}
		}
	delete[] pPlot;
/*
	if (pTestFile)
		pTestFile->TextLine(s.sprintf(L"    TWS Count %3d",TWS.nGetCount()));
*/
	//Delete aborted tracks
	TWS.GoFirst();
//	int nAborted=0;
	while(!TWS.bLast())
		{
		if (TWS.pNext()->bAborted())
			{
			TWS.GoPrev();
			TWS.Delete();
//			++nAborted;
			}
		}
/*
	if (pTestFile)
		{
		pTestFile->TextLine(s.sprintf(L"****  Scan %3d (Plots: %3d, UnUsed: %3d)",nScanCnt,nScanPlotNum,nUnUsed));
		pTestFile->TextLine(s.sprintf(L"    TWS Count %3d, Aborted: %3d",TWS.nGetCount(),nAborted));
		}
*/
	//Create current track list
	Tracks.Clear();
	TWS.GoFirst();
	while(!TWS.bLast())
		{
		pT=TWS.pNext();
		if (pT->bValidID() && (pT->bSolidTrack() || !Setup.bShowSolidOnly))
			Tracks.nAdd(new Cst4_Radar_Track(pT->GetTrackMsg()));
		}
	Tracks.Pack();
}

