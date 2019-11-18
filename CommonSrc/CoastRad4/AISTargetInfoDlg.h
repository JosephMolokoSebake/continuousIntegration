//---------------------------------------------------------------------------

#ifndef AISTargetInfoDlgH
#define AISTargetInfoDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>

#include "Cst4Messages.h"
#include "Cst4DataClient.h"

#include "Cst4SysTrackDlg.h"

#include "JFile.h"

//---------------------------------------------------------------------------
class TAISTargetInfoForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *LatEd;
   TEdit *LongEd;
   TGroupBox *DTGrp;
   TEdit *DateEd;
   TEdit *TimeEd;
   TGroupBox *GroupBox3;
   TLabel *Label5;
   TLabel *Label6;
   TEdit *SpeedEd;
   TEdit *CourseEd;
   TLabel *Label7;
   TLabel *Label8;
   TGroupBox *StaticGrp;
   TStringGrid *Grid;
   TLabel *Label3;
   TLabel *Label4;
   TEdit *NameEd;
   TEdit *CallEd;
	TGroupBox *MMSIGrp;
	TLabel *Label10;
	TEdit *MMSIEd;
	TButton *SysTrackBut;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SysTrackButClick(TObject *Sender);
private:	// User declarations

   JEdit Ed;

	Cst4_AIS_Dynamic* pTgt;
	Cst4_AIS_Static* pStat;
	String sSource;
	Polar TargetPol;
	int nVistaPlotID;
	int nVistaPlotSeat;


	String sHubAddr;
	VistaServer* pFixedServer;
	TCriticalSection* pFixedServerLock;
	int nMaxSysTrackLife_h;

	JList<AISSepStruct> AltAISList;


public:		// User declarations
	__fastcall TAISTargetInfoForm(TComponent* Owner);

	bool bReadOnly;

	void SetInfo(Cst4_AIS_Dynamic* _pTgt, Cst4_AIS_Static* _pStat, const String _sSource, const Polar& Pol)
	{
		pTgt=_pTgt;
		pStat=_pStat;
		sSource=_sSource;
		TargetPol=Pol;
}

	void SetInfo(Cst4_AIS_Dynamic* _pTgt, Cst4_AIS_Static* _pStat)
	{
		pTgt=_pTgt;
		pStat=_pStat;
		sSource="";
		TargetPol.Invalidate();
	}

	void SetVistaPlotID(const int nOwnID, const int nDispNum)
	{
		nVistaPlotID=nOwnID;
		nVistaPlotSeat=nDispNum;
	}

	void SetHubAddr(const String sAddr, VistaServer* pServer, TCriticalSection* pLock)
	{
		sHubAddr=sAddr;
		pFixedServer=pServer;
		pFixedServerLock=pLock;
	}

	void SetMaxSysTrackLife(const nLife_h)
	{
		nMaxSysTrackLife_h=nLife_h;
	}

	void SetAltAIS(JList<AISSepStruct> List)
	{
		AltAISList=List;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TAISTargetInfoForm *AISTargetInfoForm;
//---------------------------------------------------------------------------
#endif
