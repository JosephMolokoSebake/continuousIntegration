/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE   Colour Selection Dialog
 *		PROJECT			-
 *		ACTIVITY       -
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows Win2000
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *    	Colour Selection Dialog
 *		CHANGE HISTORY
************************************************************************/
//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JColorDlg.h"
#include "JUtils.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TColorForm *ColorForm;
//---------------------------------------------------------------------------
__fastcall TColorForm::TColorForm(TComponent* Owner)
   : TForm(Owner)
{
   Color=clWhite;
   BackColor=clBlack;
	bShowPrint=false;
}
//---------------------------------------------------------------------------
void __fastcall TColorForm::FormShow(TObject *)
{
   Ed.Init();
   DWORD adwCol[48]={0x008080FF,
                     0x0080FFFF,
                     0x0080FF80,
                     0x0080FF00,
                     0x00FFFF80,
                     0x00FF8000,
                     0x00C080FF,
                     0x00FF80FF,
                     0x000000FF,
                     0x0000FFFF,
                     0x0000FF80,
                     0x0040FF00,
                     0x00FFFF00,
                     0x00C08000,
                     0x00C08080,
                     0x00FF00FF,
                     0x00404080,
                     0x004080FF,
                     0x0000FF00,
                     0x00808000,
                     0x00804000,
                     0x00FF8080,
                     0x00400080,
                     0x008000FF,
                     0x00000080,
                     0x000080FF,
                     0x00008000,
                     0x00408000,
                     0x00FF0000,
                     0x00A00000,
                     0x00800080,
                     0x00FF0080,
                     0x00000040,
							0x00004080,
                     0x00004000,
                     0x00404000,
                     0x00800000,
                     0x00400000,
                     0x00400040,
                     0x00800040,
                     0x00000000,
                     0x00008080,
                     0x00408080,
                     0x00808080,
                     0x00808040,
                     0x00C0C0C0,
                     0x00400040,
                     0x00FFFFFF};

	Panel1->Color=BackColor;
	nPrevSel=-1;
	nSelected=-1;
	bSettingTB=true;
	for (int i=0; i<48; i++)
		{
		apPan[i]=new TPaintBox(this);
		apPan[i]->Parent=Panel1;
		apPan[i]->Color=(TColor)adwCol[i];
		if (apPan[i]->Color==Color)
			nSelected=i;
		apPan[i]->Cursor=crHandPoint;
		int nx=(i%6)*45+10;
		int ny=(i/6)*25+10;
		apPan[i]->SetBounds(nx,ny,40,20);
		apPan[i]->OnClick=OnPanClick;
		apPan[i]->OnDblClick=OnPanDblClick;
		}
	PaintBox->Repaint();
	UserPan->Color=Color;
	UserChk->Checked=(nSelected==-1);
	UserChkClick(0);
	if (!UserChk->Checked)
		ShowSel(nSelected);
   if (!bShowPrint)
      {
      PrintLab->Hide();
      PrintPan->Hide();
      }
	OKBut->SetFocus();

#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption
	g_Dict.Translate(PrintLab);
	g_Dict.Translate(UserChk);
#endif

}

//---------------------------------------------------------------------------
void __fastcall TColorForm::FormClose(TObject *, TCloseAction &)
{
   if (ModalResult==mrOk)
		{
		if (!UserChk->Checked)
			Color=apPan[nSelected]->Color;
		if (bShowPrint)
			{
			}
      }
}
//---------------------------------------------------------------------------

void __fastcall TColorForm::ShowSel(const int nSel)
{
	PaintBox->Canvas->Pen->Width=1;
	PaintBox->Canvas->Pen->Style=psSolid;
	PaintBox->Canvas->Brush->Style=bsClear;
	int nx,ny;
	if (nPrevSel>-1)
		{
		PaintBox->Canvas->Pen->Color=BackColor;
		nx=(nPrevSel%6)*45+7;
		ny=(nPrevSel/6)*25+7;
		PaintBox->Canvas->Rectangle(nx,ny,nx+44,ny+24);
		PaintBox->Canvas->Rectangle(nx-1,ny-1,nx+44+1,ny+24+1);
		}
	nPrevSel=nSel;

	if (nSel>=0)
		{
		nx=(nPrevSel%6)*45+7;
		ny=(nPrevSel/6)*25+7;

		TColor C1,C2;
		if (BackColor==clBtnFace)
			{
			C1=clBlack;
			C2=clWhite;
			}
		else
			{
			C1=(TColor)((~BackColor)&0x00FFFFFF);
			C2=(TColor)((~BackColor)&0x00FFFFFF);
			}
		PaintBox->Canvas->Pen->Color=C1;
		PaintBox->Canvas->Rectangle(nx,ny,nx+44,ny+24);
		PaintBox->Canvas->Pen->Color=C2;
		PaintBox->Canvas->Rectangle(nx-1,ny-1,nx+44+1,ny+24+1);
		DWORD dwCol=apPan[nSel]->Color;
		Ed.SetValRO(RedEd,(int)(dwCol&0x000000FF));
		Ed.SetValRO(GreenEd,(int)((dwCol>>8)&0x000000FF));
		Ed.SetValRO(	BlueEd,(int)((dwCol>>16)&0x000000FF));
		if (bShowPrint)
			PrintPan->Color=aPrintColor[nPrevSel];
		}
}

void __fastcall TColorForm::OnPanClick(TObject* Sender)
{
   int i=0;
   while((i<48)&&(Sender!=apPan[i]))
      {
		i++;
      }
   if (i<48)
      {
		nSelected=i;
		ShowSel(nSelected);
		Color=apPan[i]->Color;
		UserChk->Checked=false;
		UserChkClick(0);
      }
}

void __fastcall TColorForm::OnPanDblClick(TObject* Sender)
{
   int i=0;
   while((i<48)&&(Sender!=apPan[i]))
      {
      i++;
      }
   if (i<48)
      {
      nSelected=i;
		UserChk->Checked=false;
		UserChkClick(0);
		ModalResult=mrOk;
      }
}

void __fastcall TColorForm::PaintBoxPaint(TObject *)
{
   for (int i=0; i<48; i++)
      {
      apPan[i]->Canvas->Pen->Color=apPan[i]->Color;
      apPan[i]->Canvas->Brush->Color=apPan[i]->Color;
      apPan[i]->Canvas->Rectangle(0,0,40,20);
      }
   ShowSel(nSelected);
}
//---------------------------------------------------------------------------


void __fastcall TColorForm::PrintPanClick(TObject *)
{
   TColorForm* pForm=new TColorForm(0);
	pForm->Caption=L"Ecquivalent Print/Metafile Colour";
   pForm->Color=PrintPan->Color;
   TColor Color=clBlack;
   bool bFound=false;
   int n=0;
   while((n<48)&&(!bFound))
      {
      if (BackColor==apPan[n]->Color)
         {
         bFound=true;
         Color=aPrintColor[n];
         }
      else
         n++;
      }
   pForm->BackColor=Color;
   if (pForm->ShowModal()==mrOk)
      {
      PrintPan->Color=pForm->Color;
      aPrintColor[nPrevSel]=pForm->Color;
      }
   delete pForm;
}
//---------------------------------------------------------------------------

TColor SelectColor(  const String sTitle,
                     const TColor Default,
                     const TColor Back,
                     TColor* pPrintCol)
{
   TColor Color=Default;
   TColorForm* pForm=new TColorForm(0);
   pForm->Caption=sTitle;
   pForm->Color=Default;
   pForm->BackColor=Back;
   if (pPrintCol)
      for (int i=0; i<48; i++)
         pForm->MapColor(i,pPrintCol[i]);
	if (pForm->ShowModal()==mrOk)
      {
      Color=pForm->Color;
      if (pPrintCol)
         {
         for (int i=0; i<48; i++)
            pPrintCol[i]=pForm->GetMapColor(i);
         }
      }
   delete pForm;
   return Color;
}

void __fastcall TColorForm::UserChkClick(TObject */*Sender*/)
{
	bSettingTB=true;
	RedTB->Enabled=UserChk->Checked;
	RedLab->Enabled=UserChk->Checked;
	GreenTB->Enabled=UserChk->Checked;
	GreenLab->Enabled=UserChk->Checked;
	BlueTB->Enabled=UserChk->Checked;
	BlueLab->Enabled=UserChk->Checked;
	DWORD dwCol=Color;

	if (UserChk->Checked)
		ShowSel(-1);

	Ed.SetValRO(RedEd,(int)(dwCol&0x000000FF));
	Ed.SetValRO(GreenEd,(int)((dwCol>>8)&0x000000FF));
	Ed.SetValRO(BlueEd,(int)((dwCol>>16)&0x000000FF));

	RedTB->Position=255-(dwCol&0x000000FF);
	GreenTB->Position=255-((dwCol>>8)&0x000000FF);
	BlueTB->Position=255-((dwCol>>16)&0x000000FF);

	UserPan->Color=Color;
	bSettingTB=false;
}
//---------------------------------------------------------------------------

void __fastcall TColorForm::RedTBChange(TObject */*Sender*/)
{
	if (!bSettingTB)
		{
		DWORD dwRed=255-RedTB->Position;
		Ed.SetValRO(RedEd,(int)dwRed);
		DWORD dwGreen=255-GreenTB->Position;
		Ed.SetValRO(GreenEd,(int)dwGreen);
		DWORD dwBlue=255-BlueTB->Position;
		Ed.SetValRO(BlueEd,(int)dwBlue);
		Color=TColor(dwRed|(dwGreen<<8)|(dwBlue<<16));
		UserPan->Color=Color;
		}
}
//---------------------------------------------------------------------------

void __fastcall TColorForm::GreenTBChange(TObject */*Sender*/)
{
	if (!bSettingTB)
		{
		DWORD dwRed=255-RedTB->Position;
		Ed.SetValRO(RedEd,(int)dwRed);
		DWORD dwGreen=255-GreenTB->Position;
		Ed.SetValRO(GreenEd,(int)dwGreen);
		DWORD dwBlue=255-BlueTB->Position;
		Ed.SetValRO(BlueEd,(int)dwBlue);
		Color=TColor(dwRed|(dwGreen<<8)|(dwBlue<<16));
		UserPan->Color=Color;
		}
}
//---------------------------------------------------------------------------

void __fastcall TColorForm::BlueTBChange(TObject */*Sender*/)
{
	if (!bSettingTB)
		{
		DWORD dwRed=255-RedTB->Position;
		Ed.SetValRO(RedEd,(int)dwRed);
		DWORD dwGreen=255-GreenTB->Position;
		Ed.SetValRO(GreenEd,(int)dwGreen);
		DWORD dwBlue=255-BlueTB->Position;
		Ed.SetValRO(BlueEd,(int)dwBlue);
		Color=TColor(dwRed|(dwGreen<<8)|(dwBlue<<16));
		UserPan->Color=Color;
		}
}
//---------------------------------------------------------------------------


