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

    void create(obsr_entry_ptr ptr);
    void create(obsr_object_ptr ptr);

private:
    uint64_t m_next_id;
    std::map<uint64_t, data_source> m_data;
};

registry& get_registry();

}
