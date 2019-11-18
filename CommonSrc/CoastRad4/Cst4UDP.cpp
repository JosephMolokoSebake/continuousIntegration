//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "Cst4UDP.h"
#include "JCommaText.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Cst4UDP::Cst4UDP(TComponent* pOwner, const WORD _wOwnID) : LJUdpSocket(pOwner)
{
	if (_wOwnID!=0xFFFF)
		wOwnID=_wOwnID;
	else
		wOwnID=0;
	pucRXBuf=NULL;
   nTotalRXMessages=0;
}

Cst4UDP::~Cst4UDP()
{
	delete[] pucRXBuf;
}

void Cst4UDP::InitTX(const int nPort, const String sAddr, const WORD _wOwnID)
{
	if (_wOwnID!=0xFFFF)
		wOwnID=_wOwnID;
	LJUdpSocket::Reset(sAddr,nPort);
	bBroadcast=(sAddr.IsEmpty());
}

int Cst4UDP::nTXMessages()
{
   int nTXSize=Cst4Manager::nGetTXSize();
   if (nTXSize>0)
   	{
		BYTE* pucTXBuf=new BYTE[nTXSize];
		Cst4Manager::PackTX(pucTXBuf);

		if (nTXSize>10000)
			{
			int nBlockCnt=nTXSize/10000;
			if ((nTXSize%10000)>0)
				++nBlockCnt;
			int nOffs=0;
			int nBlockSize=10000;
			for (int i=0; i<nBlockCnt; i++)
				{
				if (bBroadcast)
					LJUdpSocket::BroadcastData(&pucTXBuf[nOffs],nBlockSize);
				else
					LJUdpSocket::SendData(&pucTXBuf[nOffs],nBlockSize);
				nOffs+=nBlockSize;
				if (nOffs+nBlockSize>nTXSize)
					nBlockSize=nTXSize-nOffs;
				}
			}
		else
			{
			if (bBroadcast)
				LJUdpSocket::BroadcastData(pucTXBuf,nTXSize);
			else
				LJUdpSocket::SendData(pucTXBuf,nTXSize);
			}
		delete[] pucTXBuf;
		}
	Cst4Manager::ClearMsg();
	return nTXSize;
}

void Cst4UDP::InitRX(const int nPort, const int nMaxBufSize)
{
	nMaxRXBuf=nMaxBufSize;
	delete[] pucRXBuf;
	pucRXBuf=new BYTE[nMaxRXBuf];
	Cst4Manager::Init(true);
	LJUdpSocket::Listen(nPort);
}

int Cst4UDP::nUpdateRX()
{
	int nMsgCnt=0;
	int nRXCnt=LJUdpSocket::nReadData(pucRXBuf,nMaxRXBuf);
	for (int i=0; i<nRXCnt; i++)
		{
		CST4_MSG_TYPE MsgType=Cst4Manager::AddRXByte(pucRXBuf[i]);
		if (MsgType!=CST4_NO_MSG)
			++nMsgCnt;
		}
	nTotalRXMessages+=nMsgCnt;
	return nMsgCnt;
}

void Cst4UDP::SendRXWatchdogMsg()
{
	Cst4_Request Msg(JTime(true,true),wOwnID,CST4_REQUEST_RX_WATCHDOG);
	BYTE aucBuf[1024];
	int nTXSize=Msg.nCreateTXBuf(aucBuf);
	LJUdpSocket::SendData(aucBuf,nTXSize);
}

void Cst4UDP::SendTXWatchdogMsg()
{
	Cst4_Request Msg(JTime(true,true),wOwnID,CST4_REQUEST_TX_WATCHDOG);
	BYTE aucBuf[1024];
	int nTXSize=Msg.nCreateTXBuf(aucBuf);
	LJUdpSocket::SendData(aucBuf,nTXSize);
}

void Cst4UDP::SendNotifyMsg()
{
	Cst4_Request Msg(JTime(true,true),wOwnID,CST4_REQUEST_NOTIFY);
	BYTE aucBuf[1024];
	int nTXSize=Msg.nCreateTXBuf(aucBuf);
	LJUdpSocket::SendData(aucBuf,nTXSize);
}

void Cst4UDP::SendRebootMsg()
{
	Cst4_Request Msg(JTime(true,true),wOwnID,CST4_REQUEST_REBOOT);
	BYTE aucBuf[1024];
	int nTXSize=Msg.nCreateTXBuf(aucBuf);
	LJUdpSocket::SendData(aucBuf,nTXSize);
}

DWORD Cst4UDP::dwConvertAddr(const String sAddr)
{
	CommaText Txt(sAddr,true,'.');
	if (Txt.nGetCount()==4)
		{
		DWORD adw[4];
		try
			{
			adw[3]=Txt.nGetVal(0);
			adw[2]=Txt.nGetVal(1);
			adw[1]=Txt.nGetVal(2);
			adw[0]=Txt.nGetVal(3);
			return (adw[3]<<24)|(adw[2]<<16)|(adw[1]<<8)|adw[0];
			}
		catch(...)
			{
			return 0;
			}
		}
	return 0;	//Invalid
}

String Cst4UDP::sConvertAddr(const DWORD dwAddr)
{
	String s;
	s.printf(L"%d.%d.%d.%d",(dwAddr>>24)&0xFF,(dwAddr>>16)&0xFF,(dwAddr>>8)&0xFF,dwAddr&0xFF);
	return s;
}

DWORD Cst4UDP::dwGetSourceAddr()
{
	String sSourceAddr;
	GetLastSender(sSourceAddr);
   return dwConvertAddr(sSourceAddr);
}

