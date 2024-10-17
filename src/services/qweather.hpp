#ifndef QWEATHER_HPP
#define QWEATHER_HPP

#include <nlohmann/json.hpp>

// 当前天气信息返回结果
struct CurrentWeatherData {
    struct NowData {
        std::string text;
        std::string icon;
        std::string temp;
        std::string humidity;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(NowData, text, icon, temp, humidity);
    };

    std::string code;
    std::string updateTime;
    NowData     now;

    bool valid() const { return code == "200"; }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CurrentWeatherData, code, updateTime, now);
};

// 按小时天气预报返回结果
struct HourlyForecastData {
    struct HourlyData {
        std::string fxTime;
        std::string text;
        std::string icon;
        std::string temp;
        std::string humidity;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(HourlyData, fxTime, text, icon, temp, humidity);
    };

    std::string                 code;
    std::string                 updateTime;
    std::vector<HourlyData>     hourly;

    bool valid() const { return code == "200"; }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(HourlyForecastData, code, updateTime, hourly);
};

struct CityInfoData {
    struct City {
        std::string name;
        std::string adm1;
        std::string adm2;
        std::string country;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(City, name, adm1, adm2, country);
    };

    std::string       code;
    std::vector<City> location;

    bool valid() const { return code == "200"; }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CityInfoData, code, location);
};

bool get_current_weather(CurrentWeatherData &current);

bool get_hourly_forecast(HourlyForecastData &hourly);

bool get_city_info(CityInfoData &cityInfo);

#endif /* QWEATHER_HPP */
