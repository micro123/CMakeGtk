#include "some_widget.hpp"
#include <gtkmm.h>


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

    // paint the background
    Gdk::Cairo::set_source_rgba(cr, m_background);
    Gdk::Cairo::add_rectangle_to_path(cr, rect);
    cr->fill();

    // draw the foreground
    const double scale_x = 0.001 * (allocation.get_width() - m_padding.get_left() - m_padding.get_right());
    const double scale_y = 0.001 * (allocation.get_height() - m_padding.get_top() - m_padding.get_bottom());
    Gdk::Cairo::set_source_rgba(cr, m_foreground);
    cr->translate(m_padding.get_left(), m_padding.get_top());
    cr->rectangle(0.0, 0.0, 1000.0 * scale_x, 1000.0 * scale_y);
    cr->move_to(155. * scale_x, 165. * scale_y);
    cr->line_to(155. * scale_x, 838. * scale_y);
    cr->line_to(265. * scale_x, 900. * scale_y);
    cr->line_to(849. * scale_x, 564. * scale_y);
    cr->line_to(849. * scale_x, 438. * scale_y);
    cr->line_to(265. * scale_x, 100. * scale_y);
    cr->line_to(155. * scale_x, 165. * scale_y);
    cr->move_to(265. * scale_x, 100. * scale_y);
    cr->line_to(265. * scale_x, 652. * scale_y);
    cr->line_to(526. * scale_x, 502. * scale_y);
    cr->move_to(369. * scale_x, 411. * scale_y);
    cr->line_to(633. * scale_x, 564. * scale_y);
    cr->move_to(369. * scale_x, 286. * scale_y);
    cr->line_to(369. * scale_x, 592. * scale_y);
    cr->move_to(369. * scale_x, 286. * scale_y);
    cr->line_to(849. * scale_x, 564. * scale_y);
    cr->move_to(633. * scale_x, 564. * scale_y);
    cr->line_to(155. * scale_x, 838. * scale_y);
    cr->stroke();

    queue_draw();
}

#include "utils/custom_section.hpp"
WIDGET_AUTOREG(register_my_widget, 10) {
    static_cast<void>(MyWidget{});
}
