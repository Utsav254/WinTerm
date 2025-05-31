# Find all example sources (one executable per file)
file(GLOB EXAMPLES "*.cpp")

foreach(EXAMPLE_SRC ${EXAMPLES})

    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SRC} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_definitions(${EXAMPLE_NAME} PUBLIC _DEBUG)
		target_compile_options(${EXAMPLE_NAME} PUBLIC -g -fsanitize=address)
		target_compile_options(${EXAMPLE_NAME} PUBLIC -fdiagnostics-color=always)
		target_link_options(${EXAMPLE_NAME} PUBLIC -fsanitize=address)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		target_compile_definitions(${EXAMPLE_NAME} PUBLIC _NDEBUG)
		target_compile_options(${EXAMPLE_NAME} PUBLIC -fdiagnostics-color=always)
		target_compile_options(${EXAMPLE_NAME} PUBLIC -O3 -g)
	endif()

    target_link_libraries(${EXAMPLE_NAME} PRIVATE winTerm)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

endforeach()

