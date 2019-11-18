//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "ConfigFPGA.h"
#include "JMemFile.h"
#include "JMessageBar.h"
#include "JUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


ConfigFPGA::ConfigFPGA(const int nDevice, const int nSerial)
{
	pPortB=NULL;
	bResetOK=false;
	pPortA=new FT2232H(nDevice,nSerial,'A');
	if (pPortA->bOK())
		{
		if (pPortA->bSetMode(FT2232H::RESET_MODE,0))
			bResetOK=true;
		}
	delete pPortA;
	pPortA=NULL;
	if (bResetOK)
		{
		pPortB=new FT2232H(nDevice,nSerial,'B');
		if (pPortB->bOK())
			{
			if (!pPortB->bSetMode(FT2232H::ASYNC_BITBANG_MODE,0x0F))
				pPortB->Close();
			}
		}
}

ConfigFPGA::~ConfigFPGA()
{
	delete pPortB;
	delete pPortA;
}

bool ConfigFPGA::bConfig(TForm* pForm, const String sRBF)
{
	JMemFile Fil;
	BYTE* pucRBFBuf;
	int nRBFSize=0;
	if (pPortB)
		{
		if (pPortB->bOK())
			{
			JFile::FILE_ERROR E=Fil.ReadBuffer(sRBF);
			if (E)
				{
				pPortB->Close();
				nShowError("Error Reading: "+sRBF,pPortB->sGetDevDescr()+" FPGA Configuration Error",30);
				}
			else
				{
				pucRBFBuf=Fil.pucGetBuffer();
				nRBFSize=(int)Fil.dwGetSize();
				}
			}
		if ((pPortB->bOK())&&(nRBFSize>0))
			{
			if (!bInitConfig())
				nShowError("Initialisation Failed",pPortB->sGetDevDescr()+" FPGA Configuration Error",30);
			else
				{
				bool bAbort=false;
				MessageBarForm=new TMessageBarForm(pForm);
				MessageBarForm->Show();
				MessageBarForm->SetCaption("Configuring "+pPortB->sGetDevDescr()+" FPGA",true);
				MessageBarForm->SetProgress(0);
				int nBlockSize=64;
				int nOffs=0;
				while((nOffs<nRBFSize)&&(!bAbort))
					{
					if (nOffs+nBlockSize>nRBFSize)
						nBlockSize=nRBFSize-nOffs;
					if (nBlockSize>0)
						TXBlock(&pucRBFBuf[nOffs],nBlockSize);
					nOffs+=nBlockSize;
					bAbort=MessageBarForm->bAbort;
					MessageBarForm->SetProgress(nOffs,nRBFSize);
					}
				delete MessageBarForm;
				if (bAbort)
					nShowError("Configuration Aborted by User",pPortB->sGetDevDescr()+" FPGA Configuration Error",30);
				else
					{
					if (bFinalise())
						return true;
					else
						nShowError("Configuration Confirmation Failed",pPortB->sGetDevDescr()+" FPGA Configuration Error",30);
					}
				}
			}
		}
	return false;
}

bool ConfigFPGA::bInitConfig()
{
	BYTE aucTX[24];
	//nCONFIG low for 10 us
	for (int i=0; i<12; i++)
		aucTX[i]=0x00;
	//nCONFIG high for 10 us
	for (int i=12; i<24; i++)
		aucTX[i]=nCONFIG_HIGH;
	if (pPortB->bWriteAllData(aucTX,24))
		{
		//Check that CONF_DONE is low and nSTATUS is high
		for (int i=0; i<20; i++)
			{
			BYTE ucStat=pPortB->ucGetBitStatus();
			if ((ucStat&nSTATUS_HIGH)&&((ucStat&CONF_DONE_HIGH)==0))
				return true;
			}
		}
//				return true;
	return false;
}

bool ConfigFPGA::bFinalise()
{
	BYTE aucData[2]={0x00,0x00};
	TXBlock(aucData,2);
	for (int i=0; i<20; i++)
		{
		BYTE ucStat=pPortB->ucGetBitStatus();
		if (ucStat&CONF_DONE_HIGH)
			return true;
		}
	return false;
}


void ConfigFPGA::TXBlock(const BYTE* pucBlock, const int nSize)
{
	BYTE* pucTX=new BYTE[nSize*16];
	int n=0;
	for (int i=0; i<nSize; i++)
		{
		BYTE uc=pucBlock[i];
		for (int j=0; j<8; j++)
			{
			if (uc&0x01)
				{
				pucTX[n++]=nCONFIG_HIGH | DATA_HIGH;
				pucTX[n++]=nCONFIG_HIGH | DATA_HIGH | CLK_HIGH;
				}
			else
				{
				pucTX[n++]=nCONFIG_HIGH;
				pucTX[n++]=nCONFIG_HIGH | CLK_HIGH;
				}
			uc>>=1;		//Shift right
			}
		}
	pPortB->bWriteAllData(pucTX,n);
	delete[] pucTX;
}


