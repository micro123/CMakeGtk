#include "info_row.hpp"

#include <glib-object.h>
#include <gtk/gtk.h>

#define INFO_ROW_TYPE_BOX info_row_get_type()
G_DECLARE_FINAL_TYPE(InfoRow, info_row, INFO_ROW, BOX, GtkBox);

struct _InfoRow
{
  GtkBox box;
  GtkLabel *label;
  GString  *label_str;
  GtkLabel *value;
  GString  *value_str;
};

enum InfoRowProperties {
  LABEL = 1,
  VALUE,

  CNT,
};

static GParamSpec *info_row_properties[CNT];

G_DEFINE_TYPE(InfoRow, info_row, GTK_TYPE_BOX);

static void info_row_init(InfoRow *self)
{
  self->label_str = g_string_new("LABEL");
  self->value_str = g_string_new("VALUE");
  gtk_widget_init_template(GTK_WIDGET(self));
}

static void info_row_dispose(GObject *gobj)
{
  InfoRow *self = INFO_ROW_BOX(gobj);
  gtk_widget_dispose_template(GTK_WIDGET(self), INFO_ROW_TYPE_BOX);
  g_string_free(self->label_str, true);
  g_string_free(self->value_str, true);
  G_OBJECT_CLASS(info_row_parent_class)->dispose(gobj);
}

static void info_row_set_property(GObject *obj, guint property_id, const GValue *value, GParamSpec *spec)
{
  InfoRow *self = INFO_ROW_BOX(obj);
  if (property_id == LABEL) {
    g_string_assign(self->label_str, g_value_get_string(value));
  } else if (property_id == VALUE) {
    g_string_assign(self->value_str, g_value_get_string(value));
  }
}
static void info_row_get_property(GObject *obj, guint property_id, GValue *value, GParamSpec *spec)
{
  InfoRow *self = INFO_ROW_BOX(obj);
  if (property_id == LABEL) {
    g_value_set_string(value, self->label_str->str);
  } else if (property_id == VALUE) {
    g_value_set_string(value, self->value_str->str);
  }
}

static void info_row_class_init(InfoRowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  GObjectClass   *gobj_class = G_OBJECT_CLASS(klass);

  // properties
  gobj_class->set_property = info_row_set_property;
  gobj_class->get_property = info_row_get_property;
  info_row_properties[LABEL] = g_param_spec_string("label", "label", "label of the row", "", G_PARAM_READWRITE);
  info_row_properties[VALUE] = g_param_spec_string("value", "value", "value of the row", "", G_PARAM_READWRITE);
  g_object_class_install_properties(gobj_class, G_N_ELEMENTS(info_row_properties), info_row_properties);

  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/dashboard/ui/info_row.ui");
  gtk_widget_class_bind_template_child_full(GTK_WIDGET_CLASS(klass), "label", 1,
                                            offsetof(InfoRow, label));
  gtk_widget_class_bind_template_child_full(GTK_WIDGET_CLASS(klass), "value", 1,
                                            offsetof(InfoRow, value));
}

void register_info_row() { g_type_ensure(INFO_ROW_TYPE_BOX); }
