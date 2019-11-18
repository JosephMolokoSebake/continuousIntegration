//---------------------------------------------------------------------------

#ifndef JReadExcelH
#define JReadExcelH
//---------------------------------------------------------------------------

#include "JCommaText.h"
#include "JList.h"

class JReadExcel
{

/*
	TExcelApplication Excel;
	ExcelWorkbook Book;
	ExcelWorksheet Sheet;
*/


	Variant vXL,vSheet,vCells,vCell;
   bool bValid;
   bool bXLS;

   JList<CommaText> CSV;
	int nRowCnt,nColCnt;

	int nSectionCol;
	int nSectionRow1;
	int nSectionRow2;
	String sSection;
	int nParDescrCol;
	int nParValCol;

	void OpenXLS(const String sFile, const String sSheet);
	void OpenCSV(const String sFile);


	String sGetCell(const int nRow, const int nCol);    //Row/Col start at 1 !!!

	bool bEmptyRow(const int nRow);
	bool bFindSection();

public:

	JReadExcel(const String sFile, const String sSheet="Sheet1");
	~JReadExcel();

	bool bOK() const
	{
		return bValid;
	}

	//NB Row/Col start at 1
	//NB Col can also be 'A','B', etc

	String sCell(const int nRow, const int nCol);
	double dCell(const int nRow, const int nCol);
	int nCell(const int nRow, const int nCol);
	int bCell(const int nRow, const int nCol);

	void SetSection(const int _nCol, const String _sSect, const int _nParDescrCol, const int _nParValCol)
	{
		nSectionCol=_nCol;
		sSection=_sSect.Trim().UpperCase();
		nParDescrCol=_nParDescrCol;
		nParValCol=_nParValCol;
		nSectionRow1=-1;
		nSectionRow2=-1;
	}

	bool bPar(const String sParDescr, String& sVal);
	bool bPar(const String sParDescr, double& dVal);
	bool bPar(const String sParDescr, int& nVal);
	bool bPar(const String sParDescr, bool& bVal);

};

#endif
