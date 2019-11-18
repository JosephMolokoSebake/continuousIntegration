//---------------------------------------------------------------------------

#ifndef USB_FT_DevChangeH
#define USB_FT_DevChangeH

#include <dbt.h>

//---------------------------------------------------------------------------

/*
Usage:

1. Declare USB_FT_DevChange in form header:
		USB_FT_DevChange* pDevChange;

2. Instantiate USB_FT_DevChange in form OnCreate:
		pDevChange=new USB_FT_DevChange(Handle);

3. Delete in OnDestroy:
		delete pDevChange;

4. Declare message handler and message map in form header:

		MESSAGE BOOL OnDeviceChange(TMessage& Message);

		BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_DEVICECHANGE, TMessage, OnDeviceChange);
		END_MESSAGE_MAP(TForm);

5. Create Message Handler method in main form

		MESSAGE BOOL TForm1::OnDeviceChange(TMessage& Message)
		{
			if (pDevChange->bProcessMessage(Message))
				{
				switch(pDevChange->GetAction())
					{
					case USB_FT_DevChange::USB_INSERTED:
						//Use pDevChange to get device ID & serial number
						//Do something
						break;
					case USB_FT_DevChange::USB_REMOVED:
						//Use pDevChange to get device ID & serial number
						//Do something
						break;
					default:	;
					}
				}
			return TRUE;
		}

*/




class USB_FT_DevChange
{

public:

	enum USB_FT_ACTION
	{
		USB_OTHER,
		USB_INSERTED,
		USB_REMOVED,
	};

private:

	HDEVNOTIFY NotificationHandle;
	USB_FT_ACTION Action;
	int nIMTDevice;
	int nIMTSerial;


public:


	USB_FT_DevChange(HANDLE hRecipient);
	~USB_FT_DevChange();

	bool bProcessMessage(const TMessage& Msg);

	USB_FT_ACTION GetAction() const
	{
		return Action;
	}

	int nGetIMTDeviceID() const
	{
		return nIMTDevice;
	}

	int nGetIMTSerial() const
	{
		return nIMTSerial;
	}

	String sIMTDevice();



};


#endif
