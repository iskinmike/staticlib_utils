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
 * File:   process_utils.cpp
 * Author: alex
 * 
 * Created on September 21, 2015, 8:47 AM
 */

#include <cstdlib>

#include "staticlib/utils/config.hpp"
#ifdef STATICLIB_WINDOWS
#include "staticlib/utils/windows.hpp"
#endif // STATICLIB_WINDOWS
#include "staticlib/utils/process_utils.hpp"

namespace staticlib {
namespace utils {

int shell_exec_and_wait(const std::string& cmd) {
#ifdef STATICLIB_WINDOWS
    std::string quoted = "\"" + cmd + "\"";
    std::wstring ws = widen(quoted);
    return std::_wsystem(ws);
#else
    return std::system(cmd.c_str());
#endif // STATICLIB_WINDOWS
}

} // namespace
}

