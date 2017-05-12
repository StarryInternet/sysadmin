# Locate amqp-cpp
#
# This module defines
#  AMQPCPP_FOUND, if false, do not try to link to amqp-cpp
#  AMQPCPP_LIBRARY, where to find amqp-cpp
#  AMQPCPP_INCLUDE_DIR, where to find yaml.h
#
# By default, the dynamic libraries of amqp-cpp will be found. To find the static ones instead,
# you must set the AMQPCPP_STATIC_LIBRARY variable to TRUE before calling find_package(YamlCpp ...).
#
# If amqp-cpp is not installed in a standard path, you can use the AMQPCPP_DIR CMake variable
# to tell CMake where amqp-cpp is.

# attempt to find static library first if this is set
if(AMQPCPP_STATIC_LIBRARY)
    set(AMQPCPP_STATIC libamqpcpp.a)
endif()

# find the amqp-cpp include directory
find_path(AMQPCPP_INCLUDE_DIR amqpcpp.h
          PATH_SUFFIXES include
          PATHS
          ~/Library/Frameworks/amqp-cpp/include/
          /Library/Frameworks/amqp-cpp/include/
          /usr/local/include/
          /usr/include/
          ${AMQPCPP_DIR}/include/)

# find the amqp-cpp library
find_library(AMQPCPP_LIBRARY
             NAMES ${AMQPCPP_STATIC} amqpcpp
             PATH_SUFFIXES lib64 lib
             PATHS ~/Library/Frameworks
                    /Library/Frameworks
                    /usr/local
                    /usr
                    ${AMQPCPP_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set AMQPCPP_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AMQPCPP DEFAULT_MSG AMQPCPP_INCLUDE_DIR AMQPCPP_LIBRARY)
mark_as_advanced(AMQPCPP_INCLUDE_DIR AMQPCPP_LIBRARY)
