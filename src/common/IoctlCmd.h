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


#define DIRECTIO_FILE L"\\\\.\\SPC_DIRECTIO"

//DIO stands for "Direct Io"
#define DIO_DEVTYPE     0x00008001  //device type 0~32767 are used for MS. 32768~65535 are reserved for customize.

#ifndef PCI_TYPE0_ADDRESSES
#define PCI_TYPE0_ADDRESSES             6
#endif //PCI_TYPE0_ADDRESSES
#ifndef PCI_TYPE1_ADDRESSES
#define PCI_TYPE1_ADDRESSES             2
#endif //PCI_TYPE1_ADDRESSES
#ifndef PCI_TYPE2_ADDRESSES
#define PCI_TYPE2_ADDRESSES             5
#endif //PCI_TYPE2_ADDRESSES

#define DIO_READ_PORT       (100)
#define DIO_WRITE_PORT      (101)
#define DIO_READ_REG        (102)
#define DIO_WRITE_REG       (103)

//return PCIe VENID and DEVID array
#define DIO_SCAN_PCIDEV     (120)
//read specified CAPABILITY block which followed after PCIDEV_CONFIG_SPACE
#define DIO_READ_CAP        (121)
#define DIO_PCIE_SLOT_CTRL  (140)

//#define DIO_PCIE_ATTEN_IDR  (141)     //Slot Attention Indicator(LED) cmd
//#define DIO_PCIE_POWER_IDR  (142)     //Slot Power Indicator(LED) cmd
//#define DIO_PCIE_SLOT_POWER  (143)  //Slot Power Control(turn on/off slot power)

//following macro can fetch corresponding fields from IOCTL codes
//DEVICE_TYPE_FROM_CTL_CODE
//METHOD_FROM_CTL_CODE

#define IOCTL_READ_PORT     CTL_CODE(DIO_DEVTYPE, DIO_READ_PORT, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE_PORT    CTL_CODE(DIO_DEVTYPE, DIO_WRITE_PORT, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_READ_ADDR     CTL_CODE(DIO_DEVTYPE, DIO_READ_REG, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE_ADDR    CTL_CODE(DIO_DEVTYPE, DIO_WRITE_REG, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SCAN_PCIDEV   CTL_CODE(DIO_DEVTYPE, DIO_SCAN_PCIDEV, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_READ_CAP      CTL_CODE(DIO_DEVTYPE, DIO_READ_CAP, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_PCIE_SLOT_CTRL    CTL_CODE(DIO_DEVTYPE, DIO_PCIE_SLOT_CTRL, METHOD_BUFFERED, FILE_READ_ACCESS)
//#define IOCTL_PCIE_ATTEN_IDR    CTL_CODE(DIO_DEVTYPE, DIO_PCIE_ATTEN_IDR, METHOD_BUFFERED, FILE_READ_ACCESS)
//#define IOCTL_PCIE_POWER_IDR    CTL_CODE(DIO_DEVTYPE, DIO_PCIE_POWER_IDR, METHOD_BUFFERED, FILE_READ_ACCESS)
//#define IOCTL_PCIE_SLOT_POWER   CTL_CODE(DIO_DEVTYPE, DIO_PCIE_SLOT_POWER, METHOD_BUFFERED, FILE_READ_ACCESS)

//#define IOCTL_WRITE_CAP     CTL_CODE(DIO_DEVTYPE, DIO_WRITE_CAP, METHOD_BUFFERED, FILE_WRITE_ACCESS)

enum class IO_SIZE {
    UNKNOWN = 0,
    BYTE = sizeof(UCHAR),
    USHORT = sizeof(USHORT),
    ULONG = sizeof(ULONG),
    UINT64 = sizeof(UINT64)
};

//PCI Capability ID => refer to PCI Local Bus spec 3.0
enum class PCI_CAPID {
    RESERVED = 0,
    PCI_POWER_MGR = 1,
    AGP = 2,
    VPD = 3,
    SLOT_IDENT = 4, //for PciBridge
    MSI = 5,
    COMPACT_HOTSWAP = 6,    //Compact PCI HotSwap Capability
    PCIX = 7,
    HYPER_TRANSPORT = 8,
    VENDOR_SPECIFIC = 9,
    DEBUG_PORT = 0x0A,
    COMPACT_PCI_RESCTRL = 0x0B,     //Compact PCI central resource control
    PCI_HOTPLUG = 0x0C,
    PCI_BRIDGE_VENID = 0x0D,        //PCI Bridge SubSystem VendorID?
    AGP_8X = 0x0E,
    SECURE_DEVICE = 0x0F,
    PCIE = 0x10,
    MSIX = 0x11,
    SATA_CONFIG = 0x12,
    ADV_FEATURE = 0x13,
    FPB = 0x15,
};

enum class SLOT_CTRL_FIELD {
    UNKNOWN = 0,
    ATT_INDICATOR = 1,      //Attention Indicator   (led)
    PWR_INDICATOR = 2,      //Power Indicator   (led)
    PWR_CONTROL = 3,        //Slot Power on/off
};

//for IOCTL_READPORT
typedef struct {
    ULONG_PTR Address;
    IO_SIZE IoSize;
}DIRECTIO_READ, *PDIRECTIO_READ;

//for IOCTL_WRITEPORT
typedef struct {
    ULONG_PTR Address;
    IO_SIZE IoSize;
    ULONG_PTR Data;
}DIRECTIO_WRITE, * PDIRECTIO_WRITE;

//for IOCTL_READ_CAP
typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7
    PCI_CAPID CapID;
}READ_PCI_CAP;

typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7

    SLOT_CTRL_FIELD Target;
    BOOLEAN Value;
}SET_PCIE_SLOT_CONTROL;

#pragma region ======== PCI DEVICE CONFIG and CAPABILITY structures ========
#pragma pack(1)

//for IOCTL_SCAN_PCIDEV output
//this structure definition copied from struct PCI_COMMON_CONFIG in wdm.h.
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
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        // type 0 is regular PCI and PCIe devices.
        struct _PCI_HEADER_TYPE_0 {
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
        } Type0;
        // PCI to PCI Bridge
        struct _PCI_HEADER_TYPE_1 {
            ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
            UCHAR   PrimaryBus;
            UCHAR   SecondaryBus;
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
        } Type1;
        //Cardbus is almost extinct. So I skip type2 definition...
    } DUMMYSTRUCTNAME;
    UCHAR   Device[192];
}PCIDEV_CONFIG_SPACE, * PPCIDEV_CONFIG_SPACE;
#pragma pack()

#pragma endregion ======== PCI DEVICE CONFIG and CAPABILITY structures ========
