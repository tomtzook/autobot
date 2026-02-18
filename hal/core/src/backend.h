#pragma once

#include "handles.h"

namespace autobot::hal::backend {

result<void> create();
result<void> destroy();

result<void> init_device(const handles::handle_node& node);
result<void> free_device(const handles::handle_node& node);

result<uint32_t> config_read_u32(const handles::handle_node& node, config_key key);
result<float> config_read_f32(const handles::handle_node& node, config_key key);
result<void> config_write_u32(const handles::handle_node& node, config_key key, uint32_t value);
result<void> config_write_f32(const handles::handle_node& node, config_key key, float value);

result<uint32_t> value_read_u32(const handles::handle_node& node, value_key key);
result<float> value_read_f32(const handles::handle_node& node, value_key key);
result<void> value_write_u32(const handles::handle_node& node, value_key key, uint32_t value);
result<void> value_write_f32(const handles::handle_node& node, value_key key, float value);

result<size_t> serial_read(const handles::handle_node& node, std::span<uint8_t> buffer);
result<void> serial_write(const handles::handle_node& node, std::span<const uint8_t> buffer);
result<size_t> serial_transact(const handles::handle_node& node, std::span<const uint8_t> write_buffer, std::span<uint8_t> read_buffer);

}
