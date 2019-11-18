// PCI32_WDMioctl.h
//
// Define control codes for PCI32_WDM driver
//

#ifndef __PCI32_WDMioctl__h_
#define __PCI32_WDMioctl__h_

#define PCI32_WDM_DRIVER_VERSION 1000
#define PCI32_WDM_INDEX_OFFS     0
#define PCI32_WDM_NUM_OFFS       1
#define PCI32_WDM_DATA_OFFS      2

#define PCI32_WDM_VERSION	CTL_CODE(FILE_DEVICE_UNKNOWN,	0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PCI32_WDM_CFG_WR	CTL_CODE(FILE_DEVICE_UNKNOWN,	0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PCI32_WDM_CFG_RD	CTL_CODE(FILE_DEVICE_UNKNOWN,	0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PCI32_WDM_IO_WR		CTL_CODE(FILE_DEVICE_UNKNOWN,	0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define PCI32_WDM_IO_RD		CTL_CODE(FILE_DEVICE_UNKNOWN,	0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif