#include "Precompile.h"

//todo: check segment acpi table
// https://github.com/KunYi/DumpACPI

//for debugging easier
PUCHAR FirstEcamBase = NULL;
PPCIE_ENHANCED_CAP PcieDebug = NULL;
ECAM_OFFSET DevOffset = { 0 };
ACPI_SRAT_TABLE SratTable = {0};
SRAT_MEMORY_AFFINITY SratMemoryAffinity[DEFAULT_SRAT_ENTRIES];
PSRAT_LOCAL_APIC_AFFINITY SratLocalApicAffinity = NULL;
PSRAT_X2APIC_AFFINITY SratX2ApicAffinity = NULL;
ACPI_MCFG_TABLE McfgTable = {0};

__inline void FillEasyDebugInfo(PSPCDIO_DEVEXT devext)
{
    //copy data from device extension to global variables, for easier debugging.
    for (ULONG i = 0; i < MAX_PCI_SEGMENTS; i++)
    {
        if(NULL != devext->EcamBase[i])
        {
            FirstEcamBase = devext->EcamBase[i];
            break;
        }
    }

    RtlCopyMemory(&SratTable, &devext->SratTable, sizeof(SratTable));
    RtlCopyMemory(&SratMemoryAffinity, &devext->SratMemoryAffinity, sizeof(SratMemoryAffinity));
    SratLocalApicAffinity = devext->SratLocalApicAffinity;
    SratX2ApicAffinity = devext->SratX2ApicAffinity;
    RtlCopyMemory(&McfgTable, &devext->McfgTable, sizeof(McfgTable));
}


NTSTATUS EnumAcpiTables(PSPCDIO_DEVEXT devext)
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
NTSTATUS LoadAcpiTables(PSPCDIO_DEVEXT devext)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG buf_size = 0;
    ULONG ret_size = 0;

    buf_size = sizeof(devext->McfgTable);
    ret_size = 0;
    status = AuxKlibGetSystemFirmwareTable(
                FIRMWARE_ACPI, TABLE_MCFG,
                &devext->McfgTable, buf_size, &ret_size);
    ASSERT(ret_size <= buf_size);

    buf_size = sizeof(devext->SratTable);
    ret_size = 0;
    status = AuxKlibGetSystemFirmwareTable(
        FIRMWARE_ACPI, TABLE_SRAT,
        &devext->SratTable, buf_size, &ret_size);
    ASSERT(ret_size <= buf_size);

    return status;
}
NTSTATUS LoadSratEntries(PSPCDIO_DEVEXT devext)
{
    UINT32 entry_len = 0;
    UINT32 total_entries_len = devext->SratTable.Header.Length - 
                                FIELD_OFFSET(ACPI_SRAT_TABLE, Entries) ;
    PUCHAR start = (PUCHAR)devext->SratTable.Entries;
    PUCHAR ptr = (PUCHAR)devext->SratTable.Entries;
    UINT32 mem_idx = 0, apic_idx=0, x2apic_idx=0;

    while((ptr - start) < total_entries_len)
    {
        PACPI_SRAT_ENTRY entry = (PACPI_SRAT_ENTRY)ptr;
        entry_len = ((PACPI_SRAT_ENTRY)ptr)->Length;

        if(0 == entry_len)
            break;

        switch(entry->Type)
        {
        case SRAT_ENTRY_TYPE::APIC_SAPIC:
        //some large scale numa system could has many disabled entries here.
        //skip them.
            if(!((PSRAT_LOCAL_APIC_AFFINITY)entry)->Flags.Enabled || apic_idx >= devext->CpuCount)
                break;
            RtlCopyMemory(&devext->SratLocalApicAffinity[apic_idx], ptr, entry_len);
            apic_idx++;
            break;
        case SRAT_ENTRY_TYPE::MEMORY:
            //skip disabled nodes...
            if (!((PSRAT_MEMORY_AFFINITY)entry)->Flags.Enabled)
                break;
            RtlCopyMemory(&devext->SratMemoryAffinity[mem_idx], ptr, entry_len);
            mem_idx++;
            break;
        case SRAT_ENTRY_TYPE::X2APIC:
            //skip disabled nodes...
            if (!((PSRAT_X2APIC_AFFINITY)entry)->Flags.Enabled)
                break;
            RtlCopyMemory(&devext->SratX2ApicAffinity[x2apic_idx], ptr, entry_len);
            x2apic_idx++;
            break;
        }
        ptr += entry_len;
    }

    return STATUS_SUCCESS;
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

    status = EnumAcpiTables(devext);
    if (!NT_SUCCESS(status))
        return status;
    
    status = LoadAcpiTables(devext);
    if (!NT_SUCCESS(status))
        return status;

    MapEcamBase(devext);
    LoadSratEntries(devext);
    FillEasyDebugInfo(devext);
    return status;
}
void TeardownAcpiInfo(PSPCDIO_DEVEXT devext)
{
    UnmapEcamBase(devext);
}
