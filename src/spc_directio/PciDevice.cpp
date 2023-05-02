#include "Precompile.h"

//this function generates PCI_COMMON_HEADER address in ECAM space.
//please refer to PCIe spec: Enhanced Configuration Access Mechanism.
PUCHAR GetEcamCfgAddr(PSPCDIO_DEVEXT devext, UCHAR bus, UCHAR dev, UCHAR func)
{
    ECAM_OFFSET offset = { 0 };
    if (NULL == devext->EcamBase)
        return NULL;

    offset.u.Bus = bus;
    offset.u.Dev = dev;
    offset.u.Func = func;

    return devext->EcamBase + offset.AsAddr;
}
ULONG ParsePciCapSize(PCI_CAPID cap_id)
{
    //only support PCIE, MSI, MSIX in current stage.
    switch(cap_id)
    {
        case PCI_CAPID::PCIE:
            return sizeof(PCIE_CAP);
        case PCI_CAPID::MSI:
            return sizeof(PCI_MSI_CAP);
        case PCI_CAPID::MSIX:
            return sizeof(PCI_MSIX_CAP);
    }
    
    return MAXULONG;
}
bool IsBufferSizeOkForCap(ULONG buf_size, PCI_CAPID cap_id)
{
    return (buf_size >= ParsePciCapSize(cap_id));
}
PUCHAR FindCapByDevice(PSPCDIO_DEVEXT devext, PCI_CAPID cap_id, UCHAR bus_id, UCHAR dev_id, UCHAR func_id)
{
    PUCHAR header = GetEcamCfgAddr(devext, bus_id, dev_id, func_id);
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)(header + sizeof(PCI_COMMON_HEADER));
    while (NULL != cap && 0 != (UCHAR)cap_id)
    {
        if (cap->CapabilityID == (UCHAR)cap_id)
            break;

        if (0 == cap->Next)
            cap = NULL;
        else
            cap = (PPCI_CAPABILITIES_HEADER)(header + sizeof(PCI_COMMON_HEADER) + cap->Next);
    }

    return (PUCHAR)cap;
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

    if(NULL == devext->EcamBase)
    {
        ret_size = 0;
        return STATUS_ACPI_NOT_INITIALIZED;
    }
    
    READ_PCI_CAP* arg = (READ_PCI_CAP*)buffer;
    if(!IsBufferSizeOkForCap(out_size, arg->CapID))
    {
        ret_size = ParsePciCapSize(arg->CapID);
        return STATUS_INVALID_BUFFER_SIZE;
    }

    PCI_CAPID cap_id = arg->CapID;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)FindCapByDevice(
                                        devext, cap_id, bus_id, dev_id, func_id);
    if(NULL == cap)
    {
        ret_size = 0;
        return STATUS_NOT_FOUND;
    }

    ret_size = ParsePciCapSize(cap_id);
    RtlCopyMemory(buffer, cap, ret_size);

    return STATUS_SUCCESS;
}
NTSTATUS PCIeSetSlotControl(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(out_size);
    CAutoSpinLock(&devext->Lock);
    ret_size = 0;
    if (in_size < sizeof(SET_PCIE_SLOT_CONTROL))
        return STATUS_INVALID_PARAMETER;

    SET_PCIE_SLOT_CONTROL* arg = (SET_PCIE_SLOT_CONTROL*)buffer;
    PCI_CAPID cap_id = PCI_CAPID::PCIE;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)FindCapByDevice(
        devext, cap_id, bus_id, dev_id, func_id);
    if (NULL == cap)
        return STATUS_NOT_FOUND;

    PCIE_CAP *pcie = (PCIE_CAP*)cap;
    pcie->SlotCtrl.AttentionIndicator = arg->AttentionIndicator;
    pcie->SlotCtrl.PowerIndicator = arg->PowerIndicator;
    pcie->SlotCtrl.PowerControl = arg->PowerControl;

    return STATUS_SUCCESS;
}
