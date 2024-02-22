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
            this->resmap = reinterpret_cast<resmap_t*>acquireResourceMapping(name);
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
    };
}

#endif /* WINTYPES_INCLUDE_NAMEDRESOURCE_H */
