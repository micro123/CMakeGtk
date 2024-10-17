#include "http_client.hpp"
#include "cpr/connect_timeout.h"
#include "cpr/cprtypes.h"
#include "cpr/session.h"
#include "cpr/timeout.h"

#define DEFAULT_TIMEOUT 3000

bool HttpGet(const std::string &ep, const cpr::Parameters &params, std::string &data)
{
    cpr::Session session;
    session.SetTimeout(cpr::Timeout(DEFAULT_TIMEOUT));
    session.SetConnectTimeout(cpr::ConnectTimeout(DEFAULT_TIMEOUT));
    session.SetUrl(cpr::Url(ep));
    session.SetParameters(params);
    auto res = session.Get();
    if (res.status_code == 200) { // ok
        data = res.text;
        return true;
    }
    return false;
}
