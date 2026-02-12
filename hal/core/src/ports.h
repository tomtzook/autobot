#pragma once

#include <optional>

#include "hal_types.h"

namespace autobot::hal {

struct config_def {
    bool supported;
    port_type supported_types;
    data_type type;
    data_permission permission;
};

struct value_def {
    bool supported;
    port_type supported_types;
    data_type type;
    data_permission permission;
};

struct port {
    port_id id;
    const char* name;
    port_type supported_types;

    config_def configs[max_config_key];
    value_def values[max_value_key];
};

std::optional<const port*> lookup_port(port_id id);

}
