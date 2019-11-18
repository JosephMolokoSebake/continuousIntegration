/* 	----------------- CONFIGURATION HEADER START --------------------------
 *
 *		MODULE TITLE		AIS Decoder
 *    -----------------------------------------------------------------------
 *		FILE NAME			AISDecoder_210.cpp
 *		COMPILER				Borland C++ Builder 15 (XE)
 *		PROCESSOR			Pentium IV
 *		OS						MS Windows 7
 *		PROGRAMMER			Jacques van Wyk
 *    -----------------------------------------------------------------------
 *		ABSTRACT:			AIS Decoder
 *
 *       V1_00          LJVW 2003/04/18
 *                      - Initial Version
 *		   V1_01          LJVW 2003/05/19
 *					         - Changed LatLong Routine
 *		   V1_02          LJVW 2006/08/31
 *					         - Added cargo type and ship type decoding (Message 5)
 *		   V1_03          LJVW 2007/11/13
 *					         - Added String error checking to decoder
 *		   V2_00          LJVW 2009/01/16
 *                      - Module rewritten
 *                      - Added decoding of Message 18,19,24 for Class B
 *                        Equipment
 *		   V2_10          LJVW 2011/12/07
 *                      - Updated to reflect changes in AISDataDefs_210
 *		   V2_10_01       LJVW 2012/01/26
 *                      - Added decoding of satellite timestamp received from MDS/ExactEarth
 *		   V2_10_02       LJVW 2012/06/01
 *                      - Fixed bug in Sat-decoding function
 *		   V2_10_03       LJVW 2012/09/03
 *                      - Changed Decoder to handle S-AIS specific data
 *		   V2_10_04
 *		   V2_10_05       LJVW 2013/11/08
 *                      - Changed Decoder to handle S-AIS specific data
 *
 /* 	----------------- CONFIGURATION HEADER END -------------------------- */


#include "jpch.h"
#pragma hdrstop

#include "AISDecoder_210.h"
#include <Math.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)


bool AISDecode_SM_Class::bSetMsgType(const String sRawNMEA)
{
   if (sRawNMEA.Pos("VDM")>0)
      MsgType=AIS_MSG_VDM;
	else if (sRawNMEA.Pos("VDO")>0)
		MsgType=AIS_MSG_VDO;
	else
	   MsgType=AIS_MSG_INVALID;
	return (MsgType==AIS_MSG_VDM||MsgType==AIS_MSG_VDO);
}

bool AISDecode_SM_Class::bSetTime_utc(const String sNMEA) // decode satellite timestamp if received, otherwise use PC time
{
	T_utc.Invalidate();
	if (bSatAIS)
	{
		if (bPrevSentenceLine1StaticFromSat) // time for this message already received
		{
			T_utc=T_Prev_utc;
			bPrevSentenceLine1StaticFromSat=false;
		}
		else
		{
			int nSatStart=sNMEA.Pos("\\c:");
			if (nSatStart==0)
				nSatStart=sNMEA.Pos(",c:");
			int nSatEnd=sNMEA.Pos("*");
			if (nSatStart>0 && nSatEnd>0 && (nSatEnd-nSatStart>0) && (nSatEnd-nSatStart<20))
			{
				try
				{
					String sSatTimeSec=sNMEA.SubString(nSatStart+3,nSatEnd-nSatStart-3);
					DWORD dwSatTimeSec=sSatTimeSec.ToInt();
					int nSecSince1990=int(dwSatTimeSec)-(int(dwDaysFrom1970to1990)*86400);
					if (nSecSince1990 > 0)
					{
						T_utc.SetUTC();
						T_utc.SetTotalSec(nSecSince1990,true);
						T_utc.SetMicroSec(0);
						T_Prev_utc=T_utc;
					}
				}
				catch(...)
				{
					T_utc.Invalidate();
				}
			}
			else if (sNMEA.Pos("\\g:")>0 || sNMEA.Pos("!BSVDM,2,1")>0 || sNMEA.Pos("!AIVDM,2,1")>0) // Static data received from satellite - use previous dynamic sat-ais msg time
			{
				if (T_Prev_utc.bAllValid())
				{
					T_utc=T_Prev_utc;
					bPrevSentenceLine1StaticFromSat=true;
				}
			}
			else
					T_utc.Invalidate(); // do not attempt to decode message as timestamp is not valid
		}
	}
	else // for normal VDM/VDO AIS Message Strings
		T_utc=JTime(true,true);
	return T_utc.bAllValid();
}


bool AISDecode_SM_Class::bAddString(const String sRawNMEA)
{
	try
	{
		if (bSetMsgType(sRawNMEA) && bSetTime_utc(sRawNMEA))
		{
			int nNMEAStart;
			if (MsgType==AIS_MSG_VDM)
				nNMEAStart=sRawNMEA.Pos("VDM");
			else
				nNMEAStart=sRawNMEA.Pos("VDO");
			int nNMEAEnd=sRawNMEA.Length()-nNMEAStart+1;
			String sNMEA=sRawNMEA.SubString(nNMEAStart,nNMEAEnd);
			if (sNMEA.Pos("*")>0) // remove checksum
				sNMEA=sNMEA.Delete(sNMEA.Pos("*"),3);
			CommaText CT(sNMEA,true);
			if (CT.nGetNum()>=7) // always 7
			{
				int nCurrentLine=CT.nGetVal(2);
				int nTotalLines=CT.nGetVal(1);
				switch(MsgState)
				{
					case AIS_DECODER_STATE_IDLE:
					{
						if (nCurrentLine==1) // first line of new message
						{
							Reset(GetMsgType());
							MsgState=AIS_DECODER_STATE_MSG;
							sVDM=CT.sGetPar(5);
							nFillBits=CT.nGetVal(6);
							if (nCurrentLine==nTotalLines) // only 1 line in message
								MsgState=AIS_DECODER_STATE_COMPLETE;
						}
						break;
					}
					case AIS_DECODER_STATE_MSG:
					{
						if (nTotalLines-nCurrentLine>=0)
						{
							sVDM+=CT.sGetPar(5);
							nFillBits=CT.nGetVal(6);
						}
						if (nCurrentLine==nTotalLines) // last line in message
							MsgState=AIS_DECODER_STATE_COMPLETE;
						break;
					}
					default:
					{
						Reset();
					}
				}
			}
		}
		else
			Reset();
	}
	catch(...)
	{
		Reset();
	}
	return (MsgState==AIS_DECODER_STATE_COMPLETE);
}


bool AISDecoderClass::bAddSentence(const String sRawNMEA)
{
	bool bValid=false;
	if (pAIS_SM->bAddString(sRawNMEA))
	{
		if (bDecodeVDM(pAIS_SM->sGetVDM(),pAIS_SM->GetMsgType(),pAIS_SM->GetTime_utc()))
		{
			bValid=true;
			AddExtInfo(); // including pseudo static info, i.e. Nav Status, etc.
		}
		else
			Invalidate();
		pAIS_SM->Reset();
	}
	return bValid;
}


void AISDecoderClass::Invalidate()
{
	AISMsg.Invalidate();
	pAIS_SM->Reset();
	memset(&aucVDMData[0],0,sizeof(aucVDMData));
}


bool AISDecoderClass::bDecodeVDM(const String sVDM,const AIS_MSG_TYPE MsgType,const JTime T_utc)
{
   bool bValid=true;
   int nCurrentBit=0;
   Invalidate();
	for (int i=0;i<sVDM.Length()&&bValid;i++)
   {
      BYTE uc;
      bValid=bConvertASCIICharTo6BitBinary(sVDM[i+1],uc);
      if (bValid)
         SetVDMDataBits(aucVDMData,nCurrentBit,6,uc);
   }
   if (bValid)
   {
      nTotalVDMDataBits=nCurrentBit;
      AISMsg.Invalidate();
      AISMsg.SetMsgType(MsgType);
      AISMsg.SetMsgID(dwGetVDMDataBits(aucVDMData,0,6));
      if (pDebugFil)
         pDebugFil->TextLine(AISMsg.nGetMsgID());
      switch (AISMsg.nGetMsgID())
      {
         case 1:
         case 2:
			case 3: bValid=bDecodeMessage123(); break;
         case 5: bValid=bDecodeMessage5();   break;
         case 6: bValid=bDecodeMessage6();   break;
         case 8: bValid=bDecodeMessage8();   break;
         case 18: bValid=bDecodeMessage18();   break;
         case 19: bValid=bDecodeMessage19();   break;
         case 24: bValid=bDecodeMessage24();   break;
         default: bValid=false;
		}
		AISMsg.SetMsgTime_utc(T_utc);
   }
   return bValid;
}


bool AISDecoderClass::bDecodeMessage123()
{
	AISMsg.SetMsgTime_utc(JTime(true,true));
	AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
	AISMsg.SetNavStatus(dwGetVDMDataBits(aucVDMData,38,4));
	AISMsg.SetROT(dwGetVDMDataBits(aucVDMData,42,8));
	AISMsg.SetSOG(dwGetVDMDataBits(aucVDMData,50,10));
	AISMsg.SetHighPosAcc(dwGetVDMDataBits(aucVDMData,60,1));

	// LJVW NOTE 2013/11/08: AIS-SART may transmit even when LL invalid - THIS CASE IS CURRENTLY NOT DECODED!!
//	AISMsg.SetSuppressSartAlarm(bSuppressSartAlarm); // for cases where AIS-SART is used as regular tracking device, e.g. TechAF vmsTrack

	AISMsg.SetLL(dwGetVDMDataBits(aucVDMData,89,27),dwGetVDMDataBits(aucVDMData,61,28));
   AISMsg.SetCOG(dwGetVDMDataBits(aucVDMData,116,12));
   AISMsg.SetHdgTN(dwGetVDMDataBits(aucVDMData,128,9));
   AISMsg.SetUTCSec(dwGetVDMDataBits(aucVDMData,137,6));
   return (AISMsg.bMMSIValid()&&AISMsg.bLLValid());
}


bool AISDecoderClass::bDecodeMessage5()
{
   AISMsg.SetMsgTime_utc(JTime(true,true));
   AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
   AISMsg.SetIMO(dwGetVDMDataBits(aucVDMData,40,30));
   AISMsg.ClearCallSign();
   for (int i=0;i<7;i++)
      AISMsg.SetCallSign(cGetASCII(dwGetVDMDataBits(aucVDMData,70+(i*6),6)),i);
   AISMsg.ClearShipName();
   for (int i=0;i<20;i++)
      AISMsg.SetShipName(cGetASCII(dwGetVDMDataBits(aucVDMData,112+(i*6),6)),i);
   AISMsg.SetCargoAndType(dwGetVDMDataBits(aucVDMData,232,8));
   AISMsg.SetGPSRef_ShipDims(dwGetVDMDataBits(aucVDMData,240,9),dwGetVDMDataBits(aucVDMData,249,9),dwGetVDMDataBits(aucVDMData,258,6),dwGetVDMDataBits(aucVDMData,264,6));
   AISMsg.SetNavDevice(dwGetVDMDataBits(aucVDMData,270,4));
   AISMsg.SetUTC_ETA(dwGetVDMDataBits(aucVDMData,274,4),dwGetVDMDataBits(aucVDMData,278,5),dwGetVDMDataBits(aucVDMData,283,5),dwGetVDMDataBits(aucVDMData,288,6));
   AISMsg.SetDraught(dwGetVDMDataBits(aucVDMData,294,8));
   for (int i=0;i<20;i++)
      AISMsg.SetDestination(cGetASCII(dwGetVDMDataBits(aucVDMData,302+(i*6),6)),i);
   return (AISMsg.bMMSIValid());
}


bool AISDecoderClass::bDecodeMessage18()
{
   AISMsg.SetMsgTime_utc(JTime(true,true));
   AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
   AISMsg.SetSOG(dwGetVDMDataBits(aucVDMData,46,10));
   AISMsg.SetHighPosAcc(dwGetVDMDataBits(aucVDMData,56,1));
   AISMsg.SetLL(dwGetVDMDataBits(aucVDMData,85,27),dwGetVDMDataBits(aucVDMData,57,28));
   AISMsg.SetCOG(dwGetVDMDataBits(aucVDMData,112,12));
   AISMsg.SetHdgTN(dwGetVDMDataBits(aucVDMData,124,9));
   AISMsg.SetUTCSec(dwGetVDMDataBits(aucVDMData,133,6));
	return (AISMsg.bMMSIValid()&&AISMsg.bLLValid());
}


bool AISDecoderClass::bDecodeMessage19()
{
   if (bDecodeMessage18()) // first part identical to Message 18
   {
      AISMsg.ClearShipName();
      for (int i=0;i<20;i++)
         AISMsg.SetShipName(cGetASCII(dwGetVDMDataBits(aucVDMData,143+(i*6),6)),i);
      AISMsg.SetCargoAndType(dwGetVDMDataBits(aucVDMData,263,8));
      AISMsg.SetGPSRef_ShipDims(dwGetVDMDataBits(aucVDMData,271,9),dwGetVDMDataBits(aucVDMData,280,9),dwGetVDMDataBits(aucVDMData,289,6),dwGetVDMDataBits(aucVDMData,295,6));
      AISMsg.SetNavDevice(dwGetVDMDataBits(aucVDMData,301,4));
   }
   return (AISMsg.bMMSIValid()&&AISMsg.bLLValid());
}

bool AISDecoderClass::bDecodeMessage24()
{
   AISMsg.SetMsgTime_utc(JTime(true,true));
   AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
   if (dwGetVDMDataBits(aucVDMData,38,2)==0) // Message 24 Part A
   {
      AISMsg.ClearShipName();
      for (int i=0;i<20;i++)
         AISMsg.SetShipName(cGetASCII(dwGetVDMDataBits(aucVDMData,40+(i*6),6)),i);
   }
   else // Message 24 Part B
   {
      AISMsg.SetCargoAndType(dwGetVDMDataBits(aucVDMData,40,8));
      AISMsg.ClearCallSign();
      for (int i=0;i<7;i++)
         AISMsg.SetCallSign(cGetASCII(dwGetVDMDataBits(aucVDMData,90+(i*6),6)),i);
      AISMsg.SetGPSRef_ShipDims(dwGetVDMDataBits(aucVDMData,132,9),dwGetVDMDataBits(aucVDMData,141,9),dwGetVDMDataBits(aucVDMData,150,6),dwGetVDMDataBits(aucVDMData,156,6));
   }
   String s=AISMsg.sGetShipAndCargoType();
   return AISMsg.bMMSIValid();
}

bool AISDecoderClass::bDecodeMessage6() // NOT TESTED (ITU p.58)
{
   AISMsg.SetMsgTime_utc(JTime(true,true));
	//AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
   SMSStruct SMS;
   SMS.bBroadcastMessage=false;
   SMS.nSrcMMSI=dwGetVDMDataBits(aucVDMData,8,30);
   SMS.nMessageSequenceNr=dwGetVDMDataBits(aucVDMData,38,2);
   SMS.nDestMMSI=dwGetVDMDataBits(aucVDMData,40,30);
   int nCurrentBit=88;
   SMS.sTextMessage="";
   while (nCurrentBit<=nTotalVDMDataBits-6)
   {
      SMS.sTextMessage+=cGetASCII(dwGetVDMDataBits(aucVDMData,nCurrentBit,6));
      nCurrentBit+=6;
   }
   if (!SMS.sTextMessage.IsEmpty())
      AISMsg.SetSMS(SMS);
   return (AISMsg.bSMSValid());
}


bool AISDecoderClass::bDecodeMessage8() // NOT TESTED (ITU p.60)
{
   AISMsg.SetMsgTime_utc(JTime(true,true));
   //AISMsg.SetMMSI(dwGetVDMDataBits(aucVDMData,8,30));
   SMSStruct SMS;
   SMS.bBroadcastMessage=true;
   SMS.nSrcMMSI=dwGetVDMDataBits(aucVDMData,8,30);
   int nCurrentBit=56;
   SMS.sTextMessage="";
   while (nCurrentBit<=nTotalVDMDataBits-6)
   {
      SMS.sTextMessage+=cGetASCII(dwGetVDMDataBits(aucVDMData,nCurrentBit,6));
      nCurrentBit+=6;
   }
   if (!SMS.sTextMessage.IsEmpty())
      AISMsg.SetSMS(SMS);
   return (AISMsg.bSMSValid());
}

void AISDecoderClass::AddExtInfo()
{
	bool bFound=false;
	DWORD dwNow=::GetTickCount();
	if (dwNow - dwLastExtInfoMsgsPurgeCheck  > 1000*60*60) // Purge old messages every 1 hour
	{
		PurgeExtInfoMsgs();
		dwLastExtInfoMsgsPurgeCheck=dwNow;
	}
	ExtInfoMsgLst.GoFirst();
	while (!ExtInfoMsgLst.bLast() && !bFound)
	{
		AISMsgClass* pS=ExtInfoMsgLst.pNext();
		if (pS)
		{
			if (AISMsg.nGetMMSI() == pS->nGetMMSI())
			{
				bFound=true;
				pS->UpdateExtInfo(&AISMsg);
				AISMsg.UpdateExtInfo(pS);
			}
		}
	}
	if (!bFound)
	{
		AISMsgClass* pM=new AISMsgClass;
		*pM=AISMsg;
		ExtInfoMsgLst.nAdd(pM);
	}
}

void AISDecoderClass::PurgeExtInfoMsgs()
{
	ExtInfoMsgLst.GoFirst();
	DWORD dwNow_utc=JTime(true,true).dwTotalSec();
	while (!ExtInfoMsgLst.bLast())
	{
		AISMsgClass* pS=ExtInfoMsgLst.pNext();
		if (pS)
		{
			if (dwNow_utc - pS->GetMsgTime_utc().dwTotalSec() > 24*60*60) // delete all messages older than 1 day
			{
				ExtInfoMsgLst.pPrev();
				ExtInfoMsgLst.Delete();
			}
		}
	}
}









