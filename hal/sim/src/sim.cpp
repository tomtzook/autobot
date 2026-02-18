
#include <backend/hal_control.h>
#include <hal_except.h>

#include "global.h"
#include "hal_sim.h"

namespace autobot::hal::sim {

void define(const device_id id, const char* name, const device_type supported_types) {
    if (const auto result = control::register_new(id, name, supported_types); !result) {
        result_to_exception(result);
    }

    const auto lock = lock_instance();
    auto& data = get_global_data();
    if (const auto it = data.devices.find(id); it != data.devices.end()) {
        throw device_already_configured_exception();
    }

    port port{};
    port.id = id;
    port.name = name;
    port.supported_types = supported_types;
    data.devices.emplace(id, port);
}

void define_config(const device_id id, const config_key key, const char* name,
    const device_type supported_types, const data_type type, const data_permission permission) {
    if (const auto result = control::set_config(id, key, name, supported_types, type, permission);
        !result) {
        result_to_exception(result);
    }
}

void define_value(const device_id id, const value_key key, const char* name,
    const device_type supported_types, const data_type type, const data_permission permission) {
    if (const auto result = control::set_value(id, key, name, supported_types, type, permission);
        !result) {
        result_to_exception(result);
    }
}

void define_serial(const device_id id, const device_type supported_types, const data_permission permission) {
    if (const auto result = control::set_serial(id, supported_types, permission);
        !result) {
        result_to_exception(result);
    }
}

void set_config_u32(const device_id id, const config_key key, const uint32_t value) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.configs[key].u32 = value;
}

void set_config_f32(const device_id id, const config_key key, const float value) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.configs[key].f32 = value;
}

void set_config_callback(const device_id id, const config_key key, callback_config_set&& callback) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.configs[key].callback = std::move(callback);
}

void set_value_u32(const device_id id, const value_key key, const uint32_t value) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.values[key].u32 = value;
}

void set_value_f32(const device_id id, const value_key key, const float value) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.values[key].f32 = value;
}

void set_value_callback(const device_id id, const value_key key, callback_config_set&& callback) {
    const auto lock = lock_instance();
    auto& data = get_global_data();

    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        throw device_not_exists_exception();
    }

    it->second.values[key].callback = std::move(callback);
}

}
