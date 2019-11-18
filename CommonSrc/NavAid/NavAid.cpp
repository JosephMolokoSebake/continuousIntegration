/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	General Navigational Aid Module
 *    FILE NAME		NavAid.cpp
 *		PROJECT
 *		ACTIVITY
 * 	COMPILER       Borland C++ Builder 5
 *		PROCESSOR      Pentium IV
 *		OS					MS Windows 2000
 *		PROGRAMMER     Johan Theron
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 03/12/2001
 ************************************************************************/
//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "NavAid.h"

#ifdef __COASTRAD4
   #include "CR4_Config.h"
#else
   #include "Config.h"
#endif

#include "NavSymbolDlg.h"
#include "NavLineDlg.h"
#include "NavReadDlg.h"
#include "NavArcDlg.h"
#include "NavEditDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

COLORREF g_GetColor(const TColor Color)
{
#ifdef __COASTRAD4
   return DispCfg.GetColor(Color);
#else
   return Config.GetColor(Color);
#endif
}

SymbolStruct::SymbolStruct()
{
   pFont=new TFont;
   pText=new TStringList;
   Init();
}

SymbolStruct::SymbolStruct(const SymbolStruct& S)
{
   pFont=new TFont;
   pText=new TStringList;
   *this=S;
}

SymbolStruct::~SymbolStruct()
{
   delete pText;
   delete pFont;
}

void SymbolStruct::Assign(const SymbolStruct& S)
{
	bDispSymbol=S.bDispSymbol;
	Symbol=S.Symbol;
	nSymbolSize=S.nSymbolSize;
	SymbolColor=S.SymbolColor;
	bSolidFill=S.bSolidFill;
	bDispText=S.bDispText;
	pFont->Assign(S.pFont);

	pText->Assign(S.pText);

//   sText=S.sText;
	TextAlign=S.TextAlign;
	bSolidBack=S.bSolidBack;
	bLocked=S.bLocked;
	bRef=S.bRef;
	Pos=S.Pos;
	PixPos=S.PixPos;
	Size=S.Size;
	Bounds=S.Bounds;
	bDeleted=S.bDeleted;
	bDisplay=S.bDisplay;
	FrameBackColor=S.FrameBackColor;
	bShowFrame=S.bShowFrame;
	FrameColor=S.FrameColor;
	nFrameWidth=S.nFrameWidth;
}

SymbolStruct& SymbolStruct::operator = (const SymbolStruct& S)
{
	Assign(S);
	return *this;
}

void SymbolStruct::Init()
{
	bDispText=false;
	bDispSymbol=false;
	Symbol=CIRCLE_SYM;
	bSolidFill=true;
	bSolidBack=false;
	Pos=LatLong(0.0,0.0);
	PixPos.Invalidate();
	nSymbolSize=10;
	SymbolColor=clAqua;
//   sText=L"";
   TextAlign=AL_TOPLEFT;
   bLocked=false;
   bRef=false;
   Size=SizeI(0,0);
   Bounds=TRect(0,0,0,0);
   bDeleted=false;
   pFont->Name=L"Arial";
   pFont->Size=16;
   pFont->Color=clAqua;
   bDisplay=true;
   FrameBackColor=clWhite;
   bShowFrame=false;
   FrameColor=clBlack;
   nFrameWidth=1;
   pText->Clear();
}

void SymbolStruct::Draw(HDC DC, const TColor BackColor)
{
   if ((bDispText)&&(bDisplay))
      {
      int nWeight;
      if (pFont->Style.Contains(fsBold))
         nWeight=FW_BOLD;
      else
         nWeight=FW_NORMAL;
      HFONT hFont=::CreateFont(-pFont->Size,	// logical height of font
                  0,	// logical average character width
                  0,	// angle of escapement
                  0,	// base-line orientation angle
                  nWeight,	// font weight
                  pFont->Style.Contains(fsItalic),	// italic attribute flag
                  pFont->Style.Contains(fsUnderline),	// underline attribute flag
                  pFont->Style.Contains(fsStrikeOut),	// strikeout attribute flag
                  DEFAULT_CHARSET,	// character set identifier
                  OUT_DEFAULT_PRECIS,	// output precision
						CLIP_DEFAULT_PRECIS,	// clipping precision
                  PROOF_QUALITY,	// output quality
                  DEFAULT_PITCH|FF_DONTCARE,	// pitch and family
						pFont->Name.c_str()); 	// pointer to typeface name string
      HFONT hOldFont=(HFONT)::SelectObject(DC,hFont);
      String ss;
      if (!hOldFont)
         {
         int nErr=GetLastError();
         ss=SysErrorMessage(nErr);
         }
//      ::SelectObject(DC,hFont);
      ::SetTextColor(DC,g_GetColor(pFont->Color));
      int nx=PixPos.nx;
      int ny=PixPos.ny;
      int nSymX=nSymbolSize;
      int nSymY=nSymbolSize;
      switch(TextAlign)
         {
         case AL_TOPLEFT:
            //Do Nothing
            break;
         case AL_TOP:
            nx-=Size.nW/2;
            nSymX=0;
            break;
         case AL_TOPRIGHT:
            nx-=Size.nW;
            nSymX=-nSymbolSize;
            break;
         case AL_LEFT:
            ny-=Size.nH/2;
            nSymY=0;
            break;
         case AL_RIGHT:
            nx-=Size.nW;
            ny-=Size.nH/2;
            nSymX=-nSymbolSize;
            nSymY=0;
            break;
         case AL_BOTLEFT:
            ny-=Size.nH;
            nSymY=-nSymbolSize;
            break;
         case AL_BOTRIGHT:
            nx-=Size.nW;
            ny-=Size.nH;
            nSymX=-nSymbolSize;
            nSymY=-nSymbolSize;
            break;
         case AL_BOTTOM:
            nx-=Size.nW/2;
            ny-=Size.nH;
            nSymX=0;
            nSymY=-nSymbolSize;
            break;
         case AL_CENTER:
            nx-=Size.nW/2;
            ny-=Size.nH/2;
            nSymX=0;
            nSymY=0;
            break;
         default: ;
         }
      if (bDispSymbol)
         {
         nx+=nSymX;
         ny+=nSymY;
         }
      COLORREF OldBack;
      int nOldMode;

      if (bShowFrame)
         {
         if (bSolidBack)
            {
            OldBack=::SetBkColor(DC,g_GetColor(FrameBackColor));
            HBRUSH hBrush=::CreateSolidBrush(g_GetColor(FrameBackColor));
            HPEN hPen=::CreatePen(PS_SOLID,nFrameWidth,g_GetColor(FrameColor));
            HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
            HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
            int nExtra=nFrameWidth;
            if (nExtra<2) nExtra=2;
            ::Rectangle(DC,nx-nExtra,ny-nExtra,nx+Size.nW+nExtra,ny+Size.nH+nExtra);
            ::SelectObject(DC,hOldBrush);
            ::SelectObject(DC,hOldPen);
            }
         else
            {
            HPEN hPen=::CreatePen(PS_SOLID,nFrameWidth,g_GetColor(FrameColor));
            HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
            int nExtra=nFrameWidth;
            if (nExtra<2) nExtra=2;
            ::MoveToEx(DC,nx-nExtra,ny-nExtra,NULL);
            ::LineTo(DC,nx+nExtra+Size.nW,ny-nExtra);
            ::LineTo(DC,nx+nExtra+Size.nW,ny+Size.nH+nExtra);
            ::LineTo(DC,nx-nExtra,ny+Size.nH+nExtra);
            ::LineTo(DC,nx-nExtra,ny-nExtra);
            ::SelectObject(DC,hOldPen);
            nOldMode=::SetBkMode(DC,TRANSPARENT);
            }
         }
      else
         {
         if (bSolidBack)
            OldBack=::SetBkColor(DC,g_GetColor(BackColor));
         else
            nOldMode=::SetBkMode(DC,TRANSPARENT);
         }
      int nyL=ny;
      String s;
      int nTextHeight;
      if (pText->Count>0)
         nTextHeight=Size.nH/pText->Count;
      for (int i=0; i<pText->Count; i++)
         {
         s=pText->Strings[i];
         ::TextOut(DC,nx,nyL,s.c_str(),s.Length());
         nyL+=nTextHeight;
         }
//      ::TextOut(DC,nx,ny,sText.c_str(),sText.Length());
      if (bSolidBack)
         SetBkColor(DC,OldBack);
      else
         ::SetBkMode(DC,nOldMode);
      if (!::SelectObject(DC,hOldFont))
         {
         int nErr=GetLastError();
         ss=SysErrorMessage(nErr);
         }
      ::DeleteObject(hFont);
      }
   if ((bDispSymbol)&&(bDisplay))
      {
      HPEN hPen=::CreatePen(PS_SOLID,1,g_GetColor(SymbolColor));
      HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
      HBRUSH hBrush;
      if (bSolidFill)
         hBrush=::CreateSolidBrush(g_GetColor(SymbolColor));
      else
         hBrush=(HBRUSH)::GetStockObject(NULL_BRUSH);
      HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
      switch(Symbol)
         {
         case CIRCLE_SYM:
            ::Ellipse(DC,PixPos.nx-nSymbolSize,PixPos.ny-nSymbolSize,PixPos.nx+nSymbolSize,PixPos.ny+nSymbolSize);
            break;
         case SQUARE_SYM:
            ::Rectangle(DC,PixPos.nx-nSymbolSize,PixPos.ny-nSymbolSize,PixPos.nx+nSymbolSize,PixPos.ny+nSymbolSize);
            break;
         case DIAMOND_SYM:
            MoveToEx(DC,PixPos.nx,PixPos.ny-nSymbolSize,NULL);
            LineTo(DC,PixPos.nx+nSymbolSize,PixPos.ny);
            LineTo(DC,PixPos.nx,PixPos.ny+nSymbolSize);
            LineTo(DC,PixPos.nx-nSymbolSize,PixPos.ny);
            LineTo(DC,PixPos.nx,PixPos.ny-nSymbolSize);
            break;
         case CROSS_SYM:
            MoveToEx(DC,PixPos.nx,PixPos.ny-nSymbolSize,NULL);
            LineTo(DC,PixPos.nx,PixPos.ny+nSymbolSize);
            MoveToEx(DC,PixPos.nx-nSymbolSize,PixPos.ny,NULL);
            LineTo(DC,PixPos.nx+nSymbolSize,PixPos.ny);
            break;
         default: ;
         }
      ::SelectObject(DC,hOldBrush);
      ::DeleteObject(hBrush);
      ::SelectObject(DC,hOldPen);
      ::DeleteObject(hPen);
      }
}

void SymbolStruct::DrawRect(HDC DC)
{
   if (PixPos.nx>=0)
      {
      ::MoveToEx(DC,Bounds.Left,Bounds.Top,NULL);
      ::LineTo(DC,Bounds.Right,Bounds.Top);
      ::LineTo(DC,Bounds.Right,Bounds.Bottom);
      ::LineTo(DC,Bounds.Left,Bounds.Bottom);
      ::LineTo(DC,Bounds.Left,Bounds.Top);
      }
}

void SymbolStruct::CalcBounds()
{
	Bounds.Left=PixPos.nx;
   Bounds.Top=PixPos.ny;
   if (bDispText)
      {
      int nx1=0,nx2=0,ny1=0,ny2=0;
      switch(TextAlign)
         {
         case AL_TOPLEFT:
            //Do Nothing
            nx1=nSymbolSize*2;
            ny1=nSymbolSize*2;
            break;
         case AL_TOP:
            Bounds.Left-=Size.nW/2;
            ny1=nSymbolSize*2;
            break;
         case AL_TOPRIGHT:
            Bounds.Left-=Size.nW;
            nx2=nSymbolSize*2;
            ny1=nSymbolSize*2;
            break;
         case AL_LEFT:
            Bounds.Top-=Size.nH/2;
            nx1=nSymbolSize*2;
            break;
         case AL_RIGHT:
            Bounds.Left-=Size.nW;
            Bounds.Top-=Size.nH/2;
            nx2=nSymbolSize*2;
            break;
         case AL_BOTLEFT:
            Bounds.Top-=Size.nH;
            nx1=nSymbolSize*2;
            ny2=nSymbolSize*2;
            break;
         case AL_BOTRIGHT:
				Bounds.Left-=Size.nW;
            Bounds.Top-=Size.nH;
            nx2=nSymbolSize*2;
            ny2=nSymbolSize*2;
            break;
         case AL_BOTTOM:
            Bounds.Left-=Size.nW/2;
            Bounds.Top-=Size.nH;
            ny2=nSymbolSize*2;
            break;
         case AL_CENTER:
            Bounds.Left-=Size.nW/2;
            Bounds.Top-=Size.nH/2;
            break;
         default: ;
         }
      Bounds.Right=Bounds.Left+Size.nW;
      Bounds.Bottom=Bounds.Top+Size.nH;
      if (bDispSymbol)
         {
         Bounds.Left-=nx1;
         Bounds.Right+=nx2;
         Bounds.Top-=ny1;
         Bounds.Bottom+=ny2;
         }
      }
   else
      {
      Bounds.Left-=nSymbolSize;
      Bounds.Top-=nSymbolSize;
      Bounds.Right=Bounds.Left+nSymbolSize*2;
      Bounds.Bottom=Bounds.Top+nSymbolSize*2;
      }
}

void SymbolStruct::SetPos(const Pixel& Pix)
{
   PixPos=Pix;
	CalcBounds();
}

void SymbolStruct::SetText(const String s)
{
   pText->Clear();
   pText->Add(s);
}

String SymbolStruct::sGetText() const
{
   if (pText->Count>0)
      return pText->Strings[0];
   else
      return "";
}


void SymbolStruct::Store(JIniFile* pFil, const String sName)
{
   pFil->Write(sName,L"Display Symbol",bDispSymbol);
   pFil->Write(L"Symbol Type",(int)Symbol);
   pFil->Write(L"Symbol Size",nSymbolSize);
   pFil->Write(L"Symbol Colour",SymbolColor);
   pFil->Write(L"Solid Symbol",bSolidFill);
   pFil->Write(L"Display Text",bDispText);
   pFil->Write(L"Font",pFont);
   pFil->Write(L"Text Lines",pText->Count);
   if (pText->Count>0)
      {
      pFil->Write(L"Text",String(L"#!#")+pText->Strings[0]+String(L"#!#"));
      if (pText->Count>1)
         {
         for (int i=1; i<pText->Count; i++)
            pFil->Write(L"Text Line "+String(i),String(L"#!#")+pText->Strings[i]+String(L"#!#"));
         }
      }
   else
      pFil->Write(L"Text",String(L""));
   pFil->Write(L"Alignment",(int)TextAlign);
   pFil->Write(L"Solid Text Background",bSolidBack);
   pFil->Write(L"Latitude",Pos.dLat);
   pFil->Write(L"Longitude",Pos.dLong);
   pFil->Write(L"Lock Position",bLocked);
   pFil->Write(L"Reference",bRef);
   pFil->Write(L"Display",bDisplay);
   pFil->Write(L"Background Colour",FrameBackColor);
   pFil->Write(L"Show Frame",bShowFrame);
   pFil->Write(L"Frame Colour",FrameColor);
   pFil->Write(L"Frame Width",nFrameWidth);
}

void SymbolStruct::Store(JIniFile* pFil, const int n)
{
   Store(pFil,L"SYMBOL "+String(n));
}

String SymbolStruct::sStripText(const String s1)
{
   String s=s1;
   if (s.Length()>3)
      {
      if (s.Pos(L"#!#")==1)
         {
         s=s.SubString(4,s.Length()-3);
         int n=s.Pos(L"#!#");
         if (n>0)
            s=s.SubString(1,s.Length()-3);
         }
      }
   else if (s.Length()==3)
      {
      if (s.Pos(L"#!#")==1)
         s=L"";
      }
   return s;
}

void SymbolStruct::Read(JIniFile* pFil, const String sName)
{
   pFil->Read(sName,L"Display Symbol",bDispSymbol,false);
   Symbol=(SYMBOL_TYPE)pFil->nRead(L"Symbol Type");
   pFil->Read(L"Symbol Size",nSymbolSize,10);
   pFil->Read(L"Symbol Colour",SymbolColor,clAqua);
   pFil->Read(L"Solid Symbol",bSolidFill,false);
   pFil->Read(L"Display Text",bDispText,false);
   pFil->Read(L"Font",pFont);

   int nLines=0;
   String s;
//   sText=L"";
   pText->Clear();
   pFil->Read(L"Text Lines",nLines,0);
   if (nLines>0)
      {
      pFil->Read(L"Text",s,String(L""));
      s=sStripText(s);
      pText->Add(s);
      if (nLines>1)
         {
         for (int i=1; i<nLines; i++)
            {
            pFil->Read(L"Text Line "+String(i),s,String(L""));
            s=sStripText(s);
            pText->Add(s);
            }
         }
      }
   else
      {
      pFil->Read(L"Text",s,String(L""));
      if (s!=L"")
         {
         s=sStripText(s);
         pText->Add(s);
         }
      }
   TextAlign=(ALIGN_TYPE)pFil->nRead(L"Alignment");
   pFil->Read(L"Solid Text Background",bSolidBack,false);
   pFil->Read(L"Latitude",Pos.dLat);
   pFil->Read(L"Longitude",Pos.dLong);
   pFil->Read(L"Lock Position",bLocked,false);
   pFil->Read(L"Reference",bRef,false);
   pFil->Read(L"Display",bDisplay,true);
   pFil->Read(L"Background Colour",FrameBackColor,clWhite);
   pFil->Read(L"Show Frame",bShowFrame,false);
   pFil->Read(L"Frame Colour",FrameColor,clBlack);
   pFil->Read(L"Frame Width",nFrameWidth,1);
}

void SymbolStruct::Read(JIniFile* pFil, const int n)
{
   Read(pFil,L"SYMBOL "+String(n));
}

void SymbolStruct::SetData(const bool bDefault, const bool bLockPos)
{
   NavSymbolForm->Ed.Init();
   NavSymbolForm->SymChk->Checked=bDispSymbol;
   NavSymbolForm->nSymbol=Symbol;   //Set parameter here as ItemIndex cannot be set yet
   NavSymbolForm->Ed.nSetVal(NavSymbolForm->SymSizeEd,nSymbolSize,"Symbol Size",1,99);
   NavSymbolForm->SymSizeUD->Position=nSymbolSize;
   NavSymbolForm->SymColPan->Color=SymbolColor;
   NavSymbolForm->SolidChk->Checked=bSolidFill;
   NavSymbolForm->Ed.nSetLL("Symbol Position",NavSymbolForm->LatEd,NavSymbolForm->LongEd,Pos);
   NavSymbolForm->LockChk->Checked=bLocked || bLockPos;
   NavSymbolForm->RefChk->Checked=bRef;
   NavSymbolForm->TextChk->Checked=bDispText;
   NavSymbolForm->TextMemo->Clear();
   for (int i=0; i<pText->Count; i++)
      NavSymbolForm->TextMemo->Lines->Add(pText->Strings[i]);
   NavSymbolForm->pFont->Assign(pFont);
   NavSymbolForm->SolidBackChk->Checked=bSolidBack;
   NavSymbolForm->BackColPan->Color=FrameBackColor;
   NavSymbolForm->ShowFrameChk->Checked=bShowFrame;
   NavSymbolForm->FrameColPan->Color=FrameColor;
   NavSymbolForm->Ed.nSetVal(NavSymbolForm->FrameWidthEd,nFrameWidth,"Frame Width",1,99);
   NavSymbolForm->FrameWidthUD->Position=nFrameWidth;
   NavSymbolForm->bDefault=bDefault;
   NavSymbolForm->TLRad->Checked=(TextAlign==SymbolStruct::AL_TOPLEFT);
   NavSymbolForm->TRad->Checked=(TextAlign==SymbolStruct::AL_TOP);
   NavSymbolForm->TRRad->Checked=(TextAlign==SymbolStruct::AL_TOPRIGHT);
   NavSymbolForm->LRad->Checked=(TextAlign==SymbolStruct::AL_LEFT);
   NavSymbolForm->RRad->Checked=(TextAlign==SymbolStruct::AL_RIGHT);
   NavSymbolForm->BLRad->Checked=(TextAlign==SymbolStruct::AL_BOTLEFT);
   NavSymbolForm->BRad->Checked=(TextAlign==SymbolStruct::AL_BOTTOM);
   NavSymbolForm->BRRad->Checked=(TextAlign==SymbolStruct::AL_BOTRIGHT);
   NavSymbolForm->CRad->Checked=(TextAlign==SymbolStruct::AL_CENTER);
   NavSymbolForm->DispChk->Checked=bDisplay;
}

bool SymbolStruct::bGetData()
{
   bDispSymbol=NavSymbolForm->SymChk->Checked;
   Symbol=(SymbolStruct::SYMBOL_TYPE)NavSymbolForm->SymCombo->ItemIndex;
//   NavSymbolForm->Ed.GetVal(NavSymbolForm->SymSizeEd,nSymbolSize);
   nSymbolSize=NavSymbolForm->SymSizeUD->Position;
   SymbolColor=NavSymbolForm->SymColPan->Color;
   bSolidFill=NavSymbolForm->SolidChk->Checked;
   bDispText=NavSymbolForm->TextChk->Checked;

   pText->Clear();
   for (int i=0; i<NavSymbolForm->TextMemo->Lines->Count; i++)
      pText->Add(NavSymbolForm->TextMemo->Lines->Strings[i]);
   bSolidBack=NavSymbolForm->SolidBackChk->Checked;
   FrameBackColor=NavSymbolForm->BackColPan->Color;
   bShowFrame=NavSymbolForm->ShowFrameChk->Checked;
   FrameColor=NavSymbolForm->FrameColPan->Color;
   nFrameWidth=NavSymbolForm->FrameWidthUD->Position;

   pFont->Assign(NavSymbolForm->pFont);
   NavSymbolForm->Ed.GetLL(NavSymbolForm->LatEd,NavSymbolForm->LongEd,Pos);
   bLocked=NavSymbolForm->LockChk->Checked;
   bRef=NavSymbolForm->RefChk->Checked;
   bDeleted=NavSymbolForm->bDelete;
   if (NavSymbolForm->TLRad->Checked)
      TextAlign=SymbolStruct::AL_TOPLEFT;
   else if (NavSymbolForm->TRad->Checked)
      TextAlign=SymbolStruct::AL_TOP;
   else if (NavSymbolForm->TRRad->Checked)
      TextAlign=SymbolStruct::AL_TOPRIGHT;
   else if (NavSymbolForm->LRad->Checked)
      TextAlign=SymbolStruct::AL_LEFT;
   else if (NavSymbolForm->RRad->Checked)
      TextAlign=SymbolStruct::AL_RIGHT;
   else if (NavSymbolForm->BLRad->Checked)
      TextAlign=SymbolStruct::AL_BOTLEFT;
   else if (NavSymbolForm->BRad->Checked)
      TextAlign=SymbolStruct::AL_BOTTOM;
   else if (NavSymbolForm->BRRad->Checked)
      TextAlign=SymbolStruct::AL_BOTRIGHT;
   else
      TextAlign=SymbolStruct::AL_CENTER;
   bDisplay=NavSymbolForm->DispChk->Checked;
   return NavSymbolForm->Ed.bValid();
}

bool SymbolStruct::bEdit(const bool bDefault, const bool bLockPos, const bool bShowDel)
{
   NavSymbolForm=new TNavSymbolForm(0);
   SetData(bDefault,bLockPos);
   NavSymbolForm->bShowDel=bShowDel&&(!bDefault);
   int nEd=NavSymbolForm->ShowModal();
   if (nEd==mrOk)
      bGetData();
   delete NavSymbolForm;
   return (nEd==mrOk);
}

bool SymbolStruct::bEditSiteSymbol()
{
	NavSymbolForm=new TNavSymbolForm(0);
	SetData(false,false);
	NavSymbolForm->bEditSite=true;
	NavSymbolForm->LockChk->Checked=false;
	NavSymbolForm->LockChk->Enabled=false;
	NavSymbolForm->TextMemo->Enabled=false;
	int nEd=NavSymbolForm->ShowModal();
   if (nEd==mrOk)
      bGetData();
   delete NavSymbolForm;
   return (nEd==mrOk);
}

void SymbolStruct::OpenEdit()
{
   NavSymbolForm=new TNavSymbolForm(0);
   NavSymbolForm->bSubEdit=false;
   NavSymbolForm->FormStyle=fsStayOnTop;
   SetData(false,false);
   NavSymbolForm->Show();
}

NM_DIALOG_STATE SymbolStruct::CloseEdit()
{
   NM_DIALOG_STATE State=NavSymbolForm->GetState();
   switch(State)
      {
      case NM_DIALOG_OK:
         bGetData();
         delete NavSymbolForm;
         break;
      case NM_DIALOG_CANCEL:
         delete NavSymbolForm;
         break;
      default: ;
      }
   return State;
}

String SymbolStruct::sSymbolType()
{
   switch(Symbol)
      {
      case CIRCLE_SYM:
         return "Circle";
      case SQUARE_SYM:
         return "Square";
      case DIAMOND_SYM:
         return "Diamond";
      case CROSS_SYM:
         return "Cross";
      default:
         return "Unknown";
      }
}

bool SymbolStruct::bInside(const Pixel Pix)
{
   if (bDeleted)
      return false;
   else
      return ((Pix.nx>=Bounds.Left)
            &&(Pix.nx<=Bounds.Right)
            &&(Pix.ny>=Bounds.Top)
            &&(Pix.ny<=Bounds.Bottom));
}

void SymbolStruct::CalcSize(HDC DC)
{
   if (bDispText)
      {
      int nWeight;
      if (pFont->Style.Contains(fsBold))
         nWeight=FW_BOLD;
      else
         nWeight=FW_DONTCARE;
		HFONT hFont=::CreateFont(-pFont->Size,	// logical height of font
						0,	// logical average character width
						0,	// angle of escapement
						0,	// base-line orientation angle
						nWeight,	// font weight
						pFont->Style.Contains(fsItalic),	// italic attribute flag
						pFont->Style.Contains(fsUnderline),	// underline attribute flag
						pFont->Style.Contains(fsStrikeOut),	// strikeout attribute flag
						DEFAULT_CHARSET,	// character set identifier
						OUT_DEFAULT_PRECIS,	// output precision
						CLIP_DEFAULT_PRECIS,	// clipping precision
						DEFAULT_QUALITY,	// output quality
						DEFAULT_PITCH,	// pitch and family
						pFont->Name.c_str()); 	   // pointer to typeface name string
      String s,ss;
      HFONT hOldFont=(HFONT)::SelectObject(DC,hFont);
      if (!hOldFont)
         {
         int nErr=GetLastError();
         ss=SysErrorMessage(nErr);
         }
      SIZE MaxSize;
      MaxSize.cx=0;
      MaxSize.cy=0;
      if (pText->Count>0)
         {
         SIZE TextSize;
         for (int i=0; i<pText->Count; i++)
            {
            s=pText->Strings[i];
            if (::GetTextExtentPoint(DC,s.c_str(),s.Length(),&TextSize))
               {
               if (TextSize.cx>MaxSize.cx) MaxSize.cx=TextSize.cx;
               if (TextSize.cy>MaxSize.cy) MaxSize.cy=TextSize.cy;
               }
            else
               {
               int nErr=GetLastError();
               ss=SysErrorMessage(nErr);
               }
            }
         }
      else
         {
         MaxSize.cx=0;
         MaxSize.cy=0;
         }
      Size.nW=MaxSize.cx;
		Size.nH=MaxSize.cy*pText->Count;
      if (Size.nW<6) Size.nW=6;
		if (Size.nH<4) Size.nH=4;
      if (hOldFont)
         {
			::SelectObject(DC,hOldFont);
         ::DeleteObject(hFont);
         }
      }
}

LineStruct::LineStruct()
{
   Init();
}

LineStruct::LineStruct(const LineStruct& L)
{
	*this=L;
}

LineStruct::~LineStruct()
{
}

void LineStruct::Init()
{
	Color=clRed;
	nWidth=1;
	for (int i=0; i<2; i++)
		{
		aPos[i]=LatLong(0.0,0.0);
		abShowArrow[i]=false;
		anArrowSize[i]=10;
		aPixPos[i].Invalidate();
		}
	nStyle=PS_SOLID;
	bLocked=false;
	bDeleted=false;
	bDisplay=true;
	SegPosList.Clear();
	SegPixList.Clear();
	bMulti=false;
}

LineStruct& LineStruct::operator = (const LineStruct& L)
{
	Color=L.Color;
	nWidth=L.nWidth;
	nStyle=L.nStyle;
	bLocked=L.bLocked;
	bDeleted=L.bDeleted;
	bDisplay=L.bDisplay;
	for (int i=0; i<2; i++)
		{
		aPos[i]=L.aPos[i];
		abShowArrow[i]=L.abShowArrow[i];
		anArrowSize[i]=L.anArrowSize[i];
		aPixPos[i]=L.aPixPos[i];
		aBounds[i]=L.aBounds[i];
		}
	bMulti=L.bMulti;
	SegPosList=L.SegPosList;
	SegPosList.Pack();
	SegPixList=L.SegPixList;
	SegPixList.Pack();
	return *this;
}

void LineStruct::Draw(HDC DC, const TColor)
{
	if (!SegPixList.bPacked()) SegPixList.Pack();
	if ((SegPixList.nGetCount()>1)&&(bDisplay))
		{
		if (SegPixList[0]->bValid())
			{
			POINT* pPnt=new POINT[SegPixList.nGetCount()];
			for (int i=0; i<SegPixList.nGetCount(); i++)
				{
				pPnt[i].x=SegPixList[i]->nx;
				pPnt[i].y=SegPixList[i]->ny;
				}
			HPEN hPen=::CreatePen(PS_SOLID,nWidth,g_GetColor(Color));
			HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
			::Polyline(DC,pPnt,SegPixList.nGetCount());
         ::SelectObject(DC,hOldPen);
         ::DeleteObject(hPen);
         delete[] pPnt;
         }
      }
   else
      {
      if ((aPixPos[0].bValid())&&(bDisplay))
         {
         Pixel aPix[2];
         aPix[0]=aPixPos[0];
         aPix[1]=aPixPos[1];
         for (int j=0; j<2; j++)
            {
            if (abShowArrow[j])
               {
               double dR=anArrowSize[j];
               double dP=dR/3.0;
               int n;
               if (j==0) n=1; else n=0;
               double dx=aPixPos[j].nx-aPixPos[n].nx;
               double dy=aPixPos[n].ny-aPixPos[j].ny;
               double dTheta;
               if (fabs(dy)>1e-10)
                  dTheta=atan2(dx,dy);
               else if (dx>0)
                  dTheta=M_PI/2.0;
               else
                  dTheta=-M_PI/2,0;
					double dXc=aPixPos[j].nx-dR*sin(dTheta);
               double dYc=aPixPos[j].ny+dR*cos(dTheta);
               double dPhi=M_PI/2.0-dTheta;
               double dXa=dXc-dP*sin(dPhi);
               double dYa=dYc-dP*cos(dPhi);
               double dXb=dXc+dP*cos(dTheta);
               double dYb=dYc+dP*sin(dTheta);
               POINT aPnt[3];
               aPnt[0].x=aPixPos[j].nx;
               aPnt[0].y=aPixPos[j].ny;
               aPnt[1].x=dXb;
               aPnt[1].y=dYb;
               aPnt[2].x=dXa;
               aPnt[2].y=dYa;
               HPEN hPen=::CreatePen(PS_SOLID,1,g_GetColor(Color));
               HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
               HBRUSH hBrush=::CreateSolidBrush(g_GetColor(Color));
               HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
               ::Polygon(DC,aPnt,3);
               ::SelectObject(DC,hOldBrush);
               ::DeleteObject(hBrush);
               ::SelectObject(DC,hOldPen);
               ::DeleteObject(hPen);
               aPix[j]=Pixel(dXc,dYc);
               }
            }
         HPEN hPen=::CreatePen(PS_SOLID,nWidth,g_GetColor(Color));
         HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
         if (nWidth<=1)
            {
            aPix[0]=aPixPos[0];
            aPix[1]=aPixPos[1];
            }
         ::MoveToEx(DC,aPix[0].nx,aPix[0].ny,NULL);
         ::LineTo(DC,aPix[1].nx,aPix[1].ny);
         ::SelectObject(DC,hOldPen);
         ::DeleteObject(hPen);
         }
		}
}

void LineStruct::CalcBounds()
{
	for (int i=0; i<2; i++)
		{
      aBounds[i].Left=aPixPos[i].nx-3;
      aBounds[i].Top=aPixPos[i].ny-3;
      aBounds[i].Right=aPixPos[i].nx+3;
      aBounds[i].Bottom=aPixPos[i].ny+3;
      }
}

void LineStruct::SetPos(const Pixel& p1, const Pixel& p2)
{
   aPixPos[0]=p1;
   aPixPos[1]=p2;
	CalcBounds();
}


void LineStruct::Store(JIniFile* pFil, const String sName)
{
   pFil->Write(sName,L"Colour",Color);
   pFil->Write(L"Width",nWidth);
   for (int i=0; i<2; i++)
      {
      pFil->Write(L"Latitude "+String(i),aPos[i].dLat);
      pFil->Write(L"Longitude "+String(i),aPos[i].dLong);
      pFil->Write(L"Show Arrow "+String(i),abShowArrow[i]);
      pFil->Write(L"Arrow Size "+String(i),anArrowSize[i]);
      }
   pFil->Write(L"Style",nStyle);
   pFil->Write(L"Lock Position",bLocked);
   pFil->Write(L"Display",bDisplay);
   pFil->Write(L"Multi Segment",bMulti);
   pFil->Write(L"Segment Count",SegPosList.nGetCount());
   for (int i=0; i<SegPosList.nGetCount(); i++)
      {
      pFil->Write(L"Segment "+String(i)+" Latitude",SegPosList[i]->dLat);
      pFil->Write(L"Segment "+String(i)+" Longitude",SegPosList[i]->dLong);
      }
}

void LineStruct::Store(JIniFile* pFil, const int n)
{
   Store(pFil,L"LINE "+String(n));
}

void LineStruct::Read(JIniFile* pFil, const String sName)
{
   pFil->Read(sName,L"Colour",Color,clRed);
   pFil->Read(L"Width",nWidth,1);
   for (int i=0; i<2; i++)
      {
      pFil->Read(L"Latitude "+String(i),aPos[i].dLat);
      pFil->Read(L"Longitude "+String(i),aPos[i].dLong);
      pFil->Read(L"Show Arrow "+String(i),abShowArrow[i],false);
      pFil->Read(L"Arrow Size "+String(i),anArrowSize[i],5);
      }
   pFil->Read(L"Style",nStyle);
   pFil->Read(L"Lock Position",bLocked,false);
   pFil->Read(L"Display",bDisplay,true);
   pFil->Read(L"Multi Segment",bMulti,false);
   int nSeg=pFil->nRead(L"Segment Count",0);
   SegPosList.Clear();
   SegPixList.Clear();
   for (int i=0; i<nSeg; i++)
      {
      LatLong* pLL=new LatLong;
      pFil->Read(L"Segment "+String(i)+" Latitude",pLL->dLat,0.0);
      pFil->Read(L"Segment "+String(i)+" Longitude",pLL->dLong,0.0);
      SegPosList.nAdd(pLL);
      Pixel* pPix=new Pixel;
      pPix->Invalidate();
      SegPixList.nAdd(pPix);
      }
   SegPosList.Pack();
   SegPixList.Pack();
}

void LineStruct::Read(JIniFile* pFil, const int n)
{
   Read(pFil,L"LINE "+String(n));
}

void LineStruct::SetData(const bool bDefault, const int nSide)
{
   NavLineForm->Ed.Init();
   NavLineForm->Ed.nSetLL("Line Start Position",NavLineForm->Lat1Ed,NavLineForm->Long1Ed,aPos[0]);
   NavLineForm->Ed.nSetVal(NavLineForm->Head1Ed,anArrowSize[0],"Arrow Size",1,99);
   NavLineForm->Head1UD->Position=anArrowSize[0];
   NavLineForm->Arrow1Chk->Checked=abShowArrow[0];
   NavLineForm->Ed.nSetLL("Line End Position",NavLineForm->Lat2Ed,NavLineForm->Long2Ed,aPos[1]);
   NavLineForm->Ed.nSetVal(NavLineForm->Head2Ed,anArrowSize[1],"Arrow Size",1,99);
   NavLineForm->Head2UD->Position=anArrowSize[1];
   NavLineForm->Arrow2Chk->Checked=abShowArrow[1];
   NavLineForm->Ed.nSetVal(NavLineForm->WidthEd,nWidth,"Line Width",1,99);
   NavLineForm->WidthUD->Position=nWidth;
   NavLineForm->ColorPan->Color=Color;
   NavLineForm->nStyle=nStyle;
   NavLineForm->LockChk->Checked=bLocked;
   NavLineForm->bDefault=bDefault;
   NavLineForm->nSide=nSide;
   NavLineForm->DispChk->Checked=bDisplay;
   NavLineForm->Seg.Clear();
   if (!SegPosList.bPacked()) SegPosList.Pack();
   for (int i=0; i<SegPosList.nGetCount(); i++)
      {
      LatLong* pLL=new LatLong;
      *pLL=*SegPosList[i];
      NavLineForm->Seg.nAdd(pLL);
      }
   NavLineForm->Seg.Pack();
   NavLineForm->SingleRad->Checked=!bMulti;
   NavLineForm->MultiRad->Checked=bMulti;
}

bool LineStruct::bGetData()
{
   if (NavLineForm->bGetPosChanged(0))
      NavLineForm->Ed.GetLL(NavLineForm->Lat1Ed,NavLineForm->Long1Ed,aPos[0]);
   if (NavLineForm->bGetPosChanged(1))
      NavLineForm->Ed.GetLL(NavLineForm->Lat2Ed,NavLineForm->Long2Ed,aPos[1]);
   anArrowSize[0]=NavLineForm->Head1UD->Position;
   abShowArrow[0]=NavLineForm->Arrow1Chk->Checked;
   anArrowSize[1]=NavLineForm->Head2UD->Position;
   abShowArrow[1]=NavLineForm->Arrow2Chk->Checked;
   nWidth=NavLineForm->WidthUD->Position;
   Color=NavLineForm->ColorPan->Color;
   nStyle=NavLineForm->StyleCombo->ItemIndex;
   bLocked=NavLineForm->LockChk->Checked;
   bDeleted=NavLineForm->bDeleted;
   bDisplay=NavLineForm->DispChk->Checked;
   bMulti=NavLineForm->MultiRad->Checked;
   SegPosList.Clear();
   SegPixList.Clear();
   if (bMulti)
      {
      NavLineForm->Seg.Pack();
      for (int i=0; i<NavLineForm->Seg.nGetCount(); i++)
         {
         LatLong* pLL=new LatLong;
         *pLL=*NavLineForm->Seg[i];
         SegPosList.nAdd(pLL);
         Pixel* pPix=new Pixel;
         pPix->Invalidate();
         SegPixList.nAdd(pPix);
         }
      SegPosList.Pack();
      SegPixList.Pack();
      }
   return NavLineForm->Ed.bValid();
}


bool LineStruct::bEdit(const bool bDefault, const int nSide, const bool bShowDel)
{
   NavLineForm=new TNavLineForm(0);
   SetData(bDefault,nSide);
   NavLineForm->bShowDel=bShowDel&&(!bDefault);
   int nEd=NavLineForm->ShowModal();
   if (nEd==mrOk)
      bGetData();
   delete NavLineForm;
   return (nEd==mrOk);
}

void LineStruct::OpenEdit(const int nSide)
{
   NavLineForm=new TNavLineForm(0);
   NavLineForm->bSubEdit=false;
   NavLineForm->FormStyle=fsStayOnTop;
   SetData(false,nSide);
   NavLineForm->Show();
}

NM_DIALOG_STATE LineStruct::CloseEdit()
{
   NM_DIALOG_STATE State=NavLineForm->GetState();
   switch(State)
      {
      case NM_DIALOG_OK:
         bGetData();
         delete NavLineForm;
         break;
      case NM_DIALOG_CANCEL:
         delete NavLineForm;
         break;
      default: ;
      }
   return State;
}

int LineStruct::nInside(const Pixel Pix)
{
   int n=-1;
   if (!bDeleted)
      for (int i=0; (i<2)&&(n==-1); i++)
         if ((Pix.nx>=aBounds[i].Left)&&(Pix.nx<=aBounds[i].Right)&&(Pix.ny>=aBounds[i].Top)&&(Pix.ny<=aBounds[i].Bottom))
            n=i;
   return n;
}

ArcStruct::ArcStruct()
{
   Pos=LatLong(-34.16,18.5);
   nFwdRa=1000;
   nAftRa=100;
   dCenterBe=0.0;
   dArcWidth=20.0;
   sDescr=L"Footprint";
   FrameColor=clLtGray;
   nFrameWidth=1;
   bFill=false;
   FillColor=clGray;
   pPoly=NULL;
   bDisplay=true;
   ArcType=FOOTPRINT_ARC;
   nCircleDia=1000;
   bShowDot=false;
}

ArcStruct::~ArcStruct()
{
   delete[] pPoly;
}

ArcStruct::ArcStruct(const ArcStruct& A)
{
	pPoly=NULL;
	*this=A;
}

ArcStruct& ArcStruct::operator = (const ArcStruct& A)
{
	Pos=A.Pos;
	nFwdRa=A.nFwdRa;
	nAftRa=A.nAftRa;
	dCenterBe=A.dCenterBe;
	dArcWidth=A.dArcWidth;
	sDescr=A.sDescr;
	FrameColor=A.FrameColor;
	nFrameWidth=A.nFrameWidth;
	bFill=A.bFill;
	FillColor=A.FillColor;
	PixPos=A.PixPos;
	bDeleted=A.bDeleted;
	nPoints=A.nPoints;
	bDisplay=A.bDisplay;
   ArcType=A.ArcType;
   nCircleDia=A.nCircleDia;
   bShowDot=A.bShowDot;
   SegList=A.SegList;
   SegList.Pack();
   delete[] pPoly;
   pPoly=NULL;
   if ((A.pPoly)&&(nPoints>0))
      {
      pPoly=new POINT[nPoints];
      memcpy(pPoly,A.pPoly,nPoints*sizeof(POINT));
      }
   nCirclePix=A.nCirclePix;
   return *this;
}

void ArcStruct::Draw(HDC DC, const TColor)
{
   if (bDisplay)
      {
      HBRUSH hBrush;
      if (bFill)
         hBrush=::CreateSolidBrush(g_GetColor(FillColor));
      else
         hBrush=(HBRUSH)::GetStockObject(NULL_BRUSH);
      HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
      HPEN hPen=::CreatePen(PS_SOLID,nFrameWidth,g_GetColor(FrameColor));
      HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
      if (pPoly)
         {
         ::Polygon(DC,pPoly,nPoints);
         if (ArcType==FOOTPRINT_ARC)
            ::Ellipse(DC,PixPos.nx-2,PixPos.ny-2,PixPos.nx+2,PixPos.ny+2);
         }
      else if (ArcType==CIRCLE_ARC)
         {
         ::Ellipse(DC,PixPos.nx-nCirclePix,PixPos.ny-nCirclePix,PixPos.nx+nCirclePix,PixPos.ny+nCirclePix);
         if (bShowDot)
            ::Ellipse(DC,PixPos.nx-2,PixPos.ny-2,PixPos.nx+2,PixPos.ny+2);
         }
      else if (ArcType==ROSE_ARC)
         {
         ::Ellipse(DC,PixPos.nx-nCirclePix,PixPos.ny-nCirclePix,PixPos.nx+nCirclePix,PixPos.ny+nCirclePix);
         if (bShowDot)
            ::Ellipse(DC,PixPos.nx-2,PixPos.ny-2,PixPos.nx+2,PixPos.ny+2);
         }
      ::SelectObject(DC,hOldPen);
      ::DeleteObject(hPen);
      ::SelectObject(DC,hOldBrush);
      ::DeleteObject(hBrush);
      }
}

#ifdef __DDGRAPH
int ArcStruct::nCalcArc(const XYPos& Center, const double dRadius, const double dStartAngle, const double dStopAngle, POINT* pPoint, const int nMaxPoints, DDGraph* pG)
#else
int ArcStruct::nCalcArc(const XYPos& Center, const double dRadius, const double dStartAngle, const double dStopAngle, POINT* pPoint, const int nMaxPoints, TransGraph* pG)
#endif
{
   int nPoints=0;
   int nLen,n;
   double dT,dx,dy;
   Pixel Pix;
   double d360=M_PI*2.0;
   double dStart=dStartAngle;
   double dStop=dStopAngle;
   if (dStart>d360) dStart-=d360;
   else if (dStart<0.0) dStart+=d360;
   if (dStop>d360) dStop-=d360;
   else if (dStop<0.0) dStop+=d360;
   if (dStart>dStop)
      {
      nLen=((d360-dStart)/d360)*360.0;
      for (n=1; n<nLen; n++)
         {
         dT=dStart+n*(d360-dStart)/nLen;
         dx=dRadius*sin(dT);
         dy=dRadius*cos(dT);
         Pix=pG->XY2Pix(XYPos(Center.dx+dx,Center.dy+dy));
         pPoint[nPoints].x=Pix.nx;
         pPoint[nPoints].y=Pix.ny;
         if (nPoints<nMaxPoints-1) nPoints++;
         }
      nLen=(dStop/d360)*360.0;
      for (n=0; n<nLen; n++)
         {
         dT=n*dStop/nLen;
         dx=dRadius*sin(dT);
         dy=dRadius*cos(dT);
         Pix=pG->XY2Pix(XYPos(Center.dx+dx,Center.dy+dy));
         pPoint[nPoints].x=Pix.nx;
         pPoint[nPoints].y=Pix.ny;
         if (nPoints<nMaxPoints-1) nPoints++;
         }
      }
   else
      {
      double dSweep=dStop-dStart;
      nLen=(dSweep/d360)*360.0;
      for (n=0; n<nLen; n++)
         {
         dT=dStart+n*dSweep/nLen;
         dx=dRadius*sin(dT);
         dy=dRadius*cos(dT);
         Pix=pG->XY2Pix(XYPos(Center.dx+dx,Center.dy+dy));
         pPoint[nPoints].x=Pix.nx;
         pPoint[nPoints].y=Pix.ny;
         if (nPoints<nMaxPoints-1) nPoints++;
         }
      }
   return nPoints;
}

#ifdef __DDGRAPH
void ArcStruct::CalcPoly(DDGraph* pG)
#else
void ArcStruct::CalcPoly(TransGraph* pG)
#endif
{
   if (ArcType==FOOTPRINT_ARC)
      {
      XYPos XY=pG->LL2XY(Pos);
      double dA=dCenterBe*M_PI/180.0;
      double dB=dArcWidth*M_PI/180.0;
      double dR3=nAftRa/cos((M_PI-dB)/2.0);
      double dR4=sqrt(dSqr(dR3)-dSqr(nAftRa));
      double dE=dA-M_PI/2.0;
      double dx=dR3*cos(dE);
      double dy=dR3*sin(dE);
      XYPos O=XYPos(XY.dx-dx,XY.dy+dy);

      Pixel Center=pG->XY2Pix(O);

      double dR5=dR3+nFwdRa;
      dx=dR5*sin(dA-dB/2.0);
      dy=dR5*cos(dA-dB/2.0);
      XYPos T2=XYPos(O.dx+dx,O.dy+dy);
      dx=dR5*sin(dA+dB/2.0);
      dy=dR5*cos(dA+dB/2.0);
      XYPos T3=XYPos(O.dx+dx,O.dy+dy);

      dx=dR4*sin(dA-dB/2.0);
      dy=dR4*cos(dA-dB/2.0);
      XYPos T1=XYPos(O.dx+dx,O.dy+dy);

      dx=dR4*sin(dA+dB/2.0);
      dy=dR4*cos(dA+dB/2.0);
      XYPos T4=XYPos(O.dx+dx,O.dy+dy);

      Pixel P1=pG->XY2Pix(T1);
      Pixel P2=pG->XY2Pix(T2);
      Pixel P3=pG->XY2Pix(T3);
      Pixel P4=pG->XY2Pix(T4);

      POINT aPoint[1000];

      nPoints=0;
      aPoint[nPoints].x=P1.nx;
      aPoint[nPoints].y=P1.ny;
      nPoints++;
      aPoint[nPoints].x=P2.nx;
      aPoint[nPoints].y=P2.ny;
      nPoints++;

      nPoints+=nCalcArc(O,dR5,dA-dB/2.0,dA+dB/2.0,&aPoint[nPoints],1000-nPoints,pG);

      aPoint[nPoints].x=P3.nx;
      aPoint[nPoints].y=P3.ny;
      nPoints++;
      aPoint[nPoints].x=P4.nx;
      aPoint[nPoints].y=P4.ny;
      nPoints++;

      double d90=M_PI/2.0;
      nPoints+=nCalcArc(XY,nAftRa,d90+dA+dB/2.0,-d90+dA-dB/2.0,&aPoint[nPoints],1000-nPoints,pG);

      aPoint[nPoints].x=P1.nx;
      aPoint[nPoints].y=P1.ny;
      nPoints++;
      delete[] pPoly;
      pPoly=new POINT[nPoints];
      memcpy(pPoly,aPoint,sizeof(POINT)*nPoints);
      PixPos=pG->XY2Pix(XY);
      }
   else if (SegList.nGetCount()>2)
      {
      delete[] pPoly;
      nPoints=SegList.nGetCount()+1;
      pPoly=new POINT[nPoints];
      for (int i=0; i<nPoints-1; i++)
         pPoly[i]=pG->LL2Pix(*SegList[i]).ToPoint();
      pPoly[nPoints-1]=pPoly[0];
      }
   else
      {
      XYPos XY=pG->LL2XY(Pos);
      PixPos=pG->XY2Pix(XY);
      XY.dx+=nCircleDia*0.5;
      nCirclePix=abs(pG->XY2Pix(XY).nx-PixPos.nx);      
      }
}


void ArcStruct::Store(JIniFile* pFil, const String sName)
{
   pFil->Write(sName,L"Latitude",Pos.dLat);
   pFil->Write(L"Longitude",Pos.dLong);
   pFil->Write(L"Forward Range",nFwdRa);
   pFil->Write(L"Aft Range",nAftRa);
   pFil->Write(L"Centre Be",dCenterBe);
   pFil->Write(L"Arc Width",dArcWidth);
   pFil->Write(L"Descr",sDescr);
   pFil->Write(L"Frame Colour",FrameColor);
   pFil->Write(L"Frame Width",nFrameWidth);
   pFil->Write(L"Fill Frame",bFill);
   pFil->Write(L"Fill Colour",FillColor);
   pFil->Write(L"Display",bDisplay);
   pFil->Write(L"Type",ArcType);
   pFil->Write(L"Circle Diameter",nCircleDia);
   pFil->Write(L"Segment Count",SegList.nGetCount());
   for (int i=0; i<SegList.nGetCount(); i++)
      pFil->Write(L"Segment "+String(i),*SegList[i]);
}

void ArcStruct::Read(JIniFile* pFil, const String sName)
{
   pFil->Read(sName,L"Latitude",Pos.dLat,-34.16);
   pFil->Read(L"Longitude",Pos.dLong,18.5);
   pFil->Read(L"Forward Range",nFwdRa,1000);
   pFil->Read(L"Aft Range",nAftRa,100);
   pFil->Read(L"Centre Be",dCenterBe,0.0);
   pFil->Read(L"Arc Width",dArcWidth,20.0);
   pFil->Read(L"Descr",sDescr,String(L"Footprint"));
   pFil->Read(L"Frame Colour",FrameColor,clLtGray);
   pFil->Read(L"Frame Width",nFrameWidth,1);
   pFil->Read(L"Fill Frame",bFill,false);
   pFil->Read(L"Fill Colour",FillColor,clGray);
   pFil->Read(L"Display",bDisplay,true);
   ArcType=(ARC_TYPE)pFil->nRead(L"Type",FOOTPRINT_ARC);
   pFil->Read(L"Circle Diameter",nCircleDia,1000);
   int nCnt;
   pFil->Read(L"Segment Count",nCnt);
   SegList.Clear();
   if (nCnt>0)
      {
      for (int i=0; i<SegList.nGetCount(); i++)
         {
         LatLong* pLL=new LatLong;
         pFil->Read(L"Segment "+String(i),*pLL);
         SegList.nAdd(pLL);
         }
      SegList.Pack();
      }
}

void ArcStruct::Store(JIniFile* pFil, const int n)
{
   Store(pFil,L"FOOTPRINT "+String(n));
}

void ArcStruct::Read(JIniFile* pFil, const int n)
{
   Read(pFil,L"FOOTPRINT "+String(n));
}

void ArcStruct::SetData(const bool bDefault)
{
   NavArcForm->Ed.Init();
   NavArcForm->Ed.nSetLL("",NavArcForm->LatEd,NavArcForm->LongEd,Pos);
   NavArcForm->Ed.nSetVal(NavArcForm->FRaEd,nFwdRa,"Arc Forward Range",1,99999);
   NavArcForm->Ed.nSetVal(NavArcForm->ARaEd,nAftRa,"Arc Aft Range",1,99999);
   NavArcForm->Ed.nSetVal(NavArcForm->BeEd,dCenterBe,1,"Arc Centre Bearing",0.0,359.9);
   NavArcForm->Ed.nSetVal(NavArcForm->ArcEd,dArcWidth,1,"Arc Width",0.1,9999.9);
   NavArcForm->DescrEd->Text=sDescr;
   NavArcForm->Ed.nSetVal(NavArcForm->CircleEd,nCircleDia,"Circle Diameter",1,9999);
   NavArcForm->CircleDotChk->Checked=bShowDot;
   NavArcForm->FramePan->Color=FrameColor;
   NavArcForm->Ed.nSetVal(NavArcForm->WidthEd,nFrameWidth,"Frame Width",1,99);
   NavArcForm->WidthUD->Position=nFrameWidth;
   NavArcForm->FillChk->Checked=bFill;
   NavArcForm->FillPan->Color=FillColor;
   NavArcForm->DelBut->Enabled=!bDefault;
   NavArcForm->DispChk->Checked=bDisplay;
   NavArcForm->ArcRad->Checked=(ArcType==FOOTPRINT_ARC);
   NavArcForm->SegRad->Checked=(ArcType==USER_ARC);
   NavArcForm->CircleRad->Checked=(ArcType==CIRCLE_ARC);
   if (ArcType==USER_ARC)
      {
      NavArcForm->Seg.Clear();
      SegList.Pack();
      for (int i=0; i<SegList.nGetCount(); i++)
         {
         LatLong* pLL=new LatLong;
         *pLL=*SegList[i];
         NavArcForm->Seg.nAdd(pLL);
         }
      NavArcForm->Seg.Pack();
      }
}

bool ArcStruct::bGetData()
{
   NavArcForm->Ed.GetLL(NavArcForm->LatEd,NavArcForm->LongEd,Pos);
   NavArcForm->Ed.GetVal(NavArcForm->FRaEd,nFwdRa);
   NavArcForm->Ed.GetVal(NavArcForm->ARaEd,nAftRa);
   NavArcForm->Ed.GetVal(NavArcForm->BeEd,dCenterBe);
   NavArcForm->Ed.GetVal(NavArcForm->ArcEd,dArcWidth);
   sDescr=NavArcForm->DescrEd->Text;
   NavArcForm->Ed.GetVal(NavArcForm->CircleEd,nCircleDia);
   bShowDot=NavArcForm->CircleDotChk->Checked;
   FrameColor=NavArcForm->FramePan->Color;
   nFrameWidth=NavArcForm->WidthUD->Position;
   bFill=NavArcForm->FillChk->Checked;
   FillColor=NavArcForm->FillPan->Color;
   bDeleted=NavArcForm->bDeleted;
   bDisplay=NavArcForm->DispChk->Checked;
   SegList.Clear();
   if (NavArcForm->ArcRad->Checked)
      ArcType=FOOTPRINT_ARC;
   else if (NavArcForm->SegRad->Checked)
      {
      ArcType=USER_ARC;
      for (int i=0; i<NavArcForm->Seg.nGetCount(); i++)
         {
         LatLong* pLL=new LatLong;
         *pLL=*NavArcForm->Seg[i];
         SegList.nAdd(pLL);
         }
      SegList.Pack();
      }
   else if (NavArcForm->CircleRad->Checked)
      ArcType=CIRCLE_ARC;
   else
      ArcType=ROSE_ARC;
   return NavArcForm->Ed.bValid();
}

bool ArcStruct::bEdit(const bool bDefault, const bool bShowDel)
{
   bool bOK=false;
   NavArcForm=new TNavArcForm(0);
   SetData(bDefault);
   NavArcForm->bShowDel=bShowDel&&(!bDefault);
   if (NavArcForm->ShowModal()==mrOk)
      {
      bGetData();
      bOK=true;
      }
   delete NavArcForm;
   return bOK;
}

void ArcStruct::OpenEdit()
{
   NavArcForm=new TNavArcForm(0);
   NavArcForm->bSubEdit=false;
   NavArcForm->FormStyle=fsStayOnTop;
   SetData(false);
   NavArcForm->Show();
}

NM_DIALOG_STATE ArcStruct::CloseEdit()
{
   NM_DIALOG_STATE State=NavArcForm->GetState();
   switch(State)
      {
      case NM_DIALOG_OK:
         bGetData();
         delete NavArcForm;
         break;
      case NM_DIALOG_CANCEL:
         delete NavArcForm;
         break;
      default: ;
      }
   return State;
}

ManVectStruct::ManVectStruct()
{
   pFont=new TFont;
   Init();
}

ManVectStruct::ManVectStruct(const ManVectStruct& M)
{
   pFont=new TFont;
   *this=M;
}

ManVectStruct::~ManVectStruct()
{
   delete pFont;
}

void ManVectStruct::Init()
{
   Color=clRed;
   IDColor=clYellow;
   nWidth=2;
   memset(&DT,0,sizeof(DT));
   aPos[0].Invalidate();
   aPos[1]=aPos[0];
   dSpeed=-1000.0;
   nCourse=-1;
   sShip=L"";
   aPixPos[0].Invalidate();
   aPixPos[1].Invalidate();
   IDPixPos.Invalidate();
   bDisplay=true;
}

ManVectStruct& ManVectStruct::operator = (const ManVectStruct& M)
{
	Color=M.Color;
   IDColor=M.IDColor;
   nWidth=M.nWidth;
   DT=M.DT;
   dSpeed=M.dSpeed;
   nCourse=M.nCourse;
   sShip=M.sShip;
   aPos[0]=M.aPos[0];
   aPos[1]=M.aPos[1];
   aPixPos[0]=M.aPixPos[0];
   aPixPos[1]=M.aPixPos[1];
   IDPixPos=M.IDPixPos;
   bDisplay=M.bDisplay;
   pFont->Assign(M.pFont);
   return *this;
}

void ManVectStruct::Store(JIniFile* pFil, const int n)
{
   pFil->Write(L"VECTOR "+String(n),L"Color",Color);
   pFil->Write(L"Line Width",nWidth);
   pFil->Write(L"ID Colour",IDColor);
   pFil->Write(L"Font",pFont);
   pFil->Write(L"Time",DT);
   pFil->Write(L"Line Lat 0",aPos[0].dLat);
   pFil->Write(L"Line Long 0",aPos[0].dLong);
   pFil->Write(L"Line Lat 1",aPos[1].dLat);
   pFil->Write(L"Line Long 1",aPos[1].dLong);
   pFil->Write(L"Speed",dSpeed);
   pFil->Write(L"Course",nCourse);
   pFil->Write(L"Ship",sShip);
   pFil->Write(L"Display",bDisplay);
}

void ManVectStruct::Read(JIniFile* pFil, const int n)
{
   pFil->Read(L"VECTOR "+String(n),L"Color",Color,clRed);
   pFil->Read(L"Line Width",nWidth,2);
   pFil->Read(L"ID Colour",IDColor,clYellow);
   pFil->Read(L"Font",pFont);
   pFil->Read(L"Time",DT);
   pFil->Read(L"Line Lat 0",aPos[0].dLat);
   pFil->Read(L"Line Long 0",aPos[0].dLong);
   pFil->Read(L"Line Lat 1",aPos[1].dLat);
   pFil->Read(L"Line Long 1",aPos[1].dLong);
   pFil->Read(L"Speed",dSpeed);
   pFil->Read(L"Course",nCourse);
   pFil->Read(L"Ship",sShip);
   pFil->Read(L"Display",bDisplay,true);
}

void ManVectStruct::SetPos(const Pixel& p1, const Pixel& p2)
{
   aPixPos[0]=p1;
   aPixPos[1]=p2;
   IDPixPos=aPixPos[0];
}

void ManVectStruct::Draw(HDC& DC, const int nID)
{
   if (bDisplay)
      {
      HPEN hPen=::CreatePen(PS_SOLID,nWidth,g_GetColor(Color));
      HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
      ::MoveToEx(DC,aPixPos[0].nx,aPixPos[0].ny,NULL);
      ::LineTo(DC,aPixPos[1].nx,aPixPos[1].ny);
      ::SelectObject(DC,hOldPen);
      ::DeleteObject(hPen);
      int nWeight;
      if (pFont->Style.Contains(fsBold))
         nWeight=FW_BOLD;
      else
         nWeight=FW_DONTCARE;
      HFONT hFont=::CreateFont(pFont->Size,	// logical height of font
                  0,	// logical average character width
                  0,	// angle of escapement
                  0,	// base-line orientation angle
                  nWeight,	// font weight
                  pFont->Style.Contains(fsItalic),	// italic attribute flag
                  pFont->Style.Contains(fsUnderline),	// underline attribute flag
                  pFont->Style.Contains(fsStrikeOut),	// strikeout attribute flag
                  DEFAULT_CHARSET,	   // character set identifier
                  OUT_DEFAULT_PRECIS,	// output precision
                  CLIP_DEFAULT_PRECIS,	// clipping precision
                  DEFAULT_QUALITY,	   // output quality
                  DEFAULT_PITCH,	      // pitch and family
                  pFont->Name.c_str()); 	// pointer to typeface name string
      HFONT hOldFont=(HFONT)::SelectObject(DC,hFont);
      String ss;
      if (!hOldFont)
         {
         int nErr=GetLastError();
         ss=SysErrorMessage(nErr);
         }
      ::SelectObject(DC,hFont);
      ::SetTextColor(DC,g_GetColor(pFont->Color));
      ::SetTextAlign(DC,TA_TOP|TA_LEFT);
		TCHAR szSt[8];
		swprintf(szSt,L"%d",nID);
		SIZE Size;
      ::GetTextExtentPoint(DC,szSt,wcslen(szSt),&Size);
      int nx=IDPixPos.nx;
      int ny=IDPixPos.ny;
      hPen=::CreatePen(PS_SOLID,1,g_GetColor(Color));
		hOldPen=(HPEN)::SelectObject(DC,hPen);
		HBRUSH hBrush=::CreateSolidBrush(g_GetColor(IDColor));
		HBRUSH hOldBrush=(HBRUSH)::SelectObject(DC,hBrush);
		int nR=sqrt(Size.cx*Size.cx+Size.cy*Size.cy*1.0)*0.6;
		::Ellipse(DC,nx-nR,ny-nR,nx+nR,ny+nR);
		int nPrevBkMode=::SetBkMode(DC,TRANSPARENT);
		::TextOut(DC,IDPixPos.nx-Size.cx/2,IDPixPos.ny-Size.cy/2,szSt,wcslen(szSt));
		::SetBkMode(DC,nPrevBkMode);
		::SelectObject(DC,hOldBrush);
		::SelectObject(DC,hOldPen);
		::DeleteObject(hPen);
		::SelectObject(DC,hOldFont);
		::DeleteObject(hFont);
		}
}

NavAidClass::NavAidClass()
{
	Clear();
}

NavAidClass::~NavAidClass()
{
}

NavAidClass& NavAidClass::operator = (const NavAidClass& N)
{
	LineList=N.LineList;
	LineList.Pack();
	SymbolList=N.SymbolList;
	SymbolList.Pack();
	ArcList=N.ArcList;
	ArcList.Pack();
	VectList=N.VectList;
	VectList.Pack();
	nCurrentLine=N.nCurrentLine;
	nCurrentSymbol=N.nCurrentSymbol;
	nCurrentArc=N.nCurrentArc;
	nLineDragNum=N.nLineDragNum;
	nSymDragNum=N.nSymDragNum;
	nRefSymbol=N.nRefSymbol;
	DragPix=N.DragPix;
	nDragOrigin=N.nDragOrigin;
	nDragPoint=N.nDragPoint;
	RefSymbol=N.RefSymbol;
	return *this;
}

void NavAidClass::Clear()
{
	LineList.Clear();
	SymbolList.Clear();
	ArcList.Clear();
	VectList.Clear();
	ClearRef();
	nCurrentLine=-1;
	nCurrentArc=-1;
	nCurrentSymbol=-1;
	nLineDragNum=-1;
   nSymDragNum=-1;
   nRefSymbol=-1;
}

bool NavAidClass::bRead(const String sName)
{
   try
      {
      JIniFile Fil(sName,false,false,true);
      int nVer,n;
      Fil.Read(L"NAV AIDS",L"Version",nVer,0);
      Fil.Read(L"LINES",L"Num",n,0);
      LineList.Clear();
      for (int i=0; i<n; i++)
         {
         LineStruct* pL=new LineStruct;
         pL->Read(&Fil,i);
         LineList.nAdd(pL);
         }
      LineList.Pack();
      Fil.Read(L"SYMBOLS",L"Num",n,0);
      SymbolList.Clear();
      nRefSymbol=-1;
      for (int i=0; i<n; i++)
         {
         SymbolStruct* pS=new SymbolStruct;
         pS->Read(&Fil,i);
         SymbolList.nAdd(pS);
         if (pS->bRef)
            nRefSymbol=i;
         }
      SymbolList.Pack();
      Fil.Read(L"FOOTPRINTS",L"Num",n,0);
      ArcList.Clear();
      for (int i=0; i<n; i++)
         {
         ArcStruct* pA=new ArcStruct;
         pA->Read(&Fil,i);
         ArcList.nAdd(pA);
         }
      ArcList.Pack();
      Fil.Read(L"VECTORS",L"Num",n,0);
      VectList.Clear();
      for (int i=0; i<n; i++)
         {
         ManVectStruct* pV=new ManVectStruct;
         pV->Read(&Fil,i);
         VectList.nAdd(pV);
         }
      VectList.Pack();
      return true;
      }
   catch(...)
      {
      return false;
      }
}

bool NavAidClass::bRead(const String sName, LatLong& ZoomCenter, SizeD& ZoomSize)
{
   JIniFile Fil(sName,false,false,true);
   LatLong NewCenter;
   SizeD NewSize;
   int nVer;
   Fil.Read(L"NAV AIDS",L"Version",nVer,0);
   Fil.Read(L"ZOOM",L"Center Lat",NewCenter.dLat,0.0);
   Fil.Read(L"Center Long",NewCenter.dLong,0.0);
   Fil.Read(L"Width",NewSize.dW);
   Fil.Read(L"Height",NewSize.dH);
   int nL,nS,nA,nV;
   Fil.Read(L"LINES",L"Num",nL,0);
   Fil.Read(L"SYMBOLS",L"Num",nS,0);
   Fil.Read(L"FOOTPRINTS",L"Num",nA,0);
   Fil.Read(L"VECTORS",L"Num",nV,0);
   NavReadForm=new TNavReadForm(0);
   NavReadForm->Caption=sFileNameOnly(sName,false);
   NavReadForm->Ed.SetValRO(NavReadForm->CTextEd,nSymbols());
   NavReadForm->Ed.SetValRO(NavReadForm->NTextEd,nS);
   NavReadForm->Ed.SetValRO(NavReadForm->CLineEd,nLines());
   NavReadForm->Ed.SetValRO(NavReadForm->NLineEd,nL);
   NavReadForm->Ed.SetLLRO(NavReadForm->CLatEd,NavReadForm->CLongEd,ZoomCenter);
   NavReadForm->Ed.SetValRO(NavReadForm->CSizeEd,ZoomSize.dW*0.666);
   NavReadForm->Ed.SetValRO(NavReadForm->NLatEd,NewCenter.dLat);
   NavReadForm->Ed.SetValRO(NavReadForm->NLongEd,NewCenter.dLong);
   NavReadForm->Ed.SetValRO(NavReadForm->NSizeEd,NewSize.dW*0.666);
   bool bOK=(NavReadForm->ShowModal()==mrOk);
   if (bOK)
      {
      if (NavReadForm->CurLineRad->Checked)
         LineList.Clear();
      for (int i=0; i<nL; i++)
         {
         LineStruct* pL=new LineStruct;
         pL->Read(&Fil,i);
         LineList.nAdd(pL);
         }
      LineList.Pack();
      nRefSymbol=-1;
      if (NavReadForm->CurLineRad->Checked)
         SymbolList.Clear();
      for (int i=0; i<nS; i++)
         {
         SymbolStruct* pS=new SymbolStruct;
         pS->Read(&Fil,i);
         int n=SymbolList.nAdd(pS);
         if (pS->bRef)
            nRefSymbol=n;
         }
      SymbolList.Pack();
      ArcList.Clear();
      for (int i=0; i<nA; i++)
         {
         ArcStruct* pA=new ArcStruct;
         pA->Read(&Fil,i);
         ArcList.nAdd(pA);
         }
      ArcList.Pack();
      VectList.Clear();
      for (int i=0; i<nV; i++)
         {
         ManVectStruct* pV=new ManVectStruct;
         pV->Read(&Fil,i);
         VectList.nAdd(pV);
         }
      VectList.Pack();
      if (NavReadForm->ViewChk->Checked)
         {
         ZoomCenter=NewCenter;
         ZoomSize=NewSize;
         }
      }
   delete NavReadForm;
   return bOK;
}

void NavAidClass::Store(const String sName, const LatLong& ZoomCenter, const SizeD& ZoomSize)
{
   JIniFile Fil(sName,true);
   Fil.Write(L"NAV AIDS",L"Version",NAVAID_VERSION);
   Fil.Write(L"ZOOM",L"Center Lat",ZoomCenter.dLat);
   Fil.Write(L"Center Long",ZoomCenter.dLong);
   Fil.Write(L"Width",ZoomSize.dW);
   Fil.Write(L"Height",ZoomSize.dH);
   Store(&Fil);
}

void NavAidClass::Store(JIniFile* pFil)
{
   pFil->Write(L"NAV AIDS",L"Version",NAVAID_VERSION);
   pFil->Write(L"LINES",L"Num",LineList.nGetCount());
   for (int i=0; i<LineList.nGetCount(); i++)
      LineList[i]->Store(pFil,i);
   pFil->Write(L"SYMBOLS",L"Num",SymbolList.nGetCount());
   for (int i=0; i<SymbolList.nGetCount(); i++)
      SymbolList[i]->Store(pFil,i);
   pFil->Write(L"FOOTPRINTS",L"Num",ArcList.nGetCount());
   for (int i=0; i<ArcList.nGetCount(); i++)
      ArcList[i]->Store(pFil,i);
   pFil->Write(L"VECTORS",L"Num",VectList.nGetCount());
   for (int i=0; i<VectList.nGetCount(); i++)
      VectList[i]->Store(pFil,i);
}

void NavAidClass::Read(JIniFile* pFil)
{
   Clear();
   int nL,nS,nA,nV;
   pFil->Read(L"LINES",L"Num",nL,0);
   pFil->Read(L"SYMBOLS",L"Num",nS,0);
   pFil->Read(L"FOOTPRINTS",L"Num",nA,0);
   pFil->Read(L"VECTORS",L"Num",nV,0);
   for (int i=0; i<nL; i++)
      {
      LineStruct* pL=new LineStruct;
      pL->Read(pFil,i);
      LineList.nAdd(pL);
      }
   LineList.Pack();
   for (int i=0; i<nS; i++)
      {
      SymbolStruct* pS=new SymbolStruct;
      pS->Read(pFil,i);
      SymbolList.nAdd(pS);
      }
   SymbolList.Pack();
   for (int i=0; i<nA; i++)
      {
      ArcStruct* pA=new ArcStruct;
      pA->Read(pFil,i);
      ArcList.nAdd(pA);
      }
   ArcList.Pack();
   for (int i=0; i<nV; i++)
      {
      ManVectStruct* pV=new ManVectStruct;
      pV->Read(pFil,i);
      VectList.nAdd(pV);
      }
   VectList.Pack();
}

int NavAidClass::nNew(LineStruct& Default)
{
   LineStruct* pL=new LineStruct;
   *pL=Default;
   LineList.nAdd(pL);
   LineList.Pack();
   return LineList.nGetCount()-1;
}

void NavAidClass::AddLine(const Pixel& p1, const Pixel& p2, const LatLong& LL1, const LatLong& LL2, LineStruct& Default)
{
   LineStruct* pL=new LineStruct;
   *pL=Default;
   pL->SetPos(p1,p2);
   pL->SetPos(LL1,LL2);
   LineList.nAdd(pL);
   LineList.Pack();
}

void NavAidClass::SetLine(const Pixel& p1, const Pixel& p2, const LatLong& LL1, const LatLong& LL2)
{
   LineList[nCurrentLine]->SetPos(p1,p2);
   LineList[nCurrentLine]->SetPos(LL1,LL2);
}

void NavAidClass::DeleteLine(const int n)
{
   LineList.Delete(n);
   LineList.Pack();
}

bool NavAidClass::bEditLine(const int nSide, int n, const bool bShowDel)
{
   if (n==-1)
      n=nCurrentLine;
   if (!LineList.bPacked()) LineList.Pack();
   if (LineList[n]->bEdit(false,nSide,bShowDel))
      {
      if (LineList[n]->bDeleted)
         {
         LineList.Delete(n);
         LineList.Pack();
         nCurrentLine=-1;
         }
      return true;
      }
   else
      return false;
}

void NavAidClass::DeleteSymbol(const int n)
{
   if (SymbolList[n]->bRef)
      nRefSymbol=-1;
   SymbolList.Delete(n);
   SymbolList.Pack();
}

bool NavAidClass::bEditSymbol(int n, const bool bShowDel)
{
   if (n==-1)
      n=nCurrentSymbol;
   if (!SymbolList.bPacked()) SymbolList.Pack();
   if (SymbolList[n]->bEdit(false,false,bShowDel))
      {
      if (SymbolList[n]->bDeleted)
         {
         SymbolList.Delete(n);
         SymbolList.Pack();
         nCurrentSymbol=-1;
         }
      else if (SymbolList[n]->bRef)
         {
         for (int i=0; i<SymbolList.nGetCount(); i++)
            {
            if (i!=n)
               SymbolList[i]->bRef=false;
            }
         }
      if (nCurrentSymbol>=0)
         {
         if (SymbolList[n]->bRef)
            nRefSymbol=n;
         else
            nRefSymbol=-1;
         }
      return true;
      }
   else
      return false;
}

void NavAidClass::DrawAll(HDC DC, const TColor BackColor)
{
   for (int i=0; i<ArcList.nGetCount(); i++)
      ArcList[i]->Draw(DC,BackColor);
   DrawRef(DC,BackColor);
   for (int i=0; i<VectList.nGetCount(); i++)
      VectList[i]->Draw(DC,i+1);
   for (int i=0; i<LineList.nGetCount(); i++)
      if (i!=nLineDragNum)
         LineList[i]->Draw(DC,BackColor);
   for (int i=0; i<SymbolList.nGetCount(); i++)
      {
      if (SymbolList[i]->bRef)
         nRefSymbol=i;
      if (i!=nSymDragNum)
         SymbolList[i]->Draw(DC,BackColor);
      }
}

void NavAidClass::DrawArcs(HDC DC, const TColor BackColor)
{
   for (int i=0; i<ArcList.nGetCount(); i++)
      ArcList[i]->Draw(DC,BackColor);
}

void NavAidClass::DrawVectors(HDC DC)
{
   for (int i=0; i<VectList.nGetCount(); i++)
      VectList[i]->Draw(DC,i+1);
}

void NavAidClass::DrawLines(HDC DC, const TColor BackColor)
{
   for (int i=0; i<LineList.nGetCount(); i++)
      if (i!=nLineDragNum)
         LineList[i]->Draw(DC,BackColor);
}

void NavAidClass::DrawSymbols(HDC DC, const TColor BackColor)
{
   for (int i=0; i<SymbolList.nGetCount(); i++)
      {
      if (SymbolList[i]->bRef)
         nRefSymbol=i;
      if (i!=nSymDragNum)
         SymbolList[i]->Draw(DC,BackColor);
      }
}

/*
void NavAidClass::SetSymbolRef(const bool bSetRef)
{
   for (int i=0; i<SymbolList.nGetCount(); i++)
      SymbolList[i]->bRef=false;
   if (bSetRef)
      SymbolList[nCurrentSymbol]->bRef=true;
}
*/

void NavAidClass::CalcSymbolSize(HDC DC)
{
   for (int i=0; i<SymbolList.nGetCount(); i++)
      SymbolList[i]->CalcSize(DC);
}

void NavAidClass::CalcSymbolBounds()
{
	for (int i=0; i<SymbolList.nGetCount(); i++)
		SymbolList[i]->CalcBounds();
}

NAV_HOTSPOT NavAidClass::HotSpot(const Pixel& Pix)
{
   NAV_HOTSPOT Spot=NAV_NO_HOTSPOT;
   nCurrentLine=-1;
   nCurrentSymbol=-1;
   for (int i=0; (i<LineList.nGetCount())&&(Spot==NAV_NO_HOTSPOT); i++)
      {
      if (!LineList[i]->bMulti)
         {
         int nLineEnd=LineList[i]->nInside(Pix);
         if (nLineEnd>=0)
            {
            nCurrentLine=i;
            if (nLineEnd==0)
               Spot=NAV_LINE1_HOTSPOT;
            else
               Spot=NAV_LINE2_HOTSPOT;
            }
         }
      }
   for (int i=0; (i<SymbolList.nGetCount())&&(Spot==NAV_NO_HOTSPOT); i++)
      {
      if (SymbolList[i]->bInside(Pix))
         {
         nCurrentSymbol=i;
         if (!SymbolList[i]->bDispText)
            Spot=NAV_SYMBOL_HOTSPOT;
         else
            Spot=NAV_TEXT_HOTSPOT;
         }
      }
   return Spot;
}

void NavAidClass::DrawSymbolRect(HDC DC, const Pixel& Pix, const LatLong& LL)
{
   SymbolList[nCurrentSymbol]->SetPos(LL);
   SymbolList[nCurrentSymbol]->SetPos(Pix);
   SymbolList[nCurrentSymbol]->DrawRect(DC);
}

void NavAidClass::DragSymbol(HDC DC, const Pixel& Pix, const LatLong& LL, const bool bDrawPrev)
{
	HPEN hPen=::CreatePen(PS_SOLID,1,clLtGray);
	HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
	::SetROP2(DC,R2_XORPEN);
	if ((DragPix.nx!=-10000)&&(bDrawPrev))
		{
		SymbolList[nSymDragNum]->SetPos(DragPix);
		SymbolList[nSymDragNum]->DrawRect(DC);
		}
	DragPix=Pix;
	if (nSymDragNum>=0)
		{
		SymbolList[nSymDragNum]->SetPos(DragPix);
		SymbolList[nSymDragNum]->DrawRect(DC);
		::SetROP2(DC,R2_COPYPEN);
		::SelectObject(DC,hOldPen);
		::DeleteObject(hPen);
		SymbolList[nSymDragNum]->SetPos(LL);
		}
}

void NavAidClass::DragLineStart(const Pixel& Pix, const LatLong& LL)
{
	if (nLineDragNum>=0)
		{
		LineList[nLineDragNum]->SetPos(Pix,Pixel(-10000,-10000));
		LineList[nLineDragNum]->aPos[0]=LL;
		nDragOrigin=0;
		nDragPoint=1;
		}
}

void NavAidClass::DragLine(HDC DC, const Pixel& Pix, const LatLong& LL, const bool bDrawPrev)
{
	if (nLineDragNum>=0)
		{
		HPEN hPen=::CreatePen(PS_SOLID,1,clLtGray);
		HPEN hOldPen=(HPEN)::SelectObject(DC,hPen);
		::SetROP2(DC,R2_XORPEN);
		if ((LineList[nLineDragNum]->aPixPos[nDragPoint].nx!=-10000)&&(bDrawPrev))
			{
			::MoveToEx(DC,LineList[nLineDragNum]->aPixPos[nDragOrigin].nx,LineList[nLineDragNum]->aPixPos[nDragOrigin].ny,NULL);
			::LineTo(DC,LineList[nLineDragNum]->aPixPos[nDragPoint].nx,LineList[nLineDragNum]->aPixPos[nDragPoint].ny);
			}
		LineList[nLineDragNum]->aPixPos[nDragPoint]=Pix;
		LineList[nLineDragNum]->aPos[nDragPoint]=LL;
		::MoveToEx(DC,LineList[nLineDragNum]->aPixPos[nDragOrigin].nx,LineList[nLineDragNum]->aPixPos[nDragOrigin].ny,NULL);
		::LineTo(DC,LineList[nLineDragNum]->aPixPos[nDragPoint].nx,LineList[nLineDragNum]->aPixPos[nDragPoint].ny);
		::SetROP2(DC,R2_COPYPEN);
		::SelectObject(DC,hOldPen);
		::DeleteObject(hPen);
		}
}

bool NavAidClass::bEditArc(const int n, const bool bShowDel)
{
   if (!ArcList.bPacked()) ArcList.Pack();
   if (ArcList[n]->bEdit(false,bShowDel))
      {
      if (ArcList[n]->bDeleted)
         {
         ArcList.Delete(n);
         ArcList.Pack();
         }
      return true;
      }
   else
      return false;
}

#ifdef __DDGRAPH
void NavAidClass::TransformPos(HDC DC, DDGraph* pG)
#else
void NavAidClass::TransformPos(HDC DC, TransGraph* pG)
#endif
{
   for (int nL=0; nL<nLines(); nL++)
      {
      LineStruct* pL=pGetLine(nL);
      if (pL->nGetSegNum()>0)
         {
         for (int i=0; i<pL->nGetSegNum(); i++)
            pL->SetSegPix(i,pG->LL2Pix(pL->GetSegPos(i)));
         }
      else
         pL->SetPos(pG->LL2Pix(pL->aPos[0]),pG->LL2Pix(pL->aPos[1]));
      }
   for (int nS=0; nS<nSymbols(); nS++)
      {
      SymbolStruct* pS=pGetSymbol(nS);
      pS->CalcSize(DC);
      pS->SetPos(pG->LL2Pix(pS->Pos));
      }
   for (int nA=0; nA<nArcs(); nA++)
      {
      ArcStruct* pA=pGetArc(nA);
      pA->CalcPoly(pG);
      }
   for (int nV=0; nV<nVectors(); nV++)
      {
      ManVectStruct* pV=pGetVector(nV);
      pV->SetPos(pG->LL2Pix(pV->aPos[0]),pG->LL2Pix(pV->aPos[1]));
      }
   if (bRefSet())
      RefSymbol.SetPos(pG->LL2Pix(RefSymbol.Pos));
}

void NavAidClass::ExportVectors(const String sFile)
{
   JFile Fil('O',JFile::ASCII_TYPE);
   JFile::FILE_ERROR Error=Fil.Create(sFile);
// char szSt[512];
//	char szVal[512];
	String s,sV;
	for (int i=0; (i<VectList.nGetCount())&&(!Error); i++)
		{
		s.printf(L"%3d  ",i+1);
		SYSTEMTIME T=VectList[i]->DT;
		s=s+sV.sprintf(L"%02u/%02u/%04u %02u:%02u:%02u  ",T.wDay,T.wMonth,T.wYear,T.wHour,T.wMinute,T.wSecond);
		s=s+VectList[i]->aPos[0].sLat()+L"   ";
		s=s+VectList[i]->aPos[0].sLong();
		s=s+sV.sprintf(L"  %03d deg %4.1f kts  ",VectList[i]->nCourse,VectList[i]->dSpeed);;
		s=s+VectList[i]->sShip;
		Error=Fil.TextLine(s);
		}
	if (Error) Fil.ShowError(Error);
}

SymbolStruct* NavAidClass::pGetRef()
{
   return &RefSymbol;
}

void NavAidClass::SetRef(const LatLong& LL)
{
   RefSymbol.Pos=LL;
   RefSymbol.bDispSymbol=true;
}

void NavAidClass::ClearRef()
{
   RefSymbol.bDispText=false;
   RefSymbol.bDispSymbol=false;
}

bool NavAidClass::bRefSet() const
{
   return RefSymbol.bDispText || RefSymbol.bDispSymbol;
}

#ifdef __DDGRAPH
Polar NavAidClass::GetRefPol(const Pixel& Pix, DDGraph* pG)
#else
Polar NavAidClass::GetRefPol(const Pixel& Pix, TransGraph* pG)
#endif
{
   return pG->Transform::GetPol(RefSymbol.Pos,pG->Pix2LL(Pix));
}

void NavAidClass::DrawRef(HDC DC, const TColor BackColor)
{
   if (bRefSet())
      RefSymbol.Draw(DC,BackColor);
}

bool NavAidClass::bEditAll()
{
   bool bOK=false;
//   if (bAvail())
//      {
      NavEditForm=new TNavEditForm(0);
      NavEditForm->SetNav(*this);
      if (NavEditForm->ShowModal()==mrOk)
         {
         NavEditForm->GetNav(*this);
         bOK=true;
         }
      delete NavEditForm;
//      }
   return bOK;
}

void NavAidClass::OpenEdit()
{
   NavEditForm=new TNavEditForm(0);
   NavEditForm->FormStyle=fsStayOnTop;
   NavEditForm->SetNav(*this);
   NavEditForm->Show();
}

NM_DIALOG_STATE NavAidClass::CloseEdit()
{
   NM_DIALOG_STATE State=NavEditForm->GetState();
   switch(State)
      {
      case NM_DIALOG_OK:
         NavEditForm->GetNav(*this);
         delete NavEditForm;
         break;
      case NM_DIALOG_CANCEL:
         delete NavEditForm;
         break;
      default: ;
      }
   return State;
}

void NavAidClass::PackLines()
{
   if (nLines()>0)
      {
      if (!LineList.bPacked()) LineList.Pack();
      for (int i=nLines()-1; i>=0; i--)
         {
         if (LineList[i]->bDeleted)
         LineList.Delete(i);
         LineList.Pack();
         }
      }
}

void NavAidClass::PackSymbols()
{
   if (nSymbols()>0)
      {
      if (!SymbolList.bPacked()) SymbolList.Pack();
      for (int i=nSymbols()-1; i>=0; i--)
         {
         if (SymbolList[i]->bDeleted)
         SymbolList.Delete(i);
         SymbolList.Pack();
         }
      }
}

void NavAidClass::PackArcs()
{
   if (nArcs()>0)
      {
      if (!ArcList.bPacked()) ArcList.Pack();
      for (int i=nArcs()-1; i>=0; i--)
         {
         if (ArcList[i]->bDeleted)
         ArcList.Delete(i);
         ArcList.Pack();
         }
      }
}

void NavAidClass::Pack()
{
   PackLines();
   PackSymbols();
   PackArcs();
}

