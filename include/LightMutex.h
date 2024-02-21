#ifndef WINTYPES_INCLUDE_LIGHTMUTEX_H
#define WINTYPES_INCLUDE_LIGHTMUTEX_H

// Forward declaration to not expose Windows headers
struct _RTL_CRITICAL_SECTION;
using CRITICAL_SECTION = _RTL_CRITICAL_SECTION;

namespace WinTypes {
    // Light mutex that wraps a pointer to a Windows Critical Section
    // Satisfies the C++ Mutex requirement
    // Recursive, not TimedLockable
    class LightMutex {
        mutable CRITICAL_SECTION* criticalSection;
    public:
        LightMutex();
        ~LightMutex() noexcept;

        void lock() noexcept;
        void unlock() noexcept;
        bool try_lock() noexcept;

        // Deleted as per the Mutex requirement
        LightMutex(const LightMutex&) = delete;
        LightMutex(LightMutex&&) = delete;
    };
}

#endif /* WINTYPES_INCLUDE_LIGHTMUTEX_H */
