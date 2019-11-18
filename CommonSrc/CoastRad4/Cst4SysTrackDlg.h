//---------------------------------------------------------------------------

#ifndef Cst4SysTrackDlgH
#define Cst4SysTrackDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include "JEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include "Cst4SysAltAISDlg.h"
#include "Cst4SysAltRadarDlg.h"
#include "VistaServer.h"


//---------------------------------------------------------------------------
class TSysTrackForm : public TForm
{
__published:	// IDE-managed Components
	TButton *SubmitBut;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TGroupBox *GroupBox6;
	TEdit *OrEd;
	TLabel *Label1;
	TEdit *OrSeatEd;
	TEdit *OrTimeEd;
	TEdit *OrDateEd;
	TEdit *DescrEd;
	TLabel *Label2;
	TEdit *MMSIEd;
	TEdit *RadarTrackEd;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *RadarSrcEd;
	TMemo *CommentMemo;
	TBitBtn *CloseBut;
	TButton *DeleteBut;
	TLabel *Label5;
	TLabel *Label6;
	TComboBox *ClassCB;
	TGroupBox *GroupBox7;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TEdit *UpdEd;
	TEdit *UpdSeatEd;
	TEdit *UpdTimeEd;
	TEdit *UpdDateEd;
	TTimer *Timer;
	TBevel *Bevel1;
	TLabel *Label13;
	TEdit *LifeEd;
	TLabel *Label14;
	TEdit *TimeLeftEd;
	TLabel *Label7;
	TButton *AltAISBut;
	TButton *AltRadarBut;
	TEdit *IMOEd;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label15;
	TGroupBox *GroupBox8;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TEdit *SpeedEd;
	TEdit *CourseEd;
	TGroupBox *GroupBox9;
	TLabel *Label20;
	TLabel *Label21;
	TEdit *LatEd;
	TEdit *LongEd;
	TEdit *NameEd;
	TEdit *CallsignEd;
	TLabel *Label22;
	TEdit *CargoEd;
	TLabel *Label23;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label27;
	TEdit *LengthEd;
	TEdit *WidthEd;
	TEdit *DraughtEd;
	TLabel *Label28;
	TEdit *DestEd;
	TGroupBox *GroupBox10;
	TLabel *Label24;
	TLabel *Label29;
	TLabel *Label30;
	TLabel *Label31;
	TEdit *RadarSpeedEd;
	TEdit *RadarCourseEd;
	TGroupBox *GroupBox11;
	TLabel *Label32;
	TLabel *Label33;
	TEdit *RadarLatEd;
	TEdit *RadarLongEd;
	TLabel *Label34;
	TEdit *RadarQualityEd;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
	TEdit *ETAEd;
	TEdit *NavStatusEd;
	TLabel *Label38;
	TLabel *Label39;
	TBevel *Bevel2;
	TEdit *LastAISDate;
	TEdit *LastAISTime;
	TEdit *LastRadarDate;
	TEdit *LastRadarTime;
	TLabel *Label42;
	TButton *DiscardAISBut;
	TButton *DiscardRadarBut;
	TGroupBox *GroupBox12;
	TButton *DispPictBut;
	TButton *UploadPictBut;
	TLabel *SourceLab;
	TEdit *DesAddrEd;
	TEdit *DesPortEd;
	TLabel *DesLab2;
	TLabel *DesLab1;
	TButton *DesAddrBut;
	TBitBtn *CancelPictBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SubmitButClick(TObject *Sender);
	void __fastcall DeleteButClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall LifeEdChange(TObject *Sender);
	void __fastcall AltAISButClick(TObject *Sender);
	void __fastcall AltRadarButClick(TObject *Sender);
	void __fastcall DiscardAISButClick(TObject *Sender);
	void __fastcall DiscardRadarButClick(TObject *Sender);
	void __fastcall DispPictButClick(TObject *Sender);
	void __fastcall UploadPictButClick(TObject *Sender);
	void __fastcall DesigChkClick(TObject *Sender);
	void __fastcall DesAddrButClick(TObject *Sender);
	void __fastcall CancelPictButClick(TObject *Sender);
private:	// User declarations

   JEdit Ed;

	Cst4_SysTrack SysMsg;

	JList<AISSepStruct> AltAISList;
	JList<RadarSepStruct> AltRadarList;

	String sHubAddr;

	VistaServer* pFixedServer;
   TCriticalSection* pFixedServerLock;

	bool bStarting;

	BYTE ucCurrentSeat;
	DWORD dwCurrentID;

   int nTimerCnt;
   int nPictUploadCnt;
   DWORD dwUpdate_utc_sec;

   bool bPictCancelled;

	void UpdateAIS();
	void UpdateRadar();
	void UpdateMsg();

	bool bTXMsg(const Cst4_SysTrack::SYSTRACK_ACTION Act);

	String sGetCargoSecond(const int n,const String sPrefixDelimiter="\r\n",const String sPostfixDelimiter="");
	String sGetShipAndCargoType(const String sPrefixDelimiter="\r\n",const String sPostfixDelimiter="");

public:		// User declarations
	__fastcall TSysTrackForm(TComponent* Owner);

	bool bNewMsg;
	bool bReadOnly;

	void SetMsg(Cst4_SysTrack _Msg)
	{
		SysMsg=_Msg;
	}

	Cst4_SysTrack GetMsg()
	{
		return SysMsg;
	}

	void SetHubAddr(const String sAddr,	VistaServer* pServer, TCriticalSection* pLock)
	{
		sHubAddr=sAddr;
		pFixedServer=pServer;
		pFixedServerLock=pLock;
	}

	void SetCurrentLocation(const DWORD dwID, const BYTE ucSeat)
	{
		dwCurrentID=dwID;
		ucCurrentSeat=ucSeat;
	}

	void SetAltAIS(JList<AISSepStruct> List)
	{
		AltAISList=List;
	}

	void SetAltRadar(JList<RadarSepStruct> List)
	{
		AltRadarList=List;
	}

	String sPictFolder;
   int nPictTimeout_sec;

	bool bPictureReceived;

};
//---------------------------------------------------------------------------
extern PACKAGE TSysTrackForm *SysTrackForm;
//---------------------------------------------------------------------------
#endif
