include(StarryLib)

set(TEST_SOURCES
    test_commit_history.cpp
    test_config_types.cpp
    test_configurator.cpp
    test_hook_config.cpp
    test_hook_initializer.cpp
    test_hook_manager.cpp
    test_hook_table.cpp
    test_hook_user.cpp
    test_key_value_formatter.cpp
    test_local_history_storage.cpp
    test_local_yaml_storage.cpp
    test_main.cpp
    test_memory_cache.cpp
    test_protobuf_translator.cpp
    test_service_script_runner.cpp
    test_size_age_bound_map.cpp
    test_sys_admin_controller.cpp
    test_templater_runner.cpp
    test_transaction_manager.cpp
)

add_executable(sysadmintest ${TEST_SOURCES})
target_require_cxx14(sysadmintest)
target_link_libraries(sysadmintest gtest gmock libsysadmin)

add_custom_command(
    TARGET sysadmintest
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/argdumper.py ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(check
    ./sysadmintest
    DEPENDS sysadmintest argdumper.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
