#include "path_tool.hpp"
#include "fmt/format.h"

#include <filesystem>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cassert>
#include <fstream>

static void GetExePathImpl(char* out_path, size_t path_len);
static void GetBinDirPathImpl(char* out_path, size_t path_len);
static void GetDesktopDirPathImpl(char* out_path, size_t path_len);
static void GetDataDirPathImpl(char* out_path, size_t path_len);
static void GetCfgDirPathImpl(char* out_path, size_t path_len);
static void GetTempDirPathImpl(char* out_path, size_t path_len);

#define PATH_SEPARATOR "/"

static const char* GetExecutableFileName()
{
    static char NameBuffer[PATH_MAX]{ 0 };
    static const char* NamePtr = nullptr;
    if (NamePtr == nullptr)
    {
        realpath("/proc/self/exe", NameBuffer);
        NamePtr = basename(NameBuffer);
        const char* last_dot = strrchr(NamePtr, '.');
        if (last_dot) {
            ((char*)NamePtr)[last_dot - NamePtr] = 0;
        }
    }
    return NamePtr;
}

void GetBinDirPathImpl(char* out_path, size_t path_len)
{
    char path_temp[PATH_MAX]{ 0 };
    realpath("/proc/self/exe", path_temp);
    dirname(path_temp);
    strncpy(out_path, path_temp, std::min<size_t>(strlen(path_temp), path_len));
}

void GetExePathImpl(char* out_path, size_t path_len)
{
    char path_temp[PATH_MAX]{ 0 };
    realpath("/proc/self/exe", path_temp);
    strncpy(out_path, path_temp, std::min<size_t>(strlen(path_temp), path_len));
}

void GetDataDirPathImpl(char* out_path, size_t path_len)
{
    struct passwd* pw = getpwuid(getuid());
    const char* homedir = pw->pw_dir;
    char path_temp[PATH_MAX]{ 0 };
    strcpy(path_temp, homedir);
    strcat(path_temp, "/.cache/");
    strcat(path_temp, GetExecutableFileName());
    std::error_code ec;
    std::filesystem::create_directories(path_temp, ec);
    strncpy(out_path, path_temp, std::min<size_t>(strlen(path_temp), path_len));
}

void GetCfgDirPathImpl(char* out_path, size_t path_len)
{
    struct passwd* pw = getpwuid(getuid());
    const char* homedir = pw->pw_dir;
    char path_temp[PATH_MAX]{ 0 };
    strcpy(path_temp, homedir);
    strcat(path_temp, "/.config/");
    strcat(path_temp, GetExecutableFileName());
    std::error_code ec;
    std::filesystem::create_directories(path_temp, ec);
    strncpy(out_path, path_temp, std::min<size_t>(strlen(path_temp), path_len));
}

void GetDesktopDirPathImpl(char* out_path, size_t path_len)
{
    struct passwd* pw = getpwuid(getuid());

    auto desktop_path = std::filesystem::path(pw->pw_dir) / "Desktop";

    std::string path_str = desktop_path;

    memcpy(out_path, path_str.data(), std::min(path_str.length(), path_len));
}

void GetTempDirPathImpl(char* out_path, size_t path_len)
{
    memcpy(out_path, "/tmp", 5);
}

std::string_view GetAppExecutablePath ()
{
    static char s_exe_dir[256] = { 0 };
    if (s_exe_dir[0] == 0)
    {
        GetExePathImpl(s_exe_dir, 256);
    }

    return std::string_view{ s_exe_dir };
}

std::string_view GetAppBinaryDirPath()
{
    static char s_bin_dir[256] = { 0 };
    if (s_bin_dir[0] == 0)
    {
        GetBinDirPathImpl(s_bin_dir, 256);
    }

    return std::string_view{ s_bin_dir };
}

std::string_view GetAppDataDirPath()
{
    static char s_data_dir[256] = { 0 };
    if (s_data_dir[0] == 0)
    {
        GetDataDirPathImpl(s_data_dir, 256);
    }
    return std::string_view{ s_data_dir };
}

std::string GetAssetsPath(const char* relPath)
{
    return fmt::format("{}" PATH_SEPARATOR "{}", GetAppBinaryDirPath(), relPath);
}

std::string GetDataPath(const char* relPath)
{
    return fmt::format("{}" PATH_SEPARATOR "{}", GetAppDataDirPath(), relPath);
}

std::string GetConfigPath(const char *relPath)
{
    return fmt::format("{}" PATH_SEPARATOR "{}", GetAppCfgDirPath(), relPath);
}

std::string_view GetDesktopPath()
{
    static char s_desktop_dir[256] = { 0 };
    if (s_desktop_dir[0] == 0)
    {
        GetDesktopDirPathImpl(s_desktop_dir, 256);
    }

    return std::string_view{ s_desktop_dir };
}

std::string_view GetAppCfgDirPath()
{
    static char s_cfg_dir[256] = { 0 };
    if (s_cfg_dir[0] == 0)
    {
        GetCfgDirPathImpl(s_cfg_dir, 256);
    }
    return std::string_view{ s_cfg_dir };
}

std::string_view GetTempDirPath()
{
    static char s_temp_dir[256] = { 0 };
    if (s_temp_dir[0] == 0)
    {
        GetTempDirPathImpl(s_temp_dir, 256);
    }
    return std::string_view{ s_temp_dir };
}

std::string GetTempFilePath(const char *relPath)
{
    return fmt::format("{}" PATH_SEPARATOR "{}", GetTempDirPath(), relPath);
}

std::string_view GetAppExecutableName()
{
    return GetExecutableFileName();
}