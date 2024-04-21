#pragma once

enum class PCIE_CAPID {
    AER = 0x0001,  //Advanced Error Reporting
    VIRTUAL_CHANNEL = 0x0002,
    DEVICE_SN = 0x0003,      //device serial number
    POWER_BUDGETING = 0x0004,
    RC_LINK_DECLARATION = 0x0005,
    RC_INTERNAL_LINK_CONTROL = 0x0006,
    RCEC_EP_ASSOC = 0x0007,         //Root Complex Event Collector Endpoint Association
    MFVC = 0x0008,
    VC_AND_MFVC = 0x0009,
    RCRB_HEADER = 0x000A,
    VENDOR_SPECIFIC = 0x000B,
    CONFIGURATION_ACCESS_CORRELATION = 0x000C,
    ACS = 0x000D,       //Access Control Service
    ARI = 0x000E,
    ATS = 0x000F,
    SRIOV = 0x0010,     //Single Root IO Virtualization
    MRIOV = 0x0011,     //Multi Root IO Virtualization
    MULTICAST = 0x0012,
    PAGE_REQUEST = 0x0013,
    RESERVED_FOR_AMD = 0x0014,
    RESIZABLE_BAR = 0x0015,
    DPA = 0x0016,
    TPH_REQUESTER = 0x0017,
    LTR = 0x0018,
    SECONDARY_PCIE = 0x0019,
    PMUX = 0x001A,
    PASID = 0x001B,
    LN_REQUESTER = 0x001C,
    DPC = 0x001D,
    L1_PM_SS = 0x001E,
    PTM = 0x001F,
    MPCIE = 0x0020,
    FRS_QUEUEING = 0x0021,
    READINESS_TIME_REPORTING = 0x0022,
    DESIGNATED_VENDOR_SPECIFIC = 0x0023,
    NPEM = 0x0029,
};

//PCI_EXPRESS_MAX_PAYLOAD_SIZE in ntddk.h
enum class PCIE_MAX_PAYLOAD_SIZE{
    MaxPayload128Bytes = 0,
    MaxPayload256Bytes,
    MaxPayload512Bytes,
    MaxPayload1024Bytes,
    MaxPayload2048Bytes,
    MaxPayload4096Bytes
};

//PCI_EXPRESS_DEVICE_TYPE in ntddk.h
enum class PCIE_DEVTYPE{
    Endpoint = 0,
    LegacyEndpoint,
    RootPort = 4,
    UpstreamSwitchPort,
    DownstreamSwitchPort,
    PcieToPciXBridge,
    PciXToPcieBridge,
    RCIntegratedEndpoint,   //RC stands for RootComplex
    RCEventCollector        //RC stands for RootComplex
};

//PCI_EXPRESS_INDICATOR_STATE in ntddk.h
enum class INDICATOR_STATE {
    RESERVED = 0,
    ON = 1,
    BLINK = 2,
    OFF = 3,
    MAX = 4
};

//PCI_EXPRESS_ASPM_CONTROL in ntddk.h
enum class ASPM_CTRL {
    DISABLE = 0,
    L0s = 1,
    L1 = 2,
    L0s_AND_L1 = 3,
};

//PCI_EXPRESS_ASPM_SUPPORT in ntddk.h
enum class ASPM_SUPPORT {
    NoAspm = 0,
    L0sSupport = 1,
    L1Support = 2,
    L0sAndL1Support = 3
};

// PCI_EXPRESS_L0s_EXIT_LATENCY in ntddk.h
enum class L0s_EXIT_LATENCY {
    L0s_Below64ns = 0,
    L0s_64ns_128ns,
    L0s_128ns_256ns,
    L0s_256ns_512ns,
    L0s_512ns_1us,
    L0s_1us_2us,
    L0s_2us_4us,
    L0s_Above4us
};

// PCI_EXPRESS_L1_EXIT_LATENCY in ntddk.h
enum class L1_EXIT_LATENCY{
    L1_Below1us = 0,
    L1_1us_2us,
    L1_2us_4us,
    L1_4us_8us,
    L1_8us_16us,
    L1_16us_32us,
    L1_32us_64us,
    L1_Above64us
};

// PCI_EXPRESS_POWER_STATE in ntddk.h
enum class PCIE_POWER_STATE {
    PowerOn = 0,
    PowerOff
};
