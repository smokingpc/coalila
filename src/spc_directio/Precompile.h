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


#include <ntddk.h>
#include <wdm.h>
#include <aux_klib.h>
#include <acpitabl.h>
#include "Constant.h"
#include "DeviceExtension.h"
#include "AutoLock.h"
#include "DirectIo.h"   //Direct ReadPort, Direc tWritePort...etc.
#include "IrpHandlers.h"
#include "..\common\PCI_EnumAndConsts.h"
#include "..\common\PCI_Structures.h"
#include "..\common\PCIe_EnumAndConsts.h"
#include "..\common\PCIe_Structures.h"
#include "..\common\MSIX.h"
#include "..\common\IoctlCmd.h"

#include "PciDevice.h"     //Scan PCI devices, Read PCI Cap....etc.
#include "Utils.h"

