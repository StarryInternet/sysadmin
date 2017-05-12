cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

include(FindPackageHandleStandardArgs)

find_library(YAMLCPP_LIBRARY yaml-cpp)
find_path(YAMLCPP_INCLUDE_DIR "yaml-cpp/yaml.h")

set(YAMLCPP_LIBRARIES ${YAMLCPP_LIBRARY})

find_package_handle_standard_args(YamlCpp REQUIRED_ARGS YAMLCPP_INCLUDE_DIR YAMLCPP_LIBRARIES)
