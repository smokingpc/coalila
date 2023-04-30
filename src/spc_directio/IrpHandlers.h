#pragma once

DRIVER_DISPATCH     DeviceIoControlHandler;
DRIVER_DISPATCH     IrpDefaultHandler;
DRIVER_DISPATCH     IrpCreateCloseHandler;

void InitIrpHandlers(_In_ PDRIVER_OBJECT driver);
