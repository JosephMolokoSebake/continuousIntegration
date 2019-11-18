//---------------------------------------------------------------------------
#ifndef PCI32_WDM_IntH
#define PCI32_WDM_IntH
//---------------------------------------------------------------------------

/**	CONFIGURATION HEADER **********************************************
 *  	MODULE TITLE	High Level Driver for PCI32_WDM_Int card
 *    FILE NAME		PCI32_WDM_Int
 *		PROJECT			C0953
 *		ACTIVITY       4
 * 	COMPILER       Borland C++ Builder 6
 *		PROCESSOR      Pentium III
 *		OS					MS Windows 2000 SP3
 *		PROGRAMMER     Johan Theron
 *		ABSTRACT
 *
 *		CHANGE HISTORY
 *			V1.00		Initial Version	JGT 22/01/2001
*************************************************************************/

#include "devintf.h"

class PCI32_WDM_Int
{

   HANDLE hDevice;
   bool bVerbose;


// PCI9030 GPIO Control Bits:
// GPIO#   Enable    I/O     Data  Use
//  0      b0=0(E)  b1=1(O)   b2   DCLK
//  1      b3=0(E)  b4=2(O)   b5   DATA0
//  2      b6=0(E)  b7=0(I)   b8   CFGDONE
//  3      b9=0(E)  b10=0(I)  b11  STATUS
//  4      b12=1(D) -
//  5      b15=1(D) -
//  6      b18=1(D) -
//  7      b21=1(D) -
//  8      always   b25=1(O)  b26  nCONFIG


   enum
   {
      PLX_9030_LOC_GPIOC   = 0x00000054,
      PLX_9030_GPIO_VAL    = 0x066DB012,
      PLX_9030_CFGCLK_0    = 0xFFFFFFFB,     //Set GPIO0 = 0
      PLX_9030_CFGCLK_1	   = 0x00000004,     //Set GPIP0 = 1
      PLX_9030_CFGDATA_0	= 0xFFFFFFDF,     //Set GPIO1 = 0
      PLX_9030_CFGDATA_1	= 0x00000020,     //Set GPIO1 = 1

      PLX_9030_CFGDONE_A_1	= 0x00000100,     // Alternative bit (used on HDLC_PCI & MSDS9030)
//         PLX_9030_STATUS_1    = 0x00000800,     //Check GPIO3 = 1

      PLX_9030_CFGDONE_1	= 0x00000800,     //Check GPIO2 = 1
//         PLX_9030_STATUS_1    = 0x00000100,     //Check GPIO3 = 1

      PLX_9030_NCONFIG_0	= 0xFBFFFFFF,     //Set GPIO8 = 0
      PLX_9030_NCONFIG_1	= 0x04000000,     //Set GPIO8 = 1
   };

   enum
   {
      PLX_9050_LOC_USER	   = 0x00000050,
      PLX_9050_USER_VAL    = 0x0878ED36,
      PLX_9050_CFGCLK_0	   = 0xFFFFFFFB,
      PLX_9050_CFGCLK_1	   = 0x00000004,
      PLX_9050_CFGDATA_0	= 0xFFFFFFDF,
      PLX_9050_CFGDATA_1	= 0x00000020,
      PLX_9050_CFGDONE_0	= 0xFFFFFEFF,
      PLX_9050_CFGDONE_1	= 0x00000100,
      PLX_9050_NCONFIG_0	= 0xFFFFF7FF,
      PLX_9050_NCONFIG_1	= 0x00000800,
   };

   enum
   {
      PLX_9054_LOC_USER	   = 0x0000006C,
      PLX_9054_CONFIG_ADDR = 0x00000000,
      PLX_9054_EN_APEX     = 0x00000004,
      PLX_9054_USER_VAL    = 0x00000000,
      PLX_9054_CFGCLK_0	   = 0xFFFFFFFE,
      PLX_9054_CFGCLK_1	   = 0x00000001,
      PLX_9054_CFGDATA_0	= 0xFFFFFFFD,
      PLX_9054_CFGDATA_1	= 0x00000002,
      PLX_9054_CFGDONE_0	= 0xFFFDFFFF,     //bit 17 low
      PLX_9054_CFGDONE_1	= 0x00020000,     //bit 17 high
      PLX_9054_NCONFIG_0	= 0xFFFEFFFF,     //bit 16 low
      PLX_9054_NCONFIG_1	= 0x00010000,     //bit 16 high
   };


   DWORD dwCfgOffs,dwCfgVal;
   DWORD dwCLK0,dwCLK1;
   DWORD dwDATA0,dwDATA1;
   DWORD dwNCONFIG0,dwNCONFIG1;
   DWORD dwDONE1;

protected:

   enum
   {
      CFG_BUFFER_SIZE   = 512
   };

   DWORD dwCfgBufferIndex;
   DWORD* pdwCfgBuffer;

private:

   String sName;
   String sError;

   void Set9054nCONFIG(const bool bnCFG);
   void SetBridge();
   void FlushCfgBuffer();
   void CfgOut(const bool bNCONFIG, const bool bDCLK, const bool bDATA);
   bool bUnloadDriver();
   bool bConfigDone();

public:

   void WriteCfg(const DWORD dwOffs, DWORD dwData);
   void WriteCfg(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum);
   DWORD dwReadCfg(const DWORD dwOffs);
   bool bCheckDriverVersion(const DWORD dwDriverVersion);

   HANDLE OpenByInterface( GUID* pClassGuid,	// points to the GUID that identifies the interface class
                           DWORD instance,	// specifies which instance of the enumerated devices to open
                           PDWORD pError);   // address of variable to receive error status

   bool bDeviceOK;

   bool bConfigFlex(const String sFile);
   void Write(const DWORD dwOffs, DWORD dwData);
   void Write(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum);
   DWORD dwRead(const DWORD dwOffs);
   void Read(const DWORD dwOffs, DWORD* pdwData, const DWORD dwNum);
   void ErrorMessage(const String sMess, const bool bFatal=false);

   enum BRIDGE_DEVICE
   {
      PCI_9030,
      PCI_9050,
      PCI_9054,
   };

private:

   BRIDGE_DEVICE Bridge;

public:

   PCI32_WDM_Int( const BRIDGE_DEVICE _Bridge,
                  const String _sName,
                  const DWORD dwDriverVersion,
                  GUID ClassGuid,
                  const bool _bVerbose=true
                  );

   ~PCI32_WDM_Int();

   void AltCfgDone()
   {
      dwDONE1=PLX_9030_CFGDONE_A_1;
   }

   bool bOK() const
   {
      return bDeviceOK;
   }

};

#endif
