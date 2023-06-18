#include "Precompile.h"

//__inline void DefaultIoctlHandler(PIRP irp, PIO_STACK_LOCATION stack)
__inline NTSTATUS DefaultIoctlHandler(PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size)
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(in_size);
    UNREFERENCED_PARAMETER(out_size);
    ret_size = 0;
    return STATUS_INVALID_DEVICE_REQUEST;
}

void InitIrpHandlers(_In_ PDRIVER_OBJECT driver)
{
    for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
        driver->MajorFunction[i] = IrpDefaultHandler;

    driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceIoControlHandler;
    driver->MajorFunction[IRP_MJ_CREATE] = IrpCreateCloseHandler;
    driver->MajorFunction[IRP_MJ_CLOSE] = IrpCreateCloseHandler;
}

NTSTATUS DeviceIoControlHandler(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PIO_STACK_LOCATION stack = NULL;
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PVOID buffer = NULL;
    ULONG in_size = 0;
    ULONG out_size = 0;
    ULONG ret_info = 0;
    ULONG ioctl = 0;
    PSPCDIO_DEVEXT devext = GetDevExt(DeviceObject);

    //We are going to handle this IRP. Related I/O information are stored 
    //in IO_STACK_LOCATION array. So pick current location(current element 
    //of this array) then prepare to parse it.
    stack = IoGetCurrentIrpStackLocation(Irp);

    //Parse the input data from IRP and IO_STACK_LOCATION
    //BUFFERRED_IO put input buffer in Irp->AssociatedIrp.SystemBuffer
    //DIRECT_IO put input buffer in Irp->MdlAddress
    //NEITHER_IO put input buffer in stack->Parameters.XXX.Type3InputBuffer
    ioctl = stack->Parameters.DeviceIoControl.IoControlCode;
    buffer = Irp->AssociatedIrp.SystemBuffer;
    in_size = stack->Parameters.DeviceIoControl.InputBufferLength;
    out_size = stack->Parameters.DeviceIoControl.OutputBufferLength;

    switch(ioctl)
    {
    case IOCTL_READ_PORT:
        status = DirectReadIoPort(buffer, in_size, out_size, ret_info);
        break;
    case IOCTL_WRITE_PORT:
        status = DirectWriteIoPort(buffer, in_size, out_size, ret_info);
        break;
    case IOCTL_READ_ADDR:
        status = DirectReadRegister(buffer, in_size, out_size, ret_info);
        break;
    case IOCTL_WRITE_ADDR:
        status = DirectWriteRegister(buffer, in_size, out_size, ret_info);
        break;
    case IOCTL_READ_PCIHEADER:
        status = ReadPciCfgHeader(devext, buffer, in_size, out_size, ret_info);
        break;
    case IOCTL_READ_CAP:
        status = ReadPciCap(devext, buffer, in_size, out_size, ret_info);
        break;
    //case IOCTL_PCIE_ATTEN_IDR:
    //case IOCTL_PCIE_POWER_IDR:
    //case IOCTL_PCIE_SLOT_POWER:
    case IOCTL_PCIE_SLOT_CTRL:
        status = PCIeSetSlotControl(devext, buffer, in_size, out_size, ret_info);
        break;
    default:
        status = DefaultIoctlHandler(buffer, in_size, out_size, ret_info);
        break;
    }

    //Irp->IoStatus.Status is for higher level driver.
    //if this request comes from usermode app, we should return status.
    //usermode request will not retrieve Irp->IoStatus.Status, 
    //it only check return of this dispatch function.
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = ret_info;

    //This request is handled and no necessary to pass to other driver.
    //So we have to tell IoManager : "this I/O request finished. Please feedback to caller and clean up the irp"
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;  //tell I/O Manager : this request dispatching completed.
}

NTSTATUS IrpDefaultHandler(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    PIO_STACK_LOCATION stack = NULL;

    //We are going to handle this IRP. Related I/O information are stored in IO_STACK_LOCATION array.
    //So pick current location(current element of this array) then prepare to parse it.
    stack = IoGetCurrentIrpStackLocation(Irp);

    //Remember to set this status code.
    //This status code will return back to caller.
    //Win32 subsystem will translate status code to Win32 Error Code.    
    Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
    Irp->IoStatus.Information = 0;
    //This request is handled and no necessary to pass to other driver.
    //So we have to tell IoManager : "this I/O request finished. Please feedback to caller and clean up the irp"
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;  //tell I/O Manager : this request dispatching completed.
}

NTSTATUS IrpCreateCloseHandler(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    PIO_STACK_LOCATION stack = NULL;
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

    //We are going to handle this IRP. Related I/O information are stored in IO_STACK_LOCATION array.
    //So pick current location(current element of this array) then prepare to parse it.
    stack = IoGetCurrentIrpStackLocation(Irp);


    switch (stack->MajorFunction)
    {
        //IRP_MJ_CREATE will be called by CreateFile();
        //IRP_MJ_CLOSE will be called by CloseHandle();
    case IRP_MJ_CREATE:
    case IRP_MJ_CLOSE:
        status = STATUS_SUCCESS;
        break;
    }

    //Remember to set this status code.
    //This status code will return back to caller.
    //Win32 subsystem will translate status code to Win32 Error Code.    
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;

    //This request is handled and no necessary to pass to other driver.
    //So we have to tell IoManager : "this I/O request finished. Please feedback to caller and clean up the irp"
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;  //tell I/O Manager : this request dispatching completed.
}
