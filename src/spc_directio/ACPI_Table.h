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



#define MAX_ACPI_SRAT_ENTRIES   256

enum SRAT_ENTRY_TYPE : UINT8 {
    APIC_SAPIC = 0,         //Processor Local APIC/SAPIC Affinity
    MEMORY = 1,             //Memory Affinity
    X2APIC = 2,       //Processor Local x2APIC Affinity
    GICC = 3,               //Generic Interrupt Controller(GIC) CPU Interface
    GIC_ITS = 4,            //GIC Interrupt Translation Service
    GEN_INITIATOR = 5,         //Generic Initiator
    GEN_PORT = 6,           //Generic Port
};
enum SRAT_DEVHANDLE_TYPE : UINT8 {
    ACPI_HANDLE = 0,
    PCI_HANDLE = 1,
};

#pragma pack(1)

//refer to ACPI Specification: 5. ACPI Software programming model
typedef struct {
    UINT32  Signature;
    UINT32  Length;             //total size of TABLE, including following entries.
    UINT8   Revision;
    UINT8   Checksum;
    UINT8   OemId[6];
    UINT64  OemTableId;
    UINT32  OemRevision;
    UINT32  CreatorId;
    UINT32  CreatorRevision;
} ACPI_TABLE_HEADER;

#pragma region ======== MCFG TABLE ========
//refer to "PCI Firmware Specification 3.0" for MCFG Table structre details.
// Memory Mapped Enhanced Configuration Base Address Allocation
// **This table won't be changed until reboot.
typedef struct {
    UINT64  BaseAddress;
    UINT16  PciSegmentGroupNumber;
    UINT8   StartBusNumber;
    UINT8   EndBusNumber;
    UINT32  Reserved;
} ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION;

//EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE
typedef struct {
    ACPI_TABLE_HEADER Header;
    UINT64 Reserved;
    ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION  Segment[MAX_PCI_SEGMENTS];
} ACPI_MCFG_TABLE, * PACPI_MCFG_TABLE;
#pragma endregion

#pragma region ======== SRAT TABLE ========
//SRAT == Static Resource Affinity Table.
//refer detailed info in ACPI Specification: 5. ACPI Software programming model

typedef union {
    struct {
        UINT64 AcpiHID;
        UINT32 AcpiUID;
        UINT32 Reserved;
    }Acpi;

    struct {
        UINT16 Segment;
        struct {
            UINT8 Bus;
            UINT8 Dev : 5;
            UINT8 Func : 3;
        }BDF;
        UINT8 Reserved[12];
    }Pci;
}SRAT_DEVICE_HANDLE;


typedef struct _ACPI_SRAT_ENTRY {
    SRAT_ENTRY_TYPE Type;
    
    //size of derived structure. 
    //e.g. SRAT_MEMORY_AFFINITY::Length field should be sizeof(SRAT_MEMORY_AFFINITY)
    UINT8 Length;           
}ACPI_SRAT_ENTRY, *PACPI_SRAT_ENTRY;

typedef struct _SRAT_LOCAL_APIC_SAPIC_AFFINITY: ACPI_SRAT_ENTRY 
{
    UINT8 ProximityDomainLow;       //bit 0~7 of ProximityDomain ID
    UINT8 LocalApicID;
    struct {
        UINT32 Enabled : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT8 LocalSapicEID;
    UINT8 ProximityDomainHigh[3];   //bit 8~31 of ProximityDomain ID
    UINT32 ClockDomain;
}SRAT_LOCAL_APIC_AFFINITY, *PSRAT_LOCAL_APIC_AFFINITY;

typedef struct _SRAT_MEMORY_AFFINITY : ACPI_SRAT_ENTRY
{
    UINT32 ProximityDomain;
    UINT16 Reserved1;
    UINT32 BaseAddrLow;     //Low 32bit of memory range BaseAddress(Physical) 
    UINT32 BaseAddrHigh;
    UINT32 LengthLow;       //Low 32bit of memory range length (memory range always be continuous)
    UINT32 LengthHigh;
    UINT32 Reserved2;
    struct {
        UINT32 Enabled : 1;
        UINT32 HotPluggable : 1;
        UINT32 NonVolatile : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT64 Reserved3;
}SRAT_MEMORY_AFFINITY, * PSRAT_MEMORY_AFFINITY;

typedef struct _SRAT_X2APIC_AFFINITY : ACPI_SRAT_ENTRY
{
    UINT16 Reserved1;
    UINT32 ProximityDomain;
    UINT32 x2ApicID;
    struct {
        UINT32 Enabled : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT32 ClockDomain;
    UINT32 Reserved2;
}SRAT_X2APIC_AFFINITY, *PSRAT_X2APIC_AFFINITY;

typedef struct _SRAT_GICC : ACPI_SRAT_ENTRY
{
    UINT32 ProximityDomain;
    UINT32 AcpiProcessorUID;
    struct {
        UINT32 Enabled : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT32 ClockDomain;
}SRAT_GICC, * PSRAT_GICC;

typedef struct _SRAT_GIC_ITS : ACPI_SRAT_ENTRY
{
    UINT32 ProximityDomain;
    UINT16 Reserved;
    UINT32 ItsID;
}SRAT_GIC_ITS, * PSRAT_GIC_ITS;

typedef struct _SRAT_GENERIC_INITIATOR : ACPI_SRAT_ENTRY
{
    UINT8 Reserved1;
    SRAT_DEVHANDLE_TYPE DeviceHandleType;
    UINT32 ProximityDomain;
    SRAT_DEVICE_HANDLE DeviceHandle;
    struct {
        UINT32 Enabled : 1;
        UINT32 ArchitecturalTransactions : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT32 Reserved2;
}SRAT_GENERIC_INITIATOR, * PSRAT_GENERIC_INITIATOR;

typedef struct _SRAT_GENERIC_PORT : ACPI_SRAT_ENTRY
{
    UINT8 Reserved1;
    SRAT_DEVHANDLE_TYPE DeviceHandleType;
    UINT32 ProximityDomain;
    SRAT_DEVICE_HANDLE DeviceHandle;
    struct {
        UINT32 Enabled : 1;
        UINT32 ArchitecturalTransactions : 1;
        UINT32 Reserved : 31;
    } Flags;
    UINT32 Reserved2;
}SRAT_GENERIC_PORT, * PSRAT_GENERIC_PORT;

typedef struct {
    ACPI_TABLE_HEADER Header;
    UINT32 Reserved1;
    UINT64 Reserved2;

    //should be filled by SRAT_CPU_LOCAL_APIC_SAPIC_AFFINITY + SRAT_MEMORY_AFFINITY + ....etc.
    UCHAR Entries[PAGE_SIZE * 4];
}ACPI_SRAT_TABLE;

#pragma pack()
