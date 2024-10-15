#include "small_weather.hpp"

#include <glib-object.h>
#include <gtk/gtk.h>

#define SMALL_WEATHER_TYPE_BOX small_weather_get_type()
G_DECLARE_FINAL_TYPE(SmallWeather, small_weather, SMALL_WEATHER, BOX, GtkBox);

struct _SmallWeather {
    GtkBox parent;
};

G_DEFINE_TYPE(SmallWeather, small_weather, GTK_TYPE_BOX);

void register_small_weather()
{
    g_type_ensure(SMALL_WEATHER_TYPE_BOX);
}

void small_weather_init(SmallWeather* self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

void small_weather_class_init(SmallWeatherClass* klass)
{
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/dashboard/ui/small_weather.ui");
}
