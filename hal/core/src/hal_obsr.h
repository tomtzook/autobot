#pragma once

#include <obsr.h>
#include "devices.h"


namespace autobot::hal {

obsr::object show_in_obsr(obsr::object root, const device& device);
void show_in_obsr(obsr::object root, const config_def& def, config_key key);
void show_in_obsr(obsr::object root, const value_def& def, value_key key);

}
