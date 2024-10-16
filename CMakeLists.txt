cmake_minimum_required(VERSION 3.23)

project(Dashboard CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-fvisibility=hidden)
add_link_options(-Wl,--export-dynamic)

find_package(PkgConfig REQUIRED)

set(generated_files)
add_custom_target(all_blueprint ALL)
add_library(all_resources INTERFACE)

add_subdirectory(blueprint)
add_subdirectory(resources)
add_subdirectory(src)
