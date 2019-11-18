//---------------------------------------------------------------------------

#ifndef JByteFifoH
#define JByteFifoH

#include "JList.h"

#include <sysdyn.h>

//---------------------------------------------------------------------------

class JByteFifo // this is a thread-safe class
{

	struct FifoBlock
	{
		int nSize;
		int nWrOffs;
		int nRdOffs;

		BYTE* pucData;

		FifoBlock(const int _nSize)
		{
			nSize=_nSize;
			nWrOffs=0;
			nRdOffs=0;
			pucData=new BYTE[nSize];;
		}

		~FifoBlock()
		{
			delete[] pucData;
		}

		bool bAddByte(const BYTE uc)
		{
			pucData[nWrOffs++]=uc;
			return (nWrOffs==nSize);
		}

		bool bAddBytes(const BYTE* puc, const int nBytes)
		{
			memcpy(&pucData[nWrOffs],puc,nBytes);
			nWrOffs+=nBytes;
			return (nWrOffs==nSize);
		}

		bool bRemoveByte(BYTE& uc)
		{
			uc=pucData[nRdOffs++];
			return (nRdOffs==nSize);
		}

		bool bRemoveBytes(BYTE* puc, const int nBytes)
		{
			memcpy(puc,&pucData[nRdOffs],nBytes);
			nRdOffs+=nBytes;
			return (nRdOffs==nSize);
		}

		int nWrSpaceLeft() const
		{
			return nSize-nWrOffs;
		}

		int nBytesAvail() const
		{
			return nWrOffs-nRdOffs;
		}

	};

	JList<FifoBlock> BlockList;

	int nFifoSize;
	int nBlockSize;

	TCriticalSection* pDataLock;

	void PushByte(const BYTE uc)
	{
		if (BlockList.pLast()->bAddByte(uc))
			BlockList.nAdd(new FifoBlock(nBlockSize));
		++nFifoSize;
	}

	void PushBytes(const BYTE* puc, const int nBytes)
	{
		int nLeft=BlockList.pLast()->nWrSpaceLeft();
		if (nBytes<=nLeft)
			{
			if (BlockList.pLast()->bAddBytes(puc,nBytes))
				BlockList.nAdd(new FifoBlock(nBlockSize));
			}
		else
			{
			BlockList.pLast()->bAddBytes(puc,nLeft);
			int nOffs=nLeft;
			int nTotal=nBytes-nLeft;
			BlockList.nAdd(new FifoBlock(nBlockSize));
			int nBlocks=nTotal/nBlockSize;
			for (int i=0; i<nBlocks; i++)
				{
				BlockList.pLast()->bAddBytes(&puc[nOffs],nBlockSize);
				nOffs+=nBlockSize;
				BlockList.nAdd(new FifoBlock(nBlockSize));
				}
			nLeft=nTotal%nBlockSize;
			if (nLeft>0)
				{
				if (BlockList.pLast()->bAddBytes(&puc[nOffs],nLeft))
					BlockList.nAdd(new FifoBlock(nBlockSize));
				}
			}
		nFifoSize+=nBytes;
	}

	BYTE ucPopByte()
	{
		BYTE uc;
		if (BlockList.pFirst()->bRemoveByte(uc))
			BlockList.PopFirst();
		--nFifoSize;
		if (BlockList.nGetCount()==0)
			BlockList.nAdd(new FifoBlock(nBlockSize));
		return uc;
	}

	int nPopBytes(BYTE* puc, const int nMaxBytes)
	{
		int nBytes=nMaxBytes;
		if (nBytes>nFifoSize)
			nBytes=nFifoSize;
		int nAvail=BlockList.pFirst()->nBytesAvail();
		if (nBytes<=nAvail)
			{
			if (BlockList.pFirst()->bRemoveBytes(puc,nBytes))
				BlockList.PopFirst();
			}
		else
			{
			BlockList.pFirst()->bRemoveBytes(puc,nAvail);
			BlockList.PopFirst();
			int nTotal=nBytes-nAvail;
			int nOffs=nAvail;
			int nBlocks=nTotal/nBlockSize;
			for (int i=0; i<nBlocks; i++)
				{
				BlockList.pFirst()->bRemoveBytes(&puc[nOffs],nBlockSize);
				BlockList.PopFirst();
				nOffs+=nBlockSize;
				}
			nAvail=nTotal%nBlockSize;
			if (nAvail>0)
				{
				if (BlockList.pFirst()->bRemoveBytes(&puc[nOffs],nAvail))
					BlockList.PopFirst();
				}
			}
		nFifoSize-=nBytes;
		if (BlockList.nGetCount()==0)
			BlockList.nAdd(new FifoBlock(nBlockSize));
		return nBytes;
	}



public:

	int nGetSize()
	{
		if (pDataLock)
			{
			pDataLock->Acquire();
			int nLen=nFifoSize;
			pDataLock->Release();
			return nLen;
			}
		else
			return nFifoSize;
	}

	int nPush(void* pData,const int nBytes) // returns FIFO size
	{
		if (pDataLock)
			{
			pDataLock->Acquire();
			BYTE* puc=(BYTE*)pData;
			PushBytes(puc,nBytes);
			int nLen=nFifoSize;
			pDataLock->Release();
			return nLen;
			}
		else
			{
			BYTE* puc=(BYTE*)pData;
			PushBytes(puc,nBytes);
			return nFifoSize;
			}
	}

	int nPush(TBytes AData) // receives data in Indy array format
	{
		if (pDataLock)
			{
			pDataLock->Acquire();
			int nLen;
			if (AData.Length>0)
				{
				BYTE* puc=new BYTE[AData.Length];
				for (int i=0;i<AData.Length;i++)
					puc[i]=AData[i];
				nLen=nPush(puc,AData.Length);
				delete[] puc;
				}
			else
				nLen=nFifoSize;
			pDataLock->Release();
			return nLen;
			}
		else
			{
			if (AData.Length>0)
				{
				BYTE* puc=new BYTE[AData.Length];
				for (int i=0;i<AData.Length;i++)
					puc[i]=AData[i];
				int nLen=nPush(puc,AData.Length);
				delete[] puc;
				return nLen;
				}
			else
				return nFifoSize;
			}
	}

	int nPop(void* pByte) // return FIFO size or -1 if no Bytes read
	{
		if (pDataLock)
			{
			int nLen;
			pDataLock->Acquire();
			if (nFifoSize>0)
				{
				BYTE uc=ucPopByte();
				*(BYTE*)pByte=uc;
				nLen=nFifoSize;
				}
			else
				nLen=-1;
			pDataLock->Release();
			return nLen;
			}
		else
			{
			if (nFifoSize>0)
				{
				BYTE uc=ucPopByte();
				*(BYTE*)pByte=uc;
				return nFifoSize;
				}
			else
				return -1;
			}
	}

	int nPop(void* pBytes, const int nBytes)  //Returns bytes actually read
	{
		if (pDataLock)
			{
			pDataLock->Acquire();
			BYTE* puc=(BYTE*)pBytes;
			int nBytesRead=nPopBytes(puc,nBytes);
			pDataLock->Release();
			return nBytesRead;
			}
		else
			{
			BYTE* puc=(BYTE*)pBytes;
			return nPopBytes(puc,nBytes);
			}
	}


	void Clear()
	{
		if (pDataLock)
			{
			pDataLock->Acquire();
			BlockList.Clear();
			BlockList.nAdd(new FifoBlock(nBlockSize));
			nFifoSize=0;
			pDataLock->Release();
			}
		else
			{
			BlockList.Clear();
			BlockList.nAdd(new FifoBlock(nBlockSize));
			nFifoSize=0;
			}
	}

	JByteFifo(const int _nBlockSize=1024, const bool bThreadSafe=true)
	{
		if (bThreadSafe)
			pDataLock=new TCriticalSection;
		else
			pDataLock=NULL;
		nBlockSize=_nBlockSize;
		Clear();
	};

	~JByteFifo()
	{
		delete pDataLock;
		pDataLock=NULL;
	};
};


#endif
