#pragma once

#include <optional>
#include <utility>

#include "hal_types.h"
#include "ports.h"

namespace autobot::hal {

enum class handle_type {
    port
};

struct handle_node {
    handle_type type;
    struct {
        const port* port;
        port_type type;
    } src;
};

result<std::pair<handle, handle_node*>>  allocate_handle();
void release_handle(handle handle);

std::optional<handle_node*> lookup_handle(handle handle);

}
