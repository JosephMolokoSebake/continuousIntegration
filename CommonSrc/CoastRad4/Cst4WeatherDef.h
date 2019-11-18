//---------------------------------------------------------------------------

#ifndef Cst4WeatherDefH
#define Cst4WeatherDefH
//---------------------------------------------------------------------------

#include "Cst4MsgXDefs.h"

#pragma option push -a4    //Set compiler to DWORD alignment

class Cst4_Weather_Data : public Cst4_MsgUtils
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
		Pos_LL.Pack(pucBuf,nOffs);
		PackFloat(fAlt_m,pucBuf,nOffs);
		PackFloat(fPressure_bar,pucBuf,nOffs);
		PackFloat(fAirTemp_degC,pucBuf,nOffs);
		PackFloat(fRelHumid_perc,pucBuf,nOffs);
		PackFloat(fDewPoint_degC,pucBuf,nOffs);
		PackFloat(fTrueWindDir_deg,pucBuf,nOffs);
		PackFloat(fWindSpeed_kts,pucBuf,nOffs);
		PackBYTE(ucValidData,pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		Pos_LL.Unpack(pucBuf,nOffs);
		fAlt_m=fUnpack(pucBuf,nOffs);
		fPressure_bar=fUnpack(pucBuf,nOffs);
		fAirTemp_degC=fUnpack(pucBuf,nOffs);
		fRelHumid_perc=fUnpack(pucBuf,nOffs);
		fDewPoint_degC=fUnpack(pucBuf,nOffs);
		fTrueWindDir_deg=fUnpack(pucBuf,nOffs);
		fWindSpeed_kts=fUnpack(pucBuf,nOffs);
		ucValidData=ucUnpack(pucBuf,nOffs);
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


#pragma option pop    //Set compiler back to default alignment

#endif
