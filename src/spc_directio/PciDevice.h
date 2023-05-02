#pragma once

typedef union _ECAM_OFFSET {
    struct {
        UINT32 Reserved : 2;
        UINT32 Reg : 6;
        UINT32 ExtReg : 4;
        UINT32 Func : 3;
        UINT32 Dev : 5;
        UINT32 Bus : 8;
        UINT32 NotUsed : 4;
    } u;
    UINT32 AsAddr;
} ECAM_OFFSET, * PECAM_OFFSET;

#if 0
#if defined(_WIN64) || defined(_ARM64_)
typedef union _ECAM_ADDR {
    struct{
        UINT64 Reserved:2;
        UINT64 Reg:6;
        UINT64 ExtReg: 4;
        UINT64 Func: 3;
        UINT64 Dev : 5;
        UINT64 Bus : 8;
        UINT64 BaseAddr : 36;
    } u;
    UINT64 AsAddr;
} ECAM_ADDR, *PECAM_ADDR;
#else
typedef union _ECAM_ADDR {
    struct {
        UINT32 Reserved : 2;
        UINT32 Reg : 6;
        UINT32 ExtReg : 4;
        UINT32 Func : 3;
        UINT32 Dev : 5;
        UINT32 Bus : 8;
        UINT32 BaseAddr : 4;
    } u;
    UINT32 AsAddr;
} ECAM_ADDR, * PECAM_ADDR;
#endif
#endif
NTSTATUS ScanPciBus(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS ReadPciCap(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);
NTSTATUS PCIeSetSlotControl(PSPCDIO_DEVEXT devext, PVOID buffer, ULONG in_size, ULONG out_size, ULONG& ret_size);

PUCHAR GetPciDevCfgAddrByEcam(PSPCDIO_DEVEXT devext, UCHAR bus, UCHAR dev, UCHAR func);
