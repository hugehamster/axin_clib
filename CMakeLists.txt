cmake_minimum_required(VERSION 3.20)
project(axin_clib LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(axin_clib INTERFACE)
target_include_directories(axin_clib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable(axin_string_view_test
    test/axin_string_view_test.c)
target_link_libraries(axin_string_view_test
    PRIVATE axin_clib)

add_test(NAME axin_string_view_test COMMAND axin_string_view_test)
