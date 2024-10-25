#ifndef JSON_ADL_HPP
#define JSON_ADL_HPP

#include <nlohmann/json.hpp>
#include <optional>

namespace std{
    template <typename T>
    void from_json(const nlohmann::json &j, std::optional<T> &opt) {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            T t;
            from_json(j, t);
            opt = std::move(t);
        }
    }

    template <typename T>
    void to_json(nlohmann::json &j, const std::optional<T> &opt) {
        if (opt.has_value()) {
            to_json(j, *opt);
        } else {
            j = nlohmann::json(nullptr);
        }
    }
}

#endif /* JSON_ADL_HPP */
