#pragma once

#include <cstdint>
#include <limits>
#include <expected>

namespace autobot::hal {

using handle = uint32_t;
using device_id = uint32_t;
using device_type = uint64_t;
using config_key = uint8_t;
using value_key = uint8_t;

static constexpr handle invalid_handle = 0;
static constexpr handle min_handle = 1;
static constexpr handle max_handle = std::numeric_limits<handle>::max();

static constexpr config_key max_config_key = 128;
static constexpr value_key max_value_key = 128;

enum class error {
    device_not_exists = 1,
    type_unsupported_by_device = 2,
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
    device_already_configured = 18,
    device_not_defined = 19,
    device_not_open = 20,
    cannot_configure_while_open = 21
};

template<typename t_>
using result = std::expected<t_, error>;
using error_result = std::unexpected<error>;

enum class data_type : uint8_t {
    unsigned_32bit,
    floating_32bit
};

inline const char* data_type_str(const data_type value) {
    switch (value) {
        case data_type::unsigned_32bit:
            return "unsigned_32bit";
        case data_type::floating_32bit:
            return "floating_32bit";
        default:
            return "";
    }
}

enum class data_permission : uint8_t {
    readonly,
    writeonly,
    readwrite
};

inline const char* data_permission_str(const data_permission value) {
    switch (value) {
        case data_permission::readonly:
            return "readonly";
        case data_permission::writeonly:
            return "writeonly";
        case data_permission::readwrite:
            return "readwrite";
        default:
            return "";
    }
}

enum : device_type {
    port_type_digital_input = 1 << 0,
    port_type_digital_output = 1 << 1,
    port_type_analog_input = 1 << 2,
    port_type_analog_output = 1 << 3,
    port_type_pwm_output = 1 << 4,
    serial_type_i2c = 1 << 5,
    serial_type_spi = 1 << 6,
};

inline const char* port_type_str(const device_type value) {
    switch (value) {
        case port_type_digital_input:
            return "digital_input";
        case port_type_digital_output:
            return "digital_output";
        case port_type_analog_input:
            return "analog_input";
        case port_type_analog_output:
            return "analog_output";
        case port_type_pwm_output:
            return "pwm_output";
        case serial_type_i2c:
            return "i2c";
        case serial_type_spi:
            return "spi";
        default:
            return "";
    }
}

enum : config_key {
    config_digital_poll_edge = 1, // u32, rw, digital_poll_edge
    config_digital_resistor_mode = 2, // u32, rw, digital_resistor_mode
    config_analog_max_value = 3, // u32, r, ADC units
    config_analog_max_voltage = 4, // u32, r, milli-volts
    config_analog_sample_rate = 5, // u32, r, microsecond period
    config_pwm_frequency = 6, // u32, r, microsecond period
};

enum : value_key {
    value_digital_io_signal = 1, // u32, rw/dependent, digital_signal_value
    value_analog_io_signal = 2, // u32, rw/dependent, ADC units
    value_pwm_duty_cycle = 3, // u32, rw/dependent, microsecond period
};

enum digital_poll_edge : uint32_t {
    digital_poll_edge_none = 0,
    digital_poll_edge_rising = 1,
    digital_poll_edge_falling = 2,
    digital_poll_edge_both = 3,
};

enum digital_resistor_mode : uint32_t {
    digital_resistor_none = 0,
    digital_resistor_pullup = 1,
    digital_resistor_pulldown = 2,
};

enum digital_signal_value : uint32_t {
    digital_signal_value_low = 0,
    digital_signal_value_high = 1,
};

}
