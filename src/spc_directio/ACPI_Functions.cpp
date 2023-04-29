#include "Precompile.h"

static ACPI_MCFG_TABLE McfgTable = {0};
static ULONG AcpiTableTags[1024] = {0};

NTSTATUS LoadAcpiMcfgTable()
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG buf_size = sizeof(AcpiTableTags);
    ULONG ret_size = 0;
    status = AuxKlibEnumerateSystemFirmwareTables(
                FIRMWARE_ACPI, AcpiTableTags,
                buf_size, &ret_size);
    if (NT_SUCCESS(status))
    {
        buf_size = sizeof(McfgTable);
        ret_size = 0;
        status = AuxKlibGetSystemFirmwareTable(
                    FIRMWARE_ACPI, TABLE_MCFG,
                    &McfgTable, buf_size, &ret_size);
    }

    return status;
}

NTSTATUS LoadAcpiTables()
{
    NTSTATUS status = STATUS_SUCCESS; 
    status = AuxKlibInitialize();
    if(NT_SUCCESS(status))
        status = LoadAcpiMcfgTable();
    return status;
}
