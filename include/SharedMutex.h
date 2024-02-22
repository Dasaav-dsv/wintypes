#ifndef WINTYPES_INCLUDE_SHAREDLOCK_H
#define WINTYPES_INCLUDE_SHAREDLOCK_H

namespace WinTypes {
    // Light mutex that wraps a Windows Slim Read/Write lock
    // Use with std::shared_lock for Shared/Read
    // and std::unique_lock for Exclusive/Write access
    // Satisfies the C++ SharedMutex requirement
    // NOT Recursive, not TimedLockable
    class SharedMutex {
        // A WINAPI SRWLock is the size of a pointer
        void* SRWLock;
    public:
        SharedMutex() noexcept;
        ~SharedMutex() = default;

        // Lock in Exclusive/Write mode
        void lock() noexcept;
        // Unlock in Exclusive/Write mode
        void unlock() noexcept;
        // Try lock in Exclusive/Write mode
        bool try_lock() noexcept;

        // Lock in Shared/Read mode
        void lock_shared() noexcept;
        // Unlock in Shared/Read mode
        void unlock_shared() noexcept;
        // Try lock in Shared/Read mode
        bool try_lock_shared() noexcept;

        // Deleted as per the Mutex requirement
        SharedMutex(const SharedMutex&) = delete;
        SharedMutex(SharedMutex&&) = delete;
    };
}

#endif /* WINTYPES_INCLUDE_SHAREDLOCK_H */
