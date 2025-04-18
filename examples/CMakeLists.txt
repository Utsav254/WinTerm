# Find all example sources (one executable per file)
file(GLOB EXAMPLES "*.cpp")

foreach(EXAMPLE_SRC ${EXAMPLES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SRC} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE winTerm)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
endforeach()

