#pragma once

//Generic SpinLock
class CAutoSpinLock
{
public:
    CAutoSpinLock(KSPIN_LOCK* lock);
    ~CAutoSpinLock();
private:
    KSPIN_LOCK* Lock = NULL;
    KIRQL OldIrql = PASSIVE_LEVEL;
};
