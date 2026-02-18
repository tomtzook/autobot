
#include "global.h"
#include "hal_obsr.h"
#include "backend/hal_control.h"

namespace autobot::hal::control {

result<void> register_new(const device_id id, const char* name, const device_type supported_types) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    if (const auto it = data.devices.find(id); it != data.devices.end()) {
        return error_result(error::device_already_configured);
    }

    device port{};
    port.id = id;
    port.name = name;
    port.supported_types = supported_types;

    port.obsr_object = show_in_obsr(data.root_obsr_object, port);

    data.devices.emplace(id, port);

    return {};
}

result<void> set_config(const device_id id, const config_key key, const char* name, const device_type supported_types, const data_type type, const data_permission permission) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_exists);
    }

    if (lookup_handle_to(it->second).has_value()) {
        return error_result(error::cannot_configure_while_open);
    }

    auto& config = it->second.configs[key];
    config.name = name;
    config.supported_types = supported_types;
    config.type = type;
    config.permission = permission;
    config.supported = true;

    const auto root = obsr::get_child(it->second.obsr_object, "configs");
    show_in_obsr(root, config, key);

    return {};
}

result<void> set_value(const device_id id, const value_key key, const char* name, const device_type supported_types, const data_type type, const data_permission permission) {
    const auto lock = lock_instance();

    auto& data = get_global_data();
    const auto it = data.devices.find(id);
    if (it == data.devices.end()) {
        return error_result(error::device_not_exists);
    }

    if (lookup_handle_to(it->second).has_value()) {
        return error_result(error::cannot_configure_while_open);
    }

    auto& value = it->second.values[key];
    value.name = name;
    value.supported_types = supported_types;
    value.type = type;
    value.permission = permission;
    value.supported = true;

    const auto root = obsr::get_child(it->second.obsr_object, "values");
    show_in_obsr(root, value, key);

    return {};
}

}
