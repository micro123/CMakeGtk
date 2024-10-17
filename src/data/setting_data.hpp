#ifndef SETTING_DATA_HPP
#define SETTING_DATA_HPP

// 天气设置
#define SETTING_KEY_QWEATHER_API_KEY "qweather_api_key"
#define SETTING_KEY_QWEATHER_CITY_ID "qweather_city_id"
#define SETTING_KEY_QWEATHER_UPDATE_INTERVAL "qweather_update_interval"

// 公交设置
#define SETTING_KEY_XXT_BUS_INFO_TEXT "xxt_bus_info_text"
#define SETTING_KEY_XXT_BUS_UPDATE_INTERVAL "xxt_bus_update_interval"

// 路由器设置
#define SETTING_KEY_ROUTER_IP_ADDRESS "router_ip_address"
#define SETTING_KEY_ROUTER_PORT "router_port"
#define SETTING_KEY_ROUTER_NET_DEV "router_net_dev"
#define SETTING_KEY_ROUTER_TEMP_SENSOR "router_temp_sensor"

#include <string>
#include <glib.h>

using str = std::string;
using boolean = bool;

#define SETTING_LIST(EXPANDER) \
    EXPANDER(gint);            \
    EXPANDER(guint);           \
    EXPANDER(gint64);          \
    EXPANDER(guint64);         \
    EXPANDER(float);           \
    EXPANDER(double);          \
    EXPANDER(str);             \
    EXPANDER(boolean)

#define DECLARE_SETTING_TYPE(type) \
type settings_get_##type(const char *key, type def); \
void settings_set_##type(const char *key, type val)

SETTING_LIST(DECLARE_SETTING_TYPE);

#undef DECLARE_SETTING_TYPE

void settings_reload();
void settings_save();

#endif /* SETTING_DATA_HPP */
