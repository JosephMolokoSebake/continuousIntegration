//---------------------------------------------------------------------------

#ifndef AISExtInfoDlgH
#define AISExtInfoDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TAISExtInfoForm : public TForm
{
__published:	// IDE-managed Components
   TStringGrid *Grid;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

   BYTE* pucInfo;
   int nSize;
   String sCargo;

public:		// User declarations
   __fastcall TAISExtInfoForm(TComponent* Owner);


   void SetInfo(const BYTE* _pucInfo, const int _nSize)
   {
      if (_nSize>0)
         {
         nSize=_nSize;
         delete[] pucInfo;
         pucInfo=new BYTE[nSize];
         memcpy(pucInfo,_pucInfo,nSize);
         }
   }

   void SetCargo(const String _sCargo)
   {
      sCargo=_sCargo;
   }

};
//---------------------------------------------------------------------------
extern PACKAGE TAISExtInfoForm *AISExtInfoForm;
//---------------------------------------------------------------------------
#endif
