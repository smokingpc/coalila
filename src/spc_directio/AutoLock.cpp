#include "Precompile.h"

CSpinLock::CSpinLock(KSPIN_LOCK* lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If protect it here, driver could have timing issue because SpinLock not working.
    //It is more difficult to debug.
    this->Lock = lock;
    KeAcquireSpinLock(this->Lock, &this->OldIrql);
}
CSpinLock::~CSpinLock()
{
    KeReleaseSpinLock(this->Lock, this->OldIrql);
    this->Lock = NULL;
}

CQueuedSpinLock::CQueuedSpinLock(KSPIN_LOCK* lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If protect it here, driver could have timing issue because SpinLock not working.
    //It is more difficult to debug.
    this->Lock = lock;
    KeAcquireInStackQueuedSpinLock(this->Lock, &this->Handle);
}
CQueuedSpinLock::~CQueuedSpinLock()
{
    KeReleaseInStackQueuedSpinLock(&this->Handle);
    this->Lock = NULL;
}

