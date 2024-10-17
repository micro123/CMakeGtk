#include "custom_section.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

void call_all(const FuncDecl *begin, const FuncDecl *end)
{
    assert(begin && end && "must not null");
    assert(end > begin && "invalid range");
    std::vector<FuncDecl> calls(begin, end);
    std::sort(std::begin(calls), std::end(calls), [](auto a, auto b){
        return a.priority < b.priority;
    });

    for (auto &x: calls) {
        x.entry_point();
    }
}
