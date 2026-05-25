# SecureVault: Memory-Hardened C++ Allocator





> An ultra-lightweight, zero-dependency C++ library that physically defends sensitive credentials (passwords, encryption keys, API tokens) against RAM scraping and Dead Store Elimination (DSE) vulnerabilities.



---



# The Threat Landscape (CWE-316)

When standard C++ applications handle sensitive data using `std::string`, the data is written to the heap. When the variable is destroyed, the pointer is deleted, but **the plaintext characters remain actively sitting in physical RAM** until overwritten by another program. 



If an attacker dumps the process memory (via tools like Mimikatz, Procdump, or Task Manager), they can easily extract every password the application ever touched. Furthermore, if the OS runs low on memory, it will page (swap) that plaintext password to the hard drive, making the exposure permanent.



# The Arsenal (How SecureVault Works)

SecureVault overrides standard C++ memory mechanics to provide military-grade guarantees:



1. **OS Anti-Paging (Memory Locking):** Uses OS-level APIs (`VirtualLock` on Windows, `mlock` on POSIX) to instantly pin the memory. Passwords are mathematically guaranteed to *never* touch the hard drive page file.

2. **RAII Secure Wipe:** Standard zeroing attempts are often ignored by modern compilers via Dead Store Elimination (DSE). SecureVault uses a custom allocator with volatile pointer casting to defeat the compiler, guaranteeing the physical RAM block is scrubbed with `00 00 00 00` the exact microsecond the variable goes out of scope.

3. **SSO Bypass:** Explicitly forces heap allocation to prevent the compiler from leaking small passwords onto the local stack (bypassing Small String Optimization).



---



# Forensic Proof of Concept



To prove the efficacy of this engine, I built a dynamic simulation that asks for user input at runtime (preventing the compiler from hardcoding string literals). I then triggered a manual process crash and analyzed the raw `.DMP` file in a Hex Editor.



| ❌ Standard `std::string` (Vulnerable) | ✅ `SecureString` (Protected) |
| :---: | :---: |
| ![Insecure Dump](https://github.com/user-attachments/assets/96d1badd-fdcc-44e5-9c4a-d99a2da2751e) | ![Secure Dump](https://github.com/user-attachments/assets/51076af1-dc11-483e-b42d-256c562a7477) |


*The forensic analysis proves that standard C++ leaves the dynamic password sitting in plain text, while SecureVault successfully zeroes out the physical memory block upon destruction.*



---



# Quick Start / Usage



SecureVault is designed to be **Header-Only**. There is no complex installation. Just drop `SecureAllocator.hpp`, `SecureString.hpp`, and `MemoryLocker.hpp` into your `include/` directory.



Swap out `std::string` for `SecureString` when handling sensitive data:



```cpp

#include <iostream>

#include "SecureString.hpp"



int main() {

    {

        // 1. Memory is allocated on the heap.

        // 2. Memory is instantly locked to physical RAM (Anti-Paging).

        SecureString db_password = "SuperSecretDatabasePassword123!";

        

        // Connect to database, authenticate, etc...

        std::cout << "Authenticating..." << std::endl;

        

    } // <-- Variable goes out of scope here.

      // 3. Memory is aggressively overwritten with zeros.

      // 4. Memory is unlocked and returned to the OS.



    return 0;

}
```


## About the Author

I am Yogesh Mondal, a student with a strong interest in cybersecurity, ethical hacking, and system-level programming. I enjoy working on projects that explore how software interacts with memory, operating systems, and security principles.

My primary focus is on C++ development, secure coding practices, and understanding low-level system behavior. I am actively building my skills through hands-on projects, experimentation, and continuous learning in the field of cybersecurity and software engineering.

I aim to develop reliable, efficient, and security-focused applications while strengthening my understanding of real-world computing systems.

 Contact :
📧 Email: yogeshmondal0@gmail.com
💻 GitHub: https://github.com/cybro7
🌐 Location: West Bengal, India
