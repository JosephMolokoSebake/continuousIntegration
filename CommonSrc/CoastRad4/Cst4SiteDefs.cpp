//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4SiteDefs.h"
#include "JReadExcel.h"
#include "JUtils.h"
#include "JAES.h"
#include "JTransform.h"
#include "JIniFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Cst4SiteDefs* g_pSiteDefs=NULL;

Cst4SiteDefs::Cst4SiteDefs()
{
}

Cst4SiteDefs::~Cst4SiteDefs()
{
}

LatLong Cst4SiteDefs::GetLL(const String sLat, const String sLong)
{
	LatLong LL;
	if (!sLat.IsEmpty() && !sLong.IsEmpty())
		{
		try
			{
			AnsiString sa=sLat;
			double dMult;
			for (int i=1; i<=sa.Length(); i++)
				{
				switch(sa[i])
					{
					case ':':	sa[i]=','; 	break;
					case 'N':
					case 'n':   sa[i]=' ';
									dMult=1.0;	break;
					case 'S':
					case 's':   sa[i]=' ';
									dMult=-1.0;	break;
					default:		;
					}
				}
			CommaText Lat(sa.Trim());
			if (Lat.nGetCount()==2)
				LL.dLat=(Lat.dGetVal(0)+Lat.dGetVal(1)/60.0)*dMult;
			sa=sLong;
			for (int i=1; i<=sa.Length(); i++)
				{
				switch(sa[i])
					{
					case ':':	sa[i]=','; 	break;
					case 'E':
					case 'e':   sa[i]=' ';
									dMult=1.0;	break;
					case 'W':
					case 'w':   sa[i]=' ';
									dMult=-1.0;	break;
					default:		;
					}
				}
			CommaText Long(sa.Trim());
			if (Long.nGetCount()==2)
				LL.dLong=(Long.dGetVal(0)+Long.dGetVal(1)/60.0)*dMult;
			}
		catch(...)
			{
			LL.Invalidate();
			}
		}
	return LL;
}

bool Cst4SiteDefs::bLoadXLSList(const String sFile)
{
	memset(awCountryIdx,0xFF,sizeof(awCountryIdx));
	CountryStruct* pCountry=NULL;
	LocStruct* pLoc=NULL;
	String sF=sFile;
	bool bFound=bFileExist(sF);
	nVersion=-1;
	if (bFound)
		{
		JReadExcel Exc(sF,"CoastRad4 Site Definitions");
		int nR=1;
		if (Exc.bOK())
			{
			do
				{
				String sA=Exc.sCell(nR,'A').UpperCase().Trim();
				String sB=Exc.sCell(nR,'B').Trim();
				String sNet=Exc.sCell(nR,'J').UpperCase();
				if (sA==L"VERSION")
					{
					nVersion=sB.ToInt();
					}
				if (sA==L"COUNTRY")
					{
               if (pCountry)
                  {
                  if (pLoc)
                     {
                     pCountry->LocList.nAdd(pLoc);
                     pLoc=NULL;
                     }
                  CountryList.nAdd(pCountry);
                  }
               pCountry=new CountryStruct;
               pCountry->sName=sB;
               pCountry->wID=Exc.nCell(nR,'C');
               }
				else if (sA==L"LOCATION")
					{
					bool bUseLoc=(sNet.Pos("V")>0);	//Vistanet Site
					if (pCountry)
						{
						if (pLoc)
							pCountry->LocList.nAdd(pLoc);
						if (bUseLoc)
							{
							pLoc=new LocStruct;
							pLoc->sName=sB;
							pLoc->wID=Exc.nCell(nR,'C');
							}
						else
							pLoc=NULL;
						}
					}
				else
					{
					if (sA.IsEmpty() && !sB.IsEmpty())
						{
                  if (pLoc)
							{
							bool bUse=(sNet.Pos("V")>0);	//Vistanet Site
							if (bUse)
								{
								TypeStruct* pType=new TypeStruct;
								pType->sName=sB;
								pType->wID=Exc.nCell(nR,'C');
								pType->nSurfID=Exc.nCell(nR,'D');
								pType->sSurfExt=Exc.sCell(nR,'F').Trim();
								pType->LL=GetLL(Exc.sCell(nR,'G').Trim(),Exc.sCell(nR,'H').Trim());
								pType->nHeight_m=Exc.nCell(nR,'I');
                        pType->nType=Exc.nCell(nR,'K');
                        pType->dRadius_km=Exc.dCell(nR,'L');
                        pType->sDesigAddr=Exc.sCell(nR,'M');
                        pType->nDesigPort=Exc.nCell(nR,'N');
								pLoc->TypeList.nAdd(pType);
								}
							}
						}
               }
            }
         while(++nR<1000);
         }
      if (pCountry)
         {
         if (pLoc)
            pCountry->LocList.nAdd(pLoc);
         CountryList.nAdd(pCountry);
         }
      CountryList.Pack();
      WORD wID;
      for (int nC=0; nC<CountryList.nGetCount(); nC++)
         {
         memset(CountryList[nC]->awLocIdx,0xFF,sizeof(CountryList[nC]->awLocIdx));
			awCountryIdx[CountryList[nC]->wID]=nC;
			CountryList[nC]->LocList.Sort();
			CountryList[nC]->LocList.Pack();
			for (int nL=0; nL<CountryList[nC]->LocList.nGetCount(); nL++)
				{
				CountryList[nC]->LocList[nL]->TypeList.Sort();
				CountryList[nC]->LocList[nL]->TypeList.Pack();
				wID=CountryList[nC]->LocList[nL]->wID;
            if (wID<MAX_LOCATION_CNT)
               {
               CountryList[nC]->awLocIdx[wID]=nL;
               memset(CountryList[nC]->LocList[nL]->awTypeIdx,0xFF,sizeof(CountryList[nC]->LocList[nL]->awTypeIdx));
               for (int nT=0; nT<CountryList[nC]->LocList[nL]->TypeList.nGetCount(); nT++)
                  {
                  wID=CountryList[nC]->LocList[nL]->TypeList[nT]->wID;
                  if (wID<MAX_TYPE_CNT)
                     CountryList[nC]->LocList[nL]->awTypeIdx[wID]=nT;
                  }
               }
            }
         }
      return CountryList.nGetCount()>0;
      }
   return false;
}

JFile::FILE_ERROR Cst4SiteDefs::TypeStruct::Store(JFile* pFil)
{
	AnsiString as=sName;
	int n=as.Length();
	JFile::FILE_ERROR E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(as.c_str(),n);
	if (!E)
		E=pFil->Write(&wID,sizeof(wID));
	if (!E)
		E=pFil->Write(&nSurfID,sizeof(nSurfID));
	as=sSurfExt;
	n=as.Length();
	if (!E)
		E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(as.c_str(),n);
	if (!E)
		E=pFil->Write(&LL,sizeof(LL));
	if (!E)
		E=pFil->Write(&nHeight_m,sizeof(nHeight_m));

	if (!E)
		E=pFil->Write(&nType,sizeof(nType));
	if (!E)
		E=pFil->Write(&dRadius_km,sizeof(dRadius_km));
	as=sDesigAddr;
	n=as.Length();
	if (!E)
		E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(as.c_str(),n);
	if (!E)
		E=pFil->Write(&nDesigPort,sizeof(nDesigPort));



	for (int i=0; (i<16) && (!E); i++)
		E=pFil->Write(&anExtra[i],sizeof(anExtra[i]));


	return E;
}

bool Cst4SiteDefs::TypeStruct::bRead(JMemFile* pFil, const DWORD /*dwVer*/)
{
	int n;
	bool bOK=pFil->bRead(&n,sizeof(n));
	if ((n>0)&&(bOK))
		{
		char* pacS=new char[n+1];
		bOK=pFil->bRead(pacS,n);
		pacS[n]=0;
		sName=String(pacS);
		delete[] pacS;
		}
	else
		sName="";
	if (bOK)
		bOK=pFil->bRead(&wID,sizeof(wID));
	if (bOK)
		bOK=pFil->bRead(&nSurfID,sizeof(nSurfID));
	if (bOK)
		bOK=pFil->bRead(&n,sizeof(n));
	if ((n>0)&&(bOK))
		{
		char* pacS=new char[n+1];
		bOK=pFil->bRead(pacS,n);
		pacS[n]=0;
		sSurfExt=String(pacS);
		delete[] pacS;
		}
	else
		sSurfExt="";
	if (bOK)
		bOK=pFil->bRead(&LL,sizeof(LL));
	if (bOK)
		bOK=pFil->bRead(&nHeight_m,sizeof(nHeight_m));

	if (bOK)
		bOK=pFil->bRead(&nType,sizeof(nType));
	if (bOK)
		bOK=pFil->bRead(&dRadius_km,sizeof(dRadius_km));

	if (bOK)
		bOK=pFil->bRead(&n,sizeof(n));
	if ((n>0)&&(bOK))
		{
		char* pacS=new char[n+1];
		bOK=pFil->bRead(pacS,n);
		pacS[n]=0;
		sDesigAddr=String(pacS);
		delete[] pacS;
		}
	else
		sDesigAddr="";
	if (bOK)
		bOK=pFil->bRead(&nDesigPort,sizeof(nDesigPort));

	for (int i=0; (i<16) && bOK; i++)
		bOK=pFil->bRead(&anExtra[i],sizeof(anExtra[i]));


	return bOK;
}


JFile::FILE_ERROR Cst4SiteDefs::LocStruct::Store(JFile* pFil)
{
	AnsiString asName=sName;
	int n=asName.Length();
	JFile::FILE_ERROR E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(asName.c_str(),n);
	if (!E)
		E=pFil->Write(&wID,sizeof(wID));
	n=TypeList.nGetCount();
	if (!E)
		E=pFil->Write(&n,sizeof(n));
	TypeList.Pack();
	for (int i=0; (i<n)&&(!E); i++)
		E=TypeList[i]->Store(pFil);
	if (!E)
		E=pFil->Write(awTypeIdx,sizeof(awTypeIdx));
	return E;
}

bool Cst4SiteDefs::LocStruct::bRead(JMemFile* pFil, const DWORD dwVer)
{
	int n;
	bool bOK=pFil->bRead(&n,sizeof(n));
	if ((n>0)&&(bOK))
		{
		char* pacS=new char[n+1];
		bOK=pFil->bRead(pacS,n);
		pacS[n]=0;
		sName=String(pacS);
		delete[] pacS;
		}
	else
		sName="";
	if (bOK)
		bOK=pFil->bRead(&wID,sizeof(wID));
	TypeList.Clear();
	if (bOK)
		bOK=pFil->bRead(&n,sizeof(n));
	for (int i=0; (i<n)&&(bOK); i++)
		{
		TypeStruct* pT=new TypeStruct;
		bOK=pT->bRead(pFil,dwVer);
		if (bOK)
			TypeList.nAdd(pT);
		else
			delete pT;
		}
	TypeList.Pack();
	if (bOK)
		bOK=pFil->bRead(awTypeIdx,sizeof(awTypeIdx));
	return bOK;
}



JFile::FILE_ERROR Cst4SiteDefs::CountryStruct::Store(JFile* pFil)
{
	AnsiString asName=sName;
	int n=asName.Length();
	JFile::FILE_ERROR E=pFil->Write(&n,sizeof(n));
	if ((n>0)&&(!E))
		E=pFil->Write(asName.c_str(),n);
	if (!E)
		E=pFil->Write(&wID,sizeof(wID));
	n=LocList.nGetCount();
	if (!E)
		E=pFil->Write(&n,sizeof(n));
	LocList.Pack();
	for (int i=0; (i<n)&&(!E); i++)
		E=LocList[i]->Store(pFil);
	if (!E)
		E=pFil->Write(awLocIdx,sizeof(awLocIdx));
	return E;
}

bool Cst4SiteDefs::CountryStruct::bRead(JMemFile* pFil, const DWORD dwVer)
{
	int n;
	bool bOK=pFil->bRead(&n,sizeof(n));
	if ((n>0)&&(bOK))
		{
		char* pacS=new char[n+1];
		bOK=pFil->bRead(pacS,n);
		pacS[n]=0;
		sName=String(pacS);
		delete[] pacS;
		}
	else
		sName="";
	if (bOK)
		bOK=pFil->bRead(&wID,sizeof(wID));
	LocList.Clear();
	if (bOK)
		bOK=pFil->bRead(&n,sizeof(n));
	for (int i=0; (i<n)&&(bOK); i++)
		{
		LocStruct* pL=new LocStruct;
		bOK=pL->bRead(pFil,dwVer);
		if (bOK)
			LocList.nAdd(pL);
		else
			delete pL;
		}
	LocList.Pack();
	if (bOK)
		bOK=pFil->bRead(awLocIdx,sizeof(awLocIdx));
	return bOK;
}

bool Cst4SiteDefs::bStore(const bool bEncrypt)
{
	JFile Fil('O');
	String sFile=sGet_SiteFile_Name();
	JFile::FILE_ERROR E=Fil.Create(sFile);
	if (!E)
		{
//		DWORD dwVer=SITEDEFS_VERSION;
		DWORD dwVer=nVersion;
		if (bEncrypt)
			dwVer|=0x80000000;
		E=Fil.Write(&dwVer,sizeof(dwVer));
		}
	CountryList.Pack();
	int n=CountryList.nGetCount();
	if (!E)
		Fil.Write(&n,sizeof(n));
	for (int i=0; (i<n)&&(!E); i++)
		E=CountryList[i]->Store(&Fil);
	if (!E)
		E=Fil.Write(awCountryIdx,sizeof(awCountryIdx));
	if (!E)
		E=Fil.Close();
	if ((!E)&&(bEncrypt))
		{
		JFile F('B');
		E=F.Open(sFile);
		int nPlainSize=(int)F.dwGetSize();
		if ((!E)&&(nPlainSize>0))
			{
			BYTE* pucBuf=new BYTE[nPlainSize+31];
			E=F.Read(0,pucBuf,nPlainSize);
			if (!E)
				{
				JAES AES("MacKenzie");
				nPlainSize-=sizeof(DWORD);
				BYTE* pucData=&pucBuf[sizeof(DWORD)];
				int nCipherSize=AES.nEncrypt(pucData,nPlainSize);	//Skip version
				E=F.Write(0,pucBuf,nCipherSize+sizeof(DWORD));
				}
			delete[] pucBuf;
			}
		}
	Fil.ShowError(E);
	return (E==JFile::F_NO_ERROR);
}

bool Cst4SiteDefs::bLoadSites5()
{
	DWORD dwVer;
	String sFile=sGet_SiteFile_Name();
   sFolder=sFileDirOnly(sFile);
	bool bOK=bFileExist(sFile);
	if (bOK)
		{
		JMemFile Fil;
		JFile::FILE_ERROR E=Fil.ReadBuffer(sFile);
		if (!E)
			{
			bOK=Fil.bRead(&dwVer,sizeof(dwVer));
			bool bDecrypt=(dwVer&0x80000000)!=0;
			dwVer&=0x7FFFFFFF;
         nVersion=(int)dwVer;
			if (bDecrypt)
				{
				JAES AES("MacKenzie");
				BYTE* pucBuf=Fil.pucGetBuffer();
				BYTE* pucData=&pucBuf[sizeof(DWORD)];
				int nCipherSize=Fil.dwGetSize()-sizeof(DWORD);
				int nPlainSize=AES.nDecrypt(pucData,nCipherSize);
				if (nPlainSize<=0)
					{
					//Decryption Error
               bOK=false;
					}
				}
			}
		else
			bOK=false;
		CountryList.Clear();
		int n;
		if (bOK)
			bOK=Fil.bRead(&n,sizeof(n));
		for (int i=0; (i<n)&&(bOK); i++)
			{
			CountryStruct* pC=new CountryStruct;
			bOK=pC->bRead(&Fil,dwVer);
			if (bOK)
				CountryList.nAdd(pC);
			else
				delete pC;
			}
		CountryList.Pack();
		if (bOK)
			bOK=Fil.bRead(awCountryIdx,sizeof(awCountryIdx));
		}
	if (bOK)
      SetTmpFromFile();
   else
		nShowError("Error Reading File: "+sFile,"Site Definition Error",30);
	return bOK;
}

void Cst4SiteDefs::GetIndexFromID(const WORD wID, int& nCountryIdx, int& nLocIdx, int& nTypeIdx)
{
	nCountryIdx=-1;
	nLocIdx=-1;
   nTypeIdx=-1;
	int nCountry=(wID>>COUNTRY_SHIFT)&COUNTRY_MASK;
   if (nCountry<MAX_COUNTRY_CNT)
   	{
	   nCountryIdx=awCountryIdx[nCountry];
   	if (nCountryIdx<CountryList.nGetCount())
      	{
         int nLoc=(wID>>LOCATION_SHIFT)&LOCATION_MASK;
         if (nLoc<MAX_LOCATION_CNT)
         	{
      		nLocIdx=CountryList[nCountryIdx]->awLocIdx[nLoc];
		      if (nLocIdx<CountryList[nCountryIdx]->LocList.nGetCount())
					{
               int nType=(wID>>TYPE_SHIFT)&TYPE_MASK;
               if ((nType>0)&&(nType<MAX_TYPE_CNT))
                  {
                  nTypeIdx=CountryList[nCountryIdx]->LocList[nLocIdx]->awTypeIdx[nType];
						if (nTypeIdx>=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList.nGetCount())
							nCountryIdx=-1;
                  }
               }
            }
         }
      }
}

String Cst4SiteDefs::sGetDesigAddrFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
      	{
         String sAddr=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sDesigAddr;
         return sAddr;
         }
		}
	return "";
}

int Cst4SiteDefs::nGetDesigPortFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
			return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->nDesigPort;
		}
	return 0;
}

Cst4SiteDefs::SYSTEM_TYPE Cst4SiteDefs::GetSystemType(const int nCountryIdx, const int nLocIdx,const int nTypeIdx)
{
	if ((nCountryIdx>=0)&&(nLocIdx>=0)&&(nTypeIdx>=0))
		return (SYSTEM_TYPE)CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->nType;
	return UNDEFINED_TYPE;
}

Cst4SiteDefs::SYSTEM_TYPE Cst4SiteDefs::GetSystemTypeFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if ((nCountryIdx>=0)&&(nCountryIdx<65535))
			return GetSystemType(nCountryIdx,nLocIdx,nTypeIdx);
		}
	return UNDEFINED_TYPE;
}

bool Cst4SiteDefs::bIsSensor(const WORD wSiteID)
{
	Cst4SiteDefs::SYSTEM_TYPE Sys=GetSystemTypeFromID(wSiteID);
	switch(Sys)
		{
		case AIS_TYPE:
		case RADAR_TYPE:
		case IFF_TYPE:
		case LRIT_TYPE:
		case SAT_AIS_TYPE:
		case SHIP_POS_TYPE:
		case TRACKING_RADAR_TYPE:
			return true;
		default:
			return false;
		}
}

bool Cst4SiteDefs::bIsSensor(const int nCountryIdx, const int nLocIdx,const int nTypeIdx)
{
	Cst4SiteDefs::SYSTEM_TYPE Sys=GetSystemType(nCountryIdx,nLocIdx,nTypeIdx);
	switch(Sys)
		{
		case AIS_TYPE:
		case RADAR_TYPE:
		case IFF_TYPE:
		case LRIT_TYPE:
		case SAT_AIS_TYPE:
		case SHIP_POS_TYPE:
		case TRACKING_RADAR_TYPE:
			return true;
		default:
			return false;
		}
}

String Cst4SiteDefs::sGetDescrFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if ((nCountryIdx>=0)&&(nCountryIdx<MAX_COUNTRY_CNT)&&(nLocIdx>=0)&&(nTypeIdx>=0))
			{
			return CountryList[nCountryIdx]->LocList[nLocIdx]->sName+" "+CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sName;
			}
		else
			{
			String s;
			return s.sprintf(L"Source #%04X",wSiteID);
			}
		}
	return "";
}

LatLong Cst4SiteDefs::GetLLFromID(const WORD wSiteID)
{
	LatLong LL;
	LL.Invalidate();
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if ((nCountryIdx>=0)&&(nCountryIdx<MAX_COUNTRY_CNT)&&(nLocIdx>=0)&&(nTypeIdx>=0))
			LL=GetLL(nCountryIdx,nLocIdx,nTypeIdx);
		}
	return LL;
}


String Cst4SiteDefs::sGetNameOnlyFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
			{
			return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sName;
			}
		else
			{
			String s;
			return s.sprintf(L"Source #%04X",wSiteID);
			}
		}
	return "";
}

String Cst4SiteDefs::sGetAbbrFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
			{
			return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sSurfExt;
			}
		else
			{
			String s;
			return s.sprintf(L"Source #%04X",wSiteID);
			}
		}
	return "";
}

String Cst4SiteDefs::sGetDescrFileFromID(const WORD wSiteID)
{
	return sValidateFileName(sGetDescrFromID(wSiteID));
}


String Cst4SiteDefs::sGetLocationFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
			return CountryList[nCountryIdx]->LocList[nLocIdx]->sName;
		else
			{
			String s;
			return s.sprintf(L"Source #%04X",wSiteID);
			}
		}
	else
		return "";
}

String Cst4SiteDefs::sGetTypeFromID(const WORD wSiteID)
{
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if (nCountryIdx>=0)
			return CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->sName;
		else
			{
			String s;
			return s.sprintf(L"Source #%04X",wSiteID);
			}
		}
	else
		return "";
}



WORD Cst4SiteDefs::wGetIDFromSurfID(const int nSurfID)
{
	for (int nC=0; nC<CountryList.nGetCount(); nC++)
		{
		for (int nL=0; nL<CountryList[nC]->LocList.nGetCount(); nL++)
			{
			for (int nT=0; nT<CountryList[nC]->LocList[nL]->TypeList.nGetCount(); nT++)
				{
				if (CountryList[nC]->LocList[nL]->TypeList[nT]->nSurfID==nSurfID)
					return CountryList[nC]->LocList[nL]->TypeList[nT]->wID;
				}
			}
		}
	return 0;
}

WORD Cst4SiteDefs::wGetIDFromSurfExt(const String sSurfExt)
{
   String s=sSurfExt.UpperCase();
   for (int nC=0; nC<CountryList.nGetCount(); nC++)
      {
      for (int nL=0; nL<CountryList[nC]->LocList.nGetCount(); nL++)
         {
         for (int nT=0; nT<CountryList[nC]->LocList[nL]->TypeList.nGetCount(); nT++)
            {
            if (CountryList[nC]->LocList[nL]->TypeList[nT]->sSurfExt.UpperCase()==s)
               {
               return wGetID(nC,nL,nT);
               }
            }
         }
      }
   return 0;
}

String Cst4SiteDefs::sGetExtFromSurfID(const int nSurfID)
{
	for (int nC=0; nC<CountryList.nGetCount(); nC++)
		{
		for (int nL=0; nL<CountryList[nC]->LocList.nGetCount(); nL++)
			{
			for (int nT=0; nT<CountryList[nC]->LocList[nL]->TypeList.nGetCount(); nT++)
				{
				if (CountryList[nC]->LocList[nL]->TypeList[nT]->nSurfID==nSurfID)
					return CountryList[nC]->LocList[nL]->TypeList[nT]->sSurfExt;
				}
			}
		}
	return "";
}

Polar Cst4SiteDefs::GetRaBe(const WORD wSiteID, const LatLong& TargetLL)
{
	Polar Pol;
	Pol.Invalidate();
	int nCountryIdx,nLocIdx,nTypeIdx;
	if (wSiteID!=0xFFFF)
		{
		GetIndexFromID(wSiteID,nCountryIdx,nLocIdx,nTypeIdx);
		if ((nCountryIdx>=0)&&(nLocIdx>=0)&&(nTypeIdx>=0))
			{
			LatLong SiteLL=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->LL;
			if (SiteLL.bValid())
				{
				Transform Trans;
				Trans.SetRadarPos(SiteLL,CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[nTypeIdx]->nHeight_m);
				Trans.SetLLRef(SiteLL);
				Pol=Trans.LL2Pol(TargetLL);
				}
			}
		}
	return Pol;
}

void Cst4SiteDefs::GetTempIndexes(int& nCountryIdx, int& nLocIdx)
{
   nCountryIdx=-1;
   nLocIdx=-1;
	int nCountry=TEMP_COUNTRY;
   int nLoc=TEMP_LOC;
   nCountryIdx=awCountryIdx[nCountry];
 	if (nCountryIdx<CountryList.nGetCount())
  		nLocIdx=CountryList[nCountryIdx]->awLocIdx[nLoc];
}

String Cst4SiteDefs::sGetTempLocation()
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
	   return CountryList[nCountryIdx]->LocList[nLocIdx]->sName;
	return "";
}

void Cst4SiteDefs::SetTempLocation(const String sLoc)
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
		CountryList[nCountryIdx]->LocList[nLocIdx]->sName=sLoc;
}


void Cst4SiteDefs::GetTempAISPar(LatLong& LL, int& nHeight_m)
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
      {
      LL=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_AIS_IDX]->LL;
      nHeight_m=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_AIS_IDX]->nHeight_m;
      }
}

void Cst4SiteDefs::GetTempRadarPar(LatLong& LL, int& nHeight_m)
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
      {
      LL=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_RADAR_IDX]->LL;
      nHeight_m=CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_RADAR_IDX]->nHeight_m;
      }
}

void Cst4SiteDefs::SetTempAISPar(const LatLong LL, const int nHeight_m)
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
      {
      CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_AIS_IDX]->LL=LL;
      CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_AIS_IDX]->nHeight_m=nHeight_m;
      }
}

void Cst4SiteDefs::SetTempRadarPar(const LatLong LL, const int nHeight_m)
{
	int nCountryIdx,nLocIdx;
   GetTempIndexes(nCountryIdx,nLocIdx);
   if ((nCountryIdx>=0)&&(nLocIdx>=0))
      {
      CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_RADAR_IDX]->LL=LL;
      CountryList[nCountryIdx]->LocList[nLocIdx]->TypeList[TEMP_RADAR_IDX]->nHeight_m=nHeight_m;
      }
}

void Cst4SiteDefs::ReadTempFile(const String sFil,
											String& sLoc,
                                 LatLong& AISLL, int& nAISHeight_m,
                                 LatLong& RadarLL, int& nRadarHeight_m)
{
   	JIniFile Ini(sFil,JIniFile::INI_READ);
      int nVer;
      Ini.Read("GENERAL","Version",nVer);
      Ini.Read("Location",sLoc,"");
      Ini.Read("AIS","Position",AISLL);
      Ini.Read("Height",nAISHeight_m,0);
      Ini.Read("RADAR","Position",RadarLL);
      Ini.Read("Height",nRadarHeight_m,0);
}

void Cst4SiteDefs::SetTmpFromFile()
{
   String sFil=IncludeTrailingBackslash(sFolder)+"Default.TempSite";
   if (bFileExist(sFil))
      {
      String sLoc;
      LatLong AISLL,RadarLL;
      int nAISHeight_m,nRadarHeight_m;
      ReadTempFile(sFil,sLoc,AISLL,nAISHeight_m,RadarLL,nRadarHeight_m);
      SetTempLocation(sLoc);
      SetTempAISPar(AISLL,nAISHeight_m);
      SetTempRadarPar(RadarLL,nRadarHeight_m);
      }
}

String sGetCst4Site(const WORD wSiteID)
{
	if (g_pSiteDefs)
		return g_pSiteDefs->sGetDescrFromID(wSiteID);
	else
		{
		Cst4SiteDefs Defs;
		if (Defs.bLoadSites5())
			return Defs.sGetDescrFromID(wSiteID);
		else
		return "";
		}
}

String sGetCst4SiteNameOnly(const WORD wSiteID)
{
	if (g_pSiteDefs)
		return g_pSiteDefs->sGetNameOnlyFromID(wSiteID);
	else
		{
		Cst4SiteDefs Defs;
		if (Defs.bLoadSites5())
			return Defs.sGetNameOnlyFromID(wSiteID);
		else
		return "";
		}
}

String sGetCst4SiteName(const WORD wSiteID)
{
		if (g_pSiteDefs)
			return g_pSiteDefs->sGetDescrFileFromID(wSiteID);
		else
			{
			Cst4SiteDefs Defs;
			if (Defs.bLoadSites5())
				return Defs.sGetDescrFileFromID(wSiteID);
			return "";
			}
}

String sGet_SiteFile_Name()
{
   String sFolder="c:\\IMT Software\\VistaNet Shared\\";
   String sFile=sFolder+"VistaNet.VistaSite5";
	return sFile;
}

String sGetCst4FileName(const WORD wSiteID)
{
	String sFile=sGetCst4SiteName(wSiteID);
	return sValidateFileName(sFile);
}

void g_LoadSiteDefs5()
{
	if (!g_pSiteDefs)
		g_pSiteDefs=new Cst4SiteDefs;
	bool bOK=g_pSiteDefs->bLoadSites5();
	if (!bOK)
		{
		delete g_pSiteDefs;
		g_pSiteDefs=NULL;
		}
}

void g_CloseSiteDefs5()
{
	delete g_pSiteDefs;
	g_pSiteDefs=NULL;
}
