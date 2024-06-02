#pragma once

#define PCI_VENDOR_ID_LEN   4
#define PCI_DEVICE_ID_LEN   4
#define PCI_SUBSYS_ID_LEN   4
#define PCI_SUBVENDOR_ID_LEN   4
#define NT_SUBSYS_ID_LEN    (PCI_SUBSYS_ID_LEN+PCI_SUBVENDOR_ID_LEN)
#define BDF_FORMAT          L"%04d:%02d:%02d:%d"    //decimal Segment:Bus:Device:Function location format.
#define BDF_STR_LEN         16

typedef struct _CMD_ARGS {
    BOOLEAN ShowDetail;
    BOOLEAN UseSegId;
    BOOLEAN UseBusId;
    BOOLEAN UseDevId;
    BOOLEAN UseFuncId;

    SBDF_LOCATION Target;
}CMD_ARGS, *PCMD_ARGS;


typedef struct _PCIDEV_INFO {
    _PCIDEV_INFO *Parent;   //only devices on Bus0 has no parent...
    list< _PCIDEV_INFO *> Child;

    SBDF_LOCATION Location;     //BDF location of PCI Devices
    WCHAR LocationStr[BDF_STR_LEN];
    WCHAR VendorID[PCI_VENDOR_ID_LEN+1];
    WCHAR DeviceID[PCI_DEVICE_ID_LEN+1];
    WCHAR NtSubsysID[NT_SUBSYS_ID_LEN+1];   //SUBSYS_xxxxyyyy, x==SubSysemID and y==SubVendorID in PCI_HEADER_TYPE_0.
    WCHAR Name[MAX_PATH];

    PCIDEV_COMMON_CONFIG Config;
}PCIDEV_INFO, *PPCIDEV_INFO;

DWORD QueryPCI(
    IN OUT PPCIDEV_COMMON_CONFIG result,
    IN USHORT domain, 
    IN UCHAR bus, 
    IN UCHAR dev,
    IN UCHAR func);
void PrintType0Header(PCI_HEADER_TYPE_0* type0);
void PrintType1Header(PCI_HEADER_TYPE_1* type1);
void PrintPciHeader(USHORT domain, UCHAR bus, UCHAR dev, UCHAR func, PCIDEV_COMMON_CONFIG* header);