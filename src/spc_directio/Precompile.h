#pragma once

#include <wdm.h>
#include <aux_klib.h>
#include "Constant.h"
#include "ACPI_Structures.h"
#include "DeviceExtension.h"
#include "AutoLock.h"
#include "ACPI_Functions.h"
#include "..\common\IoctlCmd.h"
#include "DirectIo.h"   //Direct ReadPort, Direc tWritePort...etc.
#include "IrpHandlers.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "PciDevice.h"     //Scan PCI devices, Read PCI Cap....etc.


