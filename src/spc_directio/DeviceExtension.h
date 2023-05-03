#pragma once
typedef struct
{
    PDEVICE_OBJECT  DevObj;
    UNICODE_STRING  DevName;
    BOOLEAN SymLinkOk;
    UNICODE_STRING  SymbolicName;
    ACPI_MCFG_TABLE McfgTable;
    ULONG AcpiTableTags[1024];

    //Mapped SystemVA of "PCIe Enhanced Configuration Access Mechanism" BaseAddress
    PUCHAR EcamBase;

    KSPIN_LOCK Lock;
} SPCDIO_DEVEXT, * PSPCDIO_DEVEXT;

PSPCDIO_DEVEXT InitDeviceExtension(PDEVICE_OBJECT device);

__inline PSPCDIO_DEVEXT GetDevExt(PDEVICE_OBJECT device)
{
    return (PSPCDIO_DEVEXT)device->DeviceExtension;
}
