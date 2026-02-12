#pragma once

#include <hal_types.h>

namespace autobot::hal::control {

result<void> register_port(port_id id, const char* name, port_type supported_types);
result<void> set_port_config(port_id id, config_key key, port_type supported_types, data_type type, data_permission permission);
result<void> set_port_value(port_id id, value_key key, port_type supported_types, data_type type, data_permission permission);

result<void> register_serial(serial_id id, const char* name, serial_type type);
result<void> set_serial_config(serial_id id, config_key key, data_type type, data_permission permission);

}
