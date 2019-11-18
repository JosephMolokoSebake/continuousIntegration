//---------------------------------------------------------------------------

#ifndef AISDataDefs_210H
#define AISDataDefs_210H

#include "JTransform.h"
#include "JTime.h"
#include "JList.h"
//---------------------------------------------------------------------------

void SetVDMDataBits(BYTE* pucData,int& nOffs,const int nSize,const DWORD dwVal)
{
  DWORD dwByte=nOffs/8;
  DWORD dwBit=nOffs%8;
  for (int i=0; i<nSize; i++)
  {
    if (dwVal&(1<<(nSize-i-1)))
      pucData[dwByte]|=(1<<dwBit);
    ++dwBit;
    if (dwBit==8)
    {
      ++dwByte;
      dwBit=0;
    }
  }
  nOffs+=nSize;
}

DWORD dwGetVDMDataBits(const BYTE* pucData, const int nOffs, const int nSize)
{
   DWORD dwByte=nOffs/8;
   DWORD dwBit=nOffs%8;
   DWORD dwData=0;
   for (int i=0; i<nSize; i++)
   {
     if (pucData[dwByte]&(1<<dwBit))
       dwData|=(1<<(nSize-1-i));
     ++dwBit;
     if (dwBit==8)
     {
       ++dwByte;
       dwBit=0;
     }
   }
   return dwData;
};

bool bConvertASCIICharTo6BitBinary(const BYTE ucCode,BYTE& ucBin)
{
  bool bValid=(ucCode>=0x30)&&(ucCode<=0x77)&&(ucCode!=0x58)&&(ucCode!=0x59);
  if (bValid)
  {
    ucBin=ucCode+0x28;
    if (ucBin<=0x80)
      ucBin+=0x28;
    else
      ucBin+=0x20;
    ucBin&=0x3F;
  }
  return bValid;
}



char cGetASCII(const BYTE uc)
{
  if (uc<=0x1F)
    return uc+0x40;
  else
    return uc;
};

BYTE ucGetByteFromASCII(const char c)
{
  if (c<=0x3F)
    return c;
  else
    return c-0x40;
};

struct SMSStruct
{
   int nSrcMMSI;
   int nDestMMSI;
   bool bBroadcastMessage;
   int nMessageSequenceNr;
   String sTextMessage;

   void Invalidate()
   {
      bBroadcastMessage=false;
      nMessageSequenceNr=-1;
      sTextMessage=L"";
      nSrcMMSI=-1000000;
      nDestMMSI=-1000000;
   };

   SMSStruct& operator = (const SMSStruct& SMS)
   {
		bBroadcastMessage=SMS.bBroadcastMessage;
		nMessageSequenceNr=SMS.nMessageSequenceNr;
      sTextMessage=SMS.sTextMessage;
      nSrcMMSI=SMS.nSrcMMSI;
      nDestMMSI=SMS.nDestMMSI;
      return *this;
   };
   SMSStruct(const SMSStruct& SMS)  // Copy constructor
   {
      *this=SMS;
   };

   SMSStruct()
   {
      Invalidate();
   };
	~SMSStruct()
   {
   };
};

#pragma option push -bi-    //Do not treat enums as ints

enum AIS_MSG_TYPE
{
	AIS_MSG_INVALID 	= 0,
	AIS_MSG_VDO 		= 1,		// Own vessel data
	AIS_MSG_VDM 		= 2,		// Vessel data
};

enum AIS_NAVSTATUS_TYPE
{
	AIS_NAVSTATUS_UNDER_WAY_USING_ENGINE		=0,
	AIS_NAVSTATUS_AT_ANCHOR							=1,
	AIS_NAVSTATUS_NOT_UNDER_COMMAND				=2,
	AIS_NAVSTATUS_RESTRICTED_MANOEUVRABILITY	=3,
	AIS_NAVSTATUS_CONSTRAINED_BY_HER_DRAUGHT	=4,
	AIS_NAVSTATUS_MOORED								=5,
	AIS_NAVSTATUS_AGROUND							=6,
	AIS_NAVSTATUS_ENGAGED_IN_FISHING				=7,
	AIS_NAVSTATUS_UNDER_WAY_SAILING				=8,
	AIS_NAVSTATUS_RESERVED_1						=9,
	AIS_NAVSTATUS_RESERVED_2						=10,
	AIS_NAVSTATUS_RESERVED_3						=11,
	AIS_NAVSTATUS_RESERVED_4						=12,
	AIS_NAVSTATUS_RESERVED_5						=13,
	AIS_NAVSTATUS_AIS_SART							=14,
	AIS_NAVSTATUS_NOT_DEFINED						=15,
};

enum AIS_NAVDEVICE_TYPE
{
   AIS_NAVDEVICE_UNDEFINED			=0,
	AIS_NAVDEVICE_GPS					=1,
   AIS_NAVDEVICE_GLONASS			=2,
	AIS_NAVDEVICE_GPS_GLONASS		=3,
   AIS_NAVDEVICE_LORAN_C			=4,
   AIS_NAVDEVICE_CHAYKA				=5,
	AIS_NAVDEVICE_INS					=6,
   AIS_NAVDEVICE_SURVEYED			=7,
   AIS_NAVDEVICE_GALILEO			=8,
};

enum AIS_MSG_STATUS_TYPE
{
   AIS_STAT_MSGID_VALID			=0x00000001,
   AIS_STAT_MMSI_VALID			=0x00000002,
   AIS_STAT_NAVSTAT_VALID		=0x00000004,
   AIS_STAT_ROT_VALID			=0x00000008,
   AIS_STAT_SOG_VALID			=0x00000010,
   AIS_STAT_POSACC_VALID		=0x00000020,
	AIS_STAT_LL_VALID				=0x00000040,
   AIS_STAT_COG_VALID			=0x00000080,
   AIS_STAT_HDG_VALID			=0x00000100,
   AIS_STAT_MSGTIME_VALID		=0x00000200,
   AIS_STAT_IMO_VALID			=0x00000400,
   AIS_STAT_CALLSIGN_VALID		=0x00000800,
   AIS_STAT_SHIPNAME_VALID		=0x00001000,
   AIS_STAT_CARGO_VALID			=0x00002000,
   AIS_STAT_SHIPDIMS_VALID		=0x00004000,
   AIS_STAT_NAVDEV_VALID		=0x00008000,
   AIS_STAT_ETA_VALID			=0x00010000,
	AIS_STAT_DRAUGHT_VALID		=0x00020000,
	AIS_STAT_DESTINATION_VALID	=0x00040000,
   AIS_STAT_SMS_VALID			=0x00080000,
   AIS_STAT_UTCSEC_VALID		=0x00100000,
	AIS_STAT_GPSREFPOS_VALID  	=0x00200000,
	AIS_STAT_TXSTATUS_VALID  	=0x00400000, // RESERVED FOR VISTA
	AIS_STAT_EXTINFOMODIFIED   =0x00800000, // RESERVED for VISTA: indicated if "static" message data has been modified (for COASTRAD 4U network re-transmission)
	AIS_STAT_HIGHPOSACC		   =0x01000000, // RESERVED for VISTA: Indicates High Position Accuracy (true if accuracy <10m)
//	AIS_STAT_NO_SART_ALARM 	   =0x02000000, // Suppress AIS-SART Alarm Message (used for cases where AIS-SART used as normal tracking device, i.e. "ALT-AIS")

//	AIS_STAT_ZDA_VALID		   =0x04000000,
//	AIS_STAT_GGA_VALID		   =0x08000000,

};

#pragma option pop    		//Set compiler back to default enum type



class AISMsgClass
{
	DWORD dwStatus;
	JTime MsgTime_utc;
	AIS_MSG_TYPE MsgType;
	bool bExtInfoModified;			// indicated if "static" message data has been modified (for COASTRAD network re-transmission)
	int nMsgID;
	int nMMSI;							//	(Messages 1,2,3,5,18)
	AIS_NAVSTATUS_TYPE NavStatus; // Navigational Status
	double dROT;						// Rate of Turn [deg/min]
	double dSOG;						// Speed over ground in knots						(Messages 1,2,3,18)
	bool bHighPosAcc;					// Indicates High Position Accuracy (true if accuracy <10m)
	LatLong LL;							// Latitude & Longitude in minutes				(Messages 1,2,3,18)
	double dCOG;						// Course of ground in degrees					(Messages 1,2,3,18)
	int nHdgTN;							// Heading relative to True North [deg]
	int nUTCSec;						//	Messages 1,2,3,4,18

	int nIMO;							//	(Message 5)
	char acCallSign[7];				//	(Message 5)
	char acShipName[20];				//	(Message 5)
	int nShipAndCargoType;			//	(Message 5)
	int nGPSRef_ShipDim_A;			// Ship's Bow to GPS [m]							(Message 5)
	int nGPSRef_ShipDim_B;			// Ship's Stern to GPS [m]							(Message 5)
	int nGPSRef_ShipDim_C;			// Ship's Port to GPS [m]							(Message 5)
	int nGPSRef_ShipDim_D;			// Ship's Starboard to GPS [m]					(Message 5)
	AIS_NAVDEVICE_TYPE NavDevice;	// Type of Electronic Position Fixing Device (Message 5)
	SYSTEMTIME UTC_ETA;				// Expected Time of Arrival						(Message 5)
	double dDraught;					// (Message 5)
	char acDestination[20];			//	(Message 5)

   SMSStruct SMS;		// SMS Message				(Messages 6,8)

//	JTime ZDA_T_utc;
//	LatLong GGA_LL;

	static const String Months[12];

   String sGetCargoSecond(const int n,const String sPrefixDelimiter="\r\n",const String sPostfixDelimiter="");

   void AddBufferData(BYTE* pucParamData,const int nSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize);
   void AddBufferData(char* cParamStr,const int nMaxStrSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize);
   void GetBufferData(void* pParamData,const int nSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize);
   void GetBufferData(char* cParamStr,const int nMaxStrSize,BYTE* pucBuf,DWORD& dwOffset,const DWORD dwMaxSize);

   JList<String>ParamDescriptionList;
   JList<String>ParamValueList;
   void UpdateParamDescriptionList();
   void AddParamString(JList<String>*pParamList,const String sParam);
   void UpdateParamValueList();

public:
   void Invalidate()
   {
      dwStatus=0;
      MsgType=AIS_MSG_INVALID;
      MsgTime_utc.Invalidate();
		bExtInfoModified=false;
		nMsgID=0;
      nMMSI=-1;
      NavStatus=AIS_NAVSTATUS_NOT_DEFINED;
      dROT=-1000000.0;
      dSOG=-1000000.0;
      LL.Invalidate();
      dCOG=-1000000.0;
      nHdgTN=-1000000;
      nUTCSec=-1000000;
      nIMO=-1;
      ClearCallSign();
      ClearShipName();
      nShipAndCargoType=0;
      nGPSRef_ShipDim_A=0;
      nGPSRef_ShipDim_B=0;
      nGPSRef_ShipDim_C=0;
      nGPSRef_ShipDim_D=0;
      NavDevice=AIS_NAVDEVICE_UNDEFINED;
      JTime T;
      T.Invalidate();
      UTC_ETA=T.Get();
      dDraught=-1000000.0;
      ClearDestination();
      SMS.Invalidate();
      UpdateParamDescriptionList();
      ParamValueList.Clear();
   };


   AISMsgClass& operator = (const AISMsgClass& _M)
   {
      Assign(_M);
      return *this;
   };

	void Assign(const AISMsgClass& _M)
   {
		dwStatus=_M.dwStatus;
		MsgType=_M.MsgType;
      MsgTime_utc=_M.MsgTime_utc;
		bExtInfoModified=_M.bExtInfoModified;
		nMsgID=_M.nMsgID;
		nMMSI=_M.nMMSI;
      NavStatus=_M.NavStatus;
      dROT=_M.dROT;
      dSOG=_M.dSOG;
      bHighPosAcc=_M.bHighPosAcc;
      LL=_M.LL;
      dCOG=_M.dCOG;
      nHdgTN=_M.nHdgTN;
      nUTCSec=_M.nUTCSec;
      nIMO=_M.nIMO;
      memcpy(&acCallSign[0],&_M.acCallSign[0],sizeof(acCallSign));
      memcpy(&acShipName[0],&_M.acShipName[0],sizeof(acShipName));
      nShipAndCargoType=_M.nShipAndCargoType;
      nGPSRef_ShipDim_A=_M.nGPSRef_ShipDim_A;
      nGPSRef_ShipDim_B=_M.nGPSRef_ShipDim_B;
      nGPSRef_ShipDim_C=_M.nGPSRef_ShipDim_C;
      nGPSRef_ShipDim_D=_M.nGPSRef_ShipDim_D;
      NavDevice=_M.NavDevice;
      UTC_ETA=_M.UTC_ETA;
      dDraught=_M.dDraught;
      memcpy(&acDestination[0],&_M.acDestination[0],sizeof(acDestination));
      SMS=_M.SMS;
		ParamDescriptionList=_M.ParamDescriptionList;
      ParamValueList=_M.ParamValueList;
   }

   AISMsgClass(AISMsgClass& AIS)  // Copy constructor
   {
      Assign(AIS);
      //*this=AIS;
   };


   void SetMsgID(const DWORD dwVal)
   {
      if (dwVal>0)
      {
         nMsgID=dwVal;
         dwStatus|=AIS_STAT_MSGID_VALID;
      }
   }
   bool bMsgIDValid() const
   {
      return (dwStatus&AIS_STAT_MSGID_VALID)!=0;
   }
   int nGetMsgID() const
   {
      return nMsgID;
   }

   void SetMMSI(const DWORD dwVal)
   {
      if (dwVal>=0) // 0=not yet allocated
      {
         nMMSI=dwVal;
         dwStatus|=AIS_STAT_MMSI_VALID;
      }
   }
   bool bMMSIValid() const
   {
      return (dwStatus&AIS_STAT_MMSI_VALID)!=0;
   }
   int nGetMMSI() const
   {
      return nMMSI;
   }

   void SetNavStatus(const DWORD dwVal)
   {
      if (dwVal>=0&&dwVal<=15)
      {
         NavStatus=(AIS_NAVSTATUS_TYPE)dwVal;
         dwStatus|=AIS_STAT_NAVSTAT_VALID;
      }
   }
   bool bNavStatusValid() const
   {
      return (dwStatus&AIS_STAT_NAVSTAT_VALID)!=0;
   }
   AIS_NAVSTATUS_TYPE GetNavStatus() const
   {
      return NavStatus;
   }

   String sGetNavStatus() const
   {
		String sStat="";
		switch (NavStatus)
		{
			case AIS_NAVSTATUS_UNDER_WAY_USING_ENGINE:		sStat="Underway Using Engine";			break;
			case AIS_NAVSTATUS_AT_ANCHOR:							sStat="At Anchor";							break;
			case AIS_NAVSTATUS_NOT_UNDER_COMMAND:				sStat="Not Under Command";					break;
			case AIS_NAVSTATUS_RESTRICTED_MANOEUVRABILITY:	sStat="Restricted Manoeuvrability";		break;
			case AIS_NAVSTATUS_CONSTRAINED_BY_HER_DRAUGHT:	sStat="Contrained By Her Draught"; 		break;
			case AIS_NAVSTATUS_MOORED:								sStat="Moored";								break;
			case AIS_NAVSTATUS_AGROUND:							sStat="Aground";								break;
			case AIS_NAVSTATUS_ENGAGED_IN_FISHING:				sStat="Engaged In Fishing";				break;
			case AIS_NAVSTATUS_UNDER_WAY_SAILING:				sStat="Underway Sailing";					break;
			case AIS_NAVSTATUS_AIS_SART:							sStat="AIS Search and Rescue Transmitter";	break;
			default: 													sStat="Not Defined";


		}
		return sStat;
	}


	void SetROT(const double dVal)
	{
		dROT=dVal;
		dwStatus|=AIS_STAT_ROT_VALID;
	}

   void SetROT(const DWORD dwVal)
   {
      short nROT=dwVal<<8;
      nROT/=256;
      if (nROT>=-126&&nROT<=126)
      {
         dROT=(nROT*1.0/4.733)*(nROT*1.0/4.733);
         if (nROT<0)
            dROT*=-1.0;
         dwStatus|=AIS_STAT_ROT_VALID;
      }
   }
   bool bROTValid() const
   {
      return (dwStatus&AIS_STAT_ROT_VALID)!=0;
   }
   double dGetROT() const
   {
      return dROT;
   }


   void SetSOG(const double dVal)
   {
      dSOG=dVal;
      dwStatus|=AIS_STAT_SOG_VALID;
   }
   
   void SetSOG(const DWORD dwVal)
   {
      if (dwVal>=0&&dwVal<=1022)
      {

         dSOG=dwVal*0.1;
         dwStatus|=AIS_STAT_SOG_VALID;
      }
   }
   bool bSOGValid() const
   {
		return (dwStatus&AIS_STAT_SOG_VALID)!=0;
	}
	double dGetSOG() const
	{
		return dSOG;
	}

//	bool bZDAValid() const
//	{
//		return (dwStatus&AIS_STAT_ZDA_VALID)!=0;
//	}
//
//	void ClearZDAValid()
//	{
//		dwStatus&=(~AIS_STAT_ZDA_VALID);
//	}
//
//	void SetZDA_utc(const JTime& T_utc)
//	{
//		ZDA_T_utc=T_utc;
//		dwStatus|=AIS_STAT_ZDA_VALID;
//	}
//
//	JTime GetZDA_T_utc() const
//	{
//		return ZDA_T_utc;
//	}
//
//	bool bGGAValid() const
//	{
//		return (dwStatus&AIS_STAT_GGA_VALID)!=0;
//	}
//
//	void ClearGGAValid()
//	{
//		dwStatus&=(~AIS_STAT_GGA_VALID);
//	}
//
//	void SetGGA_LL(const LatLong& LL)
//	{
//		GGA_LL=LL;
//		dwStatus|=AIS_STAT_GGA_VALID;
//	}
//
//	LatLong GetGGA_LL() const
//	{
//		return GGA_LL;
//	}

	void SetHighPosAcc(const DWORD dwVal)
	{
      if (dwVal==0||dwVal==1)
      {
         bHighPosAcc=dwVal;
			dwStatus|=AIS_STAT_POSACC_VALID;
      }
	}
	void SetHighPosAcc(const bool bVal)
	{
		bHighPosAcc=bVal;
		dwStatus|=AIS_STAT_POSACC_VALID;
	}
   bool bHighPosAccValid() const
   {
      return (dwStatus&AIS_STAT_POSACC_VALID)!=0;
   }
   bool bGetHighPosAcc() const
   {
      return bHighPosAcc;
   }

//	bool bSuppressSartAlarm() const
//	{
//		return (dwStatus&AIS_STAT_NO_SART_ALARM)!=0;
//	}

//	void SetSuppressSartAlarm(const bool bSuppress)
//	{
//		if (bSuppress)
//			dwStatus|=AIS_STAT_NO_SART_ALARM;
//		else
//			dwStatus&=(~AIS_STAT_NO_SART_ALARM);
//	}

   void SetLL(const double dLatVal,const double dLongVal)
   {
      LL.dLat=dLatVal;
      LL.dLong=dLongVal;
      dwStatus|=AIS_STAT_LL_VALID;
   }

   void SetLL(const DWORD dwLat,const DWORD dwLong)
   {
      int nTmpLong=dwLong<<4; // 28-bit signed - convert to obtain sign
      nTmpLong/=16;

      int nTmpLat=dwLat<<5;   // 27-bit signed - convert to obtain sign
      nTmpLat/=32;
      double dMP=10000.0*60.0; // Lat/Long in 1/10000 minute steps

      if ((nTmpLong>=-180*dMP)&&(nTmpLong<=180*dMP)&&(nTmpLat>=-90*dMP)&&(nTmpLat<=90*dMP))
      {
         LL.dLat=double(nTmpLat)/dMP;
         LL.dLong=double(nTmpLong)/dMP;
         dwStatus|=AIS_STAT_LL_VALID;
      }
   }
   bool bLLValid() const
   {
      return (dwStatus&AIS_STAT_LL_VALID)!=0;
   }
   LatLong GetLL() const
   {
      return LL;
   }

   void SetCOG(const double dVal)
   {
      dCOG=dVal;
      dwStatus|=AIS_STAT_COG_VALID;
   }

   void SetCOG(const DWORD dwVal)
   {
      if (dwVal>=0&&dwVal<=3599)
      {
         dCOG=dwVal*0.1;
         dwStatus|=AIS_STAT_COG_VALID;
      }
   }
   bool bCOGValid() const
   {
      return (dwStatus&AIS_STAT_COG_VALID)!=0;
   }
   double dGetCOG() const
   {
      return dCOG;
   }

   void SetHdgTN(const double dVal)
   {
      nHdgTN=dVal;
      dwStatus|=AIS_STAT_HDG_VALID;
   }
   void SetHdgTN(const DWORD dwVal)
   {
      if (dwVal>=0&&dwVal<=359)
      {
         nHdgTN=dwVal;
         dwStatus|=AIS_STAT_HDG_VALID;
      }
   }
   bool bHdgTNValid() const
   {
      return (dwStatus&AIS_STAT_HDG_VALID)!=0;
   }
   int nGetHdgTN() const
   {
      return nHdgTN;
   }

   void SetMsgTime_utc(const JTime T_utc)
   {
      if (T_utc.bAllValid())
      {
         MsgTime_utc=T_utc;
         dwStatus|=AIS_STAT_MSGTIME_VALID;
      }
   }
   bool bMsgTime_utcValid() const
   {
      return (dwStatus&AIS_STAT_MSGTIME_VALID)!=0;
   }
   JTime GetMsgTime_utc() const
   {
      return MsgTime_utc;
   }

   void SetUTCSec(const DWORD dwVal)
   {
      if (dwVal>=0&&dwVal<=59)
      {
         nUTCSec=dwVal;
         dwStatus|=AIS_STAT_UTCSEC_VALID;
      }
   }
   bool bUTCSecValid() const
   {
      return (dwStatus&AIS_STAT_UTCSEC_VALID)!=0;
   }
   int nGetUTCSec() const
   {
      return nUTCSec;
   }

   void SetIMO(const DWORD dwVal)
   {
      if (dwVal>=1&&dwVal<=999999999)
      {
         nIMO=dwVal;
         dwStatus|=AIS_STAT_IMO_VALID;
      }
   }
   bool bIMOValid() const
   {
      return (dwStatus&AIS_STAT_IMO_VALID)!=0;
   }
   int nGetIMO() const
   {
      return nIMO;
   }


   void SetCallSign(const String sCallSign)
	{
		ClearCallSign();
		AnsiString s1=sCallSign;
		// remove all spaces
		s1.Trim();
		while (s1.Pos(" ") > 0)
			s1.Delete(s1.Pos(" "),1);
		if (!s1.IsEmpty())
		{
			memcpy(&acCallSign[0],s1.c_str(),s1.Length());
			dwStatus|=AIS_STAT_CALLSIGN_VALID;
		}
	}
	void SetCallSign(const char c,const int nIdx)
	{
		if (c!='@' && c!=' ')
		{
			acCallSign[nIdx]=c;
			dwStatus|=AIS_STAT_CALLSIGN_VALID;
		}
	}

	void ClearCallSign()
   {
      memset(&acCallSign[0],0,sizeof(acCallSign));
      dwStatus&=(~AIS_STAT_CALLSIGN_VALID);
   }
   bool bCallSignValid() const
   {
      return (dwStatus&AIS_STAT_CALLSIGN_VALID)!=0;
   }
   String sGetCallSign() const
   {
      // ensure String returned is null terminated
      char ac[sizeof(acCallSign)+1];
      memset(&ac[0],0,sizeof(ac));
      memcpy(&ac[0],&acCallSign[0],sizeof(acCallSign));
      return (String(ac)).Trim();
   }


   void SetShipName(const String sName)
   {
		ClearShipName();
		AnsiString s1=sName;
		if (!s1.IsEmpty())
		{
			memcpy(&acShipName[0],s1.c_str(),s1.Length());
			dwStatus|=AIS_STAT_SHIPNAME_VALID;
		}
	}

	void SetShipName(const char c,const int nIdx)
	{
		if (c!='@')
		{
			acShipName[nIdx]=c;
         dwStatus|=AIS_STAT_SHIPNAME_VALID;
      }
   }

   void ClearShipName()
   {
      memset(&acShipName[0],0,sizeof(acShipName));
      dwStatus&=(~AIS_STAT_SHIPNAME_VALID);
   }

   bool bShipNameValid() const
   {
      return (dwStatus&AIS_STAT_SHIPNAME_VALID)!=0;
   }

   String sGetShipName() const
   {
      // ensure String returned is null terminated
      char ac[sizeof(acShipName)+1];
      memset(&ac[0],0,sizeof(ac));
      memcpy(&ac[0],&acShipName[0],sizeof(acShipName));
      return (String(ac)).Trim();
   }


   void SetCargoAndType(const DWORD dwVal)
   {
      if (dwVal>=1&&dwVal<=99)
      {
         nShipAndCargoType=dwVal;
         dwStatus|=AIS_STAT_CARGO_VALID;
      }
   }

   String sGetShipAndCargoType(const String sPrefixDelimiter="\r\n",const String sPostfixDelimiter="");

	int nGetShipAndCargoType()
   {
      return nShipAndCargoType;
   }

   bool bShipAndCargoTypeValid() const
   {
      return (dwStatus&AIS_STAT_CARGO_VALID)!=0;
   }


   
   void SetGPSRef_ShipDims(const DWORD dwValA,const DWORD dwValB,const DWORD dwValC,const DWORD dwValD)
   {
      if (dwValA>=0&&dwValB>0&&dwValC>=0&&dwValD>0)
      {
         if (dwValA<512&&dwValB<512&&dwValC<64&&dwValD<64)
         {
            nGPSRef_ShipDim_A=dwValA;
            nGPSRef_ShipDim_B=dwValB;
            nGPSRef_ShipDim_C=dwValC;
            nGPSRef_ShipDim_D=dwValD;
            if ((dwValA>0&&dwValC>0))
               dwStatus|=AIS_STAT_GPSREFPOS_VALID;
            dwStatus|=AIS_STAT_SHIPDIMS_VALID;
         }
      }
   }

   int nGetShipLength()
   {
      return nGPSRef_ShipDim_A+nGPSRef_ShipDim_B;
   }
   int nGetShipWidth()
   {
      return nGPSRef_ShipDim_C+nGPSRef_ShipDim_D;
   }

   int nGetGPSRef_ShipDim_A()
   {
      return nGPSRef_ShipDim_A;
   }
   int nGetGPSRef_ShipDim_B()
   {
      return nGPSRef_ShipDim_B;
   }
   int nGetGPSRef_ShipDim_C()
   {
      return nGPSRef_ShipDim_C;
   }
   int nGetGPSRef_ShipDim_D()
   {
      return nGPSRef_ShipDim_D;
   }

   bool bShipDimsValid() const
   {
      return (dwStatus&AIS_STAT_SHIPDIMS_VALID)!=0;
   }
   bool bGPSRefPosValid() const
   {
      return (dwStatus&AIS_STAT_GPSREFPOS_VALID)!=0;
   }


   void SetNavDevice(const AIS_NAVDEVICE_TYPE Val)
   {
      NavDevice=Val;
      dwStatus|=AIS_STAT_NAVDEV_VALID;
   }
   void SetNavDevice(const DWORD dwVal)
   {
      if (dwVal>=1&&dwVal<=8)
      {
         NavDevice=(AIS_NAVDEVICE_TYPE)dwVal;
         dwStatus|=AIS_STAT_NAVDEV_VALID;
      }
   }
   bool bNavDeviceValid() const
   {
      return (dwStatus&AIS_STAT_NAVDEV_VALID)!=0;
   }
	AIS_NAVDEVICE_TYPE GetNavDevice() const
   {
		return NavDevice;
   }

   String sGetNavDevice() const
   {
		String sDev="";
		switch (NavDevice)
		{
			case AIS_NAVDEVICE_GPS:				sDev="GPS";				break;
			case AIS_NAVDEVICE_GLONASS:		sDev="GLONASS";		break;
			case AIS_NAVDEVICE_GPS_GLONASS:	sDev="GPS/GLONASS";	break;
			case AIS_NAVDEVICE_LORAN_C:		sDev="Loran C";		break;
			case AIS_NAVDEVICE_CHAYKA:			sDev="Chayka";			break;
			case AIS_NAVDEVICE_INS:				sDev="INS";				break;
			case AIS_NAVDEVICE_SURVEYED:		sDev="Surveyed";		break;
			case AIS_NAVDEVICE_GALILEO:		sDev="Galileo";		break;
			default: sDev="Undefined";
      }
      return sDev;
   }


	void SetUTC_ETA(const DWORD dwMonth,const DWORD dwDay,const DWORD dwHour,const DWORD dwMinute)
   {
      if (dwMonth>=1&&dwMonth<=12)
         if (dwDay>=1&&dwDay<=31)
            if (dwHour>=0&&dwHour<=23)
               if (dwMinute>=0&&dwMinute<=59)
					{
						UTC_ETA.wMonth=dwMonth;
						UTC_ETA.wDay=dwDay;
						UTC_ETA.wHour=dwHour;
						UTC_ETA.wMinute=dwMinute;
						dwStatus|=AIS_STAT_ETA_VALID;
					}

	}


	bool bUTC_ETAValid() const
	{
		return (dwStatus&AIS_STAT_ETA_VALID)!=0;
	}

	bool bGetUTC_ETA(int& nMonth,int& nDay,int& nHour,int& nMinute) const
   {
      bool bValid=bUTC_ETAValid();
      if (bValid)
      {
			nMonth=UTC_ETA.wMonth;
			nDay=UTC_ETA.wDay;
			nHour=UTC_ETA.wHour;
			nMinute=UTC_ETA.wMinute;
		}
		return bValid;
	}

	JTime GetUTC_ETA() const
	{
		JTime T;
		if (bUTC_ETAValid())
			T.Set(UTC_ETA);
		return T;
	}

   void SetDraught(const double dVal)
   {
      dDraught=dVal;
      dwStatus|=AIS_STAT_DRAUGHT_VALID;
   }
   void SetDraught(const DWORD dwVal)
	{
		if (dwVal>=1&&dwVal<=255)
		{
			dDraught=dwVal*0.1;
			dwStatus|=AIS_STAT_DRAUGHT_VALID;
		}

	}
	bool bDraughtValid() const
	{
		return (dwStatus&AIS_STAT_DRAUGHT_VALID)!=0;
	}
	double dGetDraught() const
	{
      return dDraught;
   }


   void SetDestination(const String sDest)
   {
		ClearDestination();
		AnsiString s1=sDest;
		if (!s1.IsEmpty())
		{
			memcpy(&acDestination[0],s1.c_str(),s1.Length());
			dwStatus|=AIS_STAT_DESTINATION_VALID;
		}
	}

	void SetDestination(const char c,const int nIdx)
   {
      if (c!='@')
      {
         acDestination[nIdx]=c;
         dwStatus|=AIS_STAT_DESTINATION_VALID;
      }
   }

   void ClearDestination()
   {
      memset(&acDestination[0],0,sizeof(acDestination));
      dwStatus&=(~AIS_STAT_DESTINATION_VALID);
   }

   bool bDestinationValid() const
   {
      return (dwStatus&AIS_STAT_DESTINATION_VALID)!=0;
   }

	String sGetDestination() const
	{
		// ensure String returned is null terminated
		char ac[sizeof(acDestination)+1];
		memset(&ac[0],0,sizeof(ac));
		memcpy(&ac[0],&acDestination[0],sizeof(acDestination));
		return (String(ac)).Trim();
	}

	void SetSMS(const SMSStruct _SMS)
   {
      if (_SMS.nMessageSequenceNr>=0&&_SMS.nMessageSequenceNr<=3)
         if (_SMS.nSrcMMSI>=0&&_SMS.nDestMMSI>=0)
            if (!_SMS.sTextMessage.IsEmpty())
               dwStatus|=AIS_STAT_SMS_VALID;
   }
   bool bSMSValid() const
   {
      return (dwStatus&AIS_STAT_SMS_VALID)!=0;
   }
   SMSStruct GetSMS() const
   {
      return SMS;
   }

   void SetMsgType(const AIS_MSG_TYPE _MsgType)
   {
      MsgType=_MsgType;
   }

   AIS_MSG_TYPE GetMsgType() const
   {
      return MsgType;
   }

   String sGetMsgType() const
   {
		String sType="";
		switch (MsgType)
		{
			case AIS_MSG_VDO: sType="VDO (Own Ship)";	break;
			case AIS_MSG_VDM: sType="VDM";			break;
			default: sType="Invalid";
		}

		return sType;
	}

	bool bIsOwnMsg() const
	{
      return MsgType==AIS_MSG_VDO;
   }

   void SetOwnMsg()
   {
      MsgType=AIS_MSG_VDO;
   }


	bool bContainsUpdatedExtInfo() const
	{
		return bExtInfoModified;
	}

	void SetContainsUpdatedExtInfo(const bool bSet)
	{
		bExtInfoModified=bSet;
	}

	bool bContainsVoyageStaticData() const
   {
      bool bOK=false;
      switch (nMsgID)
      {
         case 5:
         case 19:
         case 24: bOK=true;	break;
         default: ;
      }
      return bOK;
   }

   bool bContainsDynamicData() const
   {
		bool bOK=false;
		switch (nMsgID)
		{
			case 1:
			case 2:
			case 3:
			case 18:
			case 19: bOK=true;	break;
			default: ;
		}
		return bOK;
	}


	void UpdateExtInfo(AISMsgClass* pS) // for use by AISDecoderClass only
	{
		if (pS->bCallSignValid())
			if (sGetCallSign() != pS->sGetCallSign())
			{
				SetCallSign(pS->sGetCallSign());
				bExtInfoModified=true;
			}
		if (pS->bDestinationValid())
			if (sGetDestination() != pS->sGetDestination())
			{
				SetDestination(pS->sGetDestination());
				bExtInfoModified=true;
			}
		if (pS->bDraughtValid())
			if (dGetDraught() - pS->dGetDraught() > 0.001)
			{
				SetDraught(pS->dGetDraught());
				bExtInfoModified=true;
			}
		if (pS->bGPSRefPosValid() || pS->bShipDimsValid())
			if (nGetGPSRef_ShipDim_A() != pS->nGetGPSRef_ShipDim_A()) // if one dim differs, update all dims
			{
				SetGPSRef_ShipDims(pS->nGetGPSRef_ShipDim_A(),pS->nGetGPSRef_ShipDim_B(),pS->nGetGPSRef_ShipDim_C(),pS->nGetGPSRef_ShipDim_D());
				bExtInfoModified=true;
			}
		if (pS->bHighPosAccValid())
			if (bGetHighPosAcc() != pS->bGetHighPosAcc())
			{
				SetHighPosAcc(pS->bGetHighPosAcc());
				bExtInfoModified=true;
			}
		if (pS->bIMOValid())
			if (nGetIMO() != pS->nGetIMO())
			{
				SetIMO(pS->nGetIMO());
				bExtInfoModified=true;
			}
		if (pS->bNavDeviceValid())
			if (GetNavDevice() != pS->GetNavDevice())
			{
				SetNavDevice(pS->GetNavDevice());
				bExtInfoModified=true;
			}
		if (pS->bNavStatusValid())
		{
			if (GetNavStatus() != pS->GetNavStatus())
			{
				SetNavStatus(pS->GetNavStatus());
				bExtInfoModified=true;
			}
			else if (pS->GetNavStatus()==AIS_NAVSTATUS_AIS_SART)
				bExtInfoModified=true;
		}
		if (pS->bShipAndCargoTypeValid())
			if (nGetShipAndCargoType() != pS->nGetShipAndCargoType())
			{
				SetCargoAndType(pS->nGetShipAndCargoType());
				bExtInfoModified=true;
			}
		if (pS->bShipNameValid())
			if (sGetShipName() != pS->sGetShipName())
			{
				SetShipName(pS->sGetShipName());
				bExtInfoModified=true;
			}
		if (pS->bUTC_ETAValid())
		{
			int nMon1=0,nDay1=0,nHour1=0,nMin1=0,nMon2,nDay2,nHour2,nMin2;
			bGetUTC_ETA(nMon1,nDay1,nHour1,nMin1);
			if (pS->bGetUTC_ETA(nMon2,nDay2,nHour2,nMin2))
				if (nMon1 != nMon2 || nDay1 != nDay2 || nHour1 != nHour2 || nMin1 != nMin2)
				{
					SetUTC_ETA(nMon2,nDay2,nHour2,nMin2);
					bExtInfoModified=true;
				}
		}
	}


   DWORD dwGetBuf(BYTE* pucData,const DWORD dwMaxSize); // pack data buffer with AISMsgClass parameters; returns size of buffer used
   void SetBuf(BYTE* pucData,const DWORD dwSize); // unpack data buffer and populate AISMsgClass parameters

   DWORD dwGetExtraInfoBuf(BYTE* pucData,const DWORD dwMaxSize); // pack data buffer with STATIC AISMsgClass parameters; returns size of buffer used
   void SetExtraInfoBuf(BYTE* pucData,const DWORD dwSize); // unpack STATIC data buffer and populate AISMsgClass parameters

	void GetParamsFromExtraInfoBuf(BYTE* pucData,const DWORD dwSize,String& _sCallSign,String& _sShipName,int& _nIMO)
   {
		AnsiString s1;

		s1.SetLength(sizeof(acCallSign));
		DWORD dwOffset=sizeof(dwStatus);
		GetBufferData(s1.c_str(),sizeof(acCallSign),pucData,dwOffset,dwSize);
		for (int i=1;i<=s1.Length();i++)
		{
			if (s1[i]==0)
				s1.SetLength(i-1);
		}
		_sCallSign=s1;

		s1.SetLength(sizeof(acShipName));
		GetBufferData(s1.c_str(),sizeof(acShipName),pucData,dwOffset,dwSize);
		for (int i=1;i<=s1.Length();i++)
		{
			if (s1[i]==0)
				s1.SetLength(i-1);
		}
      _sShipName=s1;

      GetBufferData(&_nIMO,sizeof(nIMO),pucData,dwOffset,dwSize);
   };

   String sGetParamInfoStrings(const String sParameterDelimiter=L":\t",const String sNewLineDelimiter=L"\r\n");
   String sGetParamDescription(const int nIdx);
	String sGetParamValue(const int nIdx);
   int nGetParamStringsCount();


   AISMsgClass()
   {
		Invalidate();
   };

   ~AISMsgClass()
   {
      ParamDescriptionList.Clear();
      ParamValueList.Clear();
   };

};


// EXTENDED INFO PARAMS
/*
dwStatus
acCallSign
acShipName
nIMO
MsgType
nMsgID
nMMSI
NavStatus
bHighPosAcc
nShipAndCargoType
nGPSRef_ShipDim_A
nGPSRef_ShipDim_B
nGPSRef_ShipDim_C
nGPSRef_ShipDim_D
NavDevice
UTC_ETA
dDraught
acDestination
*/

#endif
