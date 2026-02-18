#pragma once

#include <functional>

#include <hal_types.h>
#include <backend/hal_backend.h>

namespace autobot::hal::sim {

struct generic_value {
    data_type type;
    union {
        uint32_t u32;
        float f32;
    };
};

using callback_config_set = std::function<void(device_id, config_key, const generic_value&)>;
using callback_value_set = std::function<void(device_id, value_key, const generic_value&)>;

void initialize(backend::backend_impl* impl);

void define(device_id id, const char* name, device_type supported_types);
void define_config(device_id id, config_key key, const char* name, device_type supported_types, data_type type, data_permission permission);
void define_value(device_id id, value_key key, const char* name, device_type supported_types, data_type type, data_permission permission);
void define_serial(device_id id, device_type supported_types, data_permission permission);

void set_config_u32(device_id id, config_key key, uint32_t value);
void set_config_f32(device_id id, config_key key, float value);
void set_config_callback(device_id id, config_key key, callback_config_set&& callback);

void set_value_u32(device_id id, value_key key, uint32_t value);
void set_value_f32(device_id id, value_key key, float value);
void set_value_callback(device_id id, value_key key, callback_config_set&& callback);

}
