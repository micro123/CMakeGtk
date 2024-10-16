#ifndef CUSTOM_SECTION_HPP
#define CUSTOM_SECTION_HPP

struct __attribute__((aligned(sizeof(void*)),packed)) AutoCallFunc {
    void (*entry_point)();
    int  priority;
};

void call_all(const AutoCallFunc *begin, const AutoCallFunc *end);

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)
#define VARNAME(prefix) CONCAT(prefix##_, CONCAT(__LINE__, __COUNTER__))

#define WIDGET_SECTION __attribute__((section("widgets"),used,aligned(sizeof(AutoCallFunc))))
#define WIDGET_AUTOREG(fn, p) \
static void fn(); \
static WIDGET_SECTION const AutoCallFunc VARNAME(widget) { \
    .entry_point = fn, \
    .priority = p, \
}; \
void fn()

#endif /* CUSTOM_SECTION_HPP */
