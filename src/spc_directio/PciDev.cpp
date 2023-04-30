#include "Precompile.h"

NTSTATUS ScanPciBus(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(out_size);
    //NTSTATUS status = STATUS_SUCCESS;

    if (in_size < sizeof(DIRECTIO_READ))
    {
        ret_size = sizeof(sizeof(DIRECTIO_READ));
        return STATUS_INVALID_PARAMETER;
    }

    ret_size = 0;
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS ReadPciCap(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);
    //NTSTATUS status = STATUS_NOT_SUPPORTED;

    if (in_size < sizeof(READ_PCI_CAP))
    {
        ret_size = sizeof(READ_PCI_CAP);
        return STATUS_INVALID_PARAMETER;
    }

    if(out_size < sizeof(PCIDEV_CONFIG_HEADER))
    {
        ret_size = sizeof(PCIDEV_CONFIG_HEADER);
        return STATUS_INVALID_BUFFER_SIZE;
    }
    //scan and find block
    //copy header data 

    ret_size = 0;
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS PCIeSetSlotControl(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(out_size);
    //NTSTATUS status = STATUS_NOT_SUPPORTED;

    if (in_size < sizeof(READ_PCI_CAP))
    {
        ret_size = sizeof(READ_PCI_CAP);
        return STATUS_INVALID_PARAMETER;
    }

    //scan and find block
    //copy header data 

    ret_size = 0;
    return STATUS_NOT_SUPPORTED;
}

