/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General Navigational Aid Line Setup Form
 *    FILE NAME		NavLineDialog.cpp
 *		PROJECT
 *		ACTIVITY
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 03/12/2001
 ************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "NavLineDlg.h"
#include "NavArcSegDlg.h"
#include "JFile.h"
#include "JUtils.h"
#include "JColorDlg.h"
#include "NavLineImportDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavLineForm *NavLineForm;
//---------------------------------------------------------------------------
__fastcall TNavLineForm::TNavLineForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::FormCreate(TObject *)
{
   pLock=new TCriticalSection();
   bSubEdit=true;
   bShowDel=true;
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::FormShow(TObject *)
{
   StyleCombo->Items->Clear();
   StyleCombo->Items->Add(L"Solid");
   StyleCombo->Items->Add(L"Dash");
   StyleCombo->Items->Add(L"Dot");
   StyleCombo->Items->Add(L"DashDot");
   StyleCombo->Items->Add(L"DashDotDot");
   StyleCombo->ItemIndex=nStyle;
   abPosChanged[0]=false;
   abPosChanged[1]=false;
   bDeleted=false;
   Arrow1ChkClick(0);
   Arrow2ChkClick(0);
   Lat1Ed->Enabled=!bDefault;
   Long1Ed->Enabled=!bDefault;
   Lat1Lab->Enabled=!bDefault;
   Long1Lab->Enabled=!bDefault;
   Lat2Ed->Enabled=!bDefault;
   Long2Ed->Enabled=!bDefault;
   Lat2Lab->Enabled=!bDefault;
   Long2Lab->Enabled=!bDefault;
   DeleteBut->Enabled=!bDefault && bShowDel;
   if (bDefault)
      {
      LockChk->Checked=false;
      LockChk->Enabled=false;
      Lat1Ed->Text=L"";
      Long1Ed->Text=L"";
      Lat1Ed->Color=cl3DLight;
      Long1Ed->Color=cl3DLight;
      Lat2Ed->Text=L"";
      Long2Ed->Text=L"";
      Lat2Ed->Color=cl3DLight;
      Long2Ed->Color=cl3DLight;
      DispChk->Checked=true;
      DispChk->Enabled=false;
      }
   else
      {
      Lat1Ed->Color=clWindow;
      Long1Ed->Color=clWindow;
      Lat2Ed->Color=clWindow;
      Long2Ed->Color=clWindow;
      DispChk->Enabled=true;
      }
   if (nSide==1)
      Arrow1Chk->SetFocus();
   else if (nSide==2)
      Arrow2Chk->SetFocus();
   else
      OKBut->SetFocus();
   ColorPan->Enabled=bSubEdit;
   if (SingleRad->Checked)
      SingleRadClick(0);
   else
      MultiRadClick(0);
   SegList->Clear();
   Seg.Pack();
   if (Seg.nGetCount()>0)
      {
      for (int i=0; i<Seg.nGetCount(); i++)
         SegList->Items->Add(Seg[i]->sLat()+" "+Seg[i]->sLong());
      SegList->ItemIndex=0;
      }
   else
      SegList->ItemIndex=-1;
   EditSegBut->Enabled=(SegList->ItemIndex>=0);
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);

#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption
	g_Dict.Translate(DispChk);
	g_Dict.Translate(LockChk);
	g_Dict.Translate(DeleteBut);

	g_Dict.Translate(GroupBox4);
	g_Dict.Translate(SingleRad);
	g_Dict.Translate(MultiRad);

	g_Dict.Translate(GroupBox2);
	g_Dict.Translate(Label4);
	g_Dict.Translate(Label5);
	g_Dict.Translate(StyleCombo);

	g_Dict.Translate(GroupBox1);
	g_Dict.Translate(Lat1Lab);
	g_Dict.Translate(Long1Lab);
	g_Dict.Translate(Arrow1Chk);
	g_Dict.Translate(Head1Lab);

	g_Dict.Translate(GroupBox3);
	g_Dict.Translate(Lat2Lab);
	g_Dict.Translate(Long2Lab);
	g_Dict.Translate(Arrow2Chk);
	g_Dict.Translate(Head2Lab);

	g_Dict.Translate(GroupBox5);
	g_Dict.Translate(AddSegBut);
	g_Dict.Translate(DelSegBut);
	g_Dict.Translate(EditSegBut);
	g_Dict.Translate(ImportSegBut);

#endif

	pLock->Acquire();
   DlgState=NM_DIALOG_OPEN;
   pLock->Release();
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::FormClose(TObject *, TCloseAction &Action)
{
   if (Action==Forms::caHide)
      {
      pLock->Acquire();
      DlgState=NM_DIALOG_CANCEL;
      pLock->Release();
      }
}
//---------------------------------------------------------------------------

String TNavLineForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}


void __fastcall TNavLineForm::Arrow1ChkClick(TObject *)
{
   Head1Lab->Enabled=Arrow1Chk->Checked || bDefault;
   Head1Ed->Enabled=Arrow1Chk->Checked || bDefault;
   Head1UD->Enabled=Arrow1Chk->Checked || bDefault;
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::Arrow2ChkClick(TObject *)
{
   Head2Lab->Enabled=Arrow2Chk->Checked || bDefault;
   Head2Ed->Enabled=Arrow2Chk->Checked || bDefault;
   Head2UD->Enabled=Arrow2Chk->Checked || bDefault;
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::ColorPanClick(TObject *)
{
   ColorPan->Color=SelectColor(sGet("Line Colour"),ColorPan->Color,BackColor,pPrintColor);
}
//---------------------------------------------------------------------------
void __fastcall TNavLineForm::DeleteButClick(TObject *)
{
   if (bSubEdit)
      {
      if (bAskYes(sGet("Delete Line")+" ?",sGet("Line")))
         {
         bDeleted=true;
         ModalResult=mrOk;
         }
      }
   else
      {
      bDeleted=true;
      pLock->Acquire();
      DlgState=NM_DIALOG_OK;
      pLock->Release();
      ModalResult=mrOk;
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::WidthEdChange(TObject *)
{
   if (WidthUD->Position==1) StyleCombo->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::StyleComboChange(TObject *)
{
   if (StyleCombo->ItemIndex>0) WidthUD->Position=1;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::Lat1EdClick(TObject *)
{
   abPosChanged[0]=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::Long1EdClick(TObject *)
{
   abPosChanged[0]=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::Lat2EdChange(TObject *)
{
   abPosChanged[1]=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::Long2EdChange(TObject *)
{
   abPosChanged[1]=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::OKButClick(TObject *)
{
   pLock->Acquire();
   DlgState=NM_DIALOG_OK;
   pLock->Release();
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::CancelButClick(TObject *)
{
   pLock->Acquire();
   DlgState=NM_DIALOG_CANCEL;
   pLock->Release();
   ModalResult=mrCancel;
}
//---------------------------------------------------------------------------


void __fastcall TNavLineForm::FormDestroy(TObject *)
{
   delete pLock;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::SingleRadClick(TObject *)
{
   bool bEn=true;
   Lat1Lab->Enabled=bEn;
   Long1Lab->Enabled=bEn;
   Lat1Ed->Enabled=bEn;
   Long1Ed->Enabled=bEn;
   Arrow1Chk->Enabled=bEn;
   Head1Ed->Enabled=bEn && (Arrow1Chk->Checked || bDefault);
   Head1Lab->Enabled=bEn && (Arrow1Chk->Checked || bDefault);
   Head1UD->Enabled=bEn && (Arrow1Chk->Checked || bDefault);
   Lat2Lab->Enabled=bEn;
   Long2Lab->Enabled=bEn;
   Lat2Ed->Enabled=bEn;
   Long2Ed->Enabled=bEn;
   Arrow2Chk->Enabled=bEn;
   Head2Ed->Enabled=bEn && (Arrow2Chk->Checked || bDefault);
   Head2Lab->Enabled=bEn && (Arrow2Chk->Checked || bDefault);
   Head2UD->Enabled=bEn && (Arrow2Chk->Checked || bDefault);
   bEn=false;
   SegList->Color=cl3DLight;
   SegList->Enabled=bEn;
   AddSegBut->Enabled=bEn;
   EditSegBut->Enabled=bEn;
   DelSegBut->Enabled=bEn;
   ImportSegBut->Enabled=bEn;
   UpBut->Enabled=bEn;
   DownBut->Enabled=bEn;
   if (!bDefault)
      LockChk->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::MultiRadClick(TObject *)
{
   bool bEn=false;
   Lat1Lab->Enabled=bEn;
   Long1Lab->Enabled=bEn;
   Lat1Ed->Enabled=bEn;
   Long1Ed->Enabled=bEn;
   Arrow1Chk->Enabled=bEn;
   Head1Ed->Enabled=bEn;
   Head1Lab->Enabled=bEn;
   Head1UD->Enabled=bEn;
   Lat2Lab->Enabled=bEn;
   Long2Lab->Enabled=bEn;
   Lat2Ed->Enabled=bEn;
   Long2Ed->Enabled=bEn;
   Arrow2Chk->Enabled=bEn;
   Head2Ed->Enabled=bEn;
   Head2Lab->Enabled=bEn;
   Head2UD->Enabled=bEn;
   bEn=true;
   SegList->Color=clWindow;
   SegList->Enabled=bEn;
   AddSegBut->Enabled=bEn;
   EditSegBut->Enabled=bEn;
   DelSegBut->Enabled=bEn;
   ImportSegBut->Enabled=bEn;
   UpBut->Enabled=bEn;
   DownBut->Enabled=bEn;
   LockChk->Checked=false;
   LockChk->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::AddSegButClick(TObject *)
{
   NavArcSegForm=new TNavArcSegForm(this);
   NavArcSegForm->Caption=L"Line Segment";
   if (NavArcSegForm->ShowModal()==mrOk)
      {
      LatLong* pLL=new LatLong;
      NavArcSegForm->Ed.GetLL(NavArcSegForm->LatEd,NavArcSegForm->LongEd,*pLL);
      Seg.nAdd(pLL);
      SegList->Items->Add(pLL->sLat()+" "+pLL->sLong());
      SegList->ItemIndex=SegList->Items->Count-1;
      DelSegBut->Enabled=(SegList->ItemIndex>=0);
      UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
      DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
      }
   delete NavArcSegForm;
   EditSegBut->Enabled=(SegList->ItemIndex>=0);
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::EditSegButClick(TObject *)
{
   int n=SegList->ItemIndex;
   if (n>=0)
      {
      Seg.Pack();
      NavArcSegForm=new TNavArcSegForm(this);
      NavArcSegForm->Ed.nSetLL("User Shape Point",NavArcSegForm->LatEd,NavArcSegForm->LongEd,*Seg[n]);
      if (NavArcSegForm->ShowModal()==mrOk)
         {
         NavArcSegForm->Ed.GetLL(NavArcSegForm->LatEd,NavArcSegForm->LongEd,*Seg[n]);
         SegList->Items->Strings[n]=Seg[n]->sLat()+" "+Seg[n]->sLong();
         }
      delete NavArcSegForm;
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::DelSegButClick(TObject *)
{
   int n=SegList->ItemIndex;
   if (n>=0)
      {
      Seg.Delete(n);
      SegList->Items->Delete(n);
      if (Seg.nGetCount()>0)
         {
         if (n<Seg.nGetCount())
            SegList->ItemIndex=n;
         else
            SegList->ItemIndex=0;
         }
      else
         SegList->ItemIndex=-1;
      }
   Seg.Pack();
   EditSegBut->Enabled=(SegList->ItemIndex>=0);
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::UpButClick(TObject *)
{
   int n=SegList->ItemIndex;
   if (n>0)
      {
      Seg.Exchange(n,n-1);
      SegList->Items->Exchange(n,n-1);
      }
   EditSegBut->Enabled=(SegList->ItemIndex>=0);
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::DownButClick(TObject *)
{
   int n=SegList->ItemIndex;
   if (n<Seg.nGetCount()-1)
      {
      Seg.Exchange(n,n+1);
      SegList->Items->Exchange(n,n+1);
      }
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   EditSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::ImportSegButClick(TObject *)
{
   NavLineImportForm=new TNavLineImportForm(this);
   if (NavLineImportForm->ShowModal()==mrOk)
      {
      SegList->Clear();
      Seg.Clear();
      NavLineImportForm->Seg.Pack();
      for (int i=0; i<NavLineImportForm->Seg.nGetCount(); i++)
         {
         LatLong* pLL=new LatLong;
         *pLL=*NavLineImportForm->Seg[i];
         Seg.nAdd(pLL);
         SegList->Items->Add(pLL->sLat()+" "+pLL->sLong());
         }
      Seg.Pack();
      }
   delete NavLineImportForm;
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::SegListClick(TObject *)
{
   DelSegBut->Enabled=(SegList->ItemIndex>=0);
   UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
   DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavLineForm::SegListDblClick(TObject *)
{
   EditSegButClick(0);
}
//---------------------------------------------------------------------------

