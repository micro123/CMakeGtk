#include "custom_section.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

void call_all(const AutoCallFunc *begin, const AutoCallFunc *end)
{
    assert(begin && end && "must not null");
    assert(end > begin && "invalid range");
    std::vector<AutoCallFunc> calls(begin, end);
    std::sort(std::begin(calls), std::end(calls), [](auto a, auto b){
        return a.priority < b.priority;
    });

    for (auto &x: calls) {
        x.entry_point();
    }
}
