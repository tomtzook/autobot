#pragma once

#include <hal_types.h>
#include <backend/hal_backend.h>

namespace autobot::hal::sim {

void initialize(backend::backend_impl* impl);

result<void> define_port(port_id id, const char* name, port_type supported_types);
result<void> define_port_config(port_id id, config_key key, port_type supported_types, data_type type, data_permission permission);
result<void> define_port_value(port_id id, value_key key, port_type supported_types, data_type type, data_permission permission);

}
