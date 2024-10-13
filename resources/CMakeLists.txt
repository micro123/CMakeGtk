file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ui)

pkg_check_modules(gio REQUIRED gio-2.0)
pkg_get_variable(gresc gio-2.0 glib_compile_resources)

message(STATUS "gresc => ${gresc}")

set(resource_describe_file  ${CMAKE_CURRENT_SOURCE_DIR}/dashboard.gresource.xml)
set(generated_resource_file ${CMAKE_CURRENT_BINARY_DIR}/app_res.c)
add_custom_command(
    OUTPUT  ${generated_resource_file}
    COMMAND ${CMAKE_COMMAND} -E echo "calling glib_compile_resource"
    COMMAND ${gresc} --generate --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} --target=${generated_resource_file} ${resource_describe_file}
    COMMENT "compiling resources"
    DEPENDS ${generated_files} ${resource_describe_file} all_blueprint
)
add_custom_target(
    compile_resource ALL
    DEPENDS ${generated_resource_file}
)
add_dependencies(all_resources compile_resource)
target_sources(all_resources INTERFACE ${generated_resource_file})
