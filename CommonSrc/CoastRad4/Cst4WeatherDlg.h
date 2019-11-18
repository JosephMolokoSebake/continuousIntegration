//---------------------------------------------------------------------------

#ifndef Cst4WeatherDlgH
#define Cst4WeatherDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "JEdit.h"
#include "Cst4Messages.h"
#include "Cst4UserDefs.h"
#include "Cst4SiteDefs.h"

//---------------------------------------------------------------------------
class TWeatherForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TEdit *BarEd;
	TEdit *TempEd;
	TEdit *HumidEd;
	TEdit *DewEd;
	TEdit *DirEd;
	TEdit *SpeedEd;
	TGroupBox *GroupBox2;
	TEdit *DateEd;
	TEdit *TimeEd;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *LatEd;
	TEdit *LongEd;
	TEdit *AltEd;
	TGroupBox *GroupBox5;
	TListBox *LocLB;
	TGroupBox *GroupBox6;
	TListBox *TimeLB;
	TButton *StoreBut;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall LocLBClick(TObject *Sender);
	void __fastcall TimeLBClick(TObject *Sender);
	void __fastcall StoreButClick(TObject *Sender);
private:	// User declarations

   JEdit Ed;

	struct SiteStruct
	{
		WORD wLocation;
		String sLocation;
		JList<Cst4_User_Msg> InfoList;

		bool operator < (const SiteStruct& S) const
		{
			return (sLocation.CompareIC(S.sLocation)<0);
		}

		bool operator > (const SiteStruct& S) const
		{
			return (sLocation.CompareIC(S.sLocation)>0);
		}

	};

	JList<SiteStruct> SiteList;

   int nSite;

	void ShowDateTime(const JTime& T_loc);
	void ShowData(const Cst4_Weather_Data& Data);

	void ShowMessage(const Cst4_User_Msg& Msg);

public:		// User declarations
	__fastcall TWeatherForm(TComponent* Owner);

	void SetWeather(JList<Cst4_User_Msg> FullList);

};
//---------------------------------------------------------------------------
extern PACKAGE TWeatherForm *WeatherForm;
//---------------------------------------------------------------------------
#endif
