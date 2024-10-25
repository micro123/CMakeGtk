#ifndef IO_TOOL_HPP
#define IO_TOOL_HPP

#include <string>

std::string ReadTextFile(const std::string& path);
void WriteTextFile(const std::string& path, const std::string &content);

#endif /* IO_TOOL_HPP */
