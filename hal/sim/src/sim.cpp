
#include <backend/hal_control.h>

#include "global.h"
#include "hal_sim.h"

namespace autobot::hal::sim {

result<void> define_port(const port_id id, const char* name, const port_type supported_types) {
    if (const auto result = control::register_port(id, name, supported_types); !result) {
        return error_result(result.error());
    }

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

result<void> define_port_config(const port_id id, const config_key key,
    const port_type supported_types, const data_type type, const data_permission permission) {
    return control::set_port_config(id, key, supported_types, type, permission);
}

result<void> define_port_value(const port_id id, const value_key key,
    const port_type supported_types, const data_type type, const data_permission permission) {
    return control::set_port_value(id, key, supported_types, type, permission);
}

}
