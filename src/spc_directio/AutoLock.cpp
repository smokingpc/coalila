#include "Precompile.h"

CAutoSpinLock::CAutoSpinLock(KSPIN_LOCK* lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If protect it here, driver could have timing issue because SpinLock not working.
    //It is more difficult to debug.
    this->Lock = lock;
    KeAcquireSpinLock(this->Lock, &this->OldIrql);
}
CAutoSpinLock::~CAutoSpinLock()
{
    KeReleaseSpinLock(this->Lock, this->OldIrql);
    this->Lock = NULL;
}
