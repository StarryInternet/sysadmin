cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

find_library(SQLITE3_LIBRARY sqlite3)
find_path(SQLITE3_INCLUDE_DIR "sqlite3.h")

if(NOT SQLITE3_INCLUDE_DIR)
  message(SEND_ERROR "Failed to find sqlite3 headers")
endif()

if(NOT SQLITE3_LIBRARY)
  message(SEND_ERROR "Failed to find sqlite3 library")
endif()

set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})

find_package_handle_standard_args(Sqlite3
  REQUIRED_ARGS SQLITE3_INCLUDE_DIR SQLITE3_LIBRARIES)
