//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Weather_USB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Weather_USB::Weather_USB(const int nUSB_Serial, const int nMoveAveLength_sec) : USBSerial(USB_DETECT_DEV,nUSB_Serial)
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
	if (bOK())
		{
		NMEADecode::Reset();
		LastUpdate_utc.Invalidate();
		for (int i=0; i<MAX_SENT; i++)
			anRXCnt[i]=0;
		SetupUART(4800,'N',8,1,true);
		StartUART(true);
		}
}

Weather_USB::~Weather_USB()
{
	StopUART();
	delete pWindX;
	delete pWindY;
}

/*
bool Weather_USB::bUpdate()
{
	bool bRX=false;
	int nRX=nRXUARTData(aucRXBuf,sizeof(aucRXBuf));
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
					if (s.Pos("GPZDA")>0)
						{
						LastUpdate_utc=GetZDA_utc();
						++anRXCnt[GPZDA];
						bRX=true;
						}
					else if (s.Pos("GPGGA")>0)
						{
						Cst4Data.SetPos(LatLong(dGetLat(2,3),dGetLong(4,5)),dGetVal(9));
						++anRXCnt[GPGGA];
						bRX=true;
						}
					else if (s.Pos("WIMDA")>0)
						{
						Cst4Data.SetPressure(dGetVal(3));
						Cst4Data.SetAirTemp(dGetVal(5));
						Cst4Data.SetRelHumid(dGetVal(9));
						Cst4Data.SetDewPoint(dGetVal(11));
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
						++anRXCnt[WIMDA];
						bRX=true;
						}
					}
				catch(...)
					{
					bRX=false;
					}
				}
			}
		}
	if (bRX)
		bRX=(anRXCnt[GPZDA]>0)&&(anRXCnt[GPGGA]>0)&&(anRXCnt[WIMDA]>0);
	return bRX;
}*/

/* Modified LJVW 2014/06/12 */
bool Weather_USB::bUpdate()
{
	bool bRX=false;
	int nRX=nRXUARTData(aucRXBuf,sizeof(aucRXBuf));
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
					if (s.Pos("GPZDA")>0)
						{
						LastUpdate_utc=GetZDA_utc();
						++anRXCnt[GPZDA];
						bRX=true;
						}
					else if (s.Pos("GPGGA")>0)
						{
						Cst4Data.SetPos(LatLong(dGetLat(2,3),dGetLong(4,5)),dGetVal(9));
						++anRXCnt[GPGGA];
						bRX=true;
						}
					else if (s.Pos("WIMDA")>0)
						{
						LastUpdate_utc=JTime(true,true);
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
						bRX=true;
						}
					}
				catch(...)
					{
					bRX=false;
					}
				}
			}
		}
	if (bRX)
		bRX=(anRXCnt[GPZDA]>0)||(anRXCnt[GPGGA]>0)||(anRXCnt[WIMDA]>0);
	return bRX;
}
/* Modified LJVW 2014/06/12 */


bool Weather_USB::bUpdate_Simulated(char* aucRXBufSim,int nRXBufSim)
{
	bool bRX=false;
	int nRX=nRXBufSim;
	if (nRX>0)
		{
		String s;
		for (int i=0; i<nRX; i++)
			{

			AddByte(aucRXBufSim[i]);
			s=sGetHeader();
			if (!s.IsEmpty())
				{
				try
					{
					if (s.Pos("GPZDA")>0)
						{
						LastUpdate_utc=GetZDA_utc();
						++anRXCnt[GPZDA];
						bRX=true;
						}
					else if (s.Pos("GPGGA")>0)
						{
						Cst4Data.SetPos(LatLong(dGetLat(2,3),dGetLong(4,5)),dGetVal(9));
						++anRXCnt[GPGGA];
						bRX=true;
						}
					else if (s.Pos("WIMDA")>0)
						{
						LastUpdate_utc=JTime(true,true);
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
						bRX=true;
						}
					}
				catch(...)
					{
					bRX=false;
					}
				}
			}
		}
	if (bRX)
		bRX=(anRXCnt[GPZDA]>0)||(anRXCnt[GPGGA]>0)||(anRXCnt[WIMDA]>0);
	return bRX;
}



void Weather_USB::GetMsg(JTime& T_utc, Cst4_Weather_Data& Data)
{
	T_utc=LastUpdate_utc;
	Data=Cst4Data;
}
