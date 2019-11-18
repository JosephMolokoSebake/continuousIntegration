// interface.h - device interface classes for CommsInt

// This GUID identifies the device interface class used by the CommsIntDevice device

// TODO:	If your driver supports a standard interface, use the GUID that identifies
//			the interface class, rather than using the one defined below


#ifndef COMMSINT_GUID_DEFINED
#define COMMSINT_GUID_DEFINED

// {9832E983-ABB0-4c35-A757-9F6552A7B016}
#define CommsIntDevice_CLASS_GUID \
 { 0xe0447574, 0x60f8, 0x4c83, { 0xb0, 0xad, 0xe6, 0xa8, 0xb0, 0x51, 0x97, 0x17 } }

#endif
