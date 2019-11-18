//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Wind_Int.h"
#include "JFileDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Wind_Int::Wind_Int(const int nUSB_Serial, const int nMoveAveLength_sec)
{
	nUSBID=nUSB_Serial;
	pUSB=NULL;
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
	NMEADecode::Reset();
	LastUpdate_utc.Invalidate();
	nRXByteCnt=0;
	nRXMsgCnt=0;
	pInFil=NULL;
	pRawFil=NULL;
}

Wind_Int::~Wind_Int()
{
	if (pUSB)
		pUSB->StopUART();
	delete pWindX;
	delete pWindY;
	delete pInFil;
	delete pRawFil;
	delete pUSB;
}


bool Wind_Int::bReadFromFile()
{
	delete pInFil;
	pInFil=new JMemFile;
	String sF=sSelectOpenFile("NMEA Text Files (*.txt)","*.txt","Select NMEA File");
	if (!sF.IsEmpty())
		{
		JFile::FILE_ERROR E=pInFil->ReadBuffer(sF);
		if (!E &&(pInFil->dwGetSize()>0))
			return true;
		}
	delete pInFil;
	pInFil=NULL;
	return FALSE;
}

void Wind_Int::SaveToFile()
{
	delete pRawFil;
	pRawFil=new JFile('O');
	String sF="WindServer Raw "+JTime(true,true).sFileName("txt",true);
	pRawFil->Create(sF);
}

void Wind_Int::SetupUART(const int nBaud, const char cParity, const int nDataBytes, const int nStopBytes, const bool b422)
{
	if (!pUSB)
		{
		pUSB=new USBSerial(USB_DETECT_DEV,nUSBID);
		if (!pUSB->bOK())
			{

			delete pUSB;
			pUSB=NULL;
			}
		}
	if (pUSB)
		{
		pUSB->SetupUART(nBaud,cParity,nDataBytes,nStopBytes,b422);
		pUSB->StartUART(b422);
		}
}

bool Wind_Int::bUpdate()
{
	bool bRX=false;
	int nRX;
	if (pUSB)
		{
		nRX=pUSB->nRXUARTData(aucRXBuf,sizeof(aucRXBuf));
		if ((nRX>0)&&(pRawFil))
			pRawFil->Write(aucRXBuf,nRX);
		}
	else if (pInFil)
		{
		nRX=pInFil->dwGetSize()-pInFil->dwGetPos();
		if (nRX>30)
			nRX=30;
		if (nRX==0)
			pInFil->bSeek(0);
		else
			pInFil->bRead(aucRXBuf,nRX);
		}
	if (nRX>0)
		{
		nRXByteCnt+=nRX;
		String s;
		for (int i=0; i<nRX; i++)
			{

			AddByte(aucRXBuf[i]);
			s=sGetHeader();
			if (!s.IsEmpty())
				{
				bool bValid=false;
				double dWindDir_deg;
				double dWindSpeed_kts;
				try
					{
					if (s.Pos("WIMWV")>0)
						{
						LastUpdate_utc=JTime(true,true);
						dWindDir_deg=dGetVal(1);
						dWindSpeed_kts=dGetVal(3);
						String sP=sGetPar(4);
						if (sP.Pos("M")==1)
							dWindSpeed_kts*=1.9435;	//convert from m/s
						else if (sP.Pos("K")==1)
							dWindSpeed_kts*=0.539956;	//convert from km/h
						else if (sP.Pos("N")==1)
							{
							//Do Nothing - already in knots
							}
						sP=sGetPar(5);
						if (sP.Pos("A")==1)
							bValid=true;
						}
					}
				catch(...)
					{
					bValid=false;
					}
				if (bValid)
					{
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
					Cst4Data.SetWind(dWindDir_deg,dWindSpeed_kts);
					++nRXMsgCnt;
					bRX=true;
					}
				}
			}
		}
	return bRX;
}

void Wind_Int::GetMsg(JTime& T_utc, Cst4_Wind_Data& Data)
{
	T_utc=LastUpdate_utc;
	Data=Cst4Data;
}
