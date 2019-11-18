//---------------------------------------------------------------------------

#ifndef NavLineImportDlgH
#define NavLineImportDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#include "JEdit.h"
#include "JList.h"
#include "JTransform.h"

//---------------------------------------------------------------------------
class TNavLineImportForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TEdit *LatEd;
   TEdit *LongEd;
   TLabel *LatLab;
   TLabel *LongLab;
   TGroupBox *GroupBox2;
   TRadioButton *LLRad;
   TRadioButton *RaBeRad;
   TRadioButton *BeRaRad;
   TGroupBox *GroupBox3;
   TRadioButton *SpaceRad;
   TRadioButton *CommaRad;
   TRadioButton *TabRad;
   TEdit *SourceEd;
   TButton *SourceBut;
   TLabel *FileLab;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TGroupBox *GroupBox4;
   TRadioButton *mRad;
   TRadioButton *kmRad;
   void __fastcall SourceButClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall LLRadClick(TObject *Sender);
   void __fastcall RaBeRadClick(TObject *Sender);
   void __fastcall BeRaRadClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations

   JEdit Ed;

	String sGet(const String s);

public:		// User declarations
   __fastcall TNavLineImportForm(TComponent* Owner);

   JList<LatLong> Seg; 

};
//---------------------------------------------------------------------------
extern PACKAGE TNavLineImportForm *NavLineImportForm;
//---------------------------------------------------------------------------
#endif
