
#include "global.h"
#include "ports.h"
#include "handles.h"
#include "backend.h"

#include "hal_actions.h"

namespace autobot::hal {

result<void> initialize(const backend::backend_impl* impl) {
    const auto lock = lock_instance();

    auto& global_data = get_global_data();
    global_data.backend = *impl;

    if (const auto result = backend::create(); !result) {
        return error_result(result.error());
    }

    global_data.backend_initialized = true;
    return {};
}

result<handle> open_port(const port_id id, const port_type type) {
    const auto lock = lock_instance();

    const auto opt = lookup_port(id);
    if (!opt) {
        return error_result(error::port_not_exists);
    }

    const auto& port = opt.value();
    if ((type & port->supported_types) == 0) {
        return error_result(error::type_unsupported_by_port);
    }

    const auto allocate_result = allocate_handle();
    if (!allocate_result) {
        return error_result(allocate_result.error());
    }

    auto [handle, node] = allocate_result.value();
    node->type = handle_type::port;
    node->src.port = port;
    node->src.type = type;

    if (const auto result = backend::port_new(*node); !result) {
        release_handle(handle);
    }

    return handle;
}

result<void> close(const handle handle) {
    const auto lock = lock_instance();

    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();

    if (const auto result = backend::port_delete(*node); !result) {
        // TODO: TRACE!
    }

    release_handle(handle);

    return {};
}

}
