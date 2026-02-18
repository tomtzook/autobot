
#include "global.h"

#include "handles.h"

#include <ranges>

namespace autobot::hal {

static handle find_free_handle() {
    auto& global_data = get_global_data();
    for (auto handle = min_handle; handle < max_handle; handle++) {
        if (const auto it = global_data.handles.find(handle); it == global_data.handles.end()) {
            return handle;
        }
    }

    return invalid_handle;
}

result<std::pair<handle, handle_node*>> allocate_handle() {
    const auto new_handle = find_free_handle();
    if (new_handle == invalid_handle) {
        return error_result(error::no_more_handles);
    }

    auto& global_data = get_global_data();
    auto [it, inserted] = global_data.handles.emplace(new_handle, handle_node{});
    if (!inserted) {
        return error_result(error::handle_allocation_failure);
    }

    return std::pair<handle, handle_node*>{new_handle, &it->second};
}

void release_handle(const handle handle) {
    auto& global_data = get_global_data();
    if (const auto it = global_data.handles.find(handle); it != global_data.handles.end()) {
        global_data.handles.erase(it);
    }
}

std::optional<handle_node*> lookup_handle(const handle handle) {
    auto& global_data = get_global_data();
    if (const auto it = global_data.handles.find(handle); it != global_data.handles.end()) {
        return &it->second;
    }

    return std::nullopt;
}

std::optional<handle_node*> lookup_handle_to(const device& device) {
    auto& global_data = get_global_data();

    for (auto& data : global_data.handles | std::views::values) {
        if (data.device->id == device.id) {
            return &data;
        }
    }

    return std::nullopt;
}

}
