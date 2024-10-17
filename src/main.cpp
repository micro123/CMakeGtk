#include <gdkmm/display.h>
#include <gtk/gtk.h>
#include <gtkmm/application.h>
#include <adwaita.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

#include "views/root_view.hpp"
#include "widgets/widgets.hpp"
#include "tests/all_tests.hpp"

static void load_app_style() {
    auto provider = Gtk::CssProvider::create();
    provider->load_from_resource("/dashboard/styles/app_style.css");

    auto screen = Gdk::Display::get_default();
    Gtk::StyleContext::add_provider_for_display(screen, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int main(int argc, char *argv[])
{
    // run tests
    run_all_tests();

    auto app = Gtk::Application::create("com.example.dashboard", Gtk::Application::Flags::DEFAULT_FLAGS);
    app->signal_activate().connect([app]{
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);

        // styles
        load_app_style();

        // register widgets
        register_widgets();

        // create views
        setup_root_view(app);
    });
    app->register_application();
    return app->run(argc, argv);
}
