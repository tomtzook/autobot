
#include "hal_actions.h"
#include "hal_types.h"
#include "handles.h"
#include "global.h"
#include "backend.h"

namespace autobot::hal {

result<size_t> serial_read(const handle handle, const std::span<uint8_t> buffer) {
    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    switch (node->type) {
        case handle_type::serial:
            return backend::serial_read(*node, buffer);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<void> serial_write(const handle handle, const std::span<const uint8_t> buffer) {
    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    switch (node->type) {
        case handle_type::serial:
            return backend::serial_write(*node, buffer);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<size_t> serial_transact(const handle handle, const std::span<const uint8_t> write_buffer, const std::span<uint8_t> read_buffer) {
    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    switch (node->type) {
        case handle_type::serial:
            return backend::serial_transact(*node, write_buffer, read_buffer);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

}
