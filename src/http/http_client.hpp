#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <cpr/parameters.h>

bool HttpGet(const std::string &ep, const cpr::Parameters &params, std::string &data);

#endif /* HTTP_CLIENT_HPP */
