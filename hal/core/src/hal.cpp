
#include "../include/hal_except.h"
#include "hal.h"

namespace autobot::hal {

void initialize(std::function<void(backend::backend_impl*)>&& initializer) {
    backend::backend_impl impl{};
    initializer(&impl);

    const auto result = initialize(&impl);
    result_to_exception(result);
}

base_device::base_device(const handle handle)
    : m_handle(handle)
{}

base_device::base_device(base_device&& other) noexcept
    : m_handle(other.m_handle) {
    other.m_handle = invalid_handle;
}

base_device::~base_device() {
    close();
}

base_device& base_device::operator=(base_device&& other) noexcept {
    m_handle = other.m_handle;
    other.m_handle = invalid_handle;
    return *this;
}

base_device::operator bool() const {
    return m_handle != invalid_handle;
}

bool base_device::operator==(const base_device& other) const {
    return m_handle == other.m_handle;
}

bool base_device::operator!=(const base_device& other) const {
    return m_handle != other.m_handle;
}

bool base_device::is_open() const {
    return m_handle != invalid_handle;
}

uint32_t base_device::read_config_u32(const config_key key) {
    const auto result = hal::config_read_u32(m_handle, key);
    result_to_exception(result);

    return result.value();
}

float base_device::read_config_f32(const config_key key) {
    const auto result = hal::config_read_f32(m_handle, key);
    result_to_exception(result);

    return result.value();
}

void base_device::write_config_u32(const config_key key, const uint32_t value) {
    const auto result = hal::config_write_u32(m_handle, key, value);
    result_to_exception(result);
}

void base_device::write_config_f32(const config_key key, const float value) {
    const auto result = hal::config_write_f32(m_handle, key, value);
    result_to_exception(result);
}

uint32_t base_device::read_value_u32(const value_key key) {
    const auto result = hal::value_read_u32(m_handle, key);
    result_to_exception(result);

    return result.value();
}

float base_device::read_value_f32(const value_key key) {
    const auto result = hal::value_read_f32(m_handle, key);
    result_to_exception(result);

    return result.value();
}

void base_device::write_value_u32(const value_key key, const uint32_t value) {
    const auto result = hal::value_write_u32(m_handle, key, value);
    result_to_exception(result);
}

void base_device::write_value_f32(const value_key key, const float value) {
    const auto result = hal::value_write_f32(m_handle, key, value);
    result_to_exception(result);
}

void base_device::close() {
    if (m_handle != invalid_handle) {
        hal::close(m_handle); // NOLINT(*-unused-return-value)
        m_handle = invalid_handle;
    }
}

}
