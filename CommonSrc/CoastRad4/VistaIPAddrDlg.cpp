//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "VistaIPAddrDlg.h"
#include "JFile.h"
#include "JCommaText.h"
#include "JUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIPAddrForm *IPAddrForm;
//---------------------------------------------------------------------------
__fastcall TIPAddrForm::TIPAddrForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::FormShow(TObject */*Sender*/)
{
	AddrEd->SetFocus();

   AddrList.Clear();
	String sF="c:\\IMT Software\\VistaNet Shared\\Vista_Adresses.csv";
	if (bFileExist(sF))
		{
		JFile Fil('I',JFile::ASCII_TYPE);
		JFile::FILE_ERROR E=Fil.Open(sF);
		String s;
		while(!E)
			{
			E=Fil.Read(s);
			CommaText C(s);
			if (C.nGetNum()>=2)
				{
				AddrStruct* pA=new AddrStruct;
				pA->sAddr=C.sGetPar(0);
				pA->sDescr=C.sGetPar(1);
				AddrList.nAdd(pA);
				}
			}
		}
	LB->Clear();
	AddrList.Sort();
	AddrStruct* pA;
	int i=0;
	int nFound=-1;
	AddrList.GoFirst();
	while(!AddrList.bLast())
		{
		pA=AddrList.pNext();
		String s=pA->sAddr+"\t"+pA->sDescr;
		LB->Items->Add(s);
		if (sAddr.Trim()==pA->sAddr.Trim())
			nFound=i;
		++i;
		}
	AddrList.Pack();
	LB->ItemIndex=nFound;
	AddrEd->Text=sAddr;
	if (nFound>=0)
		DescrEd->Text=AddrList[nFound]->sDescr;
	else
		DescrEd->Text="";
	bChanged=false;
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::FormClose(TObject */*Sender*/, TCloseAction &/*Action*/)
{
	if (ModalResult==mrOk)
		{
		sAddr=AddrEd->Text;
		String sF="c:\\IMT Software\\VistaNet Shared\\Vista_Adresses.csv";
		if (bChanged)
			{
			JFile Fil('O',JFile::ASCII_TYPE);
			JFile::FILE_ERROR E=Fil.Create(sF);
			String s;
			AddrList.GoFirst();
			AddrStruct* pA;
			while((!AddrList.bLast())&&(!E))
				{
				pA=AddrList.pNext();
				s=pA->sAddr.Trim()+","+pA->sDescr.Trim();
         	E=Fil.TextLine(s);
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::AddButClick(TObject */*Sender*/)
{
	AddrStruct* pA=new AddrStruct;
	pA->sAddr=AddrEd->Text;
	pA->sDescr=DescrEd->Text;
	AddrList.nAdd(pA);
	AddrList.Sort();
	sAddr=AddrEd->Text;
	LB->Clear();
	int i=0;
	int nFound=-1;
   AddrList.GoFirst();
	while(!AddrList.bLast())
		{
		pA=AddrList.pNext();
		String s=pA->sAddr+"\t"+pA->sDescr;
		LB->Items->Add(s);
		if (sAddr.Trim()==pA->sAddr.Trim())
			nFound=i;
		++i;
		}
	AddrList.Pack();
	LB->ItemIndex=nFound;
	bChanged=true;
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::DeleteButClick(TObject */*Sender*/)
{
	int n=LB->ItemIndex;
	if (n>=0)
		{
		AddrList.Delete(n);
		AddrList.Sort();
		sAddr=AddrEd->Text;
		LB->Clear();
		int i=0;
		int nFound=-1;
		AddrStruct* pA;
		AddrList.GoFirst();
		while(!AddrList.bLast())
			{
			pA=AddrList.pNext();
			String s=pA->sAddr+"\t"+pA->sDescr;
			LB->Items->Add(s);
			if (sAddr.Trim()==pA->sAddr.Trim())
				nFound=i;
			++i;
			}
		AddrList.Pack();
		LB->ItemIndex=nFound;
		bChanged=true;
		}
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::UseButClick(TObject */*Sender*/)
{
	int n=LB->ItemIndex;
	if (n>=0)
		{
		AddrEd->Text=AddrList[n]->sAddr;
		DescrEd->Text=AddrList[n]->sDescr;
		}
}
//---------------------------------------------------------------------------
void __fastcall TIPAddrForm::LBDblClick(TObject */*Sender*/)
{
   UseButClick(0);
}
//---------------------------------------------------------------------------


void SelectVistaAddr(TComponent* pOwner, TEdit* pEd)
{
	IPAddrForm=new TIPAddrForm(pOwner);
	IPAddrForm->sAddr=pEd->Text;
	if (IPAddrForm->ShowModal()==mrOk)
		pEd->Text=IPAddrForm->sAddr;
	delete IPAddrForm;

}
