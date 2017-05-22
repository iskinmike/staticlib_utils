/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   windows.cpp
 * Author: alex
 * 
 * Created on March 16, 2015, 10:02 PM
 */

#include "staticlib/utils/windows.hpp"

#ifdef STATICLIB_WINDOWS

#include <memory>
#include <exception>
#include <string>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "staticlib/support.hpp"

#include "staticlib/utils/string_utils.hpp"

namespace staticlib {
namespace utils {

namespace { // anonymous

/**
 * Deleter implementation for smart pointer classes.
 * Will call 'LocalFree' function on the pointer.
 */
template <typename T>
class local_free_deleter {
public:
    /**
     * Delete operation, will call 'LocalFree' function on the pointer.
     * 
     * @param t pointer
     */
    void operator()(T* t) {
        LocalFree(t);
    }

};

} // namespace

std::wstring widen(const std::string& st) {
    if (st.empty()) return std::wstring();
    auto size_needed = MultiByteToWideChar(CP_UTF8, 0, st.c_str(), static_cast<int> (st.length()), nullptr, 0);
    if (0 == size_needed) throw utils_exception(TRACEMSG("Error on string widen calculation," +
            " string: [" + st + "], error: [" + errcode_to_string(GetLastError()) + "]"));
    std::wstring res{};
    res.resize(size_needed);
    auto buf = std::addressof(res.front());
    int chars_copied = MultiByteToWideChar(CP_UTF8, 0, st.c_str(), static_cast<int> (st.size()), buf, size_needed);
    if (chars_copied != size_needed) throw utils_exception(TRACEMSG("Error on string widen execution," +
            " string: [" + st + "], error: [" + errcode_to_string(GetLastError()) + "]"));
    return res;
}

std::string narrow(std::wstring wstr) {
    return narrow(wstr.c_str(), wstr.length());
}

std::string narrow(const wchar_t* wbuf, size_t length) {
    if (0 == length) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wbuf, static_cast<int> (length), nullptr, 0, nullptr, nullptr);
    if (0 == size_needed) throw utils_exception(TRACEMSG("Error on string narrow calculation," +
            " string length: [" + sl::support::to_string(length) + "], error code: [" + sl::support::to_string(GetLastError()) + "]"));
    std::string res{};
    res.resize(size_needed);
    auto buf = std::addressof(res.front());
    int bytes_copied = WideCharToMultiByte(CP_UTF8, 0, wbuf, static_cast<int> (length), buf, size_needed, nullptr, nullptr);
    if (bytes_copied != size_needed) throw utils_exception(TRACEMSG("Error on string narrow execution," +
            " string length: [" + sl::support::to_string(length) + "], error code: [" + sl::support::to_string(GetLastError()) + "]"));
    return res;
}

std::string errcode_to_string(uint32_t code) STATICLIB_NOEXCEPT {
    if (0 == code) return std::string {};
    wchar_t* buf_p = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<wchar_t*> (&buf_p), 0, nullptr);
    if (0 == size) {
        return "Cannot format code: [" + sl::support::to_string(code) + "]" +
                " into message, error code: [" + sl::support::to_string(GetLastError()) + "]";
    }
    auto buf = std::unique_ptr<wchar_t, local_free_deleter<wchar_t>> (buf_p, local_free_deleter<wchar_t>{});
    if (size <= 2) {
        return "code: [" + sl::support::to_string(code) + "], message: []";
    }
    try {
        std::string msg = narrow(buf.get(), size - 2);
        return "code: [" + sl::support::to_string(code) + "], message: [" + msg + "]";
    } catch (const std::exception& e) {
        return "Cannot format code: [" + sl::support::to_string(code) + "]" +
                " into message, narrow error: [" + e.what() + "]";
    }
}

named_mutex::named_mutex(const std::string& name) :
mutex(::CreateMutexW(nullptr, FALSE, widen(name).c_str())),
error(::GetLastError()) { }

named_mutex::named_mutex(named_mutex&& other) :
mutex(other.mutex),
error(other.error) {
    other.mutex = nullptr;
}

named_mutex& named_mutex::operator=(named_mutex&& other) {
    mutex = other.mutex;
    other.mutex = nullptr;
    error = other.error;
    return *this;
}

named_mutex::~named_mutex() STATICLIB_NOEXCEPT {
    if (nullptr != mutex) {
        ::CloseHandle(mutex);
    }
}

bool named_mutex::already_taken() const {
    return (ERROR_ALREADY_EXISTS == error);
}


} // namespace
}

#endif // STATICLIB_WINDOWS
