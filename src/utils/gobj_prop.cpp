#include <glib-object.h>
#define TEMPLATE_IMPL
#include "gobj_prop.hpp"

#define IMPL(type,gtype) \
template<> struct internal::GValueCast<type> {    \
    using rt = type;                              \
    static type  cast_from(const GValue &value);  \
    static GType value_type() { return gtype; }   \
};                                                \
type internal::GValueCast<type>::cast_from(const GValue &value) 

IMPL(std::string, G_TYPE_STRING) {
    rt r{};
    auto cstr = g_value_get_string(&value);
    if (cstr)
        r.assign(cstr);
    return r;
}

IMPL(gint, G_TYPE_INT) {
    rt r = g_value_get_int(&value);
    return r;
}

IMPL(guint, G_TYPE_UINT) {
    rt r = g_value_get_uint(&value);
    return r;
}

IMPL(gint64, G_TYPE_INT64) {
    rt r = g_value_get_int64(&value);
    return r;
}

IMPL(guint64, G_TYPE_UINT64) {
    rt r = g_value_get_uint64(&value);
    return r;
}

IMPL(float, G_TYPE_FLOAT) {
    rt r = g_value_get_float(&value);
    return r;
}

IMPL(double, G_TYPE_DOUBLE) {
    rt r = g_value_get_double(&value);
    return r;
}

IMPL(GObject*, G_TYPE_OBJECT) {
    rt r = G_OBJECT(g_value_get_object(&value));
    return r;
}

IMPL(bool, G_TYPE_BOOLEAN) {
    rt r = g_value_get_boolean(&value);
    return r;
}

#undef IMPL

struct AutoGValue {
    template <typename T>
    AutoGValue(const T&) {
        g_value_init(&v, internal::GValueCast<T>::value_type());
    }
    ~AutoGValue() {
        g_value_unset(&v);
    }
    operator GValue*() { return &v; }
    GValue v G_VALUE_INIT;
};

void set_prop(GObject *obj, const char *pname, const std::string &pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_string(val, pvalue.c_str());
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, bool pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_boolean(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, gint pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_int(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, guint pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_uint(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, double pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_double(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, float pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_float(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, GObject *pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_object(val, pvalue);
    g_object_set_property(obj, pname, val);
}

GValue get_prop(GObject *obj, const char *pname)
{
    GValue pvalue G_VALUE_INIT;
    g_object_get_property(obj, pname, &pvalue);
    return pvalue;
}

void set_prop(GObject *obj, const char *pname, gint64 pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_int64(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, guint64 pvalue)
{
    AutoGValue val(pvalue);
    g_value_set_uint64(val, pvalue);
    g_object_set_property(obj, pname, val);
}


