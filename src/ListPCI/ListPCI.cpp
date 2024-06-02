#include "Precompile.h"


DWORD QueryPCI(
    IN OUT PPCIDEV_COMMON_CONFIG result,
    IN USHORT domain,
    IN UCHAR bus,
    IN UCHAR dev,
    IN UCHAR func)
{
    RtlZeroMemory(result, sizeof(PCIDEV_COMMON_CONFIG));
    return ReadPciCfgHeader(domain, bus, dev, func, result);
}
