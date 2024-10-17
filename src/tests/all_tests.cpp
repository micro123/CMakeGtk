#include "all_tests.hpp"
#include "utils/custom_section.hpp"
#include <cstdlib>
#include <cstring>

void run_all_tests()
{
    auto env = std::getenv("RUN_TESTS");
    if (env == nullptr || strcmp("1", env) != 0)
        return;
    call_all(&__start_test_func, &__stop_test_func);
}
