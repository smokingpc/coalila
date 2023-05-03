#include "Precompile.h"

#define FIRMWARE_ACPI ((ULONG) 'ACPI')
#define TABLE_MCFG ((ULONG) 'GFCM')

NTSTATUS MapEcamBase(PSPCDIO_DEVEXT devext);
void UnmapEcamBase(PSPCDIO_DEVEXT devext);

NTSTATUS EnumAcpiMcfgTable(PSPCDIO_DEVEXT devext)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG buf_size = sizeof(devext->AcpiTableTags);
    ULONG ret_size = 0;
    status = AuxKlibEnumerateSystemFirmwareTables(
        FIRMWARE_ACPI, devext->AcpiTableTags,
        buf_size, &ret_size);
    ASSERT(ret_size <= buf_size);
    return status;
}

NTSTATUS LoadAcpiMcfgTable(PSPCDIO_DEVEXT devext)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG buf_size = 0;
    ULONG ret_size = 0;

    status = EnumAcpiMcfgTable(devext);
    if (NT_SUCCESS(status))
    {
        buf_size = sizeof(devext->McfgTable);
        ret_size = 0;
        status = AuxKlibGetSystemFirmwareTable(
                    FIRMWARE_ACPI, TABLE_MCFG,
                    &devext->McfgTable, buf_size, &ret_size);
        ASSERT(ret_size <= buf_size);
    }

    return status;
}

NTSTATUS MapEcamBase(PSPCDIO_DEVEXT devext)
{
    PHYSICAL_ADDRESS addr = { 0 };
    addr.QuadPart = devext->McfgTable.Segment.BaseAddress;
    //Each Segment has 256MB. Currently only support 1 Segment....
    devext->EcamBase = (PUCHAR)MmMapIoSpace(addr, ECAM_SEGMENT_SIZE, MmNonCached);
    if (NULL == devext->EcamBase)
        return STATUS_MEMORY_NOT_ALLOCATED;
    return STATUS_SUCCESS;
}
void UnmapEcamBase(PSPCDIO_DEVEXT devext)
{
    if (NULL != devext->EcamBase)
        MmUnmapIoSpace(devext->EcamBase, ECAM_SEGMENT_SIZE);
    devext->EcamBase = NULL;
}
NTSTATUS SetupAcpiInfo(PSPCDIO_DEVEXT devext)
{
    NTSTATUS status = STATUS_SUCCESS;
    status = AuxKlibInitialize();
    if (!NT_SUCCESS(status))
        return status;

    status = EnumAcpiMcfgTable(devext);
    if (!NT_SUCCESS(status))
        return status;
    
    status = LoadAcpiMcfgTable(devext);
    if (!NT_SUCCESS(status))
        return status;

    MapEcamBase(devext);

    return status;
}
void TeardownAcpiInfo(PSPCDIO_DEVEXT devext)
{
    UnmapEcamBase(devext);
}
