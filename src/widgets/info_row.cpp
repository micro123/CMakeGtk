#include "info_row.hpp"

#include <glib-object.h>
#include <gtk/gtk.h>

#define INFO_ROW_TYPE_BOX info_row_get_type()
G_DECLARE_FINAL_TYPE(InfoRow, info_row, INFO_ROW, BOX, GtkBox);

struct _InfoRow {
  GtkBox box;
  GtkLabel *label;
  GtkLabel *value;
};

G_DEFINE_TYPE(InfoRow, info_row, GTK_TYPE_BOX);

static void info_row_init(InfoRow *self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

static void info_row_dispose(GObject *gobj) {
  InfoRow *self = INFO_ROW_BOX(gobj);
  gtk_widget_dispose_template(GTK_WIDGET(self), INFO_ROW_TYPE_BOX);
  G_OBJECT_CLASS(info_row_parent_class)->dispose(gobj);
}

static void info_row_class_init(InfoRowClass *klass) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
  gtk_widget_class_set_template_from_resource(widget_class,
                                              "/dashboard/ui/info_row.ui");
  gtk_widget_class_bind_template_child_full(GTK_WIDGET_CLASS(klass), "label", 1,
                                            offsetof(InfoRow, label));
  gtk_widget_class_bind_template_child_full(GTK_WIDGET_CLASS(klass), "value", 1,
                                            offsetof(InfoRow, value));
}

void register_info_row() { g_type_ensure(INFO_ROW_TYPE_BOX); }
