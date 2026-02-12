#pragma once

#include "handles.h"

namespace autobot::hal::backend {

result<void> create();
result<void> destroy();

result<void> port_new(const handle_node& node);
result<void> port_delete(const handle_node& node);

result<uint32_t> port_config_read_u32(const handle_node& node, config_key key);
result<float> port_config_read_f32(const handle_node& node, config_key key);
result<void> port_config_write_u32(const handle_node& node, config_key key, uint32_t value);
result<void> port_config_write_f32(const handle_node& node, config_key key, float value);

result<uint32_t> port_value_read_u32(const handle_node& node, value_key key);
result<float> port_value_read_f32(const handle_node& node, value_key key);
result<void> port_value_write_u32(const handle_node& node, value_key key, uint32_t value);
result<void> port_value_write_f32(const handle_node& node, value_key key, float value);

result<void> serial_new(const handle_node& node);
result<void> serial_delete(const handle_node& node);

result<uint32_t> serial_config_read_u32(const handle_node& node, config_key key);
result<float> serial_config_read_f32(const handle_node& node, config_key key);
result<void> serial_config_write_u32(const handle_node& node, config_key key, uint32_t value);
result<void> serial_config_write_f32(const handle_node& node, config_key key, float value);

result<size_t> serial_read(const handle_node& node, std::span<uint8_t> buffer);
result<void> serial_write(const handle_node& node, std::span<const uint8_t> buffer);
result<size_t> serial_transact(const handle_node& node, std::span<const uint8_t> write_buffer, std::span<uint8_t> read_buffer);

}
