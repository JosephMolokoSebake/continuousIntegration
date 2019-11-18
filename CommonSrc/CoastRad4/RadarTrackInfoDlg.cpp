//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "RadarTrackInfoDlg.h"
#include "Cst4SysTrackDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TRadarTrackInfoForm *RadarTrackInfoForm;
//---------------------------------------------------------------------------
__fastcall TRadarTrackInfoForm::TRadarTrackInfoForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRadarTrackInfoForm::FormCreate(TObject * /*Sender*/)
{
	sHubAddr="";
	pFixedServer=NULL;
	pFixedServerLock=NULL;
	pTgt=NULL;
	TargetPol.Invalidate();
	bReadOnly=false;
}
//---------------------------------------------------------------------------
void __fastcall TRadarTrackInfoForm::FormShow(TObject * /*Sender*/)
{
   Ed.Init();
	String sPol;
	if (TargetPol.bValid())
		sPol.printf(L" (%1.1fkm %1.1f°)",TargetPol.dRa*0.001,TargetPol.dBe);
	else
		sPol="";
	#ifdef _CST4_DICT
		Caption=g_Dict.sTranslate("Radar Track")+" ["+sSource+"]"+sPol;
	#else
		Caption="Radar Track ["+sSource+"]"+sPol;
	#endif
	if (pTgt)
		{
		Ed.SetValRO(IDEd,pTgt->nID());
		Ed.SetValRO(QualEd,pTgt->ucQuality());
		Ed.SetDateRO(DateEd,pTgt->Time_loc());
		Ed.SetTimeRO(TimeEd,pTgt->Time_loc());
		Ed.SetLLRO(LatEd,LongEd,pTgt->GetLL());
		if (pTgt->bSpeedCourseValid())
			{
			Ed.SetValRO(SpeedEd,pTgt->dSpeed_kts(),1);
			Ed.SetValRO(CourseEd,pTgt->dCourse_deg(),1);
			}
		else
			{
			SpeedEd->Text="N/A";
			CourseEd->Text="N/A";
			}
		}

	String sTimeCapt=DTGrp->Caption;

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption

		g_Dict.Translate(GroupBox1);
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);

		g_Dict.Translate(GroupBox2);
		g_Dict.Translate(Label3);
		g_Dict.Translate(Label4);

		g_Dict.Translate(GroupBox3);
		g_Dict.Translate(Label5);
		g_Dict.Translate(Label6);
		g_Dict.Translate(Label7);
		g_Dict.Translate(Label8);

		sTimeCapt=g_Dict.sTranslate(DTGrp->Caption);



	#endif

	switch(pTgt->nTimeAge_hours())
		{
		case 0:	DTGrp->Caption=sTimeCapt+" (<1h)";	break;
		case 1:	DTGrp->Caption=sTimeCapt+" (2h)";	break;
		case 2:	DTGrp->Caption=sTimeCapt+" (3h)";	break;
		case 3:	DTGrp->Caption=sTimeCapt+" (4h)";	break;
		case 4:	DTGrp->Caption=sTimeCapt+" (5h)";	break;
		case 5:	DTGrp->Caption=sTimeCapt+" (6h)";	break;
		default:	DTGrp->Caption=sTimeCapt+" (>6h)";
		}

      SystemBut->Enabled=!bReadOnly;
      if (!bReadOnly)
      	SystemBut->SetFocus();



}
//---------------------------------------------------------------------------
void __fastcall TRadarTrackInfoForm::FormClose(TObject * /*Sender*/,
		TCloseAction & /*Action*/)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TRadarTrackInfoForm::FormDestroy(TObject * /*Sender*/)
{
//
}
//---------------------------------------------------------------------------


void __fastcall TRadarTrackInfoForm::SystemButClick(TObject */*Sender*/)
{
	Cst4_SysTrack Msg;
	Msg.SetDescr("Radar Track");
	Msg.SetRadar(pTgt);
//	Msg.SetMaxLife_h(nMaxSysTrackLife_h);

	SysTrackForm=new TSysTrackForm(this);
	SysTrackForm->bNewMsg=true;
	SysTrackForm->SetMsg(Msg);

	SysTrackForm->SetCurrentLocation(nVistaPlotID,nVistaPlotSeat);
	SysTrackForm->SetHubAddr(sHubAddr,pFixedServer,pFixedServerLock);
//	SysTrackForm->SetRadar(pTgt);

	int nRet=SysTrackForm->ShowModal();
	delete SysTrackForm;

	if (nRet==mrOk)
		Close();

}
//---------------------------------------------------------------------------

