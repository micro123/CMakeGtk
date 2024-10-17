#ifndef GOBJ_PROP_HPP
#define GOBJ_PROP_HPP

#include <glib-object.h>
#include <string>
#include <stdexcept>

namespace internal {
    template <typename T>
    struct GValueCast{ 
        static T     cast_from(const GValue &value); // No Implementation Defined
        static GType value_type();
    };

#ifndef TEMPLATE_IMPL
    extern template struct GValueCast<std::string>;
    extern template struct GValueCast<bool>;
    extern template struct GValueCast<gint>;
    extern template struct GValueCast<guint>;
    extern template struct GValueCast<gint64>;
    extern template struct GValueCast<guint64>;
    extern template struct GValueCast<float>;
    extern template struct GValueCast<double>;
    extern template struct GValueCast<GObject*>;
#endif
}

void set_prop(GObject *obj, const char *pname, bool pvalue);
void set_prop(GObject *obj, const char *pname, const std::string &pvalue);
void set_prop(GObject *obj, const char *pname, gint pvalue);
void set_prop(GObject *obj, const char *pname, guint pvalue);
void set_prop(GObject *obj, const char *pname, gint64 pvalue);
void set_prop(GObject *obj, const char *pname, guint64 pvalue);
void set_prop(GObject *obj, const char *pname, double pvalue);
void set_prop(GObject *obj, const char *pname, float pvalue);
void set_prop(GObject *obj, const char *pname, GObject *pvalue);

GValue get_prop(GObject *obj, const char *pname);

template <typename T, typename P>
inline void set_prop(T *t, const char *pname, const P &pvalue) {
    set_prop(G_OBJECT(t), pname, pvalue);
}

template <typename R, typename T>
inline R get_prop(T *t, const char *pname) {
    auto pvalue = get_prop(G_OBJECT(t), pname);
    R ret = internal::GValueCast<R>::cast_from(pvalue);
    g_value_unset(&pvalue);
    return ret;
}

#endif /* GOBJ_PROP_HPP */
