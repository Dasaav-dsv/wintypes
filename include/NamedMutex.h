#ifndef WINTYPES_INCLUDE_NAMEDMUTEX_H
#define WINTYPES_INCLUDE_NAMEDMUTEX_H

// Forward declaration to not expose Windows headers
using HANDLE = void*;

namespace WinTypes {
    // Program-wide mutex that wraps a native Windows mutex
    // Does not fully satisfy the C++ Mutex requirement (not default constructible)
    // Recursive, not TimedLockable
    class NamedMutex {
        mutable HANDLE nativeHandle;
    public:
        // Named mutex constructor for a program scope mutex
        // A mutex can be globally accessed through its name
        NamedMutex(const wchar_t* name) noexcept;
        ~NamedMutex() = default;

        void lock() noexcept;
        void unlock() noexcept;
        bool try_lock() noexcept;

        // Deleted as per the Mutex requirement
        NamedMutex(const NamedMutex&) = delete;
        NamedMutex(NamedMutex&&) = delete;
    };
}

#endif /* WINTYPES_INCLUDE_NAMEDMUTEX_H */
