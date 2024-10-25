#ifndef PATH_TOOL_HPP
#define PATH_TOOL_HPP

#include <string>
#include <fmt/format.h>

/**
 * 获取EXE/ELF文件名，不包含后缀
 */
std::string_view GetAppExecutableName();

/**
 * 获取EXE/ELF文件完整路径
 */
std::string_view GetAppExecutablePath();

/**
 * 获取程序EXE/ELF文件所在目录
 */
std::string_view GetAppBinaryDirPath();

/**
 * 获取应用程序数据目录
 * Win32: %APPDATA%\<Exe文件名>\
 * *nix : ~/.cache/<elf文件名>
 */
std::string_view GetAppDataDirPath();

/**
 * 获取应用程序数据目录
 * Win32: %APPDATA%\<Exe文件名>\
 * *nix : ~/.config/<elf文件名>
 */
std::string_view GetAppCfgDirPath();

/**
 * 获取临时文件夹路径
 */
std::string_view GetTempDirPath();

/**
 * 获取临时文件夹下的一个路径
 */
std::string GetTempFilePath(const char* relPath);

/**
 * 获取程序资源目录下的资源
 */
std::string GetAssetsPath(const char* relPath);

/**
 * 获取AppData/<AppName>目录下的资源
 */
std::string GetDataPath(const char* relPath);

/**
 * 获取.config/<AppName>目录下的资源
 */
std::string GetConfigPath(const char *relPath);


std::string_view GetDesktopPath();

inline std::string operator""_AP(const char* relPath, size_t)
{
    return GetAssetsPath(relPath);
}

inline std::string operator""_DP(const char* relPath, size_t)
{
    return GetDataPath(relPath);
}

inline std::string operator""_TP(const char *relPath, size_t)
{
    return GetTempFilePath(relPath);
}

inline std::string operator""_CP(const char *relPath, size_t)
{
    return GetConfigPath(relPath);
}

#endif /* PATH_TOOL_HPP */
