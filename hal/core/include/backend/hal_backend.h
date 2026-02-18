#pragma once

#include <cstdint>
#include <span>

#include <hal_types.h>

namespace autobot::hal::backend {

struct backend_impl {
    result<void> (*create)();
    result<void> (*destroy)();

    result<void> (*init_device)(device_id, device_type);
    result<void> (*free_device)(device_id, device_type);

    result<uint32_t> (*config_read_u32)(device_id, device_type, config_key);
    result<float> (*config_read_f32)(device_id, device_type, config_key);
    result<void> (*config_write_u32)(device_id, device_type, config_key, uint32_t);
    result<void> (*config_write_f32)(device_id, device_type, config_key, float);

    result<uint32_t> (*value_read_u32)(device_id, device_type, value_key);
    result<float> (*value_read_f32)(device_id, device_type, value_key);
    result<void> (*value_write_u32)(device_id, device_type, value_key, uint32_t);
    result<void> (*value_write_f32)(device_id, device_type, value_key, float);

    result<size_t> (*serial_read)(device_id, device_type, std::span<uint8_t>);
    result<void> (*serial_write)(device_id, device_type, std::span<const uint8_t>);
    result<size_t> (*serial_transact)(device_id, device_type, std::span<const uint8_t>, std::span<uint8_t>);
};

}
