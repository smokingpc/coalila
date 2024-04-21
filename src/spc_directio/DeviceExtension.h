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

typedef struct
{
    PDEVICE_OBJECT  DevObj;
    UNICODE_STRING  DevName;
    BOOLEAN SymLinkOk;
    UNICODE_STRING  SymbolicName;
    ACPI_MCFG_TABLE McfgTable;
    ULONG AcpiTableTags[MAX_ACPI_TABLE_TAGS];

    ULONG CpuCount;

    //Mapped SystemVA of "PCIe Enhanced Configuration Access Mechanism" BaseAddress
    PUCHAR EcamBase[MAX_PCI_SEGMENTS];
    ACPI_SRAT_TABLE SratTable;
    SRAT_MEMORY_AFFINITY SratMemoryAffinity[DEFAULT_SRAT_ENTRIES];
    PSRAT_LOCAL_APIC_AFFINITY SratLocalApicAffinity;
    PSRAT_X2APIC_AFFINITY SratX2ApicAffinity;
    KSPIN_LOCK PciCfgLock;
    KSPIN_LOCK DirectIoLock;
} SPCDIO_DEVEXT, * PSPCDIO_DEVEXT;

PSPCDIO_DEVEXT SetupDevExt(PDEVICE_OBJECT device);
void TeardownDevExt(PSPCDIO_DEVEXT devext);

__inline PSPCDIO_DEVEXT GetDevExt(PDEVICE_OBJECT device)
{
    return (PSPCDIO_DEVEXT)device->DeviceExtension;
}
