#include "xxt_bus.hpp"

#include <algorithm>
#include <cctype>
#include <glib.h>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <ctime>

#include <nlohmann/json.hpp>

#include "utils/custom_section.hpp"
#include "http/http_client.hpp"
#include "utils/json_adl.hpp"

static long long CurrentTime_us();
template <typename T> std::string minify_json(const T &obj);
static std::string Sign(const std::string &content, const std::string &key);

using opt_string = std::optional<std::string>;
template <typename T>
using vec = std::vector<T>;
template <typename T>
using opt_vec = std::optional<vec<T>>;

static struct ApiData {
    std::string apikey;
    std::string appid;
    std::string devno;
    std::string baseurl;
    std::string version;
    std::string versiontype;
} g_api_data;

struct BaseReqParam {
    std::string devno;
    int         devtype;
    float       direc;
    long long   gpstime;
    float       lat;
    float       lng;
    float       speed;
    std::string uid;
    std::string version;
    std::string versiontype;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(
        BaseReqParam, devno, devtype, direc, gpstime,
        lat, lng, speed, uid, version, versiontype
    )

    BaseReqParam(long long time) {
        devno = g_api_data.devno;
        devtype = 0;
        direc = 0.0;
        gpstime = time;
        lat = 0.0;
        lng = 0.0;
        speed = 0.0;
        uid = "";
        version = g_api_data.version;
        versiontype = g_api_data.versiontype;
    }

    template <typename T>
    BaseReqParam &operator=(const T& t) {
        return *this;
    }
};

template <typename T>
struct XxtBus {
    using RequestContent  = typename T::RequestData;
    using ResponseContent = typename T::ResponseData;

    struct Request {
        std::string     appid;
        RequestContent  data;
        long long       reqtime;
        BaseReqParam    reqpara;
        std::string     sign;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(Request, appid, data, reqpara, reqtime, sign)

        Request(RequestContent para): reqtime(CurrentTime_us()), reqpara(reqtime) {
            if constexpr (std::is_assignable_v<BaseReqParam, RequestContent>)
                reqpara = para;
            appid = g_api_data.appid;
            data = std::move(para);
            sign = MakeSign();
        }

        std::string MakeSign() {
            std::map<std::string, std::string> map;
            map["appid"] = appid;
            map["data"] = minify_json(data);
            map["reqpara"] = minify_json(reqpara);
            map["reqtime"] = std::to_string(reqtime);

            std::ostringstream oss;
            for (auto const &x: map) {
                oss << x.first << x.second;
            }
            return Sign(oss.str(), g_api_data.apikey);
        }
    };

    struct Response {
        int             retCode;
        ResponseContent retData;
        opt_string      retMsg;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Response, retCode, retData, retMsg);
    };

    template <typename ... Args>
    static ResponseContent Query(Args &&... args) {
        RequestContent request_content {std::forward<Args>(args)...};
        Request req{request_content};
        std::string url = g_api_data.baseurl + T::endpoint;
        Response response{};
        std::string body = minify_json(req);
        g_info("json: %s", body.c_str());
        HttpPostJsonData(
            url, minify_json(req), response,
            cpr::Header{
                {"Accept", "application/json"},
                {"Content-Type","application/json"},
                {"User-Agent","Dalvik/2.1.0 (Linux; U; Android 14)"},
            });
        return response.retData;
    }
};

// 根据 GPS 获取信息
struct GetByGPS {
    struct RequestData {
        float latitude;
        float longitude;
        float range;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(RequestData, latitude, longitude, range);
    };

    struct ResponseData {
        struct StationInfo {
            int         c;
            int         i;
            std::string n;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                StationInfo, c, i, n
            );
        };


        vec<StationInfo> bus;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            ResponseData, bus
        );
    };

    static constexpr const char* endpoint = "system/search/v2/getByGPS";
};
template <>
BaseReqParam &BaseReqParam::operator=<GetByGPS::RequestData>(const GetByGPS::RequestData &data) {
    lat = data.latitude;
    lng = data.longitude;
    return *this;
}

struct GetByName {
    struct RequestData {
        std::string name;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(
            RequestData, name
        );
    };

    struct ResponseData {
        struct RouteInfo {
            std::string start;
            std::string end;
            std::string n; // name
            std::string i; // id
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                RouteInfo, start, end, n, i
            );
        };
        struct StationInfo {
            std::string n;
            std::string i; // id
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                StationInfo, n, i
            );
        };
        struct BusInfo {
            vec<RouteInfo>   route;
            vec<StationInfo> station;
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                BusInfo, route, station
            );
        };
        BusInfo bus;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            ResponseData, bus
        );
    };

    static constexpr const char* endpoint = "system/search/getByName";
};


struct GetByStation {
    struct RequestData {
        std::string stationNameId;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(
            RequestData, stationNameId
        );
    };

    struct ResponseData {
        struct RouteStationInfo {
            std::string d;  // direction
            std::string ri; // route id
            std::string rsi; // route station id
            std::string rn; // route name
            std::string dn; // destination name
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                RouteStationInfo, d, ri, rsi, rn, dn
            );
        };
        
        int i; // station name id
        std::string n; // station name
        vec<RouteStationInfo> l; // route stations
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            ResponseData, i, n, l
        );
    };

    static constexpr const char* endpoint = "bus/routeStation/getByStation";
};

struct GetDynamic {
    struct RequestData {
        std::string direction;
        std::string routeId;
        std::string routeStationId;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(
            RequestData, direction, routeId, routeStationId
        );
    };

    struct ResponseData {
        struct TimeInfo {
            int count;
            std::string crowdExp;
            int crowdLevel;
            std::string fbt;
            std::string plateNumber;
            int time;
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                TimeInfo, count, crowdExp, crowdLevel, fbt, plateNumber, time
            );
        };
        vec<TimeInfo> time;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            ResponseData, time
        );
    };

    static constexpr const char* endpoint = "bus/route/getDynamic";
};

struct GetStatics {
    struct RequestData {
        std::string direction;
        std::string routeId;
        std::string uuid;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_ONLY_SERIALIZE(
            RequestData, direction, routeId, uuid
        );
    };

    struct ResponseData {
        struct RouteStationInfo {
            std::string rsi; // id
            std::string rsn; // name
            NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
                RouteStationInfo, rsi, rsn
            );
        };

        int direction;
        std::string ft;
        std::string lt;
        std::string price;
        std::string ri;
        std::string rn;
        vec<RouteStationInfo> rs;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            ResponseData, direction, ft, lt, price, ri, rn, rs
        );
    };

    static constexpr const char* endpoint = "bus/route/getStatics";
};

long long CurrentTime_us()
{
    time_t now = time(nullptr);
    return now * 1000 + g_random_int_range(0, 999);
}

template <typename T> std::string minify_json(const T &obj)
{
    auto j = nlohmann::json();
    j = obj;
    return j.dump();
}

std::string Sign(const std::string &content, const std::string &key)
{
    auto chk = g_checksum_new(G_CHECKSUM_SHA1);
    g_checksum_update(chk, (const guchar*)key.data(), key.length());
    g_checksum_update(chk, (const guchar*)content.data(), content.length());
    g_checksum_update(chk, (const guchar*)key.data(), key.length());
    std::string sum = g_checksum_get_string(chk);
    g_checksum_free(chk);
    std::for_each(begin(sum), end(sum), [](auto &x) { x = toupper(x); });
    return sum;
}

BEFORE_APP(random_seed, 1)
{
    g_random_set_seed(time(NULL));
    // load private config data
    
}

TEST_AUTOREG(xxt_bus_tests) {
    auto data1 = XxtBus<GetByGPS>::Query(23.183f,113.417f,500.f);
    auto data2 = XxtBus<GetByName>::Query("564路");
    auto data3 = XxtBus<GetByStation>::Query("4374");
    auto data4 = XxtBus<GetDynamic>::Query("0", "277", "4251197");
    auto data5 = XxtBus<GetStatics>::Query("0", "277");
    int a = 0;
}

