cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

find_library(GLOG_LIBRARY glog)
find_path(GLOG_INCLUDE_DIR "glog/logging.h")

set(GLOG_LIBRARIES ${GLOG_LIBRARY})

find_package_handle_standard_args(Glog
  REQUIRED_ARGS GLOG_INCLUDE_DIR GLOG_LIBRARIES)
