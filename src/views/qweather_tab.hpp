#ifndef QWEATHER_TAB_HPP
#define QWEATHER_TAB_HPP

#include "views/ui_tab_info.hpp"

AdwTabPage* create_qweather_tab(AdwTabView* host, const TabInfo* info, page_active_cb *active_cb);

#endif // QWEATHER_TAB_HPP
