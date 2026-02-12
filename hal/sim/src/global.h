#pragma once

#include <unordered_map>
#include <mutex>

#include <hal_types.h>
#include <obsr.h>

namespace autobot::hal::sim {

union value {
    uint32_t u32;
    float f32;
};

struct open_port {
    port_type type;
    value configs[max_config_key];
    value values[max_value_key];

    struct {

    } obsr;
};

struct port {
    port_id id;
    const char* name;
    port_type supported_types;

    bool is_open;
    open_port open;

    struct {
        obsr::object root;
        obsr::object open_port;
        obsr::entry id_entry;
    } obsr;
};

struct global_data {
    std::mutex mutex;
    std::unordered_map<port_id, port> ports;
    obsr::object root_obsr_object;
};

std::unique_lock<std::mutex> lock_instance();
global_data& get_global_data();

}
