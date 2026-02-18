#pragma once

#include <unordered_map>
#include <mutex>

#include <hal_types.h>

namespace autobot::hal::sim {

union value {
    uint32_t u32;
    float f32;
};

struct port {
    device_id id;
    const char* name;
    device_type supported_types;

    bool is_open;
    device_type open_type;

    value configs[max_config_key];
    value values[max_value_key];
};

struct global_data {
    std::mutex mutex;
    std::unordered_map<device_id, port> devices;
};

std::unique_lock<std::mutex> lock_instance();
global_data& get_global_data();

}
