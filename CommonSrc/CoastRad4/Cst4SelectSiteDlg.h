//---------------------------------------------------------------------------

#ifndef Cst4SelectSiteDlgH
#define Cst4SelectSiteDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#include "Cst4SiteDefs.h"

enum VISTA_SYSTEMS_SELECTION
{
	SYSTEMS_ALL,
	SYSTEMS_AIS,
	SYSTEMS_RADAR,
	SYSTEMS_IFF,
	SYSTEMS_WEATHER,
	SYSTEMS_GPS,
	SYSTEMS_SENSORS,
	SYSTEMS_SITEHUB,
	SYSTEMS_HUB,
	SYSTEMS_DISPLAY,
	SYSTEMS_REPLAY,
	SYSTEMS_HUB_DISPLAY,
	SYSTEMS_ALARM_PROC,
	SYSTEMS_CAMERA,
};

//---------------------------------------------------------------------------
class TCst4SelectSiteForm : public TForm
{

__published:	// IDE-managed Components
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TListBox *LocLB;
	TBitBtn *OKBut;
	TBitBtn *CancelBut;
	TListBox *CountryLB;
	TGroupBox *TypeGrp;
	TListBox *TypeLB;
	TButton *FullInfoBut;
	TCheckBox *DispAllChk;
	TButton *EditTempBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CountryLBClick(TObject *Sender);
	void __fastcall LocLBClick(TObject *Sender);
	void __fastcall TypeLBDblClick(TObject *Sender);
	void __fastcall DispAllChkClick(TObject *Sender);
	void __fastcall FullInfoButClick(TObject *Sender);
	void __fastcall EditTempButClick(TObject *Sender);
private:	// User declarations

	int nCountryIdx;
	int nLocIdx;
	int nTypeIdx;
	WORD wSite;
	String sDescr;
	String sAbbr;
	VISTA_SYSTEMS_SELECTION SystemType;

   bool bShowAll;

//	CST4_PERSONALITY Personality;

//	Cst4SiteDefs* pDefs;
//	bool bMakeDefs;

//	bool bValidType(const String sType);

	bool bValidType(const Cst4SiteDefs::SYSTEM_TYPE Sys);


public:		// User declarations
	__fastcall TCst4SelectSiteForm(TComponent* Owner);

	void SetSite(const WORD _wSite)
	{
		wSite=_wSite;
	}

	WORD wGetSite() const
	{
		return wSite;
	}

	void SetSystemType(const VISTA_SYSTEMS_SELECTION Sys)
	{
		SystemType=Sys;
	}

//	void SetDefs(Cst4SiteDefs* _pDefs)
//	{
//		pDefs=_pDefs;
//	}

	String sGetDescr()
	{
		return sDescr;
	}

	String sGetAbbr()
	{
		return sAbbr;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TCst4SelectSiteForm *Cst4SelectSiteForm;
//---------------------------------------------------------------------------

WORD wSelectVistaSite(TComponent* Owner, const WORD wCurrentSite, const VISTA_SYSTEMS_SELECTION SystemType);

#endif
