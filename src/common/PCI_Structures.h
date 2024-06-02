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
struct PCI_CAP_HEADER {
    UCHAR   CapabilityID;
    UCHAR   Next;
};

struct PCI_MSI_CAP
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
};

struct PCI_MSIX_CAP {
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
};

//PMC ==> Power Management Capabilities.
//PCI_PMC in wdm.h
struct PCI_PM_CAP {
    UCHAR       Version : 3;
    UCHAR       PMEClock : 1;
    UCHAR       Rsvd1 : 1;
    UCHAR       DeviceSpecificInitialization : 1;
    UCHAR       Rsvd2 : 2;
    struct _PM_SUPPORT {
        UCHAR   Rsvd2 : 1;
        UCHAR   D1 : 1;
        UCHAR   D2 : 1;
        UCHAR   PMED0 : 1;
        UCHAR   PMED1 : 1;
        UCHAR   PMED2 : 1;
        UCHAR   PMED3Hot : 1;
        UCHAR   PMED3Cold : 1;
    } Support;
};// PCI_PMC, * PPCI_PMC;

//PMCSR ==> Power Management Control / Status
//PCI_PMCSR in wdm.h
struct PCI_PM_CSR {
    USHORT      PowerState : 2;
    USHORT      Rsvd1 : 1;
    USHORT      NoSoftReset : 1;
    USHORT      Rsvd2 : 4;
    USHORT      PMEEnable : 1;
    USHORT      DataSelect : 4;
    USHORT      DataScale : 2;
    USHORT      PMEStatus : 1;
};// PCI_PMCSR, * PPCI_PMCSR;

//PCI_PM_B2BSE ==> PCI-PCI Bridge Support Extensions
//PCI_PMCSR_BSE in wdm.h
struct PCI_PM_B2BSE {
    UCHAR       Rsvd1 : 6;
    UCHAR       D3HotSupportsStopClock : 1;       // B2_B3#
    UCHAR       BusPowerClockControlEnabled : 1;  // BPCC_EN
};

//PCI_ADVANCED_FEATURES_CAPABILITY in ntddk.h
//it only permit when conventional PCI function integrated into a RootComplex.
struct PCI_ADV_FEATURE_CAP{
    PCI_CAP_HEADER Header;
    UCHAR Length;

    union {
        struct {
            UCHAR FunctionLevelResetSupported : 1;
            UCHAR TransactionsPendingSupported : 1;
            UCHAR Rsvd : 6;
        } u;

        UCHAR AsUCHAR;
    } Capabilities;

    union {
        struct {
            UCHAR InitiateFunctionLevelReset : 1;
            UCHAR Rsvd : 7;
        } u;

        UCHAR AsUCHAR;
    } Control;

    union {
        struct {
            UCHAR TransactionsPending : 1;
            UCHAR Rsvd : 7;
        } u;

        UCHAR AsUCHAR;
    } Status;
};

//POWMGT ==> Power Management
//PCI_PM_CAPABILITY in wdm.h
struct PCI_POWER_MGT_CAP {

    PCI_CAP_HEADER Header;

    //
    // Power Management Capabilities (Offset = 2)
    //

    union {
        PCI_PM_CAP         PowMgrCap;
        USHORT          AsUSHORT;
    } PMCAP;

    //
    // Power Management Control/Status (Offset = 4)
    //

    union {
        PCI_PM_CSR       ControlStatus;
        USHORT          AsUSHORT;
    } PMCSR;

    //
    // PMCSR PCI-PCI Bridge Support Extensions
    //

    union {
        PCI_PM_B2BSE   BridgeSupport;
        UCHAR           AsUCHAR;
    } PMCSR_BSE;

    //
    // Optional read only 8 bit Data register.  Contents controlled by
    // DataSelect and DataScale in ControlStatus.
    //

    UCHAR   Data;

} ;

// PCI to PCI Bridge Subsystem ID Capability
//PCI_SUBSYSTEM_IDS_CAPABILITY in ntddk.h
struct PCI_SUBSYS_IDS_CAP{
    PCI_CAP_HEADER Header;
    USHORT Reserved;
    USHORT SubVendorID;
    USHORT SubSystemID;
};

struct PCI_HEADER_TYPE {
    UCHAR Type : 7;
    UCHAR IsMultiFunc : 1;
};

//for pci-device
struct PCI_HEADER_TYPE_0 {
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
};

//for pci-bridge
struct PCI_HEADER_TYPE_1 {
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
};

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

    } u;
}PCIDEV_COMMON_CONFIG, * PPCIDEV_COMMON_CONFIG;

typedef struct _PCIDEV_CONFIG_SPACE : PCIDEV_COMMON_CONFIG
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