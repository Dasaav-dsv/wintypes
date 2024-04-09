#ifndef WINTYPES_INCLUDE_NAMEDRESOURCE_H
#define WINTYPES_INCLUDE_NAMEDRESOURCE_H

#include <mutex>
#include <NamedMutex.h>
#include <SharedMutex.h>
#include <NamedResource.inl>

namespace WinTypes {
    // Get current module's handle and increment its reference count
    HANDLE refCurrentModule() noexcept;

    // Increment a module's reference count
    void refModule(HANDLE hModule) noexcept;
    // Decrement a module's reference count
    void unrefModule(HANDLE hModule) noexcept;

    // Acquire or create a resource mapping structure
    NamedResourceMap<void>* acquireResourceMapping(const wchar_t* name) noexcept;

    // A named resource meant for sharing between all modules of a process
    template <typename T>
    class NamedResource {
        using resmap_t = NamedResourceMap<T>;
        resmap_t* resmap;
    public:
        // Resource access
        T& operator *() noexcept { return *this->resmap->resptr; }
        T* operator ->() noexcept { return this->resmap->resptr; }
        T& get() noexcept { return *this->resmap->resptr; }

        template <typename...Args>
        NamedResource(const wchar_t* name, Args&&...args) noexcept {
            // OS-backed file mapped memory is zero initialized
            this->resmap = reinterpret_cast<resmap_t*>(acquireResourceMapping(name));
            if (!this->resmap->owner) {
                // We can use the fact the default SRW state is also zero
                // Therefore it is always initialized after mapping memory
                std::unique_lock lock(this->resmap->srw);
                if (!this->resmap->owner) {
                    this->resmap->owner = refCurrentModule();
                    this->resmap->resptr = new T(std::forward<Args>(args)...);
                    return;
                }
            }
            refModule(this->resmap->owner);
        }

        ~NamedResource() noexcept {
            // Keeping the module referenced until all resources are destroyed
            // This prevents a DLL from being unloaded while the resource is in use
            unrefModule(this->resmap->owner);
        }

        // Lock resource in Exclusive/Write mode
        void lock() noexcept {
            this->resmap->srw.lock();
        }
        
        // Unlock resource in Exclusive/Write mode
        void unlock() noexcept {
            this->resmap->srw.unlock();
        }
        
        // Try lock resource in Exclusive/Write mode
        bool try_lock() noexcept {
            return this->resmap->srw.try_lock();
        }

        // Lock resource in Shared/Read mode
        void lock_shared() noexcept {
            this->resmap->srw.lock_shared();
        }
        
        // Unlock resource in Shared/Read mode
        void unlock_shared() noexcept {
            this->resmap->srw.unlock_shared();
        }
        
        // Try lock resource in Shared/Read mode
        bool try_lock_shared() noexcept {
            return this->resmap->srw.try_lock_shared();
        }

        // Deleted as per the Mutex requirement
        NamedResource(const NamedResource&) = delete;
        NamedResource(NamedResource&&) = delete;
    };
}

#endif /* WINTYPES_INCLUDE_NAMEDRESOURCE_H */