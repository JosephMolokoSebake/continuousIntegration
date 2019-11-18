//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Weather_Client.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Weather_Client::Weather_Client(TComponent* pOwnerForm, const String sAddr, const int nPort, const int nMoveAveLength_sec)
{
	if (nMoveAveLength_sec>1)
		{
		pWindX=new MovingAverage(nMoveAveLength_sec);
		pWindY=new MovingAverage(nMoveAveLength_sec);
		}
	else
		{
		pWindX=NULL;
		pWindY=NULL;
		}
	NMEADecode::Reset();
	LastUpdate_utc.Invalidate();
	for (int i=0; i<MAX_SENT; i++)
		anRXCnt[i]=0;
	bTX_OK=bInitTX(pOwnerForm,sAddr,nPort);
	bInitRX();
	bRX_OK=bVerifyConnection(bStatusChanged,sStatus,StatusCol,true);
	nSimSize=0;
   nMsgCnt=0;
}

Weather_Client::~Weather_Client()
{
	delete pWindX;
	delete pWindY;
}


/* Modified LJVW 2014/06/12 */
bool Weather_Client::bUpdate()
{
	bool bRX_Ready=false;
	if (nSimSize>0)
		bRX_OK=true;
	else
		bRX_OK=bVerifyConnection(bStatusChanged,sStatus,StatusCol);
	if (bRX_OK)
		{
		int nRX;
		if (nSimSize>0)
			{
			memcpy(aucRXBuf,aucSimBuf,nSimSize);
			nRX=nSimSize;
         nSimSize=0;
			}
		else
			nRX=nUpdateRawRX(aucRXBuf,sizeof(aucRXBuf));
		if (nRX>0)
			{
			String s;
			for (int i=0; i<nRX; i++)
				{

				AddByte(aucRXBuf[i]);
				s=sGetHeader();
				if (!s.IsEmpty())
					{
					try
						{
						if ((s.Pos("GPZDA")>0)&&(nGetNum()>4))
							{
							LastUpdate_utc=GetZDA_utc();
							++anRXCnt[GPZDA];
							bRX_Ready=LastUpdate_utc.bValid();
							}
						else if ((s.Pos("GPGGA")>0)&&(nGetNum()>9))
							{
							Cst4Data.SetPos(LatLong(dGetLat(2,3),dGetLong(4,5)),dGetVal(9));
							++anRXCnt[GPGGA];
							}
						else if ((s.Pos("WIMWV")>0)&&(nGetNum()>3))
							{
							LastUpdate_utc=JTime(true,true);
							double dWindDir_deg=dGetVal(1);
							double dWindSpeed_kts=dGetVal(3);
							String sP=sGetPar(4);
							if (sP.Pos("M")==1)
								dWindSpeed_kts*=1.9435;	//convert from m/s
							else if (sP.Pos("K")==1)
								dWindSpeed_kts*=0.539956;	//convert from km/h
							else if (sP.Pos("N")==1)
								{
								//Do Nothing - already in knots
								}
//							sP=sGetPar(5);
//							if (sP.Pos("A")==1)
//								bValid=true;
							if ((pWindX)&&(pWindY))
								{
								double dX=dWindSpeed_kts*sin(dWindDir_deg*M_PI/180.0);
								double dY=dWindSpeed_kts*cos(dWindDir_deg*M_PI/180.0);
								dX=pWindX->dFilter(dX);
								dY=pWindX->dFilter(dY);
								dWindSpeed_kts=sqrt(dX*dX+dY*dY);
								if (fabs(dX)>0.001)
									dWindDir_deg=atan2(dY,dX)*180.0/M_PI;
								else if (dY>0.0)
									dWindDir_deg=0.0;
								else
									dWindDir_deg=180.0;
								}
							if (dWindDir_deg<0.0)
								dWindDir_deg+=360.0;
							Cst4Data.SetTrueWindDir(dWindDir_deg);
							Cst4Data.SetWindSpeed(dWindSpeed_kts);
							++anRXCnt[WIMWV];
							bRX_Ready=LastUpdate_utc.bValid();
							}
						else if ((s.Pos("WIMDA")>0)&&(nGetNum()>17))
							{
							if (!sGetPar(3).IsEmpty())
								Cst4Data.SetPressure(dGetVal(3));
							if (!sGetPar(5).IsEmpty())
								Cst4Data.SetAirTemp(dGetVal(5));
							if (!sGetPar(9).IsEmpty())
								Cst4Data.SetRelHumid(dGetVal(9));
							if (!sGetPar(11).IsEmpty())
								Cst4Data.SetDewPoint(dGetVal(11));
							if (!sGetPar(13).IsEmpty() && !sGetPar(17).IsEmpty())
								{
								double dWindDir_deg=dGetVal(13);
								double dWindSpeed_kts=dGetVal(17);
								if ((pWindX)&&(pWindY))
									{
									double dX=dWindSpeed_kts*sin(dWindDir_deg*M_PI/180.0);
									double dY=dWindSpeed_kts*cos(dWindDir_deg*M_PI/180.0);
									dX=pWindX->dFilter(dX);
									dY=pWindX->dFilter(dY);
									dWindSpeed_kts=sqrt(dX*dX+dY*dY);
									if (fabs(dX)>0.001)
										dWindDir_deg=atan2(dY,dX)*180.0/M_PI;
									else if (dY>0.0)
										dWindDir_deg=0.0;
									else
										dWindDir_deg=180.0;
									}
								if (dWindDir_deg<0.0)
									dWindDir_deg+=360.0;
								Cst4Data.SetTrueWindDir(dWindDir_deg);
								Cst4Data.SetWindSpeed(dWindSpeed_kts);
								}
							++anRXCnt[WIMDA];
                     ++nMsgCnt;
//							bRX=true;
							}
						}
					catch(...)
						{
						bRX_Ready=false;
						}
					}
				}
			}
		if (bRX_Ready)
			bRX_Ready=(anRXCnt[WIMWV]>0)||(anRXCnt[WIMDA]>0);
		}
	return bRX_Ready;
}
/* Modified LJVW 2014/06/12 */


void Weather_Client::Update_Simulated(const char* pucRXBufSim, const int nRXBufSim)
{
	memcpy(aucSimBuf,pucRXBufSim,nRXBufSim);
	nSimSize=nRXBufSim;
}

//bool Weather_Client::bUpdate_Simulated(char* aucRXBufSim,int nRXBufSim)
//{
//	bool bRX=false;
//	int nRX=nRXBufSim;
//	if (nRX>0)
//		{
//		String s;
//		for (int i=0; i<nRX; i++)
//			{
//
//			AddByte(aucRXBufSim[i]);
//			s=sGetHeader();
//			if (!s.IsEmpty())
//				{
//				try
//					{
//					if (s.Pos("GPZDA")>0)
//						{
//						LastUpdate_utc=GetZDA_utc();
//						++anRXCnt[GPZDA];
//						bRX=true;
//						}
//					else if (s.Pos("GPGGA")>0)
//						{
//						Cst4Data.SetPos(LatLong(dGetLat(2,3),dGetLong(4,5)),dGetVal(9));
//						++anRXCnt[GPGGA];
//						bRX=true;
//						}
//					else if (s.Pos("WIMDA")>0)
//						{
//						LastUpdate_utc=JTime(true,true);
//						if (!sGetPar(3).IsEmpty())
//							Cst4Data.SetPressure(dGetVal(3));
//						if (!sGetPar(5).IsEmpty())
//							Cst4Data.SetAirTemp(dGetVal(5));
//						if (!sGetPar(9).IsEmpty())
//							Cst4Data.SetRelHumid(dGetVal(9));
//						if (!sGetPar(11).IsEmpty())
//							Cst4Data.SetDewPoint(dGetVal(11));
//						if (!sGetPar(13).IsEmpty() && !sGetPar(17).IsEmpty())
//						{
//							double dWindDir_deg=dGetVal(13);
//							double dWindSpeed_kts=dGetVal(17);
//							if ((pWindX)&&(pWindY))
//								{
//								double dX=dWindSpeed_kts*sin(dWindDir_deg*M_PI/180.0);
//								double dY=dWindSpeed_kts*cos(dWindDir_deg*M_PI/180.0);
//								dX=pWindX->dFilter(dX);
//								dY=pWindX->dFilter(dY);
//								dWindSpeed_kts=sqrt(dX*dX+dY*dY);
//								if (fabs(dX)>0.001)
//									dWindDir_deg=atan2(dY,dX)*180.0/M_PI;
//								else if (dY>0.0)
//									dWindDir_deg=0.0;
//								else
//									dWindDir_deg=180.0;
//								}
//							if (dWindDir_deg<0.0)
//								dWindDir_deg+=360.0;
//							Cst4Data.SetTrueWindDir(dWindDir_deg);
//							Cst4Data.SetWindSpeed(dWindSpeed_kts);
//						}
//						++anRXCnt[WIMDA];
//						bRX=true;
//						}
//					}
//				catch(...)
//					{
//					bRX=false;
//					}
//				}
//			}
//		}
//	if (bRX)
//		bRX=(anRXCnt[GPZDA]>0)||(anRXCnt[GPGGA]>0)||(anRXCnt[WIMDA]>0);
//	return bRX;
//}



void Weather_Client::GetMsg(JTime& T_utc, Cst4_Weather_Data& Data)
{
	T_utc=LastUpdate_utc;
	Data=Cst4Data;
}
