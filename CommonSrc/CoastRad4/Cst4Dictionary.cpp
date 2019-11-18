//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4Dictionary.h"
#include "JCommaText.h"
#include "JFile.h"
#include "JUtils.h"
#include "JReadExcel.h"
#include "JAES.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4Dictionary g_Dict;

Cst4Dictionary::Cst4Dictionary(const bool bStoreInvalid)
{
	Language=CST4_LANG_ENGLISH;
	bReadDict();
	StoreInvalid(bStoreInvalid);
}

Cst4Dictionary::Cst4Dictionary()
{
	Language=CST4_LANG_ENGLISH;
	WordList.Clear();
	StoreInvalid(false);
}

Cst4Dictionary::~Cst4Dictionary()
{
	if (InvalidList.nGetCount()>0)
		{
		InvalidList.Sort();
		JFile Fil('O',JFile::ASCII_TYPE);
		String sName="c:\\CoastRad4 Shared\\Not_In_Dictionary.txt";
		JFile::FILE_ERROR E=Fil.Create(sName);
		InvalidList.GoFirst();
		String s;
		while(!InvalidList.bLast() && !E)
			{
			s=*InvalidList.pNext();
			bool bFound=false;
			for (int i=0; (i<WordList.nGetCount()) && !bFound; i++)
				bFound=(s==WordList[i]->asLang[Language]);
			if (!bFound)
				E=Fil.TextLine(s);
			}
		}
}

void Cst4Dictionary::StoreInvalid(const bool bStore)
{
	bStoreInvalid=bStore;
	if (bStoreInvalid)
		{
		InvalidList.Clear();
		String sName="c:\\CoastRad4 Shared\\Not_In_Dictionary.txt";
		if (bFileExist(sName))
			{
			JFile Fil('I',JFile::ASCII_TYPE);
			JFile::FILE_ERROR E=Fil.Open(sName);
			String s;
			while(!E)
				{
				E=Fil.Read(s);
				if ((!E)&&(!s.IsEmpty()))
					{
					String* pS=new String;
					*pS=s;
					InvalidList.nAdd(pS);
					}
				}
			}
		}
}

bool Cst4Dictionary::bCreateFile(const String sExcelFile, const String sDictFile)
{
	JReadExcel Exc(sExcelFile);
	if (Exc.bOK())
		{
		WordList.Clear();
		int nRow=2;	//Skip first row
		int nEmptyCnt=0;
		String sEng,sPort,sFrench,sSpan,sSwahili;
		while(nEmptyCnt<5)
			{
			sEng=Exc.sCell(nRow,1).Trim();
			if (sEng.IsEmpty())
				++nEmptyCnt;
			else
				{
				nEmptyCnt=0;
				sPort=Exc.sCell(nRow,2);
				sFrench=Exc.sCell(nRow,3);
				sSpan=Exc.sCell(nRow,4);
				sSwahili=Exc.sCell(nRow,4);
				WordList.nAdd(new EntryStruct(sEng,sPort,sFrench,sSpan,sSwahili));
				}
			++nRow;
			}
		if (WordList.nGetCount()>0)
			{
			WordList.Sort();
			for (int i=0; i<256; i++)
				{
				anFirstIndex[i]=-1;
				anLastIndex[i]=-1;
				}
			for (int j=0; j<WordList.nGetCount(); j++)
				{
				int n=WordList[j]->nFirstChar;
				if (n>=0)
					{
					if (anFirstIndex[n]<0)
						anFirstIndex[n]=j;
					anLastIndex[n]=j;
					}
				}
			return bStoreDict(sDictFile);
			}
		}
	return false;
}

bool Cst4Dictionary::bStoreDict(const String sDictFile)
{
	WordList.Pack();
	int nCnt=WordList.nGetCount();
	JFile Fil('O');
	JFile::FILE_ERROR E=Fil.Create(sDictFile);
	int nVer=CST4_DICT_VERSION;
	if (!E)
		E=Fil.Write(&nVer,sizeof(nVer));
	if (!E)
		E=Fil.Write(anFirstIndex,sizeof(anFirstIndex));
	if (!E)
		E=Fil.Write(anLastIndex,sizeof(anLastIndex));
	if (!E)
		E=Fil.Write(&nCnt,sizeof(nCnt));
	for (int i=0; (i<nCnt) && (!E); i++)
		E=WordList[i]->Write(&Fil);
	if (!E)
		E=Fil.Close();
	if ((!E)&&(nCnt>0))
		{
		JFile F('B');
		E=F.Open(sDictFile);
		int n=(int)F.dwGetSize();
		if ((!E)&&(n>0))
			{
			BYTE* pucBuf=new BYTE[n+31];
			E=F.Read(0,pucBuf,n);
			if (!E)
				{
				JAES AES("MacKenzie");
				n=AES.nEncrypt(&pucBuf[sizeof(DWORD)],n-sizeof(DWORD));	//Skip version
				E=F.Write(0,pucBuf,n+sizeof(DWORD));
				}
			delete[] pucBuf;
			}
		}


	return (E==JFile::F_NO_ERROR)&&(nCnt>0);
}

bool Cst4Dictionary::bReadDict()
{
	WordList.Clear();
	bool bOK=false;
	String sDict;
//	if (Personality==CST4_COASTRAD_PERS)
//		sDict="c:\\CoastRad4 Shared\\CoastRad4.Cst4Dict";
//	else
		sDict="c:\\IMT Software\\VistaNet Shared\\VistaNet.VistaDict";
	if (!bFileExist(sDict))
		sDict="";
	JMemFile Fil;
	if (!sDict.IsEmpty())
		{
		JFile::FILE_ERROR E=Fil.ReadBuffer(sDict);
		if (!E)
			{
			int nVer;
			bOK=Fil.bRead(&nVer,sizeof(nVer));
			if ((bOK)&&(nVer!=CST4_DICT_VERSION))
				{
				ShowError("Wanted Version "+String(CST4_DICT_VERSION)+", Found Version "+String(nVer),"Incorrect Dictionary Version");
				bOK=false;
				}

			if ((bOK)&&(nVer>1000))
				{
				JAES AES("MacKenzie");
				BYTE* pucBuf=Fil.pucGetBuffer();
				int nSize=Fil.dwGetSize();
				nSize=AES.nDecrypt(&pucBuf[sizeof(DWORD)],nSize-sizeof(DWORD));
				if (nSize>0)
					bOK=true;	//No Decryption Error
				}
			}
		}
	if (bOK)
		{
		int nVer,nCnt;
//		if (bOK)
//			bOK=Fil.bRead(&nVer,sizeof(nVer));
		if (bOK)
			bOK=Fil.bRead(anFirstIndex,sizeof(anFirstIndex));
		if (bOK)
			bOK=Fil.bRead(anLastIndex,sizeof(anLastIndex));
		if (bOK)
			bOK=Fil.bRead(&nCnt,sizeof(nCnt));
		for (int i=0; (i<nCnt) && (bOK); i++)
			{
			EntryStruct* pE=new EntryStruct;
			bOK=pE->bRead(&Fil);
			if (!bOK)
				delete pE;
			else
				WordList.nAdd(pE);
			}
		WordList.Pack();
		}
	return (WordList.nGetCount()>0);
}

String Cst4Dictionary::sTranslate(const String sEnglish)
{
	if ((WordList.nGetCount()>0)&&(Language!=CST4_LANG_ENGLISH))
		{
		if (sEnglish.Length()>0)
			{
			String s=sEnglish.UpperCase();
			s=s.Trim();
			int nFirstChar=s[1];
			int nStart=anFirstIndex[nFirstChar];
			int nStop=anLastIndex[nFirstChar];
			if ((nStart>=0)&&(nStop>=0))
				{
				for (int i=nStart; i<=nStop; i++)
					{
					if (s==WordList[i]->asLang[CST4_LANG_ENGLISH])
						return WordList[i]->asLang[Language];
					}
				}
			if (bStoreInvalid)
				AddInvalid(sEnglish);
			}
		}
	return sEnglish;
}

void Cst4Dictionary::AddInvalid(const String s)
{
	String* pS;
	bool bFound=false;
	InvalidList.GoFirst();
	while(!InvalidList.bLast() && !bFound)
		bFound=(*InvalidList.pNext()==s);
	if (!bFound)
		{
		String* pS=new String;
		*pS=s;
		InvalidList.nAdd(pS);
		}
}

void Cst4Dictionary::Translate(TLabel* pLab)
{
	String s=sTranslate(pLab->Caption);
	if (!s.IsEmpty())
		pLab->Caption=s;
}

void Cst4Dictionary::Translate(TForm* pF)
{
	String s=sTranslate(pF->Caption);
	if (!s.IsEmpty())
		pF->Caption=s;
}

void Cst4Dictionary::Translate(TButton* pBut)
{
	String s=sTranslate(pBut->Caption);
	if (!s.IsEmpty())
		pBut->Caption=s;
}

void Cst4Dictionary::Translate(TCheckBox* pBox)
{
	String s=sTranslate(pBox->Caption);
	if (!s.IsEmpty())
		pBox->Caption=s;
}

void Cst4Dictionary::Translate(TGroupBox* pGrp)
{
	String s=sTranslate(pGrp->Caption);
	if (!s.IsEmpty())
		pGrp->Caption=s;
}

void Cst4Dictionary::Translate(TRadioButton* pRad)
{
	String s=sTranslate(pRad->Caption);
	if (!s.IsEmpty())
		pRad->Caption=s;
}

void Cst4Dictionary::Translate(TSpeedButton* pBut)
{
	if (Language!=CST4_LANG_ENGLISH)
		{
		String s=pBut->Hint;
/*
		int n=s.Pos("(Short");
		if (n>0)
			{
			s=s.SubString(1,n-1);
			}
*/
		s=sTranslate(s);
		if (!s.IsEmpty())
			pBut->Hint=s;
		}
}

void Cst4Dictionary::Translate(TPanel* pPan)
{
	String s;
	if (!pPan->Caption.IsEmpty())
		{
		s=sTranslate(pPan->Caption);
		if (!s.IsEmpty())
			pPan->Caption=s;
		}
	if (pPan->ShowHint && !pPan->Hint.IsEmpty())
		{
		s=sTranslate(pPan->Hint);
		if (!s.IsEmpty())
			pPan->Hint=s;
		}
}

void Cst4Dictionary::Translate(TMenuItem* pItem)
{

	AnsiString s1=pItem->Caption;
	if (s1.Length()>0)
		{
		AnsiString as="";
		for (int i=1; i<=s1.Length(); i++)
			{
			if (s1[i]!='&')
				as+=s1[i];
			}
		as=sTranslate(as);
		if (!as.IsEmpty())
			pItem->Caption=as;
		}
}

void Cst4Dictionary::Translate(TComboBox* pCB)
{
	int n=pCB->ItemIndex;
	String s;
	for (int i=0; i<pCB->Items->Count; i++)
		{
		s=sTranslate(pCB->Items->Strings[i]);
		if (!s.IsEmpty())
			pCB->Items->Strings[i]=s;
		}
	pCB->ItemIndex=n;
}

void Cst4Dictionary::Translate(TListBox* pLB)
{
	int n=pLB->ItemIndex;
	String s;
	for (int i=0; i<pLB->Items->Count; i++)
		{
		s=sTranslate(pLB->Items->Strings[i]);
		if (!s.IsEmpty())
			pLB->Items->Strings[i]=s;
		}
	pLB->ItemIndex=n;
}

void Cst4Dictionary::Translate(TCheckListBox* pLB)
{
	bool abChecked[1000];
	for (int i=0; i<pLB->Items->Count; i++)
		abChecked[i]=pLB->Checked[i];
	String s;
	for (int i=0; i<pLB->Items->Count; i++)
		{
		s=sTranslate(pLB->Items->Strings[i]);
		if (!s.IsEmpty())
			pLB->Items->Strings[i]=s;
		}
	for (int i=0; i<pLB->Items->Count; i++)
		pLB->Checked[i]=abChecked[i];
}

void Cst4Dictionary::Translate(TPageControl* pPage)
{
	String s;
	for (int i=0; i<pPage->PageCount; i++)
		{
		s=sTranslate(pPage->Pages[i]->Caption);
		if (!s.IsEmpty())
			pPage->Pages[i]->Caption=s;
		}
}





