/* 
 * File:   string_utils.cpp
 * Author: alex
 *
 * Created on January 13, 2015, 9:03 PM
 */

#include <string>
#include <exception>
#include <cstdlib>
#include <cstring>

#include "staticlib/stdlib/tracemsg.hpp"
#include "staticlib/stdlib/StdlibException.hpp"
#include "staticlib/stdlib/string_utils.hpp"

namespace staticlib {
namespace stdlib {

namespace { // anonymous

template <typename T>
T* get_buffer_internal(std::basic_string<T>& str, typename std::basic_string<T>::size_type required_size) {
    try {
        str.resize(required_size);
        return &str.front();
    } catch (const std::exception& e) {
        throw StdlibException(TRACEMSG(std::string(e.what()) + 
                "\nError getting buffer with required size: [" + to_string(required_size) + "]" + 
                " from string, length: [" + to_string(str.length()) + "]"));
    }
}

} // namespace

char* get_buffer(std::string& str, std::string::size_type required_size) {
    return get_buffer_internal<char>(str, required_size);
}

wchar_t* get_buffer(std::wstring& str, std::wstring::size_type required_size) {
    return get_buffer_internal<wchar_t>(str, required_size);
}

char* alloc_copy(const std::string& str) BOOST_NOEXCEPT {
    auto len = str.length();
    char* msg = static_cast<char*> (malloc(len + 1));
    if (nullptr == msg) {
        char* err = static_cast<char*> (malloc(2));
        err[0] = 'E';
        err[1] = '\0';
        return err;
    }
    msg[len] = '\0';
    memcpy(msg, str.c_str(), len);
    return msg;
}

}
} // namespace

