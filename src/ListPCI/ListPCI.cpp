// ListPCI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("ListPCI.exe is a tool to scan and list all PCI device.\n"));
    _tprintf(_T("No options needed.....\n"));
}

void PrintPciHeader(READ_PCI_CFGHEADER *header)
{}

void QueryPCI(USHORT seg, UCHAR bus, UCHAR dev, UCHAR func)
{
    READ_PCI_CFGHEADER result = {0};
    if(ReadPciCfgHeader(bus, dev, func, &result))
        PrintPciHeader(&result);
}

void ScanDevice(USHORT seg, UCHAR bus, UCHAR dev) 
{
    for (UCHAR func = 0; func < 7; func++)
    {
        QueryPCI(seg, bus, dev, func);
    }
}

void ScanBus(USHORT seg, UCHAR bus) 
{
    for (UCHAR dev = 0; dev < 32; dev++)
    {
        ScanDevice(seg, bus, dev);
    }
}

void ScanSegment(USHORT id)
{
    for (UCHAR bus = 0; bus < UCHAR_MAX; bus++)
    {
        ScanBus(id, bus);
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    //USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    //UCHAR BusId;    //0~255
    //UCHAR DevId;    //0~32
    //UCHAR FuncId;   //0~7
    ScanSegment(0);

    return 0;
}
