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
    UINT16 Version : 4;         //PCIe cap structure version (0x02)
    UINT16 DevicePortType : 4;  //Device/Port Type, 0==PCIe endpoint, 1==Legacy PCIe endpoint, ...etc.
    UINT16 SlotImplement : 1;          //Slot Implemented (not applicable for PCIe endpoint device.
    UINT16 InterruptMsgNumber : 5;         //Interrupt Message Number ??
    UINT16 Undefined : 1;
    UINT16 Reserved : 1;
}PCIE_CAPABILITY_REG;

typedef struct {
    UINT32 MaxPayloadSupport : 3;
    UINT32 PhantomFunctionSupport : 2;
    UINT32 ExtendTagFieldSupport : 1;
    UINT32 EndpointL0AcceptLatency : 3;
    UINT32 EndpointL1AcceptLatency : 3;
    UINT32 Undefined : 3;
    UINT32 RoleBasedErrorReport : 1;
    UINT32 ErrCorSubclassCap : 1;
    UINT32 Reserved : 1;
    UINT32 SlotPowerLimitValue : 8;
    UINT32 SlotPowerLimitScale : 2;
    UINT32 FuncLevelResetCap : 1;
    UINT32 Reserved2 : 3;
} PCIE_DEVICE_CAPABILITY;

typedef struct {
    USHORT CorrectableErrorReport : 1;
    USHORT NonFatalErrorReport : 1;
    USHORT FatalErrorReport : 1;
    USHORT UnsupportRequestReport : 1;
    USHORT RelaxedOrdering : 1;
    USHORT MaxPayloadSize : 3;
    USHORT ExtendTagField : 1;
    USHORT PhantomFunction : 1;
    USHORT AuxPowerPM : 1;
    USHORT NoSnoop : 1;
    USHORT MaxReadRequestSize : 3;
    USHORT BCR_IFLR : 1;        //this field controls Bridge Config Retry or Initiate Function Level Reset.
}PCIE_DEVICE_CONTROL;

typedef struct {
    USHORT CorrectableError : 1;
    USHORT NonFatalError : 1;
    USHORT FatalError : 1;
    USHORT UnsupportedRequest : 1;
    USHORT AuxPower : 1;
    USHORT TransactionPending : 1;
    USHORT EmergencyPowerReduction : 1;
    USHORT Reserved : 9;
}PCIE_DEVICE_STATUS;

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
} PCIE_LINK_CAPABILITY;

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
} PCIE_LINK_CONTROL;

typedef struct {
    USHORT LinkSpeed : 4;       //current link speed
    USHORT LinkWidth : 6;       //negotiated link width
    USHORT Undefined : 1;
    USHORT LinkTraining : 1;
    USHORT SlotClockConfig : 1;
    USHORT DataLinkLayerLinkActive : 1;
    USHORT LinkBandwidthMgmt : 1;       //Link Bandwidth Management status
    USHORT LinkAutoBandwidth : 1;       //Link Autonomous Bandwidth status
} PCIE_LINK_STATUS;

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
}PCIE_SLOT_CAPABILITY;

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
}PCIE_SLOT_CONTROL;

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
}PCIE_SLOT_STATUS;

//PCI_EXPRESS_ROOT_CONTROL_REGISTER in ntddk.h
typedef struct {
    UINT16 SystemErrorOnCorrectableError : 1;
    UINT16 SystemErrorOnNonFatalError : 1;
    UINT16 SystemErrorOnFatalError : 1;
    UINT16 PME_Interrupt : 1;
    UINT16 CRS_SoftwareVisibility : 1;
    UINT16 Reserved : 11;
}PCIE_ROOT_CONTROL;

//PCI_EXPRESS_ROOT_CAPABILITIES_REGISTER in ntddk.h
typedef struct {
    UINT16 CRS_SoftwareVisibility : 1;
    UINT16 Reserved : 15;
}PCIE_ROOT_CAPABILITY;

//PCI_EXPRESS_ROOT_STATUS_REGISTER in ntddk.h
typedef struct {
    UINT32 PME_RequesterId : 16;
    UINT32 PME_Status : 1;
    UINT32 PME_Pending : 1;
    UINT32 Reserved : 14;
}PCIE_ROOT_STATUS;

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
}PCIE_DEVICE_CAPABILITY2;

typedef struct 
{
    PCI_CAP_HEADER Header;
    PCIE_CAPABILITY_REG PCIeCap;
    PCIE_DEVICE_CAPABILITY DevCap;
    PCIE_DEVICE_CONTROL DevCtrl;
    PCIE_DEVICE_STATUS DevStatus;
    PCIE_LINK_CAPABILITY LinkCap;
    PCIE_LINK_CONTROL LinkCtrl;
    PCIE_LINK_STATUS LinkStatus;
    PCIE_SLOT_CAPABILITY SlotCap;
    PCIE_SLOT_CONTROL SlotCtrl;
    PCIE_SLOT_STATUS SlotStatus;
    PCIE_ROOT_CONTROL RootCtrl;
    PCIE_ROOT_CAPABILITY RootCap;
    PCIE_ROOT_STATUS RootStatus;
    PCIE_DEVICE_CAPABILITY2 DevCap2;
    //PCIE_DEVICE_CONTROL2 DevCtrl2;
    //PCIE_DEVICE_STATUS2 DevStatus2;
    UINT16 DevCtrl2;
    UINT16 DevStatus2;
}PCIE_ENHANCED_CAP, * PPCIE_ENHANCED_CAP;
#pragma pack()
