#ifndef SECURE_STRING_HPP
#define SECURE_STRING_HPP

#include <string>
#include "SecureAllocator.hpp"

/* By default, C++ defines std::string like this:
   using string = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
   
   We are creating a custom type alias that replaces the default allocator 
   with our brand new SecureAllocator.
*/

using SecureString = std::basic_string<
    char, 
    std::char_traits<char>, 
    SecureAllocator<char>
>;

#endif // SECURE_STRING_HPP