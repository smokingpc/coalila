#pragma once

#define PCI_VENID_STRLEN   4+1
#define PCI_DEVID_STRLEN   4+1
#define VEN_DEV_ID_FORMAT  _T("%04X")    //PCI VendorID and DeviceID format

#define PCI_SUBSYS_ID_LEN   4+1
#define PCI_SUBVENID_STRLEN 4+1
//#define NT_SUBSYSID_STRLEN    (PCI_SUBSYS_ID_LEN+PCI_SUBVENID_STRLEN)
//#define NT_SUBSYSID_FORMAT _T("SUBSYS_%04X%04X")    //(HEX) "SUBSYS_aaaabbbb" a==SubSysemID and b==SubVendorID
#define NT_DEVID_STRLEN     64
#define NT_DEVID_FORMAT     _T("PCI\\VEN_%04X&DEV_%04X&SUBSYS_%04X%04X")

#define BDF_STR_LEN         32
#define BDF_FORMAT          _T("%04d:%02d:%02d:%d")    //decimal Segment:Bus:Device:Function location format.


typedef struct _CMD_ARGS {
    BOOLEAN ShowDetail;
    BOOLEAN UseSegId;
    BOOLEAN UseBusId;
    BOOLEAN UseDevId;
    BOOLEAN UseFuncId;

    SBDF_LOCATION Target;
}CMD_ARGS, *PCMD_ARGS;


typedef struct _PCIDEV_INFO {
    //_PCIDEV_INFO(){}
    //~_PCIDEV_INFO(){}
    //_PCIDEV_INFO *Parent;   //only devices on Bus0 has no parent...
    //list<_PCIDEV_INFO*> Child;

    SBDF_LOCATION Location;     //BDF location of PCI Devices
    PCIDEV_COMMON_CONFIG Config;
    BOOLEAN IsBridge;

    WCHAR LocationStr[BDF_STR_LEN];     //format: (decimal)"segment:bus:device:function", e.g. 0000:23:2:1
    WCHAR NtDeviceId[NT_DEVID_STRLEN];
    //WCHAR VendorID[PCI_VENID_STRLEN];
    //WCHAR DeviceID[PCI_DEVID_STRLEN];
    //WCHAR NtSubsysID[NT_SUBSYSID_STRLEN];   //SUBSYS_xxxxyyyy, x==SubSysemID and y==SubVendorID in PCI_HEADER_TYPE_0.
    WCHAR Name[MAX_PATH];
}PCIDEV_INFO, *PPCIDEV_INFO;

DWORD QueryPCI(
    IN OUT PPCIDEV_COMMON_CONFIG result,
    IN USHORT segment, 
    IN UCHAR bus, 
    IN UCHAR dev,
    IN UCHAR func);
void UpdatePciDevInfo(
    IN OUT PPCIDEV_INFO info,
    IN PPCIDEV_COMMON_CONFIG config,
    IN USHORT segment,
    IN UCHAR bus,
    IN UCHAR dev,
    IN UCHAR func);

void PrintType0Header(PCI_HEADER_TYPE_0* type0);
void PrintType1Header(PCI_HEADER_TYPE_1* type1);
void PrintPciHeader(
    USHORT domain, 
    UCHAR bus, 
    UCHAR dev, 
    UCHAR func, 
    PCIDEV_COMMON_CONFIG* header);
void PrintPciDevInfo(PPCIDEV_INFO info, BOOLEAN show_pcicfg);
void PrintPciScanResult(list<PPCIDEV_INFO>& list, BOOLEAN verbose);

#if 0
void PrintSegment(list<PPCIDEV_INFO>& list);
void PrintBus(list<PPCIDEV_INFO>& list);
void PrintDevice(list<PPCIDEV_INFO>& list);
void PrintFunction(list<PPCIDEV_INFO>& list);
#endif
