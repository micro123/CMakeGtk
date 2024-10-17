file(GLOB_RECURSE SRC_FILES *.*)

pkg_check_modules(gtkmm REQUIRED IMPORTED_TARGET gtkmm-4.0)
pkg_check_modules(adw REQUIRED IMPORTED_TARGET libadwaita-1)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(BUILD_SHARED_LIBS OFF)
set(CPR_USE_SYSTEM_CURL ON)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.0
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(fmt)

add_executable(Dashboard ${SRC_FILES})
target_link_libraries(
    Dashboard
    PRIVATE
    PkgConfig::gtkmm
    PkgConfig::adw
    nlohmann_json::nlohmann_json
    cpr::cpr
    fmt::fmt
    all_resources
)
target_include_directories(
    Dashboard
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
