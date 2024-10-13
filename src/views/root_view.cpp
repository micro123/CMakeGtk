#include "root_view.hpp"
#include <gtkmm/builder.h>
#include <gtkmm/applicationwindow.h>

#include <adwaita.h>

static Glib::RefPtr<Glib::Object> tab_view;

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

void setup_root_view(const Glib::RefPtr<Gtk::Application> &app)
{
    auto builder = Gtk::Builder::create_from_resource("/dashboard/ui/main.ui");
    
    auto window = builder->get_widget<Gtk::ApplicationWindow>("root_window");
    tab_view = builder->get_object("tab_view");

    app->add_window(*window);
    window->show();
}
