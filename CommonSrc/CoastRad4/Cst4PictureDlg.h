//---------------------------------------------------------------------------

#ifndef Cst4PictureDlgH
#define Cst4PictureDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "JList.h"
#include "JTime.h"

#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TCst4PictureForm : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *PB;
	TPanel *Panel1;
	TButton *ResetBut;
	TButton *ReadBut;
	TButton *SaveBut;
	TStatusBar *StatusBar;
	TComboBox *FileCB;
	TSpeedButton *ZoomInBut;
	TSpeedButton *ZoomOutBut;
	TButton *DeleteBut;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall PBMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall PBMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall PBMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ResetButClick(TObject *Sender);
	void __fastcall ReadButClick(TObject *Sender);
	void __fastcall SaveButClick(TObject *Sender);
	void __fastcall FileCBChange(TObject *Sender);
	void __fastcall ZoomInButClick(TObject *Sender);
	void __fastcall ZoomOutButClick(TObject *Sender);
	void __fastcall DeleteButClick(TObject *Sender);


private:	// User declarations

	TRect DrawRect;
	bool bDragging;
	TPoint StartPoint;
	TPoint DrawOffs;
	TPoint StartOffs;
	double dDrawScale;
	bool bValid;
	Graphics::TBitmap* pBM;

   struct PictStruct
   {
   	String sFile;
      String sDescr;
      JTime T;

      bool operator < (const PictStruct& P) const
      {
      	return (P.T.dwTotalSec()<T.dwTotalSec());
      }

      bool operator > (const PictStruct& P) const
      {
     	return (P.T.dwTotalSec()>T.dwTotalSec());
       }


   };

	JList<PictStruct> PictList;

   void ShowPicture(const int n);


public:		// User declarations
	__fastcall TCst4PictureForm(TComponent* Owner);

   void AddPicture(const String sFile, const String sDescr, const JTime& T);

};
//---------------------------------------------------------------------------
extern PACKAGE TCst4PictureForm *Cst4PictureForm;
//---------------------------------------------------------------------------
#endif
