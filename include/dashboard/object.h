#pragma once

#include <string_view>
#include <functional>
#include <map>
#include <vector>
#include <optional>

#include <obsr.h>

#include "units.h"

namespace autobot::dashboard {

class registry;
class bind;

template<class t_>
concept displayable = requires(t_ t, bind&& f1_bind) {
    {t.bind_dashboard(std::move(f1_bind))} -> std::same_as<void>;
};

class entry {
public:
    using getter_func = std::function<std::optional<obsr::value>(obsr::value&)>;

    entry();
    explicit entry(obsr::object object);

    entry(const entry&) = default;
    entry(entry&&) = default;
    entry& operator=(const entry&) = default;
    entry& operator=(entry&&) = default;

    bool operator==(const entry&) const;

    void set(std::string_view name, const obsr::value& value);
    void add(std::string_view name, getter_func&& getter);
    void refresh();
    void delete_object();

private:
    struct node {
        node();
        node(obsr::entry entry, getter_func&& getter);

        obsr::entry entry;
        obsr::value last_value;

        getter_func getter;
    };

    obsr::object m_object;
    std::map<std::string, node, std::less<>> m_nodes;

    friend class entry_ref;
};

class entry_ref {
public:
    entry_ref();
    entry_ref(registry* registry, entry* entry);
    ~entry_ref();

    entry_ref(const entry_ref&) = delete;
    entry_ref(entry_ref&&) noexcept;
    entry_ref& operator=(const entry_ref&) = delete;
    entry_ref& operator=(entry_ref&&) noexcept;

    template<displayable t_>
    void add_child(std::string_view name, t_& t);

    void set_bool(std::string_view name, bool value);
    void set_int32(std::string_view name, int32_t value);
    void set_float(std::string_view name, float value);
    void set_double(std::string_view name, double value);
    void set_string(std::string_view name, std::string_view value);

    void add_bool(std::string_view name, std::function<bool()>&& getter);
    void add_int32(std::string_view name, std::function<int32_t()>&& getter);
    void add_float(std::string_view name, std::function<float()>&& getter);
    void add_double(std::string_view name, std::function<double()>&& getter);

private:
    registry* m_registry;
    entry* m_entry;
};

class registry {
public:
    registry();

    registry(const registry&) = delete;
    registry(registry&&) = delete;
    registry& operator=(const registry&) = delete;
    registry& operator=(registry&&) = delete;

    template<displayable t_>
    void add(std::string_view name, t_& t);

    void update();

private:
    template<displayable t_>
    void add(obsr::object object, t_& t);

    bind create_bind(obsr::object object);
    void remove_ref(const entry& entry);

    obsr::object m_root;
    std::vector<entry> m_entries;

    friend class entry_ref;
};

class bind {
public:
    bind();
    explicit bind(entry_ref&& entry);
    ~bind() = default;

    bind(const bind&) = delete;
    bind(bind&&) = default;
    bind& operator=(const bind&) = delete;
    bind& operator=(bind&&) = default;

    template<displayable t_>
    void add_child(std::string_view name, t_& t);

    template<typename t_>
    void set(std::string_view name, const t_& value) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_>;
    void set(std::string_view name, std::string_view value);

    template<typename t_>
    void add(std::string_view name, const t_& ref) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_>;
    template<typename t_>
    void add_func(std::string_view name, std::function<t_()>&& getter) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_>;

private:
    entry_ref m_entry;
};

registry& get_registry();

template<displayable t_>
void entry_ref::add_child(const std::string_view name, t_& t) {
    const auto parent_object = m_entry->m_object;
    const auto object = obsr::get_child(parent_object, name);
    m_registry->add(object, t);
}

template<displayable t_>
void registry::add(const std::string_view name, t_& t) {
    const auto object = obsr::get_child(m_root, name);
    add(object, t);
}

template<displayable t_>
void registry::add(const obsr::object object, t_& t) {
    auto bind = create_bind(object);
    t.bind_dashboard(std::move(bind));
}

template<displayable t_>
void bind::add_child(const std::string_view name, t_& t) {
    m_entry.add_child(name, t);
}

template<typename t_>
void bind::set(const std::string_view name, const t_& value) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_> {
    if constexpr (std::is_same_v<t_, bool>) {
        m_entry.set_bool(name, value);
    } else if constexpr (std::is_same_v<t_, int32_t>) {
        m_entry.set_int32(name, value);
    } else if constexpr (std::is_same_v<t_, float>) {
        m_entry.set_float(name, value);
    } else if constexpr (std::is_same_v<t_, double>) {
        m_entry.set_double(name, value);
    } else if constexpr (units::measure_type<t_>) {
        using inner_type = t_::type;
        if constexpr (std::is_same_v<inner_type, float>) {
            m_entry.set_float(name, value.value());
        } else if constexpr (std::is_same_v<inner_type, double>) {
            m_entry.set_double(name, value.value());
        } else {
            static_assert(false, "unsupported dashboard type");
        }
    } else {
        static_assert(false, "unsupported dashboard type");
    }
}

inline void bind::set(const std::string_view name, const std::string_view value) {
    m_entry.set_string(name, value);
}

template<typename t_>
void bind::add(const std::string_view name, const t_& ref) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_> {
    if constexpr (std::is_same_v<t_, bool>) {
        m_entry.add_bool(name, [&ref]()->auto { return ref; });
    } else if constexpr (std::is_same_v<t_, int32_t>) {
        m_entry.add_int32(name, [&ref]()->auto { return ref; });
    } else if constexpr (std::is_same_v<t_, float>) {
        m_entry.add_float(name, [&ref]()->auto { return ref; });
    } else if constexpr (std::is_same_v<t_, double>) {
        m_entry.add_double(name, [&ref]()->auto { return ref; });
    } else if constexpr (units::measure_type<t_>) {
        using inner_type = t_::type;
        if constexpr (std::is_same_v<inner_type, float>) {
            m_entry.add_float(name, [&ref]()->auto { return ref.value(); });
        } else if constexpr (std::is_same_v<inner_type, double>) {
            m_entry.add_double(name, [&ref]()->auto { return ref.value(); });
        } else {
            static_assert(false, "unsupported dashboard type");
        }
    } else {
        static_assert(false, "unsupported dashboard type");
    }
}

template<typename t_>
void bind::add_func(const std::string_view name, std::function<t_()>&& getter) requires math::numeric<t_> || std::is_same_v<t_, bool> || units::measure_type<t_> {
    if constexpr (std::is_same_v<t_, bool>) {
        m_entry.add_bool(name, std::move(getter));
    } else if constexpr (std::is_same_v<t_, int32_t>) {
        m_entry.add_int32(name, std::move(getter));
    } else if constexpr (std::is_same_v<t_, float>) {
        m_entry.add_float(name, std::move(getter));
    } else if constexpr (std::is_same_v<t_, double>) {
        m_entry.add_double(name, std::move(getter));
    } else if constexpr (units::measure_type<t_>) {
        using inner_type = t_::type;
        if constexpr (std::is_same_v<inner_type, float>) {
            m_entry.add_float(name, [getter]()->auto { return getter().value(); });
        } else if constexpr (std::is_same_v<inner_type, double>) {
            m_entry.add_double(name, [getter]()->auto { return getter().value(); });
        } else {
            static_assert(false, "unsupported dashboard type");
        }
    } else {
        static_assert(false, "unsupported dashboard type");
    }
}

}
