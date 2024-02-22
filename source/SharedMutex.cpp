#include <SharedMutex.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace WinTypes {
    SharedMutex::SharedMutex() noexcept : SRWLock(NULL) {}

    void SharedMutex::lock() noexcept {
        AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }

    void SharedMutex::unlock() noexcept {
        ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }

    bool SharedMutex::try_lock() noexcept {
        return TryAcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }

    void SharedMutex::lock_shared() noexcept {
        AcquireSRWLockShared(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }

    void SharedMutex::unlock_shared() noexcept {
        ReleaseSRWLockShared(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }

    bool SharedMutex::try_lock_shared() noexcept {
        return TryAcquireSRWLockShared(reinterpret_cast<PSRWLOCK>(&this->SRWLock));
    }
}