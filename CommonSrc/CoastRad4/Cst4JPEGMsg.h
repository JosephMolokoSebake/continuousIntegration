//---------------------------------------------------------------------------

#ifndef Cst4JPEGMsgH
#define Cst4JPEGMsgH
//---------------------------------------------------------------------------

#include "Cst4Messages.h"
#include "Cst4UserDefs.h"
#include "JMemFile.h"
#include "Jcrc.h"
#include "JFile.h"
#include "JCommaText.h"

class Cst4_JPEG_Msg
{

	JTime T_utc;
	WORD wSource;
	String sFileName;
	BYTE* pucData;
	int nDataSize;
	int nMaxDataSize;
	DWORD dwJPEG_ID;
	int nBlocksRXed;
	DWORD dwTXCRC;
	int nSysTrackNum;
   int nPrevBlockNum;

public:

	enum
	{
		MSG_NONE			= 101,
		MSG_CRC_ERROR	= 102,
		MSG_FIN			= 103,
		//Other numbers are percentage RX completed (0..100)

	};


	void Assign(const Cst4_JPEG_Msg& M)
	{
		T_utc=M.T_utc;
		wSource=M.wSource;
		sFileName=M.sFileName;
		nDataSize=M.nDataSize;
		delete[] pucData;
		if (nDataSize>0)
			{
			pucData=new BYTE[nDataSize];
			memcpy(pucData,M.pucData,nDataSize);
			}
		nMaxDataSize=M.nMaxDataSize;
		dwJPEG_ID=M.dwJPEG_ID;
		nBlocksRXed=M.nBlocksRXed;
		dwTXCRC=M.dwTXCRC;
		nSysTrackNum=M.nSysTrackNum;
      nPrevBlockNum=M.nPrevBlockNum;
	}

	Cst4_JPEG_Msg& operator = (const Cst4_JPEG_Msg& M)
	{
		Assign(M);
		return *this;
	}

	Cst4_JPEG_Msg()
	{
		pucData=NULL;
      nPrevBlockNum=-1;
	}


	Cst4_JPEG_Msg(const Cst4_JPEG_Msg& M)
	{
		pucData=NULL;
		Assign(M);
	}

	~Cst4_JPEG_Msg()
	{
		delete[] pucData;
	}

	DWORD dwGetJPEG_ID() const
	{
		return dwJPEG_ID;
	}

	bool bLoadJPEG(const String sFile)
	{
		sFileName=sFile;
		delete[] pucData;
		pucData=NULL;
		nDataSize=0;
		JMemFile F;
		JFile::FILE_ERROR E=F.ReadBuffer(sFileName);
		if (!E)
			{
			nDataSize=(int)F.dwGetSize();
			if (nDataSize>0)
				{
				pucData=new BYTE[nDataSize];
				memcpy(pucData,F.pucGetBuffer(),nDataSize);
				return true;
				}
			}
		return false;


	}

	bool bStoreJPEG(const String sFile="")
	{
		String sF;
		if (sFile.IsEmpty())
			sF=sFileName;
		else
			sF=sFile;
		if (nDataSize>0)
			{
			JFile F('O');
			JFile::FILE_ERROR E=F.Create(sF);
			if (!E)
				E=F.Write(pucData,nDataSize);
			return (E==JFile::F_NO_ERROR);
			}
		return false;
	}

	void SetFileName(const String sFile)
	{
		sFileName=sFile;
	}

	String sGetFileName() const
	{
		return sFileName;
	}

	void SetSysTrackNum(const int n)
	{
		nSysTrackNum=n;
	}

	int nGetSysTrackNum() const
	{
		return nSysTrackNum;
	}


	void InitTXMessages(const JTime& _T_utc, const WORD _wSource)
	{
		randomize();
		dwJPEG_ID=random(0x0FFFFFFF);
		T_utc=_T_utc;
		wSource=_wSource;
		RockCRC CRC;		//Default is CRC32
		for (int i=0; i<nDataSize; i++)
			CRC.Next(pucData[i]);
		dwTXCRC=CRC.ulGetCRC();
	}

	int nGetTXMsgCount() const
	{
		Cst4_Block_Data B;
		int nBlocks=nDataSize/B.nGetMaxBlockSize();
		if ((nDataSize%B.nGetMaxBlockSize())>0)
			++nBlocks;
return nBlocks;
	}

	Cst4_User_Msg CreateTXMsg(const int nB)
	{
		Cst4_User_Msg Msg;
		Cst4_Block_Data B;
      int nMax=B.nGetMaxBlockSize();
		int nBlocks=nDataSize/nMax;
		if ((nDataSize%nMax)>0)
			++nBlocks;
		int nOffs=nB*nMax;
		BYTE* pucStart=&pucData[nOffs];
		int nSize;;
		if (nB<nBlocks-1)
			nSize=nMax;
		else
			nSize=nDataSize%nMax;	//Last Block
		B.SetBlock(dwJPEG_ID,nB,nBlocks,pucStart,nSize);
		B.SetTXCRC(dwTXCRC);
		B.SetSysTrackNum(nSysTrackNum);
		B.SetFileName(sFileName);
		int nBlockSize=B.nGetSize();
		BYTE* pucBuf=new BYTE[nBlockSize];
		B.Pack(pucBuf);
		Msg.Set(T_utc,wSource,CST4_USER_PICTUREBLOCK,pucBuf,nBlockSize);
		delete[] pucBuf;

/*
      if (pTestFil)
         {
         String s;
         if (nB==0)
         	{
	         s.printf(L" ID=%08X  Total Blocks=%d  ",dwJPEG_ID,nBlocks);
   	      s=s+sFileName;
            }
         else
         	{
	         s.printf(L"ID=%08X  %d/%d  ",dwJPEG_ID,nB+1,nBlocks);

            }
         pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+s);
         }
*/


		return Msg;
	}

	int nAddRXMsg(Cst4_User_Msg& Msg)
	{
		if (Msg.dwGetType()==CST4_USER_PICTUREBLOCK)
			{
			int nSize=Msg.nGetPayloadSize();
			if (nSize>0)
				{
				Cst4_Block_Data Block;
				Block.UnPack((BYTE*)Msg.pGetPayload());
				int nTotal=Block.nGetTotalBlocks();
				int nBlockNum=Block.nGetBlockNum();
				if (nBlockNum==0)
					{
					//Start of new Picture
					dwJPEG_ID=Block.dwGetJPEG_ID();

//               if (pTestFil)
//               	{
//                  String s;
//                  s.printf(L"ID=%08X  Total Blocks=%d  ",dwJPEG_ID,nTotal);
//                  s=s+Block.sGetFileName();
//                  pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+s);
//                  }

					if (nTotal>0)
						{
						nMaxDataSize=nTotal*Block.nGetMaxBlockSize();
						delete[] pucData;
						pucData=new BYTE[nMaxDataSize];
						memcpy(pucData,Block.pucBlockData(),Block.nGetSize());
						nBlocksRXed=1;
						nDataSize=Block.nGetBlockSize();
						if (nTotal==1)
							{
							sFileName=Block.sGetFileName();
							RockCRC CRC;		//Default is CRC32
							for (int i=0; i<nDataSize; i++)
								CRC.Next(pucData[i]);
							DWORD dwRXCRC=CRC.ulGetCRC();
							dwTXCRC=Block.dwGetTXCRC();
                     nPrevBlockNum=-1;
							if (dwRXCRC!=dwTXCRC)
                     	{
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"ERROR: CRC Invalid");
								return MSG_CRC_ERROR;	//CRC Error
                        }
							else
                     	{
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"Message OK");
								return MSG_FIN;	//All blocks finished
                        }
							}
						}
					}
				else
					{

//               if (pTestFil)
//               	{
//                  String s;
//                  s.printf(L"%08X  %d/%d %d",dwJPEG_ID,nBlockNum+1,nTotal,Block.nGetSize());
//                  pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+s);
//                  }

					if (Block.dwGetJPEG_ID()==dwJPEG_ID)
						{
						int nOffs=nBlockNum*Block.nGetMaxBlockSize();
						memcpy(&pucData[nOffs],Block.pucBlockData(),Block.nGetSize());
						nDataSize+=Block.nGetBlockSize();
						++nBlocksRXed;

                  if (Block.nGetBlockNum()==0)
                  	{
                     if (nPrevBlockNum!=-1)
                     	{
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"ERROR: nPrev!=-1");
                        }
                     }
                  else
                  	{
							if ((Block.nGetBlockNum()!=nPrevBlockNum+1)&&(nPrevBlockNum!=-1))
								{
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"ERROR: nPrev!=nBlock+1");
      						}
                     }

                 	nPrevBlockNum=Block.nGetBlockNum();

						if ((nBlocksRXed==nTotal)&&(Block.nGetBlockNum()==nTotal-1))
							{
							sFileName=Block.sGetFileName();
							RockCRC CRC;		//Default is CRC32
							for (int i=0; i<nDataSize; i++)
								CRC.Next(pucData[i]);
							DWORD dwRXCRC=CRC.ulGetCRC();
							dwTXCRC=Block.dwGetTXCRC();
							if (dwRXCRC!=dwTXCRC)
                     	{
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"ERROR: CRC Invalid");
								return MSG_CRC_ERROR;	//CRC Error
                        }
							else
                     	{
                        nPrevBlockNum=-1;
//                        if (pTestFil)
//                           pTestFil->TextLine(JTime(true).sDateTime(false,true,false)+"  "+"Message OK");
								return MSG_FIN;	//All blocks finished
                        }
							}
						else
							{
                     return (nBlocksRXed*100)/nTotal;
							}

						}
					}
				}
			}
		return MSG_NONE;

	}

	String sCreateFileName(const int nSysTrack, const char cCameraType, const JTime& T_utc, const String sDescr)
   {
		String s,sV;
      s.printf(L"%04d_",nSysTrack);
      sV="T";
      sV[1]=cCameraType;
      s=s+sV+"_";
      sV=T_utc.sFileName("",false);
      if (sV[sV.Length()]=='.')
      	{
			sV[sV.Length()]='_';
         s=s+sV;
         }
      else
	      s=s+sV+"_";
		s=s+sDescr+".jpg";
		s=sValidateFileName(s,'_');
		return s;
   }

	int nExtractFileNameInfo(const String sFile,
   									char& cCameraType,
                              JTime& T_utc,
										String& sDescr)
   {
		int nTrackNum=-1;
      String s;
      CommaText Txt(sFile,true,'_');
      if (Txt.nGetCount()>=5)
      	{
         try
         	{
            nTrackNum=Txt.nGetVal(0);
            s=Txt.sGetPar(1).Trim();
            if (s.Length()>0)
            	cCameraType=s[1];
            int nY,nMon,nD,nH,nMin,nS;
            s=Txt.sGetPar(2);
            if (s.Length()>=8)
            	{
               nY=s.SubString(1,4).ToInt();
               nMon=s.SubString(5,2).ToInt();
               nD=s.SubString(7,2).ToInt();
               }
            s=Txt.sGetPar(3);
            if (s.Length()>=6)
            	{
               nH=s.SubString(1,2).ToInt();
               nMin=s.SubString(3,2).ToInt();
               nS=s.SubString(5,2).ToInt();
               }
            T_utc.SetDate(nD,nMon,nY,true);
            T_utc.SetTime(nH,nMin,nS,0);
            sDescr=Txt.sGetPar(4);
            sDescr=sFileNewExt(sDescr,"");
            }
         catch(...)
         	{
            nTrackNum=-1;
            }
         }
      return nTrackNum;
   }


};

#endif
