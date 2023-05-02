#include "Precompile.h"

BOOLEAN DirectIoReadPort(DIRECTIO_READ *request, ULONG *out_data)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_PORT, request, sizeof(DIRECTIO_READ), out_data, sizeof(ULONG), ret_size);
    if(error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN DirectIoWritePort(DIRECTIO_WRITE* request)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_WRITE_PORT, request, sizeof(DIRECTIO_WRITE), NULL, 0, ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN DirectIoReadAddr(DIRECTIO_READ* request, ULONG_PTR* out_data)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_ADDR, request, sizeof(DIRECTIO_READ), out_data, sizeof(ULONG), ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
BOOLEAN DirectIoWriteAddr(DIRECTIO_WRITE* request)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_WRITE_ADDR, request, sizeof(DIRECTIO_WRITE), NULL, 0, ret_size);
    if (error != ERROR_SUCCESS)
        return FALSE;
    return TRUE;
}
