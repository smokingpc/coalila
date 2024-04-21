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


#define DIRECTIO_FILE L"\\\\.\\SPC_DIRECTIO"

//DIO stands for "Direct Io"
#define DIO_DEVTYPE     0x00008001  //device type 0~32767 are used for MS. 32768~65535 are reserved for customize.

#define DIO_READ_PORT       (100)
#define DIO_WRITE_PORT      (101)
#define DIO_READ_REG        (102)
#define DIO_WRITE_REG       (103)

//return PCIe VENID and DEVID array
#define DIO_READ_PCIHEADER  (120)
//read specified CAPABILITY block which followed after PCIDEV_CONFIG_SPACE
#define DIO_READ_CAP        (121)
#define DIO_PCIE_SLOT_CTRL  (140)
#define DIO_PCIE_LINK_CTRL  (180)

//following macro can fetch corresponding fields from IOCTL codes
//DEVICE_TYPE_FROM_CTL_CODE
//METHOD_FROM_CTL_CODE

#define IOCTL_READ_PORT     CTL_CODE(DIO_DEVTYPE, DIO_READ_PORT, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE_PORT    CTL_CODE(DIO_DEVTYPE, DIO_WRITE_PORT, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_READ_ADDR     CTL_CODE(DIO_DEVTYPE, DIO_READ_REG, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_WRITE_ADDR    CTL_CODE(DIO_DEVTYPE, DIO_WRITE_REG, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_READ_PCIHEADER  CTL_CODE(DIO_DEVTYPE, DIO_READ_PCIHEADER, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_READ_CAP      CTL_CODE(DIO_DEVTYPE, DIO_READ_CAP, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_PCIE_SLOT_CTRL    CTL_CODE(DIO_DEVTYPE, DIO_PCIE_SLOT_CTRL, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_PCIE_LINK_CTRL    CTL_CODE(DIO_DEVTYPE, DIO_PCIE_LINK_CTRL, METHOD_BUFFERED, FILE_READ_ACCESS)

enum class IO_SIZE {
    UNKNOWN = 0,
    BYTE = sizeof(UCHAR),
    USHORT = sizeof(USHORT),
    ULONG = sizeof(ULONG),
    UINT64 = sizeof(UINT64)
};

enum class SLOT_CTRL_FIELD {
    UNKNOWN = 0,
    ATT_INDICATOR = 1,      //Attention Indicator   (led)
    PWR_INDICATOR = 2,      //Power Indicator   (led)
    PWR_CONTROL = 3,        //Slot Power on/off
};

enum class LINK_CTRL_FIELD {
    UNKNOWN = 0,
    ASPM = 1,                   //ASPMControl field.
    LINK_DISABLE = 3,           //LinkDisable field.
    RETRAIN_LINK = 4,           //RetrainLink field.
    //todo: add all fields of PCIE_LINK_CONTROL
};

//for IOCTL_READPORT
typedef struct {
    ULONG_PTR Address;
    IO_SIZE IoSize;
}DIRECTIO_READ, *PDIRECTIO_READ;

//for IOCTL_WRITEPORT
typedef struct {
    ULONG_PTR Address;
    IO_SIZE IoSize;
    ULONG_PTR Data;
}DIRECTIO_WRITE, * PDIRECTIO_WRITE;

//for IOCTL_READ_PCIHEADER
typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7
}READ_PCI_CFGHEADER;

//for IOCTL_READ_CAP
typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7
    PCI_CAPID CapID;
}READ_PCI_CAP;

typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7

    SLOT_CTRL_FIELD Target;
    union SLOT_CTRL_VALUE
    {
        BOOLEAN OnOff;
        INDICATOR_STATE Indicator;
    } u;
}SET_PCIE_SLOT_CONTROL;

typedef struct {
    USHORT Segment; //0~65535, UEFI defined segment. it is called "Domain" in Linux.
    UCHAR BusId;    //0~255
    UCHAR DevId;    //0~32
    UCHAR FuncId;   //0~7

    LINK_CTRL_FIELD Target;
    union LINK_CTRL_VALUE
    {
        USHORT RawValue;
        ASPM_CTRL Aspm;   //Active State Power Management
        BOOLEAN LinkDisable;    //default FALSE. TRUE indicates "Disable Link"
        BOOLEAN Retrain;        //TRUE indicates ReTrain Link now.
    } u;
}SET_PCIE_LINK_CONTROL;
