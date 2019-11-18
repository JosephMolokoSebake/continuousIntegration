//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "GuardDogDlg.h"
#include "JUtils.h"
#include "JIniFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TGuardDogForm *GuardDogForm;
//---------------------------------------------------------------------------
__fastcall TGuardDogForm::TGuardDogForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGuardDogForm::FormCreate(TObject *Sender)
{
	pGuard=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::FormShow(TObject *Sender)
{
	JIniFile Ini(false,"");
	int nSec;
	Ini.Read("PC WATCHDOG","Timeout (sec)",nSec,30);
	WatchTime0Ed->SetVal(nSec);
	Ini.Read("Delay (sec)",anDelay_sec[0],30);
	Ini.Read("LAPTOP 1 WATCHDOG","Timeout (sec)",nSec,30);
	WatchTime1Ed->SetVal(nSec);
	Ini.Read("Delay (sec)",anDelay_sec[1],30);
	Ini.Read("LAPTOP 2 WATCHDOG","Timeout (sec)",nSec,30);
	WatchTime2Ed->SetVal(nSec);
	Ini.Read("Delay (sec)",anDelay_sec[2],30);

	DisWatch0But->Enabled=false;
	DisWatch1But->Enabled=false;
	DisWatch2But->Enabled=false;
	Trig0But->Enabled=false;
	Trig1But->Enabled=false;
	Trig2But->Enabled=false;
	StopGPSBut->Enabled=false;
	GSMPan->Color=clLtGray;
	TimePan->Color=clLtGray;
	DatePan->Color=clLtGray;
	LeapPan->Color=clLtGray;
	GPSDateEd->Color=clLtGray;
	GPSTimeEd->Color=clLtGray;
	GPSLatEd->Color=clLtGray;
	GPSLongEd->Color=clLtGray;
	GPSHeightEd->Color=clLtGray;
	abWatchEn[0]=false;
	abWatchEn[1]=false;
	abWatchEn[2]=false;
	if (pGuard)
		{
      SerialEd->SetVal(pGuard->nGetFWVersion());
		}
	EnWatch0But->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	JIniFile Ini(true,"");
	Ini.Write("PC WATCHDOG","Timeout (sec)",WatchTime0Ed->nGetVal());
	Ini.Write("Delay (sec)",anDelay_sec[0]);
	Ini.Write("LAPTOP 1 WATCHDOG","Timeout (sec)",WatchTime1Ed->nGetVal());
	Ini.Write("Delay (sec)",anDelay_sec[1]);
	Ini.Write("LAPTOP 2 WATCHDOG","Timeout (sec)",WatchTime2Ed->nGetVal());
	Ini.Write("Delay (sec)",anDelay_sec[2]);
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::GPSButClick(TObject *Sender)
{
//	if (!pGuard)
//		EnableHardware();
	if (pGuard)
		{
		GPSBut->Enabled=false;
		StopGPSBut->Enabled=true;


		pGPSFil=NULL;

/*
		pGPSFil=new JFile('I');
		pGPSFil->Create("GPS Data.bin");
*/
//		GPSMemo->Clear();
		sGPSLine="";

		pGuard->EnableGPS(true,9600);

		::SleepEx(100,false);	//Ensure that all the setups have finished

		pGuard->SetGPSNMEARate("ZDA",0);
		pGuard->SetGPSNMEARate("GGA",0);
		pGuard->SetGPSNMEARate("GSV",0);
		pGuard->SetGPSNMEARate("GSV",0);
		pGuard->SetGPSNMEARate("VTG",0);
		pGuard->SetGPSNMEARate("RMC",0);

		pGuard->SetGPSUBXMsg(0x01,0x21,1);	//Enable nNAV-TIMEUTC
		pGuard->SetGPSUBXMsg(0x01,0x02,1);	//Enable NAV-POSLLH

		::SleepEx(1000,false);	//Ensure that all the setups have finished

		GPSTimer->Enabled=true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::GPSTimerTimer(TObject *Sender)
{
	JTime T;
	LatLong LL;
	double dHgt_m;
	bool bTimeValid,bDateValid,bLeapValid;
	if (pGuard)
		{
		DWORD dwMsgCnt=pGuard->dwReadGPSPar(T,LL,dHgt_m,bTimeValid,bDateValid,bLeapValid);
		if (bTimeValid)
			{
			TimePan->Color=clLime;
			TimePan->Caption="Time Valid";
			}
		else
			{
			TimePan->Color=clRed;
			TimePan->Caption="Time NOT Valid";
			}
		if (bDateValid)
			{
			DatePan->Color=clLime;
			DatePan->Caption="Date Valid";
			}
		else
			{
			DatePan->Color=clRed;
			DatePan->Caption="Date NOT Valid";
			}
		if (bLeapValid)
			{
			LeapPan->Color=clLime;
			LeapPan->Caption="Leap Sec Valid";
			}
		else
			{
			LeapPan->Color=clRed;
			LeapPan->Caption="Leap Sec NOT Valid";
			}

		GPSLatEd->Color=clLtGray;
		GPSLongEd->Color=clLtGray;
		GPSHeightEd->Color=clLtGray;
		String s;
		if (T.bValid())
			{
			GPSDateEd->Text=s.sprintf(L"%02d/%02d/%04d",T.Get().wDay,T.Get().wMonth,T.Get().wYear);
			GPSTimeEd->Text=s.sprintf(L"%02d:%02d:%02d.%03d",T.Get().wHour,T.Get().wMinute,T.Get().wSecond,T.Get().wMilliseconds);
			GPSDateEd->Color=clLime;
			GPSTimeEd->Color=clLime;
			}
		else
			{
			GPSDateEd->Text="";
			GPSTimeEd->Text="";
			GPSDateEd->Color=clRed;
			GPSTimeEd->Color=clRed;
			}
		if (LL.bValid())
			{
			GPSLatEd->Text=LL.sLat();
			GPSLongEd->Text=LL.sLong();
			GPSHeightEd->Text=s.sprintf(L"%3.3f m",dHgt_m);
			GPSLatEd->Color=clLime;
			GPSLongEd->Color=clLime;
			GPSHeightEd->Color=clLime;
			}
		else
			{
			GPSLatEd->Text="";
			GPSLongEd->Text="";
			GPSHeightEd->Text="";
			GPSLatEd->Color=clRed;
			GPSLongEd->Color=clRed;
			GPSHeightEd->Color=clRed;
			}
		GPSMsgCntEd->SetVal(dwMsgCnt);
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::StopGPSButClick(TObject *Sender)
{
	if (pGuard)
		{
		GPSTimer->Enabled=false;
		pGuard->EnableGPS(false);
		delete pGPSFil;
		pGPSFil=NULL;
		GPSBut->Enabled=true;
		StopGPSBut->Enabled=false;
		GSMPan->Color=clLtGray;
		TimePan->Color=clLtGray;
		DatePan->Color=clLtGray;
		LeapPan->Color=clLtGray;
		GPSDateEd->Color=clLtGray;
		GPSTimeEd->Color=clLtGray;
		GPSLatEd->Color=clLtGray;
		GPSLongEd->Color=clLtGray;
		GPSHeightEd->Color=clLtGray;
		}

}
//---------------------------------------------------------------------------



void __fastcall TGuardDogForm::EnWatch0ButClick(TObject *Sender)
{
//	if (!pGuard)
//		EnableHardware();
	if (pGuard)
		{
		pGuard->StartWatchdog(0,WatchTime0Ed->nGetVal(),anDelay_sec[0]);
		WatchLeft0Ed->Text="";
		EnWatch0But->Enabled=false;
		DisWatch0But->Enabled=true;
		Trig0But->Enabled=true;
		WatchTime0Ed->Enabled=false;
      abWatchEn[0]=true;
		CheckWatchTimer();
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::DisWatch0ButClick(TObject *Sender)
{
	abWatchEn[0]=false;
	CheckWatchTimer();
	pGuard->StopWatchdog(0);
	DisWatch0But->Enabled=false;
	Trig0But->Enabled=false;
	WatchTime0Ed->Enabled=true;
	EnWatch0But->Enabled=true;
	if (!WatchTimer->Enabled)
		{
		GSMPan->Color=clLtGray;
		GSMPan->Color=clLtGray;
		TimePan->Color=clLtGray;
		DatePan->Color=clLtGray;
		LeapPan->Color=clLtGray;
		GPSDateEd->Color=clLtGray;
		GPSTimeEd->Color=clLtGray;
		GPSLatEd->Color=clLtGray;
		GPSLongEd->Color=clLtGray;
		GPSHeightEd->Color=clLtGray;
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::Trig0ButClick(TObject *Sender)
{
	pGuard->TriggerWatchdog(0);
}
//---------------------------------------------------------------------------


void __fastcall TGuardDogForm::WatchTimerTimer(TObject *Sender)
{
	pGuard->bUpdateStatus();
	for (int nWatchID=0; nWatchID<3; nWatchID++)
		{
		if (abWatchEn[nWatchID])
			{
			bool bResetTime;
			int nCnt=pGuard->nGetWatchCount(nWatchID,bResetTime);
			switch(nWatchID)
				{
				case 0:
					WatchLeft0Ed->SetVal(nCnt);
					if (bResetTime)
						{
						Status0Ed->Text="Power OFF";
						Trig0But->Enabled=false;
						}
					else
						Status0Ed->Text="Power ON";
					if (pGuard->bGetGSMOverride())
						{
						Status0Ed->Text="Power OFF";
						GSMPan->Color=clRed;
						}
					else
						GSMPan->Color=clLime;
					break;
				case 1:
					WatchLeft1Ed->SetVal(nCnt);
					if (bResetTime)
						{
						Status1Ed->Text="Power OFF";
						Trig1But->Enabled=false;
						}
					else
						Status1Ed->Text="Power ON";
					if (pGuard->bGetGSMOverride())
						{
						Status1Ed->Text="Power OFF";
						GSMPan->Color=clRed;
						}
					else
						GSMPan->Color=clLime;
					break;
				case 2:
					WatchLeft2Ed->SetVal(nCnt);
					if (bResetTime)
						{
						Status2Ed->Text="Power OFF";
						Trig2But->Enabled=false;
						}
					else
						Status2Ed->Text="Power ON";
					if (pGuard->bGetGSMOverride())
						{
						Status2Ed->Text="Power OFF";
						GSMPan->Color=clRed;
						}
					else
						GSMPan->Color=clLime;
					break;
				default:	;
				}
			}
		}
}
//---------------------------------------------------------------------------



void __fastcall TGuardDogForm::USBRadClick(TObject *Sender)
{
	delete pGuard;
	pGuard=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::RS232RadClick(TObject *Sender)
{
	delete pGuard;
	pGuard=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::RS422RadClick(TObject *Sender)
{
	delete pGuard;
	pGuard=NULL;
}
//---------------------------------------------------------------------------

/*
void TGuardDogForm::EnableHardware()
{
	delete pGuard;
	pGuard=NULL;
	int nID;
	if (IDChk->Checked)
		nID=IDEd->nGetVal();
	else
		nID=-1;
	if (USBRad->Checked)
		{
		pGuard=new GuardDog_Int(GD_USB_INT,nID);
		if (!pGuard->bOK())
			{
			delete pGuard;
			pGuard=NULL;
			}
		if (pGuard)
			{
			if (!pGuard->bLoopbackOK())
				{
				ShowError("Watchdog USB Error","USB Loopback Failed");
				delete pGuard;
				pGuard=NULL;
				}
			}
		}
	else
		{
		if (RS232Rad->Checked)
			pGuard=new GuardDog_Int(GD_RS232_INT,nID);
		else
			pGuard=new GuardDog_Int(GD_RS422_INT,nID);
		if (!pGuard->bOK())
			{
			delete pGuard;
			pGuard=NULL;
			}
		if (pGuard)
			{
			if (!pGuard->bLoopbackOK())
				{
				ShowError("Watchdog Serial Interface Error","Loopback Failed");
				delete pGuard;
				pGuard=NULL;
				}
			}
		}
	if (pGuard)
		SerialEd->SetVal(pGuard->nGetSerial());
	StopGPSBut->Enabled=false;
}
*/

void __fastcall TGuardDogForm::EnWatch1ButClick(TObject *Sender)
{
//	if (!pGuard)
//		EnableHardware();
	if (pGuard)
		{
		pGuard->StartWatchdog(1,WatchTime1Ed->nGetVal(),anDelay_sec[1]);
		WatchLeft1Ed->Text="";
		EnWatch1But->Enabled=false;
		DisWatch1But->Enabled=true;
		Trig1But->Enabled=true;
		WatchTime1Ed->Enabled=false;
		abWatchEn[1]=true;
		CheckWatchTimer();
		}

}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::EnWatch2ButClick(TObject *Sender)
{
//	if (!pGuard)
//		EnableHardware();
	if (pGuard)
		{
		pGuard->StartWatchdog(2,WatchTime2Ed->nGetVal(),anDelay_sec[1]);
		WatchLeft2Ed->Text="";
		EnWatch2But->Enabled=false;
		DisWatch2But->Enabled=true;
		Trig2But->Enabled=true;
		WatchTime2Ed->Enabled=false;
		abWatchEn[2]=true;
		CheckWatchTimer();
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::DisWatch1ButClick(TObject *Sender)
{
	abWatchEn[1]=false;
	CheckWatchTimer();
	pGuard->StopWatchdog(1);
	DisWatch1But->Enabled=false;
	Trig1But->Enabled=false;
	WatchTime1Ed->Enabled=true;
	EnWatch1But->Enabled=true;
	if (!WatchTimer->Enabled)
		{
		GSMPan->Color=clLtGray;
		GSMPan->Color=clLtGray;
		TimePan->Color=clLtGray;
		DatePan->Color=clLtGray;
		LeapPan->Color=clLtGray;
		GPSDateEd->Color=clLtGray;
		GPSTimeEd->Color=clLtGray;
		GPSLatEd->Color=clLtGray;
		GPSLongEd->Color=clLtGray;
		GPSHeightEd->Color=clLtGray;
		}

}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::DisWatch2ButClick(TObject *Sender)
{
	abWatchEn[2]=false;
	CheckWatchTimer();
	pGuard->StopWatchdog(2);
	DisWatch2But->Enabled=false;
	Trig2But->Enabled=false;
	WatchTime2Ed->Enabled=true;
	EnWatch2But->Enabled=true;
	if (!WatchTimer->Enabled)
		{
		GSMPan->Color=clLtGray;
		GSMPan->Color=clLtGray;
		TimePan->Color=clLtGray;
		DatePan->Color=clLtGray;
		LeapPan->Color=clLtGray;
		GPSDateEd->Color=clLtGray;
		GPSTimeEd->Color=clLtGray;
		GPSLatEd->Color=clLtGray;
		GPSLongEd->Color=clLtGray;
		GPSHeightEd->Color=clLtGray;
		}
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::Trig1ButClick(TObject *Sender)
{
	pGuard->TriggerWatchdog(1);
}
//---------------------------------------------------------------------------

void __fastcall TGuardDogForm::Trig2ButClick(TObject *Sender)
{
	pGuard->TriggerWatchdog(2);
}
//---------------------------------------------------------------------------

