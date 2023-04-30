#pragma once
NTSTATUS DirectReadIoPort(PVOID buffer, ULONG in_size, ULONG out_size, ULONG &ret_size);
NTSTATUS DirectWriteIoPort(PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS DirectReadRegister(PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS DirectWriteRegister(PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);

