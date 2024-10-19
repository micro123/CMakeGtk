#include <ctime>
#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "utils/custom_section.hpp"

#define DATETIME_TYPE_BOX datetime_get_type()
G_DECLARE_FINAL_TYPE(DateTime, datetime, DATETIME, BOX, GtkBox);

struct _DateTime
{
  GtkBox    box;
  GString  *date_str;
  GString  *time_str;
  guint     update_source;
};

enum InfoRowPropertyId: guint {
  DATE = 1,
  TIME,

  CNT,
};

static GParamSpec *datetime_properties[CNT];

G_DEFINE_TYPE(DateTime, datetime, GTK_TYPE_BOX);

static gboolean datetime_update(DateTime *self)
{
    static char time_spliters[] {' ', ':'};
    time_t now = time(NULL);
    auto tm = *localtime(&now);
    
    g_string_printf(self->date_str, "%02d月%02d日", tm.tm_mon + 1, tm.tm_mday);
    g_object_notify_by_pspec(G_OBJECT(self), datetime_properties[DATE]);

    char const spliter = time_spliters[tm.tm_sec % 2];
    g_string_printf(self->time_str, "%02d%c%02d", tm.tm_hour, spliter, tm.tm_min);
    g_object_notify_by_pspec(G_OBJECT(self), datetime_properties[TIME]);
    return TRUE;
}

static void datetime_init(DateTime *self)
{
  self->date_str = g_string_new("DATE");
  self->time_str = g_string_new("TIME");

  self->update_source = g_timeout_add(500, (GSourceFunc)datetime_update, self);

  gtk_widget_init_template(GTK_WIDGET(self));
}

static void datetime_dispose(GObject *gobj)
{
  DateTime *self = DATETIME_BOX(gobj);
  g_source_remove(self->update_source);
  gtk_widget_dispose_template(GTK_WIDGET(self), DATETIME_TYPE_BOX);
  g_string_free(self->date_str, true);
  g_string_free(self->time_str, true);
  G_OBJECT_CLASS(datetime_parent_class)->dispose(gobj);
}

static void datetime_get_property(GObject *obj, guint property_id, GValue *value, GParamSpec *spec)
{
  DateTime *self = DATETIME_BOX(obj);
  if (property_id == DATE) {
    g_value_set_string(value, self->date_str->str);
  } else if (property_id == TIME) {
    g_value_set_string(value, self->time_str->str);
  }
}

static void datetime_class_init(DateTimeClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *gobj_class = G_OBJECT_CLASS(klass);

  // properties
  gobj_class->get_property = datetime_get_property;
  datetime_properties[DATE] = g_param_spec_string("date", "date", "date part", "", G_PARAM_READABLE);
  datetime_properties[TIME] = g_param_spec_string("time", "time", "time part", "", G_PARAM_READABLE);
  g_object_class_install_properties(gobj_class, G_N_ELEMENTS(datetime_properties), datetime_properties);

  // child
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/dashboard/ui/datetime.ui");
                                            
  // dispose
  gobj_class->dispose = datetime_dispose;

  // css name
  gtk_widget_class_set_css_name(widget_class, "info_raw");
}

WIDGET_AUTOREG(register_info_row, 0)
{
    g_type_ensure(DATETIME_TYPE_BOX);
}
