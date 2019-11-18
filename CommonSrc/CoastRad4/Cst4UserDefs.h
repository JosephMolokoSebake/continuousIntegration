//---------------------------------------------------------------------------

#ifndef Cst4UserDefsH
#define Cst4UserDefsH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "JMemFile.h"


//***********************************************************************************

class Cst4_Tracker_Data
{

	Cst4LatLong Pos_LL;
	float fPos_Hgt_m;
	float fAz_deg;
	float fEl_deg;
	int nRa_m;

	enum
	{
		RA_VALID		= 0x01,
		AZ_VALID		= 0x02,
		EL_VALID		= 0x04,
		POS_VALID	= 0x08,
	};


	BYTE ucValidData;

public:

	void Init()
	{
		ucValidData=0;
	}

	void Assign(const Cst4_Tracker_Data& T)
	{
		Pos_LL=T.Pos_LL;
		fPos_Hgt_m=T.fPos_Hgt_m;
		fAz_deg=T.fAz_deg;
		fEl_deg=T.fEl_deg;
		nRa_m=T.nRa_m;
		ucValidData=T.ucValidData;
	}

	Cst4_Tracker_Data()
	{
		Init();
	}

	Cst4_Tracker_Data(const Cst4_Tracker_Data& T)
	{
		Assign(T);
	}

	Cst4_Tracker_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}


	~Cst4_Tracker_Data()
	{
	}

	Cst4_Tracker_Data& operator = (const Cst4_Tracker_Data& T)
	{
		Assign(T);
		return *this;
	}


	int nGetSize() const
	{
		return 3*sizeof(float)+sizeof(Pos_LL)+sizeof(int)+sizeof(BYTE);
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&pucBuf[nOffs],&Pos_LL,sizeof(Pos_LL));     nOffs+=sizeof(Pos_LL);
		memcpy(&pucBuf[nOffs],&fPos_Hgt_m,sizeof(float));	nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fAz_deg,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fEl_deg,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&nRa_m,sizeof(nRa_m));			nOffs+=sizeof(nRa_m);
		memcpy(&pucBuf[nOffs],&ucValidData,sizeof(ucValidData));
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&Pos_LL,		&pucBuf[nOffs],sizeof(Pos_LL));     nOffs+=sizeof(Pos_LL);
		memcpy(&fPos_Hgt_m,	&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fAz_deg,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fEl_deg,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&nRa_m,			&pucBuf[nOffs],sizeof(nRa_m));		nOffs+=sizeof(nRa_m);
		memcpy(&ucValidData,	&pucBuf[nOffs],sizeof(ucValidData));
	}

	void SetPos(const LatLong& LL, const double dHgt_m)
	{
		Pos_LL=Cst4LatLong(LL);
		fPos_Hgt_m=dHgt_m;
		ucValidData|=POS_VALID;
	}

	void SetAz(const double dAz_deg)
	{
		fAz_deg=dAz_deg;
		ucValidData|=AZ_VALID;
	}

	void SetEl(const double dEl_deg)
	{
		fEl_deg=dEl_deg;
		ucValidData|=EL_VALID;
	}

	void SetRa(const int _nRa_m)
	{
		nRa_m=_nRa_m;
		ucValidData|=RA_VALID;
	}


	void SetAngles(const double dAz_deg,
						const double dEl_deg)
	{
		SetAz(dAz_deg);
		SetEl(dEl_deg);
	}

	void SetPosLL(const LatLong& LL)
	{
		Pos_LL=Cst4LatLong(LL);
	}

	void GetAngles(double& dAz_deg, double& dEl_deg) const
	{
		dAz_deg=fAz_deg;
		dEl_deg=fEl_deg;
	}

	bool bAzValid()	const	{return (ucValidData&AZ_VALID)!=0;	}
	bool bElValid()	const	{return (ucValidData&EL_VALID)!=0;	}
	bool bRaValid()	const	{return (ucValidData&RA_VALID)!=0;	}
	bool bPosValid()	const	{return (ucValidData&POS_VALID)!=0;	}
	bool bAllValid()	const	{return (ucValidData&0x0F)==0x0F;	}

	double dAz_deg()	const	{return fAz_deg;							}
	double dEl_deg()	const {return fEl_deg;   						}
	double dRa_m()		const	{return nRa_m;								}
	LatLong PosLL() 	const {return Pos_LL.GetLL();					}
	double dPosHgt_m()const {return fPos_Hgt_m; 						}

	LatLong Trans2LL(double& dHeight_m);



};

//***********************************************************************************

class Cst4_Weather_Data
{
	//Time & Date from $GPZDA

	//From $GPGGA
	Cst4LatLong Pos_LL;
	float fAlt_m;

	//from $WIMDA
	float fPressure_bar;
	float fAirTemp_degC;
	float fRelHumid_perc;
	float fDewPoint_degC;
	float fTrueWindDir_deg;
	float fWindSpeed_kts;

	BYTE ucValidData;

	enum
	{
		POS_VALID			= 0x01,
		PRESSURE_VALID		= 0x02,
		TEMP_VALID			= 0x04,
		HUMID_VALID			= 0x08,
		DEW_VALID			= 0x10,
		WINDDIR_VALID		= 0x20,
		WINDSPEED_VALID	= 0x40,
	};

public:

	void Assign(const Cst4_Weather_Data& D)
	{
		Pos_LL=D.Pos_LL;
		fAlt_m=D.fAlt_m;
		fPressure_bar=D.fPressure_bar;
		fAirTemp_degC=D.fAirTemp_degC;
		fRelHumid_perc=D.fRelHumid_perc;
		fDewPoint_degC=D.fDewPoint_degC;
		fTrueWindDir_deg=D.fTrueWindDir_deg;
		fWindSpeed_kts=D.fWindSpeed_kts;
		ucValidData=D.ucValidData;
	}

	Cst4_Weather_Data()
	{
		ucValidData=0;
	}

	Cst4_Weather_Data(const Cst4_Weather_Data& D)
	{
		Assign(D);
	}

	Cst4_Weather_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_Weather_Data()
	{
	}

	void Invalidate()
	{
		ucValidData=0;
	}

	bool bValid() const
	{
		return (ucValidData!=0);
	}

	int nGetSize() const
	{
		return 6*sizeof(float)+sizeof(Pos_LL)+sizeof(int)+sizeof(BYTE);
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&pucBuf[nOffs],&Pos_LL,sizeof(Pos_LL));     		nOffs+=sizeof(Pos_LL);
		memcpy(&pucBuf[nOffs],&fAlt_m,sizeof(fAlt_m));				nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fPressure_bar,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fAirTemp_degC,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fRelHumid_perc,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fDewPoint_degC,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fTrueWindDir_deg,sizeof(float));	nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fWindSpeed_kts,sizeof(float));		nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&ucValidData,sizeof(ucValidData));
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&Pos_LL,				&pucBuf[nOffs],sizeof(Pos_LL));     nOffs+=sizeof(Pos_LL);
		memcpy(&fAlt_m,				&pucBuf[nOffs],sizeof(fAlt_m));		nOffs+=sizeof(float);
		memcpy(&fPressure_bar,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fAirTemp_degC,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fRelHumid_perc,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fDewPoint_degC,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fTrueWindDir_deg,	&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fWindSpeed_kts,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&ucValidData,			&pucBuf[nOffs],sizeof(ucValidData));
	}

	void SetPos(const LatLong& LL, const double dAlt_m)
	{
		Pos_LL=Cst4LatLong(LL);
		fAlt_m=dAlt_m;
		ucValidData|=POS_VALID;
	}

	bool bPosValid() 			const	{return (ucValidData&POS_VALID)!=0;					}
	LatLong GetPos_LL() 		const	{return LatLong(Pos_LL.dLat(),Pos_LL.dLong());	}
	double dGetAlt_m() 		const	{return fAlt_m;									 		}

	void SetPressure(const double dPressure_bar)
	{
		fPressure_bar=dPressure_bar;
		ucValidData|=PRESSURE_VALID;
	}
	bool bPressureValid() const	{return (ucValidData&PRESSURE_VALID)!=0; 			}
	double dGetPressure() const	{return fPressure_bar;									}

	void SetAirTemp(const double dAirTemp_degC)
	{
		fAirTemp_degC=dAirTemp_degC;
		ucValidData|=TEMP_VALID;
	}
	bool bAirTempValid() const	{return (ucValidData&TEMP_VALID)!=0; 					}
	double dGetAirTemp() const	{return fAirTemp_degC;										}


	void SetRelHumid(const double dRelHumid_perc)
	{
		fRelHumid_perc=dRelHumid_perc;
		ucValidData|=HUMID_VALID;
	}
	bool bRelHumidValid() const	{return (ucValidData&HUMID_VALID)!=0; 	  			}
	double dGetRelHumid() const	{return fRelHumid_perc;						  			}

	void SetDewPoint(const double dDewPoint_degC)
	{
		fDewPoint_degC=dDewPoint_degC;
		ucValidData|=DEW_VALID;
	}
	bool bDewPointValid() const	{return (ucValidData&DEW_VALID)!=0; 	  			}
	double dGetDewPoint() const	{return fDewPoint_degC;						  			}

	void SetTrueWindDir(const double dTrueWindDir_deg)
	{
		fTrueWindDir_deg=dTrueWindDir_deg;
		ucValidData|=WINDDIR_VALID;
	}
	bool bTrueWindDirValid() const	{return (ucValidData&WINDDIR_VALID)!=0;		}
	double dGetTrueWindDir() const	{return fTrueWindDir_deg;							}

	void SetWindSpeed(const double dWindSpeed_kts)
	{
		fWindSpeed_kts=dWindSpeed_kts;
		ucValidData|=WINDSPEED_VALID;
	}
	bool bWindSpeedValid() const	{return (ucValidData&WINDSPEED_VALID)!=0; 		}
	double dGetWindSpeed() const	{return fWindSpeed_kts;									}


};

//***********************************************************************************

class Cst4_Wind_Data
{
	//Time & Date from PC

	//from $WIMWV
	float fSpeed_kts;
	float fDir_deg;
	bool bTrueNorth;
	bool bMagneticNorth;
	bool bValidData;

public:

	void Assign(const Cst4_Wind_Data& D)
	{
		fSpeed_kts=D.fSpeed_kts;
		fDir_deg=D.fDir_deg;
		bTrueNorth=D.bTrueNorth;
		bMagneticNorth=D.bMagneticNorth;
		bValidData=D.bValidData;
	}

	Cst4_Wind_Data()
	{
		fSpeed_kts=-1000.0;
		fDir_deg=-1000.0;
		bTrueNorth=false;
		bMagneticNorth=false;
		bValidData=false;

	}

	Cst4_Wind_Data(const Cst4_Wind_Data& D)
	{
		Assign(D);
	}

	Cst4_Wind_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_Wind_Data()
	{
	}

	void Invalidate()
	{
		bValidData=false;
	}

	bool bValid() const
	{
		return bValidData;
	}

	int nGetSize() const
	{
		return 2*sizeof(float)+3*sizeof(bool);
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&pucBuf[nOffs],&fSpeed_kts,sizeof(fSpeed_kts));   		nOffs+=sizeof(fSpeed_kts);
		memcpy(&pucBuf[nOffs],&fDir_deg,sizeof(fDir_deg));					nOffs+=sizeof(fDir_deg);
		memcpy(&pucBuf[nOffs],&bTrueNorth,sizeof(bTrueNorth));	  		nOffs+=sizeof(bTrueNorth);
		memcpy(&pucBuf[nOffs],&bMagneticNorth,sizeof(bMagneticNorth));	nOffs+=sizeof(bMagneticNorth);
		memcpy(&pucBuf[nOffs],&bValidData,sizeof(bValidData));			nOffs+=sizeof(bValidData);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&fSpeed_kts,&pucBuf[nOffs],sizeof(fSpeed_kts));   		nOffs+=sizeof(fSpeed_kts);
		memcpy(&fDir_deg,&pucBuf[nOffs],sizeof(fDir_deg));					nOffs+=sizeof(fDir_deg);
		memcpy(&bTrueNorth,&pucBuf[nOffs],sizeof(bTrueNorth));		 	nOffs+=sizeof(bTrueNorth);
		memcpy(&bMagneticNorth,&pucBuf[nOffs],sizeof(bMagneticNorth));	nOffs+=sizeof(bMagneticNorth);
		memcpy(&bValidData,&pucBuf[nOffs],sizeof(bValidData));			nOffs+=sizeof(bValidData);
	}

	void SetWind(const double dDir_deg, const double dSpeed_kts)
	{
		fDir_deg=dDir_deg;
		fSpeed_kts=dSpeed_kts;
		bValidData=true;
	}

	double dGetDir() const	{return fDir_deg;							}
	double dGetSpeed() const	{return fSpeed_kts;									}




};

//***********************************************************************************

class Cst4_AIS_Status_Data
{
	enum
	{
		AIS_STATUS_VALID	= 0x00000001,
		AIS_STATUS_TX_ON	= 0x00000002,
	};

	DWORD dwStatus;

public:

	void Assign(const Cst4_AIS_Status_Data& D)
	{
		dwStatus=D.dwStatus;
	}

	Cst4_AIS_Status_Data()
	{
		dwStatus=0;
	}

	Cst4_AIS_Status_Data(const Cst4_AIS_Status_Data& D)
	{
		Assign(D);
	}

	Cst4_AIS_Status_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_AIS_Status_Data()
	{
	}

	void Invalidate()
	{
		dwStatus=0;
	}

	bool bValid() const
	{
		return (dwStatus&AIS_STATUS_VALID)!=0;
	}

	int nGetSize() const
	{
		return sizeof(dwStatus);
	}

	void Pack(BYTE* pucBuf)
	{
		memcpy(pucBuf,&dwStatus,sizeof(dwStatus));
	}

	void UnPack(const BYTE* pucBuf)
	{
		memcpy(&dwStatus,pucBuf,sizeof(dwStatus));
	}

	void SetTX(const bool bTX_On)
	{
		dwStatus|=AIS_STATUS_VALID;
		if (bTX_On)
			dwStatus|=AIS_STATUS_TX_ON;
		else
			dwStatus&=(~AIS_STATUS_TX_ON);
	}

	bool bTXOn() const
	{
		if (bValid())
			return (dwStatus&AIS_STATUS_TX_ON)!=0;
		else
			return false;
	}

};

//***********************************************************************************

class Cst4_Source_Status_Data
{
	enum
	{
		AIS_STATUS_VALID	= 0x00000001,
		AIS_STATUS_TX_ON	= 0x00000002,
	};

	DWORD dwStatus;

public:

	void Assign(const Cst4_Source_Status_Data& D)
	{
		dwStatus=D.dwStatus;
	}

	Cst4_Source_Status_Data()
	{
		dwStatus=0;
	}

	Cst4_Source_Status_Data(const Cst4_Source_Status_Data& D)
	{
		Assign(D);
	}

	Cst4_Source_Status_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_Source_Status_Data()
	{
	}

	void Invalidate()
	{
		dwStatus=0;
	}

	bool bValid() const
	{
		return (dwStatus&AIS_STATUS_VALID)!=0;
	}

	int nGetSize() const
	{
		return sizeof(dwStatus);
	}

	void Pack(BYTE* pucBuf)
	{
		memcpy(pucBuf,&dwStatus,sizeof(dwStatus));
	}

	void UnPack(const BYTE* pucBuf)
	{
		memcpy(&dwStatus,pucBuf,sizeof(dwStatus));
	}

	void SetTX(const bool bTX_On)
	{
		dwStatus|=AIS_STATUS_VALID;
		if (bTX_On)
			dwStatus|=AIS_STATUS_TX_ON;
		else
			dwStatus&=(~AIS_STATUS_TX_ON);
	}

	bool bTXOn() const
	{
		if (bValid())
			return (dwStatus&AIS_STATUS_TX_ON)!=0;
		else
			return false;
	}

};

//***********************************************************************************

class Cst4_AltAIS_Data
{

	int nMMSI;
	Cst4LatLong LL;
	WORD wSpeed;
	WORD wCourse;
	WORD wCRC;
	WORD wBlockSize;
	BYTE* pucBlock;

	Cst4_AIS_Static Stat;

public:

	void Assign(const Cst4_AltAIS_Data& D)
	{

		nMMSI=D.nMMSI;
		LL=D.LL;
		wSpeed=D.wSpeed;
		wCourse=D.wCourse;
		wCRC=D.wCRC;
		wBlockSize=D.wBlockSize;
		if (pucBlock)
			delete[] pucBlock;
		if (wBlockSize>0)
			{
			pucBlock=new BYTE[wBlockSize];
			memcpy(pucBlock,D.pucBlock,wBlockSize);
			}
		else
			pucBlock=NULL;
		Stat=D.Stat;
	}

	Cst4_AltAIS_Data()
	{
		LL.Invalidate();
		wSpeed=0xFFFF;
		wCourse=0xFFFF;
		pucBlock=NULL;
		Stat.Invalidate();
	}

	Cst4_AltAIS_Data& operator = (const Cst4_AltAIS_Data& D)
	{
		Assign(D);
		return *this;
	}

	Cst4_AltAIS_Data(const Cst4_AltAIS_Data& D)
	{
		Assign(D);
	}

	Cst4_AltAIS_Data(const void* pPackedData)
	{
		pucBlock=NULL;
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_AltAIS_Data()
	{
		delete[] pucBlock;
	}

	void Invalidate()
	{
		LL.Invalidate();
		wSpeed=0xFFFF;
		wCourse=0xFFFF;
		delete[] pucBlock;
		pucBlock=NULL;
		Stat.Invalidate();
	}

	bool bValid() const
	{
		return (pucBlock!=NULL);
	}

	int nGetSize() const
	{
		return sizeof(nMMSI)+sizeof(LL)+2*sizeof(wSpeed)+sizeof(wCRC)+sizeof(wBlockSize)+wBlockSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&pucBuf[nOffs],&nMMSI,sizeof(nMMSI));     			nOffs+=sizeof(nMMSI);
		memcpy(&pucBuf[nOffs],&LL,sizeof(LL));     					nOffs+=sizeof(LL);
		memcpy(&pucBuf[nOffs],&wSpeed,sizeof(wSpeed));				nOffs+=sizeof(wSpeed);
		memcpy(&pucBuf[nOffs],&wCourse,sizeof(wCourse));			nOffs+=sizeof(wCourse);
		memcpy(&pucBuf[nOffs],&wCRC,sizeof(wCRC));     				nOffs+=sizeof(wCRC);
		memcpy(&pucBuf[nOffs],&wBlockSize,sizeof(wBlockSize));	nOffs+=sizeof(wBlockSize);
		if (wBlockSize>0)
			memcpy(&pucBuf[nOffs],pucBlock,wBlockSize);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		memcpy(&nMMSI,&pucBuf[nOffs],sizeof(nMMSI));     			nOffs+=sizeof(nMMSI);
		memcpy(&LL,&pucBuf[nOffs],sizeof(LL));     					nOffs+=sizeof(LL);
		memcpy(&wSpeed,&pucBuf[nOffs],sizeof(wSpeed));				nOffs+=sizeof(wSpeed);
		memcpy(&wCourse,&pucBuf[nOffs],sizeof(wCourse));			nOffs+=sizeof(wCourse);
		memcpy(&wCRC,&pucBuf[nOffs],sizeof(wCRC));     				nOffs+=sizeof(wCRC);
		memcpy(&wBlockSize,&pucBuf[nOffs],sizeof(wBlockSize));	nOffs+=sizeof(wBlockSize);
		delete[] pucBlock;
		if (wBlockSize>0)
			{
			pucBlock=new BYTE[wBlockSize];
			memcpy(pucBlock,&pucBuf[nOffs],wBlockSize);
			}
	}

	void SetSpeed(const double dSpeed_kts)
	{
		wSpeed=dSpeed_kts*10.0+0.5;
	}

	double dSpeed_kts() const
	{
		return wSpeed/10.0;
	}

	bool bValidSpeed() const
	{
		return (wSpeed!=0xFFFF);
	}

	bool bValidCourse() const
	{
		return (wCourse!=0xFFFF);
	}

	void SetCourse(const double dCourse_deg)
	{
		wCourse=dCourse_deg*65536.0/360.0;
	}

	double dCourse_deg() const
	{
		return wCourse*360.0/65536.0;
	}

	void SetMMSI(const int _nMMSI)
	{
		nMMSI=_nMMSI;
	}

	int nGetMMSI() const
	{
		return nMMSI;
	}

	LatLong GetLL() const
	{
		return LL.GetLL();
	}

	Cst4LatLong GetLL4() const
	{
		return LL;
	}

	void SetLL(const LatLong& _LL)
	{
		LL.SetLL(_LL);
	}

	void AddSentence(const AnsiString as)
	{


	}

	void SetRawNMEA(const BYTE* pucRaw, const WORD wSize)
	{
		wBlockSize=wSize;
		delete[] pucBlock;
		pucBlock=new BYTE[wBlockSize];
		memcpy(pucBlock,pucRaw,wBlockSize);
	}

	int nGetRawNMEA(BYTE* pucRaw)
	{
		if (pucBlock)
			memcpy(pucRaw,pucBlock,wBlockSize);
		else
			pucRaw=NULL;
		return wBlockSize;
	}

	bool bDecodeRaw(const int nCst4ID);

	bool bDecodeRaw(const BYTE* pucRaw, const WORD wSize, const int nCst4ID)
	{
		SetRawNMEA(pucRaw,wSize);
		return bDecodeRaw(nCst4ID);
	}

	void SetStatic(const Cst4_AIS_Static& _Stat)
	{
		Stat=_Stat;
	}

	void SetDynamicData(const Cst4_AIS_Dynamic& D);

	Cst4_AIS_Dynamic GetDynamicData(const JTime T, const int nCst4ID);

	Cst4_AIS_Static GetStaticData()
	{
		return Stat;
	}

	void CalcCRC();

	bool bSameCRC(const WORD _wCRC)
	{
		  return (_wCRC==wCRC);
	}

	WORD wGetCRC() const
	{
		return wCRC;
	}

	String sExtract();


};

//***********************************************************************************


class Cst4_Block_Data : public Cst4_MessageUtils
{

public:

private:

	int nBlockNum;
	int nTotalBlocks;
	BYTE* pucBlock;
	int nBlockSize;
	String sFileName;
	DWORD dwJPEG_ID;
	DWORD dwTXCRC;
	int nSysTrackNum;

public:

	Cst4_Block_Data()
	{
		pucBlock=NULL;
		nBlockSize=0;
		nBlockNum=0;
		nTotalBlocks=0;
		sFileName="";
		dwJPEG_ID=0;
		dwTXCRC=0;
		nSysTrackNum=-1;
	}

	Cst4_Block_Data(const Cst4_Block_Data& D)
	{
		pucBlock=NULL;
		Assign(D);
	}

	~Cst4_Block_Data()
	{
		delete[] pucBlock;
	}

	void Assign(const Cst4_Block_Data& D)
	{
		nBlockNum=D.nBlockNum;
		nTotalBlocks=D.nTotalBlocks;
		nBlockSize=D.nBlockSize;
		delete[] pucBlock;
		pucBlock=NULL;
		if (nBlockSize>0)
			{
			pucBlock=new BYTE[nBlockSize];
			memcpy(pucBlock,D.pucBlock,nBlockSize);
			}
		sFileName=D.sFileName;
		dwJPEG_ID=D.dwJPEG_ID;
		dwTXCRC=D.dwTXCRC;
		nSysTrackNum=D.nSysTrackNum;
	}

	Cst4_Block_Data& operator = (const Cst4_Block_Data& D)
	{
		Assign(D);
		return *this;
	}

	Cst4_Block_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	void Invalidate()
	{
		nBlockSize=0;
	}

	bool bValid() const
	{
		return (nBlockSize>0);
	}

	int nGetMaxBlockSize() const
	{
		return 65250;//65535-24-260;
	}

	int nGetSize() const
	{
	return sizeof(nBlockNum)
			+sizeof(nTotalBlocks)
			+sizeof(nBlockSize)
			+sizeof(dwJPEG_ID)
			+sizeof(dwTXCRC)
			+sizeof(nSysTrackNum)
			+nBlockSize
			+dwPackedStringSize(sFileName);
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nBlockNum,pucBuf,nOffs);
		PackInt(nTotalBlocks,pucBuf,nOffs);
		PackDWORD(dwJPEG_ID,pucBuf,nOffs);
		PackDWORD(dwTXCRC,pucBuf,nOffs);
		PackInt(nSysTrackNum,pucBuf,nOffs);
		PackString(sFileName,pucBuf,nOffs);
		PackInt(nBlockSize,pucBuf,nOffs);
		if (nBlockSize>0)
			{
			memcpy(&pucBuf[nOffs],pucBlock,nBlockSize);
			nOffs+=nBlockSize;
			}
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nBlockNum=nUnpack(pucBuf,nOffs);
		nTotalBlocks=nUnpack(pucBuf,nOffs);
		dwJPEG_ID=dwUnpack(pucBuf,nOffs);
		dwTXCRC=dwUnpack(pucBuf,nOffs);
		nSysTrackNum=nUnpack(pucBuf,nOffs);
		sFileName=sUnpack(pucBuf,nOffs);
		nBlockSize=nUnpack(pucBuf,nOffs);
		delete pucBlock;
		pucBlock=NULL;
		if (nBlockSize>0)
			{
			pucBlock=new BYTE[nBlockSize];
			memcpy(pucBlock,&pucBuf[nOffs],nBlockSize);
			nOffs+=nBlockSize;
			}
	}

	void SetBlock(const DWORD dwID, const int nNum, const int nTotal, const BYTE* pucData, const int nSize)
	{
		dwJPEG_ID=dwID;
		nBlockNum=nNum;
		nTotalBlocks=nTotal;
		delete[] pucBlock;
		nBlockSize=nSize;
		pucBlock=new BYTE[nBlockSize];
		memcpy(pucBlock,pucData,nBlockSize);
	}

	int nGetBlockNum()		const{return nBlockNum;}
	int nGetTotalBlocks() 	const{return nTotalBlocks;}
	int nGetBlockSize() 		const{return nBlockSize;}
	DWORD dwGetJPEG_ID()		const{return dwJPEG_ID;}
	DWORD dwGetTXCRC()		const{return dwTXCRC;}

	bool bLastBlock() const
	{
		return (nBlockNum==nTotalBlocks-1);
	}

	void SetSysTrackNum(const int n)
	{
		nSysTrackNum=n;
	}

	int nGetSysTrackNum() const
	{
		return nSysTrackNum;
	}

	void SetTXCRC(const DWORD _dwTXCRC)
	{
		dwTXCRC=_dwTXCRC;
	}

	void SetFileName(const String sFile)
	{
		sFileName=sFile;
	}

	String sGetFileName() const
	{
		return sFileName;
	}

	BYTE* pucBlockData()
	{
		return pucBlock;
	}

	int nGetBlockData(BYTE* pucData) const
	{
		if (nBlockSize>0)
			memcpy(pucData,pucBlock,nBlockSize);
		return nBlockSize;
	}


};


//***********************************************************************************

class Cst4_Cyclope_Data : public Cst4_MessageUtils
{

	int nTrackID;
	Cst4LatLong LL;
	int nAlt_m;

public:

	void Init()
	{
	}

	void Assign(const Cst4_Cyclope_Data& C)
	{
		nTrackID=C.nTrackID;
		LL=C.LL;
		nAlt_m=C.nAlt_m;
	}

	Cst4_Cyclope_Data()
	{
		Init();
	}

	Cst4_Cyclope_Data(const Cst4_Cyclope_Data& C)
	{
		Assign(C);
	}

	Cst4_Cyclope_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}


	~Cst4_Cyclope_Data()
	{
	}

	Cst4_Cyclope_Data& operator = (const Cst4_Cyclope_Data& C)
	{
		Assign(C);
		return *this;
	}


	int nGetSize() const
	{
		return sizeof(nTrackID)+sizeof(nAlt_m)+sizeof(LL);
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nTrackID,pucBuf,nOffs);
		PackLL(LL,pucBuf,nOffs);
		PackInt(nAlt_m,pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nTrackID=nUnpack(pucBuf,nOffs);
		LL=UnpackCst4LL(pucBuf,nOffs);
		nAlt_m=nUnpack(pucBuf,nOffs);
	}

	void SetID(const int nID)
	{
		nTrackID=nID;
	}

	void SetPos(const LatLong& _LL, const int _nAlt_m)
	{
		LL=Cst4LatLong(_LL);
		nAlt_m=_nAlt_m;
	}

	LatLong GetLL() const {return LL.GetLL();	}
	int nGetAlt_m() const {return nAlt_m; 	  	}

};

//***********************************************************************************



#endif
