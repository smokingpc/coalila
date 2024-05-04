// GetPcieCap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include "..\common\PCI_EnumAndConsts.h"
#include "..\common\PCI_Structures.h"
#include "..\common\PCIe_EnumAndConsts.h"
#include "..\common\PCIe_Structures.h"
#include "..\common\IoctlCmd.h"
#include "..\common\DllExport.h"

void Usage()
{
    _tprintf(_T("GetPcieCap.exe is a tool to retrieve PCIe CAP from spcified Device.\n"));
    _tprintf(_T("Device is specified by AcpiDomain(Segment)/BusId/DeviceId/FunctionId (BDF).\n"));
    _tprintf(_T("BDF id are decimal number.\n"));
    _tprintf(_T("Format: GetPcieCap.exe <Domain> <BusID> <DevID> <FuncID>\n"));
    _tprintf(_T("e.g.\n"));
    _tprintf(_T("Assume you want to get PCIe CAP from device (Domain 0, Bus 02, Device 31, Function 2):\n"));
    _tprintf(_T("  GetMsiXCap.exe 0 2 31 2\n\n"));
}

void PrintPCIeCap(PCIE_CAPABILITIES* cap)
{
    _tprintf(_T("[PCIe CAP]\n"));
    _tprintf(_T("CapID=%X, Next=%X\n"), cap->Header.CapabilityID, cap->Header.Next);

    _tprintf(_T("cap->PCIeCap=>\n"));
    _tprintf(_T("Version=%d, PortType=%d, SlotImplement=%d, InterruptMsgNumber=0x%04X\n\n"), 
        cap->PCIeCap.CapabilityVersion, cap->PCIeCap.DeviceType, cap->PCIeCap.SlotImplemented, cap->PCIeCap.InterruptMessageNumber);

    _tprintf(_T("cap->DevCap=>\n"));
    _tprintf(_T("MaxPayloadSupport=%d, PhantomFunctionSupport=%d, ExtendTagFieldSupport=%d,\n"),
        cap->DevCap.MaxPayloadSizeSupported, cap->DevCap.PhantomFunctionsSupported, cap->DevCap.ExtendedTagSupported);
    _tprintf(_T("EndpointL0AcceptLatency=%d, EndpointL1AcceptLatency=%d, RoleBasedErrorReport=%d,\n"),
        cap->DevCap.L0sAcceptableLatency, cap->DevCap.L1AcceptableLatency, cap->DevCap.RoleBasedErrorReporting);
    _tprintf(_T("ErrCorSubclassCap=%d, SlotPowerLimitValue=%d, SlotPowerLimitScale=%d, FuncLevelResetCap=%d\n\n"),
        cap->DevCap.ErrCorSubclassCap, cap->DevCap.CapturedSlotPowerLimit, cap->DevCap.CapturedSlotPowerLimitScale, cap->DevCap.FunctionLevelResetCapability);

    _tprintf(_T("cap->DevCtrl=>\n"));
    _tprintf(_T("CorrectableErrorReport=%d, NonFatalErrorReport=%d, FatalErrorReport=%d,\n"),
        cap->DevCtrl.u1.CorrectableErrorEnable, cap->DevCtrl.u1.NonFatalErrorEnable, cap->DevCtrl.u1.FatalErrorEnable);
    _tprintf(_T("UnsupportRequestReport=%d, RelaxedOrdering=%d, MaxPayloadSize=%d,\n"),
        cap->DevCtrl.u1.UnsupportedRequestErrorEnable, cap->DevCtrl.u1.EnableRelaxedOrder, cap->DevCtrl.u1.MaxPayloadSize);
    _tprintf(_T("ExtendTagField=%d, PhantomFunction=%d, AuxPowerPM=%d,\n"),
        cap->DevCtrl.u1.ExtendedTagEnable, cap->DevCtrl.u1.PhantomFunctionsEnable, cap->DevCtrl.u1.AuxPowerEnable);
    _tprintf(_T("NoSnoop=%d, MaxReadRequestSize=%d, BCR_IFLR=%d\n\n"),
        cap->DevCtrl.u1.NoSnoopEnable, cap->DevCtrl.u1.MaxReadRequestSize, cap->DevCtrl.u1.BCR_IFLR);

    _tprintf(_T("cap->DevStatus=>\n"));
    _tprintf(_T("CorrectableError=%d, NonFatalError=%d, FatalError=%d, UnsupportedRequest=%d,\n"),
        cap->DevStatus.CorrectableErrorDetected, cap->DevStatus.NonFatalErrorDetected, cap->DevStatus.FatalErrorDetected, cap->DevStatus.UnsupportedRequestDetected);
    _tprintf(_T("AuxPower=%d, TransactionPending=%d, EmergencyPowerReduction=%d\n\n"),
        cap->DevStatus.AuxPowerDetected, cap->DevStatus.TransactionsPending, cap->DevStatus.EmergencyPowerReduction);

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
        cap->SlotCtrl.AttentionButtonEnable, cap->SlotCtrl.PowerFaultDetectEnable, cap->SlotCtrl.MRLSensorEnable);
    _tprintf(_T("PresenceDetectChanged=%d, CommandCompletedInterrupt=%d, HotPlugInterrupt=%d,\n"),
        cap->SlotCtrl.PresenceDetectEnable, cap->SlotCtrl.CommandCompletedEnable, cap->SlotCtrl.HotPlugInterruptEnable);
    _tprintf(_T("AttentionIndicator=%d, PowerIndicator=%d, PowerControl=%d,\n"),
        cap->SlotCtrl.AttentionIndicatorControl, cap->SlotCtrl.PowerIndicatorControl, cap->SlotCtrl.PowerControllerControl);
    _tprintf(_T("EletromechanicalInterLock=%d, DataLinkLayerStateChanged=%d,\n"),
        cap->SlotCtrl.ElectromechanicalLockControl, cap->SlotCtrl.DataLinkStateChangeEnable);

    _tprintf(_T("cap->SlotStatus=>\n"));
    _tprintf(_T("AttentionButton=%d, PowerFault=%d, MRL_SensorChanged=%d,\n"),
        cap->SlotStatus.AttentionButton, cap->SlotStatus.PowerFault, cap->SlotStatus.MRL_SensorChanged);
    _tprintf(_T("PresenceDetectChanged=%d, CommandCompleted=%d, MRL_Sensor=%d,\n"),
        cap->SlotStatus.PresenceDetectChanged, cap->SlotStatus.CommandCompleted, cap->SlotStatus.MRL_Sensor);
    _tprintf(_T("PresenceDetect=%d, EletromechanicalInterLock=%d, DataLinkLayerStateChanged=%d\n\n"),
        cap->SlotStatus.PresenceDetect, cap->SlotStatus.EletromechanicalInterLock, cap->SlotStatus.DataLinkLayerStateChanged);

    _tprintf(_T("RootCap.CRS_SoftwareVisibility=%d\n"), cap->RootCap.CRS_SoftwareVisibility);
    _tprintf(_T("RootStatus=0x%08X\n"), *((UINT32*)&cap->RootStatus));
    _tprintf(_T("DevCap2=0x%08X\n"), *((UINT32*)&cap->DevCap2));
    _tprintf(_T("DevCtrl2=0x%04X\n"), *((USHORT*)&cap->DevCtrl2));
    _tprintf(_T("DevStatus2=0x%04X\n"), cap->DevStatus2);
}


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 5)
    {
        Usage();
        return -1;
    }

    PCIE_CAPABILITIES cap = { 0 };
    DWORD error = ERROR_SUCCESS;
    int domain = _tstoi(argv[1]);
    int bus_id = _tstoi(argv[2]);
    int dev_id = _tstoi(argv[3]);
    int func_id = _tstoi(argv[4]);

    _tprintf(_T("getting PCIeCap for Device BDF(%d.%d.%d.%d)\n"), domain, bus_id, dev_id, func_id);
    error = ReadPCIeCap(domain, bus_id, dev_id, func_id, &cap);
    if(ERROR_SUCCESS != error)
        _tprintf(_T("ReadPCIeCap() failed. LastError=%d\n"), GetLastError());
    else
        PrintPCIeCap(&cap);

    return 0;
}

