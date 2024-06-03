#include "Precompile.h"


DWORD QueryPCI(
    IN OUT PPCIDEV_COMMON_CONFIG result,
    IN USHORT segment,
    IN UCHAR bus,
    IN UCHAR dev,
    IN UCHAR func)
{
    RtlZeroMemory(result, sizeof(PCIDEV_COMMON_CONFIG));
    return ReadPciCfgHeader(segment, bus, dev, func, result);
}

void UpdatePciDevInfo(
    IN OUT PPCIDEV_INFO info, 
    IN PPCIDEV_COMMON_CONFIG config,
    IN USHORT segment,
    IN UCHAR bus,
    IN UCHAR dev,
    IN UCHAR func)
{
    info->Location.Segment = segment;
    info->Location.Bus = bus;
    info->Location.Dev = dev;
    info->Location.Func = func;
    RtlCopyMemory(&info->Config, config, sizeof(PCIDEV_COMMON_CONFIG));

    _stprintf_s(info->LocationStr, BDF_STR_LEN, BDF_FORMAT, 
                            segment, bus, dev, func);
    _stprintf_s(info->NtDeviceId, NT_DEVID_STRLEN, NT_DEVID_FORMAT,
        info->Config.VendorID, info->Config.DeviceID,
        info->Config.u.Type0.SubSystemID, info->Config.u.Type0.SubVendorID);

    //todo: enumerate device tree to get device name...
    //WCHAR Name[MAX_PATH];
}
