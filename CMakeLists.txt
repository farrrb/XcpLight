# MIT License see LICENSE file
# - Copyright (c) 2016-2019 0xFAB - Fabian Zahn
cmake_minimum_required(VERSION 2.8.7)

project(XcpLight C)

enable_testing()

if (CMAKE_C_COMPILER_ID MATCHES GNU)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -Wall -m32 -Werror")
  set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
endif()

add_subdirectory(externals)
add_subdirectory(source)
add_subdirectory(tests)
