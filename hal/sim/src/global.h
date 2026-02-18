#pragma once

#include <unordered_map>
#include <mutex>

#include <hal_types.h>

#include "hal_sim.h"

namespace autobot::hal::sim {

struct config {
    union {
        uint32_t u32;
        float f32;
    };

    callback_config_set callback;
};

struct value {
    union {
        uint32_t u32;
        float f32;
    };

    callback_value_set callback;
};

struct port {
    device_id id = 0;
    const char* name = "";
    device_type supported_types = 0;

    bool is_open = false;
    device_type open_type = 0;

    config configs[max_config_key]{};
    value values[max_value_key]{};
};

struct global_data {
    std::mutex mutex;
    std::unordered_map<device_id, port> devices;
};

std::unique_lock<std::mutex> lock_instance();
global_data& get_global_data();

}
