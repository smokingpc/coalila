// SetPcieSlotIndicator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

LED_STATE ConvertLedState(TCHAR *action)
{
    if(0 == _tcsnicmp(action, _T("on"), _tcslen(_T("on"))))
        return LED_STATE::ON;
    if (0 == _tcsnicmp(action, _T("off"), _tcslen(_T("off"))))
        return LED_STATE::OFF;
    if (0 == _tcsnicmp(action, _T("blink"), _tcslen(_T("blink"))))
        return LED_STATE::BLINK;
    return LED_STATE::UNKNOWN;
}

void Usage()
{
    _tprintf(_T("SetPcieSlotIndicator.exe is a tool to test indicator(LED) of specified PCIe slot.\n"));
    _tprintf(_T("Device is specified by AcpiSegment(Domain)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: SetPcieSlotIndicator.exe <Domain> <BusID> <DevID> <FuncID> <Type> <Action>\n"));
    _tprintf(_T("        <Type> = power / attention / atten\n"));
    _tprintf(_T("        <Action> = on / off / blink\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to Test Power LED of (Domain 0, Bus 02, Device 31, Function 2), Turn it to blink :\n"));
    _tprintf(_T("  SetPcieSlotIndicator.exe 0 2 31 2 power blink\n\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
    DWORD error = ERROR_SUCCESS;
    int domain = 0;
    int bus_id = 0;
    int dev_id = 0;
    int func_id = 0;
    TCHAR* type = NULL;
    TCHAR* action = NULL;
    LED_STATE led = LED_STATE::UNKNOWN;

    if (argc < 7)
        goto INVALID_PARAM;

    domain = _tstoi(argv[1]);
    bus_id = _tstoi(argv[2]);
    dev_id = _tstoi(argv[3]);
    func_id = _tstoi(argv[4]);
    type = argv[5];
    action = argv[6];
    led = ConvertLedState(action);

    if(led == LED_STATE::UNKNOWN)
    {
        _tprintf(_T("Unknown action (%s)\n"), action);
        goto INVALID_PARAM;
    }

    if (0 == _tcsnicmp(action, _T("power"), _tcslen(_T("power"))))
    {
        _tprintf(_T("Set Device BDF(%d,%d,%d,%d) LED(power=%s)\n"), domain, bus_id, dev_id, func_id, action);
        error = SetPCIeSlotPowerIndicator(domain, bus_id, dev_id, func_id, led);
        if (ERROR_SUCCESS != error)
            _tprintf(_T("Set PowerIndicator failed, lasterror=%d\n"), error);
    }
    else if (0 == _tcsnicmp(action, _T("atten"), _tcslen(_T("atten"))) ||
            0 == _tcsnicmp(action, _T("attention"), _tcslen(_T("attention"))))
    {
        _tprintf(_T("Set Device BDF(%d,%d,%d,%d) LED(atten=%s)\n"), domain, bus_id, dev_id, func_id, action);
        error = SetPCIeSlotAttentionIndicator(domain, bus_id, dev_id, func_id, led);
        if (ERROR_SUCCESS != error)
            _tprintf(_T("Set AttentionIndicator failed, lasterror=%d\n"), error);
    }
    else
    {
        _tprintf(_T("Unknown indicator (%s)\n"), type);
        goto INVALID_PARAM;
    }
    _tprintf(_T("done.\n"));
    return 0;

INVALID_PARAM:
    Usage();
    return -1;
}

