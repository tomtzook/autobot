#pragma once

#include "scheme.h"
#include "obsr.h"

namespace data {

class data_source {
public:
    data_source(uint64_t id, obsr_entry_ptr data);
    data_source(uint64_t id, obsr_object_ptr data);

    data_source(const data_source&) = default;
    data_source(data_source&&) = default;
    data_source& operator=(const data_source&) = default;
    data_source& operator=(data_source&&) = default;

    [[nodiscard]] uint64_t get_id() const;
    [[nodiscard]] std::string_view get_name() const;
    [[nodiscard]] bool has_new_data() const;
    [[nodiscard]] scheme::type get_scheme() const;

    template<typename t_>
    t_ read();

private:
    using data_type = std::variant<obsr_entry_ptr, obsr_object_ptr>;

    uint64_t m_id;
    data_type m_data;
};

template<typename t_>
t_ data_source::read() {
    return std::visit([](auto& t)->t_ {
        return t->template read<t_>();
    }, m_data);
}

}
