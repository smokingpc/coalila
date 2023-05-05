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
    _tprintf(_T("Format: SetPcieSlotIndicator.exe <BusID> <DevID> <FuncID> <atten led on/off> <power led on/off>\n"));
    _tprintf(_T("        on/off use TRUE or FALSE.\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to Power LED on and Attention LED off for device (Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 2 31 2 FALSE TRUE\n\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 6)
    {
        Usage();
        return -1;
    }

    int bus_id = _tstoi(argv[1]);
    int dev_id = _tstoi(argv[2]);
    int func_id = _tstoi(argv[3]);
    BOOLEAN att_led = FALSE;
    BOOLEAN pow_led = FALSE;
    PCI_MSIX_CAP cap = { 0 };

    if (_tcsicmp(argv[4], _T("true")) == 0)
        att_led = TRUE;

    if (_tcsicmp(argv[5], _T("true")) == 0)
        pow_led = TRUE;

    _tprintf(_T("Set Device BDF(%d,%d,%d) LED(Atten=%d, Power=%d)\n"), bus_id, dev_id, func_id, att_led, pow_led);
    SetPCIeSlotAttentionIndicator(bus_id, dev_id, func_id, att_led);
    SetPCIeSlotPowerIndicator(bus_id, dev_id, func_id, pow_led);

    return 0;
}

