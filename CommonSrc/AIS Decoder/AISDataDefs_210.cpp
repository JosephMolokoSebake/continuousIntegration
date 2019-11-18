/* 	----------------- CONFIGURATION HEADER START --------------------------
 *
 *		MODULE TITLE		AIS Data Definitions
 *    -----------------------------------------------------------------------
 *		FILE NAME			AISDataDefs_210.cpp
 *		COMPILER				Borland C++ Builder 15 (XE)
 *		PROCESSOR			Pentium IV
 *		OS						MS Windows 7
 *		PROGRAMMER			Jacques van Wyk
 *    -----------------------------------------------------------------------
 *		ABSTRACT:			AIS Data Definitions
 *
 *       V1_00          LJVW 2006/08/31
 *                      - Initial Version
 *		   V2_00          LJVW 16/01/2009
 *                      - Module rewritten
 *		   V2_01          JGT 2010/08/10
 *			               - Modified String handling to use Unicode strings -
 *                        where c_str or Length is used
 *                      - Use #pragma option push -bi- around enums to ensure
 *                        that enums are not used as integers
 *       V2_10          LJVW 2011/12/07
 *                      - Added checking and removal of spaces in call sign
 *       V2_10_01
 *       V2_10_02
 *       V2_10_03
 *       V2_10_04
 *       V2_10_05			LJVW 2013/11/08
 *                      - Added support for AIS-SART devices (and suppression of alarms)
 *
 *
/* 	----------------- CONFIGURATION HEADER START -------------------------- */

#include "jpch.h"
#pragma hdrstop

#include "AISDataDefs_210.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

const String AISMsgClass::Months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

String AISMsgClass::sGetShipAndCargoType(const String sPrefixDelimiter,const String sPostfixDelimiter)
{
	if (bShipAndCargoTypeValid())
	{
		int n=nShipAndCargoType;
		String sShipCargo;
		switch (nShipAndCargoType/10)
		{
			case 0: sShipCargo="Not Available";				break; //default
			case 1: sShipCargo="";						break; // reserved for future use
			case 2: sShipCargo="WIG"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
			case 3: sShipCargo="Vessel"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
			case 4: sShipCargo="HSC"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
			case 5: sShipCargo=sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);				break;
			case 6: sShipCargo="Passenger Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);		break;
			case 7: sShipCargo="Cargo Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
			case 8: sShipCargo="Tanker"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);			break;
			case 9: sShipCargo="Other Type Of Ship"+sGetCargoSecond(n,sPrefixDelimiter,sPostfixDelimiter);		break;
			default: sShipCargo="";
		}
		return sShipCargo;
	}
	else
		return "N/A";
}

String AISMsgClass::sGetCargoSecond(const int n,const String sPrefixDelimiter,const String sPostfixDelimiter)
{
	String sSecondDigit;
	switch(n/10) // first digit
	{
		case 2:
		case 4:
      case 6:
      case 7:
      case 8:
      case 9:
         switch (n%10) // Second digit
         {
            case 0: sSecondDigit="";						break;
            case 1: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.A)"+sPostfixDelimiter;	break;
            case 2: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.B)"+sPostfixDelimiter;	break;
            case 3: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.C)"+sPostfixDelimiter;	break;
            case 4: sSecondDigit=sPrefixDelimiter+"Carrying DG, HS or MP (IMO Cat.D)"+sPostfixDelimiter;	break;
            case 5: //reserved for future use
            case 6: //reserved for future use
            case 7: //reserved for future use
            case 8: //reserved for future use
            case 9: //reserved for future use
            default: sSecondDigit="";
         }
         break;
      case 3:
         switch (n%10) // Second digit
         {
            case 0: sSecondDigit=sPrefixDelimiter+"Fishing"+sPostfixDelimiter;						break;
            case 1: sSecondDigit=sPrefixDelimiter+"Towing"+sPostfixDelimiter;						break;
            case 2: sSecondDigit=sPrefixDelimiter+"Towing (Tow Length > 200m / Tow Breadth > 25m)"+sPostfixDelimiter;	break;
            case 3: sSecondDigit=sPrefixDelimiter+"Engaged In Dredging / Underwater Operations"+sPostfixDelimiter;	break;
            case 4: sSecondDigit=sPrefixDelimiter+"Engaged In Diving Operations"+sPostfixDelimiter;			break;
            case 5: sSecondDigit=sPrefixDelimiter+"Engaged In Military Operations"+sPostfixDelimiter;			break;
            case 6: sSecondDigit=sPrefixDelimiter+"Sailing"+sPostfixDelimiter;						break;
            case 7: sSecondDigit=sPrefixDelimiter+"Pleasure Craft"+sPostfixDelimiter;					break;
            case 8: //reserved for future use
            case 9: //reserved for future use
            default: sSecondDigit="";
         }
         break;
      case 5:
         switch (n%10) // Second digit
         {
            case 0: sSecondDigit="Pilot Vessel";					break;
            case 1: sSecondDigit="Search And Rescue Vessel";				break;
            case 2: sSecondDigit="Tug";							break;
            case 3: sSecondDigit="Port Tender";						break;
            case 4: sSecondDigit="Vessel With Anti-Pollution Facilities Or Equipment";	break;
            case 5: sSecondDigit="Law Enforcement Vessels";				break;
            case 6: sSecondDigit="";							break; //for assignments to local vessels
            case 7: sSecondDigit="";							break; //for assignments to local vessels
            case 8: sSecondDigit="Medical Transport";					break;
            case 9: sSecondDigit="Ship According To Resolution No 18 (Mob-83)";		break;
            default: sSecondDigit="";
         }
         break;
      default: sSecondDigit="";
   }
   return sSecondDigit;
}

DWORD AISMsgClass::dwGetBuf(BYTE* pucData,const DWORD dwMaxSize)
{
   DWORD dwOffset=0;
   if (pucData!=NULL)
   {
      AddBufferData((BYTE*)&dwStatus,sizeof(dwStatus),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&MsgType,sizeof(MsgType),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&MsgTime_utc,sizeof(MsgTime_utc),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nMsgID,sizeof(nMsgID),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nMMSI,sizeof(nMMSI),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&NavStatus,sizeof(NavStatus),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&dROT,sizeof(dROT),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&dSOG,sizeof(dSOG),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&bHighPosAcc,sizeof(bHighPosAcc),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&LL.dLat,sizeof(LL.dLat),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&LL.dLong,sizeof(LL.dLong),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&dCOG,sizeof(dCOG),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nHdgTN,sizeof(nHdgTN),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nUTCSec,sizeof(nUTCSec),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nIMO,sizeof(nIMO),pucData,dwOffset,dwMaxSize);
      AddBufferData(acCallSign,sizeof(acCallSign),pucData,dwOffset,dwMaxSize);
      AddBufferData(acShipName,sizeof(acShipName),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nShipAndCargoType,sizeof(nShipAndCargoType),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nGPSRef_ShipDim_A,sizeof(nGPSRef_ShipDim_A),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nGPSRef_ShipDim_B,sizeof(nGPSRef_ShipDim_B),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nGPSRef_ShipDim_C,sizeof(nGPSRef_ShipDim_C),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&nGPSRef_ShipDim_D,sizeof(nGPSRef_ShipDim_D),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&NavDevice,sizeof(NavDevice),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&UTC_ETA,sizeof(UTC_ETA),pucData,dwOffset,dwMaxSize);
      AddBufferData((BYTE*)&dDraught,sizeof(dDraught),pucData,dwOffset,dwMaxSize);
      AddBufferData(acDestination,sizeof(acDestination),pucData,dwOffset,dwMaxSize);
//      SMS;
   }
   return dwOffset;
}

void AISMsgClass::SetBuf(BYTE* pucData,const DWORD dwSize)
{
   DWORD dwOffset=0;
   GetBufferData(&dwStatus,sizeof(dwStatus),pucData,dwOffset,dwSize);
   GetBufferData(&MsgType,sizeof(MsgType),pucData,dwOffset,dwSize);
   GetBufferData(&MsgTime_utc,sizeof(MsgTime_utc),pucData,dwOffset,dwSize);
   GetBufferData(&nMsgID,sizeof(nMsgID),pucData,dwOffset,dwSize);
   GetBufferData(&nMMSI,sizeof(nMMSI),pucData,dwOffset,dwSize);
   GetBufferData(&NavStatus,sizeof(NavStatus),pucData,dwOffset,dwSize);
   GetBufferData(&dROT,sizeof(dROT),pucData,dwOffset,dwSize);
   GetBufferData(&dSOG,sizeof(dSOG),pucData,dwOffset,dwSize);
   GetBufferData(&bHighPosAcc,sizeof(bHighPosAcc),pucData,dwOffset,dwSize);
   GetBufferData(&LL.dLat,sizeof(LL.dLat),pucData,dwOffset,dwSize);
   GetBufferData(&LL.dLong,sizeof(LL.dLong),pucData,dwOffset,dwSize);
   GetBufferData(&dCOG,sizeof(dCOG),pucData,dwOffset,dwSize);
   GetBufferData(&nHdgTN,sizeof(nHdgTN),pucData,dwOffset,dwSize);
   GetBufferData(&nUTCSec,sizeof(nUTCSec),pucData,dwOffset,dwSize);
   GetBufferData(&nIMO,sizeof(nIMO),pucData,dwOffset,dwSize);
   GetBufferData(acCallSign,sizeof(acCallSign),pucData,dwOffset,dwSize);
   GetBufferData(acShipName,sizeof(acShipName),pucData,dwOffset,dwSize);
   GetBufferData(&nShipAndCargoType,sizeof(nShipAndCargoType),pucData,dwOffset,dwSize);
   GetBufferData(&nGPSRef_ShipDim_A,sizeof(nGPSRef_ShipDim_A),pucData,dwOffset,dwSize);
   GetBufferData(&nGPSRef_ShipDim_B,sizeof(nGPSRef_ShipDim_B),pucData,dwOffset,dwSize);
   GetBufferData(&nGPSRef_ShipDim_C,sizeof(nGPSRef_ShipDim_C),pucData,dwOffset,dwSize);
   GetBufferData(&nGPSRef_ShipDim_D,sizeof(nGPSRef_ShipDim_D),pucData,dwOffset,dwSize);
   GetBufferData(&NavDevice,sizeof(NavDevice),pucData,dwOffset,dwSize);
   GetBufferData(&UTC_ETA,sizeof(UTC_ETA),pucData,dwOffset,dwSize);
   GetBufferData(&dDraught,sizeof(dDraught),pucData,dwOffset,dwSize);
   GetBufferData(acDestination,sizeof(acDestination),pucData,dwOffset,dwSize);
}



DWORD AISMsgClass::dwGetExtraInfoBuf(BYTE* pucData,const DWORD dwMaxSize)
{
   DWORD dwOffset=0;
   if (pucData!=NULL)
   {
		/*AISMsgClass ExtraInfoMsg;
		ExtraInfoMsg.Invalidate();
		if (bCallSignValid())
			ExtraInfoMsg.SetCallSign(sGetCallSign());
		if (bShipNameValid())
			ExtraInfoMsg.SetShipName(sGetShipName());
		ExtraInfoMsg.SetMsgType(MsgType);
		if (bMsgIDValid())
			ExtraInfoMsg.SetMsgID(nMsgID);
		if (bMMSIValid())
			ExtraInfoMsg.SetMMSI(nMMSI);
		if (bNavStatusValid())
			ExtraInfoMsg.SetNavStatus(NavStatus);
		if (bROTValid())
			ExtraInfoMsg.SetROT(dROT);
		if (bHighPosAccValid())
			ExtraInfoMsg.SetHighPosAcc(bHighPosAcc);

		if (bIMOValid())
			ExtraInfoMsg.SetIMO(nIMO);
		if (bShipAndCargoTypeValid())
			ExtraInfoMsg.SetCargoAndType(nShipAndCargoType);
		if (bShipDimsValid())
			ExtraInfoMsg.SetGPSRef_ShipDims(nGPSRef_ShipDim_A,nGPSRef_ShipDim_B,nGPSRef_ShipDim_C,nGPSRef_ShipDim_D);
		if (bNavDeviceValid())
			ExtraInfoMsg.SetNavDevice(NavDevice);
		if (bUTC_ETAValid())
			ExtraInfoMsg.SetUTC_ETA(UTC_ETA.wMonth,UTC_ETA.wDay,UTC_ETA.wHour,UTC_ETA.wMinute);
		if (bDraughtValid())
			ExtraInfoMsg.SetDraught(dDraught);
		if (bDestinationValid())
			ExtraInfoMsg.SetDestination(sGetDestination());*/

		AddBufferData((BYTE*)&dwStatus,sizeof(dwStatus),pucData,dwOffset,dwMaxSize);
//		if (bCallSignValid())
			AddBufferData(acCallSign,sizeof(acCallSign),pucData,dwOffset,dwMaxSize);
//		if (bShipNameValid())
			AddBufferData(acShipName,sizeof(acShipName),pucData,dwOffset,dwMaxSize);
//		if (bIMOValid())
			AddBufferData((BYTE*)&nIMO,sizeof(nIMO),pucData,dwOffset,dwMaxSize);

		AddBufferData((BYTE*)&MsgType,sizeof(MsgType),pucData,dwOffset,dwMaxSize); //??

//		if (bMsgIDValid())
			AddBufferData((BYTE*)&nMsgID,sizeof(nMsgID),pucData,dwOffset,dwMaxSize);
//		if (bMMSIValid())
			AddBufferData((BYTE*)&nMMSI,sizeof(nMMSI),pucData,dwOffset,dwMaxSize);
//		if (bNavStatusValid())
			AddBufferData((BYTE*)&NavStatus,sizeof(NavStatus),pucData,dwOffset,dwMaxSize);
//		if (bHighPosAccValid())
			AddBufferData((BYTE*)&bHighPosAcc,sizeof(bHighPosAcc),pucData,dwOffset,dwMaxSize);
//		if (bShipAndCargoTypeValid())
			AddBufferData((BYTE*)&nShipAndCargoType,sizeof(nShipAndCargoType),pucData,dwOffset,dwMaxSize);
//		if (bShipDimsValid())
		{
			AddBufferData((BYTE*)&nGPSRef_ShipDim_A,sizeof(nGPSRef_ShipDim_A),pucData,dwOffset,dwMaxSize);
			AddBufferData((BYTE*)&nGPSRef_ShipDim_B,sizeof(nGPSRef_ShipDim_B),pucData,dwOffset,dwMaxSize);
			AddBufferData((BYTE*)&nGPSRef_ShipDim_C,sizeof(nGPSRef_ShipDim_C),pucData,dwOffset,dwMaxSize);
			AddBufferData((BYTE*)&nGPSRef_ShipDim_D,sizeof(nGPSRef_ShipDim_D),pucData,dwOffset,dwMaxSize);
		}
//		if (bNavDeviceValid())
			AddBufferData((BYTE*)&NavDevice,sizeof(NavDevice),pucData,dwOffset,dwMaxSize);
//		if (bUTC_ETAValid())
			AddBufferData((BYTE*)&UTC_ETA,sizeof(UTC_ETA),pucData,dwOffset,dwMaxSize);
//		if (bDraughtValid())
			AddBufferData((BYTE*)&dDraught,sizeof(dDraught),pucData,dwOffset,dwMaxSize);
//		if (bDestinationValid())
			AddBufferData(acDestination,sizeof(acDestination),pucData,dwOffset,dwMaxSize);
	}
	return dwOffset;
}

void AISMsgClass::SetExtraInfoBuf(BYTE* pucData,const DWORD dwSize)
{
	DWORD dwOffset=0;
	Invalidate();
	GetBufferData(&dwStatus,sizeof(dwStatus),pucData,dwOffset,dwSize);
//	if (bCallSignValid())
		GetBufferData(acCallSign,sizeof(acCallSign),pucData,dwOffset,dwSize);
//	if (bShipNameValid())
		GetBufferData(acShipName,sizeof(acShipName),pucData,dwOffset,dwSize);
//	if (bIMOValid())
		GetBufferData(&nIMO,sizeof(nIMO),pucData,dwOffset,dwSize);

	GetBufferData(&MsgType,sizeof(MsgType),pucData,dwOffset,dwSize);

//	if (bMsgIDValid())
		GetBufferData(&nMsgID,sizeof(nMsgID),pucData,dwOffset,dwSize);
//	if (bMMSIValid())
		GetBufferData(&nMMSI,sizeof(nMMSI),pucData,dwOffset,dwSize);
//	if (bNavStatusValid())
		GetBufferData(&NavStatus,sizeof(NavStatus),pucData,dwOffset,dwSize);
//	if (bHighPosAccValid())
		GetBufferData(&bHighPosAcc,sizeof(bHighPosAcc),pucData,dwOffset,dwSize);
//	if (bShipAndCargoTypeValid())
		GetBufferData(&nShipAndCargoType,sizeof(nShipAndCargoType),pucData,dwOffset,dwSize);
//	if (bShipDimsValid())
	{
		GetBufferData(&nGPSRef_ShipDim_A,sizeof(nGPSRef_ShipDim_A),pucData,dwOffset,dwSize);
		GetBufferData(&nGPSRef_ShipDim_B,sizeof(nGPSRef_ShipDim_B),pucData,dwOffset,dwSize);
		GetBufferData(&nGPSRef_ShipDim_C,sizeof(nGPSRef_ShipDim_C),pucData,dwOffset,dwSize);
		GetBufferData(&nGPSRef_ShipDim_D,sizeof(nGPSRef_ShipDim_D),pucData,dwOffset,dwSize);
	}
//	if (bNavDeviceValid())
		GetBufferData(&NavDevice,sizeof(NavDevice),pucData,dwOffset,dwSize);
//	if (bUTC_ETAValid())
		GetBufferData(&UTC_ETA,sizeof(UTC_ETA),pucData,dwOffset,dwSize);
//	if (bDraughtValid())
		GetBufferData(&dDraught,sizeof(dDraught),pucData,dwOffset,dwSize);
//	if (bDestinationValid())
		GetBufferData(acDestination,sizeof(acDestination),pucData,dwOffset,dwSize);
}


void AISMsgClass::AddBufferData(BYTE* pucParamData,const int nSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize)
{
	if (dwOffset+DWORD(nSize)<dwMaxSize)
	{
		memcpy(&pucBuf[dwOffset],pucParamData,nSize);
		dwOffset+=DWORD(nSize);
	}
}

void AISMsgClass::AddBufferData(char* cParamStr,const int nMaxStrSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize)
{
//	cParamStr[nMaxStrSize-1]=0;	//For size to always be <= nMaxStrSize JGT 01/12/2015
	int nStrLen=-1;
	for (int i=0;i<nMaxStrSize;i++)
	{
		if (cParamStr[i]==0)
		{
			nStrLen=i;
			i=nMaxStrSize; // exit
		}
	}
	if (nStrLen==-1) // no null found
		nStrLen=nMaxStrSize+1;
	else
		nStrLen+=1;
	char* pChar=new char[nStrLen];
	memset(pChar,0,nStrLen);
	memcpy(pChar,cParamStr,nStrLen-1);
	AddBufferData((BYTE*)pChar,nStrLen,pucBuf,dwOffset,dwMaxSize);
	delete[] pChar;
}


void AISMsgClass::GetBufferData(void* pParamData,const int nSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize)
{
	if (dwOffset+DWORD(nSize)<dwMaxSize)
	{
		memcpy(pParamData,&pucBuf[dwOffset],nSize);
		dwOffset+=DWORD(nSize);
	}
}


void AISMsgClass::GetBufferData(char* cParamStr,const int nMaxStrSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD)
{
   int nStrLen=0;
   for (int i=0;i<nMaxStrSize+1;i++)
   {
      if (pucBuf[dwOffset+i]==0)
      {
         nStrLen=i;
         i=nMaxStrSize+1; // exit
      }
   }
   memset(cParamStr,0,nMaxStrSize);
   memcpy(cParamStr,&pucBuf[dwOffset],nStrLen);
   dwOffset+=nStrLen+1; // include null char
}


String AISMsgClass::sGetParamInfoStrings(const String sParameterDelimiter,const String sNewLineDelimiter)
{
   String sParams="";
   String sNotAvail="Not Available";
   String sTmp;
   sParams+="Message Type"+sParameterDelimiter+sGetMsgType()+sNewLineDelimiter;
   sParams+="Message Time (UTC)"+sParameterDelimiter+MsgTime_utc.sDateTime()+sNewLineDelimiter;
   sParams+="Message ID"+sParameterDelimiter+IntToStr(nMsgID)+sNewLineDelimiter;
   sParams+="MMSI"+sParameterDelimiter+IntToStr(nMMSI)+sNewLineDelimiter;
   if (bNavStatusValid())
      sTmp=sGetNavStatus();
   else
      sTmp=sNotAvail;
   sParams+="Navigational Status"+sParameterDelimiter+sTmp+sNewLineDelimiter;

   if (bROTValid())
      sTmp.printf(L"%6.3f deg/s",dROT);
   else
      sTmp=sNotAvail;
   sParams+="Rate of Turn"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bSOGValid())
      sTmp.printf(L"%4.1f kts",dSOG);
   else
      sTmp=sNotAvail;
   sParams+="Speed Over Ground"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bHighPosAccValid())
   {
      if (bHighPosAcc)
         sTmp="< 10m";
      else
         sTmp="> 10m";
   }
   else
      sTmp=sNotAvail;
   sParams+="Position Accuracy"+sParameterDelimiter+sTmp+sNewLineDelimiter;

   sParams+="Latitude"+sParameterDelimiter+LL.sLat()+sNewLineDelimiter;
   sParams+="Longitude"+sParameterDelimiter+LL.sLong()+sNewLineDelimiter;
   if (bCOGValid())
		sTmp.printf(L"%5.1f deg",dCOG);
   else
      sTmp=sNotAvail;
   sParams+="Course Over Ground"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bHdgTNValid())
      sTmp.printf(L"%d deg",nHdgTN);
   else
      sTmp=sNotAvail;
   sParams+="Heading (TN)"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bIMOValid())
		sTmp.printf(L"%d",nIMO);
   else
      sTmp=sNotAvail;
   sParams+="IMO"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bCallSignValid())
      sTmp=sGetCallSign();
   else
      sTmp=sNotAvail;
   sParams+="Callsign"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bShipNameValid())
      sTmp=sGetShipName();
   else
      sTmp=sNotAvail;
   sParams+="Name"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bShipAndCargoTypeValid())
      sTmp=sGetShipAndCargoType("(",")");
   else
      sTmp=sNotAvail;
   sParams+="Ship and Cargo Type"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bShipDimsValid())
		sTmp.printf(L"%u",nGetShipLength());
   else
      sTmp=sNotAvail;
   sParams+="Ship Length"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bShipDimsValid())
		sTmp.printf(L"%u",nGetShipWidth());
   else
      sTmp=sNotAvail;
   sParams+="Ship Width"+sParameterDelimiter+sTmp+sNewLineDelimiter;
	if (bDraughtValid())
		sTmp.printf(L"%4.1f m",dDraught);
	else
		sTmp=sNotAvail;
	sParams+="Draught"+sParameterDelimiter+sTmp+sNewLineDelimiter;
	if (bNavDeviceValid())
		sTmp=sGetNavDevice();
	else
		sTmp=sNotAvail;
	sParams+="Navigational Device"+sParameterDelimiter+sTmp+sNewLineDelimiter;
	if (bUTC_ETAValid())
		sTmp.printf(L"%02u/%02u %02u:%02u [DD/MM HH:mm]",UTC_ETA.wDay,UTC_ETA.wMonth,UTC_ETA.wHour,UTC_ETA.wMinute);
	else
		sTmp=sNotAvail;
	sParams+="Estimated Time Of Arrival"+sParameterDelimiter+sTmp+sNewLineDelimiter;
   if (bDestinationValid())
      sTmp=sGetDestination();
   else
      sTmp=sNotAvail;
   sParams+="Destination"+sParameterDelimiter+sTmp+sNewLineDelimiter;

   return sParams;
}


String AISMsgClass::sGetParamDescription(const int nIdx)
{
   String sDescr="";
   if (!ParamDescriptionList.bPacked())
      ParamDescriptionList.Pack();
   if (nIdx<ParamDescriptionList.nGetCount())
      sDescr=*ParamDescriptionList[nIdx];
   return sDescr;
}

String AISMsgClass::sGetParamValue(const int nIdx)
{
   String sVal="";
   UpdateParamValueList();
   if (!ParamValueList.bPacked())
      ParamValueList.Pack();
   if (nIdx<ParamValueList.nGetCount())
      sVal=*ParamValueList[nIdx];
   return sVal;
}

int AISMsgClass::nGetParamStringsCount()
{
   return ParamDescriptionList.nGetCount();
}

void AISMsgClass::UpdateParamValueList()
{
   ParamValueList.Clear();
   String sNotAvail="Not Available";
   String sTmp;
   if (MsgType==AIS_MSG_VDO)
      AddParamString(&ParamValueList,"Yes");
   else
      AddParamString(&ParamValueList,"No");
   AddParamString(&ParamValueList,IntToStr(nMsgID));
   AddParamString(&ParamValueList,IntToStr(nMMSI));
   if (bNavStatusValid())
      sTmp=sGetNavStatus();
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);

   if (bHighPosAccValid())
   {
      if (bHighPosAcc)
         sTmp="< 10m";
      else
         sTmp="> 10m";
   }
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);

   if (bIMOValid())
		sTmp.printf(L"%d",nIMO);
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
   if (bShipAndCargoTypeValid())
      sTmp=sGetShipAndCargoType(" (",")");
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
   if (bShipDimsValid())
		sTmp.printf(L"%u m",nGetShipLength());
	else
		sTmp=sNotAvail;
	AddParamString(&ParamValueList,sTmp);
	if (bShipDimsValid())
		sTmp.printf(L"%u m",nGetShipWidth());
	else
		sTmp=sNotAvail;
	AddParamString(&ParamValueList,sTmp);
	if (bDraughtValid())
		sTmp.printf(L"%4.1f m",dDraught);
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
	if (bNavDeviceValid())
      sTmp=sGetNavDevice();
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
	if (bUTC_ETAValid())
		sTmp.printf(L"%02u/%02u %02u:%02u [DD/MM HH:mm]",UTC_ETA.wDay,UTC_ETA.wMonth,UTC_ETA.wHour,UTC_ETA.wMinute);
	else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
   if (bDestinationValid())
      sTmp=sGetDestination();
   else
      sTmp=sNotAvail;
   AddParamString(&ParamValueList,sTmp);
}


void AISMsgClass::AddParamString(JList<String> *pParamList,const String sParam)
{
	String* pString=new String(sParam);
   pParamList->nAdd(pString);
}


void AISMsgClass::UpdateParamDescriptionList()
{
   ParamDescriptionList.Clear();
   AddParamString(&ParamDescriptionList,"Own Ship");
   AddParamString(&ParamDescriptionList,"Message ID");
   AddParamString(&ParamDescriptionList,"MMSI");
   AddParamString(&ParamDescriptionList,"Navigational Status");
//   AddParamString(&ParamDescriptionList,"Rate of Turn");
//   AddParamString(&ParamDescriptionList,"Speed Over Ground");
   AddParamString(&ParamDescriptionList,"Position Accuracy");
   AddParamString(&ParamDescriptionList,"IMO");
//   AddParamString(&ParamDescriptionList,"Callsign");
//   AddParamString(&ParamDescriptionList,"Name");
   AddParamString(&ParamDescriptionList,"Ship and Cargo Type");
   AddParamString(&ParamDescriptionList,"Ship Length");
   AddParamString(&ParamDescriptionList,"Ship Width");
   AddParamString(&ParamDescriptionList,"Draught");
   AddParamString(&ParamDescriptionList,"Navigational Device");
   AddParamString(&ParamDescriptionList,"Estimated Time Of Arrival");
   AddParamString(&ParamDescriptionList,"Destination");
}

