#pragma once

#include <span>

#include "backend/hal_backend.h"
#include "hal_types.h"

namespace autobot::hal {

result<void> initialize(const backend::backend_impl* impl);

result<handle> open(device_id id, device_type type);
result<void> close(handle handle);

result<handle_query_result> query_handle(handle handle);
result<device_query_result> query_device(device_id id);

result<uint32_t> config_read_u32(handle handle, config_key key);
result<float> config_read_f32(handle handle, config_key key);
result<void> config_write_u32(handle handle, config_key key, uint32_t value);
result<void> config_write_f32(handle handle, config_key key, float value);

result<uint32_t> value_read_u32(handle handle, value_key key);
result<float> value_read_f32(handle handle, value_key key);
result<void> value_write_u32(handle handle, value_key key, uint32_t value);
result<void> value_write_f32(handle handle, value_key key, float value);

result<size_t> serial_read(handle handle, std::span<uint8_t> buffer);
result<void> serial_write(handle handle, std::span<const uint8_t> buffer);
result<size_t> serial_transact(handle handle, std::span<const uint8_t> write_buffer, std::span<uint8_t> read_buffer);

}
