
#include "dashboard/toggle.h"

namespace autobot::dashboard {

toggle::toggle()
    : m_state(false)
{}

bool toggle::get() const {
    return m_state;
}

void toggle::set(const bool value) {
    m_state = value;
}

void toggle::bind_dashboard(dashboard::bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("toggle");
    m_dashboard_bind.set_units("bool");

    m_dashboard_bind.add("state", m_state);
}


}
