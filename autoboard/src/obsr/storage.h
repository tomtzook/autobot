#pragma once

#include <map>
#include <mutex>
#include <variant>

#include <obsr.h>

namespace ui {

struct obsr_entry_data {
    obsr::entry entry = obsr::empty_handle;
    obsr::value value = obsr::value::make();
};

struct obsr_node {
    obsr_node(std::string_view name, obsr::object handle);
    obsr_node(std::string_view name, obsr::entry handle, const obsr_entry_data* data);

    [[nodiscard]] std::string_view name() const;
    [[nodiscard]] uint64_t id() const;
    [[nodiscard]] bool is_object() const;
    [[nodiscard]] bool is_entry() const;
    [[nodiscard]] obsr::handle handle() const;

    std::optional<obsr_node*> find_child(std::string_view name);
    void foreach_child(std::function<void(const obsr_node&)>&& callback) const;

    obsr_node* add_child(std::string_view name, obsr_node&& child);
    void clear();

    [[nodiscard]] const obsr::value* value() const;
    void set_entry(const obsr_entry_data* entry);

private:
    using node_map = std::map<std::string, obsr_node, std::less<>>;
    struct object_data {
        obsr::object handle;
        node_map children;
    };
    struct entry_data {
        obsr::entry handle;
        const obsr_entry_data* ptr;
    };

    std::string m_name;
    uint64_t m_id;
    std::variant<object_data, entry_data> m_data;
};

class obsr_storage {
public:
    obsr_storage();
    ~obsr_storage();

    [[nodiscard]] std::pair<std::unique_lock<std::mutex>, const obsr_node&> use();
    void update();

private:
    void handle_event(const obsr::event& event);
    void rebuild_tree();

    std::mutex m_mutex;

    bool m_rebuild_required;
    obsr_node m_root_node;
    std::map<std::string, obsr_entry_data> m_entries;

    obsr::listener m_listener;
};

}
