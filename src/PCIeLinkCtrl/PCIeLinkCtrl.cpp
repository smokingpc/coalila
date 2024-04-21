// PCIeLinkCtrl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ListPCI.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
    _tprintf(_T("PCIeLinkCtrl.exe is a tool to control PCIe Link for specified Device.\n"));
    _tprintf(_T("Device is specified by AcpiDomain(Segment)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: PCIeLinkCtrl.exe <Domain>.<BusID>.<DevID>.<FuncID> <action>\n"));
    _tprintf(_T("Action List: \n"));
    _tprintf(_T("    retrain : \n"));
    _tprintf(_T("    disable: \n"));
    _tprintf(_T("    enable : \n"));

    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to perform PCIe LinkRetrain for device (Domain 0, Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 0.2.31.2 retrain\n\n"));
}

bool ParseSBDF(TCHAR* argv, USHORT &segment, UCHAR &bus, UCHAR &dev, UCHAR &func)
{
    TCHAR* token[4] = {NULL};
    TCHAR *tok_ctx = NULL;
    token[0] = _tcstok_s(argv, _T("."), &tok_ctx);
    token[1] = _tcstok_s(NULL, _T("."), &tok_ctx);
    token[2] = _tcstok_s(NULL, _T("."), &tok_ctx);
    token[3] = _tcstok_s(NULL, _T("."), &tok_ctx);

    for(TCHAR *item : token)
    {
        if(NULL == item)
            return false;
    }

    segment = (USHORT)_ttoi(token[0]);
    bus = (UCHAR)_ttoi(token[1]);
    dev = (UCHAR)_ttoi(token[2]);
    func = (UCHAR)_ttoi(token[3]);

    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    //USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    //UCHAR BusId;    //0~255
    //UCHAR DevId;    //0~32
    //UCHAR FuncId;   //0~7
    if (argc < 3)
    {
        Usage();
        return -1;
    }

    DWORD error = ERROR_SUCCESS;
    UCHAR bus = 0, dev = 0, func = 0;
    USHORT segment = 0;
    if(!ParseSBDF(argv[1], segment, bus, dev, func))
        return -1;

    _tprintf(_T("PCIeLinkCtrl for Device BDF(%d.%d.%d.%d), Action=%s\n"), segment, bus, dev, func, argv[2]);

    if(0 == _tcsicmp(_T("retrain"), argv[2]))
    {
        error = SetPCIeLinkRetrain(segment, bus, dev, func);
        if(ERROR_SUCCESS != error)
        {
            _tprintf(_T("SetPCIeLinkRetrain() failed. LastError=%d\n"), error);
            return -1;
        }
    }
    else if (0 == _tcsicmp(_T("disable"), argv[2]))
    {
        _tprintf(_T("Not Implemented Yet!  exit....\n"));
    }
    else if (0 == _tcsicmp(_T("enable"), argv[2]))
    {
        _tprintf(_T("Not Implemented Yet!  exit....\n"));
    }
    else
    {
        _tprintf(_T("Unknown Command!  exit....\n"));
    }

    return 0;
}
