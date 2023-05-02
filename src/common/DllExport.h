#pragma once

#ifdef SPCPCILIB_EXPORTS
#define GRAID_DLLEXPORT __declspec(dllexport)
#else
#define GRAID_DLLEXPORT DECLSPEC_IMPORT
#endif


EXTERN_C_START
GRAID_DLLEXPORT BOOLEAN DirectIoReadPort(DIRECTIO_READ* request, ULONG* out_data);
GRAID_DLLEXPORT BOOLEAN DirectIoWritePort(DIRECTIO_WRITE* request);
GRAID_DLLEXPORT BOOLEAN DirectIoReadAddr(DIRECTIO_READ* request, ULONG_PTR* out_data);
GRAID_DLLEXPORT BOOLEAN DirectIoWriteAddr(DIRECTIO_WRITE* request);
GRAID_DLLEXPORT BOOLEAN ReadPCIeCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCIE_CAP* result);
GRAID_DLLEXPORT BOOLEAN ReadMsiCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSI_CAP* result);
GRAID_DLLEXPORT BOOLEAN ReadMsixCap(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, PCI_MSIX_CAP* result);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotAttentionIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotPowerIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotPowerControl(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
EXTERN_C_END

