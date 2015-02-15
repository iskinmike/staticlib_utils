/* 
 * File:   tracemsg.hpp
 * Author: alex
 *
 * Created on January 2, 2015, 11:53 AM
 */

#ifndef STATICLIB_TRACEMSG_HPP
#define	STATICLIB_TRACEMSG_HPP

#include <string>

#include "staticlib/utils/config.hpp"

#define TRACEMSG(message) staticlib::utils::tracemsg(message, __FILE__, STATICLIB_CURRENT_FUNCTION, __LINE__)

namespace staticlib {
namespace utils {

/**
 * Prepends specified message with formatted current function name, source file name and line number.
 * Can be used through macro shortcut as `TRACEMSG("Hi")`
 * 
 * @param message input message
 * @param file source filename, `__FILE__` macro is used in `TRACEMSG` macro
 * @param func current function name, `STATICLIB_CURRENT_FUNCTION` macro is used in `TRACEMSG` macro
 * @param line current line in source file, `__LINE__` macro is used in `TRACEMSG` macro
 * @return message string prepended with specified data
 */
std::string tracemsg(const std::string& message, const std::string& file, const std::string& func, int line);

}
} //namespace


#endif	/* STATICLIB_TRACEMSG_HPP */

