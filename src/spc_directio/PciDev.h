#pragma once
NTSTATUS ScanPciBus(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS ReadPciCap(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS PCIeSetSlotControl(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);

