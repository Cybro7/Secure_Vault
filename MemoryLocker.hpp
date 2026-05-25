#ifndef MEMORY_LOCKER_HPP
#define MEMORY_LOCKER_HPP


#include <new>
#include "MemoryLocker.hpp" 
#include <cstddef>

// --- OS DETECTION ---
// If compiling on Windows
#ifdef _WIN32
    #include <windows.h>
// If compiling on Linux, macOS, or Unix (POSIX standard)
#elif defined(__linux__) || defined(__APPLE__)
    #include <sys/mman.h>
#endif

// --- LOCK FUNCTION ---
inline void lock_memory(void* ptr, std::size_t size) {
    if (ptr == nullptr || size == 0) return;

#ifdef _WIN32
    // Windows API to lock memory into physical RAM
    VirtualLock(ptr, size);
#elif defined(__linux__) || defined(__APPLE__)
    // POSIX API to lock memory into physical RAM
    mlock(ptr, size);
#endif
}

// --- UNLOCK FUNCTION ---
inline void unlock_memory(void* ptr, std::size_t size) {
    if (ptr == nullptr || size == 0) return;

#ifdef _WIN32
    VirtualUnlock(ptr, size);
#elif defined(__linux__) || defined(__APPLE__)
    munlock(ptr, size);
#endif
}

#endif // MEMORY_LOCKER_HPP