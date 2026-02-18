#pragma once

#include <mutex>
#include <unordered_map>
#include <obsr.h>

#include "backend/hal_backend.h"
#include "handles.h"
#include "devices.h"

namespace autobot::hal {

struct global_data {
    global_data();
    ~global_data();

    std::mutex mutex;
    bool backend_initialized = false;
    backend::backend_impl backend;
    obsr::object root_obsr_object;
    std::unordered_map<handle, handles::handle_node> handles;
    std::unordered_map<device_id, device> devices;
};

std::unique_lock<std::mutex> lock_instance();
global_data& get_global_data();

std::optional<const device*> lookup_device(device_id id);

}
