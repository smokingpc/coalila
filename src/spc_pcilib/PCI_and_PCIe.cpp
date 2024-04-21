#include "Precompile.h"

INDICATOR_STATE LedStateToIndicatorState(LED_STATE state)
{
    switch (state)
    {
    case LED_STATE::OFF:
        return INDICATOR_STATE::OFF;
        break;
    case LED_STATE::ON:
        return INDICATOR_STATE::ON;
        break;
    case LED_STATE::BLINK:
        return INDICATOR_STATE::BLINK;
        break;
    }
    return INDICATOR_STATE::RESERVED;
}

DWORD ReadPCIeCap(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCIE_CAPABILITIES*result)
{
    READ_PCI_CAP request = {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::PCIE
    };
    
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCIE_CAPABILITIES), ret_size);
    return error;
}
DWORD ReadMsiCap(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSI_CAP* result)
{
    READ_PCI_CAP request = {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::MSI
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCI_MSI_CAP), ret_size);
    return error;
}
DWORD ReadMsixCap(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSIX_CAP* result)
{
    READ_PCI_CAP request = {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::MSIX
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCI_MSI_CAP), ret_size);
    return error;
}
DWORD ReadPciCfgHeader(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCIDEV_CFG_HEADER* result)
{
    READ_PCI_CFGHEADER request = 
    {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_PCIHEADER, &request, sizeof(READ_PCI_CFGHEADER), result, sizeof(PCIDEV_CFG_HEADER), ret_size);
    return error;
}
DWORD SetPCIeSlotAttentionIndicator(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, LED_STATE state)
{
    SET_PCIE_SLOT_CONTROL request = {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .Target = SLOT_CTRL_FIELD::ATT_INDICATOR,
    };
    request.u.Indicator = LedStateToIndicatorState(state);
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOT_CTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    return error;
}
DWORD SetPCIeSlotPowerIndicator(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, LED_STATE state)
{
    SET_PCIE_SLOT_CONTROL request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .Target = SLOT_CTRL_FIELD::PWR_INDICATOR,
    };
    request.u.Indicator = LedStateToIndicatorState(state);
    
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOT_CTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    return error;
}
DWORD SetPCIeSlotPowerControl(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN onoff)
{
    SET_PCIE_SLOT_CONTROL request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .Target = SLOT_CTRL_FIELD::PWR_CONTROL,
    };
    request.u.OnOff = onoff;

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOT_CTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    return error;
}
DWORD SetPCIeLinkRetrain(USHORT segment, UCHAR bus_id, UCHAR dev_id, UCHAR func_id)
{
    SET_PCIE_LINK_CONTROL request = {
        .Segment = segment,
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .Target = LINK_CTRL_FIELD::RETRAIN_LINK,
    };
    request.u.Retrain = TRUE;

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_LINK_CTRL, &request, sizeof(SET_PCIE_LINK_CONTROL), NULL, 0, ret_size);
    return error;
}
