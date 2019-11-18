//---------------------------------------------------------------------------

#ifndef SiteStatusDlgH
#define SiteStatusDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "SyncObjs.hpp"

#include "Cst4SiteDefs.h"
#include "Cst4Stats.h"
#include <Vcl.ExtCtrls.hpp>
#include "JEdit.h"

//---------------------------------------------------------------------------
class TSiteStatusForm : public TForm
{
__published:	// IDE-managed Components
   TStringGrid *StatGrid;
   TButton *SaveBut;
   TGroupBox *SiteGrp;
   TPaintBox *PB;
   TEdit *DateEd;
   TLabel *Label1;
   TGroupBox *GroupBox1;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
	TEdit *MsgEd;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall StatGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
   void __fastcall SaveButClick(TObject *Sender);
   void __fastcall StatGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
   void __fastcall PBPaint(TObject *Sender);
   void __fastcall PBMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
private:	// User declarations

   JEdit Ed;

   TCriticalSection* pLock;
   Cst4Stats* pStat;

  JList<Cst4SiteStats> SiteList;
  JTime CurrentTime_loc;
  JTime CurrentTime_utc;
  double dCurrent_loc_sec;
  double dCurrent_utc_sec;
  DWORD dwCurrent_loc_sec;
  DWORD dwCurrent_utc_sec;

  int nSite;


   void ShowSite(const int nR, Cst4SiteStats* pS);


public:		// User declarations
   __fastcall TSiteStatusForm(TComponent* Owner);


	void SetStats(Cst4Stats* _pStat, TCriticalSection* _pLock)
	{
		pStat=_pStat;
		pLock=_pLock;
	}

	void UpdateStats(const JTime& CurrentTime_loc);

};
//---------------------------------------------------------------------------
extern PACKAGE TSiteStatusForm *SiteStatusForm;
//---------------------------------------------------------------------------
#endif
