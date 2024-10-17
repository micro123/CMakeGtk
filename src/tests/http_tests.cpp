#include "utils/custom_section.hpp"

#include "http/http_client.hpp"

TEST_AUTOREG(http_get)
{
    std::string content;
    bool ok = HttpGet("http://www.baidu.com", {}, content);
    if (ok) {
        int a = 0;
    }
}
