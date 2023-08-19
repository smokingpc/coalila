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
    _tprintf(_T("SetPcieSlotIndicator.exe is a tool to test indicator(LED) of specified PCIe slot.\n"));
    _tprintf(_T("Device is specified by AcpiSegment(Domain)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: SetPcieSlotIndicator.exe <Domain> <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to Test Power LED and Attention LED for device (Domain 0, Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  SetPcieSlotIndicator.exe 0 2 31 2\n\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 5)
    {
        Usage();
        return -1;
    }
    LED_STATE led = LED_STATE::BLINK;
    DWORD error = ERROR_SUCCESS;
    int domain = _tstoi(argv[1]);
    int bus_id = _tstoi(argv[2]);
    int dev_id = _tstoi(argv[3]);
    int func_id = _tstoi(argv[4]);

    _tprintf(_T("Set Device BDF(%d,%d,%d,%d) LED(Atten=BLINK, Power=BLINK)\n"), domain, bus_id, dev_id, func_id);
    led = LED_STATE::BLINK;
    error = SetPCIeSlotAttentionIndicator(domain, bus_id, dev_id, func_id, led);
    if(ERROR_SUCCESS != error)
        _tprintf(_T("Set AttentionIndicator failed, lasterror=%d\n"), error);

    error = SetPCIeSlotPowerIndicator(domain, bus_id, dev_id, func_id, led);
    if (ERROR_SUCCESS != error)
        _tprintf(_T("Set AttentionIndicator failed, lasterror=%d\n"), error);

    _tprintf(_T("Led is set, wait for 5 seconds\n"));
    Sleep(1000 * 5);
    _tprintf(_T("Set Device BDF(%d,%d,%d,%d) LED(Atten=OFF, Power=OFF)\n"), domain, bus_id, dev_id, func_id);

    led = LED_STATE::OFF;
    error = SetPCIeSlotAttentionIndicator(domain, bus_id, dev_id, func_id, led);
    if (ERROR_SUCCESS != error)
        _tprintf(_T("Set AttentionIndicator failed, lasterror=%d\n"), error);

    error = SetPCIeSlotPowerIndicator(domain, bus_id, dev_id, func_id, led);
    if (ERROR_SUCCESS != error)
        _tprintf(_T("Set AttentionIndicator failed, lasterror=%d\n"), error);

    _tprintf(_T("done.\n"));
    return 0;
}

