#include "settings_tab.hpp"
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>

#include "data/setting_data.hpp"

class SettingBussiness {
public:
    SettingBussiness(GtkBuilder *builder, AdwTabPage *page) {
        // root widget
        auto root = GTK_WIDGET(gtk_builder_get_object(builder, "root"));
    }
    ~SettingBussiness() {
        
    }

    void reload() {
        g_log("Dashboard", G_LOG_LEVEL_WARNING, "Setting Reload");
    }

private:
};

static void del(SettingBussiness *s) { delete s; }

static void page_attached(SettingBussiness *b) {
    b->reload();
}

AdwTabPage *create_settings_tab(AdwTabView *host, const TabInfo *info, page_active_cb *active_cb) {
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    auto x = new SettingBussiness(builder, p);
    g_object_set_data(G_OBJECT(p), PAGE_PRIVATE_DATA_KEY, x);
    *active_cb = (page_active_cb)page_attached;
    return p;
}
