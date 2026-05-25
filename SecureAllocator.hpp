#ifndef SECURE_ALLOCATOR_HPP
#define SECURE_ALLOCATOR_HPP

#include <cstddef>
#include <limits>
#include <new>
#include "MemoryLocker.hpp"


// We use a template so this allocator can manage memory for ANY data type (chars, ints, custom objects)
template <typename T>
class SecureAllocator {
public:
    // Standard C++ alias required for all custom allocators
    using value_type = T;

    // Default constructor
    SecureAllocator() noexcept = default;

    // A copy constructor is required by standard C++ containers when they re-allocate memory
    template <typename U>
    SecureAllocator(const SecureAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        
        // Grab the raw memory from the OS
        T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));
        
        // Lock it immediately so it never pages to disk!
        lock_memory(ptr, n * sizeof(T)); // <-- ADD THIS LINE
        
        return ptr;
    }

  void deallocate(T* p, std::size_t n) noexcept {
        if (p != nullptr) {
            // 1. Scrub the memory with zeros!
            secure_wipe(p, n * sizeof(T));
            
            // 2. Unlock it now that it is safely wiped
            unlock_memory(p, n * sizeof(T)); // <-- ADD THIS LINE
            
            // 3. Give the clean, blank memory back to the Operating System
            ::operator delete(p);
        }
    }

private:
    // --- THE SECURITY WIPE ---
    // This physically overwrites the RAM with 0s.
    void secure_wipe(void* ptr, std::size_t size) noexcept {
        // By casting to a 'volatile' pointer, we strip away the compiler's ability
        // to "optimize away" this loop. It forces the CPU to execute every single 
        // write operation to the physical RAM.
        volatile char* volatile_ptr = static_cast<volatile char*>(ptr);
        while (size--) {
            *volatile_ptr++ = 0;
        }
    }
};

// --- REQUIRED BOILERPLATE ---
// Standard C++ needs to know if two allocators are compatible. 
// Since our allocator doesn't store any internal state, all instances are equal.
template <typename T, typename U>
bool operator==(const SecureAllocator<T>&, const SecureAllocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
bool operator!=(const SecureAllocator<T>&, const SecureAllocator<U>&) noexcept {
    return false;
}

#endif // SECURE_ALLOCATOR_HPP