// ResetPcieSlotPower.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("ResetPcieSlotPower.exe is a tool to Reset power of specified PCIe slot.\n"));
    _tprintf(_T("Device is specified by BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: SetPcieSlotIndicator.exe <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to reset power for slot (Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  ResetPcieSlotPower.exe 2 31 2\n\n"));
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

    SetPCIeSlotPowerControl(bus_id, dev_id, func_id, FALSE);
    Sleep(1000*5);
    SetPCIeSlotPowerControl(bus_id, dev_id, func_id, TRUE);

    return 0;
}

