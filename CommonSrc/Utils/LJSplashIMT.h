//---------------------------------------------------------------------------

#ifndef LJSplashIMTH
#define LJSplashIMTH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------

void ShowIMTSplash(const int nDelay_sec=3, const String sTitle="");


class TSplashForm : public TForm
{
__published:	// IDE-managed Components
	TImage *imageSplash;
	TTimer *timerSplash;
	TLabel *labelCaption;
	TLabel *labelVer;
	TLabel *labelRef;
	TLabel *labelCopy;
	TPaintBox *PB;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall timerSplashTimer(TObject *Sender);
	void __fastcall imageSplashClick(TObject *Sender);
	void __fastcall PBPaint(TObject *Sender);
	void __fastcall PBClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
	int nSplashTime;
	String sSplashFile;
	int nMonitor;
	String sTitle;
public:		// User declarations
	__fastcall TSplashForm(TComponent* Owner);
	void SetSplashFile(const String _sSplashFile);
	void SetSplashTime(const int _nSplashTime)
	{
		nSplashTime=_nSplashTime;
	}

	void SetMonitor(const int _nMonitor)
	{
		nMonitor=_nMonitor;
	}

	void SetTitle(const String s)
	{
		sTitle=s;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
