#define TEMPLATE_IMPL
#include "gobj_prop.hpp"

#define IMPL(type) \
template<> struct internal::GValueCast<type> {  \
    using rt = type;                            \
    static type cast_from(const GValue &value); \
};                                              \
type internal::GValueCast<type>::cast_from(const GValue &value) 

IMPL(std::string) {
    rt r{};
    auto cstr = g_value_get_string(&value);
    if (cstr)
        r.assign(cstr);
    return r;
}

IMPL(gint) {
    rt r = g_value_get_int(&value);
    return r;
}

IMPL(guint) {
    rt r = g_value_get_uint(&value);
    return r;
}

IMPL(gint64) {
    rt r = g_value_get_int64(&value);
    return r;
}

IMPL(guint64) {
    rt r = g_value_get_uint64(&value);
    return r;
}

IMPL(float) {
    rt r = g_value_get_float(&value);
    return r;
}

IMPL(double) {
    rt r = g_value_get_double(&value);
    return r;
}

IMPL(GObject*) {
    rt r = G_OBJECT(g_value_get_object(&value));
    return r;
}

IMPL(bool) {
    rt r = g_value_get_boolean(&value);
    return r;
}

#undef IMPL

struct AutoGValue {
    ~AutoGValue() {
        g_value_unset(&v);
    }
    operator GValue*() { return &v; }
    GValue v G_VALUE_INIT;
};

void set_prop(GObject *obj, const char *pname, const std::string &pvalue)
{
    AutoGValue val;
    g_value_set_string(val, pvalue.c_str());
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, bool pvalue)
{
    AutoGValue val;
    g_value_set_boolean(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, gint pvalue)
{
    AutoGValue val;
    g_value_set_int(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, guint pvalue)
{
    AutoGValue val;
    g_value_set_uint(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, double pvalue)
{
    AutoGValue val;
    g_value_set_double(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, float pvalue)
{
    AutoGValue val;
    g_value_set_float(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, GObject *pvalue)
{
    AutoGValue val;
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
    AutoGValue val;
    g_value_set_int64(val, pvalue);
    g_object_set_property(obj, pname, val);
}

void set_prop(GObject *obj, const char *pname, guint64 pvalue)
{
    AutoGValue val;
    g_value_set_uint64(val, pvalue);
    g_object_set_property(obj, pname, val);
}


