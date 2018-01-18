cmake_minimum_required(VERSION 3.5.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(sysadmin)

if (NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        Debug CACHE STRING
        "default build type" FORCE)
endif()

set(BUILD_SHARED_LIBS OFF)

set(DECIBEL_TESTS ON)
add_subdirectory(decibel-cpp)
include_directories(decibel-cpp/include)

add_subdirectory(sysadmin-api)

include_directories(src)
add_subdirectory(src)

set(CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/configs)

add_custom_target(configs)
add_custom_command(
    TARGET configs
    COMMAND rm -rf ${CONFIG_DIR} && cp -r ${CMAKE_CURRENT_SOURCE_DIR}/configs ${CONFIG_DIR}
)

if (NOT DEFINED BUILD_TESTS OR BUILD_TESTS)
add_subdirectory(googletest)
add_subdirectory(test)
endif()
