include(StarryLib)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Werror")

set(SYSADMIN_SOURCE_FILES
    CommitHistory.cpp
    ConfigTypes.cpp
    Configurator.cpp
    ExternalRunner.cpp
    HookConfig.cpp
    HookInitializer.cpp
    HookManager.cpp
    HookTable.cpp
    HookUser.cpp
    LocalHistoryStorage.cpp
    LocalYAMLStorage.cpp
    LockedConfigPairMap.cpp
    MemoryCache.cpp
    ProtobufTranslator.cpp
    ServiceScriptRunner.cpp
    SysAdminConfig.cpp
    SysAdminController.cpp
    sysadmin.cpp
    TemplaterRunner.cpp
    TransactionManager.cpp
)

set(LIBS sysadmin-api)

add_library(libsysadmin STATIC ${SYSADMIN_SOURCE_FILES})
add_executable(sysadmin sysadmin.cpp)

target_require_cxx14(libsysadmin)
target_require_cxx14(sysadmin)

find_package(log4cxx REQUIRED)
include_directories(${LOG4CXX_INCLUDE_DIRS})
link_directories(${LOG4CXX_LIBRARY_DIRS})
set(LIBS ${LIBS} ${LOG4CXX_LIBRARIES})

# Boost Regex and Context is required by Folly
find_package(Boost 1.61.0 REQUIRED system filesystem regex context date_time)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(LIBS
    ${LIBS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_CONTEXT_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
)

find_package(Folly REQUIRED)
include_directories(${FOLLY_INCLUDE_DIR})
set(LIBS ${LIBS} ${FOLLY_LIBRARY})

find_package(YamlCpp 0.5.1 REQUIRED)
include_directories(${YAMLCPP_INCLUDE_DIR})
set(LIBS ${LIBS} ${YAMLCPP_LIBRARIES})

find_package(Glog REQUIRED)
set(LIBS ${LIBS} ${GLOG_LIBRARIES})

find_package(Uv REQUIRED)
set(LIBS ${LIBS} ${UV_LIBRARIES})

target_link_libraries(libsysadmin INTERFACE ${LIBS} decibel)
target_include_directories(libsysadmin PUBLIC ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR})
target_link_libraries(sysadmin libsysadmin)

install(TARGETS sysadmin
        RUNTIME DESTINATION bin)
