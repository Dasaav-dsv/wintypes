#include <ibo32.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// First three PEB (Process Environment Block) fields
struct ProcessEnvironmentBlockExcerpt {
    char flags[8];
    void* mutant;
    void* imageBaseAddress;
};

void* getImageBase() noexcept {
    // Access the PEB (Process Environment Block) inside the TEB (Thread Environment Block)
    auto pPEB = reinterpret_cast<ProcessEnvironmentBlockExcerpt*>(__readgsqword(0x60));
    return pPEB->imageBaseAddress;
}