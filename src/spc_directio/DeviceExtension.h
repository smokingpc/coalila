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

typedef struct _SRAT_INFO {
    PACPI_SRAT SratTable = NULL;        //SRAT == System Resource Affinity Table
    ULONG SratCount = 0;
    PACPI_SRAT_ENTRY SratEntries = NULL;
    //todo: operator []

    NTSTATUS Setup();
    void Teardown();
}SRAT_INFO, *PSRAT_INFO;

typedef struct _MCFG_INFO {
    PMCFG_TABLE McfgTable;
    ULONG McfgCount;
    PMCFG_TABLE_ENTRY McfgEntries;

    //todo: operator []
    NTSTATUS Setup();
    void Teardown();
}MCFG_INFO, * PMCFG_INFO;

typedef struct 
{
    PDEVICE_OBJECT  DevObj;
    UNICODE_STRING  DevName;
    BOOLEAN SymLinkOk;
    UNICODE_STRING  SymbolicName;
    ULONG *AcpiTableTags;
    ULONG CpuCount;

    //Mapped SystemVA of "PCIe Enhanced Configuration Access Mechanism" BaseAddress
    PUCHAR EcamBase[MAX_PCI_SEGMENTS];
    SRAT_INFO SratInfo;
    MCFG_INFO McfgInfo;

    KSPIN_LOCK PciCfgLock;
    KSPIN_LOCK DirectIoLock;
} SPCDIO_DEVEXT, * PSPCDIO_DEVEXT;

PSPCDIO_DEVEXT SetupDevExt(PDEVICE_OBJECT device);
void TeardownDevExt(PSPCDIO_DEVEXT devext);
PVOID GetPciSpaceBySegment(PSPCDIO_DEVEXT devext, USHORT segment);

__inline NTSTATUS InitAcpiLib()
{
    return AuxKlibInitialize();
}
__inline PSPCDIO_DEVEXT GetDevExt(PDEVICE_OBJECT device)
{
    return (PSPCDIO_DEVEXT)device->DeviceExtension;
}
