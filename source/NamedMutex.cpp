#include <NamedMutex.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace WinTypes {
    NamedMutex::NamedMutex(const wchar_t* name) noexcept
        : nativeHandle(CreateMutexW(NULL, FALSE, name)) {}

    void NamedMutex::lock() noexcept {
        WaitForSingleObject(this->nativeHandle, -1);
    }

    void NamedMutex::unlock() noexcept {
        ReleaseMutex(this->nativeHandle);
    }

    bool NamedMutex::try_lock() noexcept {
        // Try to acquire lock with no timeout
        return WaitForSingleObject(this->nativeHandle, 0) == WAIT_OBJECT_0;
    }
}