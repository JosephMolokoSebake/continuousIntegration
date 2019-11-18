//---------------------------------------------------------------------------

#ifndef Cst4TrackerDefH
#define Cst4TrackerDefH
//---------------------------------------------------------------------------

#include "Cst4Defs.h"

#pragma option push -a4    //Set compiler to DWORD alignment


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

	double dAz_deg()	const	{return fAz_deg;							}
	double dEl_deg()	const {return fEl_deg;   						}
	double dRa_m()		const	{return nRa_m;								}
	LatLong PosLL() 	const {return Pos_LL.GetLL();					}
	double dPosHgt_m()const {return fPos_Hgt_m; 						}

};

#pragma option pop    //Set compiler back to default alignment


#endif
