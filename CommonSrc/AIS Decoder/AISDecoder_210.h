//---------------------------------------------------------------------------

#ifndef AISDecoder_210H
#define AISDecoder_210H

#include "AISDataDefs_210.h"
#include "JCommaText.h"
#include "JFile.h"
#include "JTime.h"
#include "JList.h"
//#include "LJUtils.h"
//---------------------------------------------------------------------------

//const DWORD dwDaysFrom1970to1990=7304;  // actually 7305 - fudged to account for JTime 1 day
const DWORD dwDaysFrom1970to1990=7305;  	// changed back to 7305 - the previous produced 1 day into the future JGT 14/06/2019
class AISDecode_SM_Class
{
	enum AIS_DECODER_STATE
   {
      AIS_DECODER_STATE_IDLE,
      AIS_DECODER_STATE_MSG,
      AIS_DECODER_STATE_COMPLETE,
   };

   String sVDM;
   AIS_DECODER_STATE MsgState;
   AIS_MSG_TYPE MsgType;
   int nFillBits;

   bool bSetMsgType(const String sRawNMEA);
	JTime T_utc; // Timestamp received from Satellite (Sat-AIS data only) OR Local Time
	JTime T_Prev_utc; // Previous timestamp received - FOR STATIC SATELLITE AIS DATA
	bool bSetTime_utc(const String sNMEA); // returns true even if not sat msg - only false when is sat msg but cannot decode timestamp (static msgs)
	bool bPrevSentenceLine1StaticFromSat;
	bool bSatAIS;
public:
	JTime GetTime_utc() const
	{
		return T_utc;
	}

	bool bAddString(const String sRawNMEA);

   String sGetVDM() const
   {
      return sVDM;
   }

   int nGetFillBits() const
   {
      return nFillBits;
   }

   AIS_MSG_TYPE GetMsgType() const
   {
      return MsgType;
   }

	void Reset(AIS_MSG_TYPE _MsgType=AIS_MSG_INVALID)
   {
		sVDM="";
      MsgType=_MsgType;
      nFillBits=0;
      MsgState=AIS_DECODER_STATE_IDLE;
	};

	AISDecode_SM_Class& operator = (const AISDecode_SM_Class& _SM)
	{
		sVDM=_SM.sVDM;
		MsgType=_SM.MsgType;
		nFillBits=_SM.nFillBits;
		MsgState=_SM.MsgState;
		return *this;
	};

	AISDecode_SM_Class(const AISDecode_SM_Class& _SM)  // Copy constructor
	{
		*this=_SM;
	};

	AISDecode_SM_Class(const bool _bSatAIS)
	{
		Reset();
		T_Prev_utc.Invalidate();
		bSatAIS=_bSatAIS;
		bPrevSentenceLine1StaticFromSat=false;
	};

	~AISDecode_SM_Class()
   {
   };

};


class AISDecoderClass
{
	AISDecode_SM_Class* pAIS_SM;
   BYTE aucVDMData[256];
	AISMsgClass AISMsg;
	bool bSatAIS;

	JList<AISMsgClass> ExtInfoMsgLst;
	void AddExtInfo();
	DWORD dwLastExtInfoMsgsPurgeCheck;
	void PurgeExtInfoMsgs();

	int nTotalVDMDataBits;

	bool bDecodeVDM(const String sVDM,const AIS_MSG_TYPE MsgType,const JTime T_utc);
	bool bDecodeMessage123();
	bool bDecodeMessage5();    // Static data
	bool bDecodeMessage6();    // Addressed SMS
	bool bDecodeMessage8();    // Broadcast SMS
	bool bDecodeMessage18();
	bool bDecodeMessage19();
	bool bDecodeMessage24();

	bool bDecodeToBinary();
   String sGetASCIIfromBinary(const int nStartBit);


   LatLong GetLong(const int n);
   LatLong GetLat(const int n);
   double dGetSOG(const int n);
   double dGetCOG(const int n);
   JTime GetUTCTime();
   String sGetCallSign();
   String sGetShipName();

   JFile* pDebugFil;

// *** Declared private to prevent class copy
	AISDecoderClass& operator = (const AISDecoderClass& M)
	{
		return *this;
	};
	AISDecoderClass(const AISDecoderClass& M)  // Copy constructor
	{
		*this=M;
	};
// *** Declared private to prevent class copy


public:
	void Invalidate();

	AISDecoderClass(bool _bSatAIS)
	{
		bSatAIS=_bSatAIS;
		pAIS_SM=new AISDecode_SM_Class(bSatAIS);
		Invalidate();
      bool bDebug=false;
		pDebugFil=NULL;
		dwLastExtInfoMsgsPurgeCheck=::GetTickCount();
		ExtInfoMsgLst.Clear();
		if (bDebug)
		{
			pDebugFil=new JFile('O',JFile::ASCII_TYPE);
			pDebugFil->Create(L"Debug.txt");
		}

	};

	~AISDecoderClass()
	{
		Invalidate();
		delete pAIS_SM;

/*
#ifdef _DEBUG
		nShowInfoMsg("ExtInfoMsgs Count="+IntToStr(ExtInfoMsgLst.nGetCount()));
#endif
*/

		ExtInfoMsgLst.Clear();
		if (pDebugFil)
      {
         pDebugFil->Close();
         delete pDebugFil;
         pDebugFil=NULL;
      }
   }

	bool bAddSentence(const String sRawNMEA);


	void GetAISMsg(AISMsgClass* pAISMsg)
	{
		*pAISMsg=AISMsg;
		AISMsg.SetContainsUpdatedExtInfo(false);
	}

	void GetConsolidatedAISMsgForCst3(AISMsgClass* pAISMsg)
	{
		AddExtInfo();
		*pAISMsg=AISMsg;
	}

	//void ConsolidateStaticData(AISMsgClass* pAISMsg);
};



#endif
