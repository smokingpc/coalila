#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <tchar.h>
#include <windows.h>
//#include <ntdef.h>
#include <winioctl.h>

#include "..\common\IoctlCmd.h"
#include "..\common\PCI_PCIe_Caps.h"
#include "..\common\DllExport.h"

#include "InternalFunction.h"
