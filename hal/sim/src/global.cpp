
#include "global.h"

namespace autobot::hal::sim {

std::unique_lock<std::mutex> lock_instance() {
    return std::unique_lock<std::mutex>{get_global_data().mutex};
}

global_data& get_global_data() {
    static global_data data{};
    return data;
}

}
