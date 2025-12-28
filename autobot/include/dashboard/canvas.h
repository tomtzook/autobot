#pragma once

#include <variant>
#include <map>

#include "units.h"
#include "dashboard/object.h"

namespace autobot::dashboard {

using color = int32_t; // todo: uint

static constexpr color rgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept {
    return (static_cast<color>(r) << 0) | (static_cast<color>(g) << 8) | (static_cast<color>(b) << 16) | (static_cast<color>(a) << 24);
}

class canvas_line {
public:
    canvas_line();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);
    [[nodiscard]] units::meters length() const;
    void length(units::meters value);
    [[nodiscard]] units::radians angle() const;
    void angle(units::radians value);
    [[nodiscard]] color background() const;
    void background(color value);

    void bind_dashboard(bind&& bind);

private:
    units::meters m_x;
    units::meters m_y;
    units::meters m_length;
    units::radians m_angle;
    color m_background;

    bind m_dashboard_bind;
};

class canvas_rect {
public:
    canvas_rect();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);
    [[nodiscard]] units::meters length() const;
    void length(units::meters value);
    [[nodiscard]] units::meters width() const;
    void width(units::meters value);
    [[nodiscard]] color background() const;
    void background(color value);

    void bind_dashboard(bind&& bind);

private:
    units::meters m_x;
    units::meters m_y;
    units::meters m_length;
    units::meters m_width;
    color m_background;

    bind m_dashboard_bind;
};

class canvas_circle {
public:
    canvas_circle();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);
    [[nodiscard]] units::meters radius() const;
    void radius(units::meters value);
    [[nodiscard]] color background() const;
    void background(color value);

    void bind_dashboard(bind&& bind);

private:
    units::meters m_x;
    units::meters m_y;
    units::meters m_radius;
    color m_background;

    bind m_dashboard_bind;
};

class canvas_group {
public:
    canvas_group();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);

    canvas_line& create_line(std::string_view name);
    canvas_rect& create_rect(std::string_view name);
    canvas_circle& create_circle(std::string_view name);

    void bind_dashboard(bind&& bind);

private:
    using shape = std::variant<canvas_line, canvas_rect, canvas_circle>;
    std::map<std::string, shape, std::less<>> m_shapes;

    units::meters m_x;
    units::meters m_y;

    bind m_dashboard_bind;
};

}
