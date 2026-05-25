#include <iostream>
#include <string>
#include "SecureString.hpp"

// 1. The Simulation Function (Defined outside of main)
void run_simulation() {
    std::cout << "\n[+] ENTERING DYNAMIC GENERATION MODE" << std::endl;
    
    // We create the strings dynamically by appending characters
    // This stops the compiler from hardcoding the final string in the .exe
    std::string standard_str = "VULNERABLE";
    standard_str += "_DYNAMIC_LEAK_";
    standard_str += "1234567890";

    SecureString secure_str = "SECURE";
    secure_str += "_DYNAMIC_WIPE_";
    secure_str += "0987654321";

    std::cout << "[+] Both passwords dynamically generated on the Heap." << std::endl;
    std::cout << "[+] Destroying variables..." << std::endl;
    
} // <-- Destruction happens RIGHT HERE.

// 2. The Main Program
int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "       SECURE VAULT MEMORY TEST          " << std::endl;
    std::cout << "=========================================\n" << std::endl;

    // 3. We call the simulation function here
    run_simulation();

    std::cout << "\n[!] SIMULATION PAUSED [!]" << std::endl;
    std::cout << "Now, create the Dump File in Task Manager." << std::endl;
    
    std::cout << "\nPress ENTER to close the program..." << std::endl;
    
    // Clear the input buffer so the program pauses correctly waiting for you
    // (If it skips the pause, you can just press Enter a second time)
    std::cin.ignore(10000, '\n');
    std::cin.get(); 

    return 0;
}