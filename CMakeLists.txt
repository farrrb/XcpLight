# set minimum version
cmake_minimum_required(VERSION 2.8.7)

#set project name
project(XcpLight)
set(PROJECT_NAME_TESTS ${PROJECT_NAME}_tests)

# use "C"
enable_language(C)
# use CTest
enable_testing()

# add include include directories
include_directories(source)

# add source files to the build
set(SOURCES ${SOURCES}  source/XcpLight.c)
set(SOURCES ${SOURCES}  source/XcpLight_config.c)

set(SOURCES ${SOURCES}  tests/main.c)

# add exe
add_executable(${PROJECT_NAME_TESTS} ${SOURCES})
# add test output file
add_test(TestName ${PROJECT_NAME_TESTS})
