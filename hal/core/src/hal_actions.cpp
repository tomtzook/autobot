
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
    global_data.backend_initialized = true;

    if (const auto result = backend::create(); !result) {
        global_data.backend_initialized = false;
        return error_result(result.error());
    }

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
    node->src.port.port = port;
    node->src.port.type = type;

    if (const auto result = backend::port_new(*node); !result) {
        release_handle(handle);
    }

    return handle;
}

result<handle> open_serial(const serial_id id, const serial_type type) {
    const auto lock = lock_instance();

    const auto opt = lookup_serial(id);
    if (!opt) {
        return error_result(error::serial_not_exists);
    }

    const auto& serial = opt.value();
    if (type != serial->type) {
        return error_result(error::type_unsupported_by_serial);
    }

    const auto allocate_result = allocate_handle();
    if (!allocate_result) {
        return error_result(allocate_result.error());
    }

    auto [handle, node] = allocate_result.value();
    node->type = handle_type::serial;
    node->src.serial.serial = serial;
    node->src.serial.type = type;

    if (const auto result = backend::serial_new(*node); !result) {
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

    switch (const auto& node = opt.value(); node->type) {
        case handle_type::port:
            if (const auto result = backend::port_delete(*node); !result) {
                // TODO: TRACE!
            }
            break;
        case handle_type::serial:
            if (const auto result = backend::serial_delete(*node); !result) {
                // TODO: TRACE!
            }
            break;
    }

    release_handle(handle);

    return {};
}

}
