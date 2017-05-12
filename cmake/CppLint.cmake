# enable_cpplint as a PRE_BUILD step to a target, or, if the given name doesn't
# exist as a target, adds a `cpplint-` prefix and creates a standalone target that
# is always considered out of date (and as such, always rebuilt).
macro(enable_cpplint)
    find_program(CPPLINT "cpplint")

    if(CPPLINT)
        set(target_name ${ARGV0})
        set(linty_files ${ARGN})
        list(REMOVE_AT linty_files 0)

        if(TARGET ${target_name})
            add_custom_command(TARGET ${target_name}
                               PRE_BUILD
                               COMMAND ${CPPLINT} ${linty_files}
                               WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                               COMMENT "Linting ${target_name}")
        else()
            # Check if we already added this target to avoid an error. This
            # could happen if, for example, we lint some common files that are
            # included in another component via a cmake Include.
            if(NOT TARGET "cpplint-${target_name}")
                add_custom_target("cpplint-${target_name}" ALL
                                  COMMAND ${CPPLINT} ${linty_files}
                                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                  COMMENT "Linting ${target_name}")
            endif()
        endif()
    else(CPPLINT)
        message(WARNING "cpplint not found, linting not enabled")
    endif(CPPLINT)
endmacro(enable_cpplint)
