#include <gtkmm/application.h>
#include <adwaita.h>

#include "views/root_view.hpp"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("com.example.dashboard", Gtk::Application::Flags::DEFAULT_FLAGS);
    app->signal_activate().connect([app]{
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
        setup_root_view(app);
    });
    return app->run(argc, argv);
}
