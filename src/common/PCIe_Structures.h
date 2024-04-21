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
    USHORT CapabilityVersion : 4;       //PCIe cap structure version (0x02)
    USHORT DeviceType : 4;              //PCI_EXPRESS_DEVICE_TYPE
    USHORT SlotImplemented : 1;         //Slot Implemented (not applicable for PCIe endpoint device.)
    USHORT InterruptMessageNumber : 5;  //??
    USHORT Rsvd : 2;
}PCIE_CAPABILITY_REG;

//PCI_EXPRESS_DEVICE_CAPABILITIES_REGISTER in ntddk.h
typedef struct {
    ULONG MaxPayloadSizeSupported : 3;     // EXPRESS_MAX_PAYLOAD_SIZE
    ULONG PhantomFunctionsSupported : 2;
    ULONG ExtendedTagSupported : 1;
    ULONG L0sAcceptableLatency : 3;        // EXPRESS_L0S_LATENCY
    ULONG L1AcceptableLatency : 3;         // EXPRESS_L1_LATENCY
    ULONG Undefined : 3;
    ULONG RoleBasedErrorReporting : 1;
    UINT32 ErrCorSubclassCap : 1;
    UINT32 Reserved : 1;
    ULONG CapturedSlotPowerLimit : 8;
    ULONG CapturedSlotPowerLimitScale : 2;
    ULONG FunctionLevelResetCapability : 1;
    UINT32 Reserved2 : 3;
} PCIE_DEVICE_CAPABILITY_REG;

//PCI_EXPRESS_DEVICE_CONTROL_REGISTER in ntddk.h
typedef union {
    struct{
        USHORT CorrectableErrorEnable : 1;
        USHORT NonFatalErrorEnable : 1;
        USHORT FatalErrorEnable : 1;
        USHORT UnsupportedRequestErrorEnable : 1;
        USHORT EnableRelaxedOrder : 1;
        USHORT MaxPayloadSize : 3;                 // EXPRESS_MAX_PAYLOAD_SIZE
        USHORT ExtendedTagEnable : 1;
        USHORT PhantomFunctionsEnable : 1;
        USHORT AuxPowerEnable : 1;
        USHORT NoSnoopEnable : 1;
        USHORT MaxReadRequestSize : 3;             // EXPRESS_MAX_PAYLOAD_SIZE
        USHORT BCR_IFLR : 1;        //this field controls Bridge Config Retry or Initiate Function Level Reset.
    } u1;

    struct {
        USHORT : 15;
        USHORT InitiateFunctionLevelReset : 1;
    } u2;
}PCIE_DEVICE_CONTROL_REG;

//PCI_EXPRESS_DEVICE_STATUS_REGISTER in ntddk.h
typedef struct {
    USHORT CorrectableErrorDetected : 1;
    USHORT NonFatalErrorDetected : 1;
    USHORT FatalErrorDetected : 1;
    USHORT UnsupportedRequestDetected : 1;
    USHORT AuxPowerDetected : 1;
    USHORT TransactionsPending : 1;
    USHORT EmergencyPowerReduction : 1;
    USHORT Reserved : 9;
}PCIE_DEVICE_STATUS_REG;

//PCI_EXPRESS_LINK_CAPABILITIES_REGISTER in ntddk.h
typedef struct {
    UINT32 MaxLinkSpeed : 4;
    UINT32 MaxLinkWidth : 6;
    UINT32 ASPMSupport : 2;
    UINT32 L0ExitLatency : 3;
    UINT32 L1ExitLatency : 3;
    UINT32 ClockPowerManagement : 1;
    UINT32 SurpriseDownErrorReport : 1;
    UINT32 DLL_LinkActiveReport : 1;        //Data Link Layer Link Active Reporting
    UINT32 LinkBandwidthNotification : 1;
    UINT32 ASPMOptionalCompliance : 1;
    UINT32 Reserved : 1;
    UINT32 PortNumber : 8;
} PCIE_LINK_CAPABILITY_REG;

//PCI_EXPRESS_LINK_CONTROL_REGISTER in ntddk.h
typedef struct {
    USHORT ASPMControl : 2;
    USHORT Reserved : 1;
    USHORT ReadCompletionBoundary : 1;
    USHORT LinkDisable : 1;
    USHORT RetrainLink : 1;
    USHORT CommonClockConfig : 1;
    USHORT ExtendedSync : 1;
    USHORT ClockPowerManagement : 1;
    USHORT HwAutonomousWidthDisable : 1;
    USHORT LinkBandwidthMgmtInterrupt : 1;
    USHORT LinkAutoBandwidthInterrupt : 1;  //Link Autonomous Bandwidth Interrupt
    USHORT Reserved2 : 2;
    USHORT DRSSignalControl : 2;
} PCIE_LINK_CONTROL_REG;

//PCI_EXPRESS_LINK_STATUS_REGISTER in ntddk.h
typedef struct {
    USHORT LinkSpeed : 4;       //current link speed
    USHORT LinkWidth : 6;       //negotiated link width
    USHORT Undefined : 1;
    USHORT LinkTraining : 1;
    USHORT SlotClockConfig : 1;
    USHORT DataLinkLayerLinkActive : 1;
    //USHORT Reserved : 2;
    USHORT LinkBandwidthMgmt : 1;       //Link Bandwidth Management status
    USHORT LinkAutoBandwidth : 1;       //Link Autonomous Bandwidth status
} PCIE_LINK_STATUS_REG;

//PCI_EXPRESS_SLOT_CONTROL_REGISTER in ntddk.h
typedef struct {
    UINT32 AttentionButtom : 1;
    UINT32 PowerConotroller : 1;
    UINT32 MRL_Sensor : 1;
    UINT32 AttentionIndicator : 1;
    UINT32 PowerIndicator : 1;
    UINT32 HotPlugSurprise : 1;
    UINT32 HotPlugCapable : 1;
    UINT32 SlotPowerLimitValue : 8;
    UINT32 SlotPowerLimitScale : 2;
    UINT32 EletromechanicalInterLock : 1;
    UINT32 NoCommandCompleted : 1;
    UINT32 PhysicalSlotNumber : 13;
}PCIE_SLOT_CAPABILITY_REG;

//PCI_EXPRESS_SLOT_CONTROL_REGISTER in ntddk.h
typedef struct {
    USHORT AttentionButtonEnable : 1;
    USHORT PowerFaultDetectEnable : 1;
    USHORT MRLSensorEnable : 1;
    USHORT PresenceDetectEnable : 1;
    USHORT CommandCompletedEnable : 1;
    USHORT HotPlugInterruptEnable : 1;
    USHORT AttentionIndicatorControl : 2;  // EXPRESS_INDICATOR_STATE
    USHORT PowerIndicatorControl : 2;      // EXPRESS_INDICATOR_STATE
    USHORT PowerControllerControl : 1;     // EXPRESS_POWER_STATE
    USHORT ElectromechanicalLockControl : 1;
    USHORT DataLinkStateChangeEnable : 1;
    USHORT Rsvd : 3;
}PCIE_SLOT_CONTROL_REG;

//PCI_EXPRESS_SLOT_STATUS_REGISTER in ntddk.h
typedef struct {
    UINT16 AttentionButton : 1;
    UINT16 PowerFault : 1;
    UINT16 MRL_SensorChanged : 1;
    UINT16 PresenceDetectChanged : 1;
    UINT16 CommandCompleted : 1;
    UINT16 MRL_Sensor : 1;
    UINT16 PresenceDetect : 1;
    UINT16 EletromechanicalInterLock : 1;
    UINT16 DataLinkLayerStateChanged : 1;
    UINT16 Reserved : 7;
}PCIE_SLOT_STATUS_REG;

//PCI_EXPRESS_ROOT_CONTROL_REGISTER in ntddk.h
typedef struct {
    UINT16 SystemErrorOnCorrectableError : 1;
    UINT16 SystemErrorOnNonFatalError : 1;
    UINT16 SystemErrorOnFatalError : 1;
    UINT16 PME_Interrupt : 1;
    UINT16 CRS_SoftwareVisibility : 1;
    UINT16 Reserved : 11;
}PCIE_ROOT_CONTROL_REG;

//PCI_EXPRESS_ROOT_CAPABILITIES_REGISTER in ntddk.h
typedef struct {
    UINT16 CRS_SoftwareVisibility : 1;
    UINT16 Reserved : 15;
}PCIE_ROOT_CAPABILITY_REG;

//PCI_EXPRESS_ROOT_STATUS_REGISTER in ntddk.h
typedef struct {
    UINT32 PME_RequesterId : 16;
    UINT32 PME_Status : 1;
    UINT32 PME_Pending : 1;
    UINT32 Reserved : 14;
}PCIE_ROOT_STATUS_REG;

//PCI_EXPRESS_DEVICE_CAPABILITIES_2_REGISTER in ntddk.h
typedef struct {
    ULONG CompletionTimeoutRangesSupported : 4;
    ULONG CompletionTimeoutDisableSupported : 1;
    ULONG AriForwardingSupported : 1;
    ULONG AtomicOpRoutingSupported : 1;
    ULONG AtomicOpCompleterSupported32Bit : 1;
    ULONG AtomicOpCompleterSupported64Bit : 1;
    ULONG CASCompleterSupported128Bit : 1;
    ULONG NoROEnabledPRPRPassing : 1;
    ULONG LTRMechanismSupported : 1;
    ULONG TPHCompleterSupported : 2;
    ULONG Rsvd : 4;
    ULONG OBFFSupported : 2;
    ULONG ExtendedFmtFieldSuported : 1;
    ULONG EndEndTLPPrefixSupported : 1;
    ULONG MaxEndEndTLPPrefixes : 2;
    ULONG Rsvd2 : 8;
}PCIE_DEVICE_CAPABILITY2_REG;


//PCI_EXPRESS_DEVICE_CONTROL_2_REGISTER in ntddk.h
typedef struct {
    USHORT CompletionTimeoutValue : 4;
    USHORT CompletionTimeoutDisable : 1;
    USHORT AriForwardingEnable : 1;
    USHORT AtomicOpRequesterEnable : 1;
    USHORT AtomicOpEgresBlocking : 1;
    USHORT IDORequestEnable : 1;
    USHORT IDOCompletionEnable : 1;
    USHORT LTRMechanismEnable : 1;
    USHORT Rsvd : 2;
    USHORT OBFFEnable : 2;
    USHORT EndEndTLPPrefixBlocking : 1;
}PCIE_DEVICE_CONTROL2_REG;

typedef struct 
{
    PCI_CAP_HEADER Header;
    PCIE_CAPABILITY_REG PCIeCap;
    PCIE_DEVICE_CAPABILITY_REG DevCap;
    PCIE_DEVICE_CONTROL_REG DevCtrl;
    PCIE_DEVICE_STATUS_REG DevStatus;
    PCIE_LINK_CAPABILITY_REG LinkCap;
    PCIE_LINK_CONTROL_REG LinkCtrl;
    PCIE_LINK_STATUS_REG LinkStatus;
    PCIE_SLOT_CAPABILITY_REG SlotCap;
    PCIE_SLOT_CONTROL_REG SlotCtrl;
    PCIE_SLOT_STATUS_REG SlotStatus;
    PCIE_ROOT_CONTROL_REG RootCtrl;
    PCIE_ROOT_CAPABILITY_REG RootCap;
    PCIE_ROOT_STATUS_REG RootStatus;
    PCIE_DEVICE_CAPABILITY2_REG DevCap2;
    PCIE_DEVICE_CONTROL2_REG DevCtrl2;
    UINT16 DevStatus2;  //In PCIe spec 7.5.3.18., it said DevStatus2 is just a placeholder...
}PCIE_CAPABILITIES, * PPCIE_CAPABILITIES;
#pragma pack()

#if 0

//
// All Enhanced capabilities have the following header.
//

typedef struct _PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER {

    USHORT CapabilityID;
    USHORT Version : 4;
    USHORT Next : 12;

} PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER, * PPCI_EXPRESS_ENHANCED_CAPABILITY_HEADER;

//
// Vendor Specific Capability
//

typedef struct _PCI_EXPRESS_VENDOR_SPECIFIC_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    USHORT VsecId;
    USHORT VsecRev : 4;
    USHORT VsecLength : 12;

} PCI_EXPRESS_VENDOR_SPECIFIC_CAPABILITY, * PPCI_EXPRESS_VENDOR_SPECIFIC_CAPABILITY;

//
// Serial Number Capability.
//

typedef struct _PCI_EXPRESS_SERIAL_NUMBER_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    ULONG LowSerialNumber;
    ULONG HighSerialNumber;

} PCI_EXPRESS_SERIAL_NUMBER_CAPABILITY, * PPCI_EXPRESS_SERIAL_NUMBER_CAPABILITY;

//
// ARI Capability structures
//

typedef struct _PCI_EXPRESS_ARI_CAPABILITY_REGISTER {

    USHORT MfvcFunctionGroupsCapability : 1;
    USHORT AcsFunctionGroupsCapability : 1;
    USHORT Reserved : 6;
    USHORT NextFunctionNumber : 8;

} PCI_EXPRESS_ARI_CAPABILITY_REGISTER, * PPCI_EXPRESS_ARI_CAPABILITY_REGISTER;

typedef struct _PCI_EXPRESS_ARI_CONTROL_REGISTER {

    USHORT MfvcFunctionGroupsEnable : 1;
    USHORT AcsFunctionGroupsEnable : 1;
    USHORT Reserved1 : 2;
    USHORT FunctionGroup : 3;
    USHORT Reserved2 : 9;

} PCI_EXPRESS_ARI_CONTROL_REGISTER, * PPCI_EXPRESS_ARI_CONTROL_REGISTER;

typedef struct _PCI_EXPRESS_ARI_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    PCI_EXPRESS_ARI_CAPABILITY_REGISTER Capability;
    PCI_EXPRESS_ARI_CONTROL_REGISTER Control;

} PCI_EXPRESS_ARI_CAPABILITY, * PPCI_EXPRESS_ARI_CAPABILITY;

//
// Virtual Channel (VC) Capability structures
//

typedef union _VIRTUAL_CHANNEL_CAPABILITIES1 {
    struct {
        ULONG ExtendedVCCount : 3;
        ULONG RsvdP1 : 1;
        ULONG LowPriorityExtendedVCCount : 3;
        ULONG RsvdP2 : 1;
        ULONG ReferenceClock : 2;
        ULONG PortArbitrationTableEntrySize : 2;
        ULONG RsvdP3 : 20;
    };
    ULONG AsULONG;
} VIRTUAL_CHANNEL_CAPABILITIES1, * PVIRTUAL_CHANNEL_CAPABILITIES1;

typedef union _VIRTUAL_CHANNEL_CAPABILITIES2 {
    struct {
        ULONG VCArbitrationCapability : 8;
        ULONG RsvdP : 16;
        ULONG VCArbitrationTableOffset : 8;
    };
    ULONG AsULONG;
} VIRTUAL_CHANNEL_CAPABILITIES2, * PVIRTUAL_CHANNEL_CAPABILITIES2;

typedef union _VIRTUAL_CHANNEL_CONTROL {
    struct {
        USHORT  LoadVCArbitrationTable : 1;
        USHORT  VCArbitrationSelect : 3;
        USHORT  RsvdP : 12;
    };
    USHORT AsUSHORT;
} VIRTUAL_CHANNEL_CONTROL, * PVIRTUAL_CHANNEL_CONTROL;

typedef union _VIRTUAL_CHANNEL_STATUS {
    struct {
        USHORT  VCArbitrationTableStatus : 1;
        USHORT  RsvdZ : 15;
    };
    USHORT AsUSHORT;
} VIRTUAL_CHANNEL_STATUS, * PVIRTUAL_CHANNEL_STATUS;

typedef union _VIRTUAL_RESOURCE_CAPABILITY {
    struct {
        ULONG   PortArbitrationCapability : 8;
        ULONG   RsvdP1 : 6;
        ULONG   Undefined : 1;
        ULONG   RejectSnoopTransactions : 1;
        ULONG   MaximumTimeSlots : 7;
        ULONG   RsvdP2 : 1;
        ULONG   PortArbitrationTableOffset : 8;
    };
    ULONG AsULONG;
} VIRTUAL_RESOURCE_CAPABILITY, * PVIRTUAL_RESOURCE_CAPABILITY;

typedef union _VIRTUAL_RESOURCE_CONTROL {
    struct {
        ULONG   TcVcMap : 8;
        ULONG   RsvdP1 : 8;
        ULONG   LoadPortArbitrationTable : 1;
        ULONG   PortArbitrationSelect : 3;
        ULONG   RsvdP2 : 4;
        ULONG   VcID : 3;
        ULONG   RsvdP3 : 4;
        ULONG   VcEnable : 1;
    };
    ULONG AsULONG;
} VIRTUAL_RESOURCE_CONTROL, * PVIRTUAL_RESOURCE_CONTROL;

typedef union _VIRTUAL_RESOURCE_STATUS {
    struct {
        USHORT  PortArbitrationTableStatus : 1;
        USHORT  VcNegotiationPending : 1;
        USHORT  RsvdZ : 14;
    };
    USHORT AsUSHORT;
} VIRTUAL_RESOURCE_STATUS, * PVIRTUAL_RESOURCE_STATUS;

typedef struct _VIRTUAL_RESOURCE {
    VIRTUAL_RESOURCE_CAPABILITY     Capability;
    VIRTUAL_RESOURCE_CONTROL        Control;
    USHORT                          RsvdP;
    VIRTUAL_RESOURCE_STATUS         Status;
} VIRTUAL_RESOURCE, * PVIRTUAL_RESOURCE;

typedef struct _PCI_EXPRESS_VIRTUAL_CHANNEL_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    VIRTUAL_CHANNEL_CAPABILITIES1   Capabilities1;
    VIRTUAL_CHANNEL_CAPABILITIES2   Capabilities2;
    VIRTUAL_CHANNEL_CONTROL         Control;
    VIRTUAL_CHANNEL_STATUS          Status;
    VIRTUAL_RESOURCE                Resource[8];

} PCI_EXPRESS_VIRTUAL_CHANNEL_CAPABILITY, * PPCI_EXPRESS_VIRTUAL_CHANNEL_CAPABILITY;

//
// ATS Capability structures
//

typedef struct _PCI_EXPRESS_ATS_CAPABILITY_REGISTER {

    USHORT InvalidateQueueDepth : 5;
    USHORT PageAlignedRequest : 1;
    USHORT GlobalInvalidateSupported : 1;
    USHORT RelaxedOrderingSupported : 1;
    USHORT Reserved : 8;

} PCI_EXPRESS_ATS_CAPABILITY_REGISTER, * PPCI_EXPRESS_ATS_CAPABILITY_REGISTER;

typedef union _PCI_EXPRESS_ATS_CONTROL_REGISTER {
    struct {
        USHORT SmallestTransactionUnit : 5;
        USHORT Reserved : 10;
        USHORT Enable : 1;
    };

    USHORT AsUSHORT;

} PCI_EXPRESS_ATS_CONTROL_REGISTER, * PPCI_EXPRESS_ATS_CONTROL_REGISTER;

typedef struct _PCI_EXPRESS_ATS_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    PCI_EXPRESS_ATS_CAPABILITY_REGISTER Capability;
    PCI_EXPRESS_ATS_CONTROL_REGISTER Control;

} PCI_EXPRESS_ATS_CAPABILITY, * PPCI_EXPRESS_ATS_CAPABILITY;

//
// PASID Extended Capability Structure
//

typedef union _PCI_EXPRESS_PASID_CAPABILITY_REGISTER {
    struct {
        USHORT Rsvd : 1;
        USHORT ExecutePermissionSupported : 1;
        USHORT PrivilegedModeSupported : 1;
        USHORT Rsvd2 : 5;
        USHORT MaxPASIDWidth : 5;
        USHORT Rsvd3 : 3;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;
} PCI_EXPRESS_PASID_CAPABILITY_REGISTER, * PPCI_EXPRESS_PASID_CAPABILITY_REGISTER;

typedef union _PCI_EXPRESS_PASID_CONTROL_REGISTER {
    struct {
        USHORT PASIDEnable : 1;
        USHORT ExecutePermissionEnable : 1;
        USHORT PrivilegedModeEnable : 1;
        USHORT Rsvd : 13;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;
} PCI_EXPRESS_PASID_CONTROL_REGISTER, * PPCI_EXPRESS_PASID_CONTROL_REGISTER;

typedef struct _PCI_EXPRESS_PASID_CAPABILITY {
    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;
    PCI_EXPRESS_PASID_CAPABILITY_REGISTER Capability;
    PCI_EXPRESS_PASID_CONTROL_REGISTER Control;
} PCI_EXPRESS_PASID_CAPABILITY, * PPCI_EXPRESS_PASID_CAPABILITY;

//
// PRI Extended Capability Structure
//

typedef union _PCI_EXPRESS_PRI_STATUS_REGISTER {
    struct {
        USHORT ResponseFailure : 1;
        USHORT UnexpectedPageRequestGroupIndex : 1;
        USHORT Rsvd : 6;
        USHORT Stopped : 1;
        USHORT Rsvd2 : 6;
        USHORT PrgResponsePasidRequired : 1;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;
} PCI_EXPRESS_PRI_STATUS_REGISTER, * PPCI_EXPRESS_PRI_STATUS_REGISTER;

typedef union _PCI_EXPRESS_PRI_CONTROL_REGISTER {
    struct {
        USHORT Enable : 1;
        USHORT Reset : 1;
        USHORT Rsvd : 14;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;
} PCI_EXPRESS_PRI_CONTROL_REGISTER, * PPCI_EXPRESS_PRI_CONTROL_REGISTER;

typedef struct _PCI_EXPRESS_PRI_CAPABILITY {
    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;
    PCI_EXPRESS_PRI_CONTROL_REGISTER Control;
    PCI_EXPRESS_PRI_STATUS_REGISTER Status;
    ULONG PRCapacity;
    ULONG PRAllocation;
} PCI_EXPRESS_PRI_CAPABILITY, * PPCI_EXPRESS_PRI_CAPABILITY;

//
// PTM Extended Capability structures.
//

typedef union _PCI_EXPRESS_PTM_CAPABILITY_REGISTER {

    struct {
        ULONG RequesterCapable : 1;
        ULONG ResponderCapable : 1;
        ULONG RootCapable : 1;
        ULONG Rsvd : 5;
        ULONG LocalGranularity : 8;
        ULONG Rsvd2 : 16;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_PTM_CAPABILITY_REGISTER, * PPCI_EXPRESS_PTM_CAPABILITY_REGISTER;

typedef union _PCI_EXPRESS_PTM_CONTROL_REGISTER {

    struct {
        ULONG Enable : 1;
        ULONG RootSelect : 1;
        ULONG Rsvd : 6;
        ULONG EffectiveGranularity : 8;
        ULONG Rsvd2 : 16;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_PTM_CONTROL_REGISTER, * PPCI_EXPRESS_PTM_CONTROL_REGISTER;

typedef struct _PCI_EXPRESS_PTM_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER  Header;
    PCI_EXPRESS_PTM_CAPABILITY_REGISTER     PtmCapability;
    PCI_EXPRESS_PTM_CONTROL_REGISTER        PtmControl;

} PCI_EXPRESS_PTM_CAPABILITY, * PPCI_EXPRESS_PTM_CAPABILITY;

//
// PCI Express Advanced Error Reporting structures.
//

typedef union _PCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS {

    struct {
        ULONG Undefined : 1;
        ULONG Reserved1 : 3;
        ULONG DataLinkProtocolError : 1;
        ULONG SurpriseDownError : 1;
        ULONG Reserved2 : 6;
        ULONG PoisonedTLP : 1;
        ULONG FlowControlProtocolError : 1;
        ULONG CompletionTimeout : 1;
        ULONG CompleterAbort : 1;
        ULONG UnexpectedCompletion : 1;
        ULONG ReceiverOverflow : 1;
        ULONG MalformedTLP : 1;
        ULONG ECRCError : 1;
        ULONG UnsupportedRequestError : 1;
        ULONG AcsViolation : 1;
        ULONG UncorrectableInternalError : 1;
        ULONG MCBlockedTlp : 1;
        ULONG AtomicOpEgressBlocked : 1;
        ULONG TlpPrefixBlocked : 1;
        ULONG Reserved3 : 6;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS, * PPCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS;

typedef union _PCI_EXPRESS_UNCORRECTABLE_ERROR_MASK {

    struct {
        ULONG Undefined : 1;
        ULONG Reserved1 : 3;
        ULONG DataLinkProtocolError : 1;
        ULONG SurpriseDownError : 1;
        ULONG Reserved2 : 6;
        ULONG PoisonedTLP : 1;
        ULONG FlowControlProtocolError : 1;
        ULONG CompletionTimeout : 1;
        ULONG CompleterAbort : 1;
        ULONG UnexpectedCompletion : 1;
        ULONG ReceiverOverflow : 1;
        ULONG MalformedTLP : 1;
        ULONG ECRCError : 1;
        ULONG UnsupportedRequestError : 1;
        ULONG AcsViolation : 1;
        ULONG UncorrectableInternalError : 1;
        ULONG MCBlockedTlp : 1;
        ULONG AtomicOpEgressBlocked : 1;
        ULONG TlpPrefixBlocked : 1;
        ULONG Reserved3 : 6;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_UNCORRECTABLE_ERROR_MASK, * PPCI_EXPRESS_UNCORRECTABLE_ERROR_MASK;

typedef union _PCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY {

    struct {
        ULONG Undefined : 1;
        ULONG Reserved1 : 3;
        ULONG DataLinkProtocolError : 1;
        ULONG SurpriseDownError : 1;
        ULONG Reserved2 : 6;
        ULONG PoisonedTLP : 1;
        ULONG FlowControlProtocolError : 1;
        ULONG CompletionTimeout : 1;
        ULONG CompleterAbort : 1;
        ULONG UnexpectedCompletion : 1;
        ULONG ReceiverOverflow : 1;
        ULONG MalformedTLP : 1;
        ULONG ECRCError : 1;
        ULONG UnsupportedRequestError : 1;
        ULONG AcsViolation : 1;
        ULONG UncorrectableInternalError : 1;
        ULONG MCBlockedTlp : 1;
        ULONG AtomicOpEgressBlocked : 1;
        ULONG TlpPrefixBlocked : 1;
        ULONG Reserved3 : 6;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY, * PPCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY;

typedef union _PCI_EXPRESS_CORRECTABLE_ERROR_STATUS {

    struct {
        ULONG ReceiverError : 1;
        ULONG Reserved1 : 5;
        ULONG BadTLP : 1;
        ULONG BadDLLP : 1;
        ULONG ReplayNumRollover : 1;
        ULONG Reserved2 : 3;
        ULONG ReplayTimerTimeout : 1;
        ULONG AdvisoryNonFatalError : 1;
        ULONG CorrectedInternalError : 1;
        ULONG HeaderLogOverflow : 1;
        ULONG Reserved3 : 16;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_CORRECTABLE_ERROR_STATUS, * PPCI_CORRECTABLE_ERROR_STATUS;

typedef union _PCI_EXPRESS_CORRECTABLE_ERROR_MASK {

    struct {
        ULONG ReceiverError : 1;
        ULONG Reserved1 : 5;
        ULONG BadTLP : 1;
        ULONG BadDLLP : 1;
        ULONG ReplayNumRollover : 1;
        ULONG Reserved2 : 3;
        ULONG ReplayTimerTimeout : 1;
        ULONG AdvisoryNonFatalError : 1;
        ULONG CorrectedInternalError : 1;
        ULONG HeaderLogOverflow : 1;
        ULONG Reserved3 : 16;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_CORRECTABLE_ERROR_MASK, * PPCI_CORRECTABLE_ERROR_MASK;

typedef union _PCI_EXPRESS_AER_CAPABILITIES {

    struct {
        ULONG FirstErrorPointer : 5;
        ULONG ECRCGenerationCapable : 1;
        ULONG ECRCGenerationEnable : 1;
        ULONG ECRCCheckCapable : 1;
        ULONG ECRCCheckEnable : 1;
        ULONG MultipleHeaderRecordingCapable : 1;
        ULONG MultipleHeaderRecordingEnable : 1;
        ULONG TlpPrefixLogPresent : 1;
        ULONG Reserved : 20;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_AER_CAPABILITIES, * PPCI_EXPRESS_AER_CAPABILITIES;

typedef union _PCI_EXPRESS_ROOT_ERROR_COMMAND {

    struct {
        ULONG CorrectableErrorReportingEnable : 1;
        ULONG NonFatalErrorReportingEnable : 1;
        ULONG FatalErrorReportingEnable : 1;
        ULONG Reserved : 29;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_ROOT_ERROR_COMMAND, * PPCI_EXPRESS_ROOT_ERROR_COMMAND;

typedef union _PCI_EXPRESS_ROOT_ERROR_STATUS {

    struct {
        ULONG CorrectableErrorReceived : 1;
        ULONG MultipleCorrectableErrorsReceived : 1;
        ULONG UncorrectableErrorReceived : 1;
        ULONG MultipleUncorrectableErrorsReceived : 1;
        ULONG FirstUncorrectableFatal : 1;
        ULONG NonFatalErrorMessagesReceived : 1;
        ULONG FatalErrorMessagesReceived : 1;
        ULONG Reserved : 20;
        ULONG AdvancedErrorInterruptMessageNumber : 5;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_ROOT_ERROR_STATUS, * PPCI_EXPRESS_ROOT_ERROR_STATUS;

typedef union _PCI_EXPRESS_ERROR_SOURCE_ID {

    struct {
        USHORT CorrectableSourceIdFun : 3;
        USHORT CorrectableSourceIdDev : 5;
        USHORT CorrectableSourceIdBus : 8;
        USHORT UncorrectableSourceIdFun : 3;
        USHORT UncorrectableSourceIdDev : 5;
        USHORT UncorrectableSourceIdBus : 8;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_ERROR_SOURCE_ID, * PPCI_EXPRESS_ERROR_SOURCE_ID;

typedef union _PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_STATUS {

    struct {
        ULONG TargetAbortOnSplitCompletion : 1;
        ULONG MasterAbortOnSplitCompletion : 1;
        ULONG ReceivedTargetAbort : 1;
        ULONG ReceivedMasterAbort : 1;
        ULONG RsvdZ : 1;
        ULONG UnexpectedSplitCompletionError : 1;
        ULONG UncorrectableSplitCompletion : 1;
        ULONG UncorrectableDataError : 1;
        ULONG UncorrectableAttributeError : 1;
        ULONG UncorrectableAddressError : 1;
        ULONG DelayedTransactionDiscardTimerExpired : 1;
        ULONG PERRAsserted : 1;
        ULONG SERRAsserted : 1;
        ULONG InternalBridgeError : 1;
        ULONG Reserved : 18;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_STATUS,
* PPCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_STATUS;

typedef union _PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_MASK {

    struct {
        ULONG TargetAbortOnSplitCompletion : 1;
        ULONG MasterAbortOnSplitCompletion : 1;
        ULONG ReceivedTargetAbort : 1;
        ULONG ReceivedMasterAbort : 1;
        ULONG RsvdZ : 1;
        ULONG UnexpectedSplitCompletionError : 1;
        ULONG UncorrectableSplitCompletion : 1;
        ULONG UncorrectableDataError : 1;
        ULONG UncorrectableAttributeError : 1;
        ULONG UncorrectableAddressError : 1;
        ULONG DelayedTransactionDiscardTimerExpired : 1;
        ULONG PERRAsserted : 1;
        ULONG SERRAsserted : 1;
        ULONG InternalBridgeError : 1;
        ULONG Reserved : 18;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_MASK,
* PPCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_MASK;

typedef union _PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_SEVERITY {

    struct {
        ULONG TargetAbortOnSplitCompletion : 1;
        ULONG MasterAbortOnSplitCompletion : 1;
        ULONG ReceivedTargetAbort : 1;
        ULONG ReceivedMasterAbort : 1;
        ULONG RsvdZ : 1;
        ULONG UnexpectedSplitCompletionError : 1;
        ULONG UncorrectableSplitCompletion : 1;
        ULONG UncorrectableDataError : 1;
        ULONG UncorrectableAttributeError : 1;
        ULONG UncorrectableAddressError : 1;
        ULONG DelayedTransactionDiscardTimerExpired : 1;
        ULONG PERRAsserted : 1;
        ULONG SERRAsserted : 1;
        ULONG InternalBridgeError : 1;
        ULONG Reserved : 18;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_SEVERITY,
* PPCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_SEVERITY;

typedef union _PCI_EXPRESS_SEC_AER_CAPABILITIES {

    struct {
        ULONG SecondaryUncorrectableFirstErrorPtr : 5;
        ULONG Reserved : 27;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SEC_AER_CAPABILITIES, * PPCI_EXPRESS_SEC_AER_CAPABILITIES;

#define ROOT_CMD_ENABLE_CORRECTABLE_ERROR_REPORTING  0x00000001
#define ROOT_CMD_ENABLE_NONFATAL_ERROR_REPORTING     0x00000002
#define ROOT_CMD_ENABLE_FATAL_ERROR_REPORTING        0x00000004

#define ROOT_CMD_ERROR_REPORTING_ENABLE_MASK \
    (ROOT_CMD_ENABLE_FATAL_ERROR_REPORTING | \
     ROOT_CMD_ENABLE_NONFATAL_ERROR_REPORTING | \
     ROOT_CMD_ENABLE_CORRECTABLE_ERROR_REPORTING)

//
// Advanced Error Reporting Capability structure.
//

typedef struct _PCI_EXPRESS_AER_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    PCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS UncorrectableErrorStatus;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_MASK UncorrectableErrorMask;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY UncorrectableErrorSeverity;
    PCI_EXPRESS_CORRECTABLE_ERROR_STATUS CorrectableErrorStatus;
    PCI_EXPRESS_CORRECTABLE_ERROR_MASK CorrectableErrorMask;
    PCI_EXPRESS_AER_CAPABILITIES CapabilitiesAndControl;
    ULONG HeaderLog[4];
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_STATUS SecUncorrectableErrorStatus;
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_MASK SecUncorrectableErrorMask;
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_SEVERITY SecUncorrectableErrorSeverity;
    PCI_EXPRESS_SEC_AER_CAPABILITIES SecCapabilitiesAndControl;
    ULONG SecHeaderLog[4];

} PCI_EXPRESS_AER_CAPABILITY, * PPCI_EXPRESS_AER_CAPABILITY;

//
// Advanced Error Reporting Capability structure for root port.
//

typedef struct _PCI_EXPRESS_ROOTPORT_AER_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    PCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS UncorrectableErrorStatus;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_MASK UncorrectableErrorMask;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY UncorrectableErrorSeverity;
    PCI_EXPRESS_CORRECTABLE_ERROR_STATUS CorrectableErrorStatus;
    PCI_EXPRESS_CORRECTABLE_ERROR_MASK CorrectableErrorMask;
    PCI_EXPRESS_AER_CAPABILITIES CapabilitiesAndControl;
    ULONG HeaderLog[4];
    PCI_EXPRESS_ROOT_ERROR_COMMAND RootErrorCommand;
    PCI_EXPRESS_ROOT_ERROR_STATUS RootErrorStatus;
    PCI_EXPRESS_ERROR_SOURCE_ID ErrorSourceId;

} PCI_EXPRESS_ROOTPORT_AER_CAPABILITY, * PPCI_EXPRESS_ROOTPORT_AER_CAPABILITY;

//
// Advanced Error Reporting Capability structure for root port.
//

typedef struct _PCI_EXPRESS_BRIDGE_AER_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER Header;

    PCI_EXPRESS_UNCORRECTABLE_ERROR_STATUS UncorrectableErrorStatus;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_MASK UncorrectableErrorMask;
    PCI_EXPRESS_UNCORRECTABLE_ERROR_SEVERITY UncorrectableErrorSeverity;
    PCI_EXPRESS_CORRECTABLE_ERROR_STATUS CorrectableErrorStatus;
    PCI_EXPRESS_CORRECTABLE_ERROR_MASK CorrectableErrorMask;
    PCI_EXPRESS_AER_CAPABILITIES CapabilitiesAndControl;
    ULONG HeaderLog[4];
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_STATUS SecUncorrectableErrorStatus;
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_MASK SecUncorrectableErrorMask;
    PCI_EXPRESS_SEC_UNCORRECTABLE_ERROR_SEVERITY SecUncorrectableErrorSeverity;
    PCI_EXPRESS_SEC_AER_CAPABILITIES SecCapabilitiesAndControl;
    ULONG SecHeaderLog[4];

} PCI_EXPRESS_BRIDGE_AER_CAPABILITY, * PPCI_EXPRESS_BRIDGE_AER_CAPABILITY;

//
// Access Control Services Capability structure.
//
typedef union _PCI_EXPRESS_ACS_CAPABILITY_REGISTER {

    struct {
        USHORT SourceValidation : 1;
        USHORT TranslationBlocking : 1;
        USHORT RequestRedirect : 1;
        USHORT CompletionRedirect : 1;
        USHORT UpstreamForwarding : 1;
        USHORT EgressControl : 1;
        USHORT DirectTranslation : 1;
        USHORT EnhancedCapability : 1;
        USHORT EgressControlVectorSize : 8;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;

} PCI_EXPRESS_ACS_CAPABILITY_REGISTER, * PPCI_EXPRESS_ACS_CAPABILITY_REGISTER;

#define PCI_ACS_ALLOWED     0
#define PCI_ACS_BLOCKED     1
#define PCI_ACS_REDIRECTED  2

typedef union _PCI_EXPRESS_ACS_CONTROL {

    struct {
        USHORT SourceValidation : 1;
        USHORT TranslationBlocking : 1;
        USHORT RequestRedirect : 1;
        USHORT CompletionRedirect : 1;
        USHORT UpstreamForwarding : 1;
        USHORT EgressControl : 1;
        USHORT DirectTranslation : 1;
        USHORT IoBlocking : 1;
        USHORT DspMemoryControl : 2;
        USHORT UspMemoryControl : 2;
        USHORT UnclaimedRedirect : 1;
        USHORT Reserved : 3;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;

} PCI_EXPRESS_ACS_CONTROL, * PPCI_EXPRESS_ACS_CONTROL;

typedef struct _PCI_EXPRESS_ACS_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER  Header;

    PCI_EXPRESS_ACS_CAPABILITY_REGISTER      Capability;
    PCI_EXPRESS_ACS_CONTROL                 Control;

    ULONG                                   EgressControl[1];
} PCI_EXPRESS_ACS_CAPABILITY, * PPCI_EXPRESS_ACS_CAPABILITY;

//
// Single-Root I/O Virtualization Capability structure for endpoints
//

typedef union _PCI_EXPRESS_SRIOV_CAPS {

    struct {
        ULONG VFMigrationCapable : 1;
        ULONG Reserved1 : 20;
        ULONG VFMigrationInterruptNumber : 11;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SRIOV_CAPS, * PPCI_EXPRESS_SRIOV_CAPS;

typedef union _PCI_EXPRESS_SRIOV_CONTROL {

    struct {
        USHORT VFEnable : 1;
        USHORT VFMigrationEnable : 1;
        USHORT VFMigrationInterruptEnable : 1;
        USHORT VFMemorySpaceEnable : 1;
        USHORT ARICapableHierarchy : 1;
        USHORT Reserved1 : 11;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;

} PCI_EXPRESS_SRIOV_CONTROL, * PPCI_EXPRESS_SRIOV_CONTROL;

typedef union _PCI_EXPRESS_SRIOV_STATUS {

    struct {
        USHORT VFMigrationStatus : 1;
        USHORT Reserved1 : 15;
    } DUMMYSTRUCTNAME;

    USHORT AsUSHORT;

} PCI_EXPRESS_SRIOV_STATUS, * PPCI_EXPRESS_SRIOV_STATUS;

typedef union _PCI_EXPRESS_SRIOV_MIGRATION_STATE_ARRAY {

    struct {
        ULONG VFMigrationStateBIR : 3;
        ULONG VFMigrationStateOffset : 29;
    } DUMMYSTRUCTNAME;

    ULONG AsULONG;

} PCI_EXPRESS_SRIOV_MIGRATION_STATE_ARRAY, * PPCI_EXPRESS_SRIOV_MIGRATION_STATE_ARRAY;

typedef struct _PCI_EXPRESS_SRIOV_CAPABILITY {

    PCI_EXPRESS_ENHANCED_CAPABILITY_HEADER  Header;

    PCI_EXPRESS_SRIOV_CAPS                  SRIOVCapabilities;
    PCI_EXPRESS_SRIOV_CONTROL               SRIOVControl;
    PCI_EXPRESS_SRIOV_STATUS                SRIOVStatus;
    USHORT                                  InitialVFs;
    USHORT                                  TotalVFs;
    USHORT                                  NumVFs;
    UCHAR                                   FunctionDependencyLink;
    UCHAR                                   RsvdP1;
    USHORT                                  FirstVFOffset;
    USHORT                                  VFStride;
    USHORT                                  RsvdP2;
    USHORT                                  VFDeviceId;
    ULONG                                   SupportedPageSizes;
    ULONG                                   SystemPageSize;
    ULONG                                   BaseAddresses[PCI_TYPE0_ADDRESSES];
    PCI_EXPRESS_SRIOV_MIGRATION_STATE_ARRAY VFMigrationStateArrayOffset;

} PCI_EXPRESS_SRIOV_CAPABILITY, * PPCI_EXPRESS_SRIOV_CAPABILITY;
#endif

