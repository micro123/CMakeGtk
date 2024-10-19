find_program(BLPC blueprint-compiler REQUIRED)

file(GLOB_RECURSE BLP_FILES *.blp)

set(ui_files)

foreach(blp ${BLP_FILES})
    get_filename_component(basename ${blp} NAME_WLE)
    # message(STATUS "add blueprint file: ${blp} => ${basename}")
    set(output_file ${CMAKE_SOURCE_DIR}/resources/ui/${basename}.ui)
    add_custom_command(
        OUTPUT  ${output_file}
        COMMAND ${BLPC} compile --output=${output_file} ${blp}
        COMMENT "Compile blueprint ${blp}"
        DEPENDS ${blp}
    )
    add_custom_target(
        ${basename}_blpc ALL
        DEPENDS ${output_file}
    )
    add_dependencies(all_blueprint ${basename}_blpc)
    list(APPEND ui_files ${output_file})
endforeach()

set(generated_files ${ui_files} PARENT_SCOPE)
