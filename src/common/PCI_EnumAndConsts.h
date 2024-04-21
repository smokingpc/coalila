#pragma once
#ifndef PCI_TYPE0_ADDRESSES
#define PCI_TYPE0_ADDRESSES             6
#endif //PCI_TYPE0_ADDRESSES

#ifndef PCI_TYPE1_ADDRESSES
#define PCI_TYPE1_ADDRESSES             2
#endif //PCI_TYPE1_ADDRESSES

#ifndef PCI_TYPE2_ADDRESSES
#define PCI_TYPE2_ADDRESSES             5
#endif //PCI_TYPE2_ADDRESSES

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
