//---------------------------------------------------------------------------

#ifndef VistaAlarmMessagesH
#define VistaAlarmMessagesH

#include "JTransform.h"
#include "JTime.h"
#include "Cst4Defs.h"
#include "Cst4Messages.h"
#include "JList.h"
#include "AISDataDefs_210.h"
#include "Cst4SiteDefs.h"
#include "JUtils.h"
#include "JFile.h"
#include "JCommaText.h"
#include "VistaZoneMatrix.h"

#pragma option push -a4    //Set compiler to DWORD alignment

struct VistaAlarm_VesselStruct : public Cst4_MessageUtils
{
	String sName;
	int nMMSI;
	int nIMO;

	VistaAlarm_VesselStruct()
	{
		sName="";
		nMMSI=-1;
		nIMO=-1;
	}

	VistaAlarm_VesselStruct& operator = (const VistaAlarm_VesselStruct& V)
	{
		sName=V.sName;
		nMMSI=V.nMMSI;
		nIMO=V.nIMO;
		return *this;
	}


	bool operator < (const VistaAlarm_VesselStruct& V) const
	{
		return (sName<V.sName);
	}

	bool operator > (const VistaAlarm_VesselStruct& V) const
	{
		return (sName>V.sName);
	}

	int nGetPackedSize() const
	{
		return sizeof(nMMSI)+sizeof(nIMO)+dwPackedStringSize(sName);
	}

	void Pack(BYTE* pucBuf, int& n)
	{
		PackInt(nMMSI,pucBuf,n);
		PackInt(nIMO,pucBuf,n);
		PackString(sName,pucBuf,n);
	}

	void Unpack(const BYTE* pucBuf, int& n)
	{
		nMMSI=nUnpack(pucBuf,n);
		nIMO=nUnpack(pucBuf,n);
		sName=sUnpack(pucBuf,n);
	}

	bool bParseLine(const String sLine);
	String sListBoxLine();


};



//***************************************************************************

enum ALARM_CFG_TYPE
{
	ALARM_CFG_HARBOUR_ZONEDEF,
	ALARM_CFG_OTHER_ZONEDEF,
	ALARM_CFG_OFFLIMITS,
	ALARM_CFG_NO_FISHING_ZONE,
	ALARM_CFG_CROSSING,
	ALARM_CFG_SPEED,
	ALARM_CFG_COURSE,
	ALARM_CFG_LOITER,
	ALARM_CFG_WANTED,
	ALARM_CFG_COLLISIONBEHAVE,
	ALARM_CFG_COLLISIONCOURSE,
	ALARM_CFG_NEARMISS,
	ALARM_CFG_GPS_ERROR,

};

enum ALARM_SEVERITY
{
	VISTA_WARNING,
	VISTA_ALARM,
   VISTA_SEVERITY_COUNT,
};


//enum ALARM_STATUS
//{
//	VISTA_ALARM_ACTIVE,
//	VISTA_ALARM_ACKNOWLEDGED,
//	VISTA_ALARM_CANCELLED,
////	VISTA_ALARM_BLOCKED,
//};


class VistaAlarmCfg : public Cst4_MessageBase
{

	String sDescr;

	DWORD dwType;
	WORD wDataSize;
	BYTE* pucData;
	bool bActive;
	DWORD dwMsgID;
	ALARM_SEVERITY Severity;
	int nPersistence_min;

public:

	void Set(const String _sDescr, const JTime& _T, const WORD _wSourceID, const DWORD _dwType, const void* _pucData=NULL, const WORD _wDataSize=0)
	{
	sDescr=_sDescr;
		SetTime(_T,true);
		SetSource(_wSourceID);
		SetValid();
		dwType=_dwType;
		delete[] pucData;
		wDataSize=_wDataSize;
		if ((wDataSize>0)&&(_pucData!=NULL))
			{
			pucData=new BYTE[wDataSize];
			memcpy(pucData,_pucData,wDataSize);
			}
		else
			pucData=NULL;
		bActive=true;
	}

	VistaAlarmCfg(const String _sDescr, const JTime& _T, const WORD _wSourceID, const DWORD _dwType, const void* _pucData=NULL, const WORD _wDataSize=0) : Cst4_MessageBase(VISTA_ALARM_CFG_MSG)
	{
		pucData=NULL;
		CreateMsgID();
		Set(_sDescr,_T,_wSourceID,_dwType,_pucData,_wDataSize);
		Severity=VISTA_WARNING;
		nPersistence_min=5;

	}

	VistaAlarmCfg() : Cst4_MessageBase(VISTA_ALARM_CFG_MSG)
	{
		pucData=NULL;
		CreateMsgID();
		Severity=VISTA_WARNING;
		nPersistence_min=5;
	}

	VistaAlarmCfg(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(VISTA_ALARM_CFG_MSG)
	{
		pucData=NULL;
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}


	~VistaAlarmCfg()
	{
		delete[] pucData;
	}

	void Assign(const VistaAlarmCfg& M)
	{
		Cst4_MessageBase::Assign(M);
		Severity=M.Severity;
      nPersistence_min=M.nPersistence_min;
		sDescr=M.sDescr;
		dwType=M.dwType;
		bActive=M.bActive;
		dwMsgID=M.dwMsgID;
		delete[] pucData;
		wDataSize=M.wDataSize;
		if ((wDataSize>0)&&(M.pucData))
			{
			pucData=new BYTE[wDataSize];
			memcpy(pucData,M.pucData,wDataSize);
			}
		else
			pucData=NULL;
	}

	VistaAlarmCfg& operator = (const VistaAlarmCfg& M)
	{
		Assign(M);
		return *this;
	}

	VistaAlarmCfg(const VistaAlarmCfg& M)
	{
		pucData=NULL;
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()
					+sizeof(BYTE)   //Severity
               +sizeof(nPersistence_min)
					+dwPackedStringSize(sDescr)
					+sizeof(dwType)
					+sizeof(bActive)
					+sizeof(dwMsgID)
					+sizeof(wDataSize)+wDataSize;
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		PackBYTE(Severity,pucBuf,nOffs);
		PackInt(nPersistence_min,pucBuf,nOffs);
		PackString(sDescr,pucBuf,nOffs);
		PackDWORD(dwType,pucBuf,nOffs);
		PackBool(bActive,pucBuf,nOffs);
		PackDWORD(dwMsgID,pucBuf,nOffs);
		PackData((BYTE*)pucData,wDataSize,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		Severity=(ALARM_SEVERITY)ucUnpack(pucBuf,nOffs);
		nPersistence_min=nUnpack(pucBuf,nOffs);
		sDescr=sUnpack(pucBuf,nOffs);
		dwType=dwUnpack(pucBuf,nOffs);
		bActive=bUnpackBool(pucBuf,nOffs);
		dwMsgID=dwUnpack(pucBuf,nOffs);
		wDataSize=wUnpack(pucBuf,nOffs);
		delete[] pucData;
		if (wDataSize>0)
			{
			pucData=new BYTE[wDataSize];
			memcpy(pucData,&pucBuf[nOffs],wDataSize);
			nOffs+=(int)wDataSize;
			}
		else
			pucData=NULL;
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetType(const DWORD _dwType)
	{
		dwType=_dwType;
	}

	DWORD dwGetType() const
	{
		return dwType;
	}

	ALARM_CFG_TYPE GetType() const
	{
		return (ALARM_CFG_TYPE)dwType;
	}

	void SetSeverity(const ALARM_SEVERITY S)
	{
		Severity=S;
	}

	ALARM_SEVERITY GetSeverity() const
	{
		return Severity;
	}

	void SetPersistence_min(const int nT_min)
	{
		nPersistence_min=nT_min;
	}

	int nGetPersistence_min() const
	{
		return nPersistence_min;
	}

	DWORD dwGetPersistence_sec() const
	{
			return (DWORD)(nPersistence_min*60);
	}

	void* pGetData() const
	{
		return (void*)pucData;
	}

	int nGetDataSize() const
	{
		return (int)wDataSize;
	}

	int nGetData(void* p, const int nMaxSize)
	{
		int nSize=wDataSize;
		if (nSize>nMaxSize)
			nSize=nMaxSize;
		if (nSize>0)
			memcpy(p,pucData,nSize);
		return nSize;
	}

	void SetData(const void* p, const int nSize)
	{
		delete[] pucData;
		wDataSize=(WORD)nSize;
		if (nSize>0)
			{
			pucData=new BYTE[wDataSize];
			memcpy(pucData,p,wDataSize);
			}
		else
			pucData=NULL;
	}

	JFile::FILE_ERROR Extract(JFile* pF, const bool bShowHeader=true)
	{
		JFile::FILE_ERROR E=JFile::F_NO_ERROR;
		if (bShowHeader)
			{
			E=pF->TextLine("**********************************");
			if (!E) E=pF->TextLine("Alarm Configuration");
			}
		else
			E=pF->TextLine("Configuration");
		if (!E) E=pF->TextLine("    Description: "+sDescr);



		return E;
	}

	void SetActive(const bool bAct)
	{
		bActive=bAct;
	}

	bool bGetActive() const
	{
		return bActive;
	}

	void CreateMsgID()
	{
		Randomize();
		dwMsgID=JTime(true).nMilliSec()+random(1000000);
	}

	void SetMsgID(const DWORD dwID)
	{
		dwMsgID=dwID;
	}

	DWORD dwGetMsgID() const
	{
		return dwMsgID;
	}

	bool bSameID(const DWORD dwID) const
	{
		return (dwMsgID==dwID);
	}


	String sGetDescr() const
	{
		return sDescr;
	}

	void SetDescr(const String s)
	{
		sDescr=s;
	}

};

//**************************************************************************

class ShipListClass : public Cst4_MessageUtils
{

	JList<VistaAlarm_VesselStruct> ShipList;

public:

	ShipListClass()
	{
		ShipList.Clear();
	}


	void AssignShips(const ShipListClass& S)
	{
		ShipList=S.ShipList;
	}

	ShipListClass& operator = (const class ShipListClass& S)
	{
		AssignShips(S);
		return *this;
	}

	void ClearShips()
	{
		ShipList.Clear();
	}

	int nGetShipsSize()
	{
		int nSize=sizeof(WORD);
		ShipList.GoFirst();
		while(!ShipList.bLast())
			{
			nSize+=ShipList.pNext()->nGetPackedSize();
			}
		return nSize;
	}

	void PackShips(BYTE* pucBuf, int& nOffs)
	{
		WORD wNum=ShipList.nGetCount();
		PackWORD(wNum,pucBuf,nOffs);
		ShipList.GoFirst();
		while(!ShipList.bLast())
			ShipList.pNext()->Pack(pucBuf,nOffs);
	}

	void UnpackShips(const BYTE* pucBuf, int& nOffs)
	{
		WORD wNum=wUnpack(pucBuf,nOffs);
		ShipList.Clear();
		for (WORD w=0; w<wNum; w++)
			{
			VistaAlarm_VesselStruct* pV=new VistaAlarm_VesselStruct;
			pV->Unpack(pucBuf,nOffs);
			ShipList.nAdd(pV);
			}
		ShipList.Pack();
	}


	void AddShip(const String sName, const int nMMSI, const int nIMO)
	{
		VistaAlarm_VesselStruct* pV=new VistaAlarm_VesselStruct;
		pV->sName=sName.UpperCase().Trim();
		pV->nMMSI=nMMSI;
		pV->nIMO=nIMO;
		ShipList.nAdd(pV);
		ShipList.Pack();
	}

	int nGetShipsCount() const
	{
		return ShipList.nGetCount();
	}

	String sGetShipName(const int n)
	{
		return ShipList[n]->sName;
	}

	int nGetShipMMSI(const int n)
	{
		return ShipList[n]->nMMSI;
	}

	int nGetShipIMO(const int n)
	{
		return ShipList[n]->nIMO;
	}

	VistaAlarm_VesselStruct& GetShip(const int n)
	{
		return *ShipList[n];
	}

	void AddShip(const VistaAlarm_VesselStruct& V)
	{
		VistaAlarm_VesselStruct* pV=new VistaAlarm_VesselStruct;
		*pV=V;
		ShipList.nAdd(pV);
		ShipList.Pack();
	}

	bool bValidShip(const String sShip, const int nMMSI=-1, const int nIMO=-1)
	{
		String sShipC=sShip.UpperCase().Trim();
		bool bFound=false;
		VistaAlarm_VesselStruct* pV;
		ShipList.GoFirst();
		while((!ShipList.bLast())&&(!bFound))
			{
			pV=ShipList.pNext();
			bFound=(sShipC.Pos(pV->sName)>0);
			if ((!bFound)&&(nMMSI>0))
				bFound=(pV->nMMSI==nMMSI);
			if ((!bFound)&&(nIMO>0))
				bFound=(pV->nIMO==nIMO);
			}
		return bFound;
	}

};

//**************************************************************************

class Vista_ZoneDef_Data : public Cst4_MessageUtils
{

public:

	enum ZONE_TYPE
	{
		GENERAL_ZONE,
		HARBOUR_APPROACH,
      CROSSING_ZONE,
	};

private:

	BYTE ucZoneType;
	JList<LatLong> WayPointList;
	DWORD dwID;
   String sDescr;

	ZoneMatrix* pMatrix;

public:

	Vista_ZoneDef_Data()
	{
		ucZoneType=GENERAL_ZONE;
		WayPointList.Clear();
		pMatrix=NULL;
		dwID=0;
      sDescr="";
	}

	Vista_ZoneDef_Data(const Vista_ZoneDef_Data& D)
	{
      pMatrix=NULL;
		Assign(D);
	}

	~Vista_ZoneDef_Data()
	{
		delete pMatrix;
	}

	void Assign(const Vista_ZoneDef_Data& D)
	{
		ucZoneType=D.ucZoneType;
		WayPointList=D.WayPointList;
		dwID=D.dwID;
      sDescr=D.sDescr;
	}

	Vista_ZoneDef_Data& operator = (const Vista_ZoneDef_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_ZoneDef_Data(const void* pPackedData)
	{
		pMatrix=NULL;
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(ucZoneType)
					+sizeof(WORD)+sizeof(LatLong)*WayPointList.nGetCount()+sizeof(dwID)
					+dwPackedStringSize(sDescr);
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackBYTE(ucZoneType,pucBuf,nOffs);
		PackDWORD(dwID,pucBuf,nOffs);
		PackString(sDescr,pucBuf,nOffs);
		WORD wNum=WayPointList.nGetCount();
		PackWORD(wNum,pucBuf,nOffs);
		WayPointList.GoFirst();
		while(!WayPointList.bLast())
			PackLL(*WayPointList.pNext(),pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		ucZoneType=ucUnpack(pucBuf,nOffs);
		dwID=dwUnpack(pucBuf,nOffs);
		sDescr=sUnpack(pucBuf,nOffs);
		WORD wNum=wUnpack(pucBuf,nOffs);
		WayPointList.Clear();
		for (WORD w=0; w<wNum; w++)
			{
			LatLong* pLL=new LatLong;
			*pLL=UnpackLL(pucBuf,nOffs);
			WayPointList.nAdd(pLL);
			}
		WayPointList.Pack();
	}

	void SetZoneType(const ZONE_TYPE A)
	{
		ucZoneType=(BYTE)A;
	}

	ZONE_TYPE GetZoneType() const
	{
		return (ZONE_TYPE)ucZoneType;
	}

	void SetID(const DWORD dw)
	{
		dwID=dw;
	}

	void CreateID()
	{
		Randomize();
		dwID=JTime(true).nMilliSec()+random(1000000);
	}

	DWORD dwGetID() const
	{
		return dwID;
	}

	String sGetDescr() const
	{
		return sDescr;
	}

	void SetDescr(const String s)
	{
		sDescr=s;
	}

	void ClearWayPoints()
	{
		WayPointList.Clear();
	}

	void AddWayPoint(const LatLong& LL)
	{
		LatLong* pLL=new LatLong;
		*pLL=LL;
		WayPointList.nAdd(pLL);
		WayPointList.Pack();
		if (pMatrix)
			{
			delete pMatrix;
			pMatrix=NULL;
			}
	}

	int nGetWayPointCount() const
	{
		return WayPointList.nGetCount();
	}

	LatLong GetWayPoint(const int n)
	{
		return *WayPointList[n];
	}

   bool bCalcMatrix();
	bool bInZone(const LatLong& LL);

};
//**************************************************************************

class Vista_OffLimits_Data : public ShipListClass
{

public:

	enum ZONE_TYPE
	{
		GENERAL_ZONE,
		HARBOUR_APPROACH,
		COASTLINE_APPROACH_ALARM,
		OFF_LIMITS_ALARM,
		NAV_WARNING_ALARM,
		NO_FISHING_ALARM,
	};

private:

	ZONE_TYPE ZoneType;
	DWORD dwZoneID;
	int nLoiterTime_min;
	int nLoiterRadius_m;

public:

	Vista_OffLimits_Data()
	{
		ZoneType=GENERAL_ZONE;
		ClearShips();
	}

	Vista_OffLimits_Data(const Vista_OffLimits_Data& D)
	{
		Assign(D);
	}

	~Vista_OffLimits_Data()
	{
	}

	void Assign(const Vista_OffLimits_Data& D)
	{
		ZoneType=D.ZoneType;
		nLoiterTime_min=D.nLoiterTime_min;
		nLoiterRadius_m=D.nLoiterRadius_m;
		dwZoneID=D.dwZoneID;
		AssignShips(D);
	}

	Vista_OffLimits_Data& operator = (const Vista_OffLimits_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_OffLimits_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(BYTE)  //ZoneType
					+sizeof(nLoiterRadius_m)
					+sizeof(nLoiterTime_min)
					+sizeof(dwZoneID)
					+sizeof(BYTE);  //Severity
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackBYTE(ZoneType,pucBuf,nOffs);
		PackInt(nLoiterTime_min,pucBuf,nOffs);
		PackInt(nLoiterRadius_m,pucBuf,nOffs);
		PackDWORD(dwZoneID,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		ZoneType=(ZONE_TYPE)ucUnpack(pucBuf,nOffs);
		nLoiterTime_min=nUnpack(pucBuf,nOffs);
		nLoiterRadius_m=nUnpack(pucBuf,nOffs);
		dwZoneID=dwUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetZoneType(const ZONE_TYPE Z)
	{
		ZoneType=Z;
	}

	ZONE_TYPE GetZoneType() const
	{
		return ZoneType;
	}

	void SetZoneID(const DWORD dwID)
	{
		dwZoneID=dwID;
	}

	DWORD dwGetZoneID() const
	{
      return dwZoneID;
   }

	void SetLoiterTime(const int nT_min)
	{
		nLoiterTime_min=nT_min;
	}

	int nGetLoiterTime() const
	{
		return nLoiterTime_min;
	}

	void SetLoiterRadius(const int nRadius_m)
	{
		nLoiterRadius_m=nRadius_m;
	}

	int nGetLoiterRadius() const
	{
		return nLoiterRadius_m;
	}

};

//**************************************************************************

class Vista_CrossingAlarm_Data : public ShipListClass
{

	LatLong StartPoint;
	LatLong EndPoint;
	WORD wCrossFrom_deg;
	WORD wCrossTo_deg;

public:

	Vista_CrossingAlarm_Data()
	{
		ClearShips();
	}

	Vista_CrossingAlarm_Data(const Vista_CrossingAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_CrossingAlarm_Data()
	{
	}

	void Assign(const Vista_CrossingAlarm_Data& D)
	{
		StartPoint=D.StartPoint;
		EndPoint=D.EndPoint;
		wCrossFrom_deg=D.wCrossFrom_deg;
		wCrossTo_deg=D.wCrossTo_deg;
		AssignShips(D);
	}

	Vista_CrossingAlarm_Data& operator = (const Vista_CrossingAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_CrossingAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(BYTE)  //Severity
					+sizeof(StartPoint)
					+sizeof(EndPoint)
					+sizeof(wCrossFrom_deg)
					+sizeof(wCrossTo_deg)
					+nGetShipsSize();
//					+ExclShips.nGetSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackLL(StartPoint,pucBuf,nOffs);
		PackLL(EndPoint,pucBuf,nOffs);
		PackWORD(wCrossFrom_deg,pucBuf,nOffs);
		PackWORD(wCrossTo_deg,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		StartPoint=UnpackLL(pucBuf,nOffs);
		EndPoint=UnpackLL(pucBuf,nOffs);
		wCrossFrom_deg=wUnpack(pucBuf,nOffs);
		wCrossTo_deg=wUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetLine(const LatLong& Start, const LatLong& End)
	{
		StartPoint=Start;
		EndPoint=End;
	}

	LatLong GetStartPoint() const
	{
		return StartPoint;
	}

	LatLong GetEndPoint() const
	{
		return EndPoint;
	}

	void SetCrossFrom(const double dFrom_deg)
	{
		wCrossFrom_deg=dFrom_deg*65536.0/360.0;
	}

	double dGetCrossFrom() const
	{
		return wCrossFrom_deg*360.0/65536.0;
	}

	void SetCrossTo(const double dTo_deg)
	{
		wCrossTo_deg=dTo_deg*65536.0/360.0;
	}

	double dGetCrossTo() const
	{
		return wCrossTo_deg*360.0/65536.0;
	}

};

//**************************************************************************

class Vista_SpeedCourseAlarm_Data : public ShipListClass
{

public:

	struct SpeedCourseStruct : public Cst4_MessageUtils
	{
		int nAISType;
		WORD wMinSpeed;
		WORD wMaxSpeed;
		WORD wMaxTurnRate;

		SpeedCourseStruct()
		{
			nAISType=0;
		}

		SpeedCourseStruct& operator = (const SpeedCourseStruct& V)
		{
			nAISType=V.nAISType;
			wMinSpeed=V.wMinSpeed;
			wMaxSpeed=V.wMaxSpeed;
			wMaxTurnRate=V.wMaxTurnRate;
			return *this;
		}


		int nGetPackedSize() const
		{
			return sizeof(nAISType)
					+sizeof(wMinSpeed)
					+sizeof(wMaxSpeed)
					+sizeof(wMaxTurnRate);
		}

		void Pack(BYTE* pucBuf, int& nOffs)
		{
			PackInt(nAISType,pucBuf,nOffs);
			PackWORD(wMinSpeed,pucBuf,nOffs);
			PackWORD(wMaxSpeed,pucBuf,nOffs);
			PackWORD(wMaxTurnRate,pucBuf,nOffs);
		}

		void Unpack(const BYTE* pucBuf, int& nOffs)
		{
			nAISType=nUnpack(pucBuf,nOffs);
			wMinSpeed=wUnpack(pucBuf,nOffs);
			wMaxSpeed=wUnpack(pucBuf,nOffs);
			wMaxTurnRate=wUnpack(pucBuf,nOffs);
		}

		int nGetAISType() const
		{
			return nAISType;
		}

		void SetAISType(const int n)
		{
			nAISType=n;
		}

		void SetMinSpeed_kts(const double dSpeed_kts)
		{
			wMinSpeed=dSpeed_kts*100;
		}

		double dGetMinSpeed_kts() const
		{
			return wMinSpeed/100.0;
		}

		void SetMaxSpeed_kts(const double dSpeed_kts)
		{
			wMaxSpeed=dSpeed_kts*100;
		}

		double dGetMaxSpeed_kts() const
		{
			return wMaxSpeed/100.0;
		}

		void SetMaxTurnRate_deg_sec(const double dTurn_deg_sec)
		{
			wMaxTurnRate=dTurn_deg_sec*1000;
		}

		double dGetMaxTurnRate_deg_sec() const
		{
			return wMaxTurnRate/1000.0;
		}

	};

private:

//   DWORD dwZoneID;
	JList<SpeedCourseStruct> AISList;
	int nHistory_min;

public:

	Vista_SpeedCourseAlarm_Data()
	{
		AISList.Clear();
		ClearShips();
	}

	Vista_SpeedCourseAlarm_Data(const Vista_SpeedCourseAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_SpeedCourseAlarm_Data()
	{
	}

	void Assign(const Vista_SpeedCourseAlarm_Data& D)
	{
	  AISList=D.AISList;
	  AssignShips(D);
	}

	Vista_SpeedCourseAlarm_Data& operator = (const Vista_SpeedCourseAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_SpeedCourseAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(WORD)  //Count of AIS List
						+(sizeof(int)  //AISType
						+sizeof(WORD)  //wMinSpeed
						+sizeof(WORD) 	//wMaxSpeed
						+sizeof(WORD)) //wTurnRate
							*AISList.nGetCount()
					+nGetShipsSize()
//					+sizeof(dwZoneID);
					+sizeof(nHistory_min);
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
//		PackDWORD(dwZoneID,pucBuf,nOffs);
		PackInt(nHistory_min,pucBuf,nOffs);
		WORD wNum=AISList.nGetCount();
		PackWORD(wNum,pucBuf,nOffs);
		AISList.GoFirst();
		while(!AISList.bLast())
			AISList.pNext()->Pack(pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
//		dwZoneID=dwUnpack(pucBuf,nOffs);
		nHistory_min=nUnpack(pucBuf,nOffs);
		WORD wNum=wUnpack(pucBuf,nOffs);
		AISList.Clear();
		for (WORD w=0; w<wNum; w++)
			{
			SpeedCourseStruct* pSP=new SpeedCourseStruct;
			pSP->Unpack(pucBuf,nOffs);
			AISList.nAdd(pSP);
			}
		AISList.Pack();
		UnpackShips(pucBuf,nOffs);
	}


//	void SetZoneID(const DWORD dwID)
//	{
//		dwZoneID=dwID;
//	}
//
//	DWORD dwGetZoneID() const
//	{
//		return dwZoneID;
//	}

	void SetHistory_min(const int nHist)
	{
		nHistory_min=nHist;
	}

	int nGetHistory_min() const
	{
		return nHistory_min;
	}

	void ClearAISList()
	{
		AISList.Clear();
	}

	void AddSpeedCourse(const int nAIS, const double dMinSpeed_kts, const double dMaxSpeed_kts, const double dMaxTurnRate_deg_s)
	{
		SpeedCourseStruct* pSP=new SpeedCourseStruct;
		pSP->nAISType=nAIS;
		pSP->SetMinSpeed_kts(dMinSpeed_kts);
		pSP->SetMaxSpeed_kts(dMaxSpeed_kts);
		pSP->SetMaxTurnRate_deg_sec(dMaxTurnRate_deg_s);
		AISList.nAdd(pSP);
		AISList.Pack();
	}

	void AddSpeedCourse(const SpeedCourseStruct& SC)
	{
		SpeedCourseStruct* pSP=new SpeedCourseStruct;
		*pSP=SC;
		AISList.nAdd(pSP);
		AISList.Pack();
	}

	SpeedCourseStruct& GetSpeedCourse(const int n)
	{
		if (n<AISList.nGetCount())
			return *AISList[n];
		else
			return *AISList[0];  //Always valid
	}

	int nGetAISCount() const
	{
		return AISList.nGetCount();
	}

	int nGetAISType(const int n)
	{
		if (n<AISList.nGetCount())
			return AISList[n]->nAISType;
		else
			return 0;

	}

	double dGetMinSpeed_kts(const int n)
	{
		if (n<AISList.nGetCount())
			return AISList[n]->dGetMinSpeed_kts();
		else
			return 0.0; //Will nver raise an alarm

	}

	double dGetMaxSpeed_kts(const int n)
	{
		if (n<AISList.nGetCount())
			return AISList[n]->dGetMaxSpeed_kts();
		else
			return 1000.0; //Will nver raise an alarm
	}

	double dGetMaxTurnRate_deg_sec(const int n)
	{
		if (n<AISList.nGetCount())
			return AISList[n]->dGetMaxTurnRate_deg_sec();
		else
			return 1000.0; //Will nver raise an alarm
	}

};

//**************************************************************************

class Vista_LoiterAlarm_Data : public ShipListClass
{

	int nLoiterTime_min;
	int nLoiterRadius_m;
//	DWORD dwZoneID;

public:

	Vista_LoiterAlarm_Data()
	{
		ClearShips();
	}

	Vista_LoiterAlarm_Data(const Vista_LoiterAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_LoiterAlarm_Data()
	{
	}

	void Assign(const Vista_LoiterAlarm_Data& D)
	{
		nLoiterTime_min=D.nLoiterTime_min;
		nLoiterRadius_m=D.nLoiterRadius_m;
//      dwZoneID=D.dwZoneID;
      AssignShips(D);
	}

	Vista_LoiterAlarm_Data& operator = (const Vista_LoiterAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_LoiterAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(nLoiterTime_min)+sizeof(nLoiterRadius_m);
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nLoiterTime_min,pucBuf,nOffs);
		PackInt(nLoiterRadius_m,pucBuf,nOffs);
//		PackDWORD(dwZoneID,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nLoiterTime_min=nUnpack(pucBuf,nOffs);
		nLoiterRadius_m=nUnpack(pucBuf,nOffs);
//		dwZoneID=dwUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}


//	void SetZoneID(const DWORD dwID)
//	{
//		dwZoneID=dwID;
//	}
//
//	DWORD dwGetZoneID() const
//	{
//		return dwZoneID;
//	}

	void SetLoiterTime(const int nT_min)
	{
		nLoiterTime_min=nT_min;
	}

	int nGetLoiterTime() const
	{
		return nLoiterTime_min;
   }

	void SetLoiterRadius(const int nRadius_m)
	{
		nLoiterRadius_m=nRadius_m;
	}

	int nGetLoiterRadius() const
	{
		return nLoiterRadius_m;
	}

};

//**************************************************************************

class Vista_WantedAlarm_Data : public ShipListClass
{

public:

	enum WANTED_TYPE
	{
		WANTED_SHIP,
		BLACKLISTED_SHIP,
	};

private:

	WANTED_TYPE WantedType;

public:

	Vista_WantedAlarm_Data()
	{
		ClearShips();
	}

	Vista_WantedAlarm_Data(const Vista_WantedAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_WantedAlarm_Data()
	{
	}

	void Assign(const Vista_WantedAlarm_Data& D)
	{
		WantedType=D.WantedType;
		AssignShips(D);
	}

	Vista_WantedAlarm_Data& operator = (const Vista_WantedAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_WantedAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(BYTE);    //WantedType
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackBYTE((BYTE)WantedType,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		WantedType=(WANTED_TYPE)ucUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetWantedType(const WANTED_TYPE WType)
	{
		WantedType=WType;
	}

	WANTED_TYPE GetWantedType() const
	{
		return WantedType;
	}

};

//**************************************************************************

class Vista_CollisionBehaveAlarm_Data : public ShipListClass
{

	int nHistory_min;
	int nLoiterRadius_m;
	int nSpeed_perc;

public:

	Vista_CollisionBehaveAlarm_Data()
	{
		ClearShips();
	}

	Vista_CollisionBehaveAlarm_Data(const Vista_CollisionBehaveAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_CollisionBehaveAlarm_Data()
	{
	}

	void Assign(const Vista_CollisionBehaveAlarm_Data& D)
	{
		nHistory_min=D.nHistory_min;
		nLoiterRadius_m=D.nLoiterRadius_m;
		nSpeed_perc=D.nSpeed_perc;
		AssignShips(D);
//		ExclList=D.ExclList;
	}

	Vista_CollisionBehaveAlarm_Data& operator = (const Vista_CollisionBehaveAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_CollisionBehaveAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(nHistory_min)+sizeof(nLoiterRadius_m)+sizeof(nSpeed_perc);
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nHistory_min,pucBuf,nOffs);
		PackInt(nLoiterRadius_m,pucBuf,nOffs);
		PackInt(nSpeed_perc,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nHistory_min=nUnpack(pucBuf,nOffs);
		nLoiterRadius_m=nUnpack(pucBuf,nOffs);
		nSpeed_perc=nUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetHistory_min(const int nHist)
	{
		nHistory_min=nHist;
	}

	int nGetHistory_min() const
	{
		return nHistory_min;
	}


	void SetRadius_m(const int nRad)
	{
      nLoiterRadius_m=nRad;
	}

	int nGetRadius_m() const
	{
		return nLoiterRadius_m;
	}

	int nGetPerentage() const
	{
		return nSpeed_perc;
	}

};

//**************************************************************************

class Vista_CollisionCourseAlarm_Data : public ShipListClass
{

	int nMaxTime_min;
	int nCPA_m;

public:

	Vista_CollisionCourseAlarm_Data()
	{
		ClearShips();
	}

	Vista_CollisionCourseAlarm_Data(const Vista_CollisionCourseAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_CollisionCourseAlarm_Data()
	{
	}

	void Assign(const Vista_CollisionCourseAlarm_Data& D)
	{
		nMaxTime_min=D.nMaxTime_min;
		nCPA_m=D.nCPA_m;
      AssignShips(D);
//		ExclList=D.ExclList;
	}

	Vista_CollisionCourseAlarm_Data& operator = (const Vista_CollisionCourseAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_CollisionCourseAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(nMaxTime_min)+sizeof(nCPA_m);
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nMaxTime_min,pucBuf,nOffs);
		PackInt(nCPA_m,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nMaxTime_min=nUnpack(pucBuf,nOffs);
		nCPA_m=nUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetMaxTime_min(const int nMin)
	{
		nMaxTime_min=nMin;
	}

	int nGetMaxTime_min() const
	{
		return nMaxTime_min;
	}

	void SetCPA_m(const int nC)
	{
		nCPA_m=nC;
	}

	int nGetCPA_m() const
	{
		return nCPA_m;
	}

};

//**************************************************************************

class Vista_NearMissAlarm_Data : public ShipListClass
{

	int nCPA_m;

public:

	Vista_NearMissAlarm_Data()
	{
		ClearShips();
	}

	Vista_NearMissAlarm_Data(const Vista_NearMissAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_NearMissAlarm_Data()
	{
	}

	void Assign(const Vista_NearMissAlarm_Data& D)
	{
		nCPA_m=D.nCPA_m;
		AssignShips(D);
	}

	Vista_NearMissAlarm_Data& operator = (const Vista_NearMissAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_NearMissAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(nCPA_m);
		nSize+=nGetShipsSize();
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nCPA_m,pucBuf,nOffs);
		PackShips(pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nCPA_m=nUnpack(pucBuf,nOffs);
		UnpackShips(pucBuf,nOffs);
	}

	void SetCPA_m(const int nC)
	{
		nCPA_m=nC;
	}

	int nGetCPA_m() const
	{
		return nCPA_m;
	}

};

//**************************************************************************

class Vista_GPSErrorAlarm_Data : public Cst4_MessageUtils
{

	int nMaxError_m;


public:

	Vista_GPSErrorAlarm_Data()
	{
	}

	Vista_GPSErrorAlarm_Data(const Vista_NearMissAlarm_Data& D)
	{
		Assign(D);
	}

	~Vista_GPSErrorAlarm_Data()
	{
	}

	void Assign(const Vista_GPSErrorAlarm_Data& D)
	{
		nMaxError_m=D.nMaxError_m;
	}

	Vista_GPSErrorAlarm_Data& operator = (const Vista_GPSErrorAlarm_Data& D)
	{
		Assign(D);
		return *this;
	}

	Vista_GPSErrorAlarm_Data(const void* pPackedData)
	{
		UnPack((BYTE*)pPackedData);
	}

	int nGetSize()
	{
		int nSize=sizeof(nMaxError_m);
		return nSize;
	}

	void Pack(BYTE* pucBuf)
	{
		int nOffs=0;
		PackInt(nMaxError_m,pucBuf,nOffs);
	}

	void UnPack(const BYTE* pucBuf)
	{
		int nOffs=0;
		nMaxError_m=nUnpack(pucBuf,nOffs);
	}

	void SetMaxError_m(const int nM)
	{
		nMaxError_m=nM;
	}

	int nGetMaxError_m() const
	{
		return nMaxError_m;
	}

};


//***************************************************************************

struct VistaAlarmTriggerID : public Cst4_MessageUtils
{

	DWORD dwCfgID;
	int anMMSI[2];

	VistaAlarmTriggerID()
	{
		dwCfgID=0;
		anMMSI[0]=-1;
		anMMSI[1]=-1;
	}

	VistaAlarmTriggerID(const DWORD dwID, const int nMMSI_A, const int nMMSI_B=-1)
	{
		dwCfgID=dwID;
		anMMSI[0]=nMMSI_A;
		anMMSI[1]=nMMSI_B;
	}

	VistaAlarmTriggerID& operator = (const VistaAlarmTriggerID& T)
	{
		dwCfgID=T.dwCfgID;
		anMMSI[0]=T.anMMSI[0];
		anMMSI[1]=T.anMMSI[1];
		return *this;
	}

	int nGetPackedSize() const
	{
		return sizeof(dwCfgID)
		+sizeof(anMMSI[0])
		+sizeof(anMMSI[1]);
	}

	void Pack(BYTE* pucBuf, int& n)
	{
		PackDWORD(dwCfgID,pucBuf,n);
		PackInt(anMMSI[0],pucBuf,n);
		PackInt(anMMSI[1],pucBuf,n);
	}

	void Unpack(const BYTE* pucBuf, int& n)
	{
		dwCfgID=dwUnpack(pucBuf,n);
		anMMSI[0]=nUnpack(pucBuf,n);
		anMMSI[1]=nUnpack(pucBuf,n);
	}

	bool bValid(const int n) const
	{
		return (anMMSI[n]>=0);
	}

	void Invalidate(const int n)
	{
		anMMSI[n]=-1;
	}

	void SortMMSI()
	{
		if (anMMSI[0]>anMMSI[1])
			{
			int n=anMMSI[0];
			anMMSI[0]=anMMSI[1];
			anMMSI[1]=n;
			}
	}

	bool bSameTrig(const VistaAlarmTriggerID& T) const
	{
		if (dwCfgID!=T.dwCfgID)
			return false;
		//If it is the same 2 ships, check if they are the same, irrespective of the order
		if (bValid(0) && bValid(1))
			{
			if (!T.bValid(0) || !T.bValid(1))
				return false;
			if ((anMMSI[0]!=T.anMMSI[0]) || (anMMSI[1]!=T.anMMSI[1]))
				return false;
			}
		else
			{
			if (bValid(0))
				{
				if (anMMSI[0]!=T.anMMSI[0])
					return false;
				}
			if (bValid(1))
				{
				if (anMMSI[1]!=T.anMMSI[1])
					return false;
				}
			}
		return true;
	}




};

//***************************************************************************

class VistaAlarmTrigger : public Cst4_MessageBase
{

	struct VesselStruct : public Cst4_MessageUtils
	{
	String sName;
	int nMMSI;
	int nIMO;
	int nType;
	bool bValid;
	LatLong LL;
	WORD wSpeed_kts;
	WORD wCourse_deg;

	VesselStruct()
	{
		sName="";
		nMMSI=-1;
		nIMO=-1;
		nType=-1;
		wSpeed_kts=0;
		wCourse_deg=0;
		bValid=false;
		LL.Invalidate();
	}

	VesselStruct& operator = (const VesselStruct& V)
	{
		sName=V.sName;
		nMMSI=V.nMMSI;
		nIMO=V.nIMO;
		nType=V.nType;
		bValid=V.bValid;
		LL=V.LL;
		wSpeed_kts=V.wSpeed_kts;
		wCourse_deg=V.wCourse_deg;
		return *this;
	}

	int nGetPackedSize() const
	{
		return dwPackedStringSize(sName)
		+sizeof(nMMSI)
		+sizeof(nIMO)
		+sizeof(nType)
		+sizeof(bValid)
		+sizeof(LL)
		+sizeof(wSpeed_kts)
		+sizeof(wCourse_deg);

	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		PackInt(nMMSI,pucBuf,nOffs);
		PackInt(nIMO,pucBuf,nOffs);
		PackInt(nType,pucBuf,nOffs);
		PackBool(bValid,pucBuf,nOffs);
		PackString(sName,pucBuf,nOffs);
		PackLL(LL,pucBuf,nOffs);
		PackWORD(wSpeed_kts,pucBuf,nOffs);
		PackWORD(wCourse_deg,pucBuf,nOffs);
	}

	void Unpack(const BYTE* pucBuf, int& nOffs)
	{
		nMMSI=nUnpack(pucBuf,nOffs);
		nIMO=nUnpack(pucBuf,nOffs);
		nType=nUnpack(pucBuf,nOffs);
		bValid=bUnpackBool(pucBuf,nOffs);
		sName=sUnpack(pucBuf,nOffs);
		LL=UnpackLL(pucBuf,nOffs);
		wSpeed_kts=wUnpack(pucBuf,nOffs);
		wCourse_deg=wUnpack(pucBuf,nOffs);
	}

	double dGetSpeed_kts() const
	{
		return wSpeed_kts/10.0;
	}

	double dGetCourse_deg() const
	{
		return wCourse_deg*360.0/65536.0;
	}


	JFile::FILE_ERROR Extract(JFile* pF)
	{
		JFile::FILE_ERROR E=pF->TextLine("   Vessel");
		if (!E) E=pF->TextLine("   "+sName);
		if (!E) E=pF->TextLine("   "+LL.sLL());
		String s;
		s.printf(L"    Speed: %1.1f kts",dGetSpeed_kts());
		if (!E) E=pF->TextLine(s);
		s.printf(L"    Course: %1.1f deg",dGetCourse_deg());
		if (!E) E=pF->TextLine(s);
      return E;
	}

};


	VistaAlarmCfg TriggerCfg;
	VesselStruct aVessel[2];
	int nAlarmInstance;
	String sTrigDescr;
	int nCount;
	DWORD dwLastUpdate_utc_sec;
	BYTE ucStatus;
	DWORD dwAlarmID;

public:

	enum ALARM_STATUS
	{
		ALARM_ACTIVE   = 0x00,
		ALARM_HANDLED 	= 0x01,
		ALARM_SILENCED = 0x02,
		ALARM_IGNORED  = 0x04,
		ALARM_EXPIRED  = 0x08,
	};

	VistaAlarmTrigger() : Cst4_MessageBase(VISTA_ALARM_TRIGGER_MSG)
	{
		ucStatus=ALARM_ACTIVE;
		nCount=0;
		sTrigDescr="";
		CreateAlarmID();
	}

	VistaAlarmTrigger(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(VISTA_ALARM_TRIGGER_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}


	~VistaAlarmTrigger()
	{
	}

	void Assign(const VistaAlarmTrigger& M)
	{
		Cst4_MessageBase::Assign(M);
		TriggerCfg=M.TriggerCfg;
		aVessel[0]=M.aVessel[0];
		aVessel[1]=M.aVessel[1];
		nAlarmInstance=M.nAlarmInstance;
		sTrigDescr=M.sTrigDescr;
		nCount=M.nCount;
		dwLastUpdate_utc_sec=M.dwLastUpdate_utc_sec;
		ucStatus=M.ucStatus;
      dwAlarmID=M.dwAlarmID;
	}

	VistaAlarmTrigger& operator = (const VistaAlarmTrigger& M)
	{
		Assign(M);
		return *this;
	}

	VistaAlarmTrigger(const VistaAlarmTrigger& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize()
				+TriggerCfg.dwCalcSize()
				+aVessel[0].nGetPackedSize()
				+aVessel[1].nGetPackedSize()
				+sizeof(nAlarmInstance)
				+sizeof(nCount)
				+sizeof(dwLastUpdate_utc_sec)
				+sizeof(ucStatus)
            +sizeof(dwAlarmID)
				+dwPackedStringSize(sTrigDescr);
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
		TriggerCfg.Pack(pucBuf,nOffs);
		aVessel[0].Pack(pucBuf,nOffs);
		aVessel[1].Pack(pucBuf,nOffs);
		PackInt(nAlarmInstance,pucBuf,nOffs);
		PackInt(nCount,pucBuf,nOffs);
		PackDWORD(dwLastUpdate_utc_sec,pucBuf,nOffs);
		PackBYTE(ucStatus,pucBuf,nOffs);
		PackDWORD(dwAlarmID,pucBuf,nOffs);
		PackString(sTrigDescr,pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
		TriggerCfg.bUnpack(pucBuf,nOffs,nMaxSize);
		aVessel[0].Unpack(pucBuf,nOffs);
		aVessel[1].Unpack(pucBuf,nOffs);
		nAlarmInstance=nUnpack(pucBuf,nOffs);
		nCount=nUnpack(pucBuf,nOffs);
		dwLastUpdate_utc_sec=dwUnpack(pucBuf,nOffs);
		ucStatus=ucUnpack(pucBuf,nOffs);
		dwAlarmID=dwUnpack(pucBuf,nOffs);
		sTrigDescr=sUnpack(pucBuf,nOffs);
		return true;
	};

	int nCreateTXBuf(BYTE* pucBuf)
	{
		int nOffs=0;
		pucBuf[nOffs++]='C';
		pucBuf[nOffs++]='S';
		pucBuf[nOffs++]='T';
		pucBuf[nOffs++]='4';
		Pack(pucBuf,nOffs);
		return nOffs;
	}

	void SetCfg(const VistaAlarmCfg& Cfg)
	{
		TriggerCfg=Cfg;
	}

	VistaAlarmCfg GetCfg()
	{
		return TriggerCfg;
	}

	DWORD dwGetCfgID() const
	{
		return TriggerCfg.dwGetMsgID();
	}

	bool bSameTrig(const VistaAlarmTriggerID& ID)
	{
		VistaAlarmTriggerID ID1;
		ID1=GetTriggerID();
		return ID1.bSameTrig(ID);
	}

	bool bSameTrig(VistaAlarmTrigger& T)
	{
		return bSameTrig(T.GetTriggerID());
	}

	void CreateAlarmID()
	{
		Randomize();
		dwAlarmID=JTime(true).nMilliSec()+random(1000000);
	}

	void SetAlarmID(const DWORD dwID)
	{
		dwAlarmID=dwID;
	}

	DWORD dwGetAlarmID() const
	{
		return dwAlarmID;
	}

	bool bSameID(const DWORD dwID) const
	{
		return (dwAlarmID==dwID);
	}

	void SetStatusByte(const BYTE ucStat)
	{
		ucStatus=ucStat;
	}

	void SetStatusBit(const ALARM_STATUS Stat)
	{
		ucStatus|=Stat;
	}

	void ClearStatusBit(const ALARM_STATUS Stat)
	{
			ucStatus&=(~Stat);
	}

	BYTE ucGetStatusByte() const
	{
		return ucStatus;
	}

	bool bActive() const
	{
		return (ucStatus==ALARM_ACTIVE);
	}

	bool bHandled() const
	{
		return (ucStatus&ALARM_HANDLED)!=0;
	}

	bool bSilenced() const
	{
		return (ucStatus&ALARM_SILENCED)!=0;
	}

	bool bIgnored() const
	{
		return (ucStatus&ALARM_IGNORED)!=0;
	}

	bool bExpired() const
	{
		return (ucStatus&ALARM_EXPIRED)!=0;
	}

	void SetExpired()
	{
		ucStatus|=ALARM_EXPIRED;
	}

	void SetCount(const int n)
	{
		nCount=n;
	}

	void IncrCount()
	{
		++nCount;
	}

	int nGetCount() const
	{
		return nCount;
	}

	void SetVessel(const int n, const String sName, const int nType, const int nMMSI, const int nIMO, const LatLong& LL)
	{
		aVessel[n].bValid=true;
		aVessel[n].sName=sName;
		aVessel[n].nType=nType;
		aVessel[n].nMMSI=nMMSI;
		aVessel[n].nIMO=nIMO;
		aVessel[n].LL=LL;
	}

	void SetVesselValid(const int n, const bool bV)
	{
		aVessel[n].bValid=bV;
	}


	bool bGetVesselValid(const int n) const
	{
		return aVessel[n].bValid;
	}


	String sGetVesselName(const int n) const
	{
		return aVessel[n].sName;
	}

	int nGetVesselMMSI(const int n) const
	{
		return aVessel[n].nMMSI;
	}

	int nGetVesselIMO(const int n) const
	{
		return aVessel[n].nIMO;
	}

	int nGetVesselType(const int n) const
	{
		return aVessel[n].nType;
	}

	LatLong GetVesselLL(const int n) const
	{
		return aVessel[n].LL;
	}

	void SetVesselLL(const int n, const LatLong LL)
	{
		aVessel[n].LL=LL;
	}

	void SetVesselSpeed_kts(const int n, const double dS_kts)
	{
		aVessel[n].dGetSpeed_kts();
	}

	void SetVesselCourse_deg(const int n, const double dC_deg)
	{
		aVessel[n].dGetCourse_deg();
	}

	double dGetVesselSpeed_kts(const int n) const
	{
		return aVessel[n].wSpeed_kts/10.0;
	}

	double dGetVesselCourse_deg(const int n) const
	{
		return aVessel[n].wCourse_deg*360.0/65536.0;
	}

	void CreateInstance()
	{
		Randomize();
		nAlarmInstance=JTime(true).nMilliSec()+random(1000000);
}

	void SetInstance(const int nInstance)
	{
		nAlarmInstance=nInstance;
	}

	int nGetInstance() const
	{
		return nAlarmInstance;
	}

	JFile::FILE_ERROR Extract(JFile* pF)
	{
		JFile::FILE_ERROR E=pF->TextLine("**********************************");
		if (!E) E=pF->TextLine("Alarm Trigger");
//		if (!E) E=pF->TextLine("   Time: "+Time_loc().sDateTime(false,false,true));
		JTime T;
		T.SetTotalSec(dwLastUpdate_utc_sec,true);
		if (!E) E=pF->TextLine("   Last Update: "+T.sDateTime(false,false,true));
		if (!E) E=pF->TextLine("   Count: "+String(nCount));
		if (bGetVesselValid(0))
			E=aVessel[0].Extract(pF);
		if (bGetVesselValid(1))
			E=aVessel[1].Extract(pF);

		if (!E) E=TriggerCfg.Extract(pF,false);

		return E;
	}

	void SetDescr(const String s)
	{
		sTrigDescr=s;
	}

	String sGetDescr() const
	{
		return sTrigDescr;
	}

	void SetLastUpdate_utc(const JTime& T_utc)
	{
		dwLastUpdate_utc_sec=T_utc.dwTotalSec();
	}

	void SetLastUpdate_utc(const DWORD dwT_utc_sec)
	{
		dwLastUpdate_utc_sec=dwT_utc_sec;
	}

	bool bStale_utc_sec(const DWORD dwT_utc_sec)
	{
		return (dwT_utc_sec>dwLastUpdate_utc_sec+TriggerCfg.nGetPersistence_min()*60);
	}

	VistaAlarmTriggerID GetTriggerID()
	{
		VistaAlarmTriggerID ID;
		ID.dwCfgID=dwGetCfgID();
		if (bGetVesselValid(0))
			ID.anMMSI[0]=nGetVesselMMSI(0);
		else
			ID.Invalidate(0);
		if (bGetVesselValid(1))
			ID.anMMSI[0]=nGetVesselMMSI(1);
		else
			ID.Invalidate(1);
		ID.SortMMSI();
		return ID;
	}

	String sLine()
	{
		String sL="";
		sL=Time_loc().sDateTime();
		sL=sL+"  "+sTrigDescr+"  "+GetVesselLL(0).sLL();
		if (bExpired())
			sL=sL+"  EXPIRED";
		else
			{
			if (bHandled())
				sL=sL+"  HANDLED";
			if (bSilenced())
				sL=sL+"  SILENCED";
			if (bIgnored())
				sL=sL+"  IGNORED";
			}
		return sL;
	}


};


#pragma option pop    //Set compiler back to default alignment

#endif
