/************** CONFIGURATION HEADER ************************************
MODULE      SciPlot Definition File Module
COMPILER    Borland C++ Builder 6
PROCESSOR   Pentium IV
OS				MS Windows XP
PROGRAMMER  Johan Theron
CHANGE HISTORY
   1. Version 2.0.0.00  : Initial Version JGT 02/11/2008

************************************************************************/

//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "SciDef.h"
#include "JUtils.h"
#include "JDESEncrypt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


SciDef::SciDef(const String sName, const bool bEncrypt):JIniFile(sName,true,bEncrypt,false)
{
   DataLines.Clear();
   Lines.Clear();
   Labels.Clear();
   Write(L"GRAPH",L"File Name",sFileNameOnly(sName,false)+".SC32");
   Write(L"Version",SCIDEF_VERSION);
}

SciDef::~SciDef()
{
   if (DataLines.nGetCount()>0)
      {
      Write(L"DATA LINES",L"Count",DataLines.nGetCount());
      for (int i=0; i<DataLines.nGetCount(); i++)
         {
         Write(L"DATA LINE "+String(i+1),L"File",DataLines[i]->sDataFile);
         Write(L"Text File",DataLines[i]->bTextFile);
         Write(L"Description",DataLines[i]->sDescr);
         Write(L"Offset",DataLines[i]->nOffset);
         Write(L"Axis",DataLines[i]->Axis);
         if (DataLines[i]->Color!=clNone)
            Write(L"Colour",DataLines[i]->Color);
         if (DataLines[i]->LineStyle!=SCIDEF_INVALID_STYLE)
            Write(L"Style",DataLines[i]->LineStyle);
         if (DataLines[i]->nStyleLength>0)
           Write(L"Style Length",DataLines[i]->nStyleLength);
         if (DataLines[i]->nStyleLength>-1)
            Write(L"Width",DataLines[i]->nLineWidth);
         if (DataLines[i]->Symbol!=SCIDEF_INVALID_SYM)
            Write(L"Symbol",DataLines[i]->Symbol);
         if (DataLines[i]->nSymbolSize>-1)
            Write(L"Symbol Size",DataLines[i]->nSymbolSize);
         Write(L"Show In Key",DataLines[i]->bShowInKey);
         }
      }
   if (Lines.nGetCount()>0)
      {
      Write(L"ANNOTATION LINES",L"Count",Lines.nGetCount());
      for (int i=0; i<Lines.nGetCount(); i++)
         {
         Write(L"ANNOTATION LINE "+String(i+1),L"Colour",Lines[i]->Color);
         Write(L"Width",Lines[i]->nWidth);
         Write(L"Head Size 1",Lines[i]->nHeadSize1);
         Write(L"Head Size 2",Lines[i]->nHeadSize2);
         Write(L"X1",Lines[i]->dX1);
         Write(L"Y1",Lines[i]->dY1);
         Write(L"X2",Lines[i]->dX2);
         Write(L"Y2",Lines[i]->dY2);
         }
      }
   if (Labels.nGetCount()>0)
      {
      Write(L"LABELS",L"Count",Labels.nGetCount());
      for (int n=0; n<Labels.nGetCount(); n++)
         {
         TStringList* pS=new TStringList;
         pS->Text=Labels[n]->sText;
         Write(L"LABEL "+String(n+1),L"Lines",pS->Count);
         for (int i=0; i<pS->Count; i++)
            Write(L"Line "+String(i+1),L"!#"+pS->Strings[i]);
         delete pS;
         Write(L"X Pos",Labels[n]->dX);
         Write(L"Y Pos",Labels[n]->dY);
         Write(L"Alignment",Labels[n]->nAlign);
         if (Labels[n]->pFont)
            Write(L"Font",Labels[n]->pFont);
         if (Labels[n]->nFrameWidth>=0)
            Write(L"Frame Width",Labels[n]->nFrameWidth);
         if (Labels[n]->FrameColor!=clNone)
            Write(L"Frame Colour",Labels[n]->FrameColor);
         if (Labels[n]->BackColor!=clNone)
            Write(L"Background Colour",Labels[n]->BackColor);
         }
      }

   if (Symbols.nGetCount()>0)
      {
      Write(L"SYMBOLS",L"Count",Symbols.nGetCount());
      for (int n=0; n<Symbols.nGetCount(); n++)
         {
         Write(L"SYMBOL"+String(n+1),L"Symbol",Symbols[n]->Symbol);
         Write(L"X Pos",Symbols[n]->dX);
         Write(L"Y Pos",Symbols[n]->dY);
         Write(L"Size",Symbols[n]->nSize);
         Write(L"Solid",Symbols[n]->bSolid);
         Write(L"Colour",Symbols[n]->Color);
         }
      }


   if (Pictures.nGetCount()>0)
      {
      Write(L"PICTURES",L"Count",Pictures.nGetCount());
      for (int n=0; n<Pictures.nGetCount(); n++)
         {
         Write(L"PICTURE"+String(n+1),L"File Name",Pictures[n]->sFile);
         Write(L"X Pos",Pictures[n]->dX);
         Write(L"Y Pos",Pictures[n]->dY);
         Write(L"Alignment",Pictures[n]->nAlign);
         Write(L"Size",Pictures[n]->dSize);
         Write(L"Transparent Background",Pictures[n]->bTransparent);
         Write(L"Transparent Colour",Pictures[n]->TransColor);
         }
      }
}

String SciDef::sAxis(const SCIDEF_AXIS_TYPE Axis)
{
   switch(Axis)
      {
      case SCIDEF_Y1_AXIS: return "Y1 AXIS";
      case SCIDEF_Y2_AXIS: return "Y2 AXIS";
      default:             return "X AXIS";
      }
}

void SciDef::SetReadOnly(const String sPW)
{
   Write(L"GRAPH TYPE",L"Read Only",1);
   Write(L"Read Only Password",sPW);
}

void SciDef::SetCoordinates(const SCIDEF_COORDINATE_TYPE Coord)
{
   Write(L"GRAPH",L"Coordinates",Coord);
}

void SciDef::SetFramePos(const double dX1, const double dY1, const double dX2, const double dY2)
{
   Write(L"FRAME",L"X1",dX1);
   Write(L"Y1",dY1);
   Write(L"X2",dX2);
   Write(L"Y2",dY2);
}

void SciDef::SetFrame(const int nFrameWidth, const TColor FrameColor, const bool bFill, const TColor FillColor)
{
   Write(L"FRAME",L"Width",nFrameWidth);
   Write(L"Colour",FrameColor);
   Write(L"Fill",bFill);
   Write(L"Fill Colour",FillColor);
}

int SciDef::nAddDataLine(const String sDataFile, const bool bTextFile, const String sDescr, const int nOffset, const SCIDEF_AXIS_TYPE Axis, const TColor Color)
{
   DataLineStruct* pD=new DataLineStruct;
   pD->sDataFile=sDataFile;
   pD->bTextFile=bTextFile;
   pD->sDescr=sDescr;
   pD->nOffset=nOffset;
   pD->Axis=Axis;
   pD->Color=Color;
   pD->LineStyle=SCIDEF_SOLID_STYLE;
   pD->nLineWidth=1;
   pD->nStyleLength=3;
   pD->Symbol=SCIDEF_NO_SYM;
   pD->nSymbolSize=10;
   pD->bShowInKey=false;
   int n=DataLines.nAdd(pD);
   DataLines.Pack();
   return n;
}

void SciDef::SetDataLine(const int n, const SCIDEF_LINE_STYLE LineStyle, const int nStyleLength, const int nLineWidth, const SCIDEF_SYMBOL_TYPE Symbol, const int nSymbolSize)
{
   if (n<DataLines.nGetCount())
      {
      DataLines[n]->LineStyle=LineStyle;
      DataLines[n]->nStyleLength=nStyleLength;
      DataLines[n]->nLineWidth=nLineWidth;
      DataLines[n]->Symbol=Symbol;
      DataLines[n]->nSymbolSize=nSymbolSize;
      }
}
void SciDef::SetDataLineKey(const int n, const bool bShowInKey)
{
   if (n<DataLines.nGetCount())
      DataLines[n]->bShowInKey=bShowInKey;
}


void SciDef::SetAxis(const SCIDEF_AXIS_TYPE Axis, const String sLabel, const double dMin, const double dMax, const int nDecimals, const SCIDEF_NUMBER_TYPE NumType, const int nTicks)
{
   Write(sAxis(Axis),L"Label",sLabel);
   Write(L"Min",dMin);
   Write(L"Max",dMax);
   Write(L"Decimals",nDecimals);
   Write(L"Type",NumType);
   Write(L"Ticks",nTicks);
}

void SciDef::SetAxisLabelFont(const SCIDEF_AXIS_TYPE Axis, TFont* pFont)
{
   Write(sAxis(Axis),L"Label Font",pFont);
}

void SciDef::SetAxisLabelFont(const SCIDEF_AXIS_TYPE Axis, const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   SetAxisLabelFont(Axis,pF);
   delete pF;
}

void SciDef::SetAxisNumberFont(const SCIDEF_AXIS_TYPE Axis, TFont* pFont)
{
   Write(sAxis(Axis),L"Number Font",pFont);
}

void SciDef::SetAxisNumberFont(const SCIDEF_AXIS_TYPE Axis, const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   SetAxisNumberFont(Axis,pF);
   delete pF;
}

void SciDef::SetAxisTransform(const SCIDEF_AXIS_TYPE Axis, const SCIDEF_TRANSFORM_TYPE Transform)
{
   Write(sAxis(Axis),L"Transform",Transform);
}

void SciDef::SetAxisMainTicks(const SCIDEF_AXIS_TYPE Axis, const int nTickNum, const int nTickSize)
{
   Write(sAxis(Axis),L"Ticks",nTickNum);
   Write(L"Tick Size",nTickSize);
}

void SciDef::SetAxisMainGrid(const SCIDEF_AXIS_TYPE Axis, SCIDEF_LINE_STYLE LineStyle, const int nLineWidth, const TColor Color)
{
   Write(sAxis(Axis),L"Main Grid Style",LineStyle);
   Write(L"Main Grid Width",nLineWidth);
   Write(L"Main Grid Colour",Color);
}

void SciDef::SetAxisSecTicks(const SCIDEF_AXIS_TYPE Axis, const int nTickNum, const int nTickSize)
{
   Write(sAxis(Axis),L"Sec Ticks",nTickNum);
   Write(L"Sec Tick Size",nTickSize);
}

void SciDef::SetAxisSecGrid(const SCIDEF_AXIS_TYPE Axis, SCIDEF_LINE_STYLE LineStyle, const int nLineWidth, const TColor Color)
{
   Write(sAxis(Axis),L"Sec Grid Style",LineStyle);
   Write(L"Sec Grid Width",nLineWidth);
   Write(L"Sec Grid Colour",Color);
}


void SciDef::SetHeadings(const String sMain, const String sSec)
{
   Write(L"HEADINGS",L"Main",sMain);
   if (!sSec.IsEmpty())
      Write(L"Sec",sSec);
}

void SciDef::SetHeadingFont(TFont* pMainFont, TFont* pSecFont)
{
   Write(L"HEADINGS",L"Main Font",pMainFont);
   if (pSecFont)
      Write(L"Sec Font",pSecFont);
}

void SciDef::SetMainHeadingFont(const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   Write(L"HEADINGS",L"Main Font",pF);
   delete pF;
}

void SciDef::SetSecHeadingFont(const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   Write(L"HEADINGS",L"Sec Font",pF);
   delete pF;
}

void SciDef::AddLine(const TColor Color, const int nWidth, const int nHeadSize1, const int nHeadSize2, const double dX1, const double dY1, const double dX2, const double dY2)
{
   LineStruct* pL=new LineStruct;
   pL->Color=Color;
   pL->nWidth=nWidth;
   pL->nHeadSize1=nHeadSize1;
   pL->nHeadSize2=nHeadSize2;
   pL->dX1=dX1;
   pL->dY1=dY1;
   pL->dX2=dX2;
   pL->dY2=dY2;
   Lines.nAdd(pL);
   Lines.Pack();
}

int SciDef::nAddLabel(const String sText, const double dX, const double dY, const int nAlign)
{
   LabelStruct* pT=new LabelStruct;
   pT->sText=sText;
   pT->dX=dX;
   pT->dY=dY;
   pT->nAlign=nAlign;
   pT->pFont=NULL;
   pT->nFrameWidth=-1;
   pT->FrameColor=clNone;
   pT->BackColor=clNone;
   int n=Labels.nAdd(pT);
   Labels.Pack();
   return n;
}

void SciDef::SetLabelFont(const int n, TFont* pFont)
{
   if (n<Labels.nGetCount())
      {
      if (!Labels[n]->pFont)
         Labels[n]->pFont=new TFont;
      Labels[n]->pFont->Assign(pFont);
      }
}

void SciDef::SetLabelFont(const int n, const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   SetLabelFont(n,pF);
   delete pF;
}

void SciDef::SetLabelFrame(const int n, const int nFrameWidth, const TColor FrameColor, const TColor BackColor)
{
   if (n<Labels.nGetCount())
      {
      Labels[n]->nFrameWidth=nFrameWidth;
      Labels[n]->FrameColor=FrameColor;
      Labels[n]->BackColor=BackColor;
      }
}

void SciDef::SetKey(const int nFrameWidth, const TColor FrameColor, const TColor BackColor, const double dX, const double dY)
{
   Write(L"KEY",L"Frame Width",nFrameWidth);
   Write(L"Frame Colour",FrameColor);
   Write(L"Background Colour",BackColor);
   Write(L"X",dX);
   Write(L"Y",dY);
}

void SciDef::SetKeyFont(TFont* pFont)
{
   Write(L"KEY",L"Font",pFont);
}

void SciDef::SetKeyFont(const String sFontName, const int nSize, const TColor Color)
{
   TFont* pF=new TFont;
   pF->Name=sFontName;
   pF->Size=nSize;
   pF->Color=Color;
   SetKeyFont(pF);
   delete pF;
}

int SciDef::nCreateBinDataFile(const String sBinFile, const int nYParCount, const bool bEncrypt)
{
   SciDefDataFile* pF=new SciDefDataFile(sBinFile,nYParCount,bEncrypt);
   int n=DataFiles.nAdd(pF);
   DataFiles.Pack();
   return n;
}

void SciDef::AddDataFilePar(  const int n, const double dX,
                              const double dY1, const double dY2,
                              const double dY3, const double dY4,
                              const double dY5, const double dY6,
                              const double dY7, const double dY8,
                              const double dY9, const double dY10,
                              const double dY11, const double dY12,
                              const double dY13, const double dY14,
                              const double dY15, const double dY16)
{
   if (n<DataFiles.nGetCount())
      DataFiles[n]->AddPar(dX,dY1,dY2,dY3,dY4,dY5,dY6,dY7,dY8,dY9,dY10,dY11,dY12,dY13,dY14,dY15,dY16);
}

void SciDef::AddSymbol(const SCIDEF_SYMBOL_TYPE Symbol, const TColor Color, const int nSize, const double dX, const double dY, const bool bSolid)
{
   SymbolStruct* pS=new SymbolStruct;
   pS->Symbol=Symbol;
   pS->Color=Color;
   pS->nSize=nSize;
   pS->dX=dX;
   pS->dY=dY;
   pS->bSolid=bSolid;
   Symbols.nAdd(pS);
   Symbols.Pack();
}

void SciDef::AddPicture(const String sFile, const double dX, const double dY, const double dSize_percent, const int nAlign, const bool bTransparent, const TColor TransColor)
{
   PictureStruct* pP=new PictureStruct;
   pP->dX=dX;
   pP->dY=dY;
   pP->nAlign=nAlign;
   pP->dSize=dSize_percent;
   pP->bTransparent=bTransparent;
   pP->TransColor=TransColor;
   pP->sFile=sFile;
   Pictures.nAdd(pP);
   Pictures.Pack();
}


SciDefDataFile::SciDefDataFile(const String sBinFile, const int nYParCount, const bool bEncrypt)
{
	DataFile.sFile=sBinFile;
	DataFile.bEncrypt=bEncrypt;
	DataFile.nYPar=nYParCount;
	DataFile.Values.Clear();
	pdMin=new double[nYParCount+1];
	pdMax=new double[nYParCount+1];
	for (int i=0; i<=nYParCount; i++)
		{
		pdMin[i]=1e99;
		pdMax[i]=-1e99;
		}
}

SciDefDataFile::~SciDefDataFile()
{
   if (DataFile.Values.nGetCount()>0)
      {

      DataFile.Values.Pack();
      int nValSize=(DataFile.nYPar+1)*sizeof(double);
      int nBufSize=DataFile.Values.nGetCount()*nValSize;
      BYTE* pucBuf=new BYTE[nBufSize+27];    //extra 27 bytes for possible encryption
      int nOffs=0;
      for (int i=0; i<DataFile.Values.nGetCount(); i++)
         {
         memcpy(&pucBuf[nOffs],DataFile.Values[i]->pdVal,nValSize);
         nOffs+=nValSize;
         }
      if (DataFile.bEncrypt)
         {
         DESEncrypt Des(L"SciDefGen",false);
         nBufSize=Des.nEncrypt(pucBuf,nBufSize);
         }
      int anHeader[5];
      anHeader[0]=SciDef::SCIDEF_VERSION;
      anHeader[1]=nBufSize;
      anHeader[2]=0;
      if (DataFile.bEncrypt)
         anHeader[2]|=0x00000001;
      anHeader[3]=DataFile.nYPar;
      anHeader[4]=DataFile.Values.nGetCount();
      JFile Fil('O');
      JFile::FILE_ERROR E=Fil.Create(DataFile.sFile);
      if (!E)
         E=Fil.Write(anHeader,sizeof(anHeader));
      if (!E)
         Fil.Write(pucBuf,nBufSize);
		delete[] pucBuf;
		}
	delete[] pdMax;
	delete[] pdMin;
}

void SciDefDataFile::AddPar(const double dX,
										const double dY1, const double dY2,
										const double dY3, const double dY4,
										const double dY5, const double dY6,
										const double dY7, const double dY8,
										const double dY9, const double dY10,
										const double dY11, const double dY12,
										const double dY13, const double dY14,
										const double dY15, const double dY16)
{
	DataFileValStruct* pV=new DataFileValStruct(DataFile.nYPar);
	double adV[17];
	adV[0]=dX;
	adV[1]=dY1;
	adV[2]=dY2;
	adV[3]=dY3;
	adV[4]=dY4;
	adV[5]=dY5;
	adV[6]=dY6;
	adV[7]=dY7;
	adV[8]=dY8;
	adV[9]=dY9;
	adV[10]=dY10;
	adV[11]=dY11;
	adV[12]=dY12;
	adV[13]=dY13;
	adV[14]=dY14;
	adV[15]=dY15;
	adV[16]=dY16;
	for (int i=0; i<=DataFile.nYPar; i++)
		{
		pV->pdVal[i]=adV[i];
		if (adV[i]<pdMin[i])
			pdMin[i]=adV[i];
		if (adV[i]>pdMax[i])
			pdMax[i]=adV[i];
		}
	DataFile.Values.nAdd(pV);
}

double SciDefDataFile::dGetMinY() const
{
	double dMin=1e99;
	for (int i=1; i<=DataFile.nYPar; i++)
		{
		if (pdMin[i]<dMin)
			dMin=pdMin[i];
		}
	return dMin;
}

double SciDefDataFile::dGetMaxY() const
{
	double dMax=-1e99;
	for (int i=1; i<=DataFile.nYPar; i++)
		{
		if (pdMax[i]>dMax)
			dMax=pdMax[i];
		}
	return dMax;
}

void SciDefDataFile::GetSpan(double& /*dMin*/, double& /*dMax*/, int& /*nTicks*/, int& /*nDecimals*/)
{
/*
	double dF;
	if (fabs(dMin)>fabs(dMax))
		dF=dMin;
	else
		dF=dMax;
	if (dF<1e-6) dF=1e-6;
	dF=log10(dF);
	int nD=nRnd(dF);
	switch(nD)
		{
		case -5:
			nDecimals=5;

			break;
		case 1:
			nDecimals=1;
			dMin=floor(dMin*pow(10.0,nDecimals));
			dMin*=pow(10.0,nDecimals);
			dMax=ceil(dMin*pow(10.0,nDecimals));
			dMax*=pow(10.0,nDecimals);
			nTicks=11;
			break;
		default:	;
		}
	}
	nDecimals=ceil(fabs(dF));
	if (dF<0.0)
		{
		dF=pow(10.0,nDecimals);

		int nMin=dMin*dF;
		dMin=nMin/dF;
		int nMax=dMax*dF;
		dMax=nMax/dF;
		nTicks=11;
		}
	else
		{
		dF=pow(10.0,nDecimals-1);
		int nMin=dMin/dF;
		dMin=nMin*dF;
		int nMax=dMax/dF;
		dMax=nMax*dF;
		nDecimals=0;
		nTicks=11;
		}
*/
}

void SciDefDataFile::GetXSpan(double& dMin, double& dMax, int& nTicks, int& nDecimals)
{
	dMin=dGetMinX();
	dMax=dGetMaxX();
	nTicks=11;
	nDecimals=1;
//	GetSpan(dMin,dMax,nTicks,nDecimals);
}

void SciDefDataFile::GetYSpan(double& dMin, double& dMax, int& nTicks, int& nDecimals)
{
	dMin=dGetMinY();
	dMax=dGetMaxY();
	nTicks=11;
	nDecimals=1;
//	GetSpan(dMin,dMax,nTicks,nDecimals);
}




