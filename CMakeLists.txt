cmake_minimum_required(VERSION 3.10)

project(expl LANGUAGES CXX)

find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find all .cpp files directly in the src directory
file(GLOB SOURCES_DIRECT "src/*.cpp")

# Recursively find all .cpp files in the subdirectories of src
file(GLOB_RECURSE SOURCES_RECURSIVE "src/**/*.cpp")

# Combine both lists
list(APPEND SOURCES ${SOURCES_DIRECT} ${SOURCES_RECURSIVE})

# Alternatively, if you want to ensure no duplicates
list(REMOVE_DUPLICATES SOURCES)

add_executable(exp ${SOURCES})

target_include_directories(exp PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(exp PRIVATE fmt::fmt)

# Debug-specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring Debug build")
    add_compile_definitions(_DEBUG)  # Define _DEBUG macro
    target_compile_options(exp PUBLIC -g)  # Include debug symbols
    target_compile_options(exp PUBLIC -fsanitize=address)
    target_link_options(exp PUBLIC -fsanitize=address)
endif()

# Release-specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
    add_compile_definitions(_NDEBUG)  # Define _NDEBUG macro
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")  # Optimisations for release
endif()

# Common compile options
target_compile_options(exp PUBLIC -Wall -Wextra -pedantic)

# Custom output name for compile_commands.json
add_custom_command(
    TARGET exp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_BINARY_DIR}/../compile_commands.json
)

