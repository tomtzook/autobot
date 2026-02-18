
#include "hal_actions.h"
#include "hal_types.h"
#include "handles.h"
#include "global.h"
#include "backend.h"

namespace autobot::hal {

static result<void> verify_valid_request(const handles::handle_node* node, const bool read, const bool write) {
    const auto& def = node->device->serial;
    if (!def.supported) {
        return error_result(error::serial_unsupported);
    }

    if ((def.supported_types & node->type) != node->type) {
        return error_result(error::serial_unsupported);
    }

    if (read && (def.permission != data_permission::readonly && def.permission != data_permission::readwrite)) {
        return error_result(error::no_permissions_for_serial_access);
    }

    if (write && (def.permission != data_permission::writeonly && def.permission != data_permission::readwrite)) {
        return error_result(error::no_permissions_for_serial_access);
    }

    return {};
}

result<size_t> serial_read(const handle handle, const std::span<uint8_t> buffer) {
    const auto lock = lock_instance();
    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    return backend::serial_read(*node, buffer);
}

result<void> serial_write(const handle handle, const std::span<const uint8_t> buffer) {
    const auto lock = lock_instance();
    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    return backend::serial_write(*node, buffer);
}

result<size_t> serial_transact(const handle handle, const std::span<const uint8_t> write_buffer, const std::span<uint8_t> read_buffer) {
    const auto lock = lock_instance();
    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    return backend::serial_transact(*node, write_buffer, read_buffer);
}

}
