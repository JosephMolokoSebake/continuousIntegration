//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "D2DNavEditDlg.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNavEditForm *NavEditForm;
//---------------------------------------------------------------------------
__fastcall TNavEditForm::TNavEditForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::FormCreate(TObject *)
{
   pLock=new TCriticalSection();
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::FormShow(TObject *)
{

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(Label1);
		g_Dict.Translate(Label2);
		g_Dict.Translate(Label3);
		g_Dict.Translate(EditBut);
		g_Dict.Translate(DelBut);
		g_Dict.Translate(AddSymbolBut);
		g_Dict.Translate(AddLineBut);
		g_Dict.Translate(AddFootBut);
	#endif


   String s;
   SymList->Clear();
   int nS=0;
   int nT=0;
   for (int i=0; i<Nav.nSymbols(); i++)
      {
      SymbolStruct* pS=Nav.pGetSymbol(i);
      if (pS->bDispText)
			s=sGet("Text")+" "+String(++nT)+": "+pS->sGetText();
		else if (pS->bDispSymbol)
			s=sGet("Symbol")+" "+String(++nS);
		else
         s=sGet("Invisible Symbol/Text");
      SymList->Items->Add(s);
      }
   SymList->ItemIndex=-1;
   LineList->Clear();
   for (int i=0; i<Nav.nLines(); i++)
		LineList->Items->Add(sGet("Line")+" "+String(i+1));
	LineList->ItemIndex=-1;
	ArcList->Clear();
	for (int i=0; i<Nav.nArcs(); i++)
		{
		ArcStruct* pA=Nav.pGetArc(i);
		ArcList->Items->Add(pA->sDescr);
		}
	ArcList->ItemIndex=-1;
	SetButtons();
	OKBut->SetFocus();
	pLock->Acquire();
	DlgState=NM_DIALOG_OPEN;
	pLock->Release();
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::FormClose(TObject *,
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
void __fastcall TNavEditForm::FormDestroy(TObject *)
{
	delete pLock;
}

String TNavEditForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}

//---------------------------------------------------------------------------
void __fastcall TNavEditForm::SymListClick(TObject *)
{
	LineList->ItemIndex=-1;
	ArcList->ItemIndex=-1;
	SetButtons();
   SymbolStruct* pS=Nav.pGetSymbol(SymList->ItemIndex);
	String s="["+pS->Pos.sLat()+" "+pS->Pos.sLong()+"]";
	if (pS->bDispText)
		{
		ColPan->Color=pS->pFont->Color;
		}
	else if (pS->bDispSymbol)
		{
		ColPan->Color=pS->SymbolColor;
		s=s+" "+pS->sSymbolType();
		}
	else
		ColPan->Color=clBtnFace;
	DescrEd->Text=s;
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::SymListDblClick(TObject *)
{
	SymListClick(0);
	EditButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::LineListClick(TObject *)
{
	SymList->ItemIndex=-1;
	ArcList->ItemIndex=-1;
	SetButtons();
	LineStruct* pL=Nav.pGetLine(LineList->ItemIndex);
	String s;
	if (pL->bMulti)
		s=sGet("Multi-Segment Line");
	else
		s=L"["+pL->aPos[0].sLat()+" "+pL->aPos[0].sLong()+"] - ["+pL->aPos[1].sLat()+" "+pL->aPos[1].sLong()+"]";
	DescrEd->Text=s;
	ColPan->Color=pL->Color;
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::LineListDblClick(TObject *)
{
   LineListClick(0);
   EditButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::ArcListClick(TObject *)
{
   SymList->ItemIndex=-1;
   LineList->ItemIndex=-1;
   SetButtons();
   ArcStruct* pA=Nav.pGetArc(ArcList->ItemIndex);
   String s;
   s.printf(L"Fwd Range = %d m, Aft Range = %d m",pA->nFwdRa,pA->nAftRa);
   s=L"["+pA->Pos.sLat()+" "+pA->Pos.sLong()+"] "+s;
   DescrEd->Text=s;
   ColPan->Color=pA->FrameColor;
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::ArcListDblClick(TObject *)
{
   ArcListClick(0);
   EditButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::EditButClick(TObject *)
{
   if (SymList->ItemIndex>=0)
      {
      int nS=Nav.nSymbols();
      if (Nav.bEditSymbol(SymList->ItemIndex,false))
         {
         int n=SymList->ItemIndex;
         SymbolStruct* pS=Nav.pGetSymbol(n);
         if (!pS->bDeleted)
            {
            String s;
            if (pS->bDispText)
					s=sGet("Text")+" : "+pS->sGetText();
				else if (pS->bDispSymbol)
					s=sGet("Symbol")+" "+String(++nS);
				else
					s=sGet("Invisible Symbol/Text");
				SymList->Items->Strings[n]=s;
            }
         }
      if (nS!=Nav.nSymbols())
         DeleteSymList();
      SymListClick(0);
      }
   else if (LineList->ItemIndex>=0)
      {
      int nL=Nav.nLines();
      if (Nav.bEditLine(0,LineList->ItemIndex,false))
         {
         int n=LineList->ItemIndex;
         LineStruct* pL=Nav.pGetLine(n);
         if (!pL->bDeleted)
				LineList->Items->Strings[n]=sGet("Line")+" "+String(n+1);
			}
      if (nL!=Nav.nLines())
         DeleteLineList();
      LineListClick(0);
      }
   else if (ArcList->ItemIndex>=0)
      {
      int nA=Nav.nArcs();
      if (Nav.bEditArc(ArcList->ItemIndex,false))
         {
         int n=ArcList->ItemIndex;
         ArcStruct* pA=Nav.pGetArc(n);
         if (!pA->bDeleted)
            ArcList->Items->Strings[n]=pA->sDescr;
         }
      if (nA!=Nav.nArcs())
         DeleteArcList();
      ArcListClick(0);
      }
   SetButtons();
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::UpButClick(TObject *)
{
   if (SymList->ItemIndex>0)
      {
      int n=SymList->ItemIndex;
      SymList->Items->Exchange(n-1,n);
      Nav.ExchangeSymbols(n-1,n);
      SymList->ItemIndex=n-1;
      SymListClick(0);
      }
   else if (LineList->ItemIndex>0)
      {
      int n=LineList->ItemIndex;
      LineList->Items->Exchange(n-1,n);
      Nav.ExchangeLines(n-1,n);
      LineList->ItemIndex=n-1;
      LineListClick(0);
      }
   else if (ArcList->ItemIndex>0)                 
      {
      int n=ArcList->ItemIndex;
      ArcList->Items->Exchange(n-1,n);
      Nav.ExchangeArcs(n-1,n);
      ArcList->ItemIndex=n-1;
      ArcListClick(0);
      }
   SetButtons();
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::DownButClick(TObject *)
{
   if (SymList->ItemIndex>=0)
      {
      int n=SymList->ItemIndex;
      if (n<SymList->Items->Count-1)
         {
         SymList->Items->Exchange(n,n+1);
         Nav.ExchangeSymbols(n,n+1);
         SymList->ItemIndex=n+1;
         SymListClick(0);
         }
      }
   else if (LineList->ItemIndex>=0)
      {
      int n=LineList->ItemIndex;
      if (n<LineList->Items->Count-1)
         {
         LineList->Items->Exchange(n,n+1);
         Nav.ExchangeLines(n,n+1);
         LineList->ItemIndex=n+1;
         LineListClick(0);
         }
      }
   else if (ArcList->ItemIndex>=0)
      {
      int n=ArcList->ItemIndex;
      if (n<ArcList->Items->Count-1)
         {
         ArcList->Items->Exchange(n,n+1);
         Nav.ExchangeArcs(n,n+1);
         ArcList->ItemIndex=n+1;
         ArcListClick(0);
         }
      }
   SetButtons();
}
//---------------------------------------------------------------------------
void __fastcall TNavEditForm::DelButClick(TObject *)
{
   if (SymList->ItemIndex>=0)
      {
      Nav.DeleteSymbol(SymList->ItemIndex);
      DeleteSymList();
      }
   else if (LineList->ItemIndex>=0)
      {
      Nav.DeleteLine(LineList->ItemIndex);
      DeleteLineList();
      }
   else if (ArcList->ItemIndex>=0)
      {
      Nav.DeleteArc(ArcList->ItemIndex);
      DeleteArcList();
      }
   SetButtons();
}
//---------------------------------------------------------------------------

void __fastcall TNavEditForm::SetButtons()
{
   int nS=SymList->ItemIndex;
   int nL=LineList->ItemIndex;
   int nA=ArcList->ItemIndex;
   EditBut->Enabled=(nS>=0)||(nL>=0)||(nA>=0);
   DelBut->Enabled=(nS>=0)||(nL>=0)||(nA>=0);
   UpBut->Enabled=(nS>0)||(nL>0)||(nA>0);
   DownBut->Enabled=    ((nS>=0)&&(nS<SymList->Items->Count-1))
                     || ((nL>=0)&&(nL<LineList->Items->Count-1))
                     || ((nA>=0)&&(nA<ArcList->Items->Count-1));
}

void __fastcall TNavEditForm::DeleteSymList()
{
   int n=SymList->ItemIndex;
   SymList->Items->Delete(n);
   if (SymList->Items->Count==0)
      {
      SymList->ItemIndex=-1;
      DescrEd->Text=L"";
      ColPan->Color=clBtnFace;
      }
   else
      {
      if (n<SymList->Items->Count)
         SymList->ItemIndex=n;
      else
         SymList->ItemIndex=n-1;
      }
}

void __fastcall TNavEditForm::DeleteLineList()
{
   int n=LineList->ItemIndex;
   LineList->Items->Delete(n);
   if (LineList->Items->Count==0)
      {
      LineList->ItemIndex=-1;
      DescrEd->Text=L"";
      ColPan->Color=clBtnFace;
      }
   else
      {
      if (n<LineList->Items->Count)
         LineList->ItemIndex=n;
      else
         LineList->ItemIndex=n-1;
      }
}

void __fastcall TNavEditForm::DeleteArcList()
{
   int n=ArcList->ItemIndex;
   ArcList->Items->Delete(n);
   if (ArcList->Items->Count==0)
      {
      ArcList->ItemIndex=-1;
      DescrEd->Text=L"";
      ColPan->Color=clBtnFace;
      }
   else
      {
      if (n<ArcList->Items->Count)
         ArcList->ItemIndex=n;
      else
         ArcList->ItemIndex=n-1;
      }
}

void __fastcall TNavEditForm::OKButClick(TObject *)
{
   pLock->Acquire();
   DlgState=NM_DIALOG_OK;
   pLock->Release();
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TNavEditForm::CancelButClick(TObject *)
{
   pLock->Acquire();
   DlgState=NM_DIALOG_CANCEL;
   pLock->Release();
   ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TNavEditForm::AddSymbolButClick(TObject *)
{
   SymbolStruct* pSym=new SymbolStruct;
   if (pSym->bEdit(false))
      {
      Nav.Add(pSym);
      String s;
      if (pSym->bDispText)
			s=sGet("Text")+" : "+pSym->sGetText();
		else if (pSym->bDispSymbol)
			s=sGet("Symbol")+" "+String(SymList->Count+1);
		else
         s=sGet("Invisible Symbol/Text");
      SymList->Items->Add(s);
      }
   else
      delete pSym;
}
//---------------------------------------------------------------------------

void __fastcall TNavEditForm::AddLineButClick(TObject *)
{
   LineStruct* pLine=new LineStruct;
   if (pLine->bEdit(false))
      {
      Nav.Add(pLine);
		LineList->Items->Add(sGet("Line")+" "+String(LineList->Count+1));
		}
   else
      delete pLine;
}
//---------------------------------------------------------------------------

void __fastcall TNavEditForm::AddFootButClick(TObject *)
{
   ArcStruct* pArc=new ArcStruct;
   if (pArc->bEdit())
      {
      Nav.Add(pArc);
      ArcList->Items->Add(pArc->sDescr);
      }
   else
      delete pArc;
}
//---------------------------------------------------------------------------




