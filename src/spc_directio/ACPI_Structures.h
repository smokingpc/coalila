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

#pragma pack(1)
typedef struct {
    UINT32  Signature;
    UINT32  Length;
    UINT8   Revision;
    UINT8   Checksum;
    UINT8   OemId[6];
    UINT64  OemTableId;
    UINT32  OemRevision;
    UINT32  CreatorId;
    UINT32  CreatorRevision;
} ACPI_DESCRIPTION_HEADER;

// Memory Mapped Enhanced Configuration Base Address Allocation
// **This table won't be changed before reboot
typedef struct {
    UINT64  BaseAddress;
    UINT16  PciSegmentGroupNumber;
    UINT8   StartBusNumber;
    UINT8   EndBusNumber;
    UINT32  Reserved;
} ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION;

//EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE
typedef struct {
    ACPI_DESCRIPTION_HEADER Header;
    UINT64 Reserved;
    ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION  Segment[MAX_ACPI_SEGMENT];
} ACPI_MCFG_TABLE, * PACPI_MCFG_TABLE;

#pragma pack()
