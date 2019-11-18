//---------------------------------------------------------------------------
#ifndef NavSiteH
#define NavSiteH
//---------------------------------------------------------------------------
#include "NavAid.h"
#include "JList.h"
#include "Cst4Defs.h"

struct NavSiteStruct
{
	String sDescr;
   bool bDispSite;
   bool bDispName;
   bool bDispSymbol;
	LatLong LL;

	int nHeight_m;
	bool bDrawCircle;
	bool bAutoRa;
	double dMaxRa_km;
	TRect R;
	TColor LineCol;
	int nLineWidth;
	bool bFill;
	TColor FillCol;
	int nID;

   Pixel PixPos;

	NavSiteStruct();

	NavSiteStruct(const NavSiteStruct& S)
	{
		*this=S;
	}

	NavSiteStruct& operator = (const NavSiteStruct& S)
	{
      sDescr=S.sDescr;
      bDispSite=S.bDispSite;
      bDispName=S.bDispName;
      bDispSymbol=S.bDispSymbol;
      LL=S.LL;
		nHeight_m=S.nHeight_m;
		bDrawCircle=S.bDrawCircle;
		bAutoRa=S.bAutoRa;
		dMaxRa_km=S.dMaxRa_km;
		R=S.R;
		LineCol=S.LineCol;
		nLineWidth=S.nLineWidth;
		bFill=S.bFill;
		FillCol=S.FillCol;
		nID=S.nID;
      PixPos=S.PixPos;
		return *this;
	}

	~NavSiteStruct()
	{

	}

#ifdef __DDGRAPH
	void CalcPix(DDGraph* pG);
#else
	void CalcPix(TransGraph* pG);
#endif

	void DrawSite(HDC DC, const int nSymSize, const TColor BackColor);
	void DrawArea(HDC DC);

	void Store(JIniFile* pFil, const int n);
	void Store(JIniFile* pFil, const String sName);
	void Read(JIniFile* pFil, const int n);
	void Read(JIniFile* pFil, const String sName);

};

class NavSites
{

	TFont* pFont;
   int nSymbolSize;
   TColor SymbolColor;

	JList<NavSiteStruct> Site;

public:

	NavSites();
	~NavSites();

	NavSites& operator = (const NavSites& S)
	{
		pFont->Assign(S.pFont);
   	nSymbolSize=S.nSymbolSize;
   	SymbolColor=S.SymbolColor;
		Site=S.Site;
		return *this;
	}

	void Edit(TForm* pOwner);

	void Store(const String sFile);
	void Read(const String sFile);

	void Store(JIniFile* pIni);
	void Read(JIniFile* pIni);

#ifdef __DDGRAPH
	void CalcPix(DDGraph* pG);
#else
	void CalcPix(TransGraph* pG);
#endif

	void DrawSites(HDC DC, const TColor BackColor);
	void DrawAreas(HDC DC);

   void Refresh(const int nID, const String sDescr, const LatLong&& LL, const int nHeight_m);


};



#endif
