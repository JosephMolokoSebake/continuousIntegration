//---------------------------------------------------------------------------

#ifndef Cst4INSDefH
#define Cst4INSDefH
//---------------------------------------------------------------------------

#include "Cst4Defs.h"

#pragma option push -a4    //Set compiler to DWORD alignment

class Cst4_INS_Data
{

	enum
	{
		INS_VALID	= 0x00000001,
		LL_VALID		= 0x00000002,
		SPEED_VALID	= 0x00000004,
	};


	DWORD dwValidData;

	float fHeading_deg;
	float fHeadingRate_deg_s;

   float fRoll_deg;
   float fRollRate_deg_s;

   float fPitch_deg;
   float fPitchRate_deg_s;

   float fHeave_m;
   float fHeaveRate_m_s;

   Cst4LatLong Pos_LL;
   float fSpeed_kts;


public:

	void Init()
	{
		dwValidData=0x00000000;
	}

	Cst4_INS_Data()
	{
		Init();
	}

	Cst4_INS_Data(const Cst4_INS_Data& D)
	{
		Assign(D);
	}

	Cst4_INS_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	~Cst4_INS_Data()
	{
	}

	void Assign(const Cst4_INS_Data& D)
	{
		dwValidData=D.dwValidData;
		fHeading_deg=D.fHeading_deg;
		fHeadingRate_deg_s=D.fHeadingRate_deg_s;
		fRoll_deg=D.fRoll_deg;
		fRollRate_deg_s=D.fRollRate_deg_s;
		fPitch_deg=D.fPitch_deg;
		fPitchRate_deg_s=D.fPitchRate_deg_s;
		fHeave_m=D.fHeave_m;
		fHeaveRate_m_s=D.fHeaveRate_m_s;
		Pos_LL=D.Pos_LL;
		fSpeed_kts=D.fSpeed_kts;
	}

	Cst4_INS_Data& operator = (const Cst4_INS_Data& D)
	{
		Assign(D);
		return *this;
	}

	int nGetSize() const
   {
		return sizeof(float)*9+sizeof(dwValidData)+sizeof(Pos_LL);
   }

   void Pack(BYTE* pucBuf)
   {
      int nOffs=0;
      memcpy(&pucBuf[nOffs],&dwValidData,sizeof(dwValidData));		nOffs+=sizeof(dwValidData);
      memcpy(&pucBuf[nOffs],&fHeading_deg,sizeof(float));			nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&fHeadingRate_deg_s,sizeof(float));	nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fRoll_deg,sizeof(float));				nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fRollRate_deg_s,sizeof(float));		nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fPitch_deg,sizeof(float));				nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fPitchRate_deg_s,sizeof(float));		nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fHeave_m,sizeof(float));					nOffs+=sizeof(float);
      memcpy(&pucBuf[nOffs],&fHeaveRate_m_s,sizeof(float));       nOffs+=sizeof(float);
		memcpy(&pucBuf[nOffs],&Pos_LL,sizeof(Pos_LL));              nOffs+=sizeof(Pos_LL);
		memcpy(&pucBuf[nOffs],&fSpeed_kts,sizeof(float));
   }

   void UnPack(const BYTE* pucBuf)
   {
		int nOffs=0;
		memcpy(&dwValidData,			&pucBuf[nOffs],sizeof(dwValidData));nOffs+=sizeof(dwValidData);
		memcpy(&fHeading_deg,		&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fHeadingRate_deg_s,&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fRoll_deg,			&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fRollRate_deg_s,	&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fPitch_deg,			&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fPitchRate_deg_s,	&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fHeave_m,				&pucBuf[nOffs],sizeof(float));		nOffs+=sizeof(float);
		memcpy(&fHeaveRate_m_s,		&pucBuf[nOffs],sizeof(float));   	nOffs+=sizeof(float);
		memcpy(&Pos_LL,				&pucBuf[nOffs],sizeof(Pos_LL));  	nOffs+=sizeof(Pos_LL);
		memcpy(&fSpeed_kts,			&pucBuf[nOffs],sizeof(float));
	}

   void SetData(const double dHeading_deg,
					 const double dRoll_deg,
                const double dPitch_deg,
                const double dHeave_m,
                const double dHeadingRate_deg_s,
                const double dRollRate_deg_s,
                const double dPitchRate_deg_s,
                const double dHeaveRate_m_s)
   {
		dwValidData|=INS_VALID;
		fHeading_deg=dHeading_deg;
		fHeadingRate_deg_s=dHeadingRate_deg_s;
		fRoll_deg=dRoll_deg;
		fRollRate_deg_s=dRollRate_deg_s;
		fPitch_deg=dPitch_deg;
		fPitchRate_deg_s=dPitchRate_deg_s;
		fHeave_m=dHeave_m;
		fHeaveRate_m_s=dHeaveRate_m_s;
	}

	void SetLL(const LatLong& LL)
	{
		dwValidData|=LL_VALID;
		Pos_LL=Cst4LatLong(LL);
	}

	void SetSpeed(const double dSpeed_kts)
	{
		dwValidData|=SPEED_VALID;
		fSpeed_kts=dSpeed_kts;
	}

	void GetData(double& dHeading_deg,
					 double& dRoll_deg,
					 double& dPitch_deg,
					 double& dHeave_m,
					 double& dHeadingRate_deg_s,
					 double& dRollRate_deg_s,
					 double& dPitchRate_deg_s,
					 double& dHeaveRate_m_s) const
	{
		dHeading_deg=fHeading_deg;
		dHeadingRate_deg_s=fHeadingRate_deg_s;
		dRoll_deg=fRoll_deg;
		dRollRate_deg_s=fRollRate_deg_s;
		dPitch_deg=fPitch_deg;
		dPitchRate_deg_s=fPitchRate_deg_s;
		dHeave_m=fHeave_m;
		dHeaveRate_m_s=fHeaveRate_m_s;
	}

	void GetData(double& dHeading_deg,
					 double& dRoll_deg,
					 double& dPitch_deg,
					 double& dHeave_m) const
	{
		dHeading_deg=fHeading_deg;
		dRoll_deg=fRoll_deg;
		dPitch_deg=fPitch_deg;
		dHeave_m=fHeave_m;
	}

	double dHeading_deg() 			const	{return fHeading_deg;			}
	double dHeadingRate_deg_s() 	const	{return fHeadingRate_deg_s;	}
	double dRoll_deg() 				const {return fRoll_deg;   			}
	double dRollRate_deg_s() 		const {return fRollRate_deg_s;   	}
   double dPitch_deg() 				const {return fPitch_deg;   			}
   double dPitchRate_deg_s() 		const {return fPitchRate_deg_s;   	}
   double dHeave_m() 				const {return fHeave_m;					}
   double dHeaveRate_m_s() 		const {return fHeaveRate_m_s;			}
   LatLong LL() 						const {return Pos_LL.GetLL();  		}
   double dSpeed_kts() 				const {return fSpeed_kts;   			}

	bool bINSValid() 					const	{return (dwValidData&INS_VALID)!=0;		}
	bool bLLValid() 					const	{return (dwValidData&LL_VALID)!=0;		}
	bool bSpeedValid() 				const	{return (dwValidData&SPEED_VALID)!=0;	}

};



#pragma option pop    //Set compiler back to default alignment

#endif
