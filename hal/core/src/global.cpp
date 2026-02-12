
#include <ranges>

#include "backend.h"
#include "global.h"

namespace autobot::hal {

static void close_all_handles() {
    for (auto& global_data = get_global_data(); auto data: global_data.handles | std::views::values) {
        if (const auto result = backend::port_delete(data); !result) {
            // TODO: TRACE!
        }
    }

    if (const auto result = backend::destroy(); !result) {
        // TODO: TRACE!
    }
}

global_data::global_data()
    : mutex()
    , backend()
    , handles()
    , ports() {
}

global_data::~global_data() {
    close_all_handles();
}



std::unique_lock<std::mutex> lock_instance() {
    return std::unique_lock<std::mutex>{get_global_data().mutex};
}

global_data& get_global_data() {
    static global_data data{};
    return data;
}

}
