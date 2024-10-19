#include "router_tab.hpp"
#include <gtkmm/builder.h>

#include "widgets/some_widget.hpp"

AdwTabPage * create_router_tab(AdwTabView *host, const TabInfo *info, page_active_cb *active_cb) {
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    // TODO: 配置UI逻辑

    auto b = Glib::wrap(builder, true);
    auto mw = b->get_widget_derived<MyWidget>(b, "test");
    mw->property_number() = 21;

    g_object_unref(builder);
    return p;
}
