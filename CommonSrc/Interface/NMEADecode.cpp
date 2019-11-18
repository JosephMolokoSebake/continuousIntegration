//---------------------------------------------------------------------------
#include "jpch.h"
#pragma hdrstop

#include "NMEADecode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


NMEADecode::NMEADecode()
{
//   pucTest=NULL;
   Reset(true,false);
}

void NMEADecode::AddByte(const BYTE ucData)
{
//   pucTest[nTestCnt++]=ucData;
   if (bStartChar(ucData))
		nRXIndex=0;
	if (nRXIndex>=0)
		{
		szRXBuffer[nRXIndex]=(char)ucData;
      bool bFin;
      if (bCRLF)
         bFin=(nRXIndex>0)&&(ucData=='\n')&&(szRXBuffer[nRXIndex-1]=='\r');
      else
         bFin=(nRXIndex>0)&&(ucData=='\r');
      if (bForceFin)
         {
         switch(PrevChar)
            {
            case OTHER_CHAR:
               if (ucData=='*')
                  PrevChar=STAR_CHAR;
               else
                  PrevChar=OTHER_CHAR;
               break;
            case STAR_CHAR:
               if (bCheckSumChar(ucData))
                  PrevChar=CHECK1_CHAR;
               else
                  PrevChar=OTHER_CHAR;
               break;
            case CHECK1_CHAR:
               if (ucData=='*')
                  PrevChar=STAR_CHAR;
               else
						{
						if (bCheckSumChar(ucData))
							{
							++nRXIndex;
							bFin=true;
							}
						PrevChar=OTHER_CHAR;
						}
					break;
				default: ;
				}
			}
		if (bFin)
			{
			//Sentence completed
			szRXBuffer[nRXIndex]=0;
			nRXIndex=0;
			asRX[nRXNum]=AnsiString(szRXBuffer).Trim();
			PrevChar=OTHER_CHAR;
			nRXNum++;
			if (nRXNum>=MAX_SENTENCES)
				nRXNum=0;
			}
		if (nRXIndex<COMM_BUFFER_SIZE-1)
			++nRXIndex;
		else
			Reset(bCRLF,bForceFin); 	//Buffer overflow Error - reset all
		}
}

String NMEADecode::sGetRawSentence()
{
	return sLastString;
}


String NMEADecode::sGetSentence(const bool bUseChk)
{
	AnsiString s="";
	if (nRXNum!=nReadNum)
		{
		s=asRX[nReadNum];
		nReadNum++;
		if (nReadNum>=MAX_SENTENCES)
			nReadNum=0;
		sLastString=s;
		}
	int nLen=s.Length();
	if (nLen>3)
		{
		if (bUseChk)
			{
			if (s.LastDelimiter("*")>0)
				{
				//Verify checksum
            int i=1;
            BYTE uc=0x00;
            while((s[i]!='*')&&(i<=nLen))
               {
               if (!bStartChar(s[i])) uc^=s[i];
               ++i;
               }
            bool bOK;
            if ((s[i]=='*')&&(i<nLen-1))
               bOK=((s[i+1]==H2C(uc>>4))&&(s[i+2]==H2C(uc&0x0F)));
            else
               bOK=false;
            if (bOK)
               s=s.SubString(2,i-2);   //Strip $,* and checksum off
            else
               {
               s=L"";
               nCheckErrors++;
               }
            }
         }
      else
         {
         s=s.SubString(2,s.Length()-1);   //Strip $ off
         }
		}
   else
		s="";
   return String(s);
}

String NMEADecode::sConstructSentence(const String sData, const String sPrefix)
{
	AnsiString s1=AnsiString(sData);
	BYTE uc=0x00;
	for (int i=1; i<=sData.Length(); i++)
		uc^=s1[i];
	char cMSN=uc>>4;
	if (cMSN<10) cMSN+='0'; else cMSN+='A'-10;
	char cLSN=uc&0x0f;
	if (cLSN<10) cLSN+='0'; else cLSN+='A'-10;
	AnsiString sChk;
	sChk.printf("*%c%c\r\n",cMSN,cLSN);
	return String(sPrefix+sData+sChk);
}

String NMEADecode::sGetHeader(const bool bUseChk)
{
   String s=sGetSentence(bUseChk);
   if (!s.IsEmpty())
      {
      SetText(s);
      if (nGetNum()>0)
         s=sGetPar(0);
      else
			s="";
      }
   return s;
}

int NMEADecode::nGetParCnt()
{
   return nGetNum();
}


