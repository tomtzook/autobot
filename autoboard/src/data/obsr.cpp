
#include "obsr.h"

#include <ranges>

namespace data {

static std::map<std::string, scheme::type, std::less<>> create_str_to_scheme_map() {
    std::map<std::string, scheme::type, std::less<>> map;
    map.emplace("canvas_line", scheme::type::canvas_line);
    map.emplace("canvas_rect", scheme::type::canvas_rect);
    map.emplace("canvas_circle", scheme::type::canvas_circle);
    map.emplace("canvas_group", scheme::type::canvas_group);
    map.emplace("d3_body", scheme::type::d3_body);
    map.emplace("d3_box", scheme::type::d3_box);

    return map;
}

static scheme::type get_scheme_from_value(const obsr::value& value) {
    switch (value.get_type()) {
        case obsr::value_type::string:
            return scheme::type::string;
        case obsr::value_type::boolean:
            return scheme::type::boolean;
        case obsr::value_type::integer32:
        case obsr::value_type::integer64:
        case obsr::value_type::floating_point32:
        case obsr::value_type::floating_point64:
            return scheme::type::number;
        case obsr::value_type::integer32_array:
        case obsr::value_type::integer64_array:
        case obsr::value_type::floating_point32_array:
        case obsr::value_type::floating_point64_array:
            return scheme::type::array;
        case obsr::value_type::empty:
        case obsr::value_type::raw:
        default:
            return scheme::type::unknown;
    }
}

static scheme::type get_scheme_from_string(const std::string_view type) {
    static auto map = create_str_to_scheme_map();
    if (const auto it = map.find(type); it != map.end()) {
        return it->second;
    }

    return scheme::type::unknown;
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
    , m_has_new_data(true)
    , m_value(obsr::value::make())
    , m_scheme(scheme::type::unknown)
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

bool obsr_entry::has_new_data() const {
    return m_has_new_data;
}

bool obsr_entry::has_new_data_self() const {
    return m_has_new_data;
}

bool obsr_entry::has_children() const {
    return false;
}

const obsr::value& obsr_entry::get_value() const {
    return m_value;
}

scheme::type obsr_entry::get_scheme() const {
    return m_scheme;
}

template<>
scheme::number obsr_entry::read<scheme::number>() {
    assert(m_scheme == scheme::type::number);
    m_has_new_data = false;
    return {get_as_number(m_value)};
}

void obsr_entry::update(const obsr::value& value) {
    m_value = value;
    m_scheme = get_scheme_from_value(m_value);
    m_has_new_data = true;
}

obsr_object::obsr_object(const obsr::object handle)
    : m_handle(handle)
    , m_id(0)
    , m_name(obsr::get_path_for_object(handle))
    , m_has_new_data(true)
    , m_scheme(scheme::type::unknown)
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

bool obsr_object::has_new_data() const {
    if (m_has_new_data) {
        return true;
    }

    for (const auto& child: m_children | std::views::values) {
        if (child->has_new_data()) {
            return true;
        }
    }

    for (const auto& entry: m_entries | std::views::values) {
        if (entry->has_new_data()) {
            return true;
        }
    }

    return false;
}

bool obsr_object::has_new_data_self() const {
    return m_has_new_data;
}

bool obsr_object::has_children() const {
    return true;
}

scheme::type obsr_object::get_scheme() const {
    return m_scheme;
}

template<>
scheme::canvas_line obsr_object::read<scheme::canvas_line>() {
    assert(m_scheme == scheme::type::canvas_line);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto length = get_entry_value<float>("length", 0);
    const auto angle = get_entry_value<float>("angle", 0);
    const auto color = static_cast<uint32_t>(get_entry_value<int32_t>("color", 0));

    return {x, y, length, angle, color};
}

template<>
scheme::canvas_rect obsr_object::read<scheme::canvas_rect>() {
    assert(m_scheme == scheme::type::canvas_rect);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto length = get_entry_value<float>("length", 0);
    const auto width = get_entry_value<float>("width", 0);
    const auto color = static_cast<uint32_t>(get_entry_value<int32_t>("color", 0));

    return {x, y, length, width, color};
}

template<>
scheme::canvas_circle obsr_object::read<scheme::canvas_circle>() {
    assert(m_scheme == scheme::type::canvas_circle);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto radius = get_entry_value<float>("radius", 0);
    const auto color = static_cast<uint32_t>(get_entry_value<int32_t>("color", 0));

    return {x, y, radius, color};
}

template<>
scheme::canvas_group obsr_object::read<scheme::canvas_group>() {
    assert(m_scheme == scheme::type::canvas_group);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);

    return {x, y};
}

template<>
scheme::d3_body obsr_object::read<scheme::d3_body>() {
    assert(m_scheme == scheme::type::d3_body);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto z = get_entry_value<float>("z", 0);
    const auto pitch = get_entry_value<float>("pitch", 0);
    const auto yaw = get_entry_value<float>("roll", 0);
    const auto roll = get_entry_value<float>("yaw", 0);

    return {x, y, z, pitch, yaw, roll};
}

template<>
scheme::d3_box obsr_object::read<scheme::d3_box>() {
    assert(m_scheme == scheme::type::d3_box);
    m_has_new_data = false;

    const auto x = get_entry_value<float>("x", 0);
    const auto y = get_entry_value<float>("y", 0);
    const auto z = get_entry_value<float>("z", 0);
    const auto pitch = get_entry_value<float>("pitch", 0);
    const auto yaw = get_entry_value<float>("roll", 0);
    const auto roll = get_entry_value<float>("yaw", 0);
    const auto length = get_entry_value<float>("length", 0);
    const auto width = get_entry_value<float>("width", 0);
    const auto height = get_entry_value<float>("height", 0);
    const auto color = static_cast<uint32_t>(get_entry_value<int32_t>("color", 0));

    return {x, y, z, pitch, yaw, roll, length, width, height, color};
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
    m_has_new_data = true;
}

void obsr_object::delete_object(const std::string_view name) {
    if (const auto it = m_children.find(name); it != m_children.end()) {
        m_children.erase(it);
    }

    m_has_new_data = true;
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
    m_has_new_data = true;
}

void obsr_object::delete_entry(const std::string_view name) {
    if (const auto it = m_entries.find(name); it != m_entries.end()) {
        m_entries.erase(it);
    }

    m_has_new_data = true;
}

void obsr_object::update() {
    m_scheme = read_type();
    m_has_new_data = true;
}

scheme::type obsr_object::read_type() {
    if (const auto it = m_entries.find(".type"); it != m_entries.end()) {
        const auto& value = it->second->get_value();
        if (value.get_type() != obsr::value_type::string) {
            return scheme::type::unknown;
        }

        return get_scheme_from_string(value.get_string());
    }

    return scheme::type::unknown;
}

}
