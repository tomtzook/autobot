#pragma once

#include <functional>
#include <hal_types.h>
#include <hal_actions.h>

namespace autobot::hal {

void initialize(std::function<void(backend::backend_impl*)>&& initializer);

struct base_device {
    base_device();
    explicit base_device(handle handle);
    base_device(const base_device&) = delete;
    base_device(base_device&&) noexcept;
    virtual ~base_device();

    base_device& operator=(const base_device&) = delete;
    base_device& operator=(base_device&&) noexcept;

    [[nodiscard]] explicit operator bool() const;
    bool operator==(const base_device& other) const;
    bool operator!=(const base_device& other) const;

    [[nodiscard]] bool is_open() const;
    [[nodiscard]] inline handle underlying_handle() const { return m_handle;}
    [[nodiscard]] device_id id() const;
    [[nodiscard]] device_type type() const;

    void close();

private:
    hal::handle m_handle;
};

struct device : base_device {
    device() = default;
    explicit device(handle handle);
    device(device_id id, device_type type);

    [[nodiscard]] device_query_result query() const;
    [[nodiscard]] config_query_result query_config(config_key key) const;
    [[nodiscard]] value_query_result query_value(value_key key) const;
    [[nodiscard]] serial_query_result query_serial() const;

    [[nodiscard]] uint32_t read_config_u32(config_key key) const;
    [[nodiscard]] float read_config_f32(config_key key) const;
    void write_config_u32(config_key key, uint32_t value);
    void write_config_f32(config_key key, float value);

    [[nodiscard]] uint32_t read_value_u32(value_key key) const;
    [[nodiscard]] float read_value_f32(value_key key) const;
    void write_value_u32(value_key key, uint32_t value);
    void write_value_f32(value_key key, float value);

    size_t read(std::span<uint8_t> buffer);
    void write(std::span<const uint8_t> buffer);
    size_t transact(std::span<const uint8_t> write_buffer, std::span<uint8_t> read_buffer);
};

struct digital_port final : device {
    digital_port() = default;
    explicit digital_port(handle handle);
    digital_port(device_id id, bool output);

    [[nodiscard]] digital_poll_edge poll_edge() const;
    void poll_edge(digital_poll_edge value);
    [[nodiscard]] digital_resistor_mode resistor_mode() const;
    void resistor_mode(digital_resistor_mode value);

    [[nodiscard]] digital_signal_value read() const;
    void write(digital_signal_value value);
};

}

