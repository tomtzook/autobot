
#include <functional>

#include "storage.h"

#include <ranges>

namespace ui {

static uint64_t obsr_id(const std::string& name) {
    static std::hash<std::string> hasher;
    return hasher(name);
}

static obsr_node* get_or_create_child_node(obsr_node* current, const std::string& name) {
    const auto opt = current->find_child(name);
    if (opt) {
        return opt.value();
    }

    const auto handle = obsr::get_child(current->handle(), name);
    obsr_node node(name, handle);
    return current->add_child(name, std::move(node));
}

static obsr_node* get_leaf_node(obsr_node* current, const std::string& name) {
    const auto opt = current->find_child(name);
    return opt.value_or(nullptr);
}

static obsr_node* get_or_create_leaf_node(obsr_node* current, const std::string& name) {
    if (auto result = get_leaf_node(current, name); result == nullptr) {
        const auto handle = obsr::get_entry(current->handle(), name);
        obsr_node node(name, handle, nullptr);
        return current->add_child(name, std::move(node));
    } else {
        return result;
    }
}

static obsr_node* iterate_to_path(obsr_node* root_node, const std::string& path) {
    size_t pos = 0;
    const auto len = path.length();

    obsr_node* current = root_node;
    size_t index;
    do {
        index = path.find('/', pos + 1);
        if (index < len) {
            const auto name = path.substr(pos + 1, index - pos - 1);
            pos = index;

            current = get_or_create_child_node(current, name);
            if (current == nullptr) {
                return nullptr;
            }
        } else {
            // this is the leaf
            const auto name = path.substr(pos + 1);
            return get_or_create_leaf_node(current, name);
        }
    } while (index < len);

    return nullptr;
}

obsr_node::obsr_node(const std::string_view name, const obsr::object handle)
    : m_name(name)
    , m_id(obsr_id(m_name))
    , m_data(object_data{handle, {}})
{}

obsr_node::obsr_node(const std::string_view name, const obsr::entry handle, const obsr_entry_data* data)
    : m_name(name)
    , m_id(obsr_id(m_name))
    , m_data(entry_data{handle, data})
{}

std::string_view obsr_node::name() const {
    return m_name;
}

uint64_t obsr_node::id() const {
    return m_id;
}

bool obsr_node::is_object() const {
    return std::holds_alternative<object_data>(m_data);
}

bool obsr_node::is_entry() const {
    return std::holds_alternative<entry_data>(m_data);
}

obsr::handle obsr_node::handle() const {
    if (is_object()) {
        auto& data = std::get<object_data>(m_data);
        return data.handle;
    }

    auto& data = std::get<entry_data>(m_data);
    return data.handle;
}

std::optional<obsr_node*> obsr_node::find_child(const std::string_view name) {
    if (is_object()) {
        auto& data = std::get<object_data>(m_data);
        const auto it = data.children.find(name);
        if (it != data.children.end()) {
            return {&it->second};
        }
    }

    return std::nullopt;
}

void obsr_node::foreach_child(std::function<void(const obsr_node&)>&& callback) const {
    if (is_entry()) {
        throw std::runtime_error("entry has no children");
    }

    auto& data = std::get<object_data>(m_data);
    for (auto& child: data.children | std::views::values) {
        callback(child);
    }
}

obsr_node* obsr_node::add_child(const std::string_view name, obsr_node&& child) {
    if (is_entry()) {
        throw std::runtime_error("cannot add child to entry");
    }

    auto& data = std::get<object_data>(m_data);
    auto [it, _] = data.children.emplace(name, std::move(child));
    return &it->second;
}

void obsr_node::clear() {
    if (is_object()) {
        auto& data = std::get<object_data>(m_data);
        data.children.clear();
    }
}

const obsr::value* obsr_node::value() const {
    if (is_object()) {
        throw std::runtime_error("object has no value");
    }

    auto& data = std::get<entry_data>(m_data);
    if (data.ptr != nullptr) {
        return &data.ptr->value;
    }

    return nullptr;
}

void obsr_node::set_entry(const obsr_entry_data* entry) {
    if (is_object()) {
        throw std::runtime_error("cannot set entry on object");
    }

    auto& data = std::get<entry_data>(m_data);
    data.ptr = entry;
}

obsr_storage::obsr_storage()
    : m_mutex()
    , m_rebuild_required(true)
    , m_root_node("root", obsr::get_root())
    , m_entries()
    , m_listener(obsr::empty_handle) {
    const auto root = obsr::get_root();
    m_listener = obsr::listen_object(root, [this](const obsr::event& event)->void {
        handle_event(event);
    });

    obsr::start_client("127.0.0.1", 50000);
}

obsr_storage::~obsr_storage() {
    if (obsr::empty_handle != m_listener) {
        obsr::delete_listener(m_listener);
    }
}

std::pair<std::unique_lock<std::mutex>, const obsr_node&> obsr_storage::use() {
    return {std::unique_lock<std::mutex>(m_mutex), m_root_node};
}

void obsr_storage::update() {
    std::unique_lock lock(m_mutex);

    if (m_rebuild_required) {
        m_rebuild_required = false;
        rebuild_tree();
    }
}

void obsr_storage::handle_event(const obsr::event& event) {
    std::unique_lock lock(m_mutex);

    switch (event.get_type()) {
        case obsr::event_type::created: {
            obsr_entry_data data;
            data.entry = obsr::get_entry(event.get_path());
            data.value = obsr::value::make();

            m_entries.emplace(event.get_path(), data);
            break;
        }
        case obsr::event_type::deleted: {
            m_entries.erase(event.get_path());
            break;
        }
        case obsr::event_type::value_changed: {
            if (const auto it = m_entries.find(event.get_path()); it != m_entries.end()) {
                it->second.value = event.get_value();
            }

            break;
        }
    }

    m_rebuild_required = true;
}

void obsr_storage::rebuild_tree() {
    m_root_node.clear();

    for (auto& [path, entry] : m_entries) {
        auto* node = iterate_to_path(&m_root_node, path);
        node->set_entry(&entry);
    }
}

}
