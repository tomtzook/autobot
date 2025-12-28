#pragma once

#include <map>

#include "source.h"

namespace data {

class registry {
public:
    registry();

    registry(const registry&) = delete;
    registry(registry&&) = delete;
    registry& operator=(const registry&) = delete;
    registry& operator=(registry&&) = delete;

    [[nodiscard]] std::optional<data_source> get(uint64_t id) const;
    [[nodiscard]] bool has_children(uint64_t id) const;

    void foreach_child(uint64_t id, std::function<void(data_source&)>&& callback);

    void create(obsr_entry_ptr ptr, uint64_t parent_id = 0);
    void create(obsr_object_ptr ptr, uint64_t parent_id = 0);

private:
    struct node {
        data_source source;
        std::vector<uint64_t> children;
    };

    uint64_t m_next_id;
    std::map<uint64_t, node> m_data;
};

registry& get_registry();

}
