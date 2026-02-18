#pragma once

#include <hal_types.h>

namespace autobot::hal::control {

result<void> register_new(device_id id, const char* name, device_type supported_types);
result<void> set_config(device_id id, config_key key, const char* name, device_type supported_types, data_type type, data_permission permission);
result<void> set_value(device_id id, value_key key, const char* name, device_type supported_types, data_type type, data_permission permission);

}
