#pragma once

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
