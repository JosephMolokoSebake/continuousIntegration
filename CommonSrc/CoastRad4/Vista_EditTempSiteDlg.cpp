//---------------------------------------------------------------------------

#include <jpch.h>
#pragma hdrstop

#include "Vista_EditTempSiteDlg.h"
#include "JFileDlg.h"
#include "JIniFile.h"
#include "Cst4SiteDefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditTempSiteForm *EditTempSiteForm;
//---------------------------------------------------------------------------
__fastcall TEditTempSiteForm::TEditTempSiteForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditTempSiteForm::FormShow(TObject *Sender)
{
   ReadPar();
}
//---------------------------------------------------------------------------
void __fastcall TEditTempSiteForm::FormClose(TObject *Sender, TCloseAction &Action)

{
	if (ModalResult==mrOk)
      {
      SavePar();
      g_pSiteDefs->SetTempLocation(sLocDescr);
      g_pSiteDefs->SetTempAISPar(AISLL,nAISHeight_m);
      g_pSiteDefs->SetTempRadarPar(RadarLL,nRadarHeight_m);
      }
}
//---------------------------------------------------------------------------
void __fastcall TEditTempSiteForm::ReadButClick(TObject *Sender)
{
   String sFil=sSelectOpenFile("Temporary Site Definitions (*.TempSite","*.TempSite","Specify File",g_pSiteDefs->sGetSiteDefFolder());
   if (!sFil.IsEmpty())
	   ReadPar(sFil);

}
//---------------------------------------------------------------------------
void __fastcall TEditTempSiteForm::SaveButClick(TObject *Sender)
{
   String sFil=sSelectSaveFile("Temporary Site Definitions (*.TempSite","*.TempSite","Select File",g_pSiteDefs->sGetSiteDefFolder());

   if (!sFil.IsEmpty())
      {
      if (bGetPar())
	      SavePar(sFil);
      }

}
//---------------------------------------------------------------------------

void TEditTempSiteForm::SetPar()
{
   Ed.Init();
   LocEd->Text=sLocDescr;
   Ed.nSetLL("AIS Position",AISLatEd,AISLongEd,AISLL);
   Ed.nSetVal("AIS Height",AISHeightEd,nAISHeight_m);
   Ed.SetLastLimits("AIS Height",0,9999);
   Ed.nSetLL("Radar Position",RadarLatEd,RadarLongEd,RadarLL);
   Ed.nSetVal("Radar Height",RadarHeightEd,nRadarHeight_m);
   Ed.SetLastLimits("Radar Height",0,9999);
}

bool TEditTempSiteForm::bGetPar()
{
   sLocDescr=LocEd->Text;
   Ed.GetLL(AISLatEd,AISLongEd,AISLL);
   Ed.GetVal(AISHeightEd,nAISHeight_m);
   Ed.GetLL(RadarLatEd,RadarLongEd,RadarLL);
   Ed.GetVal(RadarHeightEd,nRadarHeight_m);
   return true;//Ed.bValid();
}

void TEditTempSiteForm::ReadPar(const String sFil)
{
   String sF=sFil;
   if (sF.IsEmpty())
      {
      String sFolder=g_pSiteDefs->sGetSiteDefFolder();
      sF=IncludeTrailingBackslash(sFolder)+"Default.TempSite";
      }
   if (bFileExist(sF))
      g_pSiteDefs->ReadTempFile(sF,sLocDescr,AISLL,nAISHeight_m,RadarLL,nRadarHeight_m);
   else
      {
      //Get parameters from SiteDefs
   	sLocDescr=g_pSiteDefs->sGetTempLocation();
   	g_pSiteDefs->GetTempAISPar(AISLL,nAISHeight_m);
   	g_pSiteDefs->GetTempRadarPar(RadarLL,nRadarHeight_m);
      }
   SetPar();
}

void TEditTempSiteForm::SavePar(const String sFil)
{
   if (bGetPar())
      {
      String sF=sFil;
      if (sF.IsEmpty())
         sF=IncludeTrailingBackslash(g_pSiteDefs->sGetSiteDefFolder())+"Default.TempSite";
   	JIniFile Ini(sF,JIniFile::INI_WRITE);
      int nVer=TEMP_SITE_VERSION;
      Ini.Write("GENERAL","Version",nVer);
      Ini.Write("Location",sLocDescr);
      Ini.Write("AIS","Position",AISLL);
      Ini.Write("Height",nAISHeight_m);
      Ini.Write("RADAR","Position",RadarLL);
      Ini.Write("Height",nRadarHeight_m);
      }

}

