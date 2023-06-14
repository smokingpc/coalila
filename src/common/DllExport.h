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
GRAID_DLLEXPORT BOOLEAN ReadPciCfgHeader(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, READ_PCI_CFGHEADER *result);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotAttentionIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotPowerIndicator(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
GRAID_DLLEXPORT BOOLEAN SetPCIeSlotPowerControl(UCHAR bus_id, UCHAR dev_id, UCHAR func_id, BOOLEAN value);
EXTERN_C_END

