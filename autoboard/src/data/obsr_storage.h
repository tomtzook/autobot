#pragma once

#include <map>
#include <mutex>

#include <obsr.h>

#include "data/obsr.h"

namespace data {

class obsr_storage {
public:
    obsr_storage();
    ~obsr_storage();

    obsr_storage(const obsr_storage&) = delete;
    obsr_storage(obsr_storage&&) = delete;
    obsr_storage& operator=(const obsr_storage&) = delete;
    obsr_storage& operator=(obsr_storage&&) = delete;

    [[nodiscard]] std::pair<std::unique_lock<std::mutex>, const obsr_object*> use();
    void update();

private:
    void handle_event(const obsr::event& event);
    void iterate_path(const std::string& path, std::function<void(obsr_object*)>&& visit_obj = {});

    obsr_object* get_or_create_child_node(obsr_object* parent, const std::string& name);
    obsr_object* create_child(obsr_object* parent, obsr::object handle);

    obsr_entry* get_or_create_leaf_node(obsr_object* parent, const std::string& name);
    obsr_entry* create_leaf(obsr_object* parent, obsr::entry handle);

    static obsr_object* create_root();

    std::mutex m_mutex;

    obsr_object* m_root_node;
    std::map<std::string, obsr_entry_ptr> m_entries;

    obsr::listener m_listener;
};

}
