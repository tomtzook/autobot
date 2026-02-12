
#include "hal_actions.h"

#include "global.h"
#include "handles.h"
#include "backend.h"

namespace autobot::hal {

static result<void> verify_valid_params(const handle handle, const value_key key) {
    if (handle < min_handle || handle > max_handle) {
        return error_result(error::invalid_handle);
    }

    if (key > max_value_key) {
        return error_result(error::invalid_value_key);
    }

    return {};
}

static result<void> verify_valid_request(const handle_node* node, const value_key key, const data_type type, const bool write) {
    switch (node->type) {
        case handle_type::port: {
            const auto& def = node->src.port.port->values[key];
            if (!def.supported) {
                return error_result(error::unsupported_value);
            }

            if ((def.supported_types & node->src.port.type) != node->src.port.type) {
                return error_result(error::unsupported_value);
            }

            if (def.type != type) {
                return error_result(error::invalid_data_type_for_value);
            }

            if ((write && def.permission == data_permission::readonly) ||
                (!write && def.permission == data_permission::writeonly)) {
                return error_result(error::no_permissions_for_value_access);
            }

            break;
        }
        case handle_type::serial:
            return error_result(error::unsupported_value);
    }

    return {};
}

result<uint32_t> value_read_u32(const handle handle, const value_key key) {
    if (const auto result = verify_valid_params(handle, key); !result) {
        return error_result(result.error());
    }

    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    if (const auto result = verify_valid_request(node, key, data_type::unsigned_32bit, false); !result) {
        return error_result(result.error());
    }

    switch (node->type) {
        case handle_type::port:
            return backend::port_value_read_u32(*node, key);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<float> value_read_f32(const handle handle, const value_key key) {
    if (const auto result = verify_valid_params(handle, key); !result) {
        return error_result(result.error());
    }

    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    if (const auto result = verify_valid_request(node, key, data_type::floating_32bit, false); !result) {
        return error_result(result.error());
    }

    switch (node->type) {
        case handle_type::port:
            return backend::port_value_read_f32(*node, key);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<void> value_write_u32(const handle handle, const value_key key, const uint32_t value) {
    if (const auto result = verify_valid_params(handle, key); !result) {
        return error_result(result.error());
    }

    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    if (const auto result = verify_valid_request(node, key, data_type::unsigned_32bit, true); !result) {
        return error_result(result.error());
    }

    switch (node->type) {
        case handle_type::port:
            return backend::port_value_write_u32(*node, key, value);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<void> value_write_f32(const handle handle, const value_key key, const float value) {
    if (const auto result = verify_valid_params(handle, key); !result) {
        return error_result(result.error());
    }

    const auto lock = lock_instance();
    const auto opt = lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto& node = opt.value();
    if (const auto result = verify_valid_request(node, key, data_type::floating_32bit, true); !result) {
        return error_result(result.error());
    }

    switch (node->type) {
        case handle_type::port:
            return backend::port_value_write_f32(*node, key, value);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

}
