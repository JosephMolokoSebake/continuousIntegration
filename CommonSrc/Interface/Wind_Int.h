//---------------------------------------------------------------------------

#ifndef Wind_IntH
#define Wind_IntH
//---------------------------------------------------------------------------

#include "USBSerial.h"
#include "NMEADecode.h"
#include "Cst4UserDefs.h"
#include "JMath.h"
#include "JMemFile.h"

class Wind_Int : public NMEADecode
{
	JTime LastUpdate_utc;
	Cst4_Wind_Data Cst4Data;
	BYTE aucRXBuf[8192];
	MovingAverage* pWindX;
	MovingAverage* pWindY;

	USBSerial* pUSB;
	int nUSBID;

	JMemFile* pInFil;
	JFile* pRawFil;

	int nRXMsgCnt;
	int nRXByteCnt;

public:

	Wind_Int(const int nUSB_Serial=-1, const int nMoveAveLength_sec=0);
	~Wind_Int();

	bool bReadFromFile();

	void SaveToFile();

	void SetupUART(const int nBaud, const char cParity, const int nDataBytes, const int nStopBytes, const bool b422);


	bool bUpdate();

	void GetMsg(JTime& T_utc, Cst4_Wind_Data& Data);

	int nGetRXByteCnt()const
	{
		return nRXByteCnt;
	}

	int nGetRXMsgCnt()const
	{
		return nRXMsgCnt;
	}


};



#endif
