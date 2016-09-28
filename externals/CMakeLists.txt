include_directories(Unity PUBLIC Unity/src)

add_library(Unity STATIC Unity/src/unity.c)
