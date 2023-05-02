#include "Precompile.h"
DRIVER_UNLOAD       DriverUnload;

NTSTATUS CreateDevice(_In_ PDRIVER_OBJECT driver)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG size = sizeof(SPCDIO_DEVEXT);
    PDEVICE_OBJECT device = NULL;
    UNICODE_STRING devname = {0};
    PSPCDIO_DEVEXT devext = NULL;
    RtlInitUnicodeString(&devname, DEVICE_NAME);

    //Set "Exclusive Access" to this device object....
    status = IoCreateDevice(
                    driver, size, &devname, 
                    DIO_DEVTYPE, 0, TRUE, &device);

    if (!NT_SUCCESS(status))
        return status;

    device->Flags &= ~DO_DEVICE_INITIALIZING;
    device->Flags |= DO_BUFFERED_IO;
    devext = InitDeviceExtension(device);
    status = IoCreateSymbolicLink(&devext->SymbolicName, &devname);
    
    if (!NT_SUCCESS(status))
        return status;

    status = SetupAcpiInfo(devext);

    //will system call DriverUnload when return error?

    return status;
}
void DeleteDevice(_In_ PDEVICE_OBJECT device)
{
    PSPCDIO_DEVEXT devext = GetDevExt(device);
    TeardownAcpiInfo(devext);
    IoDeleteSymbolicLink(&devext->SymbolicName);
    IoDeleteDevice(device);
}

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    //Device with same devname will failed to create.
    //So I only need to delete one device object. 
    //No need to iterate entire list of device objects.
    DeleteDevice(DriverObject->DeviceObject);
}

EXTERN_C_START
DRIVER_INITIALIZE   DriverEntry;

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status = STATUS_SUCCESS;

    DriverObject->DriverUnload = DriverUnload;
    InitIrpHandlers(DriverObject);
    status = CreateDevice(DriverObject);

    return status;
}
EXTERN_C_END
