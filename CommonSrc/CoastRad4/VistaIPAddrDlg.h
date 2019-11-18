//---------------------------------------------------------------------------

#ifndef VistaIPAddrDlgH
#define VistaIPAddrDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#include "JList.h"

//---------------------------------------------------------------------------
class TIPAddrForm : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TEdit *DescrEd;
	TLabel *Label2;
	TEdit *AddrEd;
	TLabel *Label1;
	TListBox *LB;
	TButton *DeleteBut;
	TButton *AddBut;
	TButton *UseBut;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall AddButClick(TObject *Sender);
	void __fastcall DeleteButClick(TObject *Sender);
	void __fastcall UseButClick(TObject *Sender);
	void __fastcall LBDblClick(TObject *Sender);
private:	// User declarations

	struct AddrStruct
	{
		String sAddr;
		String sDescr;

		bool operator < (const AddrStruct& A)
		{
			return sDescr<A.sDescr;
		}

		bool operator > (const AddrStruct& A)
		{
			return sDescr>A.sDescr;
		}


	};

   JList<AddrStruct> AddrList;

   bool bChanged;

public:		// User declarations
	__fastcall TIPAddrForm(TComponent* Owner);

   String sAddr;

};
//---------------------------------------------------------------------------
extern PACKAGE TIPAddrForm *IPAddrForm;
//---------------------------------------------------------------------------
#endif

void SelectVistaAddr(TComponent* pOwner, TEdit* pEd);
