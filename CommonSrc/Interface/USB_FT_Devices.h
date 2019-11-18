#ifndef USB_FT_DEVICES
#define USB_FT_DEVICES

enum
{
	USB_ANY_DEV          = 0x0000,
	USB_SERIAL2_DEV      = 0x0001,
	USB_WATCHDOG_DEV     = 0x0002,
	USB_RESIG_DEV        = 0x0003,
	USB_PROPMON_DEV      = 0x0004,
	USB_SERIAL3_DEV      = 0x0005,
	USB_RHUBARB_DEV      = 0x0006,
	USB_PRINTER_DEV      = 0x0007,
	USB_DOSTAL_DEV       = 0x0008,
	USB_AGGR_DEV         = 0x0009,
	USB_TIME_DEV         = 0x000A,
	USB_SERIAL31_DEV     = 0x000B,  	//with serial# <=520
	USB_SERIAL40_DEV     = 0x000B,   //with serial# >520
	USB_PROPMON3_DEV     = 0x000C,
	USB_SERIAL3_4CH_DEV  = 0x000D,
	USB_RADARSIM_DEV     = 0x000E,
	USB_USB_SD_DEV       = 0x000F,
	USB_USBSMP2_DEV      = 0x0010,   //FT2232H
	USB_GUARDDOG_32_DEV  = 0x0011,
	USB_GUARDDOG_33_DEV  = 0x0012,
	USB_USBSMP3_DEV      = 0x0013,   //FT2232H
	USB_NA10DCS_DEV     	= 0x0014,   //FT2232H
	USB_COMMSINT_DEV    	= 0x0015,   //FT2232H
	USB_SERIAL_HDLC_DEV  = 0x0016,
	USB_NA10_ANT_DEV		= 0x0017,
	USB_MULTICOMMS_DEV  	= 0x0018,   //FT2232H
	USB_DETECT_DEV       = 0xFFFF,
};

class IMT_USB_Device
{


public:

	IMT_USB_Device()
	{
	}

	~IMT_USB_Device()
	{
	}


	String sGetDescr(const int nDevID, const int nSerial)
	{
		switch(nDevID)
			{
			case USB_SERIAL2_DEV    	: 	return "USB Serial Interface";
			case USB_WATCHDOG_DEV   	: 	return "USB Power Watchdog";
			case USB_RESIG_DEV      	: 	return "RESIG USB Interface";
			case USB_PROPMON_DEV    	: 	return "Propulsion Monitor Interface";
			case USB_SERIAL3_DEV    	: 	return "Enhanced USB Serial Interface";
			case USB_RHUBARB_DEV    	: 	return "RHUBARB USB Interface";
			case USB_PRINTER_DEV    	: 	return "USB Printer Emulator";
			case USB_DOSTAL_DEV     	: 	return "USB DOSTAL Target Extractor";
			case USB_AGGR_DEV       	: 	return "Aggregate Interface";
			case USB_TIME_DEV       	: 	return "USB Time Distribution Interface";
			case USB_SERIAL31_DEV   	: 	if (nSerial<0)
														return "Enhanced USB Serial";
													if (nSerial<4000)
														return "Enhanced USB Serial (ver 3.1)";
													if (nSerial>=4000)
														return "Enhanced USB Serial (ver 4.0)";
			case USB_PROPMON3_DEV   	: 	return "Propulsion Monitor Interface";
			case USB_SERIAL3_4CH_DEV	: 	return "USB Serial 4 Channel Interface";
			case USB_RADARSIM_DEV   	: 	return "Radar Simulator";
			case USB_USB_SD_DEV     	: 	return "Synchro Digital Converter";
			case USB_USBSMP2_DEV    	: 	return "Scan Converter";
			case USB_GUARDDOG_32_DEV   : 	return "CoastRad IV GuardDog";
			case USB_GUARDDOG_33_DEV   : 	return "CoastRad IV GuardDog (ver 3.3)";
			case USB_USBSMP3_DEV			: 	return "Scan Converter (ver 3)";
			case USB_NA10DCS_DEV     	: 	return "NA10 Data Capture System";
			case USB_COMMSINT_DEV     	: 	return "USB Communications Interface";
			case USB_SERIAL_HDLC_DEV  	: 	return "USB Serial HDLC Receiver";
			case USB_NA10_ANT_DEV     	: 	return "NA10 Antenna Limit Controller";
			case USB_MULTICOMMS_DEV   	: 	return "Multiple Communications Interface";
			default                 	: 	return "Unknown Device";
			}
	}

	bool bUSBSerial(const int nDevID) const
	{
		switch(nDevID)
			{
			case USB_SERIAL2_DEV:
			case USB_SERIAL31_DEV:
				return true;
			default:	return false;
			}
	}


};


#endif