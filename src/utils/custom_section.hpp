#ifndef CUSTOM_SECTION_HPP
#define CUSTOM_SECTION_HPP

struct __attribute__((aligned(sizeof(void*)),packed)) FuncDecl {
    void (*entry_point)();
    int  priority;
};
void call_all(const FuncDecl *begin, const FuncDecl *end);

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)
#define VARNAME(prefix) CONCAT(prefix##_, CONCAT(__LINE__, __COUNTER__))

#define SECTION_VARS(name) \
extern const FuncDecl __start_##name; \
extern const FuncDecl __stop_##name

#define REG_TO_SECTION(sec_name, fn, p) \
static void fn(); \
static __attribute__((section(#sec_name),used)) const FuncDecl VARNAME(sec_name) { \
    .entry_point = fn, .priority = p \
}; \
void fn()

// #define WIDGET_SECTION __attribute__((section("widgets"),used))
// #define WIDGET_AUTOREG(fn, p) \
// static void fn(); \
// static WIDGET_SECTION const FuncDecl VARNAME(widget) { \
//     .entry_point = fn, \
//     .priority = p, \
// }; \
// void fn()
// SECTION_VARS(widgets);

#define WIDGET_AUTOREG(fn, p) REG_TO_SECTION(my_widgets, fn, p)
SECTION_VARS(my_widgets);

#define TEST_AUTOREG(fn) REG_TO_SECTION(test_func, fn, 0)
SECTION_VARS(test_func);

#endif /* CUSTOM_SECTION_HPP */
