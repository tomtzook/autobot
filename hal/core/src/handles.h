#pragma once

#include <optional>
#include <utility>

#include "hal_types.h"
#include "devices.h"

namespace autobot::hal {

struct handle_node {
    device_type type;
    const device* device;
};

result<std::pair<handle, handle_node*>>  allocate_handle();
void release_handle(handle handle);

std::optional<handle_node*> lookup_handle(handle handle);
std::optional<handle_node*> lookup_handle_to(const device& device);

}
