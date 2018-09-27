cmake_minimum_required(VERSION 3.5.2)

project(sysadmin-api)

set(PROTO_FILES
    sysadminctl.proto
)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
protobuf_generate_python(PROTO_PYS ${PROTO_FILES})

# "link" python files so they get generated (they don't actually get linked)
add_library(sysadmin-api STATIC ${PROTO_SRCS} ${PROTO_PYS})
set_property(TARGET sysadmin-api PROPERTY CXX_STANDARD 11)
target_link_libraries(sysadmin-api INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(sysadmin-api INTERFACE ${CMAKE_BINARY_DIR})
link_directories(${CMAKE_CURRENT_BINARY_DIR})
