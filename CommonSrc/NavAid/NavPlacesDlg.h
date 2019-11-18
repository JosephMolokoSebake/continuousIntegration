//---------------------------------------------------------------------------

#ifndef NavPlacesDlgH
#define NavPlacesDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "JEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include "NavAid.h"
#include "JList.h"

//---------------------------------------------------------------------------
class TNavPlacesForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox2;
   TListBox *SourceList;
   TButton *SourceBut;
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TEdit *ProvEd;
   TEdit *FeatureEd;
   TEdit *LatEd;
   TEdit *LongEd;
   TButton *SelectBut;
   TGroupBox *GroupBox5;
   TListBox *ConvertList;
   TButton *RemoveBut;
   TBevel *Bevel2;
   TButton *EditBut;
   TButton *CreateBut;
   TBitBtn *CloseBut;
   TGroupBox *GroupBox3;
   TLabel *Label7;
   TEdit *FilterLatEd;
   TEdit *FilterLongEd;
   TBevel *Bevel1;
   TComboBox *ProvCombo;
   TLabel *Label4;
   TLabel *Label5;
   TComboBox *FeatureCombo;
   TEdit *FilterDistEd;
   TLabel *Label9;
   TEdit *SkippedEd;
   TLabel *Label10;
   TCheckBox *FilterChk;
   TBevel *Bevel3;
   TBitBtn *ExportBut;
   TEdit *JumpNameEd;
   TLabel *Label6;
   TBevel *Bevel4;
   TEdit *ChartEd;
   TButton *ChartBut;
   TCheckBox *ChartChk;
   TLabel *Label11;
   TEdit *ListedEd;
   TButton *SelectAllBut;
   TEdit *ConvertEd;
   TBitBtn *AbortBut;
   TBevel *Bevel5;
   TCheckBox *AreaChk;
   TEdit *TLLatEd;
   TEdit *TLLongEd;
   TEdit *BRLatEd;
   TEdit *BRLongEd;
   TLabel *Label8;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label14;
   TEdit *ChartDistEd;
   TLabel *Label15;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall SourceButClick(TObject *Sender);
   void __fastcall JumpNameEdChange(TObject *Sender);
   void __fastcall SelectButClick(TObject *Sender);
   void __fastcall SourceListDblClick(TObject *Sender);
   void __fastcall SourceListClick(TObject *Sender);
   void __fastcall RemoveButClick(TObject *Sender);
   void __fastcall EditButClick(TObject *Sender);
   void __fastcall CreateButClick(TObject *Sender);
   void __fastcall ConvertListClick(TObject *Sender);
   void __fastcall ProvComboChange(TObject *Sender);
   void __fastcall FeatureComboChange(TObject *Sender);
   void __fastcall FilterChkClick(TObject *Sender);
   void __fastcall FilterLatEdChange(TObject *Sender);
   void __fastcall FilterLongEdChange(TObject *Sender);
   void __fastcall FilterDistEdChange(TObject *Sender);
   void __fastcall ExportButClick(TObject *Sender);
   void __fastcall ChartButClick(TObject *Sender);
   void __fastcall ChartChkClick(TObject *Sender);
   void __fastcall ConvertListDblClick(TObject *Sender);
   void __fastcall SelectAllButClick(TObject *Sender);
   void __fastcall AbortButClick(TObject *Sender);
   void __fastcall AreaChkClick(TObject *Sender);
   void __fastcall TLLatEdChange(TObject *Sender);
   void __fastcall TLLongEdChange(TObject *Sender);
   void __fastcall BRLatEdChange(TObject *Sender);
   void __fastcall BRLongEdChange(TObject *Sender);
private:	// User declarations

   JEdit Ed;

   struct PlaceNameStruct
   {
      String sName;
      String sProvince;
      String sFeature;
      LatLong Pos;

      PlaceNameStruct()
      {
         sName=L"";
         sProvince=L"";
         sFeature=L"";
         Pos=LatLong(0.0,0.0);
      }

      PlaceNameStruct& operator = (const PlaceNameStruct& P)
      {
         sName=P.sName;
         sProvince=P.sProvince;
         sFeature=P.sFeature;
         Pos=P.Pos;
         return *this;
      }

   };

   JList<PlaceNameStruct> Places;

   bool bReading;

   SymbolStruct DefaultSymbol;
   NavAidClass Nav;

   LatLong* pChartLL;
   int nChartSize;

   bool bAbort;
   bool bBusy;

   bool __fastcall bGetLL(const String sLL, double& dLL);

   void __fastcall ClearSourceList();
   void __fastcall AddSourceList(const PlaceNameStruct& Place);
   void __fastcall UpdateSourceList();
   bool __fastcall bValidLL(const LatLong& LL, const LatLong& Pos, const double dMaxDist);
   String __fastcall sSetLL(const LatLong& LL);


public:		// User declarations
   __fastcall TNavPlacesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNavPlacesForm *NavPlacesForm;
//---------------------------------------------------------------------------
#endif
