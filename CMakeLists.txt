# Copyright (c) 2016 0xFAB - Fabian Zahn
cmake_minimum_required(VERSION 2.8.7)

#set project name
project(XcpLight)

# config
enable_language(C)
enable_testing()

# compiler flags
set(CMAKE_C_FLAGS "-O2 -Wall -m32 -Werror")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# add include include directories
include_directories(source)
include_directories(externals/Unity/src)

# add XcpLight
include_directories(XcpLight PUBLIC)
add_library(XcpLight STATIC source/XcpLight.c)
add_library(XcpLightConfig source/XcpLight_config.c)

# add Unity
add_subdirectory(externals)

# tests setup
include_directories(tests)

macro (Add_XcpLight_Test NAME TIMEOUT)
    add_executable(${NAME} tests/${NAME}.c)
    target_link_libraries(${NAME} XcpLight XcpLightConfig Unity)
    add_test(${NAME} ${NAME})
    set_tests_properties(${NAME} PROPERTIES TIMEOUT ${TIMEOUT})
endmacro (Add_XcpLight_Test)

# add tests
Add_XcpLight_Test(BasicCommands     5)
Add_XcpLight_Test(LoginProcedure    5)
Add_XcpLight_Test(Timestamp         5)
Add_XcpLight_Test(MemoryAccess      5)
