
#include "registry.h"
#include "obsr_storage.h"

namespace data {

obsr_storage::obsr_storage()
    : m_mutex()
    , m_root_node()
    , m_entries()
    , m_listener(obsr::empty_handle) {
    m_root_node = create_root();
    m_listener = obsr::listen_object(obsr::get_root(), [this](const obsr::event& event)->void {
        handle_event(event);
    });

    obsr::start_client("127.0.0.1", 50000);
}

obsr_storage::~obsr_storage() {
    if (obsr::empty_handle != m_listener) {
        obsr::delete_listener(m_listener);
        m_listener = obsr::empty_handle;
    }
}

[[nodiscard]] std::pair<std::unique_lock<std::mutex>, const obsr_object*> obsr_storage::use() {
    return {std::unique_lock<std::mutex>(m_mutex), m_root_node};
}

void obsr_storage::update() {}

void obsr_storage::handle_event(const obsr::event& event) {
    std::unique_lock lock(m_mutex);

    switch (event.get_type()) {
        case obsr::event_type::created: {
            iterate_path(event.get_path());
            break;
        }
        case obsr::event_type::deleted: {
            // todo: delete? but what about left over objects, and removing from registry
            // m_entries.erase(event.get_path());
            break;
        }
        case obsr::event_type::value_changed: {
            if (const auto it = m_entries.find(event.get_path()); it != m_entries.end()) {
                it->second->update(event.get_value());

                iterate_path(event.get_path(), [](obsr_object* obj)->void {
                    obj->update();
                });
            }

            break;
        }
    }
}

void obsr_storage::iterate_path(const std::string& path,
    std::function<void(obsr_object*)>&& visit_obj) {

    size_t pos = 0;
    const auto len = path.length();

    obsr_object* current = m_root_node;
    size_t index;
    do {
        index = path.find('/', pos + 1);
        if (index < len) {
            const auto name = path.substr(pos + 1, index - pos - 1);
            pos = index;

            current = get_or_create_child_node(current, name);
            if (current != nullptr) {
                if (visit_obj) {
                    visit_obj(current);
                }
            } else {
                return;
            }
        } else {
            // this is the leaf
            const auto name = path.substr(pos + 1);
            get_or_create_leaf_node(current, name);
        }
    } while (index < len);
}

obsr_object* obsr_storage::get_or_create_child_node(obsr_object* parent, const std::string& name) {
    if (const auto opt = parent->get_child(name); opt) {
        return opt.value();
    }

    const auto handle = obsr::get_child(parent->get_handle(), name);
    return create_child(parent, handle);
}

obsr_object* obsr_storage::create_child(obsr_object* parent, obsr::object handle) {
    const auto data = std::make_shared<obsr_object>(handle);

    auto& registry = get_registry();
    registry.create(data);

    parent->add_child(obsr::get_name_for_object(handle), data.get());

    return data.get();
}

obsr_entry* obsr_storage::get_or_create_leaf_node(obsr_object* parent, const std::string& name) {
    if (const auto opt = parent->get_entry(name)) {
        return opt.value();
    }

    const auto handle = obsr::get_entry(parent->get_handle(), name);
    return create_leaf(parent, handle);
}

obsr_entry* obsr_storage::create_leaf(obsr_object* parent, const obsr::entry handle) {
    auto data = std::make_shared<obsr_entry>(handle);

    auto& registry = get_registry();
    registry.create(data);

    m_entries.emplace(obsr::get_path_for_entry(handle), data);
    parent->add_entry(obsr::get_name_for_entry(handle), data.get());

    return data.get();
}

obsr_object* obsr_storage::create_root() {
    const auto data = std::make_shared<obsr_object>(obsr::get_root());

    auto& registry = get_registry();
    registry.create(data);

    return data.get();
}

}
