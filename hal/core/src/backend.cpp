
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

result<void> port_new(const handle_node& node) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_new);

    return backend->port_new(node.src.port->id, node.src.type);
}

result<void> port_delete(const handle_node& node) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_delete);

    return backend->port_delete(node.src.port->id, node.src.type);
}

result<uint32_t> port_config_read_u32(const handle_node& node, const config_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_config_read_u32);

    return backend->port_config_read_u32(node.src.port->id, node.src.type, key);
}

result<float> port_config_read_f32(const handle_node& node, const config_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_config_read_f32);

    return backend->port_config_read_f32(node.src.port->id, node.src.type, key);
}

result<void> port_config_write_u32(const handle_node& node, const config_key key, const uint32_t value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_config_write_u32);

    return backend->port_config_write_u32(node.src.port->id, node.src.type, key, value);
}

result<void> port_config_write_f32(const handle_node& node, const config_key key, const float value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_config_write_f32);

    return backend->port_config_write_f32(node.src.port->id, node.src.type, key, value);
}

result<uint32_t> port_value_read_u32(const handle_node& node, const value_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_value_read_u32);

    return backend->port_value_read_u32(node.src.port->id, node.src.type, key);
}

result<float> port_value_read_f32(const handle_node& node, const value_key key) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_value_read_f32);

    return backend->port_value_read_f32(node.src.port->id, node.src.type, key);
}

result<void> port_value_write_u32(const handle_node& node, const value_key key, const uint32_t value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_value_write_u32);

    return backend->port_value_write_u32(node.src.port->id, node.src.type, key, value);
}

result<void> port_value_write_f32(const handle_node& node, const value_key key, const float value) {
    const auto* backend = get_backend();
    verify_func_exists(backend, port_value_write_f32);

    return backend->port_value_write_f32(node.src.port->id, node.src.type, key, value);
}

}
