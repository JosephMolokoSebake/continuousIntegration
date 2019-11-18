//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "D2DNavArcDlg.h"
#include "JColorDlg.h"
#include "NavArcSegDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavArcForm *NavArcForm;
//---------------------------------------------------------------------------
__fastcall TNavArcForm::TNavArcForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FormCreate(TObject *)
{
   pLock=new TCriticalSection();
   bSubEdit=true;
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FormShow(TObject *)
{
   bDeleted=false;
   FillChkClick(0);
   FramePan->Enabled=bSubEdit;
   OKBut->SetFocus();
   if (SegRad->Checked)
      SegRadClick(0);
   else if (ArcRad->Checked)
      ArcRadClick(0);
   else if (CircleRad->Checked)
      CircleRadClick(0);
   else
      RoseRadClick(0);
   DelBut->Enabled=bShowDel;
   SegList->Clear();
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
		g_Dict.Translate(Label13);
		g_Dict.Translate(DispChk);
		g_Dict.Translate(DelBut);

		g_Dict.Translate(GroupBox4);
		g_Dict.Translate(ArcRad);
		g_Dict.Translate(CircleRad);
		g_Dict.Translate(SegRad);

		g_Dict.Translate(FirePosGrp);
		g_Dict.Translate(ArcLab1);
		g_Dict.Translate(ArcLab2);

		g_Dict.Translate(GroupBox2);
		g_Dict.Translate(ArcLab3);
		g_Dict.Translate(ArcLab4);
		g_Dict.Translate(ArcLab5);
		g_Dict.Translate(ArcLab6);
		g_Dict.Translate(ArcLab7);
		g_Dict.Translate(ArcLab8);
		g_Dict.Translate(ArcLab9);
		g_Dict.Translate(ArcLab10);

		g_Dict.Translate(GroupBox5);
		g_Dict.Translate(AddSegBut);
		g_Dict.Translate(EditSegBut);
		g_Dict.Translate(DelSegBut);

		g_Dict.Translate(CircleGrp);
		g_Dict.Translate(CircleLab1);
		g_Dict.Translate(CircleLab2);
		g_Dict.Translate(CircleDotChk);

		g_Dict.Translate(GroupBox3);
		g_Dict.Translate(Label11);
		g_Dict.Translate(Label12);
		g_Dict.Translate(FillChk);
		g_Dict.Translate(FillLab);

	#endif

	pLock->Acquire();
	DlgState=NM_DIALOG_OPEN;
	pLock->Release();
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FormClose(TObject *,
      TCloseAction &Action)
{
   if (Action==Forms::caHide)
		{
		pLock->Acquire();
		DlgState=NM_DIALOG_CANCEL;
		pLock->Release();
		}
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FormDestroy(TObject *)
{
	delete pLock;
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FramePanClick(TObject *)
{
	#ifdef _CST4_DICT
		FramePan->Color=SelectColor(g_Dict.sTranslate("Frame Colour"),FramePan->Color,BackColor,pPrintColor);
	#else
		FramePan->Color=SelectColor("Frame Colour",FramePan->Color,BackColor,pPrintColor);
	#endif
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FillChkClick(TObject *)
{
	FillLab->Enabled=FillChk->Checked;
	FillPan->Enabled=FillChk->Checked && bSubEdit;
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::FillPanClick(TObject *)
{
	#ifdef _CST4_DICT
		FillPan->Color=SelectColor(g_Dict.sTranslate("Frame Fill Colour"),FillPan->Color,BackColor,pPrintColor);
	#else
		FillPan->Color=SelectColor("Frame Fill Colour",FillPan->Color,BackColor,pPrintColor);
	#endif
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::DelButClick(TObject *)
{
	if (bSubEdit)
		{
	#ifdef _CST4_DICT
		if (bAskYes(g_Dict.sTranslate("Delete Footprint ?"),g_Dict.sTranslate("Footprint")))
	#else
		if (bAskYes("Delete Footprint ?","Footprint"))
	#endif
			{
			bDeleted=true;
			ModalResult=mrOk;
			}
		}
	else
		{
		pLock->Acquire();
		DlgState=NM_DIALOG_OK;
		pLock->Release();
		bDeleted=true;
		ModalResult=mrOk;
		}
}
//---------------------------------------------------------------------------
void __fastcall TNavArcForm::OKButClick(TObject *)
{
	DlgState=NM_DIALOG_OK;
	pLock->Acquire();
	ModalResult=mrOk;
	pLock->Release();
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::CancelButClick(TObject *)
{
	DlgState=NM_DIALOG_CANCEL;
	pLock->Acquire();
	ModalResult=mrCancel;
	pLock->Release();
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::ArcRadClick(TObject *)
{
	bool bArc=true;
	LatEd->Enabled=bArc;
	LongEd->Enabled=bArc;
	FRaEd->Enabled=bArc;
	ARaEd->Enabled=bArc;
	BeEd->Enabled=bArc;
	ArcEd->Enabled=bArc;
	ArcLab1->Enabled=bArc;
	ArcLab2->Enabled=bArc;
	ArcLab3->Enabled=bArc;
	ArcLab4->Enabled=bArc;
	ArcLab5->Enabled=bArc;
	ArcLab6->Enabled=bArc;
	ArcLab7->Enabled=bArc;
	ArcLab8->Enabled=bArc;
	ArcLab9->Enabled=bArc;
	ArcLab10->Enabled=bArc;
	SegList->Enabled=!bArc;
	AddSegBut->Enabled=!bArc;
	DelSegBut->Enabled=!bArc;
	UpBut->Enabled=!bArc;
	DownBut->Enabled=!bArc;
	if (bArc)
		SegList->Color=cl3DLight;
	else
		SegList->Color=clWindow;
	CircleLab1->Enabled=false;
	CircleLab2->Enabled=false;
	CircleEd->Enabled=false;
	CircleDotChk->Enabled=false;
	#ifdef _CST4_DICT
	FirePosGrp->Caption=g_Dict.sTranslate("Firing Position");
	#else
	FirePosGrp->Caption="Firing Position";
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::SegRadClick(TObject *)
{
   bool bArc=false;
   LatEd->Enabled=bArc;
   LongEd->Enabled=bArc;
   FRaEd->Enabled=bArc;
   ARaEd->Enabled=bArc;
   BeEd->Enabled=bArc;
   ArcEd->Enabled=bArc;
   ArcLab1->Enabled=bArc;
   ArcLab2->Enabled=bArc;
   ArcLab3->Enabled=bArc;
   ArcLab4->Enabled=bArc;
   ArcLab5->Enabled=bArc;
   ArcLab6->Enabled=bArc;
   ArcLab7->Enabled=bArc;
   ArcLab8->Enabled=bArc;
   ArcLab9->Enabled=bArc;
   ArcLab10->Enabled=bArc;
   SegList->Enabled=!bArc;
   AddSegBut->Enabled=!bArc;
   DelSegBut->Enabled=!bArc;
   UpBut->Enabled=!bArc;
   DownBut->Enabled=!bArc;
   if (bArc)
      SegList->Color=cl3DLight;
   else
      SegList->Color=clWindow;
   CircleLab1->Enabled=false;
   CircleLab2->Enabled=false;
   CircleEd->Enabled=false;
	#ifdef _CST4_DICT
	FirePosGrp->Caption=g_Dict.sTranslate("Firing Position");
	#else
	FirePosGrp->Caption="Firing Position";
	#endif
	CircleDotChk->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::SegListClick(TObject *)
{
	DelSegBut->Enabled=(SegList->ItemIndex>=0);
	UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
	DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::AddSegButClick(TObject *)
{
	NavArcSegForm=new TNavArcSegForm(this);
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
	Seg.Pack();
	delete NavArcSegForm;
	EditSegBut->Enabled=(SegList->ItemIndex>=0);
	DelSegBut->Enabled=(SegList->ItemIndex>=0);
	UpBut->Enabled=(SegList->ItemIndex>0)&&(SegList->Count>0);
	DownBut->Enabled=(SegList->ItemIndex<SegList->Items->Count-1)&&(SegList->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::DelSegButClick(TObject *)
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

void __fastcall TNavArcForm::UpButClick(TObject *)
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

void __fastcall TNavArcForm::DownButClick(TObject *)
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

void __fastcall TNavArcForm::SegListDblClick(TObject *)
{
	EditSegButClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::EditSegButClick(TObject *)
{
	int n=SegList->ItemIndex;
	if (n>=0)
		{
		NavArcSegForm=new TNavArcSegForm(this);
		NavArcSegForm->Ed.nSetLL("Segment",NavArcSegForm->LatEd,NavArcSegForm->LongEd,*Seg[n]);
		if (NavArcSegForm->ShowModal()==mrOk)
			{
			NavArcSegForm->Ed.GetLL(NavArcSegForm->LatEd,NavArcSegForm->LongEd,*Seg[n]);
			SegList->Items->Strings[n]=Seg[n]->sLat()+" "+Seg[n]->sLong();
			}
		delete NavArcSegForm;
		}
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::CircleRadClick(TObject *)
{
	bool bArc=false;
	LatEd->Enabled=true;
	LongEd->Enabled=true;
	FRaEd->Enabled=bArc;
	ARaEd->Enabled=bArc;
	BeEd->Enabled=bArc;
	ArcEd->Enabled=bArc;
	ArcLab1->Enabled=true;
	ArcLab2->Enabled=true;
	ArcLab3->Enabled=bArc;
	ArcLab4->Enabled=bArc;
	ArcLab5->Enabled=bArc;
	ArcLab6->Enabled=bArc;
	ArcLab7->Enabled=bArc;
	ArcLab8->Enabled=bArc;
	ArcLab9->Enabled=bArc;
	ArcLab10->Enabled=bArc;
	SegList->Enabled=bArc;
	AddSegBut->Enabled=bArc;
	DelSegBut->Enabled=bArc;
	UpBut->Enabled=bArc;
	DownBut->Enabled=bArc;
	SegList->Color=cl3DLight;
	CircleLab1->Enabled=true;
	CircleLab2->Enabled=true;
	CircleEd->Enabled=true;
	CircleDotChk->Enabled=true;
	#ifdef _CST4_DICT
	FirePosGrp->Caption=g_Dict.sTranslate("Circle Origin");
	#else
	FirePosGrp->Caption="Circle Origin";
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TNavArcForm::RoseRadClick(TObject *)
{
	bool bArc=false;
	LatEd->Enabled=true;
	LongEd->Enabled=true;
	FRaEd->Enabled=bArc;
	ARaEd->Enabled=bArc;
	BeEd->Enabled=bArc;
	ArcEd->Enabled=bArc;
	ArcLab1->Enabled=true;
	ArcLab2->Enabled=true;
	ArcLab3->Enabled=bArc;
	ArcLab4->Enabled=bArc;
	ArcLab5->Enabled=bArc;
	ArcLab6->Enabled=bArc;
	ArcLab7->Enabled=bArc;
	ArcLab8->Enabled=bArc;
   ArcLab9->Enabled=bArc;
   ArcLab10->Enabled=bArc;
   SegList->Enabled=bArc;
   AddSegBut->Enabled=bArc;
   DelSegBut->Enabled=bArc;
   UpBut->Enabled=bArc;
   DownBut->Enabled=bArc;
   SegList->Color=cl3DLight;
   CircleLab1->Enabled=true;
   CircleLab2->Enabled=true;
   CircleEd->Enabled=true;
   CircleDotChk->Enabled=true;
	#ifdef _CST4_DICT
	FirePosGrp->Caption=g_Dict.sTranslate("Rose Origin");
	#else
	FirePosGrp->Caption="Rose Origin";
	#endif
}
//---------------------------------------------------------------------------

