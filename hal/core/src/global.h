#pragma once

#include <mutex>
#include <unordered_map>

#include "backend/hal_backend.h"
#include "handles.h"
#include "ports.h"

namespace autobot::hal {

struct global_data {
    global_data();
    ~global_data();

    std::mutex mutex;
    bool backend_initialized = false;
    backend::backend_impl backend;
    std::unordered_map<handle, handle_node> handles;
    std::unordered_map<port_id, port> ports;
};

std::unique_lock<std::mutex> lock_instance();
global_data& get_global_data();

}
