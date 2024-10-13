file(GLOB_RECURSE SRC_FILES *.*)

pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0)
pkg_check_modules(adw REQUIRED IMPORTED_TARGET libadwaita-1)
pkg_check_modules(gme REQUIRED IMPORTED_TARGET gmodule-export-2.0)

add_executable(Dashboard ${SRC_FILES})
target_link_libraries(
    Dashboard
    PRIVATE
    PkgConfig::gtkmm
    PkgConfig::adw
    PkgConfig::gme
    all_resources
)
target_include_directories(
    Dashboard
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
