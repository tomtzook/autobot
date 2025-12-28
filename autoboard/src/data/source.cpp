
#include "scheme.h"
#include "source.h"

namespace data {

data_source::data_source(const uint64_t id, obsr_entry_ptr data)
    : m_id(id)
    , m_data(std::move(data))
{}

data_source::data_source(const uint64_t id, obsr_object_ptr data)
    : m_id(id)
    , m_data(std::move(data))
{}

uint64_t data_source::get_id() const {
    return m_id;
}

std::string_view data_source::get_name() const {
    return std::visit([](const auto& t)->std::string_view {
        return t->get_name();
    }, m_data);
}

bool data_source::has_new_data() const {
    return std::visit([](const auto& t)->bool {
        return t->has_new_data();
    }, m_data);
}

scheme::type data_source::get_scheme() const {
    return std::visit([](const auto& t)->auto {
        return t->get_scheme();
    }, m_data);
}

}
