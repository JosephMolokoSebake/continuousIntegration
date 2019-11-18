//---------------------------------------------------------------------------
#ifndef NavSiteH
#define NavSiteH
//---------------------------------------------------------------------------
#include "D2DNavAid.h"
#include "JList.h"
#include "Cst4Defs.h"

struct NavSiteStruct : public SymbolStruct
{

	int nHeight_m;
	bool bDrawCircle;
	bool bAutoRa;
	double dMaxRa_km;
	Pixel Radius_pix;
	TColor LineCol;
	int nLineWidth;
	bool bFill;
	TColor FillCol;
	WORD wID;

	NavSiteStruct()
	{
		nHeight_m=0;
		bDrawCircle=false;
		bAutoRa=false;
		dMaxRa_km=1.0;
      Radius_pix=Pixel(1,1);
		LineCol=clLtGray;
		nLineWidth=1;
		bFill=true;
		FillCol=clGreen;
		wID=0;
		SymbolStruct::Init();

		pFont->Name="Arial";
		pFont->Size=8;
		pFont->Color=TColor(33023);
		bDispSymbol=true;
		Symbol=CIRCLE_SYM;
		nSymbolSize=3;
		SymbolColor=TColor(33023);
		TextAlign=SymbolStruct::AL_TOP;
		bSolidFill=true;
		bDispText=true;
		bSolidBack=false;
		bLocked=false;
		bRef=false;
		bDisplay=true;
	}

	NavSiteStruct(const NavSiteStruct& S)
	{
		*this=S;
	}

	NavSiteStruct& operator = (const NavSiteStruct& S)
	{

		SymbolStruct::operator =(S);
		nHeight_m=S.nHeight_m;
		bDrawCircle=S.bDrawCircle;
		bAutoRa=S.bAutoRa;
		dMaxRa_km=S.dMaxRa_km;
      Radius_pix=S.Radius_pix;
		LineCol=S.LineCol;
		nLineWidth=S.nLineWidth;
		bFill=S.bFill;
		FillCol=S.FillCol;
		wID=S.wID;
		return *this;
	}

	~NavSiteStruct()
	{

	}

	void CalcPix(TransGraph* pG);

	void DrawSite(TransGraph* pG, const TColor BackColor);
	void DrawArea(TransGraph* pG);

	void Store(JIniFile* pFil, const int n);
	void Store(JIniFile* pFil, const String sName);
	void Read(JIniFile* pFil, const int n);
	void Read(JIniFile* pFil, const String sName);

};

class NavSites
{

	JList<NavSiteStruct> Site;

public:

	NavSites();
	~NavSites();

	NavSites& operator = (const NavSites& S)
	{
		Site=S.Site;
		return *this;
	}

	void Edit(TForm* pOwner);

	void Store(const String sFile);
	void Read(const String sFile);

	void Store(JIniFile* pIni);
	void Read(JIniFile* pIni);

	void CalcPix(TransGraph* pG);

	void DrawSites(TransGraph* pG, const TColor BackColor);
	void DrawAreas(TransGraph* pG);

   void Refresh(const WORD wID, const String sDescr, const LatLong&& LL, const int nHeight_m);


};



#endif
