#include "Precompile.h"

//only support PCIE, PCI_POWER_MGR, MSI, MSIX, SATA_CONFIG in current stage.
ULONG ParsePciCapSize(PCICAP_ID cap_id)
{
    switch(cap_id)
    {
        case PCICAP_ID::PCIE:
        return sizeof();
        case PCICAP_ID::PCI_POWER_MGR:
            return sizeof();
        case PCICAP_ID::MSI:
            return sizeof();
        case PCICAP_ID::MSIX:
            return sizeof();
    }

    return (ULONG)-1;
}

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
    if (in_size < sizeof(READ_PCI_CAP))
    {
        ret_size = sizeof(READ_PCI_CAP);
        return STATUS_INVALID_PARAMETER;
    }


    //if(out_size < sizeof(PCIDEV_CONFIG_SPACE))
    //{
    //    ret_size = sizeof(PCIDEV_CONFIG_SPACE);
    //    return STATUS_INVALID_BUFFER_SIZE;
    //}

    if(NULL == devext->EcamBase)
    {
        ret_size = 0;
        return STATUS_ACPI_NOT_INITIALIZED;
    }

    READ_PCI_CAP *arg = (READ_PCI_CAP*)buffer;
    PUCHAR header = GetEcamCfgAddr(devext, arg->BusId, arg->DevId, arg->FuncId);
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)(header + sizeof(PCI_COMMON_HEADER));
    bool found = false;
    while(NULL != cap && 0 != cap->CapabilityID)
    {
        //if(cap[0] == (UCHAR)arg->CapID)
        //{
        //    RtlCopyMemory()
        //    found = true;
        //    break;
        //}
        
        if(0 == cap[1])
            cap = NULL;
        else
            cap = header + sizeof(PCI_COMMON_HEADER) + cap[1];
    }

    ret_size = 0;
    return STATUS_NOT_SUPPORTED;
}

NTSTATUS PCIeSetSlotControl(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(out_size);

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

//this function generates PCI_COMMON_HEADER address in ECAM space.
//please refer to PCIe spec: Enhanced Configuration Access Mechanism.
PUCHAR GetEcamCfgAddr(PSPCDIO_DEVEXT devext, UCHAR bus, UCHAR dev, UCHAR func)
{
    ECAM_OFFSET offset = {0};
    if(NULL == devext->EcamBase)
        return NULL;

    offset.u.Bus = bus;
    offset.u.Dev = dev;
    offset.u.Func = func;

    return devext->EcamBase + offset.AsAddr;
}