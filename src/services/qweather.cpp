#include "qweather.hpp"
#include "data/setting_data.hpp"
#include "http/http_client.hpp"

struct QWeatherParams
{
    std::string city_id;
    std::string api_key;

    bool valid() const {
        return !(city_id.empty() || api_key.empty());
    }
};

static bool is_settings_ok(QWeatherParams &params)
{
    params.city_id = settings_get_str(SETTING_KEY_QWEATHER_CITY_ID, "");
    params.api_key = settings_get_str(SETTING_KEY_QWEATHER_API_KEY, "");
    return params.valid();
}

bool get_current_weather(CurrentWeatherData &current)
{
    QWeatherParams params;
    if (!is_settings_ok(params))
    {
        g_warning("Setting is invalid!");
        return false;
    }
    

    return HttpGetJsonData(
        "https://devapi.qweather.com/v7/weather/now",
        {{"key", params.api_key}, {"location", params.city_id}, {"lang", "zh"}, {"unit", "m"}},
        current);
}

bool get_hourly_forecast(HourlyForecastData &hourly)
{
    QWeatherParams params;
    if (!is_settings_ok(params))
    {
        g_warning("Setting is invalid!");
        return false;
    }

    return HttpGetJsonData(
        "https://devapi.qweather.com/v7/weather/24h",
        {{"key", params.api_key}, {"location", params.city_id}, {"lang", "zh"}, {"unit", "m"}},
        hourly);
}

bool get_city_info(CityInfoData &cityInfo)
{
    QWeatherParams params;
    if (!is_settings_ok(params))
    {
        g_warning("Setting is invalid!");
        return false;
    }

    return HttpGetJsonData(
        "https://geoapi.qweather.com/v2/city/lookup",
        {{"key", params.api_key}, {"location", params.city_id}, {"lang", "zh"}, {"unit", "m"}},
        cityInfo);
}
