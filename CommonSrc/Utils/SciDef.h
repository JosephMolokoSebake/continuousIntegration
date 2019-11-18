//---------------------------------------------------------------------------

#ifndef SciDefH
#define SciDefH
//---------------------------------------------------------------------------

#include "JIniFile.h"
#include "JList.h"
#include "JFile.h"

enum SCIDEF_AXIS_TYPE
{
   SCIDEF_X_AXIS           = 0,
   SCIDEF_Y1_AXIS          = 1,
   SCIDEF_Y2_AXIS          = 2,
   SCIDEF_INVALID_AXIS     = 3,
};

enum SCIDEF_TEXT_ALIGN_TYPE
{
   SCIDEF_TOP_ALIGN        = 0x01,
   SCIDEF_MIDDLE_ALIGN     = 0x02,
   SCIDEF_BOTTOM_ALIGN     = 0x04,
   SCIDEF_LEFT_ALIGN       = 0x08,
   SCIDEF_CENTER_ALIGN     = 0x10,
   SCIDEF_RIGHT_ALIGN      = 0x20,
};

enum SCIDEF_SYMBOL_TYPE
{
   SCIDEF_CIRCLE_SYM       = 0,
   SCIDEF_SQUARE_SYM       = 1,
   SCIDEF_CROSS_SYM        = 2,
   SCIDEF_V_DIAMOND_SYM    = 3,
   SCIDEF_H_DIAMOND_SYM    = 4,
   SCIDEF_X_SYM            = 5,
   SCIDEF_U_ARROW_SYM      = 6,
   SCIDEF_D_ARROW_SYM      = 7,
   SCIDEF_L_ARROW_SYM      = 8,
   SCIDEF_R_ARROW_SYM      = 9,
   SCIDEF_NO_SYM           = 10,
   SCIDEF_INVALID_SYM      = 11,
};

enum SCIDEF_COORDINATE_TYPE
{
   SCIDEF_CARTESIAN     = 0,
   SCIDEF_POLAR         = 1,
};

enum SCIDEF_TRANSFORM_TYPE
{
   SCIDEF_LINEAR_TRANS  = 0,
   SCIDEF_LOG_TRANS     = 1,
};

enum SCIDEF_LINE_STYLE
{
   SCIDEF_SOLID_STYLE               = 0,
   SCIDEF_DASH_STYLE                = 1,
   SCIDEF_DOT_STYLE                 = 2,
   SCIDEF_DASH_DOT_STYLE            = 3,
   SCIDEF_DASH_DOT_DOT_STYLE        = 4,
   SCIDEF_DASH_DOT_DOT_DOT_STYLE    = 5,
   SCIDEF_DASH_DASH_DOT_STYLE       = 6,
   SCIDEF_DASH_DASH_DOT_DOT_STYLE   = 7,
   SCIDEF_INVALID_STYLE             = 8,
};

enum SCIDEF_NUMBER_TYPE
{
   SCIDEF_INT_NUM    = 0,
   SCIDEF_FLOAT_NUM  = 1,
   SCIDEF_SCI_NUM    = 2,
   SCIDEF_TIME_NUM   = 3,
};

enum SCIDEF_SECNUM_TYPE
{
   SCIDEF_NONE_SECNUM   = 0,
   SCIDEF_BASE_SECNUM   = 1,
   SCIDEF_FULL_SECNUM   = 2,
};

class SciDefDataFile
{

	struct DataFileValStruct
	{
		double* pdVal;

		DataFileValStruct()
		{
			pdVal=NULL;
		}
		DataFileValStruct(const int nYPar)
		{
			pdVal=new double[nYPar+1];
		}

		~DataFileValStruct()
		{
			delete[] pdVal;
		}

	};

	struct DataFileStruct
	{
		String sFile;
		bool bEncrypt;
		int nYPar;

		JList<DataFileValStruct> Values;


	} DataFile;

	double* pdMin;
	double* pdMax;

	int nRnd(const double dV) const
	{
		if (dV<0.0)
			return floor(dV);
		else
			return ceil(dV);
	}

	void GetSpan(double& dMin, double& dMax, int& nTicks, int& nDecimals);

public:

	SciDefDataFile(const String sBinFile, const int nYParCount, const bool bEncrypt=false);

	~SciDefDataFile();

	void AddPar( const double dX,
					 const double dY1,      const double dY2=0.0,
					 const double dY3=0.0,  const double dY4=0.0,
					 const double dY5=0.0,  const double dY6=0.0,
					 const double dY7=0.0,  const double dY8=0.0,
					 const double dY9=0.0,  const double dY10=0.0,
					 const double dY11=0.0, const double dY12=0.0,
					 const double dY13=0.0, const double dY14=0.0,
					 const double dY15=0.0, const double dY16=0.0);

	double dGetMin(const int nPar) const
	{
		return pdMin[nPar];
	}

	double dGetMax(const int nPar) const
	{
		return pdMax[nPar];
	}

	double dGetMinX() const
	{
		return pdMin[0];
	}

	double dGetMaxX() const
	{
		return pdMax[0];
	}

	double dGetMinY() const;
	double dGetMaxY() const;

	void GetXSpan(double& dMin, double& dMax, int& nTicks, int& nDecimals);
	void GetYSpan(double& dMin, double& dMax, int& nTicks, int& nDecimals);

};

class SciDef : public JIniFile
{

	JList<SciDefDataFile> DataFiles;

	struct DataLineStruct
   {
      String sDataFile;
      bool bTextFile;
      String sDescr;
      int nOffset;
      SCIDEF_AXIS_TYPE Axis;
      TColor Color;
      SCIDEF_LINE_STYLE LineStyle;
      int nStyleLength;
      int nLineWidth;
      SCIDEF_SYMBOL_TYPE Symbol;
      int nSymbolSize;
      bool bShowInKey;
   };

   JList<DataLineStruct> DataLines;

   struct LineStruct
   {
      TColor Color;
      int nWidth;
      int nHeadSize1;
      int nHeadSize2;
      double dX1;
      double dY1;
      double dX2;
      double dY2;

   };

   JList<LineStruct> Lines;

   struct LabelStruct
   {
      String sText;
      double dX,dY;
      int nAlign;
      TFont* pFont;
      int nFrameWidth;
      TColor FrameColor;
      TColor BackColor;

      LabelStruct()
      {
         pFont=NULL;
      }

      ~LabelStruct()
      {
         delete pFont;
      }

   };

   JList<LabelStruct> Labels;


   struct SymbolStruct
   {
      SCIDEF_SYMBOL_TYPE Symbol;
      double dX,dY;
      int nSize;
      bool bSolid;
      TColor Color;
   };

   JList<SymbolStruct> Symbols;

   struct PictureStruct
   {
      double dX,dY;
      int nAlign;
      double dSize;
      bool bTransparent;
      TColor TransColor;
      String sFile;
   };

   JList<PictureStruct> Pictures;

   String sAxis(const SCIDEF_AXIS_TYPE Axis);


public:

   enum
   {
      SCIDEF_VERSION =  2000,
   };

   SciDef(const String sName, const bool bEncrypt=false);
   ~SciDef();

   void SetReadOnly(const String sPW);

   void SetCoordinates(const SCIDEF_COORDINATE_TYPE Coord);
// SetFramePos dX/dY positions are in percentage of screen size
   void SetFramePos(const double dX1, const double dY1, const double dX2, const double dY2);
   void SetFrame(const int nFrameWidth, const TColor FrameColor, const bool bFill, const TColor FillColor);

   int nAddDataLine(const String sDataFile, const bool bTextFile, const String sDescr, const int nOffset, const SCIDEF_AXIS_TYPE Axis, const TColor Color);
   void SetDataLine(const int n, const SCIDEF_LINE_STYLE LineStyle, const int nStyleLength, const int nLineWidth, const SCIDEF_SYMBOL_TYPE Symbol=SCIDEF_NO_SYM, const int nSymbolSize=0);
   void SetDataLineKey(const int n, const bool bShowInKey);

   int nCreateBinDataFile(const String sBinFile, const int nYParCount, const bool bEncrypt=false);
   void AddDataFilePar( const int n, const double dX,
                        const double dY1,      const double dY2=0.0,
                        const double dY3=0.0,  const double dY4=0.0,
                        const double dY5=0.0,  const double dY6=0.0,
                        const double dY7=0.0,  const double dY8=0.0,
                        const double dY9=0.0,  const double dY10=0.0,
                        const double dY11=0.0, const double dY12=0.0,
                        const double dY13=0.0, const double dY14=0.0,
                        const double dY15=0.0, const double dY16=0.0);

   void SetAxis(const SCIDEF_AXIS_TYPE Axis, const String sLabel, const double dMin, const double dMax, const int nDecimals, const SCIDEF_NUMBER_TYPE NumType, const int nTicks);
   void SetAxisLabelFont(const SCIDEF_AXIS_TYPE Axis, TFont* pFont);
   void SetAxisLabelFont(const SCIDEF_AXIS_TYPE Axis, const String sFontName, const int nSize, const TColor Color);
   void SetAxisTransform(const SCIDEF_AXIS_TYPE Axis, const SCIDEF_TRANSFORM_TYPE Transform);
   void SetAxisMainTicks(const SCIDEF_AXIS_TYPE Axis, const int nTickNum, const int nTickSize);
   void SetAxisMainGrid(const SCIDEF_AXIS_TYPE Axis, SCIDEF_LINE_STYLE LineStyle, const int nLineWidth, const TColor Color);
   void SetAxisSecTicks(const SCIDEF_AXIS_TYPE Axis, const int nTickNum, const int nTickSize);
   void SetAxisSecGrid(const SCIDEF_AXIS_TYPE Axis, SCIDEF_LINE_STYLE LineStyle, const int nLineWidth, const TColor Color);
   void SetAxisNumberFont(const SCIDEF_AXIS_TYPE Axis, TFont* pFont);
   void SetAxisNumberFont(const SCIDEF_AXIS_TYPE Axis, const String sFontName, const int nSize, const TColor Color);

   void SetHeadings(const String sMain, const String sSec=L"");
   void SetHeadingFont(TFont* pMainFont, TFont* pSecFont=NULL);
   void SetMainHeadingFont(const String sFontName, const int nSize, const TColor Color);
   void SetSecHeadingFont(const String sFontName, const int nSize, const TColor Color);

// AddLine dX/dY positions are in percentage of frame size
   void AddLine(const TColor Color, const int nWidth, const int nHeadSize1, const int nHeadSize2, const double dX1, const double dY1, const double dX2, const double dY2);

// AddLabel dX/dY positions are in percentage of frame size
   int nAddLabel(const String sText, const double dX, const double dY, const int nAlign=SCIDEF_TOP_ALIGN|SCIDEF_LEFT_ALIGN);
   void SetLabelFont(const int n, TFont* pFont);
   void SetLabelFont(const int n, const String sFontName, const int nSize, const TColor Color);
   void SetLabelFrame(const int n, const int nFrameWidth, const TColor FrameColor, const TColor BackColor);

// SetKey dX/dY positions are in percentage of frame size
   void SetKey(const int nFrameWidth, const TColor FrameColor, const TColor BackColor, const double dX, const double dY);
   void SetKeyFont(TFont* pFont);
   void SetKeyFont(const String sFontName, const int nSize, const TColor Color);

// AddSymbol dX/dY positions are in percentage of frame size
   void AddSymbol(const SCIDEF_SYMBOL_TYPE Symbol, const TColor Color, const int nSize, const double dX, const double dY, const bool bSolid=false);

// AddPicture dX/dY positions are in percentage of frame size
   void AddPicture(const String sFile, const double dX, const double dY, const double dSize_percent, const int nAlign=SCIDEF_TOP_ALIGN|SCIDEF_LEFT_ALIGN, const bool bTransparent=false, const TColor TransColor=clNone);


};


#endif
