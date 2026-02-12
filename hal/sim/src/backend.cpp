
#include <backend/hal_backend.h>
#include <backend/hal_control.h>

#include "global.h"

namespace autobot::hal::sim {

namespace {

result<void> create() {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    data.root_obsr_object = obsr::get_object("autobot/hal/sim");

    return {};
}

result<void> destroy() {
    return {};
}

result<void> port_new(const port_id id, const port_type type) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    port.open.type = type;
    port.is_open = true;

    return {};
}

result<void> port_delete(const port_id id, const port_type) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    port.is_open = false;

    return {};
}

result<uint32_t> port_config_read_u32(const port_id id, const port_type type, const config_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    return port.open.configs[key].u32;
}

result<float> port_config_read_f32(const port_id id, const port_type type, const config_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    return port.open.configs[key].f32;
}

result<void> port_config_write_u32(const port_id id, const port_type type, const config_key key, const uint32_t value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    port.open.configs[key].u32 = value;
    return {};
}

result<void> port_config_write_f32(const port_id id, const port_type type, const config_key key, const float value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    port.open.configs[key].f32 = value;
    return {};
}

result<uint32_t> port_value_read_u32(const port_id id, const port_type type, const value_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    return port.open.values[key].u32;
}

result<float> port_value_read_f32(const port_id id, const port_type type, const value_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    return port.open.values[key].f32;
}

result<void> port_value_write_u32(const port_id id, const port_type type, const value_key key, const uint32_t value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    port.open.values[key].u32 = value;
    return {};
}

result<void> port_value_write_f32(const port_id id, const port_type type, const value_key key, const float value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::port_not_open);
    }

    port.open.values[key].f32 = value;
    return {};
}

}

void initialize(backend::backend_impl* impl) {
    using namespace sim;

    impl->create = create;
    impl->destroy = destroy;
    impl->port_new = port_new;
    impl->port_delete = port_delete;
    impl->port_config_read_u32 = port_config_read_u32;
    impl->port_config_read_f32 = port_config_read_f32;
    impl->port_config_write_u32 = port_config_write_u32;
    impl->port_config_write_f32 = port_config_write_f32;
    impl->port_value_read_u32 = port_value_read_u32;
    impl->port_value_read_f32 = port_value_read_f32;
    impl->port_value_write_u32 = port_value_write_u32;
    impl->port_value_write_f32 = port_value_write_f32;
}

}
