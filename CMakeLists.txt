cmake_minimum_required(VERSION 3.10)

project(expl LANGUAGES CXX)

find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

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

target_compile_options(exp PUBLIC -fsanitize=address)
target_link_options(exp PUBLIC -fsanitize=address)

target_compile_options(exp PUBLIC -Wall -Wextra -Wpedantic)

