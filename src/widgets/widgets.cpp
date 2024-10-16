#include "widgets.hpp"
#include "utils/custom_section.hpp"

extern const AutoCallFunc __start_widgets;
extern const AutoCallFunc __stop_widgets;

void register_widgets()
{
    auto s = &__start_widgets;
    auto e = &__stop_widgets;
    call_all(s, e);
}
