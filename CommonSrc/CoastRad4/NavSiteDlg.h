//---------------------------------------------------------------------------

#ifndef NavSiteDlgH
#define NavSiteDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "NavSite.h"
#include "JEdit.h"
#include "Cst4Defs.h"
#include <Vcl.ComCtrls.hpp>


//---------------------------------------------------------------------------
class TNavSiteForm : public TForm
{
__published:	// IDE-managed Components
	TCheckListBox *SiteLB;
	TGroupBox *SiteGrp;
	TBitBtn *OKBut;
	TBitBtn *CancelBut;
	TButton *AddSiteBut;
	TButton *DelSiteBut;
	TGroupBox *GroupBox2;
	TEdit *DescrEd;
	TGroupBox *GroupBox3;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TCheckBox *CoverChk;
	TCheckBox *AutoChk;
	TLabel *RaLab1;
	TLabel *RaLab2;
	TLabel *Label7;
	TLabel *LineLab;
	TEdit *MaxRaEd;
	TEdit *WidthEd;
	TEdit *LatEd;
	TEdit *LongEd;
	TEdit *HeightEd;
	TPanel *LinePan;
	TPanel *FillPan;
	TCheckBox *FillChk;
	TButton *UpBut;
	TButton *DownBut;
	TButton *RefreshBut;
	TButton *DeleteAllBut;
	TGroupBox *GroupBox1;
	TButton *FontBut;
	TLabel *Label5;
	TLabel *Label6;
	TCheckBox *DispNameChk;
	TCheckBox *CheckBox2;
	TPanel *SymColPan;
	TEdit *SymSizeEd;
	TUpDown *SymSizeUD;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall AddSiteButClick(TObject *Sender);
	void __fastcall DelSiteButClick(TObject *Sender);
//	void __fastcall FontButClick(TObject *Sender);
	void __fastcall LinePanClick(TObject *Sender);
	void __fastcall FillPanClick(TObject *Sender);
	void __fastcall CoverChkClick(TObject *Sender);
	void __fastcall AutoChkClick(TObject *Sender);
	void __fastcall HeightEdChange(TObject *Sender);
	void __fastcall FillChkClick(TObject *Sender);
	void __fastcall DescrEdChange(TObject *Sender);
	void __fastcall SiteLBClick(TObject *Sender);
	void __fastcall UpButClick(TObject *Sender);
	void __fastcall DownButClick(TObject *Sender);
	void __fastcall RefreshButClick(TObject *Sender);
	void __fastcall DeleteAllButClick(TObject *Sender);
//	void __fastcall SymColPanClick(TObject *Sender);
//	void __fastcall FormCreate(TObject *Sender);
//	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SymColPanClick(TObject *Sender);
	void __fastcall FontButClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

   JEdit Ed;

	NavSiteStruct* pSite;
	int nSite;

//   CST4_PERSONALITY Personality;

	void GetSite(const int n);
	void SetSite(const int n);
	void CalcMaxRa();

public:		// User declarations
	__fastcall TNavSiteForm(TComponent* Owner);

//	void SetPersonality(const CST4_PERSONALITY Pers)
//	{
//		Personality=Pers;
//	}

	JList<NavSiteStruct> NavSite;
	TFont* pFont;
   int nSymbolSize;
   TColor SymbolColor;


//
};
//---------------------------------------------------------------------------
extern PACKAGE TNavSiteForm *NavSiteForm;
//---------------------------------------------------------------------------
#endif
