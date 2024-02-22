#ifndef WINTYPES_INCLUDE_PROTECTMEMORY_H
#define WINTYPES_INCLUDE_PROTECTMEMORY_H

using DWORD = unsigned long;

namespace WinTypes {
    // Callback type
    using ProtectMemoryCallback = DWORD(*)(void* address, DWORD size, DWORD newProtect);

    // Windows virtual memory protection modifier
    // Uses mutex semantics and can be used in e.g. std::scoped_lock
    // Accepts a reference or an address pointer & size pair
    class ProtectMemory {
        ProtectMemoryCallback callback;
        DWORD newProtect;
        DWORD oldProtect;
        void* address;
        size_t size;
    public:
        // WINAPI basic VirtualProtect callback
        // Throws if protection is impossible (invalid arguments)
        static DWORD winVirtualProtect(void* address, DWORD size, DWORD newProtect);

        // Address pointer & size constructor
        ProtectMemory(void* address, size_t size, DWORD newProtect, ProtectMemoryCallback callback = ProtectMemory::winVirtualProtect)
            : callback(callback), newProtect(newProtect), address(address), size(size) {}

        // Instance constructor (inferred size)
        template <typename T>
        ProtectMemory(T& instance, DWORD newProtect, ProtectMemoryCallback callback = ProtectMemory::winVirtualProtect)
            : callback(callback), newProtect(newProtect), address((void*)&instance), size(sizeof(T)) {}

        // Mutex-like memory protection - apply new    
        void lock();
        // Mutex like memory protection - restore
        void unlock();
        // Mutex like memory protection - try apply new (nothrow)
        bool try_lock() noexcept;

        // Noncopyable
        ProtectMemory(const ProtectMemory&) = delete;
        void operator = (const ProtectMemory&) = delete;

        // Movable
        ProtectMemory(ProtectMemory&& other) noexcept
            : callback(other.callback), newProtect(other.newProtect), oldProtect(other.oldProtect), address(other.address), size(other.size) {}
    };
}

#endif /* WINTYPES_INCLUDE_PROTECTMEMORY_H */
