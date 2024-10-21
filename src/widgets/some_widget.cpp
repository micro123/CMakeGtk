#include "some_widget.hpp"
#include <gtkmm.h>

#include <widgets/kplot/kplot.h>

static kpair  wave_points[1024];
static kdata *series;
static kplot *plot;
static kplotcfg plot_cfg;

MyWidget::MyWidget(): Glib::ObjectBase("MyWidget"), Gtk::Widget() {
    set_hexpand();
    set_vexpand();
    set_hexpand_set();
    set_vexpand_set();

    property_number().signal_changed().connect([] {
        g_log("MyWidget", G_LOG_LEVEL_WARNING, "number changed");
    });
    property_my_name().signal_changed().connect([] {
        g_log("MyWidget", G_LOG_LEVEL_WARNING, "my-name changed");
    });
}

MyWidget::MyWidget(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder): Glib::ObjectBase("MyWidget"),
    Gtk::Widget(cobject) {
    set_hexpand();
    set_vexpand();
    set_hexpand_set();
    set_vexpand_set();

    property_number().signal_changed().connect([] {
        g_log("MyWidget", G_LOG_LEVEL_WARNING, "number changed");
    });
    property_my_name().signal_changed().connect([] {
        g_log("MyWidget", G_LOG_LEVEL_WARNING, "my-name changed");
    });
}

MyWidget::~MyWidget() {
}

Glib::PropertyProxy<int> MyWidget::property_number() { return number.get_proxy(); }

Glib::PropertyProxy<Glib::ustring> MyWidget::property_my_name() { return my_name.get_proxy(); }

Gtk::SizeRequestMode MyWidget::get_request_mode_vfunc() const {
    return Gtk::SizeRequestMode::HEIGHT_FOR_WIDTH;
}

void MyWidget::measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural,
    int &minimum_baseline, int &natural_baseline) const {
    if (orientation == Gtk::Orientation::HORIZONTAL) {
        minimum = 60;
        natural = 100;
    } else {
        minimum = 50;
        natural = 70;
    }

    // Don't use baseline alignment.
    minimum_baseline = -1;
    natural_baseline = -1;
}

void MyWidget::on_map() {
    //Call base class:
    Gtk::Widget::on_map();
}

void MyWidget::on_unmap() {
    //Call base class:
    Gtk::Widget::on_unmap();
}

void MyWidget::on_realize() {
    //Call base class:
    Gtk::Widget::on_realize();
}

void MyWidget::on_unrealize() {
    //Call base class:
    Gtk::Widget::on_unrealize();
}

void MyWidget::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot> &snapshot) {
    const auto allocation = get_allocation();
    const Gdk::Rectangle rect(0, 0, allocation.get_width(), allocation.get_height());

    // Create a cairo context to draw on.
    auto cr = snapshot->append_cairo(rect);

    kplot_draw(plot, rect.get_width(), rect.get_height(), cr->cobj());

    // g_warning(__func__);

    g_idle_add((GSourceFunc)gtk_widget_queue_draw, gobj());
}

#include "utils/custom_section.hpp"
WIDGET_AUTOREG(register_my_widget, 10) {
    static_cast<void>(MyWidget{});


    for (int i=0; i<std::size(wave_points); ++i) {
        wave_points[i].x = i;
        wave_points[i].y = sin(0.1*i);
    }

    series = kdata_array_alloc(wave_points, std::size(wave_points));
    kplotcfg_defaults(&plot_cfg);
    plot_cfg.margin = MARGIN_ALL;
    plot_cfg.marginsz = 4;
    plot_cfg.xaxislabel = "f(x) = sin(0.1*x)";
    plot_cfg.extrema = EXTREMA_YMIN | EXTREMA_YMAX;
    plot_cfg.extrema_ymin = -1.2;
    plot_cfg.extrema_ymax =  1.2;
    plot = kplot_alloc(&plot_cfg);
    kplot_attach_data(plot, series, KPLOT_LINES, nullptr);
}
