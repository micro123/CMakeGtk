#include "router_tab.hpp"

AdwTabPage * create_router_tab(AdwTabView *host, const TabInfo *info) {
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    // TODO: 配置UI逻辑
    return p;
}
