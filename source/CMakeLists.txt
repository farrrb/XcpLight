add_library(xcplight STATIC
  XcpLight.c
  XcpLightCfg.c
  )
target_include_directories(xcplight PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
