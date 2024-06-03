#include "Precompile.h"


void PrintType0Header(PCI_HEADER_TYPE_0* type0)
{
    const TCHAR* outline = _T("    ");
    _tprintf(_T("  Type0 Header:\n"));
    _tprintf(_T("%sBaseAddress[0]=%08X\n%sBaseAddress[1]=%08X\n"),
        outline, type0->BaseAddresses[0], 
        outline, type0->BaseAddresses[1]);
    _tprintf(_T("%sBaseAddress[2]=%08X\n%sBaseAddress[3]=%08X\n"),
        outline, type0->BaseAddresses[2], 
        outline, type0->BaseAddresses[3]);
    _tprintf(_T("%sBaseAddress[4]=%08X\n%sBaseAddress[5]=%08X\n"),
        outline, type0->BaseAddresses[4], 
        outline, type0->BaseAddresses[5]);
    
    _tprintf(_T("%sCIS=%08X\n%sSubVendor=%04X\n%sSubSystem=%04X\n"),
        outline, type0->CIS, 
        outline, type0->SubVendorID, 
        outline, type0->SubSystemID);
    _tprintf(_T("%sROMBaseAddress=%08X\n%sCapabilitiesPtr=%X\n"),
        outline, type0->ROMBaseAddress, 
        outline, type0->CapabilitiesPtr);
    _tprintf(_T("%sInterruptLine=%X\n%sInterruptPin=%X\n"),
        outline, type0->InterruptLine, 
        outline, type0->InterruptPin);
    _tprintf(_T("%sMinimumGrant=%X\n%sMaximumLatency=%X\n"),
        outline, type0->MinimumGrant, 
        outline, type0->MaximumLatency);
    _tprintf(_T("\n"));
}
void PrintType1Header(PCI_HEADER_TYPE_1* type1)
{
    const TCHAR* outline = _T("    ");
    _tprintf(_T("  Type1 Header:\n"));
    _tprintf(_T("%sBaseAddress[0]=%08X%sBaseAddress[1]=%08X\n"),
        outline, type1->BaseAddresses[0],
        outline, type1->BaseAddresses[1]);
    _tprintf(_T("%sPrimaryBus=%X\n%sSecondaryBus=%X\n"),
        outline, type1->PrimaryBus, 
        outline, type1->SecondaryBus);
    _tprintf(_T("%sSubordinateBus=%X\n%sSecondaryLatency=%X\n"),
        outline, type1->SubordinateBus, 
        outline, type1->SecondaryLatency);
    _tprintf(_T("%sIOBase=%X\n%sIOLimit=%X\n%sSecondaryStatus=%04X\n"),
        outline, type1->IOBase, 
        outline, type1->IOLimit, 
        outline, type1->SecondaryStatus);
    _tprintf(_T("%sMemoryBase=%04X\n%sMemoryLimit=%04X\n"),
        outline, type1->MemoryBase, 
        outline, type1->MemoryLimit);
    _tprintf(_T("%sPrefetchBase=%04X\n%sPrefetchLimit=%04X\n"),
        outline, type1->PrefetchBase, 
        outline, type1->PrefetchLimit);
    _tprintf(_T("%sPrefetchBaseUpper32=%08X\n%sPrefetchLimitUpper32=%08X\n"),
        outline, type1->PrefetchBaseUpper32, 
        outline, type1->PrefetchLimitUpper32);
    _tprintf(_T("%sIOBaseUpper16=%04X\n%sIOLimitUpper16=%04X\n"),
        outline, type1->IOBaseUpper16, 
        outline, type1->IOLimitUpper16);
    _tprintf(_T("%sCapabilitiesPtr=%X\n"),
        outline, type1->CapabilitiesPtr);
    _tprintf(_T("%sROMBaseAddress=%08X\n%sBridgeControl=%04X\n"),
        outline, type1->ROMBaseAddress, 
        outline, type1->BridgeControl);
    _tprintf(_T("%sInterruptLine=%X\n%sInterruptPin=%X\n"),
        outline, type1->InterruptLine, 
        outline, type1->InterruptPin);
}
void PrintPciHeader(USHORT domain, UCHAR bus, UCHAR dev, UCHAR func, PCIDEV_COMMON_CONFIG* header)
{
    const TCHAR *outline = _T("  ");
    _tprintf(_T("%sVEN_%04X\n%sDEV_%04X\n%sREV=%X"),
        outline, header->VendorID, 
        outline, header->DeviceID, 
        outline, header->RevisionID);
    _tprintf(_T("%sCommand=%04X\n%sStatus=%04X\n"),
        outline, header->Command, 
        outline, header->Status);
    _tprintf(_T("%sProgramInterface=%X\n%sBaseClass=%X\n%sSubClass=%X\n%sCacheLineSize=%X\n"),
        outline, header->ProgIf, 
        outline, header->BaseClass, 
        outline, header->SubClass, 
        outline, header->CacheLineSize);
    _tprintf(_T("%sLatencyTimer=%X\n%sBIST=%X\n"),
        outline, header->LatencyTimer, 
        outline, header->BIST);

    _tprintf(_T("%sType="), outline);
    if (header->HeaderType.IsMultiFunc)
        _tprintf(_T("MultiFunction | "));
    _tprintf(_T("Type%d\n"), header->HeaderType.Type);

    switch (header->HeaderType.Type)
    {
    case 0:
        PrintType0Header(&header->u.Type0);
        break;
    case 1:
        PrintType1Header(&header->u.Type1);
        break;
    default:
        _tprintf(_T("    Unknown HeaderType, skip to print its content.\n"));
        break;
    }
    _tprintf(_T("\n"));
}
void PrintPciDevInfo(PPCIDEV_INFO info, BOOLEAN show_pcicfg)
{
    _tprintf(_T("[PCI Device %s] NtDevice=%s\n"), info->LocationStr, info->NtDeviceId);
    if(show_pcicfg)
        PrintPciHeader(
            info->Location.Segment, 
            info->Location.Bus, 
            info->Location.Dev, 
            info->Location.Func, 
            &info->Config);
    _tprintf(_T("\n"));
}
void PrintPciScanResult(list<PPCIDEV_INFO>& list, BOOLEAN verbose) 
{
    for(PPCIDEV_INFO &item : list)
    {
        PrintPciDevInfo(item, verbose);
    }
}


#if 0
void PrintSegment(list<PPCIDEV_INFO> &list){}
void PrintBus(list<PPCIDEV_INFO> &list) {}
void PrintDevice(list<PPCIDEV_INFO> &list) {}
void PrintFunction(list<PPCIDEV_INFO> &list) {}
#endif
