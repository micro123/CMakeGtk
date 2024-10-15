//
// Created by tang on 24-10-13.
//

#include "ui_tab_info.hpp"

#include "views/qweather_tab.hpp"
#include "views/xxt_bus_tab.hpp"
#include "views/router_tab.hpp"
#include "views/settings_tab.hpp"

const TabInfo tab_infos[T_CNT] = {
    {"天气预报", "weather-few-clouds", "/dashboard/ui/qweather_tab.ui", &create_qweather_tab},
    {"公交信息", "applications-internet", "/dashboard/ui/xxt_bus_tab.ui", &create_xxt_bus_tab},
    {"路由信息", "applications-utilities", "/dashboard/ui/router_tab.ui", &create_router_tab},
    {"程序设置", "preferences-system", "/dashboard/ui/settings_tab.ui", &create_settings_tab},
};


AdwTabPage * create_page(AdwTabView *host, const TabInfo *info, GtkBuilder *&builder) {
    builder = gtk_builder_new_from_resource(info->ui_resource);
    GtkWidget *view = GTK_WIDGET(gtk_builder_get_object(builder, "root"));
    AdwTabPage *page = adw_tab_view_append_pinned(host, view);
    adw_tab_page_set_title(page, info->title);
    if (info->icon_name) {
        adw_tab_page_set_icon(page, g_themed_icon_new(info->icon_name));
    }
    return page;
}
