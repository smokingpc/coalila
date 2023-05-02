#pragma once

BOOLEAN OpenLibHandles();
void CloseLibHandles();
DWORD SendIoctl(DWORD ioctl, PVOID in_buf, DWORD in_size, PVOID out_buf, DWORD out_size, DWORD& ret_size);
