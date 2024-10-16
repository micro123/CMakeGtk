#include "settings_tab.hpp"
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>

#include "data/setting_data.hpp"

class SettingBussiness {
public:
    SettingBussiness(GtkBuilder *builder) {
        auto b = Glib::wrap(builder);
        auto somebtn = b->get_widget<Gtk::Button>("btn_save_weather_settings");
        somebtn->signal_clicked().connect([]{
            settings_save();
        });
    }
    ~SettingBussiness() {
        int a = 0;
    }

private:
};

static void del(SettingBussiness *s) { delete s; }

AdwTabPage * create_settings_tab(AdwTabView *host, const TabInfo *info) {
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    auto x = new SettingBussiness(builder);
    g_signal_connect(host, "destroy", G_CALLBACK(del), x);
    return p;
}
