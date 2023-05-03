// Example_GetMsiCap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("GetMsiCap.exe is a tool to retrieve MSI CAP from spcified Device.\n"));
    _tprintf(_T("Device is specified by BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetMsiCap.exe <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get MSI CAP from device (Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiCap.exe 2 31 2\n\n"));
}

void PrintMsiCap(PCI_MSI_CAP *cap)
{
    _tprintf(_T("CapID=%X, Next=%X\n"), cap->Header.CapabilityID, cap->Header.Next);
    _tprintf(_T("MSIE=%d, MMC=%d, MME=%d, C64=%d, PVM=%d\n"), 
        cap->MC.MSIE, cap->MC.MMC, cap->MC.MME, cap->MC.C64, cap->MC.PVM);
    _tprintf(_T("MsgAddr = %llX\n"), cap->MA);
    _tprintf(_T("MD=%X, MMASK=%X, MPEND=%X\n"), cap->MD, cap->MMASK, cap->MPEND);
}


int _tmain(int argc, _TCHAR* argv[])
{
    if(argc < 4)
    {
        Usage();
        return -1;
    }

    int bus_id = _tstoi(argv[1]);
    int dev_id = _tstoi(argv[2]);
    int func_id = _tstoi(argv[3]);
    PCI_MSI_CAP cap = {0};

    if(!ReadMsiCap(bus_id, dev_id, func_id, &cap))
        _tprintf(_T("ReadMsiCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintMsiCap(&cap);

    return 0;
}

