//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "D2DNavLineImportDlg.h"
#include "JFileDlg.h"
#include "JCommaText.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavLineImportForm *NavLineImportForm;
//---------------------------------------------------------------------------
__fastcall TNavLineImportForm::TNavLineImportForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavLineImportForm::SourceButClick(TObject *)
{
	String sFile=sSelectOpenFile(sGet("Text Files")+" (*.txt)","*.txt",sGet("Select Data File"));
	if (!sFile.IsEmpty())
      {
      SourceEd->Text=sFile;
      OKBut->Enabled=true;
      }
}
//---------------------------------------------------------------------------
void __fastcall TNavLineImportForm::FormShow(TObject *)
{
#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption
	g_Dict.Translate(FileLab);

	g_Dict.Translate(GroupBox1);
	g_Dict.Translate(LatLab);
	g_Dict.Translate(LongLab);

	g_Dict.Translate(GroupBox2);
	g_Dict.Translate(LLRad);
	g_Dict.Translate(RaBeRad);
	g_Dict.Translate(BeRaRad);

	g_Dict.Translate(GroupBox3);
	g_Dict.Translate(SpaceRad);
	g_Dict.Translate(CommaRad);
	g_Dict.Translate(TabRad);

	g_Dict.Translate(GroupBox4);
	g_Dict.Translate(mRad);
	g_Dict.Translate(kmRad);

#endif

	OKBut->Enabled=false;
   CancelBut->SetFocus();
}
//---------------------------------------------------------------------------

String TNavLineImportForm::sGet(const String s)
{
	#ifdef _CST4_DICT
		return g_Dict.sTranslate(s);
	#else
		return s;
	#endif
}

void __fastcall TNavLineImportForm::LLRadClick(TObject *)
{
   LatEd->Enabled=false;
   LongEd->Enabled=false;
   LatLab->Enabled=false;
   LongLab->Enabled=false;
   mRad->Enabled=false;
   kmRad->Enabled=false;
   LatEd->Color=cl3DLight;
   LongEd->Color=cl3DLight;
	FileLab->Caption=sGet("Source File Name (Latitude - Longitude)");
}
//---------------------------------------------------------------------------

void __fastcall TNavLineImportForm::RaBeRadClick(TObject *)
{
	LatEd->Enabled=true;
	LongEd->Enabled=true;
	LatLab->Enabled=true;
	LongLab->Enabled=true;
	mRad->Enabled=true;
	kmRad->Enabled=true;
	LatEd->Color=clWindow;
	LongEd->Color=clWindow;
	FileLab->Caption=sGet("Source File Name (Range - Bearing)");
}
//---------------------------------------------------------------------------

void __fastcall TNavLineImportForm::BeRaRadClick(TObject *)
{
	LatEd->Enabled=true;
	LongEd->Enabled=true;
	LatLab->Enabled=true;
	LongLab->Enabled=true;
	mRad->Enabled=true;
	kmRad->Enabled=true;
	LatEd->Color=clWindow;
	LongEd->Color=clWindow;
	FileLab->Caption=sGet("Source File Name (Bearing - Range)");
}
//---------------------------------------------------------------------------


void __fastcall TNavLineImportForm::FormDestroy(TObject *)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNavLineImportForm::FormCreate(TObject *)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TNavLineImportForm::FormClose(TObject *,
      TCloseAction &/*Action*/)
{
   if (ModalResult==mrOk)
      {
      Screen->Cursor=crHourGlass;
      Application->ProcessMessages();


      LatLong LL=Ed.GetLL(LatEd,LongEd);
      JFile Fil('I',JFile::ASCII_TYPE);
      JFile::FILE_ERROR E=Fil.Open(SourceEd->Text);
      if (E)
         Fil.ShowError(E);
      else
         {
         Transform Trans;
         Trans.SetLLRef(LL);
         Trans.SetLLView(LL,SizeD(1.0,1.0));
         Trans.SetRadarPos(LL,0.0);
         while(!E)
            {
            char szSt[256];
            E=Fil.Read(szSt,256);
            if (!E)
               {
               double dx,dy;
               bool bValid;
               if ((SpaceRad->Checked)||(TabRad->Checked))
                  {
                  int n=sscanf(szSt,"%lf%lf",&dx,&dy);
                  bValid=(n==2);
                  }
               else
                  {
                  bValid;
                  try
                     {
                     CommaText Txt(szSt);
                     bValid=(Txt.nGetNum()==2);
                     dx=Txt.dGetVal(0);
                     dy=Txt.dGetVal(1);
                     }
                  catch(...)
                     {
                     bValid=false;
                     }
                  }
               if (bValid)
                  {
                  LatLong* pLL=new LatLong;
                  if (LLRad->Checked)
                     {
                     pLL->dLat=dx;
                     pLL->dLong=dy;
                     }
                  else if (RaBeRad->Checked)
                     {
                     if (kmRad->Checked) dx*=1000.0;
                     *pLL=Trans.Pol2LL(Polar(dx,dy));
                     }
                  else
                     {
                     if (kmRad->Checked) dy*=1000.0;
                     *pLL=Trans.Pol2LL(Polar(dy,dx));
                     }
                  Seg.nAdd(pLL);
                  }
               else
                  {
                  E=JFile::F_FORMAT_ERROR;
                  Fil.ShowError(E);
                  }
               }
            }
         }
      Seg.Pack();
      Screen->Cursor=crDefault;
      Application->ProcessMessages();
      }
}
//---------------------------------------------------------------------------

