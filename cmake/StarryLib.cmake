macro(target_require_cxx11 target)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
endmacro(target_require_cxx11)

macro(target_require_cxx14 target)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
endmacro(target_require_cxx14)

macro(target_require_cxx17 target)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
endmacro(target_require_cxx17)

# Adds a prefix to every item in a list.
# e.g. add_prefix(PREFIXED_LIST "prefix/" "one" "two" "three") ->
#   PREFIXED_LIST = "prefix/one" "prefix/two" "prefix/three"
function(add_prefix var prefix)
    set(local_list "")

    foreach(item ${ARGN})
        list(APPEND local_list "${prefix}${item}")
    endforeach(item)

    set(${var} "${local_list}" PARENT_SCOPE)
endfunction(add_prefix)

# Interleaves an item throughout a list.
# e.g. interleave(INTERLEAVED_LIST "-i" "one" "two" "three") ->
#   INTERLEAVED_LIST = "-i" "one" "-i" "two" "-i" "three"
function(interleave var interleaved)
    set(local_list "")

    foreach(item ${ARGN})
        list(APPEND local_list ${interleaved} ${item})
    endforeach(item)

    set(${var} "${local_list}" PARENT_SCOPE)
endfunction(interleave)

# Creates a namespaced check for the specified target and adds the target to a
# top level "check" target. If a top level "check" target doesn't exist, one is
# created.
# e.g. add_check_target(kathy
#                       COMMAND ./run_kathy_tests
#                       DEPENDS libkathy some_other_target
#                       WORKING_DIRECTORY some/working/dir)
function(add_check_target target)
    set(mv_kws COMMAND DEPENDS WORKING_DIRECTORY)

    cmake_parse_arguments(add_check_target "" "" "${mv_kws}" ${ARGN})

    set(check_target_name "check-${target}")
    set(check_target_args COMMAND ${add_check_target_COMMAND})

    if (NOT "${add_check_target_DEPENDS}" STREQUAL "")
        list(APPEND check_target_args DEPENDS ${add_check_target_DEPENDS})
    endif()

    if (NOT "${add_check_target_WORKING_DIRECTORY}" STREQUAL "")
        list(APPEND check_target_args WORKING_DIRECTORY ${add_check_target_WORKING_DIRECTORY})
    endif()

    add_custom_target(${check_target_name}
                      ${check_target_args})

    if (NOT TARGET check)
        add_custom_target(check)
    endif()

    add_dependencies(check ${check_target_name})
endfunction(add_check_target)

# Get the project version as a string from `git describe` or the directory name
# when run inside buildroot
# e.g. get_version(version)
function(get_version _var)
    if(BUILD_SHARED_LIBS)
        get_filename_component(directory_name
                               ${PROJECT_SOURCE_DIR}
                               NAME)
        string(REGEX MATCH "[^-]+$" version ${directory_name})
        if(NOT version)
            message(FATAL_ERROR "Failed to parse version from directory name")
        endif()
        set(${_var} ${version} PARENT_SCOPE)
    elseif()
        find_package(Git)
        if(NOT DEFINED GIT_FOUND OR NOT GIT_FOUND)
            message(FATAL_ERROR "Could not find git")
        endif()

        set(GIT_DESCRIBE describe --tags --abbrev=0 --always --first-parent)
        execute_process(COMMAND
                        ${GIT_EXECUTABLE} ${GIT_DESCRIBE}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE result
                        OUTPUT_VARIABLE version
                        ERROR_QUIET
                        OUTPUT_STRIP_TRAILING_WHITESPACE)

        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Fail to fetch version from a git tag")
        endif()
        set(${_var} ${version} PARENT_SCOPE)
    endif()
endfunction()
