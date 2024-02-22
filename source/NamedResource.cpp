#include <NamedResource.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace WinTypes {
    HANDLE refCurrentModule() noexcept {
        HMODULE hModule = NULL;
        // Use the address of this function to get the module it is in
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&refCurrentModule, &hModule);
        return hModule;
    }

    void refModule(HANDLE hModule) noexcept {
        // Module handles are pseudohandles (actually just the module base address)
        // GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS increments reference count
        HMODULE hDiscard = NULL;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)hModule, &hDiscard);
    }

    void unrefModule(HANDLE hModule) noexcept {
        // Standard way of decrementing a module's refcount
        FreeLibrary(reinterpret_cast<HMODULE>(hModule));
    }

    NamedResourceMap<void>* acquireResourceMapping(const wchar_t* name) noexcept {
        constexpr size_t resmapSize = sizeof(NamedResourceMap<void>);
        // Create a program-wide memory mapping
        HANDLE fileHandle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, resmapSize, name);
		auto mappedMemory = reinterpret_cast<NamedResourceMap<void>*>(
            MapViewOfFile(fileHandle, FILE_MAP_ALL_ACCESS, 0, 0, resmapSize));
        return mappedMemory;
    }
}
