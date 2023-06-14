#include "Precompile.h"

bool IsValidCap(PPCI_CAPABILITIES_HEADER cap)
{
    if(NULL == cap || 0 == cap->CapabilityID || 0xFF == cap->CapabilityID)
        return false;
    return true;
}

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

    PUCHAR ret = devext->EcamBase + offset.AsAddr;
    PPCI_COMMON_CONFIG cfg = (PPCI_COMMON_CONFIG) ret;
    if(cfg->VendorID == 0xffff)  //not mapped so only can read 0xFFFF(invalid value)
        return NULL;
    return ret;
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
    if(NULL == header)
        return NULL;

    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)(header + sizeof(PCI_COMMON_HEADER));
    while (IsValidCap(cap))
    {
        if (cap->CapabilityID == (UCHAR)cap_id)
            break;

        if (0 == cap->Next)
            cap = NULL;
        else
        {
            //cap->Next means "offset from HeaderBegin, not "
            cap = (PPCI_CAPABILITIES_HEADER)(header + cap->Next);
        }
    }

    return (PUCHAR)cap;
}

NTSTATUS ReadPciCfgHeader(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(out_size);

    if (in_size < sizeof(READ_PCI_CFGHEADER))
    {
        ret_size = sizeof(READ_PCI_CFGHEADER);
        return STATUS_INVALID_PARAMETER;
    }
    if (out_size < sizeof(PCIDEV_CFG_HEADER))
    {
        ret_size = sizeof(PCIDEV_CFG_HEADER);
        return STATUS_INVALID_PARAMETER;
    }

    if (NULL == devext->EcamBase)
    {
        ret_size = 0;
        return STATUS_ACPI_NOT_INITIALIZED;
    }

    READ_PCI_CFGHEADER* arg = (READ_PCI_CFGHEADER*)buffer;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;

    PCIDEV_CFG_HEADER *pcicfg = (PCIDEV_CFG_HEADER*)GetEcamCfgAddr(devext,bus_id, dev_id, func_id);
    if(NULL == pcicfg)
        return STATUS_NOT_FOUND;
    ret_size = sizeof(PCIDEV_CFG_HEADER);
    RtlCopyMemory(buffer, pcicfg, ret_size);

    return STATUS_SUCCESS;
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
    switch(arg->Target)
    {
        case SLOT_CTRL_FIELD::ATT_INDICATOR:      //Attention Indicator   (led)
            if(pcie->SlotCap.AttentionIndicator && pcie->SlotCtrl.AttentionIndicator != arg->Value)
                pcie->SlotCtrl.AttentionIndicator = arg->Value;
            break;
        case SLOT_CTRL_FIELD::PWR_INDICATOR:      //Power Indicator   (led)
            if (pcie->SlotCap.PowerIndicator && pcie->SlotCtrl.PowerIndicator != arg->Value)
                pcie->SlotCtrl.PowerIndicator = arg->Value;
            break;
        case SLOT_CTRL_FIELD::PWR_CONTROL:        //Slot Power on/off
            if (pcie->SlotCap.PowerConotroller && pcie->SlotCtrl.PowerControl != arg->Value)
                pcie->SlotCtrl.PowerControl = arg->Value;
            break;
    }

    return STATUS_SUCCESS;
}
