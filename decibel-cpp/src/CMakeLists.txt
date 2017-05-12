include(StarryLib)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Werror")

set(DECIBEL_SOURCE_FILES
    decibel/niceuv/BaseTimerEvent.cpp
    decibel/niceuv/EventLoop.cpp
    decibel/niceuv/IdleEvent.cpp
    decibel/niceuv/InterfaceInfo.cpp
    decibel/niceuv/OneShotTimerEvent.cpp
    decibel/niceuv/Poller.cpp
    decibel/niceuv/TcpConn.cpp
    decibel/niceuv/TcpServer.cpp
    decibel/niceuv/TimerEvent.cpp
    decibel/niceuv/SubprocessHandler.cpp
    decibel/niceuv/FutureSubprocess.cpp
    decibel/niceuv/WorkCancellable.cpp
    decibel/niceuv/UvStream.cpp
    decibel/niceuv/ITimer.cpp
    decibel/niceuv/FileChangeEvent.cpp
    decibel/messaging/Exchanges.cpp
    decibel/messaging/LoopingTask.cpp
    decibel/messaging/ITransport.cpp
    decibel/messaging/Protocol.cpp
    decibel/messaging/Reactor.cpp
    decibel/messaging/TcpTransport.cpp
    decibel/messaging/TcpServer.cpp
    decibel/Protobuf.cpp)

set(DECIBEL_RABBITMQ_SOURCE_FILES
    decibel/messaging/IRabbitMQMessageHandler.cpp
    decibel/messaging/RabbitMQProtocol.cpp)

option(DECIBEL_RABBITMQ_SUPPORT "decibel-cpp rabbitmq support" OFF)

if (DECIBEL_RABBITMQ_SUPPORT)
  message("Enabling rabbitmq support...")
  set(DECIBEL_SOURCE_FILES
    ${DECIBEL_SOURCE_FILES}
    ${DECIBEL_RABBITMQ_SOURCE_FILES}
    )
endif()

if(BUILD_SHARED_LIBS)
    add_library(decibel SHARED ${DECIBEL_SOURCE_FILES})
else()
    add_library(decibel STATIC ${DECIBEL_SOURCE_FILES})
endif()

target_require_cxx14(decibel)

find_package(Protobuf REQUIRED)
set(PUBLIC_LIBS ${PUBLIC_LIBS} ${PROTOBUF_LIBRARIES})

find_package(log4cxx REQUIRED)
include_directories(${LOG4CXX_INCLUDE_DIRS})
link_directories(${LOG4CXX_LIBRARY_DIRS})
set(PRIVATE_LIBS ${PRIVATE_LIBS} ${LOG4CXX_LIBRARIES})

# Boost Regex and Context is required by Folly
find_package(Boost 1.61.0 REQUIRED filesystem system regex context)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(PRIVATE_LIBS ${PRIVATE_LIBS}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_CONTEXT_LIBRARY}
    )
set(PUBLIC_LIBS ${PUBLIC_LIBS} ${Boost_FILESYSTEM_LIBRARY})

find_package(Folly REQUIRED)
include_directories(${FOLLY_INCLUDE_DIR})
set(PUBLIC_LIBS ${PUBLIC_LIBS} ${FOLLY_LIBRARY})

find_package(YamlCpp 0.5.1 REQUIRED)
include_directories(${YAMLCPP_INCLUDE_DIR})
set(PUBLIC_LIBS ${PUBLIC_LIBS} ${YAMLCPP_LIBRARIES})

find_package(Glog REQUIRED)
set(PRIVATE_LIBS ${PRIVATE_LIBS} ${GLOG_LIBRARIES})

find_package(Uv REQUIRED)
set(PUBLIC_LIBS ${PUBLIC_LIBS} ${UV_LIBRARIES})

target_link_libraries(decibel PRIVATE ${PRIVATE_LIBS})
target_link_libraries(decibel PUBLIC ${PUBLIC_LIBS})
if(BUILD_SHARED_LIBS)
    get_version(version)

    # If not set SOVERSION will inherit from VERSION
    set_target_properties(decibel PROPERTIES VERSION ${version})
    install(TARGETS decibel LIBRARY DESTINATION lib)
endif()
