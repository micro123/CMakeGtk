#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include "cpr/cprtypes.h"
#include <cpr/parameters.h>
#include <nlohmann/json.hpp>

bool HttpGet(const std::string &ep, const cpr::Parameters &params, std::string &data, cpr::Header header = {});

template <typename T>
bool HttpGetJsonData(const std::string &ep, const cpr::Parameters &params, T &data, cpr::Header header = {})
{
    std::string str_content;
    bool str_ok = HttpGet(ep, params, str_content, std::move(header));
    if (str_ok) {
        auto content = nlohmann::json::parse(str_content);
        content.get_to(data);
    }
    return str_ok;
}

bool HttpPost(const std::string &ep, const std::string &body, std::string &data, cpr::Header header = {});

template <typename T>
bool HttpPostJsonData(const std::string &ep, const std::string &body, T &data, cpr::Header header = {})
{
    std::string str_content;
    bool str_ok = HttpPost(ep, body, str_content, std::move(header));
    if (str_ok) {
        auto content = nlohmann::json::parse(str_content);
        content.get_to(data);
    }
    return str_ok;
}

#endif /* HTTP_CLIENT_HPP */
