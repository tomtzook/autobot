#pragma once

#include <obsr.h>

#include "hal_types.h"

namespace autobot::hal {

struct config_def {
    const char* name;
    bool supported;
    device_type supported_types;
    data_type type;
    data_permission permission;
};

struct value_def {
    const char* name;
    bool supported;
    device_type supported_types;
    data_type type;
    data_permission permission;
};

struct device {
    device_id id;
    const char* name;
    device_type supported_types;
    obsr::object obsr_object;

    config_def configs[max_config_key];
    value_def values[max_value_key];
};

}
