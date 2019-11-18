//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "AISExtInfoDlg.h"
#include "Cst4U_AISDataDefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAISExtInfoForm *AISExtInfoForm;
//---------------------------------------------------------------------------
__fastcall TAISExtInfoForm::TAISExtInfoForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAISExtInfoForm::FormShow(TObject *)
{
   Grid->ColWidths[0]=128;
   Grid->ColWidths[1]=256;
   Grid->Width=Grid->ColWidths[0]+Grid->ColWidths[1]+5;
   if (nSize>0)
      {
		Cst4U_AISMsgClass AIS;
		AIS.SetExtraInfoFromBuf(pucInfo,nSize);
		if (AIS.nGetParamStringsCount()>0)
			{
         Grid->RowCount=AIS.nGetParamStringsCount();
         Grid->Height=(Grid->DefaultRowHeight+1)*Grid->RowCount;
         for (int i=0; i<AIS.nGetParamStringsCount(); i++)
            {
            Grid->Cells[0][i]=AIS.sGetParamDescription(i);
            Grid->Cells[1][i]=AIS.sGetParamValue(i);
            }
         }
      }
   else if (!sCargo.IsEmpty())
      {
      Grid->RowCount=2;
      Grid->Height=(Grid->DefaultRowHeight+1)*Grid->RowCount;
      AnsiString s0="";
      AnsiString s1="";
      bool b1=false;
      for (int i=1; i<=sCargo.Length(); i++)
         {
         if ((sCargo[i]=='\r')||(sCargo[i]=='\n'))
            b1=true;
         else
            {
            if (b1)
               s1=s1+sCargo[i];
            else
               s0=s0+sCargo[i];
            }
         }
      Grid->Cells[0][0]="VESSEL TYPE";
      Grid->Cells[1][0]=s0;
      Grid->Cells[0][1]="ACTIVITY";
      Grid->Cells[1][1]=s1;
      }
   TGridRect myRect;
   myRect.Left = -1;
   myRect.Top = -1;
   myRect.Right = -1;
   myRect.Bottom = -1;
   Grid->Selection = myRect;
   ClientHeight=(Grid->DefaultRowHeight+1)*Grid->RowCount+19;
   ClientWidth=Grid->ColWidths[0]+Grid->ColWidths[1]+1+19;
}
//---------------------------------------------------------------------------
void __fastcall TAISExtInfoForm::FormClose(TObject *,
      TCloseAction &)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TAISExtInfoForm::FormCreate(TObject *)
{
   pucInfo=NULL;
   sCargo="";
}
//---------------------------------------------------------------------------

void __fastcall TAISExtInfoForm::FormDestroy(TObject *)
{
   delete[] pucInfo;
}
//---------------------------------------------------------------------------

