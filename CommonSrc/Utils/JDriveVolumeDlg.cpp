//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JDriveVolumeDlg.h"
#include "JUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDriveVolumeForm *DriveVolumeForm;
//---------------------------------------------------------------------------
__fastcall TDriveVolumeForm::TDriveVolumeForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDriveVolumeForm::FormShow(TObject *Sender)
{
	VolLB->Clear();
   String sDrive=" :\\";
   int nV=-1;
	for (char cDrive='C'; cDrive<='Z'; cDrive++)
		{
      sDrive[1]=cDrive;
      String sVol=sGetDriveVolume(sDrive);
      if (!sVol.IsEmpty())
      	{
      	VolLB->Items->Add(sDrive+"  "+sVol);
         if (sVol==sVolume)
         	nV=VolLB->Items->Count-1;
         }
      }
  	VolLB->ItemIndex=nV;
   sVolume="";
}
//---------------------------------------------------------------------------
void __fastcall TDriveVolumeForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TDriveVolumeForm::VolLBDblClick(TObject *Sender)
{
	int n=VolLB->ItemIndex;
   if (n>=0)
   	{
      AnsiString s=VolLB->Items->Strings[n];
      sVolume=s.SubString(6,s.Length()-5);
      }
   else
   	sVolume="";
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TDriveVolumeForm::OKButClick(TObject *Sender)
{
	VolLBDblClick(0);
}
//---------------------------------------------------------------------------
