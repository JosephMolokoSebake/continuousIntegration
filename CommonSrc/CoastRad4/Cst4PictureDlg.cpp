//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4PictureDlg.h"
#include "JFileDlg.h"
#include "Cst4Defs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCst4PictureForm *Cst4PictureForm;
//---------------------------------------------------------------------------
__fastcall TCst4PictureForm::TCst4PictureForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCst4PictureForm::FormCreate(TObject */*Sender*/)
{
	ControlStyle<<csOpaque;          //Prevent erasing of background with repaint
//	PB->ControlStyle<<csOpaque;      //Prevent erasing of background with repaint
	bValid=false;
	pBM=new Graphics::TBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TCst4PictureForm::FormShow(TObject */*Sender*/)
{
	PictList.Sort(true);
	PictList.Pack();
   FileCB->Clear();
   for (int i=0; i<PictList.nGetCount(); i++)
   	{
      JTime T=PictList[i]->T;
      T.ForceConvertToLocal(g_nCst4TimeBias_hours);
   	FileCB->Items->Add(T.sDateTime(false,false,false)+" "+PictList[i]->sDescr);
      }
   if (FileCB->Items->Count>0)
   	{
   	FileCB->ItemIndex=0;
		ShowPicture(0);
      }
   else
   	FileCB->ItemIndex=-1;
}
//---------------------------------------------------------------------------
void __fastcall TCst4PictureForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)

{
//
}
//---------------------------------------------------------------------------
void __fastcall TCst4PictureForm::FormDestroy(TObject */*Sender*/)
{
	delete pBM;
}

void TCst4PictureForm::AddPicture(const String sFile, const String sDescr, const JTime& T)
{
	PictStruct* p=new PictStruct;
   p->sFile=sFile;
   p->sDescr=sDescr;
   p->T=T;
   PictList.nAdd(p);
   PictList.Pack();
}

void TCst4PictureForm::ShowPicture(const int n)
{
	bValid=false;
	bDragging=false;
   bool bOK=true;
	try{
      TJPEGImage *pImage=new TJPEGImage;
      pImage->LoadFromFile(PictList[n]->sFile);
      pBM->Width=pImage->Width;
      pBM->Height=pImage->Height;
      pBM->Assign(pImage);
      delete pImage;
      }
   catch(...)
      {
      bOK=false;
      }
   if (bOK)
   	{
      double dAspect=(pBM->Width*1.0)/pBM->Height;
      ClientHeight=ClientWidth/dAspect+Panel1->Height;
      DrawRect=TRect(0,0,ClientWidth,ClientHeight-Panel1->Height);
      dDrawScale=1.0;
      DrawOffs=TPoint(0,0);
      StartOffs=TPoint(0,0);
      StatusBar->Panels->Items[1]->Text=sFileNameOnly(PictList[n]->sFile);
      bValid=true;
		PB->Repaint();
      }
   else
      ShowError(PictList[n]->sFile,"Error Reading Image File");
}

//---------------------------------------------------------------------------
void __fastcall TCst4PictureForm::FormPaint(TObject */*Sender*/)
{
	if (bValid)
		{
		TRect R;
		R=DrawRect;
		R.Left-=(dDrawScale-1.0)*100;
		R.Top-=(dDrawScale-1.0)*100;
		R.Right+=(dDrawScale-1.0)*100;
		R.Bottom+=(dDrawScale-1.0)*100;
		R.Left+=(DrawOffs.X+StartOffs.X);
		R.Right+=(DrawOffs.X+StartOffs.X);
		R.Top+=(DrawOffs.Y+StartOffs.Y);
		R.Bottom+=(DrawOffs.Y+StartOffs.Y);
		PB->Canvas->StretchDraw(R,pBM);
		}
}
//---------------------------------------------------------------------------


void __fastcall TCst4PictureForm::PBMouseDown(TObject */*Sender*/, TMouseButton Button,
			 TShiftState /*Shift*/, int X, int Y)
{
	if (Button==mbLeft)
		{
		bDragging=true;
		StartPoint=TPoint(X,Y);
		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::PBMouseMove(TObject */*Sender*/, TShiftState /*Shift*/,
			 int X, int Y)
{
	if (bDragging)
		{
		DrawOffs.X=X-StartPoint.X;
		int nLeft=DrawRect.Left-(dDrawScale-1.0)*100+DrawOffs.X+StartOffs.X;
		if (nLeft>0)
			DrawOffs.X=-DrawRect.Left+(dDrawScale-1.0)*100-StartOffs.X;
		int nRight=DrawRect.Right+(dDrawScale-1.0)*100+DrawOffs.X+StartOffs.X;
		if (nRight<DrawRect.Width())
			DrawOffs.X=DrawRect.Width()-DrawRect.Right-(dDrawScale-1.0)*100-StartOffs.X;

		DrawOffs.Y=Y-StartPoint.Y;
		int nTop=DrawRect.Top-(dDrawScale-1.0)*100+DrawOffs.Y+StartOffs.Y;
		if (nTop>0)
			DrawOffs.Y=-DrawRect.Top+(dDrawScale-1.0)*100-StartOffs.Y;
		int nBottom=DrawRect.Bottom+(dDrawScale-1.0)*100+DrawOffs.Y+StartOffs.Y;
		if (nBottom<DrawRect.Height())
			DrawOffs.Y=DrawRect.Height()-DrawRect.Bottom-(dDrawScale-1.0)*100-StartOffs.Y;
		PB->Repaint();
		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::PBMouseUp(TObject */*Sender*/, TMouseButton /*Button*/,
			 TShiftState /*Shift*/, int /*X*/, int /*Y*/)
{
	bDragging=false;
	StartOffs.X+=DrawOffs.X;
	StartOffs.Y+=DrawOffs.Y;
	DrawOffs=TPoint(0,0);
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::ResetButClick(TObject */*Sender*/)
{
	dDrawScale=1.0;
	DrawOffs=TPoint(0,0);
	StartOffs=TPoint(0,0);
	PB->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::ReadButClick(TObject */*Sender*/)
{
	String sFile=sSelectOpenFile("Picture Files (*.jpg)","*.jpg","Select Picture File");
	if (!sFile.IsEmpty())
		{
		try
			{
			TJPEGImage *pImage=new TJPEGImage;
			pImage->LoadFromFile(sFile);
			pBM->Width=pImage->Width;
			pBM->Height=pImage->Height;
			pBM->Assign(pImage);
			delete pImage;
			}
		catch(...)
			{
			}
		bDragging=false;
		double dAspect=(pBM->Width*1.0)/pBM->Height;
		ClientHeight=ClientWidth/dAspect+Panel1->Height;
		DrawRect=TRect(0,0,ClientWidth,ClientHeight-Panel1->Height);
		dDrawScale=1.0;
		DrawOffs=TPoint(0,0);
		StartOffs=TPoint(0,0);


		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::SaveButClick(TObject */*Sender*/)
{
	String sFile=sSelectSaveFile("Picture Files (*.jpg)","*.jpg","Specify Picture File");
	if (!sFile.IsEmpty())
		{
		try
			{
			TJPEGImage *pImage=new TJPEGImage;
			pImage->Assign(pBM);
			pImage->SaveToFile(sFile);
			delete pImage;
			}
		catch(...)
			{
			ShowError("Error Saving JPEG File","File Error");
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::FileCBChange(TObject */*Sender*/)
{
	int n=FileCB->ItemIndex;
   if (n>=0)
   	ShowPicture(n);
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::ZoomInButClick(TObject */*Sender*/)
{
		dDrawScale*=1.2;
		if (dDrawScale>10.0)
			dDrawScale=10.0;
		PB->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::ZoomOutButClick(TObject */*Sender*/)
{
		dDrawScale/=1.2;
		if (dDrawScale<1.0)
			dDrawScale=1.0;
		PB->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TCst4PictureForm::DeleteButClick(TObject */*Sender*/)
{
	int n=FileCB->ItemIndex;
   if (n>=0)
   	{
      String sF=PictList[n]->sFile;
      if (bAskYes(sF,"Delete Picture File"))
      	{
         if (DeleteFile(sF))
         	{
            PictList.pFind(n);
            PictList.Delete();
				PictList.Sort(true);
            PictList.Pack();
            FileCB->Clear();
            for (int i=0; i<PictList.nGetCount(); i++)
               {
               JTime T=PictList[i]->T;
               T.ForceConvertToLocal(g_nCst4TimeBias_hours);
               FileCB->Items->Add(T.sDateTime(false,false,false)+" "+PictList[i]->sDescr);
               }
            if (FileCB->Items->Count>0)
               {
               FileCB->ItemIndex=0;
               ShowPicture(0);
               }
            else
               FileCB->ItemIndex=-1;

            }
         else
         	ShowError(sF,"Error Deleting File");
         }
      }
}
//---------------------------------------------------------------------------

