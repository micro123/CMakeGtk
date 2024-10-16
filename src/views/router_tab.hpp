#ifndef ROUTER_TAB_HPP
#define ROUTER_TAB_HPP

#include "views/ui_tab_info.hpp"

AdwTabPage* create_router_tab(AdwTabView* host, const TabInfo* info, page_active_cb *active_cb);

#endif // ROUTER_TAB_HPP
