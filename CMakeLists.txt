#**********************************************************************************
#* MIT License                                                                    *
#*                                                                                *
#* Copyright © 2024 by Christophe Benne, this file is part of BANGOR software.    *
#*                                                                                *
#* Permission is hereby granted, free of charge, to any person obtaining a copy   *
#* of this software and associated documentation files (the "Software"), to deal  *
#* in the Software without restriction, including without limitation the rights   *
#* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
#* copies of the Software, and to permit persons to whom the Software is          *
#* furnished to do so, subject to the following conditions:                       *
#*                                                                                *
#* The above copyright notice and this permission notice shall be included in all *
#* copies or substantial portions of the Software.                                *
#*                                                                                *
#* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
#* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
#* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
#* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
#* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
#* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
#* SOFTWARE.                                                                      *
#**********************************************************************************


#
# BANGOR CMAKE common definitions.
# 
cmake_minimum_required     (VERSION 3.18.4)

#
# BANGOR       common definitions.
# 
project                    (Bangor VERSION 0.1.0 LANGUAGES C CXX)

set                        (CMAKE_CXX_STANDARD 20)

include                    (FetchContent)
FetchContent_Declare       (googletest
                            GIT_REPOSITORY https://github.com/google/googletest.git
                            GIT_TAG        release-1.12.1)
FetchContent_MakeAvailable (googletest)

if(CODE_COVERAGE)
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

enable_testing()

#
# BANGOR        container librarie definitions.
# 
add_library                (bgcontainers               sources/c++/containers/bitsstrings.cc)
target_include_directories (bgcontainers       PRIVATE ${PROJECT_SOURCE_DIR}/sources/c++/includes)

add_executable             (bgcontainers_tests         tests/c++/containers/bitsstrings_tests.cc)
target_include_directories (bgcontainers_tests PRIVATE ${PROJECT_SOURCE_DIR}/sources/c++/includes)
target_link_libraries      (bgcontainers_tests          bgcontainers gtest gtest_main pthread)
add_test                   (NAME Test_Containers 
                            COMMAND $<TARGET_FILE:bgcontainers_tests>)
