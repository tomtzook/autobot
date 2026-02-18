
#include <backend/hal_backend.h>
#include <backend/hal_control.h>

#include "global.h"

namespace autobot::hal::sim {

namespace {

result<void> create() {
    return {};
}

result<void> destroy() {
    return {};
}

result<void> init_device(const device_id id, const device_type type) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    port.open_type = type;
    port.is_open = true;

    return {};
}

result<void> free_device(const device_id id, const device_type) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    port.is_open = false;

    return {};
}

result<uint32_t> config_read_u32(const device_id id, const device_type type, const config_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    return port.configs[key].u32;
}

result<float> config_read_f32(const device_id id, const device_type type, const config_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    return port.configs[key].f32;
}

result<void> config_write_u32(const device_id id, const device_type type, const config_key key, const uint32_t value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    port.configs[key].u32 = value;
    return {};
}

result<void> config_write_f32(const device_id id, const device_type type, const config_key key, const float value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    port.configs[key].f32 = value;
    return {};
}

result<uint32_t> value_read_u32(const device_id id, const device_type type, const value_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    return port.values[key].u32;
}

result<float> value_read_f32(const device_id id, const device_type type, const value_key key) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    const auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    return port.values[key].f32;
}

result<void> value_write_u32(const device_id id, const device_type type, const value_key key, const uint32_t value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    port.values[key].u32 = value;
    return {};
}

result<void> value_write_f32(const device_id id, const device_type type, const value_key key, const float value) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_defined);
    }

    auto& port = it->second;
    if (!port.is_open) {
        return error_result(error::device_not_open);
    }

    port.values[key].f32 = value;
    return {};
}

}

void initialize(backend::backend_impl* impl) {
    using namespace sim;

    impl->create = create;
    impl->destroy = destroy;
    impl->init_device = init_device;
    impl->free_device = free_device;
    impl->config_read_u32 = config_read_u32;
    impl->config_read_f32 = config_read_f32;
    impl->config_write_u32 = config_write_u32;
    impl->config_write_f32 = config_write_f32;
    impl->value_read_u32 = value_read_u32;
    impl->value_read_f32 = value_read_f32;
    impl->value_write_u32 = value_write_u32;
    impl->value_write_f32 = value_write_f32;
}

}
