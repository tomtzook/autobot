
#include "global.h"
#include "handles.h"
#include "hal_actions.h"


namespace autobot::hal {

result<handle_query_result> query_handle(const handle handle) {
    const auto lock = lock_instance();
    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto* node = opt.value();
    return handle_query_result{ node->device->id, node->type };
}

result<device_query_result> query_device(const device_id id) {
    const auto lock = lock_instance();
    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto* device = opt.value();
    device_query_result result{};
    result.id = device->id;
    result.supported_types = device->supported_types;

    if (const auto opt_handle = handles::lookup_handle_to(*device)) {
        const auto* handle_node = opt_handle.value();
        result.handle = handle_node->handle;
        result.open_type = handle_node->type;
    } else {
        result.handle = invalid_handle;
        result.open_type = 0;
    }

    return result;
}

}
