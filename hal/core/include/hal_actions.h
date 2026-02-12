#pragma once

#include "backend/hal_backend.h"
#include "hal_types.h"

namespace autobot::hal {

result<void> initialize(const backend::backend_impl* impl);

result<handle> open_port(port_id id, port_type type);
result<void> close(handle handle);

result<uint32_t> config_read_u32(handle handle, config_key key);
result<float> config_read_f32(handle handle, config_key key);
result<void> config_write_u32(handle handle, config_key key, uint32_t value);
result<void> config_write_f32(handle handle, config_key key, float value);

result<uint32_t> value_read_u32(handle handle, value_key key);
result<float> value_read_f32(handle handle, value_key key);
result<void> value_write_u32(handle handle, value_key key, uint32_t value);
result<void> value_write_f32(handle handle, value_key key, float value);

}
