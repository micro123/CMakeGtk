#include "widgets.hpp"
#include "utils/custom_section.hpp"

#include <gtkmm/wrap_init.h>

void register_widgets()
{
    auto s = &__start_my_widgets;
    auto e = &__stop_my_widgets;
    call_all(s, e);
}
