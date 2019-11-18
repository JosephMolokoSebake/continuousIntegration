//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4SiteInfoDlg.h"
#include "Cst4SiteDefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCst4SiteInfoForm *Cst4SiteInfoForm;
//---------------------------------------------------------------------------
__fastcall TCst4SiteInfoForm::TCst4SiteInfoForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCst4SiteInfoForm::FormShow(TObject */*Sender*/)
{

//	Cst4SiteDefs* pDefs;
//	if (g_pSiteDefs)
//		pDefs=g_pSiteDefs;
//	else
//		g_pSiteDefs->bLoadSites();
	int nCountryIdx,nLocIdx,nTypeIdx;
	g_pSiteDefs->GetIndexFromID(nID,nCountryIdx,nLocIdx,nTypeIdx);

   Memo->Clear();
	String s,sV;
	s=g_pSiteDefs->sGetCountry(nCountryIdx);
	Memo->Lines->Add("Country/System: "+s);
	s=g_pSiteDefs->sGetLocation(nCountryIdx,nLocIdx);
	Memo->Lines->Add("Location:       "+s);
   s=g_pSiteDefs->sGetType(nCountryIdx,nLocIdx,nTypeIdx);
	Memo->Lines->Add("Type:           "+s);
   LatLong LL=g_pSiteDefs->GetLL(nCountryIdx,nLocIdx,nTypeIdx);
   if (g_pSiteDefs->bIsSensor(nID))
   	{
		Memo->Lines->Add("Position:       "+LL.sLL());
   	int nHgt=g_pSiteDefs->nGetHeight_m(nCountryIdx,nLocIdx,nTypeIdx);
		Memo->Lines->Add("Sensor Height:  "+String(nHgt)+" m");
		double dRad_km=g_pSiteDefs->dGetRadius_km(nCountryIdx,nLocIdx,nTypeIdx);
   	if (dRad_km>0.1)
	   	{
			sV.printf(L"%1.1f km",dRad_km);
			Memo->Lines->Add("Max Distance:   "+sV);
	      }
		String sAddr=g_pSiteDefs->sGetDesigAddrFromID(nID);
      if (!sAddr.IsEmpty())
         {
         int nPort=g_pSiteDefs->nGetDesigPortFromID(nID);
         Memo->Lines->Add("Designation IP Address: "+sAddr);
         Memo->Lines->Add("Designation Port:       "+String(nPort));
         }
      }
//	if (!g_pSiteDefs)
//   	delete pDefs;

}
//---------------------------------------------------------------------------


