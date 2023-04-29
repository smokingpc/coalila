#pragma once
//
// Common ACPI description table header.  This structure prefaces most ACPI tables.
//
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
} EFI_ACPI_DESCRIPTION_HEADER;

//
// MCFG Structure Definitions
//
//
// Memory Mapped Enhanced Configuration Base Address Allocation
// Structure Definition
//
typedef struct {
    UINT64  BaseAddress;
    UINT16  PciSegmentGroupNumber;
    UINT8   StartBusNumber;
    UINT8   EndBusNumber;
    UINT32  Reserved;
} EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_BASE_ADDRESS;

///
/// MCFG Table header definition.  The rest of the table 
/// must be defined in a platform specific manner.
///
typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER                       Header;
    UINT64                                            Reserved;
} EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER;


typedef struct {
    EFI_ACPI_DESCRIPTION_HEADER Header;
    UINT64 Reserved;
    EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_BASE_ADDRESS  Segment;
} EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE;

#pragma pack()