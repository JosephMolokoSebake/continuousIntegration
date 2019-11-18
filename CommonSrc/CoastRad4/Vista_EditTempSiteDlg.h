//---------------------------------------------------------------------------

#ifndef Vista_EditTempSiteDlgH
#define Vista_EditTempSiteDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#include "JEdit.h"

//---------------------------------------------------------------------------
class TEditTempSiteForm : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TEdit *LocEd;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *AISLatEd;
	TEdit *AISLongEd;
	TEdit *AISHeightEd;
	TLabel *Label1;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *RadarLatEd;
	TEdit *RadarLongEd;
	TEdit *RadarHeightEd;
	TButton *ReadBut;
	TButton *SaveBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ReadButClick(TObject *Sender);
	void __fastcall SaveButClick(TObject *Sender);
private:	// User declarations

   enum
   {
      TEMP_SITE_VERSION = 1000,
   };

   JEdit Ed;

   String sLocDescr;

   LatLong AISLL;
   int nAISHeight_m;

   LatLong RadarLL;
   int nRadarHeight_m;

   void SetPar();
   bool bGetPar();

   void ReadPar(String sFil="");
   void SavePar(String sFil="");

public:		// User declarations
	__fastcall TEditTempSiteForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditTempSiteForm *EditTempSiteForm;
//---------------------------------------------------------------------------
#endif
