#include <ProtectMemory.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <stdexcept>

namespace WinTypes {
    DWORD ProtectMemory::winVirtualProtect(void* address, DWORD size, DWORD newProtect) {
        DWORD oldProtect;
        if (!VirtualProtect(address, size, newProtect, &oldProtect))
            throw std::invalid_argument("Invalid VirtualProtect argument");
        return oldProtect;
    }

    void ProtectMemory::lock() {
        this->oldProtect = this->callback(this->address, static_cast<DWORD>(this->size), this->newProtect);
    }

    void ProtectMemory::unlock() {
        this->callback(this->address, static_cast<DWORD>(this->size), this->oldProtect);
    }

    bool ProtectMemory::try_lock() noexcept {
        try {
            this->lock();
        }
        catch(...) {
            return false;
        }
        return true;
    }
}
