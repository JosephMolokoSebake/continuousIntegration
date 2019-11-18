//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JReadExcel.h"
#include "JUtils.h"
#include "JFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


JReadExcel::JReadExcel(const String sFile, const String sSheet)
{
   bValid=false;
   bXLS=false;
   vXL=Unassigned;
   nRowCnt=0;
	nColCnt=0;

	nSectionCol=-1;
	nSectionRow1=-1;
	nSectionRow2=-1;
	sSection="";
	nParDescrCol=-1;
	nParValCol=-1;

   CSV.Clear();
   if (bFileExist(sFile))
      {
      if (sFileExtOnly(sFile).UpperCase().Pos(L"XLS")>0)
			OpenXLS(sFile,sSheet);
      else
         OpenCSV(sFile);
		}
}

JReadExcel::~JReadExcel()
{
   if (bXLS)
      {
      vXL.OleProcedure(L"Quit");
      vXL=Unassigned;
      }
}

void JReadExcel::OpenXLS(const String sFile, const String sSheet)
{
   bXLS=true;
   bValid=true;
   try
      {
      vXL=Variant::CreateObject(L"excel.application");
		}
   catch(...)
      {
		ShowError(L"Could Not Open Excel Application",L"Excel Error");
		bXLS=false;
      bValid=false;
      }
   if (bValid)
      {
      try
			{
         vXL.OlePropertySet(L"Visible",false);
         }
      catch(...)
         {
			ShowError(L"Could Not Hide Excel Application",L"Excel Error");
			bValid=false;
         }
		}
   if (bValid)
		{
		try
			{
			vXL.OlePropertyGet(L"Workbooks").OleProcedure(L"Open",sFile.c_str());
			}
		catch(...)
			{
			ShowError(L"Could Not Open Excel File: "+sFile,L"Excel Error");
			bValid=false;
			}
		}
	if (bValid)
		{
		try
			{
			vSheet=vXL.OlePropertyGet(L"Sheets",AnsiString(sSheet).c_str());
			}
      catch(...)
         {
         ShowError(L"Invalid Excel Worksheet: "+sSheet,L"Excel Error");
         bValid=false;
         }
      }
   if (bValid)
      {
      try
         {
         vCells=vSheet.OlePropertyGet(L"Cells");
         }
      catch(...)
         {
         ShowError(L"Error Opening Worksheet Cells",L"Excel Error");
         bValid=false;
         }
      }
}

void JReadExcel::OpenCSV(const String sFile)
{
   JFile Fil('I',JFile::ASCII_TYPE);
   JFile::FILE_ERROR E=Fil.Open(sFile);
   char szSt[8192];
   while(!E)
      {
      E=Fil.Read(szSt,8192);
      if (!E)
         {
         CommaText* pTxt=new CommaText(szSt);
         if (pTxt->nGetCount()>nColCnt)
            nColCnt=pTxt->nGetCount();
         nRowCnt=CSV.nAdd(pTxt);
         }
      }
   CSV.Pack();
   bValid=(nRowCnt>0)&&(nColCnt>0);
}

String JReadExcel::sGetCell(const int nRow, const int nCol)
{
   int nC=nCol;
   if (nC>='a')
      nC-='a'-1;
   else if (nC>='A')
      nC-='A'-1;
   String s=L"";
   if (bValid)
      {
      if (!bXLS)
         {
			if (nRow<=nRowCnt)
				if (nC<=CSV[nRow-1]->nGetCount())
					s=CSV[nRow-1]->sGetPar(nC-1);
			}
		else
			{
			try
				{
				//Get the content of the Cell
				vCell=vCells.OlePropertyGet(L"Item",nRow,nC);
				//store that content
				s=vCell.OlePropertyGet(L"Value");
				}
			catch(...)
            {
            ShowError(L"Error Reading Worksheet Cell : Row "+String(nRow)+" Col "+String(nC),L"Excel Error");
            s=L"";
            }
         }
      }
   return s;
}

String JReadExcel::sCell(const int nRow, const int nCol)
{
   return sGetCell(nRow,nCol);
}

double JReadExcel::dCell(const int nRow, const int nCol)
{
   try
      {
      String s=sCell(nRow,nCol).Trim();
      if (s.IsEmpty())
         return 0.0;
      else
         return dAltToDouble(s);
      }
   catch(...)
      {
      return 0.0;
      }
}

int JReadExcel::nCell(const int nRow, const int nCol)
{
   try
      {
      String s=sCell(nRow,nCol).Trim();
      if (s.IsEmpty())
         return 0;
      else
         return s.ToInt();
      }
	catch(...)
      {
      return 0;
      }
}

int JReadExcel::bCell(const int nRow, const int nCol)
{
	String s=sCell(nRow,nCol).UpperCase().Trim();
	return (s.Pos(L"TRUE")>0)||(s.Pos(L"1")>0);
}

bool JReadExcel::bEmptyRow(const int nRow)
{
	for (int nCol=1; nCol<10; nCol++)
		{
		if (!sGetCell(nRow,nCol).IsEmpty())
			return false;
		}
	return true;
}

bool JReadExcel::bFindSection()
{
	if ((nSectionRow1<=0)||(nSectionRow2<=0))
		{
		int nRows;
		if (bXLS)
			{
			//Find rows in Excel document
			int nEmptyRows=0;
			nRows=1;
			while((nEmptyRows<5)&&(nRows<1000))
				{
				if (bEmptyRow(nRows))
					++nEmptyRows;
				++nRows;
				}
			}
		else
			nRows=nRowCnt;
		if (nRows>0)
			{
			for (int nRow=1; nRow<nRows; nRow++)
				{
				String s=sGetCell(nRow,nSectionCol).Trim().UpperCase();
				if (!s.IsEmpty())
					{
					if (s==sSection)
						nSectionRow1=nRow;
					else if (nSectionRow1>0)
						{
						nSectionRow2=nRow-1;
						return true;
                  }
               }
				}
			if (nSectionRow1>0)
				{
				nSectionRow2=nRows-1;
				return true;
				}
			}
		return false;
		}
	else
		return true;
}

bool JReadExcel::bPar(const String sParDescr, String& sVal)
{
	if (bFindSection())
		{
		String sP=sParDescr.Trim().UpperCase();
		for (int nRow=nSectionRow1; nRow<=nSectionRow2; nRow++)
			{
			String s=sGetCell(nRow,nParDescrCol).Trim().UpperCase();
			if (s==sP)
				{
				sVal=sCell(nRow,nParValCol);
				return true;
            }
         }
		}
	return false;
}

bool JReadExcel::bPar(const String sParDescr, double& dVal)
{
	if (bFindSection())
		{
		String sP=sParDescr.Trim().UpperCase();
		for (int nRow=nSectionRow1; nRow<=nSectionRow2; nRow++)
			{
			String s=sGetCell(nRow,nParDescrCol).Trim().UpperCase();
			if (s==sP)
				{
				dVal=dCell(nRow,nParValCol);
				return true;
				}
			}

		}
	return false;
}

bool JReadExcel::bPar(const String sParDescr, int& nVal)
{
	if (bFindSection())
		{
		String sP=sParDescr.Trim().UpperCase();
		for (int nRow=nSectionRow1; nRow<=nSectionRow2; nRow++)
			{
			String s=sGetCell(nRow,nParDescrCol).Trim().UpperCase();
			if (s==sP)
				{
				nVal=nCell(nRow,nParValCol);
				return true;
				}
			}
		}
	return false;
}

bool JReadExcel::bPar(const String sParDescr, bool& bVal)
{
	if (bFindSection())
		{
		String sP=sParDescr.Trim().UpperCase();
		for (int nRow=nSectionRow1; nRow<=nSectionRow2; nRow++)
			{
			String s=sGetCell(nRow,nParDescrCol).Trim().UpperCase();
			if (s==sP)
				{
				bVal=bCell(nRow,nParValCol);
				return true;
				}
			}
		}
	return false;
}


