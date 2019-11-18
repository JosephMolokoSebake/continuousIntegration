//---------------------------------------------------------------------------

#ifndef ConfigFPGAH
#define ConfigFPGAH
//---------------------------------------------------------------------------

#include "FT2232H.h"

class ConfigFPGA
{

	enum
	{
		nCONFIG_HIGH	= 0x01,
		CLK_HIGH			= 0x02,
		DATA_HIGH		= 0x04,

		nSTATUS_HIGH	= 0x10,
		CONF_DONE_HIGH	= 0x20,
		INIT_DONE_HIGH	= 0x40,

	};

	FT2232H* pPortA;
	FT2232H* pPortB;

	bool bResetOK;

	bool bInitConfig();
	bool bFinalise();

	int nGetClkTX(const int nCnt, BYTE* pucTX);
	int nGetByteTX(const BYTE uc, BYTE* pucTX);

	void TXBlock(const BYTE* pucBlock, const int nSize);


public:

	ConfigFPGA(const int nDevice, const int nSerial=-1);

	~ConfigFPGA();

	bool bConfig(TForm* pForm, const String sRBF);

	bool bOK() const
	{
		return bResetOK;
	}


};





#endif
