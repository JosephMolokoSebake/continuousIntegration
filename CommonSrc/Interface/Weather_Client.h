//---------------------------------------------------------------------------

#ifndef Weather_ClientH
#define Weather_ClientH
//---------------------------------------------------------------------------

#include "VistaClient.h"
#include "NMEADecode.h"
#include "Cst4UserDefs.h"
#include "JMath.h"

class Weather_Client : public VistaClient, NMEADecode
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
      WIMWV,
		MAX_SENT,
	};

	int anRXCnt[MAX_SENT];

	bool bTX_OK;
	bool bRX_OK;

	bool bStatusChanged;
	String sStatus;
	TColor StatusCol;

	int nSimSize;
	BYTE aucSimBuf[16384];

   int nMsgCnt;

public:

	Weather_Client(TComponent* pOwnerForm, const String sAddr, const int nPort, const int nMoveAveLength_sec=0);
	~Weather_Client();

	bool bUpdate(); /* Modified LJVW 2014/06/12 */
//	bool bUpdate_Simulated(char* aucRXBufSim,int nRXBufSim); /* Added LJVW 2014/06/12 */
	void Update_Simulated(const char* pucRXBufSim, const int nRXBufSim); /* Added JGT 2018/04/24  */

	void GetMsg(JTime& T_utc, Cst4_Weather_Data& Data);

	int nGetRXCnt(const int n=MAX_SENT)const
	{
		if (n<MAX_SENT)
			return anRXCnt[n];
		else
			return anRXCnt[WIMDA];
	}

	bool bGetStatusChanged() const
	{
		return bStatusChanged;
	}

	String sGetStatus() const
	{
		return sStatus;
	}

	TColor GetStatusCol() const
	{
		return StatusCol;
	}

	int nGetMsgCount() const
	{
		return nMsgCnt;
	}

};



#endif
