include(StarryLib)

set(TEST_SOURCES
  fixtures/EchoServerFixture.cpp
  test_config.cpp
  test_connection_handler.cpp
  test_itimer.cpp
  test_looping_task.cpp
  test_main.cpp
  test_niceuv.cpp
  test_protobuf_get_set.cpp
  test_protobuf_handler.cpp
  test_protobuf_protocol.cpp
  test_reactor.cpp
)

set(PROTO_FILES
  testpb.proto
  testrpc.proto
)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
add_library(libproto STATIC ${PROTO_SRCS})
target_link_libraries(libproto INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(libproto INTERFACE ${CMAKE_BINARY_DIR})

add_executable(decibeltest ${TEST_SOURCES})
target_require_cxx17(decibeltest)
target_link_libraries(decibeltest decibel gtest gmock libproto ${NN_LIBS})

add_custom_target(decibel-check
  ./decibeltest
  DEPENDS decibeltest
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
