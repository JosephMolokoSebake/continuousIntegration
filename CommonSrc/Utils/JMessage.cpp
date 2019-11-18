/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	Utility Message
 *    FILE NAME		Message.cpp
 *		PROJECT			Virtue IV/C0564
 *		ACTIVITY       6
 * 	COMPILER       Borland C++ Builder 3
 *		PROCESSOR      Pentium
 *		OS					MS Windows 95/NT4
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *    	General message utility.
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 25/09/1998
 *       V1.10    Use on B++ Builder 5 
 ************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "JMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMyMessage *MyMessage;
//---------------------------------------------------------------------------
__fastcall TMyMessage::TMyMessage(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
