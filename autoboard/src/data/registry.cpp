
#include "registry.h"

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

    return it->second;
}

void registry::create(obsr_entry_ptr ptr) {
    const auto id = m_next_id++;
    ptr->set_id(id);

    data_source source(id, std::move(ptr));
    m_data.emplace(id, std::move(source));
}

void registry::create(obsr_object_ptr ptr) {
    const auto id = m_next_id++;
    ptr->set_id(id);

    data_source source(id, std::move(ptr));
    m_data.emplace(id, std::move(source));
}

registry& get_registry() {
    static registry registry;
    return registry;
}

}
