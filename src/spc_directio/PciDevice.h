#pragma once
// ================================================================
// Coalila : OpenSource PCI util for windows with ACPI only.
// Author : Roy Wang(SmokingPC).
// Licensed by MIT License.
// 
// Copyright (C) 2023, Roy Wang (SmokingPC)
// https://github.com/smokingpc/
// 
// Contact Me : smokingpc@gmail.com
// ================================================================
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this softwareand associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and /or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.
// ================================================================
// [Additional Statement]
// This Driver is implemented by PCI 3.0 and PCIe 5.0 spec.
// You can copy, modify, redistribute the source code. 
// 
// There is only one requirement to use this source code:
// PLEASE DO NOT remove or modify my name of this codes.
// 
// Enjoy it.
// ================================================================


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
