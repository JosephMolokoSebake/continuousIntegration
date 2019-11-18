//---------------------------------------------------------------------------

#pragma hdrstop

#include "JEdit.h"
#include "JUtils.h"
#include "JColorDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool JEdit::EditStruct::bCheckValid(const double d, const int nParCnt)
{
   bool bValid=false;
   int n;
   switch(DataType)
      {
      case INT_TYPE:
         n=d;
         if (nParCnt==1)
            {
            if ((n>=nMin)&&(n<=nMax))
               bValid=true;
            else
               {
               String s;
               s.printf(L"Value must be between %d and %d",nMin,nMax);
               ShowError(s,"Numeric Entry Error - "+sDescr);
               }
            }
         else
            ShowError("Invalid Number Format","Numeric Entry Error - "+sDescr);
      	break;
      case DOUBLE_TYPE:
         if (nParCnt==1)
            {
            if ((d>=dMin)&&(d<=dMax))
               bValid=true;
            else
               {
               String s,sV,sMin,sMax;
               sV.printf(L"%d.%df",nDeci,nDeci);
               sV="%"+sV;
               sMin.printf(sV.c_str(),dMin);
               sMax.printf(sV.c_str(),dMax);
               s="Value must be between "+sMin+" and "+sMax;
               ShowError(s,"Numeric Entry Error - "+sDescr);
               }
            }
         break;
   	default:
			ShowError("Invalid Number Format","Numeric Entry Error - "+sDescr);

      }
   return bValid;
}

JEdit::JEdit()
{
   nInvalid=-1000000;
   dInvalid=-1e9;
   Init();
}

JEdit::~JEdit()
{


}


int JEdit::nFindIndex(TEdit* pEd1, TEdit* pEd2, const bool bVerbose)
{
	if (!EditList.bPacked())
		EditList.Pack();
	int nIdx=-1;
	for (int i=0; i<EditList.nGetCount(); i++)
		{
		if ((EditList[i]->pEd1==pEd1)&&(EditList[i]->pEd2==pEd2))
			{
			nIdx=i;
			i=EditList.nGetCount();
			}
		}
   if ((nIdx<0)&&(bVerbose))
      {
		ShowError("Invalid TEdit Pointer","nFindIndex");
		bDataValid=false;
      }
   return nIdx;
}

int JEdit::nFindIndex(TEdit* pEd1, const bool bVerbose)
{
	if (!EditList.bPacked())
		EditList.Pack();
	int nIdx=-1;
	for (int i=0; i<EditList.nGetCount(); i++)
		{
      bool bFound;//=false;
		if (EditList[i]->pEd1==pEd1)
			{
         bFound=true;
         if (bFound)
            {
				nIdx=i;
				i=EditList.nGetCount();
            }
			}
		}
   if ((nIdx<0)&&(bVerbose))
      {
		ShowError("Invalid TEdit Pointer","nFindIndex");
		bDataValid=false;
      }
   return nIdx;
}

JEdit::EditStruct* JEdit::pCheckIndex(const int nIdx, const String sDescr)
{
   bool bOK=true;
	if ((nIdx<EditList.nGetCount())&&(nIdx>=0))
		{
		if (!EditList.bPacked())
			EditList.Pack();
		if (!EditList[nIdx]->pEd1)
			{
			ShowError("Invalid TEdit pointer",sDescr);
         bOK=false;
			bDataValid=false;
			}
		}
	else
		{
      String s;
		s.printf(L"Invalid Index %d/%d",nIdx,EditList.nGetCount());
		ShowError(s,sDescr);
		bDataValid=false;
      bOK=false;
		}
   if (bOK)
      {
      EditStruct* p;
      p=EditList[nIdx];
      return p;
      }
   else
   	return NULL;
}

//*************************************************************************
//Set ReadOnly Values

void JEdit::SetValRO(TEdit* pEd,const int n)
{
   pEd->ReadOnly=true;
	pEd->Text=String(n);
}

void JEdit::SetValRO(TEdit* pEd,const DWORD dw)
{
   pEd->ReadOnly=true;
	pEd->Text=String(dw);
}

void JEdit::SetVal(TEdit* pEd,const double d, const int nDeci)
{
	String s,sV;
   int nD=nDeci;
   if (nD<0)
         nD=1;
	sV.printf(L"%d.%df",nD,nD);
   sV="%"+sV;
	s.printf(sV.c_str(),d);
	pEd->Text=s;
}

void JEdit::SetValRO(TEdit* pEd,const double d, const int nDeci)
{
   pEd->ReadOnly=true;
   SetVal(pEd,d,nDeci);
}

void JEdit::SetTimeRO(TEdit* pTimeEd, const JTime& T)
{
   pTimeEd->ReadOnly=true;
	pTimeEd->Text=T.sTime(false,false);
}

void JEdit::SetTimeRO(TEdit* pTimeEd, const int& nT_ms)
{
   JTime T;
   T.SetTime(nT_ms);
   SetTimeRO(pTimeEd,T);
}

void JEdit::SetDateRO(TEdit* pDateEd, const JTime& T)
{
   pDateEd->ReadOnly=true;
	pDateEd->Text=T.sDate(false,false);
}

void JEdit::SetDateTimeRO(TEdit* pDateEd, TEdit* pTimeEd, const JTime& T)
{
	SetDateRO(pDateEd,T);
	SetTimeRO(pTimeEd,T);
}

void JEdit::SetBinRO(TEdit* pEd, const int nVal, const int nBits)
{
   String s="";
   int nMask;
   for (int i=0; i<nBits; i++)
   	{
      if ((i==8) || (i==16) ||(i==24))
         s=" "+s;
      nMask=1<<i;
      if (nVal & nMask)
         s="1"+s;
      else
         s="0"+s;
      }
   pEd->Text=s;
}

void JEdit::SetHexRO(TEdit* pEd, const int nVal, const int nBits)
{
   String s="";
   int nMask;
   for (int i=0; i<nBits; i++)
   	{
      if ((i==8) || (i==16) ||(i==24))
         s=" "+s;
      nMask=1<<i;
      if (nVal & nMask)
         s="1"+s;
      else
         s="0"+s;
      }
   pEd->Text=s;
}


void JEdit::SetLimits(TEdit* pEd,const String sDescr, const double dMin, const double dMax)
{
	int nIdx=nFindIndex(pEd);
	if (nIdx>=0)
      {
      EditList[nIdx]->sDescr=sDescr;
      switch(EditList[nIdx]->DataType)
         {
         case INT_TYPE:
            EditList[nIdx]->nMin=dMin+0.5;
            EditList[nIdx]->nMax=dMax+0.5;
            if (EditList[nIdx]->nMin>=0)
               EditList[nIdx]->pEd1->NumbersOnly=true;
            break;
         case DOUBLE_TYPE:
		      EditList[nIdx]->dMin=dMin;
   		   EditList[nIdx]->dMax=dMax;
            break;
         default: ;
         }
      }
}

void JEdit::SetLastLimits(const String sDescr, const double dMin, const double dMax)
{
	EditStruct* p=pCheckIndex(nLastIdx,sDescr);
	if (p)
      {
      p->sDescr=sDescr;
      switch(p->DataType)
         {
         case INT_TYPE:
            p->nMin=dMin+0.5;
            p->nMax=dMax+0.5;
            if (p->nMin>=0)
               p->pEd1->NumbersOnly=true;
            break;
         case DOUBLE_TYPE:
		      p->dMin=dMin;
   		   p->dMax=dMax;
            break;
         default: ;
         }
      }
}

void JEdit::SetLastLimits(const double dMin, const double dMax)
{
	EditStruct* p=pCheckIndex(nLastIdx,"");
	if (p)
      {
      switch(p->DataType)
         {
         case INT_TYPE:
            p->nMin=dMin+0.5;
            p->nMax=dMax-0.5;
            if (p->nMin>=0)
               p->pEd1->NumbersOnly=true;
            break;
         case DOUBLE_TYPE:
		      p->dMin=dMin;
   		   p->dMax=dMax;
            break;
         default: ;
         }
      }
}


//*************************************************************************
//Integer
//*************************************************************************


int JEdit::nSetVal(TEdit* pEd, const int n, const String sDescr, const int nMin, const int nMax)
{
	EditStruct* p;
   int nIdx=nFindIndex(pEd,false);
   if (nIdx>=0)
      p=EditList[nIdx];
   else
      {
		p=new EditStruct;
		nIdx=EditList.nAdd(p);
      EditList.Pack();
      }
   p->DataType=INT_TYPE;
	p->pEd1=pEd;
   if (n==nInvalid)
		pEd->Text="";
   else
		pEd->Text=String(n);
   nLastIdx=nIdx;
   if (!sDescr.IsEmpty())
   	{
      p->sDescr=sDescr;
      EditList[nIdx]->nMin=nMin;
      EditList[nIdx]->nMax=nMax;
      if (EditList[nIdx]->nMin>=0)
         EditList[nIdx]->pEd1->NumbersOnly=true;
   	}
   return nIdx;
}

int JEdit::nSetVal(const String sDescr, TEdit* pEd, const int n)
{
	EditStruct* p;
   int nIdx=nFindIndex(pEd,false);
   if (nIdx>=0)
      p=EditList[nIdx];
   else
      {
		p=new EditStruct;
		nIdx=EditList.nAdd(p);
      EditList.Pack();
      }
   p->sDescr=sDescr;
   p->DataType=INT_TYPE;
	p->pEd1=pEd;
   if (n==nInvalid)
		pEd->Text="";
   else
		pEd->Text=String(n);
   nLastIdx=nIdx;
   return nIdx;
}

void JEdit::SetVal(const int nIdx, const int nVal)
{
   EditStruct* p=pCheckIndex(nIdx,"Set Integer");
   if (p)
      {
      if (nVal==nInvalid)
         p->pEd1->Text="";
      else
	      p->pEd1->Text=String(nVal);
   	p->DataType=INT_TYPE;
	   nLastIdx=nIdx;
		}
}

int JEdit::nGetVal(const int nIdx)
{
   if (bDataValid)
   	{
      EditStruct* p=pCheckIndex(nIdx,"Get Integer");
      if (p)
         {
         AnsiString s=p->pEd1->Text.Trim();
         if (!s.IsEmpty())
            {
            int n;
            int nPar=sscanf(s.c_str(),"%d",&n);
            if (nPar==1)
               {
               if ((n>=p->nMin)&&(n<=p->nMax))
                  return n;
               else
                  {
                  String s;
                  s.printf(L"Value must be between %d and %d",p->nMin,p->nMax);
                  ShowError(s,"Numeric Entry Error - "+p->sDescr);
                  bDataValid=false;
                  }
               }
            else
               {
               ShowError("Invalid Number Format","Numeric Entry Error - "+EditList[nIdx]->sDescr);
               bDataValid=false;
               }
            }
         }
      }
	return nInvalid;
}

void JEdit::GetVal(const int nIdx, int& nVal)
{
   nVal=nGetVal(nIdx);
}


int JEdit::nGetVal(const int nIdx, const int nMin, const int nMax)
{
   if (bDataValid)
   	{
      if (nIdx<EditList.nGetCount())
         {
         EditStruct* p;
         if (!EditList.bPacked())
            EditList.Pack();
         p=EditList[nIdx];
         p->nMin=nMin;
         p->nMax=nMax;
         return nGetVal(nIdx);
         }
      }
   return nInvalid;
}

void JEdit::GetVal(const int nIdx, int& nVal, const int nMin, const int nMax)
{
   if (bDataValid)
	   nVal=nGetVal(nIdx,nMin,nMax);
}

int JEdit::nGetVal(TEdit* pEd)
{
   if (bDataValid)
   	{
      int nIdx=nFindIndex(pEd);
      if (nIdx>=0)
         return nGetVal(nIdx);
      }
   return nInvalid;
}

int JEdit::nGetVal(TEdit* pEd, const int nMin, const int nMax)
{
   if (bDataValid)
   	{
      int nIdx=nFindIndex(pEd);
      if (nIdx>=0)
         return nGetVal(nIdx,nMin,nMax);
      }
  	return nInvalid;
}


void JEdit::GetVal(TEdit* pEd, int& nVal)
{
   if (bDataValid)
	   nVal=nGetVal(pEd);
}

void JEdit::GetVal(TEdit* pEd, int& nVal, const int nMin, const int nMax)
{
   if (bDataValid)
	   nVal=nGetVal(pEd,nMin,nMax);
}


//*************************************************************************
//Double
//*************************************************************************

int JEdit::nSetVal(TEdit* pEd, const double d, const int nDeci, const String sDescr, const double dMin, const double dMax)
{
	EditStruct* p=new EditStruct;
   p->nDeci=nDeci;
	p->pEd1=pEd;
	if (d<dInvalid+1e-10)
		pEd->Text="";
	else
		SetVal(pEd,d,nDeci);
   p->DataType=DOUBLE_TYPE;
	int nIdx=EditList.nAdd(p);
   EditList.Pack();
   nLastIdx=nIdx;
   if (!sDescr.IsEmpty())
   	{
      p->sDescr=sDescr;
      p->dMin=dMin;
	   p->dMax=dMax;
   	}
   return nIdx;
}

int JEdit::nSetVal(const String sDescr, TEdit* pEd, const double d, const int nDeci)
{
	EditStruct* p=new EditStruct;
   p->nDeci=nDeci;
	p->pEd1=pEd;
	if (d<dInvalid+1e-10)
		pEd->Text="";
	else
		SetVal(pEd,d,nDeci);
   p->sDescr=sDescr;
   p->DataType=DOUBLE_TYPE;
	int nIdx=EditList.nAdd(p);
   EditList.Pack();
   nLastIdx=nIdx;
   return nIdx;
}

void JEdit::SetVal(const int nIdx, const double dVal, const int nDeci)
{
	EditStruct* p=pCheckIndex(nIdx,"Set Double");
   if (p)
   	SetVal(p->pEd1,dVal,nDeci);
   nLastIdx=nIdx;
}

double JEdit::dGetVal(const int nIdx)
{
   if (bDataValid)
   	{
      EditStruct* p=pCheckIndex(nIdx,"Set Double");
      if (p)
         {
         AnsiString s=p->pEd1->Text.Trim();
         if (!s.IsEmpty())
            {
            double d;
            int nParCnt=sscanf(s.c_str(),"%lf",&d);
            if (p->bCheckValid(d,nParCnt))
               return d;
            else
               bDataValid=false;
            return dInvalid;
            }
         }
      }
   return dInvalid;
}

double JEdit::dGetVal(const int nIdx, const double dMin, const double dMax)
{
   if (bDataValid)
   	{
      EditStruct* p=pCheckIndex(nIdx,"Get Double");
      if (p)
         {
         p->dMin=dMin;
         p->dMax=dMax;
         return dGetVal(nIdx);
         }
      }
   return dInvalid;
}

double JEdit::dGetVal(TEdit* pEd, const double dMin, const double dMax)
{
   int nIdx=nFindIndex(pEd);
	if (nIdx>=0)
		return dGetVal(nIdx,dMin,dMax);
	else
	   return dInvalid;
}

void JEdit::GetVal(TEdit* pEd, double& dVal, const double dMin, const double dMax)
{
   if (bDataValid)
	   dVal=dGetVal(pEd,dMin,dMax);
}

double JEdit::dGetVal(TEdit* pEd)
{
   if (bDataValid)
   	{
      int nIdx=nFindIndex(pEd);
      if (nIdx>=0)
         return dGetVal(nIdx);
      }
   return dInvalid;
}

void JEdit::GetVal(TEdit* pEd, double& dVal)
{
   if (bDataValid)
	   dVal=dGetVal(pEd);
}

//*************************************************************************
//LatLong
//*************************************************************************

void JEdit::SetLL(TEdit* pLatEd, TEdit* pLongEd, const LatLong LL)
{
	pLatEd->Text=LL.sLat();
	pLongEd->Text=LL.sLong();
}

void JEdit::SetLLRO(TEdit* pLatEd, TEdit* pLongEd, const LatLong LL)
{
   pLatEd->ReadOnly=true;
   pLongEd->ReadOnly=true;
	pLatEd->Text=LL.sLat();
	pLongEd->Text=LL.sLong();
}


int JEdit::nSetLL(const String sDescr, TEdit*pLatEd, TEdit*pLongEd, const LatLong LL)
{
	SetLL(pLatEd,pLongEd,LL);
	EditStruct* p;
   int nIdx=nFindIndex(pLatEd,pLongEd,false);
   if (nIdx>=0)
     {
      p=EditList[nIdx];
		p->sDescr=sDescr;
	   nLastIdx=nIdx;
     	return nIdx;
		}
   else
      {
		p=new EditStruct;
		p->sDescr=sDescr;
		p->pEd1=pLatEd;
		p->pEd2=pLongEd;
		return EditList.nAdd(p);
      }
}

void JEdit::SetLL(const int nIdx, const LatLong LL)
{
   EditStruct* p=pCheckIndex(nIdx,"");
   if (p)
		SetLL(p->pEd1,p->pEd2,LL);
   nLastIdx=nIdx;
}

LatLong JEdit::GetLL(const int nIdx)
{
   LatLong LL;
   LL.Invalidate();
   if (bDataValid)
   	{
      EditStruct*p=pCheckIndex(nIdx,"Position");
      if (p)
         {
         bool bOK;
         int nDeg,nMin,nMilli;
         double dVal;

         AnsiString sLat=p->pEd1->Text;
         if (sLat.Length()==10)
            {
            bOK=true;
            try
               {
               nDeg=sLat.SubString(1,2).ToInt();
               nMin=sLat.SubString(4,2).ToInt();
               nMilli=sLat.SubString(7,3).ToInt();
               }
            catch(const EConvertError& e)
               {
               bOK=false;
               }
            if (bOK)
               bOK=((sLat[10]=='S')||(sLat[10]=='s')||(sLat[10]=='N')||(sLat[10]=='n'));
            if (bOK)
               {
               if ((nDeg>=0)&&(nDeg<=90)&&(nMin>=0)&&(nMin<60)&&(nMilli>=0)&&(nMilli<=999))
                  {
                  dVal=(nDeg*60000+nMin*1000+nMilli)/60000.0;
                  if ((sLat[10]=='S')||(sLat[10]=='s'))
                     dVal*=-1.0;
                  }
               else
                  bOK=false;
               }
            if (bOK)
               {
               LL.dLat=dVal;
               AnsiString sLong=p->pEd2->Text;
               int nOffs=sLong.Length()-10;
               bOK=((nOffs==0)||(nOffs==1));
               if (bOK)
                  bOK=(sLong[nOffs+3]==':');
               if (bOK)
                  {
                  try
                     {
                     nDeg=sLong.SubString(1,2+nOffs).ToInt();
                     nMin=sLong.SubString(4+nOffs,2).ToInt();
                     nMilli=sLong.SubString(7+nOffs,3).ToInt();
                     }
                  catch(const EConvertError& e)
                     {
                     bOK=false;
                     }
                  if (bOK)
                     bOK=((sLong[10+nOffs]=='W')||(sLong[10+nOffs]=='w')||(sLong[10+nOffs]=='E')||(sLong[10+nOffs]=='e'));
                  if (bOK)
                     {
                     if ((nDeg>=0)&&(nDeg<=180)&&(nMin>=0)&&(nMin<60)&&(nMilli>=0)&&(nMilli<=999))
                        {
                        dVal=(nDeg*60000+nMin*1000+nMilli)/60000.0;
                        if ((sLong[10+nOffs]=='W')||(sLong[10+nOffs]=='w'))
                           dVal*=-1.0;
                        }
                     else
                        bOK=false;
                     if (bOK)
                        LL.dLong=dVal;
                     }
                  }
               }
            }
         if (!bOK)
            bDataValid=false;
         }
      }
   return LL;
}

void JEdit::GetLL(const int nIdx, LatLong& LL)
{
   if (bDataValid)
	   LL=GetLL(nIdx);
}

LatLong JEdit::GetLL(TEdit* pLatEd, TEdit* pLongEd)
{
   LatLong LL;
   LL.Invalidate();
   if (bDataValid)
   	{
	   int nIdx=nFindIndex(pLatEd,pLongEd);
		if (nIdx>=0)
			return GetLL(nIdx);
      }
   return LL;
}

void JEdit::GetLL(TEdit* pLatEd, TEdit* pLongEd, LatLong& LL)
{
   if (bDataValid)
	   LL=GetLL(pLatEd,pLongEd);
}


   //DateTime
//*************************************************************************

void JEdit::SetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime, JTime T)
{
   SetDate(pDate,T);
   SetTime(pTime,T);
}

void JEdit::SetDate(TDateTimePicker* pDate, JTime T)
{
   pDate->Kind=dtkDate;
   pDate->DateFormat=dfShort;
   pDate->DateMode=dmComboBox;
   pDate->Format="dd/MM/yyyy";
   if (T.bValid())
	   pDate->Date=T.GetDate();
   else
      {
      JTime T1(true);
	   pDate->Date=T1.GetDate();
      }
}

void JEdit::SetTime(TDateTimePicker* pTime, JTime T)
{
   pTime->Kind=dtkTime;
   pTime->Format="HH:mm:ss";
   pTime->Time=T.GetTime();
}

JTime JEdit::GetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime)
{
   JTime T;
   T.SetDate(pDate->Date);
  	T.SetTime(pTime->Time);
   return T;
}

void JEdit::GetDateTime(TDateTimePicker* pDate, TDateTimePicker* pTime, JTime& T)
{
   T=GetDateTime(pDate,pTime);
}

JTime JEdit::GetDate(TDateTimePicker* pDate)
{
   JTime T;
   T.SetDate(pDate);
   return T;
}

void JEdit::GetDate(TDateTimePicker* pDate, JTime& T)
{
   T=GetDate(pDate);
}

JTime JEdit::GetTime(TDateTimePicker* pTime)
{
   JTime T;
   T.SetTime(pTime->Time);
   return T;
}

void JEdit::GetTime(TDateTimePicker* pTime, JTime& T)
{
	T=GetTime(pTime);
}


//*************************************************************************

void JEdit::SetPanel(const String sDescr, TPanel* pPan, TColor Col)
{
   PanelStruct* pFound=NULL;
   PanelStruct* p;
   PanList.GoFirst();
   while((!PanList.bLast()) &&(!pFound))
      {
      p=PanList.pNext();
      if (p->pPan==pPan)
         pFound=p;
      }
   if (!pFound)
   	{
      pPan->ParentColor=false;
      pPan->ParentBackground=false;
      pFound=new PanelStruct;
      pFound->pPan=pPan;
      PanList.nAdd(pFound);
      PanList.Pack();
      }
   pPan->Color=Col;
   pFound->sDescr=sDescr;
   pFound->pPan=pPan;
}

void JEdit::SelectPanel(TPanel* pPan, const TColor BackCol)
{
   PanelStruct* pFound=NULL;
   PanelStruct* p;
   PanList.GoFirst();
   while((!PanList.bLast()) &&(!pFound))
      {
      p=PanList.pNext();
      if (p->pPan==pPan)
         pFound=p;
      }
   if (!pFound)
		ShowError("Invalid TPanel Pointer","Pointer Not Found");
   else
      {
		pPan->Color=SelectColor(pFound->sDescr,pPan->Color,BackCol);
      }
}

TColor JEdit::GetColor(TPanel* pPan)
{
   return pPan->Color;
}

