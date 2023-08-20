#include "Precompile.h"

#define FIRMWARE_ACPI ((ULONG) 'ACPI')
#define TABLE_MCFG ((ULONG) 'GFCM')

//todo: check segment acpi table
// https://github.com/KunYi/DumpACPI

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
    //Each Segment has 256MB. Currently only support 64 Segment....
    RtlZeroMemory(devext->EcamBase, sizeof(devext->EcamBase));
    for(ULONG i=0; i< MAX_PCI_SEGMENTS; i++)
    {
        PHYSICAL_ADDRESS addr = { 0 };
        addr.QuadPart = devext->McfgTable.Segment[i].BaseAddress;
        if(0 == addr.QuadPart)
            continue;
        devext->EcamBase[i] = (PUCHAR)MmMapIoSpace(addr, ECAM_SEGMENT_SIZE, MmNonCached);
        if (NULL == devext->EcamBase)
            continue;
    }
    return STATUS_SUCCESS;
}
void UnmapEcamBase(PSPCDIO_DEVEXT devext)
{
    for (ULONG i = 0; i < MAX_PCI_SEGMENTS; i++)
    {
    //don't clean up devext->EcamBase content, leave some debug info... 
        if (NULL != devext->EcamBase[i])
            MmUnmapIoSpace(devext->EcamBase[i], ECAM_SEGMENT_SIZE);
    }
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
