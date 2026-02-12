#pragma once

#include <cstdint>
#include <span>

#include <hal_types.h>

namespace autobot::hal::backend {

struct backend_impl {
    result<void> (*create)();
    result<void> (*destroy)();

    result<void> (*port_new)(port_id, port_type);
    result<void> (*port_delete)(port_id, port_type);

    result<uint32_t> (*port_config_read_u32)(port_id, port_type, config_key);
    result<float> (*port_config_read_f32)(port_id, port_type, config_key);
    result<void> (*port_config_write_u32)(port_id, port_type, config_key, uint32_t);
    result<void> (*port_config_write_f32)(port_id, port_type, config_key, float);

    result<uint32_t> (*port_value_read_u32)(port_id, port_type, value_key);
    result<float> (*port_value_read_f32)(port_id, port_type, value_key);
    result<void> (*port_value_write_u32)(port_id, port_type, value_key, uint32_t);
    result<void> (*port_value_write_f32)(port_id, port_type, value_key, float);

    result<void> (*serial_new)(serial_id, serial_type);
    result<void> (*serial_delete)(serial_id, serial_type);

    result<uint32_t> (*serial_config_read_u32)(serial_id, serial_type, config_key);
    result<float> (*serial_config_read_f32)(serial_id, serial_type, config_key);
    result<void> (*serial_config_write_u32)(serial_id, serial_type, config_key, uint32_t);
    result<void> (*serial_config_write_f32)(serial_id, serial_type, config_key, float);

    result<size_t> (*serial_read)(serial_id, serial_type, std::span<uint8_t>);
    result<void> (*serial_write)(serial_id, serial_type, std::span<const uint8_t>);
    result<size_t> (*serial_transact)(serial_id, serial_type, std::span<const uint8_t>, std::span<uint8_t>);
};

}
