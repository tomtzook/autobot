
#include "hal_except.h"
#include "hal.h"

namespace autobot::hal {

static handle try_open(const device_id id, const device_type type) {
    const auto result = open(id, type);
    result_to_exception(result);

    return result.value();
}

void initialize(std::function<void(backend::backend_impl*)>&& initializer) {
    backend::backend_impl impl{};
    initializer(&impl);

    const auto result = initialize(&impl);
    result_to_exception(result);
}

base_device::base_device()
    : m_handle(invalid_handle)
{}

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

device_id base_device::id() const {
    if (is_open()) {
        return invalid_handle;
    }

    const auto result = query_handle(m_handle);
    result_to_exception(result);

    return result.value().id;
}

device_type base_device::type() const {
    if (is_open()) {
        return 0;
    }

    const auto result = query_handle(m_handle);
    result_to_exception(result);

    return result.value().type;
}

void base_device::close() {
    if (m_handle != invalid_handle) {
        hal::close(m_handle); // NOLINT(*-unused-return-value)
        m_handle = invalid_handle;
    }
}

handle_iterator::iterator::iterator(std::unique_ptr<handle_iterator_context>& context)
    : m_context(std::move(context))
{}

handle_iterator::iterator::pointer handle_iterator::iterator::operator->() const {
    return &m_context->handle;
}

handle_iterator::iterator::reference handle_iterator::iterator::operator*() const {
    return m_context->handle;
}

bool handle_iterator::iterator::operator==(const iterator& other) const {
    if ((m_context == nullptr || m_context->done) && (other.m_context == nullptr || other.m_context->done)) {
        return true;
    }
    if (m_context == nullptr || other.m_context == nullptr) {
        return false;
    }

    return m_context->handle == other.m_context->handle;
}

bool handle_iterator::iterator::operator!=(const iterator& other) const {
    return !operator==(other);
}

handle_iterator::iterator& handle_iterator::iterator::operator++() {
    const auto result = iterate_handles_next(m_context);
    result_to_exception(result);

    return *this;
}

handle_iterator::iterator handle_iterator::begin() {
    auto result = iterate_handles();
    result_to_exception(result);

    return iterator{result.value()};
}

handle_iterator::iterator handle_iterator::end() {
    return iterator{};
}

device_iterator::iterator::iterator(std::unique_ptr<device_iterator_context>& context)
    : m_context(std::move(context))
{}

device_iterator::iterator::pointer device_iterator::iterator::operator->() const {
    return &m_context->id;
}

device_iterator::iterator::reference device_iterator::iterator::operator*() const {
    return m_context->id;
}

bool device_iterator::iterator::operator==(const iterator& other) const {
    if ((m_context == nullptr || m_context->done) && (other.m_context == nullptr || other.m_context->done)) {
        return true;
    }
    if (m_context == nullptr || other.m_context == nullptr) {
        return false;
    }

    return m_context->id == other.m_context->id;
}

bool device_iterator::iterator::operator!=(const iterator& other) const {
    return !operator==(other);
}

device_iterator::iterator& device_iterator::iterator::operator++() {
    const auto result = iterate_devices_next(m_context);
    result_to_exception(result);

    return *this;
}

device_iterator::iterator device_iterator::begin() {
    auto result = iterate_devices();
    result_to_exception(result);

    return iterator{result.value()};
}

device_iterator::iterator device_iterator::end() {
    return iterator{};
}

device::device(const handle handle)
    : base_device(handle)
{}

device::device(const device_id id, const device_type type)
    : device(try_open(id, type))
{}

device_query_result device::query() const {
    const auto result = query_device(id());
    result_to_exception(result);

    return result.value();
}

config_query_result device::query_config(const config_key key) const {
    const auto result = query_device_config(id(), key);
    result_to_exception(result);

    return result.value();
}

value_query_result device::query_value(const value_key key) const {
    const auto result = query_device_value(id(), key);
    result_to_exception(result);

    return result.value();
}

serial_query_result device::query_serial() const {
    const auto result = query_device_serial(id());
    result_to_exception(result);

    return result.value();
}

uint32_t device::read_config_u32(const config_key key) const {
    const auto result = hal::config_read_u32(underlying_handle(), key);
    result_to_exception(result);

    return result.value();
}

float device::read_config_f32(const config_key key) const {
    const auto result = hal::config_read_f32(underlying_handle(), key);
    result_to_exception(result);

    return result.value();
}

void device::write_config_u32(const config_key key, const uint32_t value) {
    const auto result = hal::config_write_u32(underlying_handle(), key, value);
    result_to_exception(result);
}

void device::write_config_f32(const config_key key, const float value) {
    const auto result = hal::config_write_f32(underlying_handle(), key, value);
    result_to_exception(result);
}

uint32_t device::read_value_u32(const value_key key) const {
    const auto result = hal::value_read_u32(underlying_handle(), key);
    result_to_exception(result);

    return result.value();
}

float device::read_value_f32(const value_key key) const {
    const auto result = hal::value_read_f32(underlying_handle(), key);
    result_to_exception(result);

    return result.value();
}

void device::write_value_u32(const value_key key, const uint32_t value) {
    const auto result = hal::value_write_u32(underlying_handle(), key, value);
    result_to_exception(result);
}

void device::write_value_f32(const value_key key, const float value) {
    const auto result = hal::value_write_f32(underlying_handle(), key, value);
    result_to_exception(result);
}

size_t device::read(const std::span<uint8_t> buffer) {
    const auto result = hal::serial_read(underlying_handle(), buffer);
    result_to_exception(result);

    return result.value();
}

void device::write(const std::span<const uint8_t> buffer) {
    const auto result = hal::serial_write(underlying_handle(), buffer);
    result_to_exception(result);
}

size_t device::transact(const std::span<const uint8_t> write_buffer, const std::span<uint8_t> read_buffer) {
    const auto result = hal::serial_transact(underlying_handle(), write_buffer, read_buffer);
    result_to_exception(result);

    return result.value();
}

digital_port::digital_port(const handle handle)
    : device(handle)
{}

digital_port::digital_port(const device_id id, const bool output)
    : device(id, output ? type_port_digital_output : type_port_digital_input)
{}

digital_poll_edge digital_port::poll_edge() const {
    return static_cast<digital_poll_edge>(read_config_u32(config_digital_poll_edge));
}

void digital_port::poll_edge(const digital_poll_edge value) {
    write_config_u32(config_digital_poll_edge, value);
}

digital_resistor_mode digital_port::resistor_mode() const {
    return static_cast<digital_resistor_mode>(read_config_u32(config_digital_resistor_mode));
}

void digital_port::resistor_mode(const digital_resistor_mode value) {
    write_config_u32(config_digital_resistor_mode, value);
}

digital_signal_value digital_port::read() const {
    return static_cast<digital_signal_value>(read_value_u32(value_digital_io_signal));
}

void digital_port::write(const digital_signal_value value) {
    write_value_u32(value_digital_io_signal, value);
}

}
