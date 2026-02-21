
#include "registry.h"

#include <ranges>

namespace data {

registry::registry()
    : m_next_id(0)
    , m_data()
{}

std::optional<data_source> registry::get(const uint64_t id) const {
    const auto it = m_data.find(id);
    if (it == m_data.end()) {
        return std::nullopt;
    }

    return it->second.source;
}

bool registry::has_children(const uint64_t id) const {
    if (const auto it = m_data.find(id); it != m_data.end()) {
        return !it->second.children.empty();
    }

    return false;
}

void registry::foreach(std::function<void(data_source&)>&& callback) {
    for (auto& source: m_data | std::views::values) {
        callback(source.source);
    }
}

void registry::foreach_child(const uint64_t id, std::function<void(data_source&)>&& callback) {
    const auto it = m_data.find(id);
    if (it == m_data.end()) {
        return;
    }

    for (auto& child : it->second.children) {
        if (const auto it_child = m_data.find(child); it_child != m_data.end()) {
            callback(it_child->second.source);
        }
    }
}

void registry::create(obsr_entry_ptr ptr, const uint64_t parent_id) {
    const auto id = m_next_id++;
    ptr->set_id(id);

    data_source source(id, std::move(ptr));
    m_data.emplace(id, std::move(source));

    if (parent_id != 0) {
        if (const auto it = m_data.find(parent_id); it != m_data.end()) {
            it->second.children.push_back(id);
        }
    }
}

void registry::create(obsr_object_ptr ptr, const uint64_t parent_id) {
    const auto id = m_next_id++;
    ptr->set_id(id);

    data_source source(id, std::move(ptr));
    m_data.emplace(id, std::move(source));

    if (parent_id != 0) {
        if (const auto it = m_data.find(parent_id); it != m_data.end()) {
            it->second.children.push_back(id);
        }
    }
}

registry& get_registry() {
    static registry registry;
    return registry;
}

}
