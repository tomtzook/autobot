#pragma once

#include <cstdint>
#include <limits>
#include <expected>

namespace autobot::hal {

using handle = uint32_t;
using port_id = uint8_t;
using port_type = uint8_t;
using config_key = uint16_t;
using value_key = uint16_t;

static constexpr handle invalid_handle = 0;
static constexpr handle min_handle = 1;
static constexpr handle max_handle = std::numeric_limits<handle>::max();

static constexpr port_id max_port_id = 128;
static constexpr config_key max_config_key = 128;
static constexpr value_key max_value_key = 128;

enum class error {
    port_not_exists = 1,
    type_unsupported_by_port = 2,
    no_more_handles = 3,
    handle_allocation_failure = 4,
    no_such_handle = 5,
    invalid_handle = 6,
    invalid_config_key = 7,
    invalid_value_key = 8,
    unsupported_config = 9,
    unsupported_value = 10,
    invalid_data_type_for_config = 11,
    invalid_data_type_for_value = 12,
    no_permissions_for_config_access = 13,
    no_permissions_for_value_access = 14,
    unsupported_operation_for_type = 15,
    backend_not_configured = 16,
    backend_does_not_support_operation = 17,
    port_already_configured = 18,
    port_not_defined = 19,
    port_not_open = 20
};

template<typename t_>
using result = std::expected<t_, error>;
using error_result = std::unexpected<error>;

enum class data_type : uint8_t {
    unsigned_32bit,
    floating_32bit
};

enum class data_permission : uint8_t {
    readonly,
    writeonly,
    readwrite
};

enum : port_type {
    port_type_digital_input = 1 << 0,
    port_type_digital_output = 1 << 1,
    port_type_analog_input = 1 << 2,
    port_type_analog_output = 1 << 3,
    port_type_pwm_output = 1 << 4,
};

enum : value_key {
    value_digital_io_signal = 1, // u32, rw/dependent
    value_analog_io_signal = 2, // u32, rw/dependent
    value_pwm_duty_cycle = 3, // float, rw/dependent
};

}
