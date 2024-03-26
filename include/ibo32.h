#ifndef WINTYPES_INCLUDE_IBO32_H
#define WINTYPES_INCLUDE_IBO32_H

#include <cstddef>
#include <cstdint>

namespace WinTypes {
    void* getImageBase() noexcept;

    // 31-bit integer offset type used by many Windows internals
    // Convertible to and from integers and pointers
    template <typename T>
    struct ibo32_t {
        using pointer_t = T*;
        using const_pointer_t = const T*;

        // Default, integer and pointer constructor
        ibo32_t() noexcept : value(-1) {}
        ibo32_t(int value) noexcept : value(value) {}
        ibo32_t(const_pointer_t address) noexcept
            : value(this->ptrDiff(getImageBase(), address)) {}

        // Assignment operators (integer and pointer)
        void operator = (int rhs) noexcept { this->value = rhs; }
        void operator = (const_pointer_t rhs) noexcept {
            this->value = this->ptrDiff(getImageBase(), rhs);
        }

        // Explicit conversion operators to corresponding pointer types
        explicit operator pointer_t() noexcept { return this->fromImageBase(getImageBase()); }
        explicit operator const_pointer_t() const noexcept { return this->fromImageBase(getImageBase()); }

        operator int() const noexcept { return this->value; }
        explicit operator bool() const noexcept { return this->value >= 0; }

        // Pointer access
        T& operator *() noexcept { return *static_cast<pointer_t>(*this); }
        const T& operator *() const noexcept { return *static_cast<const_pointer_t>(*this); }
        pointer_t operator->() noexcept { return static_cast<pointer_t>(*this); }
        const_pointer_t operator->() const noexcept { return static_cast<const_pointer_t>(*this); }

        // Comparison operators
        bool operator == (const ibo32_t<T>& other) const noexcept { return this->value == other.value; }
        bool operator != (const ibo32_t<T>& other) const noexcept { return this->value != other.value; }
        bool operator < (const ibo32_t<T>& other) const noexcept { return this->value < other.value; }
        bool operator > (const ibo32_t<T>& other) const noexcept { return this->value > other.value; }
        bool operator <= (const ibo32_t<T>& other) const noexcept { return this->value <= other.value; }
        bool operator >= (const ibo32_t<T>& other) const noexcept { return this->value >= other.value; }

        // Pointer from base + offset
        pointer_t fromImageBase(void* base) noexcept {
            if (this->value >= 0) {
                return reinterpret_cast<pointer_t>((void*)(reinterpret_cast<uintptr_t>(base) + this->value));
            }
            else return nullptr;
        }

        // Pointer from base + offset (const)
        const_pointer_t fromImageBase(void* base) const noexcept {
            if (this->value >= 0) {
                return reinterpret_cast<const_pointer_t>((void*)(reinterpret_cast<uintptr_t>(base) + this->value));
            }
            else return nullptr;
        }

    private:
        template <typename L, typename R>
        int ptrDiff(L* l, R* r) {
            auto diff = reinterpret_cast<uintptr_t>(r) - reinterpret_cast<uintptr_t>(l);
            return diff >> 31 ? -1 : static_cast<int>(diff);
        }

        int value;
    };

    // 31-bit integer offset type used by many Windows internals
    // Convertible to and from integers and pointers (void* version)
    using ibo32 = ibo32_t<void>;
}

#endif /* WINTYPES_INCLUDE_IBO32_H */
