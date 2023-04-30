#pragma once

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
    ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION  Segment;
} ACPI_MCFG_TABLE, * PACPI_MCFG_TABLE;

#pragma pack()
