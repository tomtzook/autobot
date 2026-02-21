
#include "dashboard/world.h"

namespace autobot::dashboard {

template<typename t_, typename map_t_>
t_& create_ligament(map_t_& map, bind& dashboard_bind, const std::string_view name) {
    if (const auto it = map.find(name); it != map.end()) {
        throw std::runtime_error("name already used");
    }

    auto [it, added] = map.emplace(name, t_());
    if (!added) {
        throw std::runtime_error("failed to add item to map");
    }

    auto& child = std::get<t_>(it->second);
    dashboard_bind.add_child(name, child);

    return child;
}

d3_box::d3_box()
    : m_x()
    , m_y()
    , m_z()
    , m_pitch()
    , m_yaw()
    , m_roll()
    , m_length()
    , m_width()
    , m_height()
    , m_foreground()
{}

units::meters d3_box::x() const {
    return m_x;
}

void d3_box::x(const units::meters value) {
    m_x = value;
}

units::meters d3_box::y() const {
    return m_y;
}

void d3_box::y(const units::meters value) {
    m_y = value;
}

units::meters d3_box::z() const {
    return m_z;
}

void d3_box::z(const units::meters value) {
    m_z = value;
}

units::radians d3_box::pitch() const {
    return m_pitch;
}

void d3_box::pitch(const units::radians value) {
    m_pitch = value;
}

units::radians d3_box::yaw() const {
    return m_yaw;
}

void d3_box::yaw(const units::radians value) {
    m_yaw = value;
}

units::radians d3_box::roll() const {
    return m_roll;
}

void d3_box::roll(const units::radians value) {
    m_roll = value;
}

units::meters d3_box::length() const {
    return m_length;
}

void d3_box::length(const units::meters value) {
    m_length = value;
}

units::meters d3_box::width() const {
    return m_width;
}

void d3_box::width(const units::meters value) {
    m_width = value;    
}

units::meters d3_box::height() const {
    return m_height;
}

void d3_box::height(const units::meters value) {
    m_height = value;
}

color d3_box::foreground() const {
    return m_foreground;
}

void d3_box::foreground(const color value) {
    m_foreground = value;
}

void d3_box::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("d3_box");
    m_dashboard_bind.set_units("meters,radians");

    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
    m_dashboard_bind.add_readonly("z", m_z);
    m_dashboard_bind.add_readonly("pitch", m_pitch);
    m_dashboard_bind.add_readonly("yaw", m_yaw);
    m_dashboard_bind.add_readonly("roll", m_roll);
    m_dashboard_bind.add_readonly("length", m_length);
    m_dashboard_bind.add_readonly("width", m_width);
    m_dashboard_bind.add_readonly("height", m_height);
    m_dashboard_bind.add_readonly("color", m_foreground);
}

d3_body::d3_body()
    : m_ligaments()
    , m_x()
    , m_y()
    , m_z()
    , m_pitch()
    , m_yaw()
    , m_roll()
{}

units::meters d3_body::x() const {
    return m_x;
}

void d3_body::x(const units::meters value) {
    m_x = value;
}

units::meters d3_body::y() const {
    return m_y;
}

void d3_body::y(const units::meters value) {
    m_y = value;
}

units::meters d3_body::z() const {
    return m_z;
}

void d3_body::z(const units::meters value) {
    m_z = value;
}

units::radians d3_body::pitch() const {
    return m_pitch;
}

void d3_body::pitch(const units::radians value) {
    m_pitch = value;
}

units::radians d3_body::yaw() const {
    return m_yaw;
}

void d3_body::yaw(const units::radians value) {
    m_yaw = value;
}

units::radians d3_body::roll() const {
    return m_roll;
}

void d3_body::roll(const units::radians value) {
    m_roll = value;
}

d3_box& d3_body::create_box(const std::string_view name) {
    return create_ligament<d3_box>(m_ligaments, m_dashboard_bind, name);
}

void d3_body::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("d3_body");
    m_dashboard_bind.set_units("meters,radians");

    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
    m_dashboard_bind.add_readonly("z", m_z);
    m_dashboard_bind.add_readonly("pitch", m_pitch);
    m_dashboard_bind.add_readonly("yaw", m_yaw);
    m_dashboard_bind.add_readonly("roll", m_roll);
}

}
