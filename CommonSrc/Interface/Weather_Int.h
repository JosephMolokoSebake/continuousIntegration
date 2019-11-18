//---------------------------------------------------------------------------

#ifndef Weather_IntH
#define Weather_IntH
//---------------------------------------------------------------------------

#include "USBSerial.h"
#include "NMEADecode.h"
#include "Cst4UserDefs.h"
#include "JMath.h"

class Weather_Int : public USBSerial, NMEADecode
{
	JTime LastUpdate_utc;
	Cst4_Weather_Data Cst4Data;
	BYTE aucRXBuf[8192];
	MovingAverage* pWindX;
	MovingAverage* pWindY;

	enum
	{
		GPZDA,
		GPGGA,
		WIMDA,
		MAX_SENT,
	};

	int anRXCnt[MAX_SENT];

public:

	Weather_Int(const int nUSB_Serial=-1, const int nMoveAveLength_sec=0);
	~Weather_Int();

	bool bUpdate(); /* Modified LJVW 2014/06/12 */
	bool bUpdate_Simulated(char* aucRXBufSim,int nRXBufSim); /* Added LJVW 2014/06/12 */

	void GetMsg(JTime& T_utc, Cst4_Weather_Data& Data);

	int nGetRXCnt(const int n=MAX_SENT)const
	{
		if (n<MAX_SENT)
			return anRXCnt[n];
		else
			return anRXCnt[WIMDA];
	}


};



#endif
