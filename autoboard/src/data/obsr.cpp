
#include "obsr.h"

#include <ranges>

namespace data {

static std::map<std::string, scheme, std::less<>> create_str_to_scheme_map() {
    std::map<std::string, scheme, std::less<>> map;
    map.emplace("canvas_line", scheme::canvas_line);

    return map;
}

static scheme get_scheme_from_value(const obsr::value& value) {
    switch (value.get_type()) {
        case obsr::value_type::string:
            return scheme::string;
        case obsr::value_type::boolean:
            return scheme::boolean;
        case obsr::value_type::integer32:
        case obsr::value_type::integer64:
        case obsr::value_type::floating_point32:
        case obsr::value_type::floating_point64:
            return scheme::number;
        case obsr::value_type::integer32_array:
        case obsr::value_type::integer64_array:
        case obsr::value_type::floating_point32_array:
        case obsr::value_type::floating_point64_array:
            return scheme::array;
        case obsr::value_type::empty:
        case obsr::value_type::raw:
        default:
            return scheme::unknown;
    }
}

static scheme get_scheme_from_string(const std::string_view type) {
    static auto map = create_str_to_scheme_map();
    if (const auto it = map.find(type); it != map.end()) {
        return it->second;
    }

    return scheme::unknown;
}

static float get_as_number(const obsr::value& value) {
    switch (value.get_type()) {
        case obsr::value_type::boolean: return value.get_boolean();
        case obsr::value_type::integer32: return static_cast<float>(value.get_int32());
        case obsr::value_type::integer64: return static_cast<float>(value.get_int64());
        case obsr::value_type::floating_point32: return value.get_float();
        case obsr::value_type::floating_point64: return static_cast<float>(value.get_double());
        default: throw std::logic_error("type is not number");
    }
}

obsr_entry::obsr_entry(const obsr::entry handle)
    : m_handle(handle)
    , m_id(0)
    , m_name(obsr::get_path_for_entry(handle))
    , m_value(obsr::value::make())
    , m_scheme(scheme::unknown)
{}

uint64_t obsr_entry::get_id() const {
    return m_id;
}

void obsr_entry::set_id(const uint64_t id) {
    m_id = id;
}

obsr::entry obsr_entry::get_handle() const {
    return m_handle;
}

std::string_view obsr_entry::get_name() const {
    return m_name;
}

const obsr::value& obsr_entry::get_value() const {
    return m_value;
}

scheme obsr_entry::get_scheme() const {
    return m_scheme;
}

template<>
number_scheme obsr_entry::read<number_scheme>() {
    assert(m_scheme == scheme::number);
    return {get_as_number(m_value)};
}

void obsr_entry::update(const obsr::value& value) {
    m_value = value;
    m_scheme = get_scheme_from_value(m_value);
}

obsr_object::obsr_object(const obsr::object handle)
    : m_handle(handle)
    , m_id(0)
    , m_name(obsr::get_path_for_object(handle))
    , m_scheme(scheme::unknown)
    , m_children()
    , m_entries()
{}

uint64_t obsr_object::get_id() const {
    return m_id;
}

void obsr_object::set_id(const uint64_t id) {
    m_id = id;
}

obsr::object obsr_object::get_handle() const {
    return m_handle;
}

std::string_view obsr_object::get_name() const {
    return m_name;
}

scheme obsr_object::get_scheme() const {
    return m_scheme;
}

template<>
canvas_line_scheme obsr_object::read<canvas_line_scheme>() {
    assert(m_scheme == scheme::canvas_line);

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto length = get_entry_value<float>("length", 0);
    const auto angle = get_entry_value<float>("angle", 0);

    return {x, y, length, angle};
}

std::optional<obsr_object*> obsr_object::get_child(const std::string_view name) {
    if (const auto it = m_children.find(name); it != m_children.end()) {
        return it->second;
    }

    return std::nullopt;
}

void obsr_object::foreach_child(std::function<void(const obsr_object*)>&& callback) const {
    for (const auto& child: m_children | std::views::values) {
        callback(child);
    }
}

void obsr_object::add_child(const std::string_view name, obsr_object* ptr) {
    m_children.emplace(name, ptr);
}

void obsr_object::delete_object(const std::string_view name) {
    if (const auto it = m_children.find(name); it != m_children.end()) {
        m_children.erase(it);
    }
}

std::optional<obsr_entry*> obsr_object::get_entry(const std::string_view name) {
    if (const auto it = m_entries.find(name); it != m_entries.end()) {
        return it->second;
    }

    return std::nullopt;
}

void obsr_object::foreach_entry(std::function<void(const obsr_entry*)>&& callback) const {
    for (const auto& entry: m_entries | std::views::values) {
        callback(entry);
    }
}

void obsr_object::add_entry(const std::string_view name, obsr_entry* ptr) {
    m_entries.emplace(name, ptr);
}

void obsr_object::delete_entry(const std::string_view name) {
    if (const auto it = m_entries.find(name); it != m_entries.end()) {
        m_entries.erase(it);
    }
}

void obsr_object::update() {
    m_scheme = read_type();
}

scheme obsr_object::read_type() {
    if (const auto it = m_entries.find(".type"); it != m_entries.end()) {
        const auto& value = it->second->get_value();
        if (value.get_type() != obsr::value_type::string) {
            return scheme::unknown;
        }

        return get_scheme_from_string(value.get_string());
    }

    return scheme::unknown;
}

}
