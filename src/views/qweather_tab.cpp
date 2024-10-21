#include "qweather_tab.hpp"

#include <gtkmm/builder.h>

#include "services/qweather.hpp"
#include <utils/gobj_prop.hpp>
#include <fmt/format.h>
#include <fmt/chrono.h>

#define G_LOG_DOMAIN "WeatherTab"

class WeatherBusiness
{
public:
    WeatherBusiness(GtkBuilder *builder)
    {
        // current
        current_.icon = gtk_builder_get_object(builder, "weather_icon");
        current_.city = gtk_builder_get_object(builder, "weather_city");
        current_.text = gtk_builder_get_object(builder, "weather_text");
        current_.temp = gtk_builder_get_object(builder, "weather_temp");
        current_.feels = gtk_builder_get_object(builder, "weather_feels");
        current_.humi = gtk_builder_get_object(builder, "weather_humi");
        current_.time = gtk_builder_get_object(builder, "weather_time");

        // hourly
        hourly_[0] = gtk_builder_get_object(builder, "next_1h");
        hourly_[1] = gtk_builder_get_object(builder, "next_2h");
        hourly_[2] = gtk_builder_get_object(builder, "next_3h");
        hourly_[3] = gtk_builder_get_object(builder, "next_4h");
        hourly_[4] = gtk_builder_get_object(builder, "next_5h");
        hourly_[5] = gtk_builder_get_object(builder, "next_6h");

        refresh_weather();

        // setup scheduled task
        g_timeout_add_seconds(60 * 5, (GSourceFunc)+[](WeatherBusiness *b) -> gboolean { b->refresh_weather(); return true; }, this);
    }

    void refresh_weather()
    {
        g_info("weather refresh begin");
        if (!skip_city_info_)
        {
            CityInfoData city;
            if (!get_city_info(city) || !city.valid())
            {
                return;
            }
            auto &loc = city.location.front();
            set_prop(current_.city, "value", fmt::format("{} - {}", loc.adm2, loc.name));
            skip_city_info_ = true;
        }

        {
            CurrentWeatherData current;
            if (!get_current_weather(current) || !current.valid())
            {
                return;
            }
            auto &now = current.now;
            set_prop(current_.icon, "resource", fmt::format("/dashboard/icons/{}.png", now.icon));
            set_prop(current_.text, "value", now.text);
            set_prop(current_.temp, "value", fmt::format("{} ℃", now.temp));
            set_prop(current_.feels, "value", fmt::format("{} ℃", now.feelsLike));
            set_prop(current_.humi, "value", fmt::format("{} %", now.humidity));
            set_prop(current_.time, "value", fmt::format("{:%H:%M}", fmt::localtime(time(NULL))));
        }

        {
            HourlyForecastData hourly;
            if (!get_hourly_forecast(hourly) || !hourly.valid())
            {
                return;
            }

            for (int i=0; i<std::size(hourly_); ++i) {
                auto &data = hourly.hourly[i];
                auto &view = hourly_[i];

                set_prop(view, "icon", fmt::format("/dashboard/icons/{}.png", data.icon));
                set_prop(view, "text", data.text);
                set_prop(view, "temp", fmt::format("{} ℃", data.temp));
                set_prop(view, "humi", fmt::format("{} %", data.humidity));
                set_prop(view, "time", data.fxTime.substr(11,5));
            }
        }
        g_info("weather refresh end");
    }

private:
    struct
    {
        GObject *icon;
        GObject *city;
        GObject *text;
        GObject *temp;
        GObject *feels;
        GObject *humi;
        GObject *time;
    } current_;

    GObject *hourly_[6];

    bool skip_city_info_{false};
};

AdwTabPage *create_qweather_tab(AdwTabView *host, const TabInfo *info, page_active_cb *active_cb)
{
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    auto x = new WeatherBusiness(builder);
    g_object_set_data(G_OBJECT(p), PAGE_PRIVATE_DATA_KEY, x);
    g_object_unref(builder);
    return p;
}
