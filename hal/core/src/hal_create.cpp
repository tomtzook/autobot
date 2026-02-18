
#include "global.h"
#include "devices.h"
#include "handles.h"
#include "backend.h"

#include "hal_actions.h"

namespace autobot::hal {

result<void> initialize(const backend::backend_impl* impl) {
    const auto lock = lock_instance();

    auto& global_data = get_global_data();
    global_data.backend = *impl;
    global_data.backend_initialized = true;

    if (const auto result = backend::create(); !result) {
        global_data.backend_initialized = false;
        return error_result(result.error());
    }

    return {};
}

result<handle> open(const device_id id, const device_type type) {
    const auto lock = lock_instance();

    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto& device = opt.value();
    if ((type & device->supported_types) == 0) {
        return error_result(error::type_unsupported_by_device);
    }

    if (const auto opt_handle = handles::lookup_handle_to(*device)) {
        return error_result(error::device_already_open);
    }

    const auto allocate_result = handles::allocate_handle();
    if (!allocate_result) {
        return error_result(allocate_result.error());
    }

    auto [handle, node] = allocate_result.value();
    node->handle = handle;
    node->device = device;
    node->type = type;

    if (const auto result = backend::init_device(*node); !result) {
        handles::release_handle(handle);
    }

    return handle;
}

result<void> close(const handle handle) {
    const auto lock = lock_instance();

    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto* node = opt.value();
    if (const auto result = backend::free_device(*node); !result) {
        // TODO: TRACE!
    }

   handles::release_handle(handle);

    return {};
}

}
