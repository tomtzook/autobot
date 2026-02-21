#pragma once

#include <variant>
#include <map>

#include "units.h"
#include "dashboard/object.h"
#include "dashboard/color.h"

namespace autobot::dashboard {

class d3_box {
public:
    d3_box();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);
    [[nodiscard]] units::meters z() const;
    void z(units::meters value);
    [[nodiscard]] units::radians pitch() const;
    void pitch(units::radians value);
    [[nodiscard]] units::radians yaw() const;
    void yaw(units::radians value);
    [[nodiscard]] units::radians roll() const;
    void roll(units::radians value);
    [[nodiscard]] units::meters length() const;
    void length(units::meters value);
    [[nodiscard]] units::meters width() const;
    void width(units::meters value);
    [[nodiscard]] units::meters height() const;
    void height(units::meters value);
    [[nodiscard]] color foreground() const;
    void foreground(color value);

    void bind_dashboard(bind&& bind);

private:
    units::meters m_x;
    units::meters m_y;
    units::meters m_z;
    units::radians m_pitch;
    units::radians m_yaw;
    units::radians m_roll;
    units::meters m_length;
    units::meters m_width;
    units::meters m_height;
    color m_foreground;

    bind m_dashboard_bind;
};

class d3_body {
public:
    d3_body();

    [[nodiscard]] units::meters x() const;
    void x(units::meters value);
    [[nodiscard]] units::meters y() const;
    void y(units::meters value);
    [[nodiscard]] units::meters z() const;
    void z(units::meters value);
    [[nodiscard]] units::radians pitch() const;
    void pitch(units::radians value);
    [[nodiscard]] units::radians yaw() const;
    void yaw(units::radians value);
    [[nodiscard]] units::radians roll() const;
    void roll(units::radians value);

    d3_box& create_box(std::string_view name);

    void bind_dashboard(bind&& bind);

private:
    using ligament = std::variant<d3_box>;
    std::map<std::string, ligament, std::less<>> m_ligaments;

    units::meters m_x;
    units::meters m_y;
    units::meters m_z;
    units::radians m_pitch;
    units::radians m_yaw;
    units::radians m_roll;

    bind m_dashboard_bind;
};

}
