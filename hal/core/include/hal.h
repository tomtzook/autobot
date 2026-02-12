#pragma once

#include <functional>
#include <hal_types.h>
#include <hal_actions.h>

namespace autobot::hal {

void initialize(std::function<void(backend::backend_impl*)>&& initializer);

struct base_device {
public:
    explicit base_device(handle handle);
    base_device(const base_device&) = delete;
    base_device(base_device&&) noexcept;
    ~base_device();

    base_device& operator=(const base_device&) = delete;
    base_device& operator=(base_device&&) noexcept;

    [[nodiscard]] explicit operator bool() const;
    bool operator==(const base_device& other) const;
    bool operator!=(const base_device& other) const;

    [[nodiscard]] bool is_open() const;

    uint32_t read_config_u32(config_key key);
    float read_config_f32(config_key key);
    void write_config_u32(config_key key, uint32_t value);
    void write_config_f32(config_key key, float value);

    uint32_t read_value_u32(value_key key);
    float read_value_f32(value_key key);
    void write_value_u32(value_key key, uint32_t value);
    void write_value_f32(value_key key, float value);

    void close();

private:
    hal::handle m_handle;
};

struct digital_port : base_device {

};

struct analog_port : base_device {

};

struct pwm_port : base_device {

};

}

