#ifndef SETTINGS_TAB_HPP
#define SETTINGS_TAB_HPP

#include "views/ui_tab_info.hpp"

AdwTabPage* create_settings_tab(AdwTabView* host, const TabInfo* info, page_active_cb *active_cb);

#endif // SETTINGS_TAB_HPP
