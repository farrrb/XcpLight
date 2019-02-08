add_library(unity STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src/unity.c)
target_include_directories(unity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Unity/src)
