//---------------------------------------------------------------------------

#ifndef Cst4SysAltAISDlgH
#define Cst4SysAltAISDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "JEdit.h"
#include <Vcl.Buttons.hpp>

#include "Cst4Messages.h"

struct AISSepStruct
{
	Cst4_AIS_Dynamic D;
	Cst4_AIS_Static S;
	int nSep_m;

	AISSepStruct()
	{
		D.Invalidate();
		S.Invalidate();
		nSep_m=-1;
	}

	AISSepStruct& operator = (const AISSepStruct& T)
	{
		D=T.D;
		S=T.S;
		nSep_m=T.nSep_m;
		return *this;
	}

	~AISSepStruct()
	{
	}

		bool operator < (const AISSepStruct& T) const
		{
			return (nSep_m<T.nSep_m);
		}

		bool operator > (const AISSepStruct& T) const
		{
			return (nSep_m>T.nSep_m);
		}

};


//---------------------------------------------------------------------------
class TSysAltAISForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *TrackLB;
	TGroupBox *GroupBox1;
	TEdit *CNameEd;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *CMMSIEd;
	TEdit *CSpeedEd;
	TEdit *CCourseEd;
	TLabel *Label9;
	TLabel *Label10;
	TBitBtn *CancelBut;
	TButton *ReplaceBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TrackLBDblClick(TObject *Sender);
	void __fastcall ReplaceButClick(TObject *Sender);
	void __fastcall InfoButClick(TObject *Sender);
private:	// User declarations
      JEdit Ed;

		JList<AISSepStruct> AltList;

public:		// User declarations
	__fastcall TSysAltAISForm(TComponent* Owner);

	Cst4_SysTrack::AISStruct AIS;

	void SetAlt(JList<AISSepStruct> List)
	{
		AltList=List;
	}



};
//---------------------------------------------------------------------------
extern PACKAGE TSysAltAISForm *SysAltAISForm;
//---------------------------------------------------------------------------
#endif
