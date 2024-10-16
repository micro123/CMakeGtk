#include "settings_tab.hpp"
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>

#include "data/setting_data.hpp"
#include "utils/gobj_prop.hpp"

#define GW(builder, group,field,type) \
    group.field = type(gtk_builder_get_object(builder, #group "" #field))

class SettingBussiness {
public:
    SettingBussiness(GtkBuilder *builder, AdwTabPage *page) {
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
        g_log("Dashboard", G_LOG_LEVEL_WARNING, "Setting Load");
        settings_reload(); // reload config file
        {
            // qweather
            auto city_id = settings_get_str(SETTING_KEY_QWEATHER_CITY_ID, "123");
            set_prop(qweather_.city_id, "text", city_id);
        }
    }

    void save() {
        g_log("Dashboard", G_LOG_LEVEL_WARNING, "Setting Save");
        {
            auto city_id = get_prop<std::string>(qweather_.city_id, "text");
            settings_set_str(SETTING_KEY_QWEATHER_CITY_ID, city_id);
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
        AdwEntryRow *ip_addr;
        AdwSpinRow  *port_num;
        AdwComboRow *ethernet_device;
        AdwComboRow *thermal_sensor;
        GtkButton   *fetch_devices;
    } router_;
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
