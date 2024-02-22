#include <ibo32.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// First three PEB (Process Environment Block) fields
struct ProcessEnvironmentBlockExcerpt {
    char flags[3];
    void* mutant;
    void* imageBaseAddress;
};

void* getImageBase() noexcept {
    // Access the PEB (Process Environment Block) inside the TEB (Thread Environment Block)
#if defined(__x86_64__) || defined(_WIN64) // 64-bit
    auto pPEB = reinterpret_cast<ProcessEnvironmentBlockExcerpt*>(__readgsqword(0x60));
#else // 32-bit
    auto pPEB = reinterpret_cast<ProcessEnvironmentBlockExcerpt*>(__readfsdword(0x30));
#endif
    return pPEB->imageBaseAddress;
}