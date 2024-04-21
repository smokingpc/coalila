// GetMsiXCap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\PCI_EnumAndConsts.h"
#include "..\common\PCI_Structures.h"
#include "..\common\PCIe_EnumAndConsts.h"
#include "..\common\PCIe_Structures.h"
#include "..\common\IoctlCmd.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("GetMsiXCap.exe is a tool to retrieve MSI CAP from spcified Device.\n"));
    _tprintf(_T("Device is specified by AcpiDomain(Segment)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetMsiXCap.exe <Domain> <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get MSIX CAP from device (Domain 0, Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 0 2 31 2\n\n"));
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
    if (argc < 5)
    {
        Usage();
        return -1;
    }

    PCI_MSIX_CAP cap = { 0 };
    DWORD error = ERROR_SUCCESS;
    int domain = _tstoi(argv[1]);
    int bus_id = _tstoi(argv[2]);
    int dev_id = _tstoi(argv[3]);
    int func_id = _tstoi(argv[4]);

    _tprintf(_T("getting MsixCap for Device BDF(%d.%d.%d.%d)\n"), domain, bus_id, dev_id, func_id);
    error = ReadMsixCap(domain, bus_id, dev_id, func_id, &cap);
    if (ERROR_SUCCESS != error)
        _tprintf(_T("ReadMsixCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintMsixCap(&cap);


    return 0;
}

