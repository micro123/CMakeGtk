#include "settings_tab.hpp"
#include <adwaita.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>

#include "data/setting_data.hpp"
#include "utils/gobj_prop.hpp"

// Get Widget
#define GW(builder, group,field,type) \
    group.field = type(gtk_builder_get_object(builder, #group "" #field))

using str = std::string;

// setting to widget
#define s2w(w, sk, t, p, def) \
do { \
    t sv = settings_get_##t(sk, def); \
    set_prop(w, p, sv); \
} while(0)

// widget to setting
#define w2s(w, sk, t, p) \
do { \
    t wv = get_prop<t>(w, p); \
    settings_set_##t(sk, wv); \
} while(0)

class SettingBussiness {
public:
    SettingBussiness(GtkBuilder *builder) {
        auto cb = Glib::wrap(builder);

        // entries
        {
            // qweather
            GW(builder, qweather_, city_id, ADW_ENTRY_ROW);
            GW(builder, qweather_, api_key, ADW_ENTRY_ROW);
            GW(builder, qweather_, update_interval, ADW_COMBO_ROW);
        }

        {
            // xxt_bus
            GW(builder, xxt_bus_, bus_info, ADW_ENTRY_ROW);
            GW(builder, xxt_bus_, update_interval, ADW_COMBO_ROW);
        }

        {
            // router
            GW(builder, router_, ip_addr, ADW_ENTRY_ROW);
            GW(builder, router_, port_num, ADW_SPIN_ROW);
            GW(builder, router_, ethernet_device, ADW_COMBO_ROW);
            GW(builder, router_, net_exachange, ADW_SWITCH_ROW);
            GW(builder, router_, thermal_sensor, ADW_COMBO_ROW);
            GW(builder, router_, fetch_devices, GTK_BUTTON);
        }


        // save and reload
        auto save_btn  = cb->get_widget<Gtk::Button>("btn_settings_save");
        save_btn->signal_clicked().connect([this]{ save(); });
        auto reset_btn = cb->get_widget<Gtk::Button>("btn_settings_reset");
        reset_btn->signal_clicked().connect([this]{ reload(); });
    }
    ~SettingBussiness() {
        
    }

    void reload() {
        g_info("Setting Load");
        settings_reload(); // reload config file
        {
            // qweather
            s2w(qweather_.city_id, SETTING_KEY_QWEATHER_CITY_ID, str, "text", "");
            s2w(qweather_.api_key, SETTING_KEY_QWEATHER_API_KEY, str, "text", "");
            s2w(qweather_.update_interval, SETTING_KEY_QWEATHER_UPDATE_INTERVAL, guint, "selected", ~guint(0));

            // xxt bus
            s2w(xxt_bus_.bus_info, SETTING_KEY_XXT_BUS_INFO_TEXT, str, "text", "");
            s2w(xxt_bus_.update_interval, SETTING_KEY_XXT_BUS_UPDATE_INTERVAL, guint, "selected", ~guint(0));

            // router
            s2w(router_.ip_addr, SETTING_KEY_ROUTER_IP_ADDRESS, str, "text", "");
            s2w(router_.port_num, SETTING_KEY_ROUTER_PORT, double, "value", 19999);
            s2w(router_.ethernet_device, SETTING_KEY_ROUTER_NET_DEV, guint, "selected", ~guint(0));
            s2w(router_.net_exachange, SETTING_KEY_ROUTER_NET_EXCHANGE, boolean, "active", false);
            s2w(router_.thermal_sensor, SETTING_KEY_ROUTER_TEMP_SENSOR, guint, "selected", ~guint(0));
        }
    }

    void save() {
        g_info("Setting Save");
        {
            // qweather
            w2s(qweather_.city_id, SETTING_KEY_QWEATHER_CITY_ID, str, "text");
            w2s(qweather_.api_key, SETTING_KEY_QWEATHER_API_KEY, str, "text");
            w2s(qweather_.update_interval, SETTING_KEY_QWEATHER_UPDATE_INTERVAL, guint, "selected");

            // xxt bus
            w2s(xxt_bus_.bus_info, SETTING_KEY_XXT_BUS_INFO_TEXT, str, "text");
            w2s(xxt_bus_.update_interval, SETTING_KEY_XXT_BUS_UPDATE_INTERVAL, guint, "selected");

            // router
            w2s(router_.ip_addr, SETTING_KEY_ROUTER_IP_ADDRESS, str, "text");
            w2s(router_.port_num, SETTING_KEY_ROUTER_PORT, double, "value");
            w2s(router_.ethernet_device, SETTING_KEY_ROUTER_NET_DEV, guint, "selected");
            w2s(router_.net_exachange, SETTING_KEY_ROUTER_NET_EXCHANGE, boolean, "active");
            w2s(router_.thermal_sensor, SETTING_KEY_ROUTER_TEMP_SENSOR, guint, "selected");
        }
        settings_save();
    }

private:
    // qweather
    struct {
        AdwEntryRow *city_id;
        AdwEntryRow *api_key;
        AdwComboRow *update_interval;
    } qweather_;

    // xxtbus
    struct {
        AdwEntryRow *bus_info;
        AdwComboRow *update_interval;
    } xxt_bus_;

    // router
    struct {
        AdwEntryRow  *ip_addr;
        AdwSpinRow   *port_num;
        AdwComboRow  *ethernet_device;
        AdwSwitchRow *net_exachange;
        AdwComboRow  *thermal_sensor;
        GtkButton    *fetch_devices;
    } router_;
};

static void del(SettingBussiness *s) { delete s; }

static void page_attached(SettingBussiness *b) {
    b->reload();
}

AdwTabPage *create_settings_tab(AdwTabView *host, const TabInfo *info, page_active_cb *active_cb) {
    GtkBuilder *builder = nullptr;
    auto p = create_page(host, info, builder);
    auto x = new SettingBussiness(builder);
    g_object_set_data(G_OBJECT(p), PAGE_PRIVATE_DATA_KEY, x);
    *active_cb = (page_active_cb)page_attached;
    return p;
}
