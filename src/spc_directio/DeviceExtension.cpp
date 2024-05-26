#include "Precompile.h"

//todo: check segment acpi table
// https://github.com/KunYi/DumpACPI

//for debugging easier
PPCIE_CAPABILITIES PcieDebug = NULL;
PMSIX_TABLE_ENTRY MsixDebug;
ECAM_OFFSET DevOffset = { 0 };
PSPCDIO_DEVEXT SpcDevExt;

#pragma region  _SRAT_INFO and _MCFG_INFO
static ULONG CalcSratEntryCount(PACPI_SRAT table)
{
    PUCHAR cursor = (PUCHAR)(table+1);
    PUCHAR max_cursor = cursor + table->Header.Length;
    PACPI_SRAT_ENTRY entry = (PACPI_SRAT_ENTRY)cursor;
    ULONG count = 0;
    //typedef enum {
    //    SratProcessorLocalAPIC,
    //    SratMemory,
    //    SratProcessorLocalX2APIC,
    //    SratGicc
    //} SRAT_ENTRY_TYPE;

    while(entry->Length > 0 &&
            cursor < max_cursor &&
            entry->Type >= SratProcessorLocalAPIC && 
            entry->Type <= SratGicc)
    {
        count++;
        cursor += entry->Length;
        entry = (PACPI_SRAT_ENTRY)cursor;
    }
    return count;
}

NTSTATUS _SRAT_INFO::Setup()
{
    ULONG buf_size = 0;
    ULONG ret_size = 0;
    NTSTATUS status = AuxKlibGetSystemFirmwareTable(
                FIRMWARE_ACPI, ACPI_SRAT_SIGNATURE,
                NULL, 0, &ret_size);

    if(STATUS_BUFFER_TOO_SMALL != status)
        return status;

    buf_size = ret_size;
    this->SratTable = (PACPI_SRAT) ExAllocatePoolWithTag(
        NonPagedPool, buf_size, TAG_SRAT_DATA);

    if(NULL == this->SratTable)
        return STATUS_MEMORY_NOT_ALLOCATED;

    ret_size = 0;
    RtlZeroMemory(this->SratTable, buf_size);
    status = AuxKlibGetSystemFirmwareTable(
        FIRMWARE_ACPI, ACPI_SRAT_SIGNATURE,
        this->SratTable, buf_size, &ret_size);

    if(NT_SUCCESS(status))
    {
        //traverse all entries to determine SRAT Entries Count.
        this->SratCount = CalcSratEntryCount(this->SratTable);
        this->SratEntries = (PACPI_SRAT_ENTRY)(this->SratTable+1);
    }
    else
        this->Teardown();

    return status;
}
void _SRAT_INFO::Teardown() 
{
    if(NULL != this->SratTable)
    {
        ExFreePoolWithTag(this->SratTable, TAG_SRAT_DATA);
        this->SratTable = NULL;
    }
}

NTSTATUS _MCFG_INFO::Setup()
{
    ULONG buf_size = 0;
    ULONG ret_size = 0;
    NTSTATUS status = AuxKlibGetSystemFirmwareTable(
        FIRMWARE_ACPI, ACPI_SRAT_SIGNATURE,
        NULL, 0, &ret_size);

    if (STATUS_BUFFER_TOO_SMALL != status)
        return status;

    buf_size = ret_size;
    this->McfgTable = (PMCFG_TABLE)ExAllocatePoolWithTag(
        NonPagedPool, buf_size, TAG_MCFG_DATA);

    if (NULL == this->McfgTable)
        return STATUS_MEMORY_NOT_ALLOCATED;

    ret_size = 0;
    RtlZeroMemory(this->McfgTable, buf_size);
    status = AuxKlibGetSystemFirmwareTable(
        FIRMWARE_ACPI, MCFG_TABLE_SIGNATURE,
        this->McfgTable, buf_size, &ret_size);

    if (NT_SUCCESS(status))
    {
        //traverse all entries to determine MCFG Entries Count.
        this->McfgCount = 
            (this->McfgTable->Header.Length - sizeof(DESCRIPTION_HEADER)) / sizeof(MCFG_TABLE_ENTRY);
        this->McfgEntries = (PMCFG_TABLE_ENTRY)(this->McfgTable + 1);
    }
    else
    {
        this->Teardown();
    }

    return status;
}
void _MCFG_INFO::Teardown() 
{
    if (NULL != this->McfgTable)
    {
        ExFreePoolWithTag(this->McfgTable, TAG_SRAT_DATA);
        this->McfgTable = NULL;
    }
}
#pragma endregion

static NTSTATUS EnumAcpiTables(PSPCDIO_DEVEXT devext)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG buf_size = 0;
    ULONG ret_size = 0;
    status = AuxKlibEnumerateSystemFirmwareTables(
        FIRMWARE_ACPI, NULL, 0, &ret_size);

    if (STATUS_BUFFER_TOO_SMALL != status)
        return status;

    buf_size = ret_size;
    devext->AcpiTableTags = (PULONG)ExAllocatePoolWithTag(
        NonPagedPool, buf_size, TAG_ACPI_TAGS_TABLE);

    if (NULL == devext->AcpiTableTags)
        return STATUS_MEMORY_NOT_ALLOCATED;

    ret_size = 0;
    RtlZeroMemory(devext->AcpiTableTags, buf_size);
    status = AuxKlibEnumerateSystemFirmwareTables(
        FIRMWARE_ACPI, devext->AcpiTableTags, buf_size, &ret_size);

    if(!NT_SUCCESS(status))
    {
        ExFreePoolWithTag(devext->AcpiTableTags, TAG_ACPI_TAGS_TABLE);
        devext->AcpiTableTags = NULL;
    }
    return status;
}
void MapEcamBase(PSPCDIO_DEVEXT devext)
{
    //Each Segment has 256MB. Currently only support 64 Segment....
    RtlZeroMemory(devext->EcamBase, sizeof(devext->EcamBase));
    ULONG entry_limit = min(devext->McfgInfo.McfgCount, MAX_PCI_SEGMENTS);
    for (ULONG i = 0; i< entry_limit; i++)
    {
        PHYSICAL_ADDRESS addr = { 0 };
        addr.QuadPart = devext->McfgInfo.McfgEntries[i].BaseAddress;
        devext->EcamBase[i] = (PUCHAR)
            MmMapIoSpace(addr, ECAM_SEGMENT_SIZE, MmNonCached);
    }
}
void UnmapEcamBase(PSPCDIO_DEVEXT devext)
{
    ULONG entry_limit = min(devext->McfgInfo.McfgCount, MAX_PCI_SEGMENTS);
    for (ULONG i = 0; i < entry_limit; i++)
    {
        //don't clean up devext->EcamBase content, leave some debug info... 
        if (NULL != devext->EcamBase[i])
            MmUnmapIoSpace(devext->EcamBase[i], ECAM_SEGMENT_SIZE);
    }
}

PSPCDIO_DEVEXT SetupDevExt(PDEVICE_OBJECT device)
{
    PSPCDIO_DEVEXT devext = GetDevExt(device);
    ULONG size = 0;

    size = sizeof(SPCDIO_DEVEXT);
    RtlZeroMemory(devext, size);

    devext->DevObj = device;
    devext->SymLinkOk = FALSE;
    devext->CpuCount = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
    
    EnumAcpiTables(devext);
    devext->SratInfo.Setup();
    devext->McfgInfo.Setup();
    MapEcamBase(devext);

    RtlInitUnicodeString(&devext->DevName, DEVICE_NAME);
    RtlInitUnicodeString(&devext->SymbolicName, SYMLINK_NAME);
    KeInitializeSpinLock(&devext->PciCfgLock);
    KeInitializeSpinLock(&devext->DirectIoLock);
    SpcDevExt = devext;
    return devext;
}
void TeardownDevExt(PSPCDIO_DEVEXT devext)
{
    if(NULL != devext->AcpiTableTags)
    {
        ExFreePoolWithTag(devext->AcpiTableTags, TAG_ACPI_TAGS_TABLE);
        devext->AcpiTableTags = NULL;
    }
    UnmapEcamBase(devext);
    devext->SratInfo.Teardown();
    devext->McfgInfo.Teardown();
    SpcDevExt = NULL;
}
PVOID GetPciSpaceBySegment(PSPCDIO_DEVEXT devext, USHORT segment)
{
    
    for(ULONG i=0; i< MAX_PCI_SEGMENTS; i++)
    {
        if(0 != devext->McfgInfo.McfgEntries->BaseAddress &&
            segment == devext->McfgInfo.McfgEntries->SegmentNumber)
            return devext->EcamBase[i];
    }
    return NULL;
}
