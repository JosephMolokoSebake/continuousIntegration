//---------------------------------------------------------------------------
#ifndef JMessageH
#define JMessageH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TMyMessage : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TLabel *Label;
private:	// User declarations
public:		// User declarations

   __fastcall TMyMessage(TComponent* Owner);

   void __fastcall SetText(const String& sTxt)
   {
      Label->Caption=sTxt;
      Application->ProcessMessages();
   }

   void __fastcall SetText(const String& sTxt1, const String& sTxt2)
   {
      Label->Caption=sTxt1+sTxt2;
      Application->ProcessMessages();
   }

   void __fastcall SetText(const String& sTxt1, const String& sTxt2, const String& sTxt3)
   {
      Label->Caption=sTxt1+sTxt2+sTxt3;
      Application->ProcessMessages();
   }

};
//---------------------------------------------------------------------------
extern PACKAGE TMyMessage *MyMessage;
//---------------------------------------------------------------------------
#endif
