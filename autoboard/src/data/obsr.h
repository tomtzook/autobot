#pragma once

#include <memory>
#include <map>
#include <obsr.h>

#include "scheme.h"

namespace data {

class obsr_entry;
using obsr_entry_ptr = std::shared_ptr<obsr_entry>;

class obsr_object;
using obsr_object_ptr = std::shared_ptr<obsr_object>;

class obsr_entry {
public:
    explicit obsr_entry(obsr::entry handle);

    obsr_entry(const obsr_entry&) = delete;
    obsr_entry(obsr_entry&&) = delete;
    obsr_entry& operator=(const obsr_entry&) = delete;
    obsr_entry& operator=(obsr_entry&&) = delete;

    [[nodiscard]] uint64_t get_id() const;
    void set_id(uint64_t id);

    [[nodiscard]] obsr::entry get_handle() const;
    [[nodiscard]] std::string_view get_name() const;
    [[nodiscard]] const obsr::value& get_value() const;
    [[nodiscard]] scheme get_scheme() const;

    template<typename t_>
    t_ read() {assert(false);}
    template<>
    [[nodiscard]] number_scheme read();

    void update(const obsr::value& value);

private:
    obsr::entry m_handle;
    uint64_t m_id;
    std::string m_name;
    obsr::value m_value;
    scheme m_scheme;
};

class obsr_object {
public:
    explicit obsr_object(obsr::object handle);

    obsr_object(const obsr_object&) = delete;
    obsr_object(obsr_object&&) = delete;
    obsr_object& operator=(const obsr_object&) = delete;
    obsr_object& operator=(obsr_object&&) = delete;

    [[nodiscard]] uint64_t get_id() const;
    void set_id(uint64_t id);

    [[nodiscard]] obsr::object get_handle() const;
    [[nodiscard]] std::string_view get_name() const;
    [[nodiscard]] scheme get_scheme() const;

    template<typename t_>
    t_ read() {assert(false);}
    template<>
    [[nodiscard]] canvas_line_scheme read();

    [[nodiscard]] std::optional<obsr_object*> get_child(std::string_view name);
    void foreach_child(std::function<void(const obsr_object*)>&& callback) const;
    void add_child(std::string_view name, obsr_object* ptr);
    void delete_object(std::string_view name);

    [[nodiscard]] std::optional<obsr_entry*> get_entry(std::string_view name);
    void foreach_entry(std::function<void(const obsr_entry*)>&& callback) const;
    void add_entry(std::string_view name, obsr_entry* ptr);
    void delete_entry(std::string_view name);

    void update();

private:
    scheme read_type();

    template<typename t_>
    t_ get_entry_value(std::string_view name, const t_& default_value);

    obsr::handle m_handle;
    uint64_t m_id;
    std::string m_name;
    scheme m_scheme;
    std::map<std::string, obsr_object*, std::less<>> m_children;
    std::map<std::string, obsr_entry*, std::less<>> m_entries;

};

template<typename t_>
t_ obsr_object::get_entry_value(const std::string_view name, const t_& default_value) {
    const auto opt = get_entry(name);
    if (!opt) {
        return default_value;
    }

    const auto value = opt.value()->get_value();
    if constexpr (std::is_same_v<t_, bool>) {
        return value.get_type() == obsr::value_type::boolean ? value.get_boolean() : default_value;
    } else if constexpr (std::is_same_v<t_, int32_t>) {
        return value.get_type() == obsr::value_type::integer32 ? value.get_int32() : default_value;
    } else if constexpr (std::is_same_v<t_, int64_t>) {
        return value.get_type() == obsr::value_type::integer64 ? value.get_int64() : default_value;
    } else if constexpr (std::is_same_v<t_, float>) {
        return value.get_type() == obsr::value_type::floating_point32 ? value.get_float() : default_value;
    } else if constexpr (std::is_same_v<t_, double>) {
        return value.get_type() == obsr::value_type::floating_point64 ? value.get_double() : default_value;
    } else {
        static_assert(false, "unsupported type");
    }

    return default_value;
}

}
