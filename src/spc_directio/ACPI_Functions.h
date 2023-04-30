#pragma once
NTSTATUS SetupAcpiInfo(PSPCDIO_DEVEXT devext);
void TeardownAcpiInfo(PSPCDIO_DEVEXT devext);
NTSTATUS EnumAcpiMcfgTable(PSPCDIO_DEVEXT devext);
NTSTATUS LoadAcpiMcfgTable(PSPCDIO_DEVEXT devext);
