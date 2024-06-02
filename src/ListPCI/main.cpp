#include "Precompile.h"

void Usage()
{
    _tprintf(_T("ListPCI.exe is a tool to scan and list all PCI device.\n"));
    _tprintf(_T("Options:\n"));
    _tprintf(_T("    -addr=[segment.[bus.[dev.[func]]]] => list device tree by specified address.\n"));
    _tprintf(_T("    -detail => print detail device info.\n"));
    _tprintf(_T("Usage:\n"));
    _tprintf(_T("    ListPCI.exe => (no options) List entire PCI Device Tree. equal to ListPCI.exe -addr 0\n"));
    _tprintf(_T("    ListPCI.exe -addr=[segment.[bus.[dev.[func]]]] , (all id are decimal)\n"));
    _tprintf(_T("Example:\n"));
    _tprintf(_T("    ListPCI.exe => (no options) List entire PCI Device Tree. equal to ListPCI.exe -addr 0\n"));
    _tprintf(_T("    List all devices on segment 0, type \"ListPCI.exe\"\n"));
    _tprintf(_T("    List detail of devices on segment 0, type \"ListPCI.exe -detail\"\n"));
    _tprintf(_T("    List devices on segment 0 bus 7, type \"ListPCI.exe -addr=0.7\"\n"));
    _tprintf(_T("    Show detail of devices on segment 0 bus 13 device 1, type \"ListPCI.exe -detail -addr=0.13.1\"\n"));
}
void ScanDevice(
    IN OUT list<PPCIDEV_INFO>& list, 
    IN USHORT segment, 
    IN UCHAR bus, 
    IN UCHAR dev)
{
    for (UCHAR func = 0; func < 7; func++)
    {
//        QueryPCI(domain, bus, dev, func);
    }
}
void ScanBus(
    IN OUT list<PPCIDEV_INFO>& list, 
    IN USHORT segment, 
    IN UCHAR bus)
{
    for (UCHAR dev = 0; dev < 32; dev++)
    {
        ScanDevice(list, segment, bus, dev);
    }
}
void ScanSegment(
    IN OUT list<PPCIDEV_INFO> &list, 
    IN USHORT segment)
{
    for (UCHAR bus = 0; bus < UCHAR_MAX; bus++)
    {
        ScanBus(list, segment, bus);
    }
}



void ParseSBDF(IN _TCHAR *str, IN OUT PCMD_ARGS cmd)
{
    TCHAR *ctx = NULL;
    TCHAR* token = _tcstok_s(str, _T("."), &ctx);
    int cursor = 0;
    while(NULL != token)
    {
        switch(cursor)
        {
        case 0:
            cmd->Target.Segment = _ttoi(token);
            cmd->UseSegId = TRUE;
            break;
        case 1:
            cmd->Target.Bus = _ttoi(token);
            cmd->UseBusId = TRUE;
            break;
        case 2:
            cmd->Target.Dev = _ttoi(token);
            cmd->UseDevId = TRUE;
            break;
        case 3:
            cmd->Target.Func = _ttoi(token);
            cmd->UseFuncId = TRUE;
            break;
        default:
            return;
        }

        cursor++;
        token = _tcstok_s(NULL, _T("."), &ctx);
    }
}
BOOLEAN ParseArgs(
    IN OUT PCMD_ARGS cmd, 
    IN int argc, 
    IN _TCHAR* argv[])
{
    for(int i=1;i<argc;i++)
    {
        _TCHAR* arg = argv[i];
        if(0==_tcsnicmp(arg,_T("-addr"), _tcslen(_T("-addr"))))
        {
            TCHAR* ctx = NULL;
            TCHAR *token = _tcstok_s(arg, _T("="), &ctx);
            token = _tcstok_s(NULL, _T("="), &ctx);
            if(NULL != token)
                ParseSBDF(token, cmd);
        }
        else if (0 == _tcsnicmp(arg, _T("-detail"), _tcslen(_T("-detail"))))
        {
            cmd->ShowDetail = TRUE;
        }
        else
            continue;
    }

    return TRUE;
}
void DoCommand(IN PCMD_ARGS cmd)
{
    list<PPCIDEV_INFO> list;
    DWORD err = 0;
    USHORT segment = 0;
    UCHAR bus = 0;
    UCHAR dev = 0;
    UCHAR func = 0;

    if(cmd->UseSegId)
        segment = cmd->Target.Segment;
    if (cmd->UseBusId)
        bus = cmd->Target.Bus;
    if (cmd->UseDevId)
        dev = cmd->Target.Dev;
    if (cmd->UseFuncId)
        func = cmd->Target.Func;

    if(cmd->UseSegId && cmd->UseBusId && cmd->UseDevId && cmd->UseFuncId)
    {
        PPCIDEV_INFO query = new PCIDEV_INFO;
        err = QueryPCI(&query->Config, segment, bus, dev, func);
        if(ERROR_SUCCESS != err)
            delete query;
        else
            list.push_back(query);
    }
    else if (cmd->UseSegId && cmd->UseBusId && cmd->UseDevId)
    {
        ScanDevice(list, segment, bus, dev);
    }
    else if (cmd->UseSegId && cmd->UseBusId)
    {
        ScanBus(list, segment, bus);
    }
    else if (cmd->UseSegId)
    {
        ScanSegment(list, segment);
    }



}

int _tmain(int argc, _TCHAR* argv[])
{
    CMD_ARGS cmd = 
    {
        .UseSegId = TRUE,
        .Target = {.Segment = 0},
    };
    if(!ParseArgs(&cmd, argc, argv))
        return -1;

    DoCommand(&cmd);
    return 0;
}

