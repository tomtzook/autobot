
#include "global.h"
#include "backend/hal_control.h"

namespace autobot::hal::control {

result<void> register_port(const port_id id, const char* name, const port_type supported_types) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    if (const auto it = data.ports.find(id); it != data.ports.end()) {
        return error_result(error::port_already_configured);
    }

    port port{};
    port.id = id;
    port.name = name;
    port.supported_types = supported_types;
    data.ports.emplace(id, port);

    return {};
}

result<void> set_port_config(const port_id id, const config_key key, const port_type supported_types, const data_type type, const data_permission permission) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_exists);
    }

    auto& config = it->second.configs[key];
    config.supported_types = supported_types;
    config.type = type;
    config.permission = permission;
    config.supported = true;

    return {};
}

result<void> set_port_value(const port_id id, const value_key key, const port_type supported_types, const data_type type, const data_permission permission) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.ports.find(id);
    if (it == data.ports.end()) {
        return error_result(error::port_not_exists);
    }

    auto& value = it->second.values[key];
    value.supported_types = supported_types;
    value.type = type;
    value.permission = permission;
    value.supported = true;

    return {};
}

result<void> register_serial(const serial_id id, const char* name, const serial_type type) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    if (const auto it = data.serials.find(id); it != data.serials.end()) {
        return error_result(error::serial_already_configured);
    }

    serial serial{};
    serial.id = id;
    serial.name = name;
    serial.type = type;
    data.serials.emplace(id, serial);

    return {};
}

result<void> set_serial_config(const serial_id id, const config_key key, const data_type type, const data_permission permission) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.serials.find(id);
    if (it == data.serials.end()) {
        return error_result(error::serial_not_exists);
    }

    auto& config = it->second.configs[key];
    config.type = type;
    config.permission = permission;
    config.supported = true;

    return {};
}

}
