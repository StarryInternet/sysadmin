cmake_minimum_required(VERSION 3.5.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

project(decibel-cpp)

include(StarryLib)

if (NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        Debug CACHE STRING
        "default build type" FORCE)
endif()

include_directories(include)
add_subdirectory(src)

if ((NOT DEFINED DECIBEL_TESTS OR DECIBEL_TESTS)
    AND (NOT DEFINED BUILD_TESTS OR BUILD_TESTS))
    add_subdirectory(test)
endif()

if(BUILD_SHARED_LIBS)
    install(DIRECTORY include/ DESTINATION include)
endif(BUILD_SHARED_LIBS)
