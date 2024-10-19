#ifndef SOME_WIDGET_HPP
#define SOME_WIDGET_HPP

#include <glibmm/property.h>
#include <gtkmm/builder.h>

#include <gtkmm/widget.h>
#include <gtkmm/snapshot.h>

class MyWidget : public Gtk::Widget {
public:
    MyWidget();

    MyWidget(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);

    ~MyWidget();

    Glib::PropertyProxy<int> property_number();

    Glib::PropertyProxy<Glib::ustring> property_my_name();

protected:
    Gtk::SizeRequestMode get_request_mode_vfunc() const override;

    void measure_vfunc(Gtk::Orientation orientation, int for_size, int &minimum, int &natural, int &minimum_baseline,
                       int &natural_baseline) const override;

    void on_map();

    void on_unmap();

    void on_realize();

    void on_unrealize();

    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot> &snapshot);

private:
    Glib::Property<int> number{*this, "number"};
    Glib::Property<Glib::ustring> my_name{*this, "my-name"};

    Gtk::Border m_padding;
    Gdk::RGBA m_foreground{0.0, 0.0, 1.0};
    Gdk::RGBA m_background{1.0, 0.0, 0.0};
};


#endif // SOME_WIDGET_HPP
