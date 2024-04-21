#include "Precompile.h"

PSPCDIO_DEVEXT SetupDevExt(PDEVICE_OBJECT device)
{
    PSPCDIO_DEVEXT devext = GetDevExt(device);
    ULONG size = 0;

    size = sizeof(SPCDIO_DEVEXT);
    RtlZeroMemory(devext, size);

    devext->DevObj = device;
    devext->SymLinkOk = FALSE;
    devext->CpuCount = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);

    size = devext->CpuCount * sizeof(SRAT_LOCAL_APIC_AFFINITY);
    devext->SratLocalApicAffinity = (PSRAT_LOCAL_APIC_AFFINITY)
                    ExAllocatePoolWithTag(NonPagedPool, size, TAG_GENERIC_BUFFER);
    RtlZeroMemory(devext->SratLocalApicAffinity, size);

    size = devext->CpuCount * sizeof(SRAT_X2APIC_AFFINITY);
    devext->SratX2ApicAffinity = (PSRAT_X2APIC_AFFINITY)
        ExAllocatePoolWithTag(NonPagedPool, size, TAG_GENERIC_BUFFER);
    RtlZeroMemory(devext->SratX2ApicAffinity, size);

    RtlInitUnicodeString(&devext->DevName, DEVICE_NAME);
    RtlInitUnicodeString(&devext->SymbolicName, SYMLINK_NAME);
    KeInitializeSpinLock(&devext->PciCfgLock);
    KeInitializeSpinLock(&devext->DirectIoLock);
    return devext;
}

void TeardownDevExt(PSPCDIO_DEVEXT devext)
{
    if(NULL != devext->SratLocalApicAffinity)
    {
        ExFreePoolWithTag(devext->SratLocalApicAffinity, TAG_GENERIC_BUFFER);
        devext->SratLocalApicAffinity = NULL;
    }
    if (NULL != devext->SratX2ApicAffinity)
    {
        ExFreePoolWithTag(devext->SratX2ApicAffinity, TAG_GENERIC_BUFFER);
        devext->SratX2ApicAffinity = NULL;
    }
}