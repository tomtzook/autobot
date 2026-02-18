
#include <ranges>

#include "backend.h"
#include "global.h"

namespace autobot::hal {

static void close_all_handles() {
    for (auto& global_data = get_global_data(); auto data: global_data.handles | std::views::values) {
        if (const auto result = backend::free_device(data); !result) {
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
    , root_obsr_object(obsr::get_object("/autobot/hal"))
    , handles()
    , devices() {
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

std::optional<const device*> lookup_device(const device_id id) {
    auto& global_data = get_global_data();
    if (const auto it = global_data.devices.find(id); it != global_data.devices.end()) {
        return &it->second;
    }

    return std::nullopt;
}

}
