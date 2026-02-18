
#include "backend/hal_backend.h"
#include "backend.h"

#include "global.h"
#include "../include/hal_except.h"

namespace autobot::hal::backend {

#define verify_func_exists(_backend, _func) \
    if ((_backend) == nullptr) { return error_result(error::backend_not_configured); } \
    if ((_backend)->_func == nullptr) { return error_result(error::backend_does_not_support_operation); }

static backend_impl* get_backend() {
    auto& data =  get_global_data();
    if (!data.backend_initialized) {
        return nullptr;
    }

    return &data.backend;
}

result<void> create() {
    const auto* backend = get_backend();
    verify_func_exists(backend, create);

    return backend->create();
}

result<void> destroy() {
    const auto* backend = get_backend();
    verify_func_exists(backend, destroy);

    return backend->destroy();
}

result<void> init_device(const handles::handle_node& node) {
    const auto* backend = get_backend();
    verify_func_exists(backend, init_device);

    return backend->init_device(node.device->id, node.type);
}

result<void> free_device(const handles::handle_node& node) {
    const auto* backend = get_backend();
    verify_func_exists(backend, free_device);

    return backend->free_device(node.device->id, node.type);
}

result<uint32_t> config_read_u32(const handles::handle_node& node, const config_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, config_read_u32);

    return backend->config_read_u32(node.device->id, node.type, key);
}

result<float> config_read_f32(const handles::handle_node& node, const config_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, config_read_f32);

    return backend->config_read_f32(node.device->id, node.type, key);
}

result<void> config_write_u32(const handles::handle_node& node, const config_key key, const uint32_t value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, config_write_u32);

    return backend->config_write_u32(node.device->id, node.type, key, value);
}

result<void> config_write_f32(const handles::handle_node& node, const config_key key, const float value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, config_write_f32);

    return backend->config_write_f32(node.device->id, node.type, key, value);
}

result<uint32_t> value_read_u32(const handles::handle_node& node, const value_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, value_read_u32);

    return backend->value_read_u32(node.device->id, node.type, key);
}

result<float> value_read_f32(const handles::handle_node& node, const value_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, value_read_f32);

    return backend->value_read_f32(node.device->id, node.type, key);
}

result<void> value_write_u32(const handles::handle_node& node, const value_key key, const uint32_t value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, value_write_u32);

    return backend->value_write_u32(node.device->id, node.type, key, value);
}

result<void> value_write_f32(const handles::handle_node& node, const value_key key, const float value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, value_write_f32);

    return backend->value_write_f32(node.device->id, node.type, key, value);
}

result<size_t> serial_read(const handles::handle_node& node, const std::span<uint8_t> buffer) {
    const auto* backend = get_backend();
    verify_func_exists(backend, serial_read);

    return backend->serial_read(node.device->id, node.type, buffer);
}

result<void> serial_write(const handles::handle_node& node, const std::span<const uint8_t> buffer) {
    const auto* backend = get_backend();
    verify_func_exists(backend, serial_write);

    return backend->serial_write(node.device->id, node.type, buffer);
}

result<size_t> serial_transact(const handles::handle_node& node, const std::span<const uint8_t> write_buffer, const std::span<uint8_t> read_buffer) {
    const auto* backend = get_backend();
    verify_func_exists(backend, serial_transact);

    return backend->serial_transact(node.device->id, node.type, write_buffer, read_buffer);
}

}
