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
 * File:   string_utils_test.cpp
 * Author: alex
 *
 * Created on January 2, 2015, 1:06 PM
 */

#include "staticlib/utils/string_utils.hpp"

#include <iostream>
#include <cstring>

#include "staticlib/config/assert.hpp"

namespace ss = staticlib::utils;

void test_alloc_copy() {
    std::string st{"foo"};
    auto buf = ss::alloc_copy(st);
    slassert(3 == strlen(buf));
    slassert('\0' == buf[3]);
    free(buf);
}

void test_split() {
    std::string src{"foo:bar::baz:"};
    std::vector<std::string> vec = ss::split(src, ':');
    slassert(3 == vec.size());
    slassert("foo" == vec[0]);
    slassert("bar" == vec[1]);
    slassert("baz" == vec[2]);
}

void test_ends_with() {
    slassert(ss::ends_with("foo", "oo"));
    slassert(ss::ends_with("foo", "foo"));
    slassert(!ss::ends_with("foo", "1foo"));
    slassert(ss::ends_with("foo", ""));
    slassert(!ss::ends_with("foo", "ar"));
}

void test_strip_filename() {
    slassert("/foo/bar/" == ss::strip_filename("/foo/bar/baz"));
    slassert("c:\\foo\\bar\\" == ss::strip_filename("c:\\foo\\bar\\baz"));
    slassert("/foo/bar/" == ss::strip_filename("/foo/bar/baz.foo"));
    slassert("/foo/bar/" == ss::strip_filename("/foo/bar/"));
    slassert("/" == ss::strip_filename("/foo"));
    slassert("foo" == ss::strip_filename("foo"));
    slassert("" == ss::strip_filename(""));
}

void test_strip_parent_dir() {    
    slassert("baz" == ss::strip_parent_dir("/foo/bar/baz"));
    slassert("baz" == ss::strip_parent_dir("c:\\foo\\bar\\baz"));
    slassert("baz.foo" == ss::strip_parent_dir("/foo/bar/baz.foo"));
    slassert("" == ss::strip_parent_dir("/foo/bar/"));
    slassert("" == ss::strip_parent_dir("/foo///bar/"));
    slassert("" == ss::strip_parent_dir("/foo/bar//"));
    slassert("foo" == ss::strip_parent_dir("/foo"));
    slassert("foo" == ss::strip_parent_dir("c:\\bar\\foo"));    
    slassert("foo" == ss::strip_parent_dir("foo"));    
    slassert("a" == ss::strip_parent_dir("/a"));
    slassert("" == ss::strip_parent_dir("/"));
    slassert("" == ss::strip_parent_dir("///"));
    slassert("" == ss::strip_parent_dir("\\"));
    slassert("" == ss::strip_parent_dir(""));
}

int main() {
    try {
        test_alloc_copy();
        test_split();
        test_ends_with();
        test_strip_filename();
        test_strip_parent_dir();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}

