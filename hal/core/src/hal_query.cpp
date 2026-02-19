
#include "global.h"
#include "handles.h"
#include "hal_actions.h"


namespace autobot::hal {

template<typename t_, typename ctx_, size_t data_entry_offset_>
result<std::unique_ptr<ctx_>> iterate_start(const std::list<t_>& lst) {
    struct context_full {
        std::list<t_>::const_iterator iterator;
    };

    constexpr auto alloc_size = sizeof(ctx_) + sizeof(context_full);
    auto ptr = std::unique_ptr<ctx_>(reinterpret_cast<ctx_*>(new uint8_t[alloc_size]));

    auto* ptr_data = reinterpret_cast<context_full*>(reinterpret_cast<uint8_t*>(ptr.get()) + sizeof(ctx_));
    ptr_data->iterator = lst.cbegin();
    if (ptr_data->iterator == lst.cend()) {
        ptr->done = true;
    } else {
        ptr->done = false;
        auto* data_entry = reinterpret_cast<t_*>(reinterpret_cast<uint8_t*>(ptr.get()) + data_entry_offset_);
        *data_entry = static_cast<t_>(*(ptr_data->iterator));
    }

    return std::move(ptr);
}

template<typename t_, typename ctx_, size_t data_entry_offset_>
result<void> iterate_next(const std::list<t_>& lst, const std::unique_ptr<ctx_>& ptr) {
    struct context_full {
        std::list<t_>::const_iterator iterator;
    };

    auto* ptr_data = reinterpret_cast<context_full*>(reinterpret_cast<uint8_t*>(ptr.get()) + sizeof(ctx_));
    ++(ptr_data->iterator);
    if (ptr_data->iterator == lst.cend()) {
        ptr->done = true;
    } else {
        ptr->done = false;
        auto* data_entry = reinterpret_cast<t_*>(reinterpret_cast<uint8_t*>(ptr.get()) + data_entry_offset_);
        *data_entry = static_cast<t_>(*(ptr_data->iterator));
    }

    return {};
}

result<handle_query_result> query_handle(const handle handle) {
    const auto lock = lock_instance();
    const auto opt = handles::lookup_handle(handle);
    if (!opt) {
        return error_result(error::no_such_handle);
    }

    const auto* node = opt.value();
    return handle_query_result{ node->device->id, node->type };
}

result<device_query_result> query_device(const device_id id) {
    const auto lock = lock_instance();
    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto* device = opt.value();
    device_query_result result{};
    result.id = device->id;
    result.supported_types = device->supported_types;

    if (const auto opt_handle = handles::lookup_handle_to(*device)) {
        const auto* handle_node = opt_handle.value();
        result.handle = handle_node->handle;
        result.open_type = handle_node->type;
    } else {
        result.handle = invalid_handle;
        result.open_type = 0;
    }

    return result;
}

result<config_query_result> query_device_config(const device_id id, const config_key key) {
    if (key > max_config_key) {
        return error_result(error::invalid_config_key);
    }

    const auto lock = lock_instance();
    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto* device = opt.value();
    const auto& def = device->configs[key];
    return config_query_result{def.supported, def.supported_types, def.type, def.permission};
}

result<value_query_result> query_device_value(const device_id id, const value_key key) {
    if (key > max_value_key) {
        return error_result(error::invalid_value_key);
    }

    const auto lock = lock_instance();
    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto* device = opt.value();
    const auto& def = device->values[key];
    return value_query_result{def.supported, def.supported_types, def.type, def.permission};
}

result<serial_query_result> query_device_serial(const device_id id) {
    const auto lock = lock_instance();
    const auto opt = lookup_device(id);
    if (!opt) {
        return error_result(error::device_not_exists);
    }

    const auto* device = opt.value();
    const auto& def = device->serial;
    return serial_query_result{def.supported, def.supported_types, def.permission};
}

result<std::unique_ptr<handle_iterator_context>> iterate_handles() {
    const auto lock = lock_instance();
    const auto& data = get_global_data();

    return iterate_start<handle, handle_iterator_context, offsetof(handle_iterator_context, handle)>(data.ordered_handles);
}

result<void> iterate_handles_next(const std::unique_ptr<handle_iterator_context>& ptr) {
    const auto lock = lock_instance();
    const auto& data = get_global_data();

    return iterate_next<handle, handle_iterator_context, offsetof(handle_iterator_context, handle)>(data.ordered_handles, ptr);
}

result<std::unique_ptr<device_iterator_context>> iterate_devices() {
    const auto lock = lock_instance();
    const auto& data = get_global_data();

    return iterate_start<device_id, device_iterator_context, offsetof(device_iterator_context, id)>(data.ordered_devices);
}

result<void> iterate_devices_next(const std::unique_ptr<device_iterator_context>& ptr) {
    const auto lock = lock_instance();
    const auto& data = get_global_data();

    return iterate_next<device_id, device_iterator_context, offsetof(device_iterator_context, id)>(data.ordered_devices, ptr);
}

}
