#pragma once

#include "dashboard/object.h"

namespace autobot::dashboard {

class toggle {
public:
    toggle();

    [[nodiscard]] bool get() const;
    void set(bool value);

    void bind_dashboard(dashboard::bind&& bind);

private:
    bool m_state;
    dashboard::bind m_dashboard_bind;
};

}
