#include "init_calls.hpp"
#include "utils/custom_section.hpp"

void before_app_init()
{
    call_all(&__start_before_app, &__stop_before_app);
}

void after_app_init()
{
    call_all(&__start_after_app, &__stop_after_app);
}

EMPTY_SECTION(before_app);
EMPTY_SECTION(after_app);
