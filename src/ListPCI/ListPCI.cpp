// ListPCI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("ListPCI.exe is a tool to scan and list all PCI device.\n"));
    _tprintf(_T("No options needed.....\n"));
}

void PrintType0Header(PCIDEV_CFG_HEADER::_PCI_HEADER_TYPE_0 *type0) 
{
    //ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
    //ULONG   CIS;
    //USHORT  SubVendorID;
    //USHORT  SubSystemID;
    //ULONG   ROMBaseAddress;
    //UCHAR   CapabilitiesPtr;
    //UCHAR   Reserved1[3];
    //ULONG   Reserved2;
    //UCHAR   InterruptLine;      //
    //UCHAR   InterruptPin;       // (ro)
    //UCHAR   MinimumGrant;       // (ro)
    //UCHAR   MaximumLatency;     // (ro)


    _tprintf(_T("    BaseAddress[0]=%08X, BaseAddress[1]=%08X\n"),
        type0->BaseAddresses[0], type0->BaseAddresses[1]);
    _tprintf(_T("    BaseAddress[2]=%08X, BaseAddress[3]=%08X\n"),
        type0->BaseAddresses[2], type0->BaseAddresses[3]);
    _tprintf(_T("    BaseAddress[4]=%08X, BaseAddress[5]=%08X\n"),
        type0->BaseAddresses[4], type0->BaseAddresses[5]);
    _tprintf(_T("    \n"));
}

void PrintType1Header(PCIDEV_CFG_HEADER::_PCI_HEADER_TYPE_1 *type1)
{
    _tprintf(_T("    \n"));
    _tprintf(_T("    \n"));
    _tprintf(_T("    \n"));
    _tprintf(_T("    \n"));
}

void PrintPciHeader(UCHAR bus, UCHAR dev, UCHAR func, PCIDEV_CFG_HEADER* header)
{
    _tprintf(_T("[PCI Device %02X.%02X.%X]\n"), bus, dev, func);

    _tprintf(_T("    VEN_%04X  DEV_%04X  REV=%X\n"), 
        header->VendorID, header->DeviceID, header->RevisionID);

    _tprintf(_T("    Command=%04X, Status=%04X\n"), header->Command, header->Status);
    _tprintf(_T("    ProgIf=%X, BaseClass=%X, SubClass=%X, CacheLineSize=%X\n"),
        header->ProgIf, header->BaseClass, header->SubClass, header->CacheLineSize);
    _tprintf(_T("    LatencyTimer=%X, HeaderType=%X, BIST=%X\n"),
        header->LatencyTimer, header->HeaderType.Type, header->BIST);
    if(header->HeaderType.IsMultiFunc)
        _tprintf(_T("    **This is multi-function device.\n"));

    switch(header->HeaderType.Type)
    {
    case 0:
        PrintType0Header(&header->Type0);
        break;
    case 1:
        PrintType1Header(&header->Type1);
        break;
    default:
        _tprintf(_T("    Unknown HeaderType, skip to print its content.\n"));
        break;
    }
}

void QueryPCI(USHORT seg, UCHAR bus, UCHAR dev, UCHAR func)
{
    PCIDEV_CFG_HEADER result = {0};
    if(ReadPciCfgHeader(bus, dev, func, &result))
        PrintPciHeader(bus, dev, func, &result);
}

void ScanDevice(USHORT seg, UCHAR bus, UCHAR dev) 
{
    for (UCHAR func = 0; func < 7; func++)
    {
        QueryPCI(seg, bus, dev, func);
    }
}

void ScanBus(USHORT seg, UCHAR bus) 
{
    for (UCHAR dev = 0; dev < 32; dev++)
    {
        ScanDevice(seg, bus, dev);
    }
}

void ScanSegment(USHORT id)
{
    for (UCHAR bus = 0; bus < UCHAR_MAX; bus++)
    {
        ScanBus(id, bus);
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    //USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    //UCHAR BusId;    //0~255
    //UCHAR DevId;    //0~32
    //UCHAR FuncId;   //0~7
    ScanSegment(0);

    return 0;
}
