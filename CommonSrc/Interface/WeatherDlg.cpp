//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "WeatherDlg.h"
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
//
}
//---------------------------------------------------------------------------
void __fastcall TWeatherForm::FormShow(TObject */*Sender*/)
{
   Ed.Init();
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

void TWeatherForm::ShowDateTime_loc(const JTime& T_loc)
{
   Ed.SetDateTimeRO(DateEd,TimeEd,T_loc);
//	DateEd->SetDate(T_loc.Get());
//	TimeEd->SetTime(T_loc.Get());
}

void TWeatherForm::ShowData(const Cst4_Weather_Data& Data)
{
	if (Data.bPosValid())
		{
		LatLong LL=Data.GetPos_LL();
      Ed.SetLLRO(LatEd,LongEd,LL);
//		LatEd->SetVal(LL.dLat);
//		LongEd->SetVal(LL.dLong);
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

void TWeatherForm::ShowMessage(const Cst4_User_Msg& Msg, const int nCount)
{
	if (Msg.dwGetType()==CST4_USER_WEATHER)
		{
      ShowDateTime_loc(Msg.Time_loc());
		Cst4_Weather_Data* pData=(Cst4_Weather_Data*)Msg.pGetPayload();
		ShowData(*pData);
      Ed.SetValRO(CountEd,nCount);
		}

}

