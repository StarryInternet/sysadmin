cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

find_library(UV_LIBRARY uv)
find_path(UV_INCLUDE_DIR "uv.h")

set(UV_LIBRARIES ${UV_LIBRARY})

find_package_handle_standard_args(Uv
  REQUIRED_ARGS UV_INCLUDE_DIR UV_LIBRARIES)
