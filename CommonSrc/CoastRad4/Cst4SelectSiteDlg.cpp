//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Cst4SelectSiteDlg.h"
#include "JUtils.h"
#include "Cst4SiteInfoDlg.h"
#include "Vista_EditTempSiteDlg.h"
#ifdef _CST4_DICT
	#include "Cst4Dictionary.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCst4SelectSiteForm *Cst4SelectSiteForm;
//---------------------------------------------------------------------------
__fastcall TCst4SelectSiteForm::TCst4SelectSiteForm(TComponent* Owner)
	: TForm(Owner)
{
	wSite=0;
	SystemType=SYSTEMS_ALL;
	sDescr="";
	sAbbr="";
//	pDefs=NULL;
   bShowAll=false;
//	Personality=CST4_COASTRAD_PERS;
}
//---------------------------------------------------------------------------
void __fastcall TCst4SelectSiteForm::FormShow(TObject *Sender)
{
	switch(SystemType)
		{
		case SYSTEMS_AIS:
		case SYSTEMS_RADAR:
		case SYSTEMS_IFF:
		case SYSTEMS_SENSORS:
			TypeGrp->Caption="Sensor";
			break;
		default:
			TypeGrp->Caption="System";
		}

//	bMakeDefs=(pDefs==NULL);
	bool bOK;
//	if (bMakeDefs)
//		{
//		pDefs=new Cst4SiteDefs;
//		bOK=pDefs->bLoadSites();
//		if (!bOK)
//			Close();
//		}
//	else
		bOK=true;
	if (bOK)
		{

		String sVer;
		sVer.printf(L"%3.3f",g_pSiteDefs->dGetVersion());
		Caption="(Select VistaNet Site ID (Version "+sVer+")";

		g_pSiteDefs->GetIndexFromID(wSite,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx<0)
			{
			nCountryIdx=0;
			nLocIdx=0;
			nTypeIdx=0;
			}
		CountryLB->Clear();
		for (int i=0; i<g_pSiteDefs->nGetCountryCnt(); i++)
         CountryLB->Items->Add(g_pSiteDefs->sGetCountry(i));
      if (nCountryIdx<g_pSiteDefs->nGetCountryCnt())
         CountryLB->ItemIndex=nCountryIdx;
      else
         CountryLB->ItemIndex=0;
      CountryLBClick(Sender);
      }
	OKBut->SetFocus();

	#ifdef _CST4_DICT
		g_Dict.Translate(this);	//Form Caption
		g_Dict.Translate(TypeGrp);
		g_Dict.Translate(GroupBox2);
		g_Dict.Translate(GroupBox3);
	#endif

}

//---------------------------------------------------------------------------
void __fastcall TCst4SelectSiteForm::FormClose(TObject */*Sender*/,
		TCloseAction &Action)
{
	if (ModalResult==mrOk)
		{
		int nT=TypeLB->ItemIndex;
		if (nT>=0)
			{
			int n=0;
			nTypeIdx=-1;
			for (int i=0; (i<g_pSiteDefs->nGetTypeCnt(nCountryIdx,nLocIdx))&&(nTypeIdx==-1); i++)
				{
				Cst4SiteDefs::SYSTEM_TYPE Sys=g_pSiteDefs->GetSystemType(nCountryIdx,nLocIdx,i);
				if (bValidType(Sys))
					{
					if (nT==n)
						nTypeIdx=i;
					else
						n++;
					}
				}
			wSite=g_pSiteDefs->wGetID(CountryLB->ItemIndex,LocLB->ItemIndex,nTypeIdx);
			sDescr=g_pSiteDefs->sGetDescrFromID(wSite);
			sAbbr=g_pSiteDefs->sGetAbbrFromID(wSite);
//			if (bMakeDefs)
//				delete pDefs;
			}
		else
			{
			ShowInfo("System Type MUST be selected","Invalid Site Selection");
			Action=caNone;
			}
		}
	else
		{
//		if (bMakeDefs)
//			delete pDefs;
		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4SelectSiteForm::CountryLBClick(TObject *Sender)
{
	nCountryIdx=CountryLB->ItemIndex;

	LocLB->Clear();
	for (int i=0; i<g_pSiteDefs->nGetLocCnt(nCountryIdx); i++)
		LocLB->Items->Add(g_pSiteDefs->sGetLocation(nCountryIdx,i));
	if (nLocIdx<g_pSiteDefs->nGetLocCnt(nCountryIdx))
		LocLB->ItemIndex=nLocIdx;
	else
		LocLB->ItemIndex=0;
	LocLBClick(Sender);
}
//---------------------------------------------------------------------------

bool TCst4SelectSiteForm::bValidType(const Cst4SiteDefs::SYSTEM_TYPE Sys)
{
	if (bShowAll)
   	return true;
   else
   	{
      switch(SystemType)
         {
         case SYSTEMS_ALL:
            return true;
         case SYSTEMS_AIS:
            switch(Sys)
               {
               case Cst4SiteDefs::AIS_TYPE:
               case Cst4SiteDefs::LRIT_TYPE:
               case Cst4SiteDefs::SAT_AIS_TYPE:
                  return true;
               default:
                  return false;
               }
         case SYSTEMS_RADAR:
            switch(Sys)
               {
               case Cst4SiteDefs::RADAR_TYPE:
               case Cst4SiteDefs::TRACKING_RADAR_TYPE:
                  return true;
               default:
                  return false;
               }
         case SYSTEMS_IFF:
            return (Sys==Cst4SiteDefs::IFF_TYPE);
         case SYSTEMS_GPS:
            return (Sys==Cst4SiteDefs::SHIP_POS_TYPE);
         case SYSTEMS_WEATHER:
            return (Sys==Cst4SiteDefs::WEATHER_TYPE);
			case SYSTEMS_CAMERA:
            return (Sys==Cst4SiteDefs::CAMERA_TYPE);
			case SYSTEMS_SENSORS:
            switch(Sys)
               {
               case Cst4SiteDefs::AIS_TYPE:
               case Cst4SiteDefs::RADAR_TYPE:
               case Cst4SiteDefs::IFF_TYPE:
               case Cst4SiteDefs::WEATHER_TYPE:
               case Cst4SiteDefs::LRIT_TYPE:
               case Cst4SiteDefs::SAT_AIS_TYPE:
               case Cst4SiteDefs::SHIP_POS_TYPE:
               case Cst4SiteDefs::TRACKING_RADAR_TYPE:
                  return true;
               default:
                  return false;
               }
         case SYSTEMS_SITEHUB:
            return (Sys==Cst4SiteDefs::SITE_HUB_TYPE);
         case SYSTEMS_HUB:
            switch(Sys)
               {
					case Cst4SiteDefs::VPN_HUB_TYPE:
               case Cst4SiteDefs::LOCAL_HUB_TYPE:
                  return true;
               default:
                  return false;
               }
         case SYSTEMS_DISPLAY:
               return (Sys==Cst4SiteDefs::DISPLAY_TYPE);
         case SYSTEMS_HUB_DISPLAY:
            switch(Sys)
               {
               case Cst4SiteDefs::VPN_HUB_TYPE:
               case Cst4SiteDefs::LOCAL_HUB_TYPE:
               case Cst4SiteDefs::DISPLAY_TYPE:
                  return true;
               default:
                  return false;
               }
			case SYSTEMS_ALARM_PROC:
				switch(Sys)
					{
					case Cst4SiteDefs::ALARMPROC_TYPE:
						return true;
               default:
                  return false;
               }

         case SYSTEMS_REPLAY:
               return (Sys==Cst4SiteDefs::REPLAY_TYPE);
         default:
            return true;

         }
      }
}




void __fastcall TCst4SelectSiteForm::LocLBClick(TObject */*Sender*/)
{
	nLocIdx=LocLB->ItemIndex;
	TypeLB->Clear();
	if (nLocIdx>=0)
		{
		int n=0;
		int nIdx=-1;
		for (int i=0; i<g_pSiteDefs->nGetTypeCnt(nCountryIdx,nLocIdx); i++)
			{
			Cst4SiteDefs::SYSTEM_TYPE Sys=g_pSiteDefs->GetSystemType(nCountryIdx,nLocIdx,i);
			if (bValidType(Sys))
				{
				String sType=g_pSiteDefs->sGetType(nCountryIdx,nLocIdx,i);
				TypeLB->Items->Add(sType);
				if (nTypeIdx==i)
					nIdx=n;
				n++;
				}
			}
		TypeLB->ItemIndex=nIdx;
		}
}
//---------------------------------------------------------------------------

void __fastcall TCst4SelectSiteForm::TypeLBDblClick(TObject */*Sender*/)
{
	ModalResult=mrOk;
}
//---------------------------------------------------------------------------

WORD wSelectVistaSite(TComponent* Owner, const WORD wCurrentSite, const VISTA_SYSTEMS_SELECTION SystemType)
{
	WORD wSite=wCurrentSite;
	Cst4SelectSiteForm=new TCst4SelectSiteForm(Owner);
	Cst4SelectSiteForm->SetSystemType(SystemType);
//	if (g_pSiteDefs)
//		Cst4SelectSiteForm->SetDefs(g_pSiteDefs);
//	else
//		Cst4SelectSiteForm->SetDefs(NULL);
	Cst4SelectSiteForm->SetSite(wSite);
	if (Cst4SelectSiteForm->ShowModal()==mrOk)
		wSite=Cst4SelectSiteForm->wGetSite();
	delete Cst4SelectSiteForm;
	return wSite;
}



void __fastcall TCst4SelectSiteForm::DispAllChkClick(TObject */*Sender*/)
{
	bShowAll=DispAllChk->Checked;
  	LocLBClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TCst4SelectSiteForm::FullInfoButClick(TObject */*Sender*/)
{
   int nT=TypeLB->ItemIndex;
   if (nT>=0)
      {
      int n=0;
      nTypeIdx=-1;
		for (int i=0; (i<g_pSiteDefs->nGetTypeCnt(nCountryIdx,nLocIdx))&&(nTypeIdx==-1); i++)
			{
			Cst4SiteDefs::SYSTEM_TYPE Sys=g_pSiteDefs->GetSystemType(nCountryIdx,nLocIdx,i);
         if (bValidType(Sys))
            {
            if (nT==n)
               nTypeIdx=i;
            else
               n++;
            }
         }
      wSite=g_pSiteDefs->wGetID(CountryLB->ItemIndex,LocLB->ItemIndex,nTypeIdx);
		Cst4SiteInfoForm=new TCst4SiteInfoForm(this);
   	Cst4SiteInfoForm->nID=wSite;
	   Cst4SiteInfoForm->ShowModal();
   	delete Cst4SiteInfoForm;
      }
}
//---------------------------------------------------------------------------

void __fastcall TCst4SelectSiteForm::EditTempButClick(TObject *Sender)
{
   EditTempSiteForm=new TEditTempSiteForm(this);
   EditTempSiteForm->ShowModal();
   delete EditTempSiteForm;
}
//---------------------------------------------------------------------------

