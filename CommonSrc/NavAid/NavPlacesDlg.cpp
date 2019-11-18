//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "NavPlacesDlg.h"
#include "JFileDlg.h"
#include "JCommaText.h"
#include "JMessage.h"

#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "NumEdit"
#pragma resource "*.dfm"
TNavPlacesForm *NavPlacesForm;
//---------------------------------------------------------------------------
__fastcall TNavPlacesForm::TNavPlacesForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::FormShow(TObject *)
{
   Ed.Init();

   SourceList->Clear();
   ConvertList->Clear();

   DefaultSymbol.bDispSymbol=true;
   DefaultSymbol.Symbol=SymbolStruct::CIRCLE_SYM;
   DefaultSymbol.nSymbolSize=2;
   DefaultSymbol.SymbolColor=clWhite;
   DefaultSymbol.bSolidFill=true;
   DefaultSymbol.bDispText=true;
   DefaultSymbol.pFont->Name=L"Arial";
   DefaultSymbol.pFont->Color=clWhite;
   DefaultSymbol.pFont->Size=10;
   DefaultSymbol.pText->Clear();
   DefaultSymbol.TextAlign=SymbolStruct::AL_TOPLEFT;
   DefaultSymbol.bSolidBack=true;
   DefaultSymbol.bLocked=true;

   EditBut->Enabled=false;
   CreateBut->Enabled=false;
   RemoveBut->Enabled=false;

   nChartSize=0;
   pChartLL=NULL;
   bBusy=false;

   CloseBut->SetFocus();
	bReading=false;

#ifdef _CST4_DICT
	g_Dict.Translate(this);	//Form Caption




#endif

}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::FormClose(TObject *,
		TCloseAction &/*Action*/)
{
	ClearSourceList();
	delete[] pChartLL;
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::SourceButClick(TObject *)
{
	bReading=true;
	String sFile=sSelectOpenFile(L"Comma Delimited Files (*.csv)",L"*.csv",L"Select Comma Delimited File");
   if (!sFile.IsEmpty())
      {
      Screen->Cursor=crHourGlass;
      MyMessage->Show();
      MyMessage->SetText(L"Reading Place Name Data ...");
      bAbort=false;
      AbortBut->Show();
      ClearSourceList();
      Nav.Clear();
      Places.Clear();
      ConvertList->Clear();
      ProvCombo->Clear();
      ProvCombo->Items->Add(L"All Provinces");
      FeatureCombo->Clear();
      FeatureCombo->Items->Add(L"All Features");
      EditBut->Enabled=false;
      ListedEd->Text=L"";
      SkippedEd->Text=L"";
      ChartChk->Checked=false;
      FilterChk->Checked=false;
      AreaChk->Checked=false;
      Caption=L"Create Nav Aid for Place Names";
      CreateBut->Enabled=false;
      RemoveBut->Enabled=false;
      JFile Fil('I',JFile::ASCII_TYPE);
      JFile::FILE_ERROR Error=Fil.Open(sFile);
      char szSt[256];
      while((!Error)&&(!bAbort))
         {
         Error=Fil.Read(szSt,256);
         if (!Error)
            {
            CommaText Txt(szSt);
            if (Txt.nGetNum()==6)
               {
               PlaceNameStruct* pPlace=new PlaceNameStruct;
               bool bOK;
               try
                  {
                  pPlace->sProvince=Txt.sGetPar(0);
                  pPlace->sName=Txt.sGetPar(1);
                  pPlace->sFeature=Txt.sGetPar(2);
                  String s=Txt.sGetPar(3);
                  bOK=bGetLL(s,pPlace->Pos.dLat);
                  if (bOK)
                     {
                     s=Txt.sGetPar(4);
                     bOK=bGetLL(s,pPlace->Pos.dLong);
                     }
                  }
               catch(...)
                  {
                  bOK=false;
                  }
               if (bOK)
                  Places.nAdd(pPlace);
               else
                  delete pPlace;
               }
            }
         Application->ProcessMessages();
         }
      Places.Pack();
      MyMessage->SetText(L"Updating Filter Lists ...");
      Application->ProcessMessages();
      if ((Places.nGetCount()>0)&&(!bAbort))
         {
         PlaceNameStruct* pP;
         for (int i=0; (i<Places.nGetCount())&&(!bAbort); i++)
            {
            pP=Places[i];
            bool bFound=false;
            for (int j=0; (j<ProvCombo->Items->Count)&&(!bFound); j++)
               bFound=ProvCombo->Items->IndexOf(pP->sProvince)>=0;
            if (!bFound)
               ProvCombo->Items->Add(pP->sProvince);
            bFound=false;
            for (int j=0; (j<FeatureCombo->Items->Count)&&(!bFound); j++)
               bFound=FeatureCombo->Items->IndexOf(pP->sFeature)>=0;
            if (!bFound)
               FeatureCombo->Items->Add(pP->sFeature);
            Application->ProcessMessages();
            }
         bReading=false;
         SourceList->ItemIndex=0;
         SourceListClick(0);
         }
      else
         SourceList->ItemIndex=-1;

      ProvCombo->ItemIndex=ProvCombo->Items->IndexOf(L"All Provinces");
      FeatureCombo->ItemIndex=FeatureCombo->Items->IndexOf(L"All Features");
      Screen->Cursor=crDefault;
      MyMessage->SetText(L"Updating Source List ...");
      Application->ProcessMessages();
      UpdateSourceList();
      MyMessage->Hide();
      Application->ProcessMessages();
      Caption=L"Create Nav Aid for Place Names ["+sFileNameOnly(sFile)+" - "+String(Places.nGetCount())+" Entries]";
      AbortBut->Hide();
      }
   bReading=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::JumpNameEdChange(TObject *)
{
   String s=JumpNameEd->Text;
   if (!s.IsEmpty())
      {
      Screen->Cursor=crHourGlass;
      s=s.UpperCase();
      int nFound=-1;
      for (int i=0; (i<SourceList->Count)&&(nFound<0); i++)
         {
         String sL=SourceList->Items->Strings[i].UpperCase();
         if (sL.Pos(s)==1)
            nFound=i;
         }
      if (nFound>=0)
         {
         SourceList->ItemIndex=nFound;
         SourceListClick(0);
         }
      Screen->Cursor=crDefault;
      }
}

//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::SelectButClick(TObject *)
{
   int n=SourceList->ItemIndex;
   if (n>=0)
      {
      PlaceNameStruct* pPlace=(PlaceNameStruct*)SourceList->Items->Objects[n];
      SymbolStruct* pS=new SymbolStruct;
      *pS=DefaultSymbol;
      pS->SetText(pPlace->sName);
      pS->Pos=pPlace->Pos;
      Nav.Add(pS);
      ConvertList->Items->Add(pPlace->sName);
      ConvertList->ItemIndex=ConvertList->Items->Count-1;
      CreateBut->Enabled=(ConvertList->Items->Count>0);
      ConvertListClick(0);
      Ed.SetValRO(ConvertEd,ConvertList->Items->Count);
      }
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::SourceListDblClick(TObject *)
{
   SelectButClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::SourceListClick(TObject *)
{
   if (!bReading)
      {
      int n=SourceList->ItemIndex;
      if (n>=0)
         {
         PlaceNameStruct* pPlace=(PlaceNameStruct*)SourceList->Items->Objects[n];
         ProvEd->Text=pPlace->sProvince;
         FeatureEd->Text=pPlace->sFeature;
         Ed.nSetLL("Position",LatEd,LongEd,pPlace->Pos);
         }
      }
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::RemoveButClick(TObject *)
{
   if (ConvertList->SelCount>0)
      {
      int n=ConvertList->Items->Count-1;
      for (int i=ConvertList->Items->Count-1; i>=0; i--)
         {
         if (ConvertList->Selected[i])
            {
            Nav.DeleteSymbol(i);
            ConvertList->Items->Delete(i);
            n=i;
            }
         }
      if (ConvertList->Items->Count>0)
         {
         if (n>ConvertList->Items->Count-1)
            n=ConvertList->Items->Count-1;
         ConvertList->Selected[n]=true;
         ConvertListClick(0);
         }
      CreateBut->Enabled=(ConvertList->Items->Count>0);
      RemoveBut->Enabled=(ConvertList->SelCount>0);
      EditBut->Enabled=(ConvertList->SelCount>0);
      Ed.SetValRO(ConvertEd,ConvertList->Items->Count);
      }
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::EditButClick(TObject *)
{
   if (ConvertList->SelCount>0)
      {
      SymbolStruct* pS=NULL;
      bool bOK=false;
      for (int i=0; (i<ConvertList->Items->Count)&&(!pS); i++)
         {
         if (ConvertList->Selected[i])
            {
            pS=Nav.pGetSymbol(i);
            bOK=pS->bEdit(ConvertList->SelCount>1,true);
            }
         }
      if (bOK)
         {
         for (int i=0; i<ConvertList->Items->Count; i++)
            {
            if (ConvertList->Selected[i])
               {
               SymbolStruct* pSym=Nav.pGetSymbol(i);
               pS->SetText(pSym->sGetText());
               *pSym=*pS;
               }
            }
         }
      }
}
//---------------------------------------------------------------------------
void __fastcall TNavPlacesForm::CreateButClick(TObject *)
{
   String sFile=sSelectSaveFile(L"Nav Aid Files (*.NavAid2)",L"*.NavAid2",L"Specify Nav Aid File");
   if (!sFile.IsEmpty())
      {
      MyMessage->Show();
      MyMessage->SetText(L"Storing Nav Aids ...");
      Nav.Store(sFile,LatLong(0.0,0.0),SizeD(0.0,0.0));
      MyMessage->Hide();
      }
}
//---------------------------------------------------------------------------

bool __fastcall TNavPlacesForm::bGetLL(const String sLL, double& dLL)
{
   //30:56:25S
   String s=sLL;
   bool bOK=(s.Length()==9);
   if (bOK)
      {
      for (int i=1; i<=s.Length(); i++)
         {
         if (s[i]==':')
            s[i]=',';
         }
      char c=s[9];
      s[9]=' ';
      s=s.Trim();
      CommaText Txt(s);
      bOK=(Txt.nGetNum()==3);
      if (bOK)
         {
         double dDeg=Txt.dGetVal(0);
         double dMin=Txt.dGetVal(1);
         double dSec=Txt.dGetVal(2);
         dLL=dDeg+dMin/60.0+dSec/3600.0;
         switch(c)
            {
            case 'S':
            case 'W':
               dLL*=-1.0;
            default: ;
            }
         }
      }
   return bOK;
}
void __fastcall TNavPlacesForm::ConvertListClick(TObject *)
{
   RemoveBut->Enabled=(ConvertList->SelCount>0);
   EditBut->Enabled=(ConvertList->SelCount>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::ClearSourceList()
{
   for (int i=0; i<SourceList->Count; i++)
      {
      PlaceNameStruct* pPlace=(PlaceNameStruct*)SourceList->Items->Objects[i];
      delete pPlace;
      }
   SourceList->Clear();
}

void __fastcall TNavPlacesForm::AddSourceList(const PlaceNameStruct& Place)
{
   PlaceNameStruct* pPlace=new PlaceNameStruct;
   *pPlace=Place;
   SourceList->Items->AddObject(pPlace->sName,(TObject*)pPlace);
   SourceList->ItemIndex=SourceList->Count-1;
   SourceListClick(0);
   Application->ProcessMessages();
}

void __fastcall TNavPlacesForm::ProvComboChange(TObject *)
{
   if (!bReading)
      {
      bAbort=false;
      AbortBut->Show();
      UpdateSourceList();
      AbortBut->Hide();
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::FeatureComboChange(TObject *)
{
   if (!bReading)
      {
      bAbort=false;
      AbortBut->Show();
      UpdateSourceList();
      AbortBut->Hide();
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::UpdateSourceList()
{
   Screen->Cursor=crHourGlass;
   Application->ProcessMessages();
   String sFeature=FeatureCombo->Items->Strings[FeatureCombo->ItemIndex];
   String sProvince=ProvCombo->Items->Strings[ProvCombo->ItemIndex];
   bool bAllProvinces=(sProvince==L"All Provinces");
   bool bAllFeatures=(sFeature==L"All Features");
   bool bAllPos=!FilterChk->Checked;
   LatLong LL=Ed.GetLL(FilterLatEd,FilterLongEd);
   bool bAllArea=!AreaChk->Checked;
   LatLong TL=Ed.GetLL(TLLatEd,TLLongEd);
   LatLong BR=Ed.GetLL(BRLatEd,BRLongEd);
   bool bAllChart=!ChartChk->Checked;
   double dFilterDist=Ed.dGetVal(FilterDistEd)*1000.0; //in m
   double dChartDist=Ed.dGetVal(ChartDistEd)*1000.0; //in m
   PlaceNameStruct* pP;
   ClearSourceList();
   int nSkipped=0;
   SkippedEd->Text=L"";
   ListedEd->Text=L"";
   for (int i=0; (i<Places.nGetCount())&&(!bAbort); i++)
      {
      pP=Places[i];
		bool bValidFeature=bAllFeatures || (sFeature.CompareIC(pP->sFeature)==0);
		bool bValidProvince=bAllProvinces || (sProvince.CompareIC(pP->sProvince)==0);
      bool bValidPos=true;
      if (!bAllPos)
         bValidPos=bValidLL(LL,pP->Pos,dFilterDist);
      bool bValidArea=true;
      if (!bAllArea)
         {
         bValidArea=(pP->Pos.dLat<=TL.dLat)&&(pP->Pos.dLong>=TL.dLong);
         if (bValidArea)
            bValidArea=(pP->Pos.dLat>=BR.dLat)&&(pP->Pos.dLong<=BR.dLong);
         }
      bool bValidChart;
      if ((!bAllChart)&&(nChartSize>0))
         {
         bValidChart=false;
         Transform Trans;
         for (int n=0; n<nChartSize; n++)
            {
            Trans.SetLLRef(pP->Pos);
            Trans.SetLLView(pP->Pos,SizeD(1.0,1.0));
            Trans.SetRadarPos(pP->Pos,0.0);
            if (Trans.dGetDist(pP->Pos,pChartLL[n])<dChartDist)
               {
               bValidChart=true;
               n=nChartSize;
               }
            }
         }
      else
         bValidChart=true;
      if ((bValidProvince)&&(bValidFeature)&&(bValidPos)&&(bValidChart)&&(bValidArea))
         {
         AddSourceList(*pP);
         Ed.SetValRO(ListedEd,SourceList->Items->Count);
         }
      else
         Ed.SetValRO(SkippedEd,++nSkipped);
      Application->ProcessMessages();
      }
   if (SourceList->Items->Count>0)
      {
      SourceList->ItemIndex=0;
      SourceListClick(0);
      }
   Screen->Cursor=crDefault;
}

void __fastcall TNavPlacesForm::FilterChkClick(TObject *)
{
   if (!bBusy)
      {
      bBusy=true;
      if (FilterChk->Checked)
         {
         LatLong LL=Ed.GetLL(FilterLatEd,FilterLongEd);
         bAbort=(LL.dLat<-500.0)||(LL.dLong<-500.0);
         if (bAbort)
            FilterChk->Checked=false;
         }
      else
         bAbort=false;
      if (!bAbort)
         {
         AbortBut->Show();
         UpdateSourceList();
         AbortBut->Hide();
         }
      else
         ShowInfo(L"Invalid Latitude/Longitude Values",L"Invalid Filter Setup");
      bBusy=false;
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::FilterLatEdChange(TObject *)
{
   FilterChk->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::FilterLongEdChange(TObject *)
{
   FilterChk->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::FilterDistEdChange(TObject *)
{
   FilterChk->Checked=false;
}
//---------------------------------------------------------------------------

bool __fastcall TNavPlacesForm::bValidLL(const LatLong& LL, const LatLong& Pos, const double dMaxDist)
{
   Transform Trans;
   Trans.SetLLRef(LL);
   Trans.SetLLView(LL,SizeD(1.0,1.0));
   Trans.SetRadarPos(LL,0.0);
   return Trans.dGetDist(LL,Pos)<dMaxDist;
}

void __fastcall TNavPlacesForm::ExportButClick(TObject *)
{
   String sFile=sSelectSaveFile(L"Comma Delimited Files (*.csv)",L"*.csv",L"Specify Comma Delimited File");
   if (!sFile.IsEmpty())
      {
      MyMessage->Show();
      MyMessage->SetText(L"Exporting Place Names ...");
      Screen->Cursor=crHourGlass;
      Application->ProcessMessages();
      JFile Fil('O',JFile::ASCII_TYPE);
      JFile::FILE_ERROR Error=Fil.Create(sFile);
      if (!Error)
         Error=Fil.TextLine(L"PROVINCE,NAME,FEATYPE,LATITUDE,LONGITUDE,REFERENCE");
      String s;
      for (int i=0; (i<SourceList->Items->Count)&&(!Error); i++)
         {
         PlaceNameStruct* pP=(PlaceNameStruct*)SourceList->Items->Objects[i];
         s=pP->sProvince+",L"+pP->sName+",L"+pP->sFeature+",L"+sSetLL(pP->Pos)+",NONE";
         Error=Fil.TextLine(s);
         }
      MyMessage->Hide();
      Screen->Cursor=crDefault;
      }
}
//---------------------------------------------------------------------------

String __fastcall TNavPlacesForm::sSetLL(const LatLong& LL)
{
   String sC;
   if (LL.dLat<0.0)
      sC=L"S";
   else
      sC=L"N";
   double dInt;
   double dFrac=modf(fabs(LL.dLat),&dInt);
   int nDeg=floor(dInt);
   dFrac=modf(dFrac*60.0,&dInt);
   int nMin=floor(dInt);
   int nSec=floor(dFrac*60.0);
   String sLat;
   sLat.printf(L"%02d:%02d:%02d",nDeg,nMin,nSec);
   sLat=sLat+sC;
   if (LL.dLong<0.0)
      sC=L"W";
   else
      sC=L"E";
   dFrac=modf(fabs(LL.dLong),&dInt);
   nDeg=floor(dInt);
   dFrac=modf(dFrac*60.0,&dInt);
   nMin=floor(dInt);
   nSec=floor(dFrac*60.0);
   String sLong;
   sLong.printf(L"%02d:%02d:%02d",nDeg,nMin,nSec);
   sLong=sLong+sC;
   return sLat+",L"+sLong;
}

void __fastcall TNavPlacesForm::ChartButClick(TObject *)
{
   String sFile=sSelectOpenFile("Chart Files (*.chart)",L"*.chart",L"Select Chart File");
   if (!sFile.IsEmpty())
      {
      nChartSize=0;
      delete[] pChartLL;
      pChartLL=NULL;
      JFile Fil('I');
      JFile::FILE_ERROR Error=Fil.Open(sFile);
      if (!Error)
         Error=Fil.Read(&nChartSize,sizeof(nChartSize));
      if ((!Error)&&(nChartSize>0))
         {
         pChartLL=new LatLong[nChartSize];
         Error=Fil.Read(pChartLL,sizeof(LatLong)*nChartSize);
         }
      Fil.ShowError(Error);
      if (Error)
         {
         nChartSize=0;
         delete[] pChartLL;
         pChartLL=NULL;
         ChartChk->Checked=false;
         }
      else
         ChartEd->Text=sFileNameOnly(sFile);
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::ChartChkClick(TObject *)
{
   if (ChartChk->Checked)
      {
      if (ChartEd->Text.IsEmpty())
         ChartButClick(0);
      if (!ChartEd->Text.IsEmpty())
         {
         bAbort=false;
         AbortBut->Show();
         UpdateSourceList();
         AbortBut->Hide();
         }
      }
   else
      {
      bAbort=false;
      AbortBut->Show();
      UpdateSourceList();
      AbortBut->Hide();
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::ConvertListDblClick(TObject *)
{
   EditButClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::SelectAllButClick(TObject *)
{
   for (int i=0; i<SourceList->Items->Count; i++)
      {
      PlaceNameStruct* pPlace=(PlaceNameStruct*)SourceList->Items->Objects[i];
      SymbolStruct* pS=new SymbolStruct;
      *pS=DefaultSymbol;
      pS->SetText(pPlace->sName);
      pS->Pos=pPlace->Pos;
      Nav.Add(pS);
      ConvertList->Items->Add(pPlace->sName);
      ConvertList->ItemIndex=ConvertList->Items->Count-1;
      ConvertListClick(0);
      Ed.SetValRO(ConvertEd,ConvertList->Items->Count);
      }
   CreateBut->Enabled=(ConvertList->Items->Count>0);
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::AbortButClick(TObject *)
{
   bAbort=true;   
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::AreaChkClick(TObject *)
{
   if (!bBusy)
      {
      bBusy=true;
      if (AreaChk->Checked)
         {
         LatLong TL=Ed.GetLL(TLLatEd,TLLongEd);
         LatLong BR=Ed.GetLL(BRLatEd,BRLongEd);
         bAbort=(TL.dLat<-500.0)||(TL.dLong<-500.0)||(BR.dLat<-500.0)||(BR.dLong<-500.0);
         }
      else
         bAbort=false;
      if (!bAbort)
         {
         AbortBut->Show();
         UpdateSourceList();
         AbortBut->Hide();
         }
      else
         {
         AreaChk->Checked=false;
         ShowInfo(L"Invalid Latitude/Longitude Values",L"Invalid Filter Setup");
         }
      bBusy=false;
      }
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::TLLatEdChange(TObject *)
{
   AreaChk->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::TLLongEdChange(TObject *)
{
   AreaChk->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::BRLatEdChange(TObject *)
{
   AreaChk->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TNavPlacesForm::BRLongEdChange(TObject *)
{
   AreaChk->Checked=false;
}
//---------------------------------------------------------------------------

