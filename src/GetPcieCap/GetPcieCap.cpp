// GetPcieCap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("GetPcieCap.exe is a tool to retrieve PCIe CAP from spcified Device.\n"));
    _tprintf(_T("Device is specified by BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetPcieCap.exe <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get PCIe CAP from device (Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 2 31 2\n\n"));
}

void PrintPCIeCap(PCIE_CAP* cap)
{
    _tprintf(_T("[PCIe CAP]\n"));
    _tprintf(_T("CapID=%X, Next=%X\n"), cap->Header.CapabilityID, cap->Header.Next);

    _tprintf(_T("cap->PCIeCap=>\n"));
    _tprintf(_T("Version=%d, PortType=%d, SlotImplement=%d, InterruptMsgNumber=0x%04X\n\n"), 
        cap->PCIeCap.Version, cap->PCIeCap.DevicePortType, cap->PCIeCap.SlotImplement, cap->PCIeCap.InterruptMsgNumber);

    _tprintf(_T("cap->DevCap=>\n"));
    _tprintf(_T("MaxPayloadSupport=%d, PhantomFunctionSupport=%d, ExtendTagFieldSupport=%d,\n"),
        cap->DevCap.MaxPayloadSupport, cap->DevCap.PhantomFunctionSupport, cap->DevCap.ExtendTagFieldSupport);
    _tprintf(_T("EndpointL0AcceptLatency=%d, EndpointL1AcceptLatency=%d, RoleBasedErrorReport=%d,\n"),
        cap->DevCap.EndpointL0AcceptLatency, cap->DevCap.EndpointL1AcceptLatency, cap->DevCap.RoleBasedErrorReport);
    _tprintf(_T("ErrCorSubclassCap=%d, SlotPowerLimitValue=%d, SlotPowerLimitScale=%d, FuncLevelResetCap=%d\n\n"),
        cap->DevCap.ErrCorSubclassCap, cap->DevCap.SlotPowerLimitValue, cap->DevCap.SlotPowerLimitScale, cap->DevCap.FuncLevelResetCap);

    _tprintf(_T("cap->DevCtrl=>\n"));
    _tprintf(_T("CorrectableErrorReport=%d, NonFatalErrorReport=%d, FatalErrorReport=%d,\n"),
        cap->DevCtrl.CorrectableErrorReport, cap->DevCtrl.NonFatalErrorReport, cap->DevCtrl.FatalErrorReport);
    _tprintf(_T("UnsupportRequestReport=%d, RelaxedOrdering=%d, MaxPayloadSize=%d,\n"),
        cap->DevCtrl.UnsupportRequestReport, cap->DevCtrl.RelaxedOrdering, cap->DevCtrl.MaxPayloadSize);
    _tprintf(_T("ExtendTagField=%d, PhantomFunction=%d, AuxPowerPM=%d,\n"),
        cap->DevCtrl.ExtendTagField, cap->DevCtrl.PhantomFunction, cap->DevCtrl.AuxPowerPM);
    _tprintf(_T("NoSnoop=%d, MaxReadRequestSize=%d, BCR_IFLR=%d\n\n"),
        cap->DevCtrl.NoSnoop, cap->DevCtrl.MaxReadRequestSize, cap->DevCtrl.BCR_IFLR);

    _tprintf(_T("cap->DevStatus=>\n"));
    _tprintf(_T("CorrectableError=%d, NonFatalError=%d, FatalError=%d, UnsupportedRequest=%d,\n"),
        cap->DevStatus.CorrectableError, cap->DevStatus.NonFatalError, cap->DevStatus.FatalError, cap->DevStatus.UnsupportedRequest);
    _tprintf(_T("AuxPower=%d, TransactionPending=%d, EmergencyPowerReduction=%d\n\n"),
        cap->DevStatus.AuxPower, cap->DevStatus.TransactionPending, cap->DevStatus.EmergencyPowerReduction);

    _tprintf(_T("cap->LinkCap=>\n"));
    _tprintf(_T("MaxLinkSpeed=%d, MaxLinkWidth=%d, ASPMSupport=%d, L0ExitLatency=%d,\n"),
        cap->LinkCap.MaxLinkSpeed, cap->LinkCap.MaxLinkWidth, cap->LinkCap.ASPMSupport, cap->LinkCap.L0ExitLatency);
    _tprintf(_T("L1ExitLatency=%d, ClockPowerManagement=%d, SurpriseDownErrorReport=%d, DLL_LinkActiveReport=%d,\n"),
        cap->LinkCap.L1ExitLatency, cap->LinkCap.ClockPowerManagement, cap->LinkCap.SurpriseDownErrorReport, cap->LinkCap.DLL_LinkActiveReport);
    _tprintf(_T("LinkBandwidthNotification=%d, ASPMOptionalCompliance=%d, PortNumber=%d\n\n"),
        cap->LinkCap.LinkBandwidthNotification, cap->LinkCap.ASPMOptionalCompliance, cap->LinkCap.PortNumber);

    _tprintf(_T("cap->LinkCtrl=>\n"));
    _tprintf(_T("ASPMControl=%d, ReadCompletionBoundary=%d, LinkDisable=%d, RetrainLink=%d,\n"),
        cap->LinkCtrl.ASPMControl, cap->LinkCtrl.ReadCompletionBoundary, cap->LinkCtrl.LinkDisable, cap->LinkCtrl.RetrainLink);
    _tprintf(_T("CommonClockConfig=%d, ExtendedSync=%d, ClockPowerManagement=%d, HwAutonomousWidthDisable=%d,\n"),
        cap->LinkCtrl.CommonClockConfig, cap->LinkCtrl.ExtendedSync, cap->LinkCtrl.ClockPowerManagement, cap->LinkCtrl.HwAutonomousWidthDisable);
    _tprintf(_T("LinkBandwidthMgmtInterrupt=%d, LinkAutoBandwidthInterrupt=%d, DRSSignalControl=%d\n\n"),
        cap->LinkCtrl.LinkBandwidthMgmtInterrupt, cap->LinkCtrl.LinkAutoBandwidthInterrupt, cap->LinkCtrl.DRSSignalControl);

    _tprintf(_T("cap->LinkStatus=>\n"));
    _tprintf(_T("LinkSpeed=%d, LinkWidth=%d, LinkTraining=%d, SlotClockConfig=%d,\n"),
        cap->LinkStatus.LinkSpeed, cap->LinkStatus.LinkWidth, cap->LinkStatus.LinkTraining, cap->LinkStatus.SlotClockConfig);
    _tprintf(_T("DataLinkLayerLinkActive=%d, LinkBandwidthMgmt=%d, LinkAutoBandwidth=%d\n\n"),
        cap->LinkStatus.DataLinkLayerLinkActive, cap->LinkStatus.LinkBandwidthMgmt, cap->LinkStatus.LinkAutoBandwidth);

    _tprintf(_T("cap->SlotCap=>\n"));
    _tprintf(_T("AttentionButtom=%d, PowerConotroller=%d, MRL_Sensor=%d, AttentionIndicator=%d,\n"),
        cap->SlotCap.AttentionButtom, cap->SlotCap.PowerConotroller, cap->SlotCap.MRL_Sensor, cap->SlotCap.AttentionIndicator);
    _tprintf(_T("PowerIndicator=%d, HotPlugSurprise=%d, HotPlugCapable=%d, SlotPowerLimitValue=%d,\n"),
        cap->SlotCap.PowerIndicator, cap->SlotCap.HotPlugSurprise, cap->SlotCap.HotPlugCapable, cap->SlotCap.SlotPowerLimitValue);
    _tprintf(_T("SlotPowerLimitScale=%d, EletromechanicalInterLock=%d, NoCommandCompleted=%d, PhysicalSlotNumber=%d\n\n"),
        cap->SlotCap.SlotPowerLimitScale, cap->SlotCap.EletromechanicalInterLock, cap->SlotCap.NoCommandCompleted, cap->SlotCap.PhysicalSlotNumber);

    _tprintf(_T("cap->SlotCtrl=>\n"));
    _tprintf(_T("AttentionButtonPressed=%d, PowerFaultDetected=%d, MRL_SensorChanged=%d,\n"),
        cap->SlotCtrl.AttentionButtonPressed, cap->SlotCtrl.PowerFaultDetected, cap->SlotCtrl.MRL_SensorChanged);
    _tprintf(_T("PresenceDetectChanged=%d, CommandCompletedInterrupt=%d, HotPlugInterrupt=%d,\n"),
        cap->SlotCtrl.PresenceDetectChanged, cap->SlotCtrl.CommandCompletedInterrupt, cap->SlotCtrl.HotPlugInterrupt);
    _tprintf(_T("AttentionIndicator=%d, PowerIndicator=%d, PowerControl=%d,\n"),
        cap->SlotCtrl.AttentionIndicator, cap->SlotCtrl.PowerIndicator, cap->SlotCtrl.PowerControl);
    _tprintf(_T("EletromechanicalInterLock=%d, DataLinkLayerStateChanged=%d, AutoSlotPowerLimitDisable=%d,n"),
        cap->SlotCtrl.EletromechanicalInterLock, cap->SlotCtrl.DataLinkLayerStateChanged, cap->SlotCtrl.AutoSlotPowerLimitDisable);
    _tprintf(_T("InBandPDDisable=%d\n\n"),
        cap->SlotCtrl.InBandPDDisable);

    _tprintf(_T("cap->SlotStatus=>\n"));
    _tprintf(_T("AttentionButton=%d, PowerFault=%d, MRL_SensorChanged=%d,\n"),
        cap->SlotStatus.AttentionButton, cap->SlotStatus.PowerFault, cap->SlotStatus.MRL_SensorChanged);
    _tprintf(_T("PresenceDetectChanged=%d, CommandCompleted=%d, MRL_Sensor=%d,\n"),
        cap->SlotStatus.PresenceDetectChanged, cap->SlotStatus.CommandCompleted, cap->SlotStatus.MRL_Sensor);
    _tprintf(_T("PresenceDetect=%d, EletromechanicalInterLock=%d, DataLinkLayerStateChanged=%d\n\n"),
        cap->SlotStatus.PresenceDetect, cap->SlotStatus.EletromechanicalInterLock, cap->SlotStatus.DataLinkLayerStateChanged);

    _tprintf(_T("RootCap=0x%04X\n"), cap->RootCap);
    _tprintf(_T("RootStatus=0x%08X\n"), cap->RootStatus);
    _tprintf(_T("DevCap2=0x%08X\n"), cap->DevCap2);
    _tprintf(_T("DevCtrl2=0x%04X\n"), cap->DevCtrl2);
    _tprintf(_T("DevStatus2=0x%04X\n"), cap->DevStatus2);
}


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 4)
    {
        Usage();
        return -1;
    }

    int bus_id = _tstoi(argv[1]);
    int dev_id = _tstoi(argv[2]);
    int func_id = _tstoi(argv[3]);
    PCIE_CAP cap = { 0 };

    _tprintf(_T("getting PCIeCap for Device BDF(%d,%d,%d)\n"), bus_id, dev_id, func_id);
    if (!ReadPCIeCap(bus_id, dev_id, func_id, &cap))
        _tprintf(_T("ReadPCIeCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintPCIeCap(&cap);

    return 0;
}

