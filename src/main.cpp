#include <gdkmm/display.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gtkmm/application.h>
#include <adwaita.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

#include "views/root_view.hpp"
#include "widgets/widgets.hpp"
#include "tests/all_tests.hpp"
#include "init/init_calls.hpp"

static void load_app_style() {
    auto provider = Gtk::CssProvider::create();
    provider->load_from_resource("/dashboard/styles/app_style.css");

    auto screen = Gdk::Display::get_default();
    Gtk::StyleContext::add_provider_for_display(screen, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// static void my_log_handler(const gchar *domain, GLogLevelFlags flags, const gchar *message, gpointer user)
// {
//     switch (flags & G_LOG_LEVEL_MASK)
//     {
//     case G_LOG_LEVEL_DEBUG:
//         fprintf(stdout, "%s-DEBUG **: %s\n", domain, message);
//         break;
//     case G_LOG_LEVEL_INFO:
//         fprintf(stdout, "%s-INFO **: %s\n", domain, message);
//         break;
//     case G_LOG_LEVEL_MESSAGE:
//         fprintf(stdout, "%s-MESSAGE **: %s\n", domain, message);
//         break;
//     case G_LOG_LEVEL_WARNING:
//         fprintf(stdout, "%s-WARNING **: %s\n", domain, message);
//         break;
//     case G_LOG_LEVEL_CRITICAL:
//         fprintf(stdout, "%s-CRITICAL **: %s\n", domain, message);
//         break;
//     case G_LOG_LEVEL_ERROR:
//         fprintf(stdout, "%s-ERROR **: %s\n", domain, message);
//         break;
//     default:
//         fprintf(stderr, "unknown log level!\n");
//         break;
//     }
// }

int main(int argc, char *argv[])
{
    // g_log_set_default_handler(my_log_handler, nullptr);
    before_app_init();

    // run tests
    run_all_tests();

    auto app = Gtk::Application::create("com.example.dashboard", Gtk::Application::Flags::DEFAULT_FLAGS);
    app->signal_activate().connect([app]{
        // additional init
        after_app_init();

        // adw
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);

        // styles
        load_app_style();

        // register widgets
        register_widgets();

        // create views
        setup_root_view(app);
    });
    app->register_application();

    app->add_action("quit", [app](){ app->quit(); });

    return app->run(argc, argv);
}
