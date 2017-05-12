cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

find_library(DECIBEL_CPP_LIBRARY decibel)
find_path(DECIBEL_CPP_INCLUDE_DIR "decibel/Protobuf.h")

set(DECIBEL_CPP_LIBRARIES ${DECIBEL_CPP_LIBRARY})

find_package_handle_standard_args(DecibelCpp
  REQUIRED_ARGS DECIBEL_CPP_INCLUDE_DIR DECIBEL_CPP_LIBRARIES)
