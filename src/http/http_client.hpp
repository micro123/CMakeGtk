#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <cpr/parameters.h>
#include <nlohmann/json.hpp>

bool HttpGet(const std::string &ep, const cpr::Parameters &params, std::string &data);

template <typename T>
bool HttpGetJsonData(const std::string &ep, const cpr::Parameters &params, T &data)
{
    std::string str_content;
    bool str_ok = HttpGet(ep, params, str_content);
    if (str_ok) {
        auto content = nlohmann::json::parse(str_content);
        content.get_to(data);
    }
    return str_ok;
}

#endif /* HTTP_CLIENT_HPP */
