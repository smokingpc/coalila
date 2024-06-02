#include "Precompile.h"

static __inline bool IsValidVendorID(PPCI_COMMON_CONFIG cfg)
{
    //not mapped PCI HEADER so only can read 0xFFFF or 0(invalid value)
    return !(0xffff == cfg->VendorID || 0 == cfg->VendorID);
}
static __inline ULONG ParsePciCapSize(PCI_CAPID cap_id)
{
    //only support PCIE, MSI, MSIX in current stage.
    switch (cap_id)
    {
    case PCI_CAPID::PCIE:
        return sizeof(PCIE_CAPABILITIES);
    case PCI_CAPID::MSI:
        return sizeof(PCI_MSI_CAP);
    case PCI_CAPID::MSIX:
        return sizeof(PCI_MSIX_CAP);
    }

    return MAXULONG;
}
static __inline bool IsBufferSizeOkForCap(ULONG buf_size, PCI_CAPID cap_id)
{
    return (buf_size >= ParsePciCapSize(cap_id));
}
static bool IsValidCap(PPCI_CAPABILITIES_HEADER cap)
{
    if(NULL == cap || 0 == cap->CapabilityID || 0xFF == cap->CapabilityID)
        return false;
    return true;
}
static bool IsValidIndicatorState(INDICATOR_STATE state)
{
    return ((state != INDICATOR_STATE::RESERVED) && (state != INDICATOR_STATE::MAX));
}
#if 0
PUCHAR GetEcamCfgAddr(PSPCDIO_DEVEXT devext, USHORT segment, UCHAR bus, UCHAR dev, UCHAR func)
{
    ECAM_OFFSET offset = { 0 };
    if (NULL == devext->EcamBase)
        return NULL;

    offset.u.Bus = bus;
    offset.u.Dev = dev;
    offset.u.Func = func;

    PUCHAR ret = devext->EcamBase[segment] + offset.AsAddr;
    PPCI_COMMON_CONFIG cfg = (PPCI_COMMON_CONFIG)ret;
    if (!IsValidVendorID(cfg))
        return NULL;
    return ret;
}
#endif
//this function generates PCI_COMMON_HEADER address in ECAM space.
//please refer to PCIe spec: Enhanced Configuration Access Mechanism.
static PUCHAR GetPciCfgSpace(PVOID seg_base, UCHAR bus, UCHAR dev, UCHAR func)
{
    ECAM_OFFSET offset = { 0 };
    offset.u.Bus = bus;
    offset.u.Dev = dev;
    offset.u.Func = func;

    PUCHAR ret = ((PUCHAR)seg_base) + offset.AsAddr;
    PPCI_COMMON_CONFIG cfg = (PPCI_COMMON_CONFIG) ret;
    if(!IsValidVendorID(cfg))
        return NULL;
    return ret;
}
PUCHAR FindCapByDevice(
    PSPCDIO_DEVEXT devext, 
    PCI_CAPID cap_id, 
    USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id)
{
    PVOID seg_base = GetPciSpaceBySegment(devext, segment);
    if(NULL == seg_base)
        return NULL;

    PUCHAR header = GetPciCfgSpace(seg_base, bus_id, dev_id, func_id);
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
NTSTATUS ReadPciCfgHeader(
    PSPCDIO_DEVEXT devext, 
    PVOID buffer, ULONG in_size, 
    ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(devext);
    UNREFERENCED_PARAMETER(buffer);

    if (NULL == devext->EcamBase)
        return STATUS_ACPI_NOT_INITIALIZED;
    if (NULL == buffer || 
        (in_size < sizeof(READ_PCI_CFGHEADER)) ||
        (out_size < sizeof(PCIDEV_COMMON_CONFIG)))
    {
        return STATUS_INVALID_PARAMETER;
    }

    READ_PCI_CFGHEADER* arg = (READ_PCI_CFGHEADER*)buffer;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;
    USHORT segment = arg->Segment;
    PVOID seg_base = GetPciSpaceBySegment(devext, segment);
    if (NULL == seg_base)
        return STATUS_NOT_FOUND;

    PCIDEV_COMMON_CONFIG* pcicfg = (PCIDEV_COMMON_CONFIG*)
            GetPciCfgSpace(seg_base, bus_id, dev_id, func_id);
    if (NULL == pcicfg)
        return STATUS_NOT_FOUND;

    ret_size = sizeof(PCIDEV_COMMON_CONFIG);
    RtlCopyMemory(buffer, pcicfg, ret_size);

    return STATUS_SUCCESS;
}
NTSTATUS ReadPciCap(
    PSPCDIO_DEVEXT devext, 
    PVOID buffer, ULONG in_size, 
    ULONG out_size, ULONG& ret_size)
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
    USHORT segment = arg->Segment;
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)
                FindCapByDevice(devext, cap_id, segment, 
                                    bus_id, dev_id, func_id);
    if(NULL == cap)
    {
        ret_size = 0;
        return STATUS_NOT_FOUND;
    }

    ret_size = ParsePciCapSize(cap_id);
    RtlCopyMemory(buffer, cap, ret_size);

    return STATUS_SUCCESS;
}
NTSTATUS PCIeSetSlotControl(
    PSPCDIO_DEVEXT devext, 
    PVOID buffer, ULONG in_size, 
    ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(out_size);
    CSpinLock lock(&devext->PciCfgLock);
    ret_size = 0;
    if (in_size < sizeof(SET_PCIE_SLOT_CONTROL))
        return STATUS_INVALID_PARAMETER;

    SET_PCIE_SLOT_CONTROL* arg = (SET_PCIE_SLOT_CONTROL*)buffer;
    PCI_CAPID cap_id = PCI_CAPID::PCIE;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;
    USHORT segment = arg->Segment;
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)
        FindCapByDevice(devext, cap_id, segment,
            bus_id, dev_id, func_id);
    if (NULL == cap)
        return STATUS_NOT_FOUND;

    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PPCI_EXPRESS_CAPABILITY pcie = (PPCI_EXPRESS_CAPABILITY)cap;
    INDICATOR_STATE state = INDICATOR_STATE::RESERVED;
    switch(arg->Target)
    {
        case SLOT_CTRL_FIELD::ATT_INDICATOR:      //Attention Indicator   (led)
            state = arg->u.Indicator;
            if (!pcie->SlotCapabilities.AttentionIndicatorPresent)
                status = STATUS_NOT_SUPPORTED;
            else if (!IsValidIndicatorState(state))
            {
                status = STATUS_INVALID_PARAMETER;
            }
            else
            {
                pcie->SlotControl.AttentionIndicatorControl = (UINT16)state;
                status = STATUS_SUCCESS;
            }
            break;
        case SLOT_CTRL_FIELD::PWR_INDICATOR:      //Power Indicator   (led)
            state = arg->u.Indicator;
            if (!pcie->SlotCapabilities.PowerIndicatorPresent)
                status = STATUS_NOT_SUPPORTED;
            else if (!IsValidIndicatorState(state))
            {
                status = STATUS_INVALID_PARAMETER;
            }
            else
            {
                pcie->SlotControl.PowerIndicatorControl = (UINT16)state;
                status = STATUS_SUCCESS;
            }
            break;
        case SLOT_CTRL_FIELD::PWR_CONTROL:        //Slot Power on/off
            if (!pcie->SlotCapabilities.PowerControllerPresent)
                status = STATUS_NOT_SUPPORTED;
            else
            {
            //refer to PCIe spec v5.0
            //In PowerControl, 0 indicates "ON".
                pcie->SlotControl.PowerControllerControl = !arg->u.OnOff;
                status = STATUS_SUCCESS;
            }
            break;
    }

    return status;
}
NTSTATUS PCIeSetLinkControl(
    PSPCDIO_DEVEXT devext, 
    PVOID buffer, ULONG in_size, 
    ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(out_size);
    CSpinLock lock(&devext->PciCfgLock);
    ret_size = 0;
    if (in_size < sizeof(SET_PCIE_LINK_CONTROL))
        return STATUS_INVALID_PARAMETER;

    SET_PCIE_LINK_CONTROL* arg = (SET_PCIE_LINK_CONTROL*)buffer;
    PCI_CAPID cap_id = PCI_CAPID::PCIE;
    UCHAR bus_id = arg->BusId;
    UCHAR dev_id = arg->DevId;
    UCHAR func_id = arg->FuncId;
    USHORT segment = arg->Segment;
    PPCI_CAPABILITIES_HEADER cap = (PPCI_CAPABILITIES_HEADER)
        FindCapByDevice(devext, cap_id, segment,
            bus_id, dev_id, func_id);
    if (NULL == cap)
        return STATUS_NOT_FOUND;

    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PPCI_EXPRESS_CAPABILITY pcie = (PPCI_EXPRESS_CAPABILITY)cap;
    switch (arg->Target)
    {
    case LINK_CTRL_FIELD::ASPM:
        status = STATUS_NOT_IMPLEMENTED;
        break;
    case LINK_CTRL_FIELD::LINK_DISABLE:
        pcie->LinkControl.LinkDisable = arg->u.LinkDisable;
        status = STATUS_SUCCESS;
        break;
    case LINK_CTRL_FIELD::RETRAIN_LINK:
        pcie->LinkControl.RetrainLink = arg->u.Retrain;
        status = STATUS_SUCCESS;
        break;
    }

    return status;
}
