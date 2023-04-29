// spc_pcilib.cpp : Defines the functions for the static library.
//

#include "pch.h"
//Mcfg table won't be changed before reboot.
static EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE MCFG = { 0 };
static bool HasMcfgTable = false;

void GetMcfgTable()
{
    DWORD in_size = sizeof(MCFG);
    DWORD out_size = 0;
    DWORD error = ERROR_SUCCESS;
    if(HasMcfgTable)
        return;

    out_size = GetSystemFirmwareTable(FIRMWARE_ACPI, TABLE_MCFG, &MCFG, in_size);
    if(0 == out_size)
    {
        error = GetLastError();
    }
    else if (out_size > in_size)
    {
        //not enough buffer size
    }
    else 
    {
        HasMcfgTable = true;
    }
}
