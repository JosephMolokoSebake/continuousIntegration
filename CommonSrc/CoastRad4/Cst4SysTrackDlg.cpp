//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4SysTrackDlg.h"
#include "Cst4SiteDefs.h"
#include "Cst4UserDefs.h"
#include "JUtils.h"
#include "LJTcpClientSocket.h"
#include "AISTargetInfoDlg.h"
#include "RadarTrackInfoDlg.h"
#include "Cst4PictureDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma //link "NumEdit"
#pragma resource "*.dfm"
TSysTrackForm *SysTrackForm;
//---------------------------------------------------------------------------
__fastcall TSysTrackForm::TSysTrackForm(TComponent* Owner)
	: TForm(Owner)
{
	bNewMsg=true;
	AltRadarList.Clear();
	AltAISList.Clear();
	sHubAddr="";
	bReadOnly=false;
	bStarting=true;
	bPictureReceived=false;
	pFixedServer=NULL;
	pFixedServerLock=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TSysTrackForm::FormShow(TObject * /*Sender*/)
{
   nTimerCnt=0;
   nPictUploadCnt=-1;
   CancelPictBut->Visible=false;
   bPictCancelled=false;
   dwUpdate_utc_sec=0;
	ClassCB->Clear();
	for (int i=0; i<CST4_SHIP_CLASS_COUNT; i++)
		ClassCB->Items->Add(SysMsg.sGetShipClassification((CST4_SHIP_CLASSIFICATION)i));
	ClassCB->ItemIndex=-1;
//	if (SysMsg.bValid())
	UpdateMsg();
	DeleteBut->Enabled=!bNewMsg;
   UploadPictBut->Enabled=!bNewMsg;
	if (bNewMsg)
		{
		SubmitBut->Caption="Submit";
		SysMsg.SetUpdate(JTime(true,true),dwCurrentID,ucCurrentSeat);
		SysMsg.CalcExpire();
		Caption="System Track [Track Number Not Assigned]";
		}
	else
		{
		SubmitBut->Caption="Update";
		Caption="System Track [Track Number = "+String(SysMsg.nGetTrackNumber())+"]";
		}
	bool bSubEnable=!sHubAddr.IsEmpty() ||(pFixedServer);
	DeleteBut->Enabled=bSubEnable && !bNewMsg;
	SubmitBut->Enabled=bSubEnable;
//	AISInfoBut->Enabled=AISTgt.bValid() && AISStat.bValid();
	AltAISBut->Enabled=AltAISList.nGetCount()>0;
//	RadarInfoBut->Enabled=Radar_T.bValid();
	AltRadarBut->Enabled=AltRadarList.nGetCount()>0;
	Timer->Enabled=!bNewMsg;
	TimeLeftEd->Text="";
	Ed.SetValRO(LifeEd,SysMsg.nGetMaxLife_h());
   Ed.SetLastLimits("Max Life",1,9999);
	SysMsg.SetUpdate(JTime(true,true),dwCurrentID,ucCurrentSeat);
	CloseBut->SetFocus();
	SubmitBut->Visible=!bReadOnly;
	DeleteBut->Visible=!bReadOnly;
	DescrEd->ReadOnly=bReadOnly;
	CommentMemo->ReadOnly=bReadOnly;
	LifeEd->Enabled=!bReadOnly;
	ClassCB->Enabled=!bReadOnly;
   bStarting=false;
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)

{
//
}
//---------------------------------------------------------------------------

void TSysTrackForm::UpdateRadar()
{
	Cst4_SysTrack::RadarStruct Radar;
	Radar=SysMsg.GetRadar();
	RadarSrcEd->Text=sGetCst4Site(Radar.dwID);
	Ed.SetValRO(RadarTrackEd,Radar.nTrack);
	Ed.SetValRO(RadarQualityEd,Radar.ucQuality);
	LatLong LL=Radar.LL.GetLL();
	Ed.SetLLRO(RadarLatEd,RadarLongEd,LL);
	Ed.SetValRO(RadarSpeedEd,Radar.dSpeed_kts(),1);
	Ed.SetValRO(RadarCourseEd,Radar.dCourse_deg(),1);
	JTime T_loc;
	T_loc.SetFullSec(Radar.dwGetLastUpdate_utc_sec(),true);
	T_loc.ConvertToLocal(g_nCst4TimeBias_hours);
	Ed.SetTimeRO(LastRadarTime,T_loc);
	Ed.SetDateRO(LastRadarDate,T_loc);
}

String TSysTrackForm::sGetShipAndCargoType(const String sPrefixDelimiter,const String sPostfixDelimiter)
{
	int n=SysMsg.GetAIS().nType;
	String sShipCargo;
	switch (n/10)
		{
		case 0: sShipCargo="Not Available";				break; //default
		case 1: sShipCargo="";						break; // reserved for future use
		case 2: sShipCargo="WIG"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
		case 3: sShipCargo="Vessel"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
		case 4: sShipCargo="HSC"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
		case 5: sShipCargo=sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);				break;
		case 6: sShipCargo="Passenger Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);		break;
		case 7: sShipCargo="Cargo Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
		case 8: sShipCargo="Tanker"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
		case 9: sShipCargo="Other Type Of Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);		break;
		default: sShipCargo="";
		}
	return sShipCargo;
}

String TSysTrackForm::sGetCargoSecond(const int n,const String sPrefixDelimiter,const String sPostfixDelimiter)
{
	String sSecondDigit;
	switch(n/10) // first digit
	{
		case 2:
		case 4:
		case 6:
		case 7:
		case 8:
		case 9:
			switch (n%10) // Second digit
			{
				case 0: sSecondDigit="";						break;
				case 1: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.A)"+sPostfixDelimiter;	break;
				case 2: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.B)"+sPostfixDelimiter;	break;
				case 3: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.C)"+sPostfixDelimiter;	break;
				case 4: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.D)"+sPostfixDelimiter;	break;
				case 5: //reserved for future use
				case 6: //reserved for future use
				case 7: //reserved for future use
				case 8: //reserved for future use
				case 9: //reserved for future use
				default: sSecondDigit="";
			}
			break;
		case 3:
			switch (n%10) // Second digit
			{
				case 0: sSecondDigit=sPrefixDelimiter+"Fishing"+sPostfixDelimiter;						break;
				case 1: sSecondDigit=sPrefixDelimiter+"Towing"+sPostfixDelimiter;						break;
				case 2: sSecondDigit=sPrefixDelimiter+"Towing (Tow Length > 200m / Tow Breadth > 25m)"+sPostfixDelimiter;	break;
				case 3: sSecondDigit=sPrefixDelimiter+"Engaged In Dredging / Underwater Operations"+sPostfixDelimiter;	break;
				case 4: sSecondDigit=sPrefixDelimiter+"Engaged In Diving Operations"+sPostfixDelimiter;			break;
				case 5: sSecondDigit=sPrefixDelimiter+"Engaged In Military Operations"+sPostfixDelimiter;			break;
				case 6: sSecondDigit=sPrefixDelimiter+"Sailing"+sPostfixDelimiter;						break;
				case 7: sSecondDigit=sPrefixDelimiter+"Pleasure Craft"+sPostfixDelimiter;					break;
				case 8: //reserved for future use
				case 9: //reserved for future use
				default: sSecondDigit="";
			}
			break;
		case 5:
			switch (n%10) // Second digit
			{
				case 0: sSecondDigit="Pilot Vessel";					break;
				case 1: sSecondDigit="Search And Rescue Vessel";				break;
				case 2: sSecondDigit="Tug";							break;
				case 3: sSecondDigit="Port Tender";						break;
				case 4: sSecondDigit="Vessel With Anti-Pollution Facilities Or Equipment";	break;
				case 5: sSecondDigit="Law Enforcement Vessels";				break;
				case 6: sSecondDigit="";							break; //for assignments to local vessels
				case 7: sSecondDigit="";							break; //for assignments to local vessels
				case 8: sSecondDigit="Medical Transport";					break;
				case 9: sSecondDigit="Ship According To Resolution No 18 (Mob-83)";		break;
				default: sSecondDigit="";
			}
			break;
		default: sSecondDigit="";
	}
	return sSecondDigit;
}




void TSysTrackForm::UpdateAIS()
{
	Cst4_SysTrack::AISStruct AIS;
	AIS=SysMsg.GetAIS();
	if (AIS.nMMSI>0)
		Ed.SetValRO(MMSIEd,AIS.nMMSI);
	if (AIS.nIMO>0)
		Ed.SetValRO(IMOEd,AIS.nIMO);
	LatLong LL=AIS.LL.GetLL();
	Ed.SetLLRO(LatEd,LongEd,LL);
	Ed.SetValRO(SpeedEd,AIS.dSpeed_kts(),1);
	Ed.SetValRO(CourseEd,AIS.dCourse_deg(),1);
	NameEd->Text=AIS.sName;
	CallsignEd->Text=AIS.sCallsign;
	String s;
	switch (AIS.ucNavStatus)
		{
		case AIS_NAVSTATUS_UNDER_WAY_USING_ENGINE:		s="Underway Using Engine";			break;
		case AIS_NAVSTATUS_AT_ANCHOR:							s="At Anchor";							break;
		case AIS_NAVSTATUS_NOT_UNDER_COMMAND:				s="Not Under Command";			  	break;
		case AIS_NAVSTATUS_RESTRICTED_MANOEUVRABILITY:	s="Restricted Manoeuvrability"; 	break;
		case AIS_NAVSTATUS_CONSTRAINED_BY_HER_DRAUGHT:	s="Contrained By Her Draught";  	break;
		case AIS_NAVSTATUS_MOORED:								s="Moored";								break;
		case AIS_NAVSTATUS_AGROUND:							s="Aground";						  	break;
		case AIS_NAVSTATUS_ENGAGED_IN_FISHING:				s="Engaged In Fishing";				break;
		case AIS_NAVSTATUS_UNDER_WAY_SAILING:				s="Underway Sailing";			  	break;
		case AIS_NAVSTATUS_AIS_SART:							s="AIS Search and Rescue TX";		break;
		default: 													s="Not Defined";
		}
	CargoEd->Text=sGetShipAndCargoType();
	NavStatusEd->Text=s;
	Ed.SetValRO(LengthEd,AIS.wLength);
	Ed.SetValRO(WidthEd,AIS.wWidth);
	Ed.SetValRO(DraughtEd,AIS.wDraught/10.0);
	DestEd->Text=AIS.sDest;
	int nDay=AIS.ETA_utc.Get_loc().Get().wDay;
	int nMonth=AIS.ETA_utc.Get_loc().Get().wMonth;
	s.printf(L"%02d/%02d",nDay,nMonth);
	ETAEd->Text=s;

	JTime T_loc;
	T_loc.SetFullSec(AIS.dwGetLastUpdate_utc_sec(),true);
	T_loc.ConvertToLocal(g_nCst4TimeBias_hours);
	Ed.SetTimeRO(LastAISTime,T_loc);
	Ed.SetDateRO(LastAISDate,T_loc);

}

void TSysTrackForm::UpdateMsg()
{
	DWORD dwID;
	BYTE ucSeat;
	JTime T_utc;
//Origin
	SysMsg.GetOrigin(T_utc,dwID,ucSeat);
	if (dwID>0)
		{
		OrEd->Text=sGetCst4Site(dwID);
		Ed.SetValRO(OrSeatEd,ucSeat);
		}
	else
		{
		OrEd->Text="";
		OrSeatEd->Text="";
		}
	if (T_utc.bValid())
		{
		JTime T_loc=T_utc.ReturnLocal(g_nCst4TimeBias_hours);
		Ed.SetTimeRO(OrTimeEd,T_loc);
		Ed.SetDateRO(OrDateEd,T_loc);
		}
	else
		{
		OrTimeEd->Text="";
		OrDateEd->Text="";
		}
//Update
	SysMsg.GetUpdate(T_utc,dwID,ucSeat);
	if (dwID>0)
		{
		UpdEd->Text=sGetCst4Site(dwID);
		Ed.SetValRO(UpdSeatEd,ucSeat);
		}
	else
		{
		UpdEd->Text="";
		UpdSeatEd->Text="";
		}
	if (T_utc.bValid())
		{
		JTime T_loc=T_utc.ReturnLocal(g_nCst4TimeBias_hours);
		Ed.SetTimeRO(UpdTimeEd,T_loc);
		Ed.SetDateRO(UpdDateEd,T_loc);
		}
	else
		{
		UpdTimeEd->Text="";
		UpdDateEd->Text="";
		}
	//Last Position
//	dwID=SysMsg.dwGetLastSource();
//	if (dwID>0)
//		LastPosSourceEd->Text=sGetCst4Site(dwID,CST4_VISTA_PERS);
//	else
//		LastPosSourceEd->Text="";
//	JTime T_loc=SysMsg.GetLastSourceTime_loc();
//	if (T_loc.bValid())
//		{
//		LastPosTimeEd->SetTime(T_loc.Get());
//		LastPosDateEd->SetDate(T_loc.Get());
//		}
//	else
//		{
//		LastPosTimeEd->Text="";
//		LastPosDateEd->Text="";
//		}


	if (SysMsg.bAISAvail())
		UpdateAIS();
	if (SysMsg.bRadarAvail())
		UpdateRadar();
	DescrEd->Text=SysMsg.sGetDescr();
	CommentMemo->Text=SysMsg.sGetComments();
	ClassCB->ItemIndex=SysMsg.GetClassification();
	DiscardAISBut->Enabled=!bReadOnly && SysMsg.bAISAvail();
	DiscardRadarBut->Enabled=!bReadOnly && SysMsg.bRadarAvail();

	DWORD dwSource=SysMsg.dwGetLastSource();
   String sSource=g_pSiteDefs->sGetDescrFromID(dwSource);
   SourceLab->Caption=sSource;
	if (g_pSiteDefs->bDesigAvail(dwSource))
		{
//		DesigChk->Checked=SysMsg.bDesignate();
//      UploadPictBut->Enabled=DesigChk->Checked;
		DispPictBut->Enabled=bPictureReceived;
      if (DispPictBut->Enabled)
         DispPictBut->Caption="Display Picture";
      String sAddr=SysMsg.sGetDesAddr();
      int nPort=SysMsg.nGetDesPort();
      if (sAddr.IsEmpty())
      	{
      	sAddr=g_pSiteDefs->sGetDesigAddrFromID(dwSource);
      	nPort=g_pSiteDefs->nGetDesigPortFromID(dwSource);
         }
      DesLab1->Enabled=true;
      DesLab2->Enabled=true;
      DesAddrEd->Text=sAddr;
      DesAddrEd->Enabled=true;
      Ed.nSetVal(DesPortEd,nPort,"");
      Ed.SetLastLimits("Designation Port",0,65535);
      DesPortEd->Enabled=true;
		}
	else
		{
//		DesigChk->Checked=false;
//		DesigChk->Enabled=false;
//      UploadPictBut->Enabled=false;
		DispPictBut->Enabled=false;
      DesLab1->Enabled=false;
      DesLab2->Enabled=false;
      DesAddrEd->Text="";
      DesAddrEd->Enabled=false;
      DesPortEd->Text="";
      DesPortEd->Enabled=false;
		}


}

void __fastcall TSysTrackForm::SubmitButClick(TObject * /*Sender*/)
{
	DWORD dwMsgType;
	SysMsg.SetDescr(DescrEd->Text);
	SysMsg.SetComments(CommentMemo->Text);
	SysMsg.SetClassification((CST4_SHIP_CLASSIFICATION)ClassCB->ItemIndex);
	SysMsg.SetValid();
//	SysMsg.SetDesig(DesigChk->Checked);
   String sAddr=DesAddrEd->Text;
   int nPort=Ed.nGetVal(DesPortEd);
   SysMsg.SetDesIP(sAddr,nPort);
	JTime T_utc=JTime(true,true);
	Cst4_SysTrack::SYSTRACK_ACTION Act;
	if (bNewMsg)
		{
		Act=Cst4_SysTrack::SYSTRACK_CREATE;
		SysMsg.SetOrigin(T_utc,dwCurrentID,ucCurrentSeat);
		}
	else
		Act=Cst4_SysTrack::SYSTRACK_UPDATE;
	SysMsg.SetUpdate(T_utc,dwCurrentID,ucCurrentSeat);
	SysMsg.CalcExpire();
	if (bTXMsg(Act))
		ModalResult=mrOk;
	else
		ShowError("Error Transmitting System Message to Local Hub","System Message Error");
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DeleteButClick(TObject * /*Sender*/)
{
	if (bAskYes("Delete Current System Track"))
		{
		if (bTXMsg(Cst4_SysTrack::SYSTRACK_DELETE))
			ModalResult=mrOk;
		else
			ShowError("Error Deleting System Message","System Message Error");
		}
}
//---------------------------------------------------------------------------


bool TSysTrackForm::bTXMsg(const Cst4_SysTrack::SYSTRACK_ACTION Act)
{
	bool bTXed=false;
	if ((pFixedServer)&&(pFixedServerLock))
		{
		SysMsg.SetTime(JTime(true,true));
		SysMsg.SetSource(dwCurrentID);
		SysMsg.SetAction(Act);
		pFixedServerLock->Acquire();
		pFixedServer->AddTXMessage(SysMsg);
		pFixedServerLock->Release();
      bTXed=true;
		}
	else
		{
		LJTcpClientSockets Sock;
		int nIdx=Sock.nAddClient(this,sHubAddr,CST4_SYSMSG_PORT);
		if (nIdx>=0)
			{
			Sock.ConnectClient(nIdx);
			bool bOK=Sock.bClientConnected(nIdx);
			DWORD dwT_ms=::GetTickCount();
			while((!bOK)&&(::GetTickCount()-dwT_ms<1000))
				{
				Application->ProcessMessages();
				bOK=Sock.bClientConnected(nIdx);
				if (!bOK)
					::SleepEx(10,false);
				}
			if (bOK)
				{
				SysMsg.SetTime(JTime(true,true));
				SysMsg.SetSource(dwCurrentID);
				SysMsg.SetAction(Act);
				BYTE aucBuf[65536];
				int nSize=SysMsg.nCreateTXBuf(aucBuf);
				bTXed=Sock.bSendData(nIdx,aucBuf,nSize);
				}
			Sock.DisconnectClient(nIdx);
			}
      }
	return bTXed;
}

void __fastcall TSysTrackForm::TimerTimer(TObject * /*Sender*/)
{
	++nTimerCnt;
	int nLeft_sec=SysMsg.nGetTimeLeft_sec(JTime(true,true));
	int nH=nLeft_sec/3600;
	int nS=nLeft_sec%3600;
	int nM=nS/60;
	nS%=60;
	String s;
	s.printf(L"%02d:%02d:%02d",nH,nM,nS);
	TimeLeftEd->Text=s;
//	DispPictBut->Enabled=(SysMsg.nGetPictInFolder(sPictFolder,dwUpdate_utc_sec)>0);
   if (bPictureReceived)
   	{
		DispPictBut->Enabled=true;
      DispPictBut->Caption="Display Picture";
      UploadPictBut->Visible=true;
		UploadPictBut->Enabled=true;
      CancelPictBut->Visible=false;
		dwUpdate_utc_sec=0;
      nPictUploadCnt=-1;
      }
   if (nPictUploadCnt>=0)
   	{
      if ((nTimerCnt>nPictUploadCnt+nPictTimeout_sec)||(bPictCancelled))
      	{
         nPictUploadCnt=-1;
         dwUpdate_utc_sec=0;
         if (!bPictCancelled)
	         ShowInfo("Requested Picture Not Received","Picture Upload");
	      UploadPictBut->Visible=true;
			UploadPictBut->Enabled=true;
	      CancelPictBut->Visible=false;
         DispPictBut->Caption="Display Picture";
         }
      }
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::LifeEdChange(TObject * /*Sender*/)
{
	int n=Ed.nGetVal(LifeEd);
	SysMsg.SetMaxLife_h(n);
	SysMsg.CalcExpire();
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::AltAISButClick(TObject * /*Sender*/)
{
	SysAltAISForm=new TSysAltAISForm(this);
	SysAltAISForm->AIS=SysMsg.GetAIS();
	SysAltAISForm->SetAlt(AltAISList);
	int nRet=SysAltAISForm->ShowModal();
   if (nRet==mrOk)
   	{
		SysMsg.SetAIS(SysAltAISForm->AIS);
		UpdateMsg();
      }
   delete SysAltAISForm;
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::AltRadarButClick(TObject * /*Sender*/)
{
	SysAltRadarForm=new TSysAltRadarForm(this);
	if (SysMsg.bRadarAvail())
		{
		SysAltRadarForm->ReplaceBut->Caption="Replace Track";
		Ed.SetValRO(SysAltRadarForm->TrackEd,SysMsg.GetRadar().nTrack);
		SysAltRadarForm->SourceEd->Text=sGetCst4Site(SysMsg.GetRadar().dwID);
		Ed.SetValRO(SysAltRadarForm->SpeedEd,SysMsg.GetRadar().dSpeed_kts());
		Ed.SetValRO(SysAltRadarForm->CourseEd,SysMsg.GetRadar().dCourse_deg());
		}
	else
		SysAltRadarForm->ReplaceBut->Caption="Add Track";
	SysAltRadarForm->SetAlt(AltRadarList);
	int nRet=SysAltRadarForm->ShowModal();
	if (nRet==mrOk)
		{
		SysMsg.SetRadar(&SysAltRadarForm->Radar);
		UpdateMsg();
      }
	delete SysAltRadarForm;
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DiscardAISButClick(TObject * /*Sender*/)
{
	if (!SysMsg.bRadarAvail())
		{
		if (bAskYes("This will Delete the Entire System Track","Discard AIS Track"))
			{
			if (bTXMsg(Cst4_SysTrack::SYSTRACK_DELETE))
				ModalResult=mrOk;
			else
				ShowError("Error Deleting System Message","System Message Error");
			}
		}
	else
		{
		if (bAskYes("Discard AIS Track"))
			{
			SysMsg.ClearAIS();
			UpdateMsg();
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DiscardRadarButClick(TObject * /*Sender*/)
{
	if (!SysMsg.bAISAvail())
		{
		if (bAskYes("This will Delete the Entire System Track","Discard Radar Track"))
			{
			if (bTXMsg(Cst4_SysTrack::SYSTRACK_DELETE))
				ModalResult=mrOk;
			else
				ShowError("Error Deleting System Message","System Message Error");
			}
		}
	else
		{
		if (bAskYes("Discard Radar Track"))
			{
			SysMsg.ClearRadar();
			UpdateMsg();
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DispPictButClick(TObject * /*Sender*/)
{
   int n=SysMsg.nGetPictInFolder(sPictFolder,dwUpdate_utc_sec);
   if (n>0)
   	{
      Cst4PictureForm=new TCst4PictureForm(this);
      for (int i=0; i<n; i++)
         Cst4PictureForm->AddPicture(SysMsg.sGetPictFile(i),SysMsg.sGetPictDescr(i),SysMsg.GetPictTime(i));
      Cst4PictureForm->ShowModal();
		delete Cst4PictureForm;
      }
   else
		ShowError("No Valid Images in Folder","Picture Error");
}
//---------------------------------------------------------------------------


void __fastcall TSysTrackForm::UploadPictButClick(TObject */*Sender*/)
{
	UploadPictBut->Enabled=false;
   bPictureReceived=false;
//	SysMsg.SetDesignate(true);
	SysMsg.SetUploadPicture(true);
   String sAddr=DesAddrEd->Text;
   int nPort=Ed.nGetVal(DesPortEd);
   SysMsg.SetDesIP(sAddr,nPort);
   if (!bTXMsg(Cst4_SysTrack::SYSTRACK_UPDATE))
      ShowError("Error Updating System Message","System Message Error");
	nPictUploadCnt=nTimerCnt;
   DispPictBut->Caption="Waiting for Image ...";
	DispPictBut->Enabled=false;
   UploadPictBut->Visible=false;
  	CancelPictBut->Visible=true;
   bPictCancelled=false;
   dwUpdate_utc_sec=JTime(true,true).dwTotalSec();
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DesigChkClick(TObject */*Sender*/)
{
/*
	if (!bStarting)
   	{
      SysMsg.SetDesig(DesigChk->Checked);
      String sAddr=DesAddrEd->Text;
      int nPort=DesPortEd->nGetVal();
      SysMsg.SetDesIP(sAddr,nPort);
      UploadPictBut->Enabled=DesigChk->Checked;
      if (!bTXMsg(Cst4_SysTrack::SYSTRACK_UPDATE))
         ShowError("Error Deleting System Message","System Message Error");
      }
*/
}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::DesAddrButClick(TObject */*Sender*/)
{
	DWORD dwSource=SysMsg.dwGetLastSource();
	if (g_pSiteDefs->bDesigAvail(dwSource))
		{
     	String sAddr=g_pSiteDefs->sGetDesigAddrFromID(dwSource);
     	int nPort=g_pSiteDefs->nGetDesigPortFromID(dwSource);
      DesAddrEd->Text=sAddr;
      Ed.nSetVal(DesPortEd,nPort,"");
      Ed.SetLastLimits("Designation Port",0,65535);
		}
	else
		{
      DesAddrEd->Text="";
      DesPortEd->Text="";
		}

}
//---------------------------------------------------------------------------

void __fastcall TSysTrackForm::CancelPictButClick(TObject *Sender)
{
	bPictCancelled=true;
}
//---------------------------------------------------------------------------

