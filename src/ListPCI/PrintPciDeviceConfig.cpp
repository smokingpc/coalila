#include "Precompile.h"


void PrintType0Header(PCI_HEADER_TYPE_0* type0)
{
    _tprintf(_T("    BaseAddress[0]=%08X, BaseAddress[1]=%08X\n"),
        type0->BaseAddresses[0], type0->BaseAddresses[1]);
    _tprintf(_T("    BaseAddress[2]=%08X, BaseAddress[3]=%08X\n"),
        type0->BaseAddresses[2], type0->BaseAddresses[3]);
    _tprintf(_T("    BaseAddress[4]=%08X, BaseAddress[5]=%08X\n"),
        type0->BaseAddresses[4], type0->BaseAddresses[5]);
    _tprintf(_T("    CIS=%08X, SubVendor=%04X, SubSystem=%04X\n"),
        type0->CIS, type0->SubVendorID, type0->SubSystemID);
    _tprintf(_T("    ROMBaseAddress=%08X, CapabilitiesPtr=%X\n"),
        type0->ROMBaseAddress, type0->CapabilitiesPtr);
    _tprintf(_T("    InterruptLine=%X, InterruptPin=%X\n"),
        type0->InterruptLine, type0->InterruptPin);
    _tprintf(_T("    MinimumGrant=%X, MaximumLatency=%X\n"),
        type0->MinimumGrant, type0->MaximumLatency);
    _tprintf(_T("\n"));
}
void PrintType1Header(PCI_HEADER_TYPE_1* type1)
{
    _tprintf(_T("    BaseAddress[0]=%08X, BaseAddress[1]=%08X\n"),
        type1->BaseAddresses[0], type1->BaseAddresses[1]);
    _tprintf(_T("    PrimaryBus=%X, SecondaryBus=%X\n"),
        type1->PrimaryBus, type1->SecondaryBus);
    _tprintf(_T("    SubordinateBus=%X, SecondaryLatency=%X\n"),
        type1->SubordinateBus, type1->SecondaryLatency);
    _tprintf(_T("    IOBase=%X, IOLimit=%X, SecondaryStatus=%04X\n"),
        type1->IOBase, type1->IOLimit, type1->SecondaryStatus);
    _tprintf(_T("    MemoryBase=%04X, MemoryLimit=%04X\n"),
        type1->MemoryBase, type1->MemoryLimit);
    _tprintf(_T("    PrefetchBase=%04X, PrefetchLimit=%04X\n"),
        type1->PrefetchBase, type1->PrefetchLimit);
    _tprintf(_T("    PrefetchBaseUpper32=%08X, PrefetchLimitUpper32=%08X\n"),
        type1->PrefetchBaseUpper32, type1->PrefetchLimitUpper32);
    _tprintf(_T("    IOBaseUpper16=%04X, IOLimitUpper16=%04X\n"),
        type1->IOBaseUpper16, type1->IOLimitUpper16);
    _tprintf(_T("    CapabilitiesPtr=%X\n"),
        type1->CapabilitiesPtr);
    _tprintf(_T("    ROMBaseAddress=%08X, BridgeControl=%04X\n"),
        type1->ROMBaseAddress, type1->BridgeControl);
    _tprintf(_T("    InterruptLine=%X, InterruptPin=%X\n"),
        type1->InterruptLine, type1->InterruptPin);
    _tprintf(_T("\n"));
}
void PrintPciHeader(USHORT domain, UCHAR bus, UCHAR dev, UCHAR func, PCIDEV_COMMON_CONFIG* header)
{
    _tprintf(_T("[PCI Device %04X.%02X.%02X.%X]\n"), domain, bus, dev, func);

    _tprintf(_T("    VEN_%04X  DEV_%04X  REV=%X\n"),
        header->VendorID, header->DeviceID, header->RevisionID);
    _tprintf(_T("    Command=%04X, Status=%04X\n"),
        header->Command, header->Status);
    _tprintf(_T("    ProgIf=%X, BaseClass=%X, SubClass=%X, CacheLineSize=%X\n"),
        header->ProgIf, header->BaseClass, header->SubClass, header->CacheLineSize);
    _tprintf(_T("    LatencyTimer=%X, HeaderType=%X, BIST=%X\n"),
        header->LatencyTimer, header->HeaderType.Type, header->BIST);
    if (header->HeaderType.IsMultiFunc)
        _tprintf(_T("    **This is multi-function device.\n"));

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
}
