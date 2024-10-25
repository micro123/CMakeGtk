#include "http_client.hpp"
#include "cpr/body.h"
#include "cpr/connect_timeout.h"
#include "cpr/cprtypes.h"
#include "cpr/redirect.h"
#include "cpr/session.h"
#include "cpr/timeout.h"

#define DEFAULT_TIMEOUT 3000

static void MergeHeaders(cpr::Header &dst, const cpr::Header &from)
{
    for (auto const &x: from) {
        if (dst.count(x.first))
            continue;
        dst[x.first] = x.second;
    }
}

bool HttpGet(const std::string &ep, const cpr::Parameters &params, std::string &data, cpr::Header header)
{
    cpr::Session session;
    session.SetRedirect(cpr::Redirect(true));
    MergeHeaders(header, cpr::Header({
        {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
        {"Accept-Encoding", "gzip, deflate, br, zstd"},
        {"Accept-Language", "zh-CN,zh;q=0.9"},
    }));
    session.SetHeader(header);
    session.SetTimeout(cpr::Timeout(DEFAULT_TIMEOUT));
    session.SetConnectTimeout(cpr::ConnectTimeout(DEFAULT_TIMEOUT));
    session.SetUrl(cpr::Url(ep));
    session.SetParameters(params);
    auto res = session.Get();
    if (res.status_code == 200) { // ok
        data = res.text;
        printf("Get: %s\n", data.c_str());
        return true;
    }
    return false;
}

bool HttpPost(const std::string &ep, const std::string &body, std::string &data, cpr::Header header)
{
    cpr::Session session;
    session.SetRedirect(cpr::Redirect(true));
    MergeHeaders(header, cpr::Header({
        {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"},
        {"Accept-Encoding", "gzip, deflate, br, zstd"},
        {"Accept-Language", "zh-CN,zh;q=0.9"},
    }));
    session.SetHeader(header);
    session.SetTimeout(cpr::Timeout(DEFAULT_TIMEOUT));
    session.SetConnectTimeout(cpr::ConnectTimeout(DEFAULT_TIMEOUT));
    session.SetUrl(cpr::Url(ep));
    session.SetBody(cpr::Body{body.data(), body.length()});
    auto res = session.Post();
    if (res.status_code == 200) { // ok
        data = res.text;
        printf("Post: %s\n", data.c_str());
        return true;
    }
    return false;
}
