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

#pragma region ======== PCI DEVICE CONFIG and CAPABILITY structures ========
#pragma pack(1)

//following structure definition copied from struct PCI_COMMON_CONFIG in wdm.h.
typedef struct _PCI_CAP_HEADER {
    UCHAR   CapabilityID;
    UCHAR   Next;
} PCI_CAP_HEADER, * PPCI_CAP_HEADER;

typedef struct _PCI_MSI_CAP
{
    PCI_CAP_HEADER Header;
    struct {
        UINT16 MSIE : 1;        //MSI Enable. device will use INTx pins if set 0 here.
        UINT16 MMC : 3;         //Multiple Message Capable. Indicates the number of messages the controller "wants" to assert.
        UINT16 MME : 3;         //Multiple Message Enable. How many message the soft want controller assert. vendor will imp this field as ReadOnly if doesn't support multi-msi.
        UINT16 C64 : 1;         //64 Bit Address Capable. NVMe device should set 1 here.
        UINT16 PVM : 1;         //Per-Vector Masking Capable. indicates this device support PVM or not.
        UINT16 RESERVED : 7;
    }MC;            //Message Signaled Interrupt Message Control
 
    //MA_LOW must be aligned to DWORD.
    UINT32 MA_LOW;       //30 bits of system specificed message address. always DWORD aligned.
    UINT32 MA_UP;       //Message Signaled Interrupt Upper Address

    UINT32 MD;            //Message Signaled Interrupt Message Data
    UINT32 MMASK;         //Message Signaled Interrupt Mask Bits (Optional). each bit can mask associated message.(by id)
    UINT32 MPEND;         //Message Signaled Interrupt Pending Bits (Optional) each bit indicates has pending msg(by id)
}PCI_MSI_CAP, * PPCI_MSI_CAP;

typedef struct _PCI_MSIX_CAP {
    PCI_CAP_HEADER Header;
    struct {
        UINT16 TS : 11;         //Table Size. how many MSI-X message this device support?
        UINT16 RESERVED : 3;
        UINT16 FM : 1;          //Function Mask. Block MSI-X function if set 1.
        UINT16 MXE : 1;         //MSI-X Enable. If MXE==1 and MSIE==0, this device use MSIX and block INTx pin
    }MXC;           //MSI-X Message Control

    struct {                    //MSIX table
        UINT32 TBIR : 3;        //Table BIR. refer to NVMe spec 1.3 , section 2.4.3
        UINT32 TO : 29;         //Table Offset since BusData begin address.  refer to NVMe spec 1.3 , section 2.4.3
    }MTAB;          //MSI-X Table Offset / Table BIR

    struct {                     //PBA is Pending Bit of interrupt array
        UINT32 PBIR : 3;        //PBA BIR.  refer to NVMe spec 1.3 , section 2.4.4
        UINT32 PBAO : 29;       //PBA Offset since BusData begin address.  refer to NVMe spec 1.3 , section 2.4.4
    }MPBA;          //MSI-X PBA Offset / PBA BIR


}PCI_MSIX_CAP, * PPCI_MSIX_CAP;

typedef struct _PCI_HEADER_TYPE {
    UCHAR Type : 7;
    UCHAR IsMultiFunc : 1;
}PCI_HEADER_TYPE, * PPCI_HEADER_TYPE;

//for pci-device
typedef struct _PCI_HEADER_TYPE_0 {
    ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
    ULONG   CIS;
    USHORT  SubVendorID;
    USHORT  SubSystemID;
    ULONG   ROMBaseAddress;
    UCHAR   CapabilitiesPtr;
    UCHAR   Reserved1[3];
    ULONG   Reserved2;
    UCHAR   InterruptLine;      //
    UCHAR   InterruptPin;       // (ro)
    UCHAR   MinimumGrant;       // (ro)
    UCHAR   MaximumLatency;     // (ro)
}PCI_HEADER_TYPE_0, * PPCI_HEADER_TYPE_0;

//for pci-bridge
typedef struct _PCI_HEADER_TYPE_1 {
    ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
    UCHAR   PrimaryBus;     //parent bus id
    UCHAR   SecondaryBus;   //child bus id
    UCHAR   SubordinateBus;
    UCHAR   SecondaryLatency;
    UCHAR   IOBase;
    UCHAR   IOLimit;
    USHORT  SecondaryStatus;
    USHORT  MemoryBase;
    USHORT  MemoryLimit;
    USHORT  PrefetchBase;
    USHORT  PrefetchLimit;
    ULONG   PrefetchBaseUpper32;
    ULONG   PrefetchLimitUpper32;
    USHORT  IOBaseUpper16;
    USHORT  IOLimitUpper16;
    UCHAR   CapabilitiesPtr;
    UCHAR   Reserved1[3];
    ULONG   ROMBaseAddress;
    UCHAR   InterruptLine;
    UCHAR   InterruptPin;
    USHORT  BridgeControl;
} PCI_HEADER_TYPE_1, * PPCI_HEADER_TYPE_1;

typedef struct {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    PCI_HEADER_TYPE   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        // type 0 is regular PCI and PCIe devices.
        PCI_HEADER_TYPE_0 Type0;
        // PCI to PCI Bridge
        PCI_HEADER_TYPE_1 Type1;
        //NOTE: In WDK deifnition , there is a Type2 structure for Cardbus device here.
        //According Cardbus is almost extinct, so I skip type2 definition...

    } DUMMYSTRUCTNAME;
    UCHAR   Device[192];
}PCIDEV_CFG_HEADER, * PPCIDEV_CFG_HEADER;

typedef struct _PCIDEV_CONFIG_SPACE : PCIDEV_CFG_HEADER
{
    UCHAR DeviceSpecific[192];
}PCIDEV_CONFIG_SPACE, * PPCIDEV_CONFIG_SPACE;

#pragma pack()
#pragma endregion //======== PCI DEVICE CONFIG and CAPABILITY structures ========

//SBDF stands for "Segment Bus Device Function"
typedef struct _SBDF_LOCATION {
    USHORT Segment;
    UCHAR Bus;
    UCHAR Dev;
    UCHAR Func;
}SBDF_LOCATION, *PSBDF_LOCATION;