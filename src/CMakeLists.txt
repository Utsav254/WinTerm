# Find sources
file(GLOB_RECURSE LIB_SOURCES "*.cpp")

# Create a library target
add_library(winTerm ${LIB_SOURCES})

target_include_directories(winTerm PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Debug options
target_compile_options(winTerm PUBLIC -Wall -Wextra -pedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(winTerm PUBLIC _DEBUG)
    target_compile_options(winTerm PUBLIC -g -fsanitize=address)
    target_compile_options(winTerm PUBLIC -fdiagnostics-color=always)
    target_link_options(winTerm PUBLIC -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(winTerm PUBLIC _NDEBUG)
    target_compile_options(winTerm PUBLIC -fdiagnostics-color=always)
    target_compile_options(winTerm PUBLIC -O3 -g)
endif()

