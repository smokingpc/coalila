#include "Precompile.h"

BOOLEAN ReadPCIeCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCIE_CAP *result)
{
    READ_PCI_CAP request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::PCIE
    };
    
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCIE_CAP), ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN ReadMsiCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSI_CAP* result)
{
    READ_PCI_CAP request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::MSI
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCI_MSI_CAP), ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN ReadMsixCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSIX_CAP* result)
{
    READ_PCI_CAP request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .CapID = PCI_CAPID::MSIX
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_CAP, &request, sizeof(READ_PCI_CAP), result, sizeof(PCI_MSI_CAP), ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN SetPCIeSlotAttentionIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value)
{
    SET_PCIE_SLOT_CONTROL request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .AttentionIndicator = value,
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOTCTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN SetPCIeSlotPowerIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value)
{
    SET_PCIE_SLOT_CONTROL request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .PowerIndicator = value,
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOTCTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN SetPCIeSlotPowerControl(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value)
{
    SET_PCIE_SLOT_CONTROL request = {
        .BusId = bus_id,
        .DevId = dev_id,
        .FuncId = func_id,
        .PowerControl = value,
    };

    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_PCIE_SLOTCTRL, &request, sizeof(SET_PCIE_SLOT_CONTROL), NULL, 0, ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
