// SetPcieSlotIndicator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("SetPcieSlotIndicator.exe is a tool to turn on/off the indicator(LED) \nof specified PCIe slot.\n"));
    _tprintf(_T("Device is specified by BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetMsiXCap.exe <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get MSIX CAP from device (Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 2 31 2\n\n"));
}

void PrintMsixCap(PCI_MSIX_CAP* cap)
{
    _tprintf(_T("[MSIX CAP]\n"));
    _tprintf(_T("CapID=%X, Next=%X\n"), cap->Header.CapabilityID, cap->Header.Next);
    _tprintf(_T("MXC.FM=%d, MXC.MXE=%d, MXC.TS=%d\n"),
        cap->MXC.FM, cap->MXC.MXE, cap->MXC.TS);
    _tprintf(_T("MTAB.TBIR=%d, cap->MTAB.TO=0x%08X\n"),
        cap->MTAB.TBIR, cap->MTAB.TO << 3);
    _tprintf(_T("MPBA.PBIR=%d, cap->MPBA.PBAO=0x%08X\n"),
        cap->MPBA.PBIR, cap->MPBA.PBAO << 3);
}


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 4)
    {
        Usage();
        return -1;
    }

    int bus_id = _tstoi(argv[1]);
    int dev_id = _tstoi(argv[2]);
    int func_id = _tstoi(argv[3]);
    PCI_MSIX_CAP cap = { 0 };

    _tprintf(_T("getting MsixCap for Device BDF(%d,%d,%d)\n"), bus_id, dev_id, func_id);
    if (!ReadMsixCap(bus_id, dev_id, func_id, &cap))
        _tprintf(_T("ReadMsixCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintMsixCap(&cap);


    return 0;
}

