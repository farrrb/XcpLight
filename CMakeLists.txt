cmake_minimum_required( VERSION 2.8.5 )

project( XcpLight )

enable_language( C )


include_directories(source)

set(SOURCES ${SOURCES}  source/XcpLight.c)
set(SOURCES ${SOURCES}  source/XcpLight_config.c)

set(SOURCES ${SOURCES}  tests/main.c)

add_executable(XcpLightTests ${SOURCES})
