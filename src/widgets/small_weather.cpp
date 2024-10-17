#include <glib-object.h>
#include <glib.h>
#include <glibconfig.h>
#include <gtk/gtk.h>

#include "utils/custom_section.hpp"

#define SMALL_WEATHER_TYPE_BOX small_weather_get_type()
G_DECLARE_FINAL_TYPE(SmallWeather, small_weather, SMALL_WEATHER, BOX, GtkBox);

enum SamllWeatherPropertyId: guint {
    ICON = 1,
    TEXT,
    TEMP,
    HUMI,
    TIME,

    CNT,
    PROP_MIN = ICON,
};

struct _SmallWeather {
    GtkBox   parent;
    // 数据
    GString *properties[CNT];
};

static GParamSpec *small_weather_properties[CNT];

G_DEFINE_TYPE(SmallWeather, small_weather, GTK_TYPE_BOX);

static void small_weather_init(SmallWeather* self)
{
    for (guint i = PROP_MIN; i<CNT; ++i) {
        self->properties[i] = g_string_new("");
    }
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void small_weather_dispose(GObject *obj) {
    auto *self = SMALL_WEATHER_BOX(obj);
    gtk_widget_dispose_template(GTK_WIDGET(self), SMALL_WEATHER_TYPE_BOX);
    for (guint i = PROP_MIN; i<CNT; ++i) {
        g_string_free(self->properties[i], true);
    }
    G_OBJECT_CLASS(small_weather_parent_class)->dispose(obj);
}

static void small_weather_set_property(GObject *obj, guint id, const GValue *value, GParamSpec *spec)
{
    auto self = SMALL_WEATHER_BOX(obj);
    g_string_assign(self->properties[id], g_value_get_string(value));
}

static void small_weather_get_property(GObject *obj, guint id, GValue *value, GParamSpec *spec)
{
    auto self = SMALL_WEATHER_BOX(obj);
    g_value_set_string(value, self->properties[id]->str);
}

static void small_weather_class_init(SmallWeatherClass* klass)
{
    auto gclass = G_OBJECT_CLASS(klass);

    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/dashboard/ui/small_weather.ui");

    // properties
    gclass->set_property = small_weather_set_property;
    gclass->get_property = small_weather_get_property;
    small_weather_properties[ICON] = g_param_spec_string("icon","icon","weather icon's resource path", "/dashboard/icons/999.png", G_PARAM_READWRITE);
    small_weather_properties[TEXT] = g_param_spec_string("text","text","weather describe", "多云", G_PARAM_READWRITE);
    small_weather_properties[TEMP] = g_param_spec_string("temp","temp","weather temperature", "16 ℃", G_PARAM_READWRITE);
    small_weather_properties[HUMI] = g_param_spec_string("humi","humi","weather humidity", "65 %", G_PARAM_READWRITE);
    small_weather_properties[TIME] = g_param_spec_string("time","time","weather forecast time", "00:00", G_PARAM_READWRITE);
    g_object_class_install_properties(gclass, G_N_ELEMENTS(small_weather_properties), small_weather_properties);

    // dispose
    gclass->dispose = small_weather_dispose;

    // css class
    gtk_widget_class_set_css_name(GTK_WIDGET_CLASS(klass), "small_weather");
}

WIDGET_AUTOREG(register_small_weather, 0)
{
    g_type_ensure(SMALL_WEATHER_TYPE_BOX);
}