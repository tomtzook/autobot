#pragma once

#include <exception>

#include <hal_types.h>

namespace autobot::hal {

class hal_exception : public std::exception {
public:
    explicit hal_exception(const error error)
        : error(error) {}

    [[nodiscard]] const char* what() const noexcept override {
        return "hal exception";
    }

    const error error;
};

class port_not_exists_exception final : public hal_exception {
public:
    port_not_exists_exception()
        : hal_exception(error::port_not_exists)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "requested port does not exist";
    }
};

class type_unsupported_by_port_exception final : public hal_exception {
public:
    type_unsupported_by_port_exception()
        : hal_exception(error::type_unsupported_by_port)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "requested type is not supported for requested port";
    }
};

class no_more_handles_exception final : public hal_exception {
public:
    no_more_handles_exception()
        : hal_exception(error::no_more_handles)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "handle storage is full";
    }
};

class handle_allocation_failure_exception final : public hal_exception {
public:
    handle_allocation_failure_exception()
        : hal_exception(error::handle_allocation_failure)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "handle allocation failed";
    }
};

class no_such_handle_exception final : public hal_exception {
public:
    no_such_handle_exception()
        : hal_exception(error::no_such_handle)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "handle not found";
    }
};

class invalid_handle_exception final : public hal_exception {
public:
    invalid_handle_exception()
        : hal_exception(error::invalid_handle)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "handle is invalid";
    }
};

class invalid_config_key_exception final : public hal_exception {
public:
    invalid_config_key_exception()
        : hal_exception(error::invalid_config_key)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "config key is invalid";
    }
};

class invalid_value_key_exception final : public hal_exception {
public:
    invalid_value_key_exception()
        : hal_exception(error::invalid_value_key)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "value key is invalid";
    }
};

class unsupported_config_exception final : public hal_exception {
public:
    unsupported_config_exception()
        : hal_exception(error::unsupported_config)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "config is unsupported by handle";
    }
};

class unsupported_value_exception final : public hal_exception {
public:
    unsupported_value_exception()
        : hal_exception(error::unsupported_value)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "value is unsupported by handle";
    }
};

class invalid_data_type_for_config_exception final : public hal_exception {
public:
    invalid_data_type_for_config_exception()
        : hal_exception(error::invalid_data_type_for_config)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "data type given is not supported by config";
    }
};

class invalid_data_type_for_value_exception final : public hal_exception {
public:
    invalid_data_type_for_value_exception()
        : hal_exception(error::invalid_data_type_for_value)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "data type given is not supported by value";
    }
};

class no_permissions_for_config_access_exception final : public hal_exception {
public:
    no_permissions_for_config_access_exception()
        : hal_exception(error::no_permissions_for_config_access)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "no permissions for requested access to config";
    }
};

class no_permissions_for_value_access_exception final : public hal_exception {
public:
    no_permissions_for_value_access_exception()
        : hal_exception(error::no_permissions_for_value_access)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "no permissions for requested access to value";
    }
};

class unsupported_operation_for_type_exception final : public hal_exception {
public:
    unsupported_operation_for_type_exception()
        : hal_exception(error::unsupported_operation_for_type)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "handle cannot perform the requested operation";
    }
};

class backend_not_configured_exception final : public hal_exception {
public:
    backend_not_configured_exception()
        : hal_exception(error::backend_not_configured)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "backend was not configured";
    }
};

class backend_does_not_support_operation final : public hal_exception {
public:
    backend_does_not_support_operation()
        : hal_exception(error::backend_does_not_support_operation)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "backend does not support requested operation";
    }
};

class port_already_configured_exception final : public hal_exception {
public:
    port_already_configured_exception()
        : hal_exception(error::port_already_configured)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "port with given ID is already configured and cannot be again";
    }
};

class port_not_defined_exception final : public hal_exception {
public:
    port_not_defined_exception()
        : hal_exception(error::port_not_defined)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "requested port is not defined and cannot be used";
    }
};

class port_not_open_exception final : public hal_exception {
public:
    port_not_open_exception()
        : hal_exception(error::port_not_open)
    {};

    [[nodiscard]] const char* what() const noexcept override {
        return "requested port is not open";
    }
};

template<typename t_>
void result_to_exception(const result<t_>& res) {
    if (res) {
        return;
    }

    switch (res.error()) {
        case error::port_not_exists:
            throw port_not_exists_exception();
        case error::type_unsupported_by_port:
            throw type_unsupported_by_port_exception();
        case error::no_more_handles:
            throw no_more_handles_exception();
        case error::handle_allocation_failure:
            throw handle_allocation_failure_exception();
        case error::no_such_handle:
            throw no_such_handle_exception();
        case error::invalid_handle:
            throw invalid_handle_exception();
        case error::invalid_config_key:
            throw invalid_config_key_exception();
        case error::invalid_value_key:
            throw invalid_value_key_exception();
        case error::unsupported_config:
            throw unsupported_config_exception();
        case error::unsupported_value:
            throw unsupported_value_exception();
        case error::invalid_data_type_for_config:
            throw invalid_data_type_for_config_exception();
        case error::invalid_data_type_for_value:
            throw invalid_data_type_for_value_exception();
        case error::no_permissions_for_config_access:
            throw no_permissions_for_config_access_exception();
        case error::no_permissions_for_value_access:
            throw no_permissions_for_value_access_exception();
        case error::unsupported_operation_for_type:
            throw unsupported_operation_for_type_exception();
        case error::backend_not_configured:
            throw backend_not_configured_exception();
        case error::backend_does_not_support_operation:
            throw backend_does_not_support_operation();
        case error::port_already_configured:
            throw port_already_configured_exception();
        case error::port_not_defined:
            throw port_not_defined_exception();
        case error::port_not_open:
            throw port_not_open_exception();
        default:
            throw hal_exception(res.error());
    }
}

}
