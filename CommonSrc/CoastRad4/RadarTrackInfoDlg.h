//---------------------------------------------------------------------------

#ifndef RadarTrackInfoDlgH
#define RadarTrackInfoDlgH
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
#include "VistaServer.h"
#include "JFile.h"

//---------------------------------------------------------------------------
class TRadarTrackInfoForm : public TForm
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
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *IDEd;
	TEdit *QualEd;
	TButton *SystemBut;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SystemButClick(TObject *Sender);
private:	// User declarations

	JEdit Ed;

	Cst4_Radar_Track* pTgt;
	String sSource;
	Polar TargetPol;

	int nVistaPlotID;
	int nVistaPlotSeat;
	String sHubAddr;
	VistaServer* pFixedServer;
	TCriticalSection* pFixedServerLock;

public:		// User declarations
	__fastcall TRadarTrackInfoForm(TComponent* Owner);

   bool bReadOnly;

	void SetInfo(Cst4_Radar_Track* _pTgt, const String _sSource, const Polar& Pol)
	{
		pTgt=_pTgt;
		sSource=_sSource;
		TargetPol=Pol;
	}

	void SetVistaPlotID(const int nOwnID, const int nDispNum)
	{
		nVistaPlotID=nOwnID;
		nVistaPlotSeat=nDispNum;
	}

	void SetHubAddr(const String sAddr,VistaServer* pServer, TCriticalSection* pLock)
	{
		sHubAddr=sAddr;
		pFixedServer=pServer;
		pFixedServerLock=pLock;
	}


};
//---------------------------------------------------------------------------
extern PACKAGE TRadarTrackInfoForm *RadarTrackInfoForm;
//---------------------------------------------------------------------------
#endif
