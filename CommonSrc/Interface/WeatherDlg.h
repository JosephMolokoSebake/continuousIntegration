//---------------------------------------------------------------------------

#ifndef WeatherDlgH
#define WeatherDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "JEdit.h"
#include "Cst4Messages.h"
#include "Cst4UserDefs.h"

//---------------------------------------------------------------------------
class TWeatherForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TGroupBox *GroupBox2;
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
	TLabel *Label17;
	TEdit *BarEd;
	TEdit *TempEd;
	TEdit *HumidEd;
	TEdit *DewEd;
	TEdit *DirEd;
	TEdit *SpeedEd;
	TEdit *DateEd;
	TEdit *TimeEd;
	TEdit *CountEd;
	TEdit *LatEd;
	TEdit *LongEd;
	TEdit *AltEd;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

   JEdit Ed;

public:		// User declarations
	__fastcall TWeatherForm(TComponent* Owner);

	void ShowDateTime_loc(const JTime& T_loc);
	void ShowData(const Cst4_Weather_Data& Data);

	void ShowMessage(const Cst4_User_Msg& Msg, const int nCount);

};
//---------------------------------------------------------------------------
extern PACKAGE TWeatherForm *WeatherForm;
//---------------------------------------------------------------------------
#endif
