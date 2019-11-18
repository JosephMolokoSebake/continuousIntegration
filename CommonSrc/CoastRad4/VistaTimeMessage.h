//---------------------------------------------------------------------------

#ifndef VistaTimeMessageH
#define VistaTimeMessageH
//---------------------------------------------------------------------------
#include "Cst4Defs.h"
#include "Cst4Messages.h"

class VistaTimeMessage : public Cst4_MessageBase
{

public:

	void Set(const JTime& _T, const WORD _wSourceID)
	{
		SetTime(_T,true);
		SetSource(_wSourceID);
		SetValid();
	}

	VistaTimeMessage(const JTime& _T, const WORD _wSourceID) : Cst4_MessageBase(VISTA_TIME_MSG)
	{
		Set(_T,_wSourceID);
	}

	VistaTimeMessage() : Cst4_MessageBase(VISTA_TIME_MSG)
	{
		JTime T;
		T.Invalidate();
		SetTime(T,true);
	}

	VistaTimeMessage(const BYTE* pucBuf, const int nMaxSize) : Cst4_MessageBase(VISTA_TIME_MSG)
	{
		int nOffs=0;
		bUnpack(pucBuf,nOffs,nMaxSize,false);
	}


	~VistaTimeMessage()
	{
	}

	void Assign(const VistaTimeMessage& M)
	{
		Cst4_MessageBase::Assign(M);
	}

	VistaTimeMessage& operator = (const VistaTimeMessage& M)
	{
		Assign(M);
		return *this;
	}

	VistaTimeMessage(const VistaTimeMessage& M)
	{
		Assign(M);
	}

	DWORD dwCalcSize() const
	{
		return dwGetBaseSize();
	}

	void Pack(BYTE* pucBuf, int& nOffs)
	{
		SetSize(dwCalcSize());
		PackBase(pucBuf,nOffs);
	};

	bool bUnpack(const BYTE* pucBuf, int& nOffs, const int nMaxSize, const bool bSkipStatus=false)
	{
		if (!bUnpackBase(pucBuf,nOffs,nMaxSize,bSkipStatus))
			return false;
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

};


#endif
