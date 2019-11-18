//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "USB_FT_DevChange.h"
#include "USB_FT245.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

USB_FT_DevChange::USB_FT_DevChange(HANDLE hRecipient)
{
	Action=USB_OTHER;
	nIMTDevice=-1;
	nIMTSerial=-1;
	DEV_BROADCAST_DEVICEINTERFACE dbch;
	::ZeroMemory(&dbch,sizeof(dbch));
	dbch.dbcc_size=sizeof(dbch);
	dbch.dbcc_devicetype=DBT_DEVTYP_DEVICEINTERFACE;
	NotificationHandle=::RegisterDeviceNotification(hRecipient,&dbch
									,DEVICE_NOTIFY_WINDOW_HANDLE|DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
}

USB_FT_DevChange::~USB_FT_DevChange()
{
	if (NotificationHandle)
		::UnregisterDeviceNotification(NotificationHandle);
	NotificationHandle=NULL;
}

bool USB_FT_DevChange::bProcessMessage(const TMessage& Msg)
{
	Action=USB_OTHER;
	nIMTDevice=-1;
	nIMTSerial=-1;
	switch(Msg.WParam)
		{
		case DBT_DEVICEARRIVAL:
			Action=USB_INSERTED;
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			Action=USB_REMOVED;
			break;
		default:	;
		}
	if (Action!=USB_OTHER)
		{
		PDEV_BROADCAST_DEVICEINTERFACE pdbch=(PDEV_BROADCAST_DEVICEINTERFACE)Msg.LParam;
		if (!pdbch)
			{
			Action=USB_OTHER;
			return false;
			}
		String s=pdbch->dbcc_name;
		int n=s.Pos("VID_");
		if (n>0)
			{
			String sVID=s.SubString(n+4,4);
			if (sVID=="0403")	//FTDI
				{
				n+=18;
				String sDev=s.SubString(n,4);
				if (!sDev.IsEmpty())
					{
					sscanf(AnsiString("0x"+sDev).c_str(),"%x",&nIMTDevice);
					if (nIMTDevice>0)
						{
						n+=4;
						String sSer=s.SubString(n,4);
						if (!sSer.IsEmpty())
							{
							sscanf(AnsiString("0x"+sSer).c_str(),"%x",&nIMTSerial);
							return (nIMTSerial>=0);
							}
						}
					}
				}
			}
		}
	return false;
}

String USB_FT_DevChange::sIMTDevice()
{
	IMT_USB_Device Dev;
	return Dev.sGetDescr(nIMTDevice,0);
}

