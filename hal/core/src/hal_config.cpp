
#include "hal_actions.h"

#include "global.h"
#include "handles.h"
#include "backend.h"

namespace autobot::hal {

static result<void> verify_valid_params(const handle handle, const config_key key) {
    if (handle < min_handle || handle > max_handle) {
        return error_result(error::invalid_handle);
    }

    if (key > max_config_key) {
        return error_result(error::invalid_config_key);
    }

    return {};
}

static result<void> verify_valid_request(const handle_node* node, const config_key key, const data_type type, const bool write) {
    switch (node->type) {
        case handle_type::port: {
            const auto& def = node->src.port->configs[key];
            if (!def.supported) {
                return error_result(error::unsupported_config);
            }

            if ((def.supported_types & node->src.type) != node->src.type) {
                return error_result(error::unsupported_config);
            }

            if (def.type != type) {
                return error_result(error::invalid_data_type_for_config);
            }

            if ((write && def.permission == data_permission::readonly) ||
                (!write && def.permission == data_permission::writeonly)) {
                return error_result(error::no_permissions_for_config_access);
            }

            break;
        }
    }

    return {};
}

result<uint32_t> config_read_u32(const handle handle, const config_key key) {
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
            return backend::port_config_read_u32(*node, key);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<float> config_read_f32(const handle handle, const config_key key) {
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
            return backend::port_config_read_f32(*node, key);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<void> config_write_u32(const handle handle, const config_key key, const uint32_t value) {
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
            return backend::port_config_write_u32(*node, key, value);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

result<void> config_write_f32(const handle handle, const config_key key, const float value) {
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
            return backend::port_config_write_f32(*node, key, value);
        default:
            return error_result(error::unsupported_operation_for_type);
    }
}

}
