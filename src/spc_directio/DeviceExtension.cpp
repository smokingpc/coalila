#include "Precompile.h"

PSPCDIO_DEVEXT InitDeviceExtension(PDEVICE_OBJECT device)
{
    PSPCDIO_DEVEXT devext = GetDevExt(device);
    RtlZeroMemory(devext, sizeof(SPCDIO_DEVEXT));

    devext->DevObj = device;
    RtlInitUnicodeString(&devext->DevName, DEVICE_NAME);
    RtlInitUnicodeString(&devext->SymbolicName, SYMLINK_NAME);
    return devext;
}
