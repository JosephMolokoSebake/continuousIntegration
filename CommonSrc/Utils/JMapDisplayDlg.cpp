//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "JMapDisplayDlg.h"
#include "JColorDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMapDisplayForm *MapDisplayForm;
//---------------------------------------------------------------------------
__fastcall TMapDisplayForm::TMapDisplayForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMapDisplayForm::FormShow(TObject *)
{
	for (int i=0; i<48; i++)
		{

		int nx=(i%6)*70+10;
		int ny=(i/6)*25+10;

		apDisp[i]=new TPanel(this);
		apDisp[i]->ParentBackground=false;
		apDisp[i]->Parent=Panel;
		apDisp[i]->Color=aDispCol[i];

		apDisp[i]->SetBounds(nx,ny,20,20);

		apPrint[i]=new TPanel(this);
		apPrint[i]->ParentBackground=false;
		apPrint[i]->Parent=Panel;
		apPrint[i]->Color=aPrintCol[i];
      apPrint[i]->Cursor=crHandPoint;
      apPrint[i]->SetBounds(nx+35,ny,20,20);
      apPrint[i]->OnClick=OnPrintClick;

      apLab[i]=new TLabel(this);
      apLab[i]->Parent=Panel;
      apLab[i]->Caption=L"®";
      apLab[i]->Left=nx+23;
      apLab[i]->Top=ny+2;
      apLab[i]->Font->Name=L"Symbol";
      }

   for (int i=0; i<5; i++){
      int nx=(i%6)*70+10;
      apBev[i]=new TBevel(this);
      apBev[i]->Parent=Panel;
      apBev[i]->SetBounds(nx+63,10,2,195);
      }
}
//---------------------------------------------------------------------------
void __fastcall TMapDisplayForm::FormClose(TObject *, TCloseAction &/*Action*/)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TMapDisplayForm::OnPrintClick(TObject* Sender)
{
	int i=0;
	while((i<48)&&(Sender!=apPrint[i]))
		{
		i++;
		}
	if (i<48)
		{
		ColorForm=new TColorForm(0);
		ColorForm->Color=apPrint[i]->Color;
		ColorForm->ShowModal();
		apPrint[i]->ParentColor=false;
		apPrint[i]->Color=ColorForm->Color;
		delete ColorForm;
		}
}

void __fastcall TMapDisplayForm::MapColor(const int n, const TColor DispCol, const TColor PrintCol)
{
   aDispCol[n]=DispCol;
   aPrintCol[n]=PrintCol;
}

TColor __fastcall TMapDisplayForm::GetMapColor(const int n){
   return apPrint[n]->Color;
}


void __fastcall TMapDisplayForm::ResetButClick(TObject *)
{
   for (int i=0; i<48; i++){
      aPrintCol[i]=aDispCol[i];
      apPrint[i]->Color=aPrintCol[i];
      }
}
//---------------------------------------------------------------------------

