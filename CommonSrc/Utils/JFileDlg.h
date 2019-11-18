//---------------------------------------------------------------------------

#ifndef JFileDlgH
#define JFileDlgH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>

#include "JList.h"
#include "JFile.h"
#include "JUtils.h"

//---------------------------------------------------------------------------
class TJFileForm : public TForm
{
__published:	// IDE-managed Components
   TEdit *FileEd;
   TBitBtn *OKBut;
   TBitBtn *CancelBut;
   TStringGrid *FileGrid;
   TComboBox *MaskCombo;
   TSpeedButton *TreeBut;
   TLabel *NameLab;
   TLabel *TypeLab;
   TLabel *Label3;
   TSpeedButton *RootBut;
   TComboBox *DirCombo;
   TSpeedButton *FavBut;
   TListBox *FavList;
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall RootButClick(TObject *Sender);
   void __fastcall TreeButClick(TObject *Sender);
   void __fastcall FileGridClick(TObject *Sender);
   void __fastcall FileGridDblClick(TObject *Sender);
   void __fastcall DirComboChange(TObject *Sender);
   void __fastcall MaskComboChange(TObject *Sender);
   void __fastcall FileGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FavButClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FavListDblClick(TObject *Sender);
   void __fastcall DirComboClick(TObject *Sender);
   void __fastcall FormClick(TObject *Sender);
   void __fastcall FileEdClick(TObject *Sender);
   void __fastcall MaskComboClick(TObject *Sender);
   void __fastcall NameLabClick(TObject *Sender);
   void __fastcall TypeLabClick(TObject *Sender);
   void __fastcall OKButClick(TObject *Sender);
   void __fastcall FileEdChange(TObject *Sender);
   void __fastcall CancelButClick(TObject *Sender);
private:	// User declarations


   enum SORT_TYPE
      {
      NO_SORT,
      DATE_DOWN_SORT,
      DATE_UP_SORT,
      NAME_DOWN_SORT,
      NAME_UP_SORT,
      EXT_DOWN_SORT,
      EXT_UP_SORT,
      SIZE_DOWN_SORT,
      SIZE_UP_SORT
      };

   struct DirEntryStruct
   {
      String sFile;
      String sExt;
      DWORD dwSize;
      SYSTEMTIME T;
      FILETIME FT;
      bool bDir;
      bool bSel;
   	TJFileForm::SORT_TYPE Sort;

      DirEntryStruct& operator = (const DirEntryStruct& D)
      {
         sFile=D.sFile;
         sExt=D.sExt;
         dwSize=D.dwSize;
         T=D.T;
         FT=D.FT;
         bDir=D.bDir;
         bSel=D.bSel;
         Sort=D.Sort;
         return *this;
      }

      bool operator < (const DirEntryStruct& D) const
      {
         switch(Sort)
            {
            case DATE_DOWN_SORT:
               return ::CompareFileTime(&FT,&D.FT)<0;
            case DATE_UP_SORT:
               return ::CompareFileTime(&FT,&D.FT)>0;
            case NAME_DOWN_SORT:
               return sFile.Compare(D.sFile)<0;
            case NAME_UP_SORT:
               return sFile.Compare(D.sFile)>0;
            case EXT_DOWN_SORT:
               return sExt.Compare(D.sExt)<0;
            case EXT_UP_SORT:
               return sExt.Compare(D.sExt)>0;
            case SIZE_DOWN_SORT:
               if (dwSize==D.dwSize)
                  return true;
               else if (dwSize>D.dwSize)
                  return false;
               else
                  return true;
            case SIZE_UP_SORT:
               if (dwSize==D.dwSize)
                  return true;
               else if (dwSize>D.dwSize)
                  return true;
               else
                  return false;
            default:
               return false;
            }
      }

      bool operator > (const DirEntryStruct& D) const
      {
         switch(Sort)
            {
            case DATE_DOWN_SORT:
               return ::CompareFileTime(&FT,&D.FT)>0;
            case DATE_UP_SORT:
               return ::CompareFileTime(&FT,&D.FT)<0;
            case NAME_DOWN_SORT:
               return sFile.Compare(D.sFile)>0;
            case NAME_UP_SORT:
               return sFile.Compare(D.sFile)<0;
            case EXT_DOWN_SORT:
               return sExt.Compare(D.sExt)>0;
            case EXT_UP_SORT:
               return sExt.Compare(D.sExt)<0;
            case SIZE_DOWN_SORT:
               if (dwSize==D.dwSize)
                  return true;
               else if (dwSize<D.dwSize)
                  return false;
               else
                  return true;
            case SIZE_UP_SORT:
               if (dwSize==D.dwSize)
                  return true;
               else if (dwSize<D.dwSize)
                  return true;
               else
                  return false;
            default:
               return false;
            }
      }

   };

   JList<DirEntryStruct> DirEntryList;
   JList<DirEntryStruct> FileEntryList;

   TStringList* pDirHistory;
   TStringList* pFav;

   struct MaskStruct
   {
      String sDescr;
      String sMask;

      MaskStruct()
      {
         sDescr=L"";
         sMask=L"";
      }

      MaskStruct(const String sD, const String sM);

   };

   JList<MaskStruct> MaskList;


	SORT_TYPE Sort;

   String sCurrentMask;
   int nLastDirRow;
   bool bTopRow;
   bool bCheckExist;

   void __fastcall ReadDir();
   void __fastcall ShowEntry(const DirEntryStruct* pE, const int nR);
   void __fastcall ShowFiles(const bool bSetRow=true);
//   String __fastcall sSelectDir();
   bool __fastcall bGetDirInfo(const WIN32_FIND_DATA& FileData, DirEntryStruct* pE);
   void __fastcall ShowDirCombo(const String sDir);
   void __fastcall UpdateDirHistory(const String sDir);

   void __fastcall StoreSettings();
   void __fastcall ReadSettings();
   void __fastcall ShowHeading();
   bool __fastcall bSelValid();
	void __fastcall SortFiles(const bool bPreserveSel);

	String sGet(const String s);


//   static int __fastcall SortFunction(void *a, void *b);

public:		// User declarations
   __fastcall TJFileForm(TComponent* Owner);


   String sDir;
   String sFileName;
   String sTitle;
   int nFilterIndex;
   NM_DIALOG_STATE DlgState;

   void __fastcall AddMask(const String sDescr, const String sMask);

   String __fastcall sGetMask();

   void __fastcall CheckExist()
   {
      bCheckExist=true;
   }

};

String sSelectOpenFile(const String sMaskDescr, const String sMask, const String sTitle="", const String sFolder="");
String sSelectSaveFile(const String sMaskDescr, const String sMask, const String sTitle="", const String sFolder="");
String sSelectSaveFileDef(const String sMaskDescr, const String sMask, const String sTitle, const String sDefaultFile);

//---------------------------------------------------------------------------
extern PACKAGE TJFileForm *JFileForm;
//---------------------------------------------------------------------------
#endif
