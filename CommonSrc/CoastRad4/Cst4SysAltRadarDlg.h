//---------------------------------------------------------------------------

#ifndef Cst4SysAltRadarDlgH
#define Cst4SysAltRadarDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "JEdit.h"
#include <Vcl.Buttons.hpp>

#include "Cst4Messages.h"

struct RadarSepStruct
{
	Cst4_Radar_Track T;
	int nSep_m;

	RadarSepStruct()
	{
		T.Invalidate();
		nSep_m=-1;
	}

	RadarSepStruct& operator = (const RadarSepStruct& R)
	{
		T=R.T;
		nSep_m=R.nSep_m;
		return *this;
	}

	~RadarSepStruct()
	{
	}

		bool operator < (const RadarSepStruct& R) const
		{
			return (nSep_m<R.nSep_m);
		}

		bool operator > (const RadarSepStruct& R) const
		{
			return (nSep_m>R.nSep_m);
		}

};


//---------------------------------------------------------------------------
class TSysAltRadarForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *TrackLB;
	TGroupBox *CurrentGrp;
	TEdit *SourceEd;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *TrackEd;
	TEdit *SpeedEd;
	TEdit *CourseEd;
	TLabel *Label9;
	TLabel *Label10;
	TButton *ReplaceBut;
	TBitBtn *CancelBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TrackLBDblClick(TObject *Sender);
	void __fastcall ReplaceButClick(TObject *Sender);
private:	// User declarations

		JList<RadarSepStruct> AltList;

public:		// User declarations
	__fastcall TSysAltRadarForm(TComponent* Owner);

	Cst4_Radar_Track Radar;

	void SetAlt(JList<RadarSepStruct> List)
	{
		AltList=List;
	}




};
//---------------------------------------------------------------------------
extern PACKAGE TSysAltRadarForm *SysAltRadarForm;
//---------------------------------------------------------------------------
#endif
