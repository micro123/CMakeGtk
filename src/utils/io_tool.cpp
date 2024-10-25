#include "io_tool.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <glib.h>

std::string ReadTextFile (const std::string &path)
{
    std::string text;
    int fd = open(path.c_str(), O_RDONLY);
    if (fd >= 0) {
        // get file length
        const auto length = lseek(fd, 0, SEEK_END);
        text.resize(length);
        lseek(fd,0,SEEK_SET);
        read(fd, text.data(), length);
        close(fd);
    }
    else {
        g_warning("read file %s failed: %d", path.c_str(), errno);
    }
    return text;
}

void WriteTextFile(const std::string &path, const std::string &content)
{
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
    if (fd >= 0) {
        write(fd, content.data(), content.length());
        close(fd);
    }
    else {
        g_warning("write file %s failed: %d", path.c_str(), errno);
    }
}
