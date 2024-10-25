#ifndef CRYPTO_TOOL_HPP
#define CRYPTO_TOOL_HPP

#include <string>

std::string EncryptDataAes256(const std::string& input, const std::string &password);
bool        DecryptDataAes256(const std::string& input, const std::string &password, std::string &output);

#endif /* CRYPTO_TOOL_HPP */
