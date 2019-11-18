 //---------------------------------------------------------------------------
#ifndef NavAidH                                  `
#define NavAidH
//---------------------------------------------------------------------------

#ifdef __DDGRAPH
	#include "JDDGraph.h"
#else
	#include "JTransGraph.h"
#endif

#include "JFile.h"
#include "JUtils.h"
#include "JIniFile.h"
#include "JList.h"

#define NAVAID_VERSION 2001

enum NAV_HOTSPOT
{
   NAV_NO_HOTSPOT,
   NAV_LINE1_HOTSPOT,
   NAV_LINE2_HOTSPOT,
   NAV_SYMBOL_HOTSPOT,
   NAV_TEXT_HOTSPOT
};

//********************************************************************************

struct SymbolStruct
{

   enum ALIGN_TYPE
   {
      AL_TOPLEFT,
      AL_TOP,
      AL_TOPRIGHT,
      AL_BOTLEFT,
      AL_BOTTOM,
      AL_BOTRIGHT,
      AL_LEFT,
      AL_RIGHT,
      AL_CENTER,
      AL_AUTO,
      AL_DUMMY=0xFFFFFFFF,
   };

   enum SYMBOL_TYPE
   {
      CIRCLE_SYM,
      SQUARE_SYM,
      DIAMOND_SYM,
      CROSS_SYM,
      DUMMY_SYM=0xFFFFFFFF,
   };

   bool bDispSymbol;
   SYMBOL_TYPE Symbol;
   int nSymbolSize;
   TColor SymbolColor;
   bool bSolidFill;
   bool bDispText;
   TFont* pFont;
//   String sText;
   ALIGN_TYPE TextAlign;
   bool bSolidBack;
   bool bLocked;
   bool bRef;
   LatLong Pos;
   Pixel PixPos;
   SizeI Size;
   TRect Bounds;
   bool bDeleted;
   bool bDisplay;

   TStringList* pText;
   TColor FrameBackColor;
   bool bShowFrame;
   TColor FrameColor;
   int nFrameWidth;

   int nDummy;

private:

   void SetData(const bool bDefault, const bool bLockPos);
   bool bGetData();

public:

   void Init();
	SymbolStruct();
   SymbolStruct(const SymbolStruct& S);
   ~SymbolStruct();
	SymbolStruct& operator = (const SymbolStruct& S);

	void Assign(const SymbolStruct& S);

   void SetPos(const Pixel& Pix);
   void SetPos(const LatLong& LL){Pos=LL;}
   void CalcBounds();
   void Draw(HDC DC, const TColor BackColor);
   void DrawRect(HDC DC);
   void Store(JIniFile* pFil, const int n);
   void Store(JIniFile* pFil, const String sName);
   String sStripText(const String s1);
   void Read(JIniFile* pFil, const int n);
	void Read(JIniFile* pFil, const String sName);
	bool bEdit(const bool bDefault, const bool bLockPos=false, const bool bShowDel=true);
	bool bEditSiteSymbol();
	void OpenEdit();
   NM_DIALOG_STATE CloseEdit();
   bool bInside(const Pixel Pix);
   void CalcSize(HDC DC);
   String sSymbolType();

   void SetText(const String s);
   String sGetText() const;

};

//********************************************************************************

struct LineStruct
{

   TColor Color;
   int nWidth;
   LatLong aPos[2];
   bool abShowArrow[2];
   int anArrowSize[2];
   int nStyle;
   bool bLocked;
   Pixel aPixPos[2];
   TRect aBounds[2];
   bool bDeleted;
   bool bDisplay;
   bool bMulti;
   JList<LatLong> SegPosList;
   JList<Pixel> SegPixList;

private:

   void Init();
   void SetData(const bool bDefault, const int nSide=0);
   bool bGetData();

public:

	LineStruct();
	LineStruct(const LineStruct& L);
	~LineStruct();
	LineStruct& operator = (const LineStruct& L);

	void SetPos(const Pixel& p1, const Pixel& p2);
	void SetPos(const LatLong& L1, const LatLong& L2)
	{
		aPos[0]=L1;
		aPos[1]=L2;
	}

	void CalcBounds();
	void Draw(HDC DC, const TColor BackColor);
	void DrawRect(HDC DC);
	void Store(JIniFile* pFil, const int n);
	void Store(JIniFile* pFil, const String sName);
	void Read(JIniFile* pFil, const int n);
	void Read(JIniFile* pFil, const String sName);
	bool bEdit(const bool bDefault, const int nSide=0, const bool bShowDel=true);
	void OpenEdit(const int nSide=0);
	NM_DIALOG_STATE CloseEdit();
	int nInside(const Pixel Pix);

	void ClearSegments()
	{
		SegPosList.Clear();
		SegPixList.Clear();
   }

   void AddSeg(const LatLong& LL)
   {
      LatLong* pLL=new LatLong;
      *pLL=LL;
      Pixel* pPix=new Pixel;
      pPix->Invalidate();
      SegPosList.nAdd(pLL);
      SegPosList.Pack();
      SegPixList.nAdd(pPix);
      SegPixList.Pack();
   }

   int nGetSegNum() const
   {
      return SegPosList.nGetCount();
   }

   bool bMultiSeg() const
   {
      return bMulti;
   }

   LatLong GetSegPos(const int n)
   {
      return *SegPosList[n];
   }

   void SetSegPos(const int n, const LatLong& LL)
   {
      *SegPosList[n]=LL;
   }

   void SetSegPix(const int n, const Pixel& p)
   {
      *SegPixList[n]=p;
   }

};

//********************************************************************************

struct ArcStruct
{
   LatLong Pos;
   int nFwdRa;
   int nAftRa;
   double dCenterBe;
   double dArcWidth;
   String sDescr;
   TColor FrameColor;
   int nFrameWidth;
   bool bFill;
   TColor FillColor;
   bool bDeleted;
   bool bDisplay;
   int nCircleDia;
   bool bShowDot;

   enum ARC_TYPE
   {
      FOOTPRINT_ARC,
      USER_ARC,
      CIRCLE_ARC,
      ROSE_ARC,
   } ArcType;

   JList<LatLong> SegList;


private:

   POINT* pPoly;
   int nPoints;
   Pixel PixPos;
   int nCirclePix;

#ifdef __DDGRAPH
   int nCalcArc(const XYPos& Center, const double dRadius, const double dStartAngle, const double dStopAngle, POINT* pPoint, const int nMaxPoints, DDGraph* pG);
#else
   int nCalcArc(const XYPos& Center, const double dRadius, const double dStartAngle, const double dStopAngle, POINT* pPoint, const int nMaxPoints, TransGraph* pG);
#endif

   double dSqr(const double dV) const
   {
      return dV*dV;
   }

   void SetData(const bool bDefault);
   bool bGetData();

public:

   ArcStruct();
	ArcStruct(const ArcStruct& A);
	ArcStruct& operator = (const ArcStruct& A);
	~ArcStruct();

#ifdef __DDGRAPH
	void CalcPoly(DDGraph* pG);
#else
	void CalcPoly(TransGraph* pG);
#endif

   void Draw(HDC DC, const TColor BackColor);
   void Store(JIniFile* pFil, const int n);
   void Read(JIniFile* pFil, const int n);
   void Store(JIniFile* pFil, const String sName);
   void Read(JIniFile* pFil, const String sName);
   bool bEdit(const bool bDefault=false, const bool bShowDel=true);
   void OpenEdit();
   NM_DIALOG_STATE CloseEdit();

};

//********************************************************************************

struct ManVectStruct
{
   SYSTEMTIME DT;
   double dSpeed;
   int nCourse;
   String sShip;
   LatLong aPos[2];
   Pixel aPixPos[2];
   Pixel IDPixPos;
   TColor Color;
   TColor IDColor;
   int nWidth;
   TFont* pFont;
   bool bDisplay;

private:

   void Init();

public:

   ManVectStruct();
   ManVectStruct(const ManVectStruct& M);
   ~ManVectStruct();
	ManVectStruct& operator = (const ManVectStruct& M);

	void Store(JIniFile* pFil, const int n);
	void Read(JIniFile* pFil, const int n);
	void SetPos(const Pixel& p1, const Pixel& p2);
	void Draw(HDC& DC, const int nID);

};

//********************************************************************************

class NavAidClass
{

   JList<LineStruct> LineList;
   JList<SymbolStruct> SymbolList;
   JList<ArcStruct> ArcList;
   JList<ManVectStruct> VectList;
   SymbolStruct RefSymbol;
   int nCurrentLine;
   int nCurrentSymbol;
   int nCurrentArc;
	int nLineDragNum;
	int nSymDragNum;
   int nRefSymbol;
   Pixel DragPix;
   int nDragOrigin,nDragPoint;

public:

   NavAidClass();
   ~NavAidClass();

	NavAidClass& operator = (const NavAidClass& N);

	NavAidClass(const NavAidClass& N)
	{
		*this=N;
	}

	void Clear();

	bool bRead(const String sName);
	bool bRead(const String sName, LatLong& ZoomCenter, SizeD& ZoomSize);
	void Store(const String sName, const LatLong& ZoomCenter, const SizeD& ZoomSize);

	void Store(JIniFile* pFil);
	void Read(JIniFile* pFil);

	void DrawAll(HDC DC, const TColor BackColor);
   NAV_HOTSPOT HotSpot(const Pixel& Pix);

   bool bAvail()
   {
      return (nLines()>0)||(nSymbols()>0)||(nArcs()>0)||(nVectors()>0);
   }

#ifdef __DDGRAPH
   void TransformPos(HDC DC, DDGraph* pG);
#else
   void TransformPos(HDC DC, TransGraph* pG);
#endif

   void Pack();

   bool bEditAll();
   void OpenEdit();
   NM_DIALOG_STATE CloseEdit();

//Lines *******************************************************

   int nNew(LineStruct& Default);

   void Add(LineStruct* pL)
   {
      LineList.nAdd(pL);
      LineList.Pack();
      nCurrentLine=LineList.nGetCount()-1;
   }

   void AddLine(const Pixel& p1, const Pixel& p2, const LatLong& LL1, const LatLong& LL2, LineStruct& Default);
   void SetLine(const Pixel& p1, const Pixel& p2, const LatLong& LL1, const LatLong& LL2);
   void DeleteLine(const int n);
   
   LineStruct* pGetLine(const int n)
   {
      return LineList[n];
   }

   LineStruct* pGetCurrentLine()
   {
      if (nCurrentLine>=0)
         return LineList[nCurrentLine];
      else
         return NULL;
   }

   bool bEditLine(const int nSide, int n=-1, const bool bShowDel=true);

   void ExchangeLines(const int n1, const int n2)
   {
      LineList.Exchange(n1,n2);
   }

   bool bLineLocked()
   {
      return LineList[nCurrentLine]->bLocked;
   }

   Pixel GetLinePix(const int nSide)
   {
      return LineList[nCurrentLine]->aPixPos[nSide];
   }

   int nLines()
   {
      return LineList.nGetCount();
   }

   void DrawLines(HDC DC, const TColor BackColor);

   int nLine() const
	{
      return nCurrentLine;
   }

   void SetLineDrag(const bool bDrag)
   {
		if (bDrag)
			nLineDragNum=nCurrentLine;
		else
			{
			LineList[nLineDragNum]->CalcBounds();
			nLineDragNum=-1;
         }
   }

	void DragLineStart(const Pixel& Pix, const LatLong& LL);

   void DragLineStart(const int n)
   {
      nDragPoint=n;
      if (n==0) nDragOrigin=1; else nDragOrigin=0;
   }

   void DragLine(HDC DC, const Pixel& Pix, const LatLong& LL, const bool bDrawPrev=true);

   void PackLines();

// Symbols *******************************************************

   void Add(SymbolStruct* pS)
   {
      SymbolList.nAdd(pS);
      SymbolList.Pack();
      nCurrentSymbol=SymbolList.nGetCount()-1;
   }

   void DeleteSymbol(const int n);

   SymbolStruct* pGetSymbol(const int n)
   {
      return SymbolList[n];
   }

   SymbolStruct* pGetCurrentSymbol()
   {
      if (nCurrentSymbol>=0)
         return SymbolList[nCurrentSymbol];
      else
         return NULL;
   }

   void ExchangeSymbols(const int n1, const int n2)
   {
      SymbolList.Exchange(n1,n2);
   }

   Pixel GetSymbolPix()
   {
      return SymbolList[nCurrentSymbol]->PixPos;
   }

   void DrawSymbolRect(HDC DC, const Pixel& Pix, const LatLong& LL);
   bool bEditSymbol(int n=-1, const bool bShowDel=true);

   bool bSymbolLocked()
   {
      return SymbolList[nCurrentSymbol]->bLocked;
   }

   bool bRefValid() const
   {
      return (nRefSymbol>=0);
   }

//   void SetSymbolRef(const bool bSetRef);

   LatLong GetRefLL()
   {
      if (nRefSymbol>=0)
         return SymbolList[nRefSymbol]->Pos;
      else
         return LatLong(); //Invalidated
   }

   int nSymbols()
   {
      return SymbolList.nGetCount();
   }

   void CalcSymbolSize(HDC DC);
	void CalcSymbolBounds();

   void DrawSymbols(HDC DC, const TColor BackColor);

   int nSymbol() const
   {
      return nCurrentSymbol;
   }

   void SetSymbolDrag(const bool bDrag)
   {
      if (bDrag)
         {
         nSymDragNum=nCurrentSymbol;
         DragPix.nx=-10000;
         }
      else
         nSymDragNum=-1;
   }

   void DragSymbol(HDC DC, const Pixel& Pix, const LatLong& LL, const bool bDrawPrev=true);

   void PackSymbols();

// Arcs *********************************************************

   void DrawArcs(HDC DC, const TColor BackColor);

   int nArcs() const
   {
      return ArcList.nGetCount();
   }

   void Add(ArcStruct* pA)
   {
      ArcList.nAdd(pA);
      ArcList.Pack();
      nCurrentArc=ArcList.nGetCount()-1;
   }

   void DeleteArc(const int n)
   {
      ArcList.Delete(n);
      ArcList.Pack();
   }

   ArcStruct* pGetArc(const int n)
   {
      return ArcList[n];
   }

   ArcStruct* pGetCurrentArc()
   {
      return ArcList[nCurrentArc];
   }

   int nArc() const
   {
      return nCurrentArc;
   }

   bool bEditArc(const int n, const bool bShowDel=true);

   void ExchangeArcs(const int n1, const int n2)
   {
      ArcList.Exchange(n1,n2);
   }

   void PackArcs();

//Manual Vectors ************************************************

   void DrawVectors(HDC DC);

   int nVectors()
   {
      return VectList.nGetCount();
   }

   void Add(ManVectStruct* pV)
   {
      VectList.nAdd(pV);
      VectList.Pack();
   }


   void DeleteVector(const int n)
   {
      VectList.Delete(n);
      VectList.Pack();
   }


   ManVectStruct* pGetVector(const int n)
   {
      return VectList[n];
   }


   void ExchangeVectors(const int n1, const int n2)
   {
      VectList.Exchange(n1,n2);
   }

   void ClearVectors()
   {
      VectList.Clear();
   }

   void ExportVectors(const String sFile);

// Reference Symbol

   SymbolStruct* pGetRef();
   void SetRef(const LatLong& LL);
   void ClearRef();
   bool bRefSet() const;
#ifdef __DDGRAPH
   Polar GetRefPol(const Pixel& Pix, DDGraph* pG);
#else
   Polar GetRefPol(const Pixel& Pix, TransGraph* pG);
#endif

   void DrawRef(HDC DC, const TColor BackColor);

};

COLORREF g_GetColor(const TColor Color);


#endif
