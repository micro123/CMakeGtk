//
// Created by tang on 24-10-13.
//

#ifndef UI_TAB_INFO_HPP
#define UI_TAB_INFO_HPP

#include <adwaita.h>

#define PAGE_PRIVATE_DATA_KEY "page_private"

enum TabId {
    T_QWeather,
    T_XxtBus,
    T_Router,

    T_Settings,

    T_CNT,
};

using page_active_cb = void(*)(gpointer user);
struct TabInfo {
    const char   *title;
    const char   *icon_name;
    const char   *ui_resource;
    AdwTabPage* (*setup)(AdwTabView*, const TabInfo*, page_active_cb *active_cb);
};

extern const TabInfo tab_infos[T_CNT];

AdwTabPage* create_page(AdwTabView* host, const TabInfo* info, GtkBuilder *&builder);

#endif //UI_TAB_INFO_HPP
