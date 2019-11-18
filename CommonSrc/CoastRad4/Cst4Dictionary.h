//---------------------------------------------------------------------------
#ifndef Cst4DictionaryH
#define Cst4DictionaryH
//---------------------------------------------------------------------------

#include <Vcl.Buttons.hpp>
#include <CheckLst.hpp>

#include "JList.h"
#include "JMemFile.h"
#include "Cst4Defs.h"

enum CST4_LANGUAGE
{
	CST4_LANG_ENGLISH,
	CST4_LANG_PORTUGESE,
	CST4_LANG_FRENCH,
	CST4_LANG_SPANISH,
	CST4_LANG_SWAHILI,
	CST4_LANG_EXTRA0,
	CST4_LANG_EXTRA1,
	CST4_LANG_EXTRA2,
	CST4_LANG_EXTRA3,
	CST4_LANG_EXTRA4,
	CST4_LANG_EXTRA5,
	CST4_LANG_EXTRA6,
	CST4_LANG_EXTRA7,
	CST4_LANG_COUNT,
};

enum
{
	CST4_DICT_VERSION	= 1010,
};

class Cst4Dictionary
{

	struct EntryStruct
	{
		String asLang[CST4_LANG_COUNT];
		int nFirstChar;

		EntryStruct()
		{
			nFirstChar=-1;
		}

		EntryStruct(const String sEng, const String sPort, const String sFrench="", const String sSpanish="", const String sSwahili="")
		{
			String sE=sEng.UpperCase();
			asLang[CST4_LANG_ENGLISH]=sE;
			asLang[CST4_LANG_PORTUGESE]=sPort;
			asLang[CST4_LANG_FRENCH]=sFrench;
			asLang[CST4_LANG_SPANISH]=sSpanish;
			asLang[CST4_LANG_SWAHILI]=sSwahili;
			if (sEng.Length()>0)
				nFirstChar=sE[1];
			else
				nFirstChar=-1;
		}

		bool operator < (EntryStruct& E) const
		{
			return (nFirstChar<E.nFirstChar);
		}

		bool operator > (EntryStruct& E) const
		{
			return (nFirstChar>E.nFirstChar);
		}

		JFile::FILE_ERROR Write(JFile* pFil)
		{
			JFile::FILE_ERROR E=pFil->Write(&nFirstChar,sizeof(nFirstChar));
			BYTE aucBuf[1024];
			for (int i=0; (i<CST4_LANG_COUNT) && (!E); i++)
				{
				int nLen=asLang[i].Length();
				AnsiString s1=AnsiString(asLang[i]);
				for (int j=0; j<nLen; j++)
					aucBuf[j]=s1[j+1]^0xAA;
				E=pFil->Write(&nLen,sizeof(nLen));
				if ((!E)&&(nLen>0))
					E=pFil->Write(aucBuf,nLen);
				}
			return E;
		}

		bool bRead(JMemFile* pFil)
		{
			bool bOK=pFil->bRead(&nFirstChar,sizeof(nFirstChar));
			char acBuf[1024];
			for (int i=0; (i<CST4_LANG_COUNT) && (bOK); i++)
				{
				int nLen;
				bOK=pFil->bRead(&nLen,sizeof(nLen));
				if ((bOK)&&(nLen>0))
					{
					bOK=pFil->bRead(acBuf,nLen);
					if (bOK)
						{
						for (int j=0; j<nLen; j++)
							acBuf[j]^=0xAA;
						acBuf[nLen]=0;
						asLang[i]=String(acBuf);
						}
					}
				else
					asLang[i]="";
				}
			return bOK;
		}


	};

	CST4_LANGUAGE Language;

	JList<EntryStruct> WordList;
	int anFirstIndex[256];
	int anLastIndex[256];

	JList<String> InvalidList;
	bool bStoreInvalid;

	bool bStoreDict(const String sDictFile);

	void AddInvalid(const String s);



public:

	Cst4Dictionary(const bool _bStoreInvalid);
	Cst4Dictionary();
	~Cst4Dictionary();

	void StoreInvalid(const bool bStore);

	bool bCreateFile(const String sExcelFile, const String sDictFile);

	bool bReadDict();

	void SetLanguage(const CST4_LANGUAGE _Language)
	{
		Language=_Language;
	}

	bool bOK()
	{
		return WordList.nGetCount()>0;
   }

	String sTranslate(const String sEnglish);

	void Translate(TForm* pF);
	void Translate(TLabel* pLab);
	void Translate(TButton* pBut);
	void Translate(TCheckBox* pBox);
	void Translate(TGroupBox* pGrp);
	void Translate(TRadioButton* pRad);
	void Translate(TSpeedButton* pBut);
	void Translate(TPanel* pPan);
	void Translate(TMenuItem* pItem);
	void Translate(TComboBox* pCB);
	void Translate(TListBox* pLB);
	void Translate(TCheckListBox* pLB);
	void Translate(TPageControl* pPage);


};

extern Cst4Dictionary g_Dict;

#endif
