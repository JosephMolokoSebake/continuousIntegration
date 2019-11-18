//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4UserDefs.h"
#include "AISDecoder_210.h"
#include "NMEADecode.h"
#include "Jcrc.h"
#include "JTransform.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)


LatLong Cst4_Tracker_Data::Trans2LL(double& dHeight_m)
{
	LatLong LL;
	if (bAllValid())
		{
		Transform Trans;
		Trans.SetTransformType(Transform::FLAT_TRANS);
		Trans.SetLLRef(PosLL());
		Trans.SetRadarPos(PosLL(),dPosHgt_m());
		LL=Trans.Pol2LL(Polar(nRa_m,fAz_deg));
		double dSRa_m=nRa_m;
		double dGRa_m=dSRa_m*cos(fEl_deg*M_PI/180.0);
		if (dSRa_m-dGRa_m>1.0)
			dHeight_m=sqrt(dSRa_m*dSRa_m-dGRa_m*dGRa_m);
		else
			dHeight_m=0.0;
		}
	else
		LL.Invalidate();
	return LL;
}



Cst4_AIS_Dynamic Cst4_AltAIS_Data::GetDynamicData(const JTime T, const int nCst4ID)
{
	Cst4_AIS_Dynamic Msg(T,nMMSI,GetLL(),dSpeed_kts(),dCourse_deg(),nCst4ID);
	return Msg;
}

void Cst4_AltAIS_Data::SetDynamicData(const Cst4_AIS_Dynamic& D)
{
	nMMSI=D.nGetMMSI();
	SetLL(D.GetLL());
	SetSpeed(D.dSpeed_kts());
	SetCourse(D.dCourse_deg());
}

bool Cst4_AltAIS_Data::bDecodeRaw(const int nCst4ID)
{
	if (pucBlock)
		{
		NMEADecode NMEADec;
		AISDecoderClass AISDec(false);
		String s="";
//		String sV="";
//		bool bCS=false;
		for (WORD w=0; w<wBlockSize; w++)
			{
			NMEADec.AddByte(pucBlock[w]);
			s=NMEADec.sGetSentence();
			if (!s.IsEmpty())
				{
				bool bDataReady=AISDec.bAddSentence(s);
				if (bDataReady)
					{
					AISMsgClass AISMsg;
					AISDec.GetAISMsg(&AISMsg);
					Cst4_AIS_Dynamic AIS_D;
					if (AISMsg.bContainsDynamicData())
						{
						if (AISMsg.bLLValid())
							{
							AIS_D.SetValid();
							AIS_D.SetSource(nCst4ID);

							// Modified by LJVW 2012/08/15
							//AIS_D.SetTime(T_utc);
							AIS_D.SetTime(AISMsg.GetMsgTime_utc());
							// Modified by LJVW 2012/08/15

							AIS_D.SetLL(AISMsg.GetLL());
							if (AISMsg.bSOGValid())
								AIS_D.SetSpeed(AISMsg.dGetSOG());
							if (AISMsg.bCOGValid())
								AIS_D.SetCourse(AISMsg.dGetCOG());
							if (AISMsg.bMMSIValid())
								AIS_D.SetMMSI(AISMsg.nGetMMSI());
							}
						}
         		if (AISMsg.bContainsVoyageStaticData())
//					if (AISMsg.bContainsUpdatedExtInfo())
						{
						Stat.SetValid();
						Stat.SetTime(AISMsg.GetMsgTime_utc());
						BYTE aucInfo[4096];
						int nSize=AISMsg.dwGetExtraInfoBuf(aucInfo,4096);
						Stat.Set(AISMsg.nGetMMSI(),aucInfo,nSize,nCst4ID);
						}
					return true;
					}
				}
			}
		}
	return false;
}

void Cst4_AltAIS_Data::CalcCRC()
{
	RockCRC CRC(16,0x1021,0xffff,false,false,0x0000);
	for (WORD w=0; w<wBlockSize; w++)
		CRC.Next(pucBlock[w]);
	wCRC=(WORD)(CRC.ulGetCRC()&0x0000FFFF);
}

String Cst4_AltAIS_Data::sExtract()
{
	String s="";
   String sV;
   WORD wCRC1=wGetCRC();

	RockCRC CRC(16,0x1021,0xffff,false,false,0x0000);
	for (WORD w=0; w<wBlockSize; w++)
		CRC.Next(pucBlock[w]);
	WORD wCRC2=(WORD)(CRC.ulGetCRC()&0x0000FFFF);

   sV.printf(L"0x%04X",wCRC1);
   s=s+"Embedded CRC:   "+sV+"\n\r";
   sV.printf(L"0x%04X",wCRC2);
   s=s+"Calculated CRC: "+sV+"\n\r";
   s=s+"MMSI: "+String(nMMSI)+"\n\r";

   LatLong LL=GetLL();
   if (LL.bValid())
      {
      s=s+"Position: "+LL.sLL()+"\n\r";
      if (bValidSpeed())
         {
         sV.printf(L"%1.1f",dSpeed_kts());
         s=s+"Speed: "+sV+" kts"+"\n\r";
         }
      else
         s=s+"Speed: INVALID"+"\n\r";
      if (bValidCourse())
         {
         sV.printf(L"%1.1f",dCourse_deg());
         s=s+"Course: "+sV+" deg"+"\n\r";
         }
      else
         s=s+"Course: INVALID"+"\n\r";
      }
   else
      {
      s=s+"Position: INVALID"+"\n\r";
      s=s+"Speed: INVALID"+"\n\r";
      s=s+"Course: INVALID"+"\n\r";
		}


	if (bDecodeRaw(0))
		{
		if (Stat.bValid())
			{
			String sCall;
			int nIMO;
			int nType;
			String sName=Stat.sBasicInfo(nType,nIMO,sCall);
			s=s+"Ship Ship Name: "+sName+"\n\r";
			s=s+"Ship Callsign: "+sCall+"\n\r";
			}
		}

	char acNMEA[4096];
   int nNMEASize=nGetRawNMEA(acNMEA);
   acNMEA[nNMEASize]=0;
   s=s+"NMEA: "+String(acNMEA);


	return s;
}







