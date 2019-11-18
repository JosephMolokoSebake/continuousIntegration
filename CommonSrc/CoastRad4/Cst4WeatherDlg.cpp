//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4WeatherDlg.h"
#include "JFileDlg.h"
#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif


//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TWeatherForm *WeatherForm;
//---------------------------------------------------------------------------
__fastcall TWeatherForm::TWeatherForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWeatherForm::FormCreate(TObject */*Sender*/)
{
	nSite=-1;
}
//---------------------------------------------------------------------------
void __fastcall TWeatherForm::FormShow(TObject */*Sender*/)
{
   Ed.Init();
	TimeLB->Clear();
	LocLB->Clear();
	SiteList.Sort();
	SiteList.Pack();
	for (int nS=0; nS<SiteList.nGetCount(); nS++)
		{
		SiteList[nS]->InfoList.Sort();
		SiteList[nS]->InfoList.Pack();
		LocLB->Items->Add(SiteList[nS]->sLocation);
		}
	LocLB->ItemIndex=0;
	LocLBClick(0);
	LocLB->SetFocus();

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(GroupBox1);
		g_Dict.Translate(GroupBox2);
		g_Dict.Translate(GroupBox3);
		g_Dict.Translate(GroupBox4);
		g_Dict.Translate(GroupBox5);
		g_Dict.Translate(GroupBox6);
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);
		g_Dict.Translate(Label3);
		g_Dict.Translate(Label5);
		g_Dict.Translate(Label7);
		g_Dict.Translate(Label9);
		g_Dict.Translate(Label11);
		g_Dict.Translate(Label13);
		g_Dict.Translate(Label14);
		g_Dict.Translate(Label15);
		g_Dict.Translate(Label16);
		g_Dict.Translate(StoreBut);
	#endif


}
//---------------------------------------------------------------------------
void __fastcall TWeatherForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TWeatherForm::FormDestroy(TObject */*Sender*/)
{
//
}
//---------------------------------------------------------------------------

void TWeatherForm::SetWeather(JList<Cst4_User_Msg> FullList)
{
	Cst4_User_Msg* pW;
	SiteStruct* pS;
	SiteStruct* pSite;
	SiteList.Clear();
	FullList.GoFirst();
	while(!FullList.bLast())
		{
		pW=FullList.pNext();
		pSite=NULL;
		SiteList.GoFirst();
		while((!SiteList.bLast())&&(!pSite))
			{
			pS=SiteList.pNext();
			if (pS->wLocation==pW->wSourceLocation())
				pSite=pS;
			}
		if (!pSite)
			{
			pSite=new SiteStruct;
			pSite->wLocation=pW->wSourceLocation();
			pSite->sLocation=g_pSiteDefs->sGetLocationFromID(pSite->wLocation);
			SiteList.nAdd(pSite);
			}
      pSite->InfoList.nAdd(new Cst4_User_Msg(*pW));
		}
}


void TWeatherForm::ShowDateTime(const JTime& T_loc)
{
	JTime T=T_loc;
	T.RoundToMinute();
	Ed.SetDateRO(DateEd,T);
	Ed.SetTimeRO(TimeEd,T);
}

void TWeatherForm::ShowData(const Cst4_Weather_Data& Data)
{
	if (Data.bPosValid())
		{
		LatLong LL=Data.GetPos_LL();
		Ed.SetLLRO(LatEd,LongEd,LL);
		Ed.SetValRO(AltEd,Data.dGetAlt_m());
		}
	else
		{
		LatEd->Text="";
		LongEd->Text="";
		AltEd->Text="";
		}
	String s;
	if (Data.bPressureValid())
		BarEd->Text=s.sprintf(L"%3.3f",Data.dGetPressure());
	else
		BarEd->Text="";
	if (Data.bAirTempValid())
		TempEd->Text=s.sprintf(L"%1.1f",Data.dGetAirTemp());
	else
		TempEd->Text="";
	if (Data.bRelHumidValid())
		HumidEd->Text=s.sprintf(L"%1.1f",Data.dGetRelHumid());
	else
		HumidEd->Text="";
	if (Data.bDewPointValid())
		DewEd->Text=s.sprintf(L"%1.1f",Data.dGetDewPoint());
	else
		DewEd->Text="";
	if (Data.bTrueWindDirValid())
		DirEd->Text=s.sprintf(L"%1.1f",Data.dGetTrueWindDir());
	else
		DirEd->Text="";
	if (Data.bWindSpeedValid())
		SpeedEd->Text=s.sprintf(L"%1.1f",Data.dGetWindSpeed());
	else
		SpeedEd->Text="";
}

void TWeatherForm::ShowMessage(const Cst4_User_Msg& Msg)
{
	if (Msg.dwGetType()==CST4_USER_WEATHER)
		{
		ShowDateTime(Msg.Time_loc());
		Cst4_Weather_Data* pData=(Cst4_Weather_Data*)Msg.pGetPayload();
		ShowData(*pData);
		}

}

void __fastcall TWeatherForm::LocLBClick(TObject */*Sender*/)
{
	TimeLB->Clear();
	nSite=LocLB->ItemIndex;
	if (nSite>=0)
		{
		SiteStruct* pS=SiteList[nSite];
		JTime T;
		String s;
		for (int nT=0; nT<pS->InfoList.nGetCount(); nT++)
			{
			T=pS->InfoList[nT]->Time_loc();
			T.RoundToMinute();
			s.printf(L"%02d:%02d %02d/%02d/%04d",T.Get().wHour,T.Get().wMinute,T.Get().wDay,T.Get().wMonth,T.Get().wYear);
			TimeLB->Items->Add(s);
			}
      TimeLB->ItemIndex=0;
		TimeLBClick(0);
		}
	else
		TimeLB->ItemIndex=-1;
	StoreBut->Enabled=(nSite>=0);
}
//---------------------------------------------------------------------------

void __fastcall TWeatherForm::TimeLBClick(TObject */*Sender*/)
{
	int n=TimeLB->ItemIndex;
	if ((n>=0)&&(nSite>=0))
		ShowMessage(*SiteList[nSite]->InfoList[n]);
}
//---------------------------------------------------------------------------

void __fastcall TWeatherForm::StoreButClick(TObject */*Sender*/)
{
	String sFile="Met Data "+SiteList[nSite]->sLocation;
	sFile=sValidateFileName(sFile);
	sFile=sFile+" "+SiteList[nSite]->InfoList[0]->Time_loc().sFileName("csv");
	sFile=sSelectSaveFileDef("Comma Delimited Files (*.csv)","*.csv","Specify Export File",sFile);
	if (!sFile.IsEmpty())
		{
		JFile Fil('O',JFile::ASCII_TYPE);
		JFile::FILE_ERROR E=Fil.Create(sFile);
		if (!E)
			E=Fil.TextLine("Time,Date,Pressure (bar),Air Temp (C),Rel Humid (%),Dew Point (C),Wind Dir (deg T),Wind Speed (kts)");
		SiteStruct* pS=SiteList[nSite];
		JTime T;
		String s,sV;
		Cst4_Weather_Data* pD;
		for (int nT=0; (nT<pS->InfoList.nGetCount())&&(!E); nT++)
			{
			T=pS->InfoList[nT]->Time_loc();
			T.RoundToMinute();
			s.printf(L"%02d:%02d,%02d/%02d/%04d",T.Get().wHour,T.Get().wMinute,T.Get().wDay,T.Get().wMonth,T.Get().wYear);
			pD=(Cst4_Weather_Data*)pS->InfoList[nT]->pGetPayload();
			if (pD->bPressureValid())
				s=s+sV.sprintf(L",%3.3f",pD->dGetPressure());
			else
				s=s+",";
			if (pD->bAirTempValid())
				s=s+sV.sprintf(L",%1.1f",pD->dGetAirTemp());
			else
				s=s+",";
			if (pD->bRelHumidValid())
				s=s+sV.sprintf(L",%1.1f",pD->dGetRelHumid());
			else
				s=s+",";
			if (pD->bDewPointValid())
				s=s+sV.sprintf(L",%1.1f",pD->dGetDewPoint());
			else
				s=s+",";
			if (pD->bTrueWindDirValid())
				s=s+sV.sprintf(L",%1.1f",pD->dGetTrueWindDir());
			else
				s=s+",";
			if (pD->bWindSpeedValid())
				s=s+sV.sprintf(L",%1.1f",pD->dGetWindSpeed());
			else
				s=s+",";
			E=Fil.TextLine(s);
			}
		Fil.ShowError(E);
		}
}
//---------------------------------------------------------------------------

