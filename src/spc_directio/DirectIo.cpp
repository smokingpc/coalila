#include "Precompile.h"

NTSTATUS DirectReadIoPort(
    PSPCDIO_DEVEXT devext,
    PVOID buffer, ULONG in_size,
    ULONG out_size, ULONG& ret_size)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG out_data = 0;
    CQueuedSpinLock lock(&devext->DirectIoLock);

    if(in_size < sizeof(DIRECTIO_READ))
    {
        ret_size = sizeof(sizeof(DIRECTIO_READ));
        return STATUS_INVALID_PARAMETER;
    }
    if(out_size < sizeof(out_data))
    {
        ret_size = sizeof(out_data);
        return STATUS_INVALID_BUFFER_SIZE;
    }

    PDIRECTIO_READ arg = (PDIRECTIO_READ)buffer;
    switch (arg->IoSize)
    {
    case IO_SIZE::BYTE:
        out_data = READ_PORT_UCHAR((PUCHAR)arg->Address);
        break;
    case IO_SIZE::USHORT:
        out_data = READ_PORT_USHORT((PUSHORT)arg->Address);
        break;
    case IO_SIZE::ULONG:
        out_data = READ_PORT_ULONG((PULONG)arg->Address);
        break;
    default:
        status = STATUS_NOT_SUPPORTED;
        ret_size = 0;
        break;
    }

    if(ret_size > 0)
        RtlCopyMemory(buffer, &out_data, ret_size);
    return status;
}
NTSTATUS DirectWriteIoPort(
    PSPCDIO_DEVEXT devext,
    PVOID buffer, ULONG in_size,
    ULONG out_size, ULONG& ret_size)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(out_size);
    CQueuedSpinLock lock(&devext->DirectIoLock);

    if (in_size < sizeof(DIRECTIO_WRITE))
    {
        ret_size = sizeof(DIRECTIO_WRITE);
        return STATUS_INVALID_PARAMETER;
    }
    ret_size = 0;
    PDIRECTIO_WRITE arg = (PDIRECTIO_WRITE)buffer;
    switch (arg->IoSize)
    {
    case IO_SIZE::BYTE:
        WRITE_PORT_UCHAR((PUCHAR)arg->Address, (UCHAR)arg->Data);
        break;
    case IO_SIZE::USHORT:
        WRITE_PORT_USHORT((PUSHORT)arg->Address, (USHORT)arg->Data);
        break;
    case IO_SIZE::ULONG:
        WRITE_PORT_ULONG((PULONG)arg->Address, (ULONG)arg->Data);
        break;
    default:
        status = STATUS_NOT_SUPPORTED;
        break;
    }

    return status;
}
NTSTATUS DirectReadRegister(
    PSPCDIO_DEVEXT devext,
    PVOID buffer, ULONG in_size,
    ULONG out_size, ULONG& ret_size)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG64 out_data = 0;
    CQueuedSpinLock lock(&devext->DirectIoLock);

    if (in_size < sizeof(DIRECTIO_READ))
    {
        ret_size = sizeof(sizeof(DIRECTIO_READ));
        return STATUS_INVALID_PARAMETER;
    }
    if (out_size < sizeof(out_data))
    {
        ret_size = sizeof(out_data);
        return STATUS_INVALID_BUFFER_SIZE;
    }

    PDIRECTIO_READ arg = (PDIRECTIO_READ)buffer;
    switch (arg->IoSize)
    {
    case IO_SIZE::BYTE:
        out_data = READ_REGISTER_UCHAR((PUCHAR)arg->Address);
        break;
    case IO_SIZE::USHORT:
        out_data = READ_REGISTER_USHORT((PUSHORT)arg->Address);
        break;
    case IO_SIZE::ULONG:
        out_data = READ_REGISTER_ULONG((PULONG)arg->Address);
        break;
#if defined(_WIN64) || defined(_ARM64_)
    case IO_SIZE::UINT64:
        out_data = READ_REGISTER_ULONG64((PULONG64)arg->Address);
        break;
#endif 
    default:
        status = STATUS_NOT_SUPPORTED;
        ret_size = 0;
        break;
    }

    if (ret_size > 0)
        RtlCopyMemory(buffer, &out_data, ret_size);
    return status;
}
NTSTATUS DirectWriteRegister(
    PSPCDIO_DEVEXT devext,
    PVOID buffer, ULONG in_size,
    ULONG out_size, ULONG& ret_size)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(out_size);
    CQueuedSpinLock lock(&devext->DirectIoLock);

    if (in_size < sizeof(DIRECTIO_WRITE))
    {
        ret_size = sizeof(DIRECTIO_WRITE);
        return STATUS_INVALID_PARAMETER;
    }
    ret_size = 0;
    PDIRECTIO_WRITE arg = (PDIRECTIO_WRITE)buffer;
    switch (arg->IoSize)
    {
    case IO_SIZE::BYTE:
        WRITE_REGISTER_UCHAR((PUCHAR)arg->Address, (UCHAR)arg->Data);
        break;
    case IO_SIZE::USHORT:
        WRITE_REGISTER_USHORT((PUSHORT)arg->Address, (USHORT)arg->Data);
        break;
    case IO_SIZE::ULONG:
        WRITE_REGISTER_ULONG((PULONG)arg->Address, (ULONG)arg->Data);
        break;
#if defined(_WIN64) || defined(_ARM64_)
    case IO_SIZE::UINT64:
        WRITE_REGISTER_ULONG64((PULONG64)arg->Address, (ULONG64)arg->Data);
        break;
#endif 
    default:
        status = STATUS_NOT_SUPPORTED;
        break;
    }

    return status;
}
