// Example_GetMsiCap.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
    _tprintf(_T("GetMsiCap.exe is a tool to retrieve MSI CAP from spcified Device.\n"));
    _tprintf(_T("Device is specified by AcpiDomain(Segment)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetMsiCap.exe <Domain> <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get MSI CAP from device (Domain 0, Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiCap.exe 0 2 31 2\n\n"));
}

void PrintMsiCap(PCI_MSI_CAP *cap)
{
    _tprintf(_T("[MSI CAP]\n"));
    _tprintf(_T("CapID=%X, Next=%X\n"), cap->Header.CapabilityID, cap->Header.Next);
    _tprintf(_T("MSIE=%d, MMC=%d, MME=%d, C64=%d, PVM=%d\n"), 
        cap->MC.MSIE, cap->MC.MMC, cap->MC.MME, cap->MC.C64, cap->MC.PVM);
    _tprintf(_T("MsgAddr = %08X%08X\n"), cap->MA_UP, cap->MA_LOW);
    _tprintf(_T("MD=%X, MMASK=%X, MPEND=%X\n"), cap->MD, cap->MMASK, cap->MPEND);
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc < 5)
    {
        Usage();
        return -1;
    }

    PCI_MSI_CAP cap = { 0 };
    DWORD error = ERROR_SUCCESS;
    int domain = _tstoi(argv[1]);
    int bus_id = _tstoi(argv[2]);
    int dev_id = _tstoi(argv[3]);
    int func_id = _tstoi(argv[4]);

    _tprintf(_T("getting MsiCap for Device BDF(%d.%d.%d.%d)\n"), domain, bus_id, dev_id, func_id);
    error = ReadMsiCap(domain, bus_id, dev_id, func_id, &cap);
    if(ERROR_SUCCESS != error)
        _tprintf(_T("ReadMsiCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintMsiCap(&cap);

    return 0;
}

