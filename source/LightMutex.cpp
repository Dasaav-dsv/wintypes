#include <LightMutex.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace WinTypes {
    LightMutex::LightMutex()
        : criticalSection(new CRITICAL_SECTION) {
        InitializeCriticalSection(this->criticalSection);
    }

    LightMutex::~LightMutex() noexcept {
        DeleteCriticalSection(this->criticalSection);
        delete this->criticalSection;
    }

    void LightMutex::lock() noexcept {
        EnterCriticalSection(this->criticalSection);
    }

    void LightMutex::unlock() noexcept {
        LeaveCriticalSection(this->criticalSection);
    }

    bool LightMutex::try_lock() noexcept {
        return TryEnterCriticalSection(this->criticalSection);
    }
}