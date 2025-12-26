
#include "dashboard/object.h"

#include <ranges>


namespace autobot::dashboard {

entry::entry()
    : m_object(obsr::empty_handle)
    , m_nodes()
{}

entry::entry(const obsr::object object)
    : m_object(object)
    , m_nodes()
{}

bool entry::operator==(const entry& other) const {
    return m_object == other.m_object;
}

void entry::set(const std::string_view name, const obsr::value& value) {
    if (const auto it = m_nodes.find(name); it != m_nodes.end()) {
        throw std::runtime_error("name already taken");
    }

    const auto entry = obsr::get_entry(m_object, name);
    obsr::set_value(entry, value);

    node node;
    m_nodes.emplace(name, std::move(node));
}

void entry::add(const std::string_view name, getter_func&& getter) {
    if (const auto it = m_nodes.find(name); it != m_nodes.end()) {
        throw std::runtime_error("name already taken");
    }

    const auto entry = obsr::get_entry(m_object, name);
    node node(entry, std::move(getter));
    m_nodes.emplace(name, std::move(node));
}

void entry::refresh() {
    for (auto& node: m_nodes | std::views::values) {
        if (node.entry == obsr::empty_handle) {
            continue;
        }

        if (auto opt_value = node.getter(node.last_value)) {
            auto& new_value = opt_value.value();
            obsr::set_value(node.entry, new_value);
            node.last_value = new_value;
        }
    }
}

void entry::delete_object() {
    m_nodes.clear();
    obsr::delete_object(m_object);
}

entry::node::node()
    : entry(obsr::empty_handle)
    , last_value(obsr::value::make())
    , getter()
{}

entry::node::node(const obsr::entry entry, getter_func&& getter)
    : entry(entry)
    , last_value(obsr::value::make())
    , getter(std::move(getter))
{}

entry_ref::entry_ref()
    : m_registry(nullptr)
    , m_entry(nullptr)
{}

entry_ref::entry_ref(registry* registry, entry* entry)
    : m_registry(registry)
    , m_entry(entry)
{}

entry_ref::~entry_ref() {
    if (m_entry != nullptr) {
        m_registry->remove_ref(*m_entry);
    }
}

entry_ref::entry_ref(entry_ref&& other) noexcept
    : m_registry(other.m_registry)
      , m_entry(other.m_entry) {
    other.m_entry = nullptr;
}

entry_ref& entry_ref::operator=(entry_ref&& other) noexcept {
    m_registry = other.m_registry;
    m_entry = other.m_entry;
    other.m_entry = nullptr;

    return *this;
}

void entry_ref::set_bool(const std::string_view name, const bool value) {
    m_entry->set(name, obsr::value::make_boolean(value));
}

void entry_ref::set_int32(const std::string_view name, const int32_t value) {
    m_entry->set(name, obsr::value::make_int32(value));
}

void entry_ref::set_float(const std::string_view name, const float value) {
    m_entry->set(name, obsr::value::make_float(value));
}

void entry_ref::set_double(const std::string_view name, const double value) {
    m_entry->set(name, obsr::value::make_double(value));
}

void entry_ref::set_string(const std::string_view name, const std::string_view value) {
    m_entry->set(name, obsr::value::make_string(value));
}

void entry_ref::add_bool(const std::string_view name, std::function<bool()>&& getter) {
    m_entry->add(name, [getter](const auto& last_value)-> std::optional<obsr::value> {
        const auto current_value = getter();
        if (last_value.get_type() != obsr::value_type::boolean || last_value.get_boolean() != current_value) {
            return obsr::value::make_boolean(current_value);
        }

        return std::nullopt;
    });
}

void entry_ref::add_int32(const std::string_view name, std::function<int32_t()>&& getter) {
    m_entry->add(name, [getter](const auto& last_value)-> std::optional<obsr::value> {
        const auto current_value = getter();
        if (last_value.get_type() != obsr::value_type::integer32 || last_value.get_int32() != current_value) {
            return obsr::value::make_int32(current_value);
        }

        return std::nullopt;
    });
}

void entry_ref::add_float(const std::string_view name, std::function<float()>&& getter) {
    m_entry->add(name, [getter](const auto& last_value)-> std::optional<obsr::value> {
        const auto current_value = getter();
        if (last_value.get_type() != obsr::value_type::floating_point32 || last_value.get_float() != current_value) {
            return obsr::value::make_float(current_value);
        }

        return std::nullopt;
    });
}

void entry_ref::add_double(const std::string_view name, std::function<double()>&& getter) {
    m_entry->add(name, [getter](const auto& last_value)-> std::optional<obsr::value> {
        const auto current_value = getter();
        if (last_value.get_type() != obsr::value_type::floating_point64 || last_value.get_double() != current_value) {
            return obsr::value::make_double(current_value);
        }

        return std::nullopt;
    });
}

registry::registry()
    : m_root(obsr::get_child(obsr::get_root(), "autobot"))
    , m_entries()
{}

void registry::update() {
    for (auto& entry : m_entries) {
        entry.refresh();
    }
}

bind registry::create_bind(const obsr::object object) {
    auto& entry = m_entries.emplace_back(object);
    entry_ref ref(this, &entry);
    return bind{std::move(ref)};
}

void registry::remove_ref(const entry& entry) {
    if (const auto it = std::ranges::find(m_entries, entry); it != m_entries.end()) {
        it->delete_object();
        m_entries.erase(it);
    }
}

bind::bind()
    : m_entry()
{}

bind::bind(entry_ref&& entry)
    : m_entry(std::move(entry))
{}

registry& get_registry() {
    static registry registry;
    return registry;
}

}
