cmake_minimum_required(VERSION 3.25)
project(redasm-tests C)

if(NOT TARGET redasm::redasm)
    find_package(redasm REQUIRED)
endif()

# Collect all plugin paths into a semicolon-separated list
set(REDASM_PLUGIN_PATH_LIST "")
foreach(type loaders processors commands)
    get_property(subdirs DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../${type}" PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirs})
        get_property(targets DIRECTORY ${subdir} PROPERTY BUILDSYSTEM_TARGETS)
        foreach(target ${targets})
            get_target_property(target_type ${target} TYPE)
            if(target_type STREQUAL "SHARED_LIBRARY")
                list(APPEND REDASM_PLUGIN_PATH_LIST "$<TARGET_FILE:${target}>")
            endif()
        endforeach()
    endforeach()
endforeach()

string(JOIN "|" REDASM_PLUGIN_PATHS ${REDASM_PLUGIN_PATH_LIST})

enable_testing()

add_library(rdtest_helpers STATIC rdtest_helpers.c)
target_link_libraries(rdtest_helpers PRIVATE redasm::redasm)

target_compile_definitions(rdtest_helpers
    PRIVATE
        REDASM_PLUGIN_PATHS="${REDASM_PLUGIN_PATHS}"
        REDASM_KB_PATH="$<TARGET_FILE_DIR:redasm-gui>/kb"
)

macro(rd_add_test name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE redasm::redasm rdtest_helpers)
    target_include_directories(${name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
endmacro()

# units
rd_add_test(test_utils unit/test_utils.c)
rd_add_test(test_memory unit/test_memory.c)

add_test(NAME utils COMMAND test_utils)
add_test(NAME memory COMMAND test_memory)

# integrations
rd_add_test(test_tier0 integration/test_tier0.c)
rd_add_test(test_tier1 integration/test_tier1.c)
rd_add_test(test_vb integration/test_vb.c)

add_test(NAME tier0 COMMAND test_tier0 --samples "${REDASM_SAMPLES}")
add_test(NAME tier1 COMMAND test_tier1 --samples "${REDASM_SAMPLES}")
add_test(NAME vb COMMAND test_vb --samples "${REDASM_SAMPLES}")

set_tests_properties(tier0 PROPERTIES DEPENDS "memory utils")
set_tests_properties(tier1 PROPERTIES DEPENDS "tier0")
set_tests_properties(vb PROPERTIES DEPENDS "tier1")
