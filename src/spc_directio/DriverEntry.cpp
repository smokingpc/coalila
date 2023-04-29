#include "Precompile.h"

EXTERN_C_START
DRIVER_INITIALIZE   DriverEntry;
DRIVER_UNLOAD       DriverUnload;
DRIVER_DISPATCH     DeviceIoControlHandler;

NTSTATUS DeviceIoControlHandler(
    _In_ struct _DEVICE_OBJECT* DeviceObject,
    _Inout_ struct _IRP* Irp
)
{
    //GET irp stack location
    //process irp mj codes

}

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{}

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    NTSTATUS status = STATUS_SUCCESS;
    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = NULL;

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceIoControlHandler;
    DriverObject->DriverUnload = DriverUnload;

    status = LoadAcpiTables();
    
    return status;
}
EXTERN_C_END

