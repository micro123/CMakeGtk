#include "root_view.hpp"
#include <glibconfig.h>
#include <glibmm/varianttype.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>

#include <adwaita.h>

#include "utils/glib_utility.hpp"
#include "views/ui_tab_info.hpp"

class RootView
{
public:
    RootView(const g_ptr<Gtk::Application> &app) {
        builder = Gtk::Builder::create_from_resource("/dashboard/ui/main.ui");

        main_window = builder->get_widget<Gtk::Window>("root_window");
        app->add_window(*main_window);
        main_window->show();

        // buttons
        btn_prev_tab = builder->get_widget<Gtk::Button>("btn_prev_tab");
        btn_next_tab = builder->get_widget<Gtk::Button>("btn_next_tab");
        btn_show_settings = builder->get_widget<Gtk::Button>("btn_show_settings");

        btn_prev_tab->signal_clicked().connect([this] {
            tab_view_page_mod(-1);
        });
        btn_next_tab->signal_clicked().connect([this] {
            tab_view_page_mod(1);
        });
        btn_show_settings->signal_clicked().connect([this] {
            adw_tab_view_set_selected_page(tab_view, tab_pages[T_Settings].page);
        });

        // tab objects
        tab_view = ADW_TAB_VIEW(gtk_builder_get_object(builder->gobj(), "tab_view"));
        memset(tab_pages, 0, sizeof(tab_pages));
        for (unsigned i=0; i<std::size(tab_infos); i++) {
            const auto &info = tab_infos[i];
            tab_pages[i].page = info.setup(tab_view, &info, &tab_pages[i].active_cb);
        }

        // initial state
        adw_tab_view_set_selected_page(tab_view, tab_pages[T_QWeather].page);

        // page_attached
        g_signal_connect(G_OBJECT(tab_view), "notify::selected-page", G_CALLBACK(&RootView::OnTabPageSelect), this);

        // additional actions
        app->add_action_with_parameter("to-page", Glib::VARIANT_TYPE_INT32, [this](const Glib::VariantBase &index) {
            gint32 idx = index.get_dynamic<gint32>();
            if (idx < 0 || idx >= T_CNT)
                idx = T_Settings;
            adw_tab_view_set_selected_page(tab_view, tab_pages[idx].page);
        });
        app->add_action("next-page", [this]{ tab_view_page_mod(1); });
        app->add_action("prev-page", [this]{ tab_view_page_mod(-1); });

        app->set_accel_for_action("app.next-page", "Right");
        app->set_accel_for_action("app.prev-page", "Left");
        app->set_accels_for_action("app.to-page(-1)", {"Up","Down", "s"});
        app->set_accels_for_action("app.quit", {"Escape","q"});

        // pages
        app->set_accel_for_action("app.to-page(0)", "1");
        app->set_accel_for_action("app.to-page(1)", "2");
        app->set_accel_for_action("app.to-page(2)", "3");
    }

    ~RootView() {

    }
private:
    static void OnTabPageSelect(GObject *tab_view, GParamSpec *spec, RootView *pThis)
    {
        GValue page;
        g_object_get_property(tab_view, spec->name, &page);
        AdwTabPage *adw_tab_page = ADW_TAB_PAGE(g_value_get_object(&page));
        pThis->NotifyPageSelected(adw_tab_page);
    }

    void NotifyPageSelected(AdwTabPage *p)
    {
        for (const auto &x: tab_pages) {
            if (p == x.page) {
                if (x.active_cb) {
                    x.active_cb(g_object_get_data(G_OBJECT(p), PAGE_PRIVATE_DATA_KEY));
                }
            }
        }
    }

    void tab_view_page_mod(int diff) const {
        constexpr int max = T_Settings;
        auto const sel = adw_tab_view_get_selected_page(tab_view);
        const auto sel_idx = adw_tab_view_get_page_position(tab_view, sel);
        adw_tab_view_set_selected_page(tab_view, tab_pages[(sel_idx + max + diff) % max].page);
    }

    g_ptr<Gtk::Builder> builder;

    // root window
    Gtk::Window *main_window;

    // btns
    Gtk::Button *btn_prev_tab;
    Gtk::Button *btn_next_tab;
    Gtk::Button *btn_show_settings;

    // tab
    AdwTabView *tab_view;
    // AdwTabPage *tab_pages[std::size(tab_infos)];
    struct PageData {
        AdwTabPage     *page;
        page_active_cb  active_cb;
    } tab_pages[std::size(tab_infos)];
};

/*
static void tabview_select_page(AdwTabView *tabview, int page) {
    if (tabview == nullptr)
        tabview = ADW_TAB_VIEW(tab_view->gobj());
    if (page < 0)
        page = adw_tab_view_get_n_pages(tabview) - 1;
    adw_tab_view_set_selected_page(tabview, adw_tab_view_get_nth_page(tabview, page));
}

static void tabview_select_mod(int diff) {
    AdwTabView *tabview = ADW_TAB_VIEW(tab_view->gobj());
    auto const max = adw_tab_view_get_n_pages(tabview) - 1; // last page is setting
    auto current = adw_tab_view_get_page_position(tabview, adw_tab_view_get_selected_page(tabview));
    tabview_select_page(tabview, ((current + diff + max) % max));
}

extern "C" {

G_MODULE_EXPORT
void tab_previous(GtkButton *b, gpointer arg)
{
    tabview_select_mod(-1);
}

G_MODULE_EXPORT
void tab_next(GtkButton* b, gpointer arg)
{
    tabview_select_mod(1);
}

G_MODULE_EXPORT
void tab_settings(GtkButton* b, gpointer arg)
{
    tabview_select_page(nullptr, -1);
}

}
*/

void setup_root_view(const Glib::RefPtr<Gtk::Application> &app)
{
    auto rv = new RootView(app);
    app->signal_shutdown().connect([rv] {
            delete rv;
        });
}

extern "C" {
    G_MODULE_EXPORT
    void fuck_you(AdwTabView *self, AdwTabPage *page, gint pos, gpointer user) {
        int a = 0;
    }
}
