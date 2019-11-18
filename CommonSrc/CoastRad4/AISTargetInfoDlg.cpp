//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "AISTargetInfoDlg.h"
#include "AISDataDefs_210.h"
#include "AISExtInfoDlg.h"
#include "JUtils.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TAISTargetInfoForm *AISTargetInfoForm;
//---------------------------------------------------------------------------
__fastcall TAISTargetInfoForm::TAISTargetInfoForm(TComponent* Owner)
   : TForm(Owner)
{
	sHubAddr="";
	pFixedServer=NULL;
	pFixedServerLock=NULL;
	bReadOnly=false;
}
//---------------------------------------------------------------------------
void __fastcall TAISTargetInfoForm::FormCreate(TObject *)
{
	TargetPol.Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TAISTargetInfoForm::FormShow(TObject *)
{
   Ed.Init();

	Ed.SetValRO(MMSIEd,pTgt->nGetMMSI());

	#ifdef _CST4_DICT
		if (sSource.IsEmpty())
			Caption=g_Dict.sTranslate("AIS Information");
		else
			{
			String sPol="";
			if (TargetPol.bValid())
				sPol.printf(L" (%1.1fkm %1.1f°)",TargetPol.dRa*0.001,TargetPol.dBe);
			Caption=g_Dict.sTranslate("Data Source")+": "+sSource+sPol;
			}
	#else
		if (sSource.IsEmpty())
			Caption="AIS Information";
		else
			{
			String sPol="";
			if (TargetPol.bValid())
				sPol.printf(L" (%1.1fkm %1.1f°)",TargetPol.dRa*0.001,TargetPol.dBe);
			Caption="Data Source: "+sSource+sPol;
			}
	#endif
	Grid->ColWidths[0]=128;
   Grid->ColWidths[1]=256;
   if (pStat)
      {
      AISMsgClass AIS;
      AIS.SetExtraInfoBuf(pStat->pucGetData(),pStat->wGetDataSize());
      NameEd->Text=pStat->sShipName();
      if (AIS.bCallSignValid())
         CallEd->Text=AIS.sGetCallSign();
      else
         CallEd->Text=L"";
      if (AIS.nGetParamStringsCount()>0)
         {
         Grid->RowCount=AIS.nGetParamStringsCount();
         Grid->Height=(Grid->DefaultRowHeight+1)*Grid->RowCount;
         for (int i=0; i<AIS.nGetParamStringsCount(); i++)
				{
				#ifdef _CST4_DICT
					Grid->Cells[0][i]=g_Dict.sTranslate(AIS.sGetParamDescription(i));
					Grid->Cells[1][i]=g_Dict.sTranslate(AIS.sGetParamValue(i));
				#else
					Grid->Cells[0][i]=AIS.sGetParamDescription(i);
					Grid->Cells[1][i]=AIS.sGetParamValue(i);
				#endif
				}
			}
		MMSIGrp->Hide();
		StaticGrp->Top=88;
		TGridRect myRect;
		myRect.Left = -1;
		myRect.Top = -1;
		myRect.Right = -1;
		myRect.Bottom = -1;
		Grid->Selection = myRect;
      }
   else
      {
      StaticGrp->Hide();
      SysTrackBut->Top=144;
      ClientHeight=183;
      DTGrp->SetFocus();
		}


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

	String sTimeCapt=DTGrp->Caption;

	#ifdef _CST4_DICT
//		g_Dict.Translate(this);	//Form Caption
		sTimeCapt=g_Dict.sTranslate(DTGrp->Caption);

		g_Dict.Translate(GroupBox1);
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);

		g_Dict.Translate(GroupBox3);
		g_Dict.Translate(Label5);
		g_Dict.Translate(Label6);
		g_Dict.Translate(Label7);
		g_Dict.Translate(Label8);

		g_Dict.Translate(MMSIGrp);
		g_Dict.Translate(Label10);

		g_Dict.Translate(StaticGrp);
		g_Dict.Translate(Label3);
		g_Dict.Translate(Label4);


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

	SysTrackBut->Enabled=!bReadOnly;
   if (!bReadOnly)
		SysTrackBut->SetFocus();

}
//---------------------------------------------------------------------------
void __fastcall TAISTargetInfoForm::FormClose(TObject *,
		TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TAISTargetInfoForm::FormDestroy(TObject *)
{
//
}
//---------------------------------------------------------------------------


void __fastcall TAISTargetInfoForm::SysTrackButClick(TObject */*Sender*/)
{
	Cst4_SysTrack Msg;
	Msg.SetAIS(pTgt);
	Msg.SetAIS(pStat);
   if (pStat)
		Msg.SetDescr("Vessel: "+pStat->sShipName());
   else
			Msg.SetDescr("Unknown Vessel");
	Msg.SetMaxLife_h(nMaxSysTrackLife_h);

	SysTrackForm=new TSysTrackForm(this);
	SysTrackForm->bNewMsg=true;
	SysTrackForm->SetMsg(Msg);
	SysTrackForm->SetCurrentLocation(nVistaPlotID,nVistaPlotSeat);
	SysTrackForm->SetHubAddr(sHubAddr,pFixedServer,pFixedServerLock);

	SysTrackForm->SetAltAIS(AltAISList);

	int nRet=SysTrackForm->ShowModal();
	delete SysTrackForm;
	if (nRet==mrOk)
		{
		SysTrackBut->Enabled=false;
		Close();
		}
}
//---------------------------------------------------------------------------

