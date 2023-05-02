#include "Precompile.h"

DWORD HandleError = 0;
HANDLE KernelHandle = INVALID_HANDLE_VALUE;
CRITICAL_SECTION FuncLock = {0};

BOOLEAN OpenLibHandles()
{
    InitializeCriticalSection(&FuncLock);

    //OPEN FILE
    KernelHandle = CreateFile(DIRECTIO_FILE, GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == KernelHandle)
    {
        HandleError = GetLastError();
        return FALSE;
    }

    return TRUE;
}
void CloseLibHandles()
{
    if (INVALID_HANDLE_VALUE != KernelHandle)
    {
        CloseHandle(KernelHandle);
        KernelHandle = INVALID_HANDLE_VALUE;
    }
    DeleteCriticalSection(&FuncLock);
}

DWORD SendIoctl(DWORD ioctl, PVOID in_buf, DWORD in_size, PVOID out_buf, DWORD out_size, DWORD &ret_size)
{
    if(INVALID_HANDLE_VALUE == KernelHandle)
        return HandleError;

    BOOL ok = DeviceIoControl(KernelHandle, ioctl, in_buf, in_size, out_buf, out_size, &ret_size, NULL);
    if(!ok)
        return GetLastError();
    return ERROR_SUCCESS;
}
