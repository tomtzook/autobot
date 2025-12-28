
#include "dashboard/canvas.h"


namespace autobot::dashboard {

canvas_line::canvas_line()
    : m_x(0)
    , m_y(0)
    , m_length(0)
    , m_angle(0)
    , m_background(0)
{}

units::meters canvas_line::x() const {
    return m_x;
}

void canvas_line::x(const units::meters value) {
    m_x = value;
}

units::meters canvas_line::y() const {
    return m_y;
}

void canvas_line::y(const units::meters value) {
    m_y = value;
}

units::meters canvas_line::length() const {
    return m_length;
}

void canvas_line::length(const units::meters value) {
    m_length = value;
}

units::radians canvas_line::angle() const {
    return m_angle;
}

void canvas_line::angle(const units::radians value) {
    m_angle = value;
}

color canvas_line::background() const {
    return m_background;
}

void canvas_line::background(const color value) {
    m_background = value;
}

void canvas_line::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("canvas_line");
    m_dashboard_bind.set_units("meters");

    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
    m_dashboard_bind.add_readonly("length", m_length);
    m_dashboard_bind.add_readonly("angle", m_angle);
    m_dashboard_bind.add_readonly("color", m_background);
}

canvas_rect::canvas_rect()
    : m_x(0)
    , m_y(0)
    , m_length(0)
    , m_width(0)
    , m_background(0)
{}

units::meters canvas_rect::x() const {
    return m_x;
}

void canvas_rect::x(const units::meters value) {
    m_x = value;
}

units::meters canvas_rect::y() const {
    return m_y;
}

void canvas_rect::y(const units::meters value) {
    m_y = value;
}

units::meters canvas_rect::length() const {
    return m_length;
}

void canvas_rect::length(const units::meters value) {
    m_length = value;
}

units::meters canvas_rect::width() const {
    return m_length;
}

void canvas_rect::width(const units::meters value) {
    m_width = value;
}

color canvas_rect::background() const {
    return m_background;
}

void canvas_rect::background(const color value) {
    m_background = value;
}

void canvas_rect::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("canvas_rect");
    m_dashboard_bind.set_units("meters");

    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
    m_dashboard_bind.add_readonly("length", m_length);
    m_dashboard_bind.add_readonly("width", m_width);
    m_dashboard_bind.add_readonly("color", m_background);
}

template<typename t_, typename map_t_>
t_& create_shape(map_t_& map, bind& dashboard_bind, const std::string_view name) {
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

canvas_circle::canvas_circle()
    : m_x(0)
    , m_y(0)
    , m_radius(0)
    , m_background(0)
{}

units::meters canvas_circle::x() const {
    return m_x;
}

void canvas_circle::x(const units::meters value) {
    m_x = value;
}

units::meters canvas_circle::y() const {
    return m_y;
}

void canvas_circle::y(const units::meters value) {
    m_y = value;
}

units::meters canvas_circle::radius() const {
    return m_radius;
}

void canvas_circle::radius(const units::meters value) {
    m_radius = value;
}

color canvas_circle::background() const {
    return m_background;
}

void canvas_circle::background(const color value) {
    m_background = value;
}

void canvas_circle::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("canvas_circle");
    m_dashboard_bind.set_units("meters");

    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
    m_dashboard_bind.add_readonly("radius", m_radius);
    m_dashboard_bind.add_readonly("color", m_background);
}

canvas_group::canvas_group()
    : m_shapes()
    , m_x(0)
    , m_y(0)
{}

units::meters canvas_group::x() const {
    return m_x;
}

void canvas_group::x(const units::meters value) {
    m_x = value;
}

units::meters canvas_group::y() const {
    return m_y;
}

void canvas_group::y(const units::meters value) {
    m_y = value;
}

canvas_line& canvas_group::create_line(const std::string_view name) {
    return create_shape<canvas_line>(m_shapes, m_dashboard_bind, name);
}

canvas_rect& canvas_group::create_rect(const std::string_view name) {
    return create_shape<canvas_rect>(m_shapes, m_dashboard_bind, name);
}

canvas_circle& canvas_group::create_circle(const std::string_view name) {
    return create_shape<canvas_circle>(m_shapes, m_dashboard_bind, name);
}

void canvas_group::bind_dashboard(bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("canvas_group");
    m_dashboard_bind.add_readonly("x", m_x);
    m_dashboard_bind.add_readonly("y", m_y);
}

}
