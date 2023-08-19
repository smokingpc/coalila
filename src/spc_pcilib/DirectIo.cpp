#include "Precompile.h"

DWORD DirectIoReadPort(DIRECTIO_READ *request, ULONG *out_data)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_PORT, request, sizeof(DIRECTIO_READ), out_data, sizeof(ULONG), ret_size);
    return error;
}
DWORD DirectIoWritePort(DIRECTIO_WRITE* request)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_WRITE_PORT, request, sizeof(DIRECTIO_WRITE), NULL, 0, ret_size);
    return error;
}
DWORD DirectIoReadAddr(DIRECTIO_READ* request, ULONG_PTR* out_data)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_READ_ADDR, request, sizeof(DIRECTIO_READ), out_data, sizeof(ULONG), ret_size);
    return error;
}
DWORD DirectIoWriteAddr(DIRECTIO_WRITE* request)
{
    DWORD error = 0;
    DWORD ret_size = 0;
    error = SendIoctl(IOCTL_WRITE_ADDR, request, sizeof(DIRECTIO_WRITE), NULL, 0, ret_size);
    return error;
}
